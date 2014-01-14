(**************************************************************)
(* BYPASS.ML *)
(* Author: Mark Hayden, 11/95 *)
(* Based in part on ideas of Robbert vanRenesse *)
(**************************************************************)
open Stacke
open Layer
open Event
open Util
open Transport
open View
open Glue
(**************************************************************)
let name = Trace.filel "BYPASS"
let failwith s = failwith (Util.failmsg name s)
(**************************************************************)
let ts_done = Timestamp.add "BYPASS:recv_done"

let procure a = Layer.procure_hide a

type 'abv cbypass_env = {
  c_appl_intf_recv_cast : origin -> Iovec.t -> (Iovec.t, Iovec.t) Appl_intf.action list ;
  c_bottom_alive 	: bool array ;
  c_bypass_xmit 	: int -> Iovec.t -> unit ;
  c_handle_action 	: (Iovec.t, Iovec.t) Appl_intf.action -> unit ;
  c_iq_add 		: 'abv Iq.t -> 'abv -> unit ;
  c_iq_opt_insert_check_doread : 'abv Iq.t -> int -> 'abv -> bool ;
  c_mnak_casts 		: 'abv Iq.t ;
  c_mnak_my_casts 	: 'abv Iq.t ;
  c_origin 		: rank ;
  c_rank 		: rank ;
  c_stable_my_row 	: seqno array ;
  c_stable_ncasts 	: seqno array ;
  c_top_appl_s 		: Top_appl.state ;
  c_no_bypass		: seqno -> Iovec.t -> unit
}

external cbypass_receive : 'abv cbypass_env -> seqno -> Iovec.t -> unit = "bypass_receive"

type 'abv h1 = (Top_appl.header ,'abv   ) msg
type 'abv h2 = (Sync.header     ,'abv h1) msg
type 'abv h6 = (Pt2pt.header	,'abv h2) msg
type 'abv h7 = (Stable.header	,'abv h6) msg
type 'abv h8 = (Mnak.header	,'abv h7) msg
type 'abv h9 = (Bottom.header	,'abv h8) msg

(* Bypass accounting info.
 *)
let act_recv_normal	 = ref 0
and act_recv_bypass_fail = ref 0
and act_recv_bypass_succ = ref 0
and act_recv_next	 = ref 0
and act_send_normal	 = ref 0
and act_send_bypass	 = ref 0
and act_fail_mnak_seqno	 = ref 0
and act_next		 = ref Time.zero

let getdns_l dn_ref dn_insert inher =
  let l {empty_lout=empty;up_lout=up;dn_lout=dn} =
    dn_ref := dn ;
    let dn ev msg =
      if not (dn_insert ev msg) then dn ev msg
    in

    let up ev abv =
      if getType ev = ETimer & !verbose & (getTime ev) >= !act_next then (
	act_next := Time.add !act_next (Time.of_int 1) ;
	eprintf "BYPFIFO: send{norm=%d byp=%d} recv{norm=%d fail=%d succ=%d (ns=%d)}\n"
	  !act_send_normal !act_send_bypass
	  !act_recv_normal !act_recv_bypass_fail !act_recv_bypass_succ
	  !act_fail_mnak_seqno
      ) ;
      up ev abv
    in

    {empty_lin=empty;up_lin=up;dn_lin=dn}
  in ((),l)

(* This is the actual layer function, which takes as
 * arguments the initialized layers and their states and
 * handlers out of this layer.	
 *)
let l state vs =

  (* Set of functions for coercing objects to the correct
   * type.
   *)


  




  let l hdlrs =

  (*let iq_add = profile_2 "Iq.add[BYPASS]" Iq.add in*)
  (*let iq_opt_insert_doread = profile_3
      "Iq.opt_insert_doread[BYPASS]"
      Iq.opt_insert_doread in*)

    let rank		= ls.rank in
    let bypass_xmit_r	= ref (fun _ _ -> failwith "byp_cast unset") in
    let bypass_disable_r = ref (fun () -> failwith "byp_disable unset") in
    let bypass_disable () = !bypass_disable_r () in

    let dn_nobyp_r	= ref (fun _ _ -> failwith "sanity:dn_nobyp_r")
    and dn_catch_r	= ref (fun _ _ -> failwith "sanity:dn_catch_r") in
    let dn_nobyp ev msg	= !dn_nobyp_r ev msg
    and dn_catch ev msg	= !dn_catch_r ev msg in

    let top_appl_r	= ref None
    and sync_r		= ref None
    and pt2pt_r		= ref None
    and mnak_r		= ref None
    and stable_r	= ref None
    and bottom_r	= ref None in

    let cap r l vs =
      let (s,h) = l vs in
      r := Some s ;
      (s,h)
    in

    let cap r l =
      let l = cap r l in
      convert Imperative l
    in

    (* Initialize the sub-layers.
     *)
(*
    let top_appl = cap top_appl_r (procure "Top_appl" : ('top_appl1,'top_appl2) Top_appl.t)
    and sync	= cap sync_r	(procure Sync	: ('sync1,'sync2) Sync.t)
    and pt2pt	= cap pt2pt_r	(procure (Pt2pt(stable_sweep,stable_max_idle)) : ('pt2pt1,'pt2pt2,'pt2pt_cps) Pt2pt.t)
    and mnak	= cap mnak_r	(procure Mnak 	: ('mnak1,'mnak2,'mnak_cps) Mnak.t)
    and stable	= cap stable_r	(procure (Stable(stable_sweep,stable_max_idle,false)) : ('stable1,'stable2) Stable.t)
    and bottom	= cap bottom_r	(procure Bottom	: ('bottom1,'bottom2) Bottom.t)
    in
*)
    let top_appl= cap top_appl_r(procure "Top_appl" state)
    and sync	= cap sync_r	(procure "Sync"	  state)
    and pt2pt	= cap pt2pt_r	(procure "Pt2pt"  state)
    and mnak	= cap mnak_r	(procure "Mnak"   state)
    and stable	= cap stable_r	(procure "Stable" state)
    and bottom	= cap bottom_r	(procure "Bottom" state)
    in

    let (|||) = compose in

    (* Compose them.
     *)
    let sub_l =
	  top_appl
      ||| sync
      ||| pt2pt
      ||| stable
      ||| mnak
      ||| bottom
    in

    (* Get the handlers into the stack.
     *)

(*
    let up = init sub_l () () Appl.root_sched vs in
*)

(*
    let {empty_lout=empty;up_lout=up;dn_lout=dn} = hdlrs in
    let dn ev = 
      if getType ev = ELeave then
      	bypass_disable () ;
      dn ev
    in
    let hdlrs = {empty_lout=empty;up_lout=up;dn_lout=dn} in
*)
(*
    let hdlrs = sub_l hdlrs in
*)

    let top_appl_s 	= some_of !top_appl_r
    and sync_s	   	= some_of !sync_r
    and pt2pt_s	   	= some_of !pt2pt_r
    and mnak_s	   	= some_of !mnak_r (* : ('abv h6,'e) Mnak.state)*)
    and stable_s   	= some_of !stable_r
    and bottom_s	= some_of !bottom_r in

    let expand_mnak =
      let abv = local_msg Top_appl.NoHdr in
      let abv = compose_msg abv Sync.NoHdr in
      let abv = compose_msg abv pt2pt_s.Pt2pt.byp_hdr in
      let abv = compose_msg abv stable_s.Stable.byp_hdr in
      (abv : 'abv h7)
    in

    let expand_bottom mnak_seqno =
      let abv = expand_mnak in
      let abv = compose_msg abv (Mnak.Data mnak_seqno) in
      let abv = compose_msg abv bottom_s.Bottom.byp_hdr in
      abv
    in

    (* Set the normCastMsg and pre-expand the buffers at the
     * mnak layer.  
     *)
    mnak_s.Mnak.normCastMsg <- Some expand_mnak ;
    Array.iter (fun iq -> Iq.grow iq 200) mnak_s.Mnak.casts ;

    (* Prebind some variables.	Hack!
     *)
    let appl_intf_recv_cast = top_appl_s.Top_appl.intf.Appl_intf.recv_cast in
    let mnak_casts_v	= mnak_s.Mnak.casts in
    let mnak_my_casts	= mnak_casts_v.(rank) in
    let stable_my_row	= stable_s.Stable.my_row in
    let stable_ncasts	= stable_s.Stable.ncasts in
    let bottom_alive	= bottom_s.Bottom.alive in

    let bypass_ECast iov =
      (* Create the message and send it via bypass.                     
       *)
      let mnak_seqno = Iq.tail mnak_my_casts in

      !bypass_xmit_r mnak_seqno iov ;

      (* Do postop updates.
       *)
      incr act_send_bypass ;
      stable_ncasts.(rank) <- succ stable_ncasts.(rank) ;(* BUG!!!! *)
      Iq.add mnak_my_casts (Mnak.CompressedIov(iov)) ;
      true
    in

  (*let bypass_ECast = profile_1 "bypass_ECast" bypass_ECast in*)
    (* Outside critical path.
     *)
    dn_catch_r := (fun ev abv ->
      match getType ev, abv with
      | ECast, Empty -> (		(* hack, hack, hack *)
	  let iov = Iovecl.flatten (getIov ev) in
	  let res = bypass_ECast iov in	(* returns bool *)
	  res
	)
      | _ -> false
    ) ;

    let sched = Appl.root_sched in 
    let enqueue a b = Sched.enqueue sched (fun () -> dn_nobyp a b) in

    let rec handle_actions = function
    | [] -> ()
    | hd::tl -> (
      	begin match hd with
	| Appl_intf.Cast(iov) ->
	    if not (bypass_ECast iov) then
	      enqueue (castIov name [|iov|])
	       (local_msg Top_appl.NoHdr)
	| Appl_intf.Send(dests,msg) ->
            enqueue (sendPeersIov name dests [|msg|])
              (local_msg Top_appl.NoHdr)
	| Appl_intf.LeaveBlock -> (
	    top_appl_s.Top_appl.state <- Top_appl.Blocked ;
	    enqueue (create name ELeave[]) (no_msg ())
	  )
	| Appl_intf.XferDone -> (
	    enqueue (create name EXferDone []) (no_msg ())
	  )
	| Appl_intf.Protocol(proto_id) -> (
	    enqueue (create name EProtocol[
		(ProtoId proto_id)
	      ]) (no_msg ())
	  )
	| Appl_intf.Dump -> failwith "Dump not supported"
	| Appl_intf.Timeout _ -> failwith "Timeout not supported"
	end ;
	handle_actions tl
      )
    in

    let bypass_receive kind origin =
      let mnak_casts = mnak_casts_v.(origin) in
      fun mnak_seqno msg_iov -> (
        if bottom_alive.(origin)
	&& top_appl_s.Top_appl.state = Top_appl.Normal
	&& Iq.opt_insert_check_doread mnak_casts mnak_seqno (Mnak.CompressedIov(msg_iov))
	then (
	  (* Pass to application.
	   *)
	  handle_actions (appl_intf_recv_cast origin msg_iov) ;

	  (* State updates.
	   *)
          stable_ncasts.(origin) <- succ stable_ncasts.(origin) ;
	  if mnak_seqno >= stable_my_row.(origin) then
	    stable_my_row.(origin) <- succ mnak_seqno ;

     	  ts_done () ;
	  incr act_recv_bypass_succ

	) else if bottom_s.Bottom.enabled then (
	  incr act_recv_bypass_fail ;

	  (* Reconstruct message and send up.
	   *)
	  let abv = expand_bottom mnak_seqno in
()(* BUG
          hdlrs.up_lin (upBodyCore name UpInvalid origin [|msg_iov|]) abv
*)
	)
      )
    in
(*
    let bypass_receive origin kind =
      let mnak_casts = mnak_casts_v.(origin) in
      let no_bypass mnak_seqno msg_iov =
	(* Reconstruct message and send up.
	 *)
	let abv = expand_bottom mnak_seqno in
        hdlrs.up_lin (upBodyCore name UpInvalid origin [|msg_iov|]) abv
      in
      
      let env = {
	c_appl_intf_recv_cast 		= appl_intf_recv_cast ;
	c_bottom_alive 			= bottom_alive ;
	c_bypass_xmit 			= (fun a b -> !bypass_xmit_r a b) ;
	c_handle_action 		= (fun a -> handle_actions [a]) ;
	c_iq_add 			= Iq.add ;
	c_iq_opt_insert_check_doread 	= Iq.opt_insert_check_doread ;
	c_mnak_casts 			= mnak_casts ;
	c_mnak_my_casts 		= mnak_my_casts ;
	c_origin 			= origin ;
	c_rank 				= rank ;
	c_stable_my_row 		= stable_my_row ;
	c_stable_ncasts 		= stable_ncasts ;
	c_top_appl_s 			= top_appl_s ;
	c_no_bypass			= no_bypass
      } in

      fun mnak_seqno msg_iov -> cbypass_receive env mnak_seqno msg_iov
    in
*)
    (* Problem: we want to preprocess bypass_receive, but
     * can't because some handlers get a (-1) for the
     * origin, so we filter those out here. 
     *)
    let bypass_receive kind origin =
      if origin = -1 then 
      	(fun _ _ -> failwith "sanity:bypass_receive") 
      else
        bypass_receive kind origin
    in

  (*let bypass_receive = Profile.profile_3 "bypass_receive" bypass_receive in*)
  (*let bypass_receive = Util.gc_profile3 "bypass_receive" bypass_receive in*)

    (* Initialize bypass receipt in transport.
     *)
    let bypass_trans = Transport.f
      vs.group
      ls.endpt
      Stack_id.Bypass
      vs.proto_id
      Security.NoKey
      vs.view
      vs.view_id
      false
      Routers.bypass
      bypass_receive
    in
    bypass_xmit_r    := cast bypass_trans ;
    bypass_disable_r := (fun () -> Transport.disable bypass_trans) ;

    hdlrs
  in ((),l)

let l = (l :  Layer.state -> ('abv,'abv h9,unit) Layer.basic)
(*
let _ = Elink.layer_install name (Layer.initTop_appl l)
*)

(**************************************************************)
