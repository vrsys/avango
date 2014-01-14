(**************************************************************)
(* BYPFIFO.ML *)
(* Author: Mark Hayden, 11/95 *)
(* Based in part on ideas of Robbert vanRenesse *)
(**************************************************************)
open Layer
open Event
open Util
open Transport
open Compose
open Procure
open Stacke
open View
(**************************************************************)
let name = Trace.filel "BYPFIFO"
let failwith s = failwith (Util.failmsg name s)
(**************************************************************)

type 'abv h2 = (Leave.header	,'abv	) msg
type 'abv h3 = (Sync.header	,'abv h2) msg
type 'abv h4 = (Gmp.header	,'abv h3) msg
type 'abv h5 = (Merge.header	,'abv h4) msg
type 'abv h6 = (Pt2pt.header	,'abv h5) msg
type 'abv h7 = (Stable.header	,'abv h6) msg
type 'abv h8 = (Mnak.header	,'abv h7) msg
type 'abv h9 = (Bottom.header	,'abv h8) msg

(* Bypass accounting info.
 *)
let act_cast_bypass 	= ref 0
and act_cast_fail 	= ref 0
and act_fail_mnak_seqno	= ref 0
and act_send_bypass	= ref 0
and act_send_fail	= ref 0
and act_fail_pt2pt_seqno = ref 0
and act_next		= ref Time.zero

(* This is the actual layer function, which takes as
 * arguments the initialized layers and their states and
 * handlers out of this layer.	
 *)
let l
  (merge_sweep,
   merge_timeout,
   stable_sweep,
   stable_max_idle)
  vs
=
  (* Set of functions for coercing objects to the correct
   * type.
   *)
  let (pack,unpack) = Util.make_magic () in
  let pack_msg  (msg : 'abv) = Some(pack msg)
  and unpack_msg = function
    | None -> failwith "unpack_msg:None"
    | Some msg -> unpack msg
  in

  let l hdlrs_out =
    let my_rank		= ls.rank
    and leave_r		= ref None
    and sync_r		= ref None
    and gmp_r		= ref None
    and merge_r		= ref None
    and pt2pt_r		= ref None
    and mnak_r		= ref None
    and stable_r	= ref None
    and bottom_r	= ref None in

    let cap r l vs =
      let (s,h) = l vs in
      r := Some s ;
      (s,h)
    in

    (* Initialize the sub-layers.
     *)
    let leave	= cap leave_r	(procure Leave 	: ('leave1,'leave2) Leave.t)
    and sync	= cap sync_r	(procure Sync	: ('sync1,'sync2) Sync.t)
    and gmp	= cap gmp_r	(procure Gmp	: ('gmp1,'gmp2) Gmp.t)
    and merge	= cap merge_r	(procure (Merge(merge_sweep,merge_timeout)) : ('merge1,'merge2) Merge.t)
    and pt2pt	= cap pt2pt_r	(procure (Pt2pt(stable_sweep,stable_max_idle)) : ('pt2pt1,'pt2pt2,'pt2pt_cps) Pt2pt.t)
    and stable	= cap stable_r	(procure (Stable(stable_sweep,stable_max_idle,false)) : ('stable1,'stable2) Stable.t)
    and mnak	= cap mnak_r	(procure Mnak 	: ('mnak1,'mnak2,'mnak_cps) Mnak.t)
    and bottom	= cap bottom_r	(procure Bottom : ('bottom1,'bottom2) Bottom.t)
    in

    (* Compose them.
     *)
    let sub_l =
          leave
      ||| sync
      ||| gmp
      ||| merge
      ||| pt2pt
      ||| stable
      ||| mnak
      ||| bottom
    in

    let (sub_l,enq1,enq2,sched) = Atomic.f sub_l in

    (* Get the handlers into the stack.
     *)
    let (_,sub_l) = sub_l vs in
    let hdlrs_in = sub_l hdlrs_out in

    let leave_s	  	= some_of !leave_r
    and sync_s	  	= some_of !sync_r
    and gmp_s	  	= some_of !gmp_r
    and merge_s	  	= some_of !merge_r
    and pt2pt_s	  	= some_of !pt2pt_r
    and stable_s  	= some_of !stable_r
    and mnak_s	  	= some_of !mnak_r (*: ('abv h5,'abv) Mnak.state)*)
    and bottom_s	= some_of !bottom_r in

    let expand_mnak hdr =
      let abv = hdr in
      let abv = compose_msg abv leave_s.Leave.byp_hdr in
      let abv = compose_msg abv sync_s.Sync.byp_hdr in
      let abv = compose_msg abv gmp_s.Gmp.byp_hdr in
      let abv = compose_msg abv merge_s.Merge.byp_hdr in
      let abv = compose_msg abv pt2pt_s.Pt2pt.byp_hdr in
      let abv = compose_msg abv stable_s.Stable.byp_hdr in
      (abv : 'abv h7)
    in

    let expand_pt2pt hdr =
      let abv = hdr in
      let abv = compose_msg abv leave_s.Leave.byp_hdr in
      let abv = compose_msg abv sync_s.Sync.byp_hdr in
      let abv = compose_msg abv gmp_s.Gmp.byp_hdr in
      let abv = compose_msg abv merge_s.Merge.byp_hdr in
      (abv : 'abv h5)
    in

    (* Prebind some variables.	Hack!
     *)
    let mnak_casts_v	= mnak_s.Mnak.casts in
    let mnak_my_casts	= mnak_casts_v.(my_rank) in
    let pt2pt_sends_v 	= pt2pt_s.Pt2pt.sends in
    let pt2pt_recvs_v 	= pt2pt_s.Pt2pt.recvs in
    let stable_my_row	= stable_s.Stable.my_row in
    let stable_ncasts	= stable_s.Stable.ncasts in
    let bottom_alive	= bottom_s.Bottom.alive in

    let bypass_receive origin = function
    | Conn.Cast -> (
	let mnak_casts = mnak_s.Mnak.casts.(origin) in
	fun mnak_seqno msg_obj msg_iov ->
          let abv = unpack_msg msg_obj in
	  if bottom_alive.(origin) 
	  && Iq.opt_insert_check_doread mnak_casts mnak_seqno (Mnak.Compressed(abv,expand_mnak,msg_iov))
	  then (
	    (* Pass to application.
	     *)
	    hdlrs_out.up_lout (upBypassCast name origin msg_iov) abv ;

	    (* Ack the message too....
	     *)
	    stable_s.Stable.ncasts.(origin) <- succ stable_s.Stable.ncasts.(origin) ;
	    if mnak_seqno >= stable_my_row.(origin) then
	      stable_my_row.(origin) <- succ mnak_seqno ;

	    incr act_cast_bypass
	  ) else if bottom_s.Bottom.enabled then (
	    incr act_cast_fail ;

	    (* Reconstruct message and send up.
	     *)
	    let abv = compose_msg abv leave_s.Leave.byp_hdr in
	    let abv = compose_msg abv sync_s.Sync.byp_hdr in
	    let abv = compose_msg abv gmp_s.Gmp.byp_hdr in
	    let abv = compose_msg abv merge_s.Merge.byp_hdr in
	    let abv = compose_msg abv pt2pt_s.Pt2pt.byp_hdr in
	    let abv = compose_msg abv stable_s.Stable.byp_hdr in
	    let abv = compose_msg abv (mnak_s.Mnak.normCastHdr mnak_seqno) in
	    let abv = compose_msg abv bottom_s.Bottom.byp_hdr in
	    hdlrs_in.up_lin (upBodyCore name ECast origin msg_iov) abv
	  )
      )
    | Conn.Send -> (
      	let pt2pt_recvs = pt2pt_recvs_v.(origin) in
	fun pt2pt_seqno msg_obj msg_iov ->
          let abv = unpack_msg msg_obj in
	  if bottom_alive.(origin)
	  && Iq.opt_check_update pt2pt_recvs pt2pt_seqno
	  then (
	    (* Pass to application.
	     *)
	    hdlrs_out.up_lout (upBypassSend name origin msg_iov) abv ;

	    incr act_send_bypass
	  ) else if bottom_s.Bottom.enabled then (
	    incr act_send_fail ;

	    (* Reconstruct message and send up.
	     *)
	    let abv = compose_msg abv leave_s.Leave.byp_hdr in
	    let abv = compose_msg abv sync_s.Sync.byp_hdr in
	    let abv = compose_msg abv gmp_s.Gmp.byp_hdr in
	    let abv = compose_msg abv merge_s.Merge.byp_hdr in
	    let abv = compose_msg abv (pt2pt_s.Pt2pt.normSendHdr pt2pt_seqno) in
	    let abv = compose_msg abv stable_s.Stable.byp_hdr in
	    let abv = compose_msg abv mnak_s.Mnak.byp_hdr in
	    let abv = compose_msg abv bottom_s.Bottom.byp_hdr in
	    hdlrs_in.up_lin (upBodyCore name ESend origin msg_iov) abv
	  )
      )
    | _ -> failwith sanity
    in

    let bypass_receive origin kind =
      if origin = (-1) then
      	fun _ _ _ -> failwith sanity
      else
        bypass_receive origin kind
    in

  (*let bypass_receive = Profile.profile_3 "bypass_receive" bypass_receive in*)

    (* Initialize bypass receipt in transport.
     *)
    let bypass_trans = Transport.f
      vs.group
      ls.endpt
      Stack_id.Bypass
      vs.proto_id
      vs.key
      vs.view
      vs.view_id
      false
      (Routers.standard vs.key)
      bypass_receive
    in
    let bypass_cast = Transport.cast bypass_trans in
    let bypass_send = 
      array_create ls.nmembers (fun i ->
	if i = ls.rank then (fun _ _ -> failwith "send to me") else (
	  let res = Transport.send bypass_trans i in res
	)
      ) 
    in
(* BUG!!!
    bypass_disable_r := (fun () -> disable bypass_trans) ;
*)

    let {dn_lin=dn;up_lin=up;empty_lin=empty} = hdlrs_in in

    let dn ev abv = match getType ev with
    | ECast ->
	(* Create the message and send it via bypass.
	 *)
	bypass_cast
      	  (Iq.tail mnak_my_casts)
	  (pack_msg abv)
      	  (getIov ev) ;

	(* Do postop updates.
	 *)
	Iq.add mnak_my_casts (Mnak.Compressed(abv,expand_mnak,(getIov ev))) ;
	stable_ncasts.(my_rank) <- succ stable_ncasts.(my_rank) ;
	stable_my_row.(my_rank) <- succ stable_my_row.(my_rank) ;
	free name ev
    | ESend ->
      	List.iter (fun dest ->
	  (* Create the message and send it via bypass.
	   *)
	  bypass_send.(dest)
      	    (Iq.tail pt2pt_sends_v.(dest))
      	    (pack_msg abv)
      	    (getIov ev) ;

	  (* Do postop updates.
	   *)
	  Iq.add pt2pt_sends_v.(dest) (Pt2pt.Compressed(abv,expand_pt2pt,(getIov ev)))
	) (getPeers ev) ;
	free name ev
    | _ -> dn ev abv
    in

    let hdlrs_in = {dn_lin=dn;up_lin=up;empty_lin=empty} in
    hdlrs_in
  in ((),l) 

(*
let l = (l : Time.t * Time.t * Time.t * int -> (('a,'b)Layer.msg,('a,'b) Layer.msg h9,unit) Layer.basic)
*)

let _ = Elink.layer_install name (Layer.initBypfifo l)

(**************************************************************)
