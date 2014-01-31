(**************************************************************)
(* LEAVE.ML : leave protocol *)
(* Author: Mark Hayden, 12/95 *)
(**************************************************************)
open View
open Event
open Layer
open Trans
open Util
(**************************************************************)
let name = Trace.filel "LEAVE"
let failwith = Trace.make_failwith name
(**************************************************************)

type header = 
| Leave 
| GotView of bool Arrayf.t

type state = {
  mutable exited : bool ;
  mutable leaving : bool ;
  mutable got_view : bool Arrayf.t ;
  mutable failed : bool Arrayf.t ;
  mutable coord : rank
}

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "exited=%b leaving=%b\n" s.exited s.leaving ;
  sprintf "got_view=%s\n" (Arrayf.bool_to_string s.got_view)
|])

let init state (ls,vs) = {
  exited      = false ;
  got_view    = ls.falses ;
  failed      = ls.falses ;
  leaving     = false ;
  coord       = vs.View.coord
}

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
(*let ack = make_acker name dnnm in*)

  let do_gossip () =
    if Arrayf.get s.got_view ls.rank 
    && ls.rank <> s.coord 
    then (
      dnlm (sendPeer name s.coord) (GotView s.got_view)
    )
  in

  let check_exit () =
    if not s.exited 
    && Arrayf.get s.got_view ls.rank 
    && Arrayf.for_all2 (||) s.got_view s.failed
    then (
      s.exited <- true ;
      
      (* If I am the coordinator, then tell the others
       * that we're done.  Be sure to broadcast the
       * GotView before doing the exit (which will
       * disable the transport).  
       *)
      if ls.rank = s.coord then (
	dnlm (castEv name) (GotView s.got_view)
      ) ;

      dnnm (create name EExit[]) ;
    )
  in

  let up_hdlr ev abv () = up ev abv
  and uplm_hdlr ev hdr = match getType ev,hdr with
  | (ECast iovl | ECastUnrel iovl), Leave ->
      let origin = getPeer ev in 
      if origin <> ls.rank then
	dnnm (suspectReason name (Arrayf.of_ranks ls.nmembers [origin]) name) ;
      Iovecl.free iovl

    (* Watch out!  Be careful about these GotView messages.
     * Because we may get them from leaving members, in
     * which case they should not have set things they
     * shouldn't have.
     *)
  | (ECast iovl | ECastUnrel iovl |ESend iovl |ESendUnrel iovl), GotView got_view ->
      if Arrayf.get got_view  ls.rank
      && not (Arrayf.get s.got_view ls.rank) then
	failwith sanity ;
	  
      s.got_view <- Arrayf.map2 (||) s.got_view got_view ;
      check_exit () ;
      Iovecl.free iovl

  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | EView ->
      (* Only pass up views if we are not leaving.
       *)
      if not s.leaving then
      	upnm ev ;

      (* Check for exiting.
       *)
      s.got_view <- Arrayf.fset s.got_view ls.rank true ;
      do_gossip () ;
      check_exit ()

  | EFail ->
      s.failed <- getFailures ev ;

      (* Hack!  Recompute the rank of the coordinator. 
       *)
      s.coord <- Arrayf.min_false s.failed ;

      do_gossip () ;
      check_exit () ;
      upnm ev

  | ELeave ->
      (* Cast a message to the group.
       *)
      dnlm (castEv name) Leave ;

      (* Mark me as leaving.
       *)
      s.leaving <- true ;

      let suspects = Arrayf.init ls.nmembers (fun i -> i <> ls.rank) in
      dnnm (suspectReason name suspects "leaving") ;
      
      upnm ev

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv ()
  and dnnm_hdlr ev = match getType ev with

    (* This should not happen.  Don't exit twice, and
     * remember if someone else generates an exit.  
     *)
  | EExit ->
      if s.exited then (
	free name ev
      ) else (
	s.exited <- true ;
	dnnm ev
      )

    (* Why is this necessary?
     *)
  | EBlockOk ->
      if s.exited then
        free name ev
      else
        dnnm ev

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
