(**************************************************************)
(* MFLOW.ML : MFLOW window-based flow control. *)
(* Authors: Zhen Xiao, Mark Hayden, 2/97 *)
(* Note that the window-cost of a message here is some
 * number of bytes more than the number of bytes in the iov
 * field of the message. This # is set by the mflow_overhead
 * parameter.  
 * Ohad Rodeh: Added application flow-control feed-back. 
 * I Used a Hi-Low water-mark scheme. 10/98
*)
(**************************************************************)
open Buf
open Trans
open Layer
open Event
open Util
open View
(**************************************************************)
let name = Trace.filel "MFLOW"
(**************************************************************)

type header = NoHdr
  | NoFlow
  | Ack of int

(**************************************************************)

type mark_state = 
  | Hi
  | Low

type 'abv state = {
  mutable exiting : bool ;
  mutable mark : mark_state ;
  hi_wmark   : int ;
  stagger    : bool ;
  window     : int ;
  ack_thresh : int ;
  overhead   : len ;
  send_buf   : (Event.dn * 'abv) Queuee.t ;
  credit     : Mcredit.t
} 

(**************************************************************)

let msg_len s iovl = int_of_len ((Iovecl.len iovl) +|| s.overhead)

let string_of_queue_len q = string_of_int (Queuee.length q)

let queue_bytes s q = 
  let len = ref 0 in
  Queuee.iter (fun (ev,_) ->
    match getType ev with 
      | ECast iovl | ESend iovl | ECastUnrel iovl | ESendUnrel iovl ->
	  len := !len + msg_len s iovl
      | _ -> ()
  ) q ;
  !len

let string_of_queue_bytes s q = 
  string_of_int (queue_bytes s q)

let dump vf s = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "send_buf=%s\n" (string_of_queue_len s.send_buf) 
(*; eprintf "%s\n" (Mcredit.to_string_list s.credit)*)
|]) vf s

(**************************************************************)

let init_credit sdr dst nmembers ack_thresh window =
  if nmembers = 1 then
    (0, window)			(* Avoid div-by-0 *)
  else (
    let sdr = if sdr > dst then pred sdr else sdr in
    let amt = - ((sdr * ack_thresh) / (pred nmembers)) in
    (amt,window-amt)
  )

let init _ (ls,vs) = 
  let hi_wmark   = Param.int  vs.params "mflow_hi_wmark" in
  let window     = Param.int  vs.params "mflow_window" in
  let ack_thresh = Param.int  vs.params "mflow_ack_thresh" in
  let stagger    = Param.bool vs.params "mflow_stagger" in

  let send_credit,recv_credit = 
    if stagger then (
      let send = 
	Array.init ls.nmembers (fun dst -> 
	  snd (init_credit dst ls.rank ls.nmembers ack_thresh window)
	)
      and recv = 
	Array.init ls.nmembers (fun dst -> 
	  fst (init_credit ls.rank dst ls.nmembers ack_thresh window)
	)
      in send,recv
    ) else (
      let send = Array.create ls.nmembers window
      and recv = Array.create ls.nmembers 0 
      in send,recv
    )
  in

  { exiting    = false ;
    mark       = Low ;
    hi_wmark   = hi_wmark;
    window     = window ;
    stagger    = stagger ;
    ack_thresh = ack_thresh ;
    send_buf   = Queuee.create () ;
    overhead   = Buf.len_of_int (Param.int vs.params "mflow_overhead") ;
    credit     = Mcredit.create ls.rank ls.nmembers ack_thresh send_credit recv_credit
  }

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
(*let ack = make_acker name dnnm in*)
  let log = Trace.log2 name ls.name in
  let logl = Trace.logl2 name ls.name in
  let logb = Trace.log3 Layer.buffer ls.name name in
  let logc = Trace.log2 (name^"C") ls.name in

  (* Check the water-mark. If it is low, tell the application to continue 
   * multicasting messagses; if it is high, tell it to block.
   *)
  let check_wmark () = 
    if not s.exiting then 
      if Queuee.length s.send_buf >= s.hi_wmark
	&& s.mark = Low then (
	  logc (fun () -> "Hi");
	  s.mark <- Hi;
	  let ev = create name EFlowBlock[FlowBlock (None,true)] in
	  upnm ev 
	) else if Queuee.length s.send_buf = 0 
	  && s.mark=Hi then (
	    logc (fun () -> sprintf "Low, credit_left=%d" (Mcredit.left s.credit));
	    s.mark <- Low;
	    let ev = create name EFlowBlock[FlowBlock (None,false)] in
	    upnm ev 
	  )
  in				

  let check_msg s = 
    while (not (Queuee.empty s.send_buf))
    && Mcredit.check s.credit 
    do
    (* Queue.take will not fail here.>
     *)
      let ev,abv = Queuee.take s.send_buf in
      let len = 
	match getType ev with 
	  | ECast iovl | ESend iovl | ECastUnrel iovl | ESendUnrel iovl ->
	      msg_len s iovl
	  | _ -> 0
      in
      
      (* The assignment below is unnecessary, but is
       * added for the functional optimizations.
       *)
      (*s.credit <-*) ignore (Mcredit.take s.credit len) ;
      dn ev abv NoHdr
    done;
    check_wmark ()
  in

  let up_hdlr ev abv hdr = match getType ev, hdr with
    (* Increment amount of credit to pass back to sender.
     * If the amount of credit is beyond the threshhold,
     * send an acknowledgement.  Finally, deliver the
     * message.  
     *)
  | ECast iovl,NoHdr ->
      let origin = getPeer ev in

      (* Don't do any credit stuff with casts from myself.
       *)
      if origin <>| ls.rank then (
        let len = msg_len s iovl in
	let current_credit = Mcredit.got_msg s.credit origin len in
	if current_credit >=| s.ack_thresh then (
	  let nacks = current_credit / s.ack_thresh in
	  let remainder = current_credit - (nacks * s.ack_thresh) in
	  dnlm (sendPeer name origin) (Ack nacks) ;
	  Mcredit.set_credit s.credit origin remainder
        )
      ) ;
      up ev abv

  | ECast _, NoFlow -> up ev abv
  | _, NoHdr -> up ev abv
  | _        -> failwith bad_header

  and uplm_hdlr ev hdr = match getType ev,hdr with
    (* Some credit were sent back, send more data if its waiting.
     *)
  | ESend iovl, Ack(nacks) ->
      let origin = getPeer ev in 
      for i = 1 to nacks do
	Mcredit.got_credit s.credit origin ;
      done ;
      check_msg s ;
      Iovecl.free iovl
  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with
    (* Mark failed members, and pass on up.
     *)
  | EFail ->      
      let failed = getFailures ev in
      for rank = 0 to pred ls.nmembers do
	if Arrayf.get failed rank then
	  Mcredit.fail s.credit rank
      done ;
      check_msg s ;
      upnm ev

  | EExit -> 
      (* Release all events in the send queue.
       *)
      s.exiting <- true;
      Queuee.clean (fun (ev,_) -> free name ev) s.send_buf ;
      upnm ev

  | EAccount ->
      logb (fun () -> sprintf "blocked(msgs):%s" 
	  (string_of_queue_len s.send_buf)) ;
      logb (fun () -> sprintf "blocked(byte):%s" 
	  (string_of_queue_bytes s s.send_buf)) ;
      logl (fun () -> Mcredit.to_string_list s.credit) ;
      log (fun () -> sprintf "credit_left = %d" (Mcredit.left s.credit)) ;
      Mcredit.clear s.credit ;
      upnm ev

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  (* Send a message to each destination.  If we don't have
   * any credit, then buffer it.  Note: multicast flow
   * control takes effect only to reliable application
   * messages and to fragmented messages (see note in Frag
   * layer).  For other messages, just send them
   * immediately.  In contrast, unicast flow control takes
   * effect to both application and control messages
   * (unreliable ones are still sent immediately.  
   *)
  | ECast iovl ->      
      if getApplMsg ev || getFragment ev then (
	if Mcredit.check s.credit then (
	  (* Normal case.  We have enough credit.
	   *)
	  ignore (Mcredit.take s.credit (msg_len s iovl)) ;
	  dn ev abv NoHdr
	) else (
	  Queuee.add (ev,abv) s.send_buf;
	  check_wmark ()
	)
      ) else (
	dn ev abv NoFlow
      )

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm 

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr init hdlrs None (FullNoHdr NoHdr) args vs

let _ = 
  Param.default "mflow_window" (Param.Int 50000) ;
  Param.default "mflow_hi_wmark" (Param.Int 100) ;
  Param.default "mflow_ack_thresh" (Param.Int 25000) ;
  Param.default "mflow_overhead" (Param.Int 100) ;
  Param.default "mflow_stagger" (Param.Bool true) ;
  Layer.install name l

(**************************************************************)
