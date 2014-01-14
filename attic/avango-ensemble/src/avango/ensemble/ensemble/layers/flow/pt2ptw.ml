(**************************************************************)
(* PT2PTW.ML : Point-to-point window-based flow control. *)
(* Author: Mark Hayden, 3/96 *)
(* Note that the window-cost of a message here is 1 more
 * than the number of bytes in the iov field of the message. *)
(**************************************************************)
open Buf
open Layer
open View
open Event
open Util
open Trans
(**************************************************************)
let name = Trace.filel "PT2PTW"
(**************************************************************)

type header = NoHdr | Ack of int

(**************************************************************)

type mark_state = Hi | Low

type 'abv state = {
  window : int ;
  ack_thresh : int ;
  mark : mark_state array ;
  hi_wmark : int ;
  overhead : int ;
  send_buf : (Event.dn * 'abv) Queuee.t array ;
  send_credit : int array ;
  recv_credit : int array ;
  mutable failed : bool Arrayf.t
}  

(**************************************************************)

let msg_len s iovl = (int_of_len (Iovecl.len iovl)) + s.overhead

let string_of_queue_len q = string_of_int (Queuee.length q)
let string_of_queue_bytes s q = 
  let len = ref 0 in
  Queuee.iter (fun (ev,_) -> 
    match getType ev with 
      | ECast iovl | ESend iovl | ECastUnrel iovl | ESendUnrel iovl ->
	  len := !len + msg_len s iovl
      | _ -> ()
  ) q ;
  string_of_int !len

let dump vf s = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "send_buf=%s\n" (string_of_array string_of_queue_len s.send_buf) ;
  sprintf "send_credit=%s\n" (string_of_int_array s.send_credit) ;
  sprintf "recv_credit=%s\n" (string_of_int_array s.recv_credit)
|]) vf s

(**************************************************************)

let init _ (ls,vs) = 
  let window = Param.int vs.params "pt2ptw_window" in
  { window = window ;
    overhead = Param.int vs.params "pt2ptw_overhead" ;
    ack_thresh = Param.int vs.params "pt2ptw_ack_thresh" ;
    mark = Array.create ls.nmembers Low ;
    hi_wmark = Param.int vs.params "pt2ptw_hi_wmark" ;
    send_buf = Array.init ls.nmembers (fun _ -> Queuee.create ()) ;
    send_credit = Array.create ls.nmembers window ;
    recv_credit = Array.create ls.nmembers 0 ;
    failed = ls.falses
  }

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
(*let ack = make_acker name dnnm in*)
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in
  let logb = Trace.log3 Layer.buffer ls.name name in
  let logc = Trace.log2 (name^"C") ls.name in

  (* Check the water-mark. If it is low, tell the application to 
   * continue sending messagses; if it is high, tell it to block.
   *)
  let check_wmark peer = 
    if Queuee.length s.send_buf.(peer) >= s.hi_wmark && s.mark.(peer) = Low then (
      logc (fun () -> sprintf "Hi") ;
      s.mark.(peer) <- Hi ;
      let ev = create name EFlowBlock[FlowBlock (Some(peer),true)] in
      upnm ev 
    ) else if Queuee.length s.send_buf.(peer) = 0 && s.mark.(peer) = Hi then (
      logc (fun () -> sprintf "Low") ;
      s.mark.(peer) <- Low ;
      let ev = create name EFlowBlock[FlowBlock (Some(peer),false)] in
      upnm ev 
    )
  in

  let up_hdlr ev abv hdr = match getType ev, hdr with

    (* Increase amount of credit to pass back to sender.
     * If the amount of credit is beyond the threshhold,
     * send an acknowledgement.  Finally, deliver the
     * message.  
     *)
  | ESend iovl, NoHdr ->
      let origin = getPeer ev in
      array_add s.recv_credit origin (msg_len s iovl) ;
      if s.recv_credit.(origin) > s.ack_thresh then (
	dnlm (sendPeer name origin) (Ack(s.recv_credit.(origin))) ;
	s.recv_credit.(origin) <- 0 ;
      ) ;
      up ev abv

  | _, NoHdr -> up ev abv
  | _        -> failwith bad_header

  and uplm_hdlr ev hdr = match getType ev,hdr with
    (* Some credit were sent back, send more data if its waiting.
     *)
  | ESend iovl, Ack(credit) ->
      let origin = getPeer ev in 
      array_add s.send_credit origin credit ;

      let buf = s.send_buf.(origin) in
      while s.send_credit.(origin) > 0 && not (Queuee.empty buf) do
	(* Queuee.take will not fail here.
	 *)
	let ev,abv = Queuee.take buf in
	dn ev abv NoHdr ;
      	array_sub s.send_credit origin (msg_len s iovl)
      done ;

      check_wmark origin ;

      (*ack ev ;*) free name ev 
  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with
    (* EFail: Mark failed members, and pass on up.
     *)
  | EFail ->
      let failed = getFailures ev in
      s.failed <- failed ;
      for rank = 0 to pred ls.nmembers do
	if Arrayf.get failed rank then (
	  Queuee.iter (fun (ev,_) ->
	    free name ev ;
	  ) s.send_buf.(rank) ;
	  Queuee.clear s.send_buf.(rank) ;

	  s.send_credit.(rank) <- 0 ;
	  s.recv_credit.(rank) <- 0
	)
      done ;
      upnm ev

  | EExit -> 
      (* Release all events in the send queues.
       *)
      Array.iter (Queuee.clean (fun (ev,_) -> free name ev)) s.send_buf ;
      upnm ev

  | EAccount ->
      logb (fun () -> sprintf "blocked(msgs):%s" 
	(string_of_array string_of_queue_len s.send_buf)) ;
      logb (fun () -> sprintf "blocked(byte):%s" 
	(string_of_array (string_of_queue_bytes s) s.send_buf)) ;
      upnm ev

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with

    (* Send a message to each destination.  If we don't have
     * any credit, then buffer it.  
     *)
  | ESend iovl ->
      let dest = getPeer ev in
      if Arrayf.get s.failed dest then (
	log (fun () -> sprintf "send to failed member") ;
	Iovecl.free iovl
      ) else if s.send_credit.(dest) > 0 then (
	(* Normal case. I have enough credit.
	 *)
	array_sub s.send_credit dest (msg_len s iovl) ;
        dn ev abv NoHdr
      ) else (
	log (fun () -> sprintf "messages are buffered") ;
	Queuee.add (ev,abv) s.send_buf.(dest) ;
	check_wmark dest 
      )

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr init hdlrs None (FullNoHdr NoHdr) args vs

let _ = 
  Param.default "pt2ptw_window" (Param.Int 50000) ;
  Param.default "pt2ptw_ack_thresh" (Param.Int 25000) ;
  Param.default "pt2ptw_hi_wmark"    (Param.Int 100) ;
  Param.default "pt2ptw_overhead" (Param.Int 100) ;
  Layer.install name l

(**************************************************************)
