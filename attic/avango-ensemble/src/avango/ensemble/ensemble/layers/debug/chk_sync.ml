(**************************************************************)
(* CHK_SYNC.ML *)
(* Author: Mark Hayden, 6/95 *)
(* Tests that all members saw the same number of broadcasts
 * messages in each view, and that all pt2pt sends were
 * received. *)
(**************************************************************)
open Trans
open Layer
open Event
open Util
open View
(**************************************************************)
let name = Trace.filel "CHK_SYNC"
(**************************************************************)

type header = NoHdr 
  | Appl
  | Gossip of View.id * seqno Arrayf.t * seqno Arrayf.t
  | ViewState of View.state

type state = {
    mutable failed : bool Arrayf.t ;
    mutable cast_xmit : seqno ;
    cast_recv : seqno array ;
    send_xmit : seqno array ;
    send_recv : seqno array ;
    got_dn_block : Once.t ;
    got_view : Once.t
  } 
      
let init _ (ls,vs) = {
  failed = ls.falses ;
  cast_xmit = 0 ;
  cast_recv = Array.create ls.nmembers 0 ;
  send_xmit = Array.create ls.nmembers 0 ;
  send_recv = Array.create ls.nmembers 0 ;
  got_view = Once.create "CHK_SYNC:got_view" ;
  got_dn_block = Once.create "CHK_SYNC:got_dn_block"
}

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "failed=%s\n" (Arrayf.bool_to_string s.failed) ;
  sprintf "cast_xmit=%d\n" s.cast_xmit ;
  sprintf "cast_recv=%s\n" (string_of_int_array s.cast_recv) ;
  sprintf "send_xmit=%s\n" (string_of_int_array s.send_xmit) ;
  sprintf "send_recv=%s\n" (string_of_int_array s.send_recv)
|])
 
let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in
(*let ack = make_acker name dnnm in*)

  let chk_recv = ref None in
  let chk_rmt = ref [] in
  let chk_sync () =
    match !chk_recv with
    | Some(chk_vs,chk_cast,chk_send_recv) ->
      	List.iter (fun (rmt_rank,rmt_vs,rmt_cast,rmt_send_xmit) ->
	  if rmt_vs = chk_vs then (
      	    log (fun () -> sprintf "verifying:%d->%d" ls.rank rmt_rank) ;
	    if rmt_cast <> chk_cast 
	    || rmt_send_xmit <> Arrayf.get chk_send_recv rmt_rank
	    then (
	      eprintf "CHK_SYNC:virtual synchrony failure{%s}\n" ls.name ;
	      eprintf "  rank=%d rmt_rank=%d\n" ls.rank rmt_rank ;
	      eprintf "  view_id=%s\n" (View.string_of_id ls.view_id) ;
	      eprintf "   my failed=%s\n" (Arrayf.bool_to_string s.failed) ;
	      eprintf "     my cast=%s\n" (Arrayf.int_to_string chk_cast) ;
	      eprintf "    rmt cast=%s\n" (Arrayf.int_to_string rmt_cast) ;
	      eprintf "    rmt_send_xmt=%d my_send_xmit=%d\n"  
	      	rmt_send_xmit (Arrayf.get chk_send_recv rmt_rank) ;
	      dnnm (create name EDump[])
	    )
	  )
	) !chk_rmt ;
      	chk_rmt := []
    | _ -> ()
  in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | (ESend iovl|ECast iovl), Appl ->
      if Once.isset s.got_view then (
	eprintf "CHK_SYNC:received message after Up(EView) (dropping msg)\n" ;
	eprintf "  event=%s\n" (Event.to_string ev) ;
	dnnm (create name EDump[]) ;
	Iovecl.free iovl
      ) else (
	let origin = getPeer ev in
	if getCompactType ev = C_ECast then
	  array_incr s.cast_recv origin
	else 
	  array_incr s.send_recv origin ;
	up ev abv
      )

  | _,NoHdr -> up ev abv
  | _,_ -> failwith bad_up_event

  and uplm_hdlr ev hdr = match getType ev,hdr with
  (* Check whether all members have the same view states.
   *)
  | ECast iovl, ViewState vs' ->
      if vs' <> vs then 
	failwith "mismatched view state" ;
      Iovecl.free iovl

  | ECast iovl,Gossip(rmt_vs,rmt_casts,rmt_send_xmit) ->
      let origin = getPeer ev in
      let rmt_send_xmit = Arrayf.get rmt_send_xmit ls.rank in
      chk_rmt := (origin,rmt_vs,rmt_casts,rmt_send_xmit) :: !chk_rmt ;
      chk_sync () ;
      Iovecl.free iovl
  | _,_ -> failwith "bad uplm event"

  and upnm_hdlr ev = match getType ev with

  (* If I'm coordinator then tell the others what my view
   * state is.  
   *)
  | EInit ->
      if ls.am_coord then
      	dnlm (castEv name) (ViewState vs) ;
      upnm ev

  | EView ->
      Once.set s.got_view ;
      let cast_recv = Arrayf.of_array s.cast_recv in
      let cast_recv = Arrayf.fset cast_recv ls.rank s.cast_xmit in
      let send_recv = Arrayf.of_array s.send_recv in
      let send_xmit = Arrayf.of_array s.send_xmit in
      let next_view_id = View.id_of_state (getViewState ev) in
      chk_recv := Some(next_view_id,cast_recv,send_recv) ;
      chk_sync () ;
      dnlm (create name (ECast Iovecl.empty) [NoTotal]) 
        (Gossip(next_view_id,cast_recv,send_xmit)) ;
      upnm ev

  | EFail ->
      s.failed <- getFailures ev ;
      upnm ev

  | EDump -> dump vf s ; upnm ev 
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | ECast _| ESend _->
      if getApplMsg ev then (
	if Once.isset s.got_dn_block then (
	  eprintf "CHK_SYNC:sending message after Dn(Block)\n" ;
	  dnnm (create name EDump[])
	) ;
	if getCompactType ev = C_ECast then
      	  s.cast_xmit <- succ s.cast_xmit
	else 
          array_incr s.send_xmit (getPeer ev) ;
        dn ev abv Appl
      ) else (
	dn ev abv NoHdr
      )
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr ev = match getType ev with
  | EBlock ->
      Once.set s.got_dn_block ;
      dnnm ev
  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
