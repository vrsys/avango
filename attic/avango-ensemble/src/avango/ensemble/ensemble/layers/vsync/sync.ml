(**************************************************************)
(* SYNC.ML : view synchronization protocol *)
(* Author: Mark Hayden, 12/95 *)
(* Manages group synchronization during view changes. *)
(**************************************************************)
(* BUG: the comments in this file are out of date *)
(**************************************************************)
(* TODO:

 * change computation of coord.

 *)
(**************************************************************)
open Util
open Event
open Layer
open Trans
open View
(**************************************************************)
let name = Trace.filel "SYNC"
(**************************************************************)

type header =
  | Block
  | BlockOk of bool Arrayf.t

type state = {
  mutable failed          : bool Arrayf.t ;
  mutable gossip          : rank Arrayf.t ;
  mutable coord           : rank ;	(* who do I think is coord? hack *)
  mutable dn_block        : bool ;	(* have I passed down a EBlock? *)
  mutable req_up_block_ok : bool ;	(* have I got a EBlock from above? *)
  mutable up_block_ok     : bool ;	(* have I passed up an EBlockOk? *)
  block_ok	          : bool array
}

(**************************************************************)

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "req_up_block_ok=%b rank=%d\n" s.req_up_block_ok ls.rank ;
  sprintf "dn_block=%b up_block_ok=%b\n" s.dn_block s.up_block_ok ;
  sprintf "failed  =%s\n" (Arrayf.bool_to_string s.failed) ;
  sprintf "block_ok=%s\n" (string_of_bool_array s.block_ok)
|])

(**************************************************************)

let init _ (ls,vs) = {
  req_up_block_ok = false ;
  coord         = 0 ;
  failed        = ls.falses ;
  gossip        = Arrayf.gossip ls.falses ls.rank ;
  dn_block    	= false ;
  up_block_ok	= false ;
  block_ok    	= Array.create ls.nmembers false
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
(*let ack = make_acker name dnnm in*)
  let log = Trace.log2 name ls.name in
  let logs = Trace.log3 Layer.syncing ls.name name in
  let logb = Trace.logl3 Layer.block ls.name name in

  let do_gossip () =
    let hdr = BlockOk(Arrayf.of_array s.block_ok) in
    if ls.rank <> s.coord then (
      logs (fun () -> "sending BlockOk to coordinator") ;
      dnlm (sendPeer name s.coord) hdr
    )
(*
    dnlm (castEv name) hdr
*)
  in

  let check_ok () =
    if s.req_up_block_ok 
    && not s.up_block_ok
    && Arrayf.for_all2 (||) (Arrayf.of_array s.block_ok) s.failed
    then (
      logs (fun () -> sprintf "releasing EBlockOk") ;
      s.up_block_ok <- true ;
      upnm (create name EBlockOk[]) ;
    )
  in

  let up_hdlr ev abv () = up ev abv

  and uplm_hdlr ev hdr = match getType ev,hdr with

  (* Block: cast from coordinator. 
   *)
  | (ECast iovl|ESend iovl), Block ->
      if not s.dn_block then (
	s.dn_block <- true ;
	dnnm (create name EBlock[])
      ) ;
      Iovecl.free iovl

  (* BlockOk: Got block Ok from other members, mark him
   * as OK and check whether we're done blocking.
   *)
  | (ECast iovl|ESend iovl), BlockOk(block_ok) ->
      for i = 0 to pred (Array.length s.block_ok) do
	s.block_ok.(i) <- s.block_ok.(i) || Arrayf.get block_ok i
      done ;
      check_ok () ;
      Iovecl.free iovl

  | _ -> failwith bad_up_event

  and upnm_hdlr ev = match getType ev with

  (* EFail: Mark some members as being failed.  Check if
   * we're done blocking.
   *)
  | EFail ->
      s.failed <- getFailures ev ;

      (* Hack!  Recompute the rank of the coordinator. 
       *)
      for i = pred ls.nmembers downto 0 do
	if not (Arrayf.get s.failed i) then
      	  s.coord <- i
      done ;
      s.gossip <- Arrayf.gossip s.failed ls.rank ;
      do_gossip () ;
      upnm ev ;
      check_ok ()

  (* EBlockOk: Collect the EBlockOk.
   *)
  | EBlockOk -> 
      logs (fun () -> sprintf "capturing EBlockOk") ;
      s.block_ok.(ls.rank) <- true ;
      do_gossip () ;
      check_ok () ;
      free name ev ;

  | EAccount ->
      logb (fun () -> [
  	sprintf "req_up_block_ok=%b" s.req_up_block_ok ;
  	sprintf "dn_block=%b up_block_ok=%b" s.dn_block s.up_block_ok ;
  	sprintf "failed  =%s" (Arrayf.bool_to_string s.failed) ;
  	sprintf "block_ok=%s" (string_of_bool_array s.block_ok)
      ]) ;
      upnm ev

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv ()

  and dnnm_hdlr ev = match getType ev with

  (* Layer above has started blocking group.  If blocking
   * is already done then deliver EBlockOk.  
   *)
  | EBlock ->
      if s.req_up_block_ok then failwith "2nd EBlock" ;
      s.req_up_block_ok <- true ;
      if not s.dn_block then (
	logs (fun () -> "broadcasting Block") ;
        s.dn_block <- true ;
	dnlm (create name (ECast Iovecl.empty) [NoTotal]) Block ;
        dnnm ev
      ) else (
      	free name ev
      )

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
