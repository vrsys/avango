(**************************************************************)
(* PT2PTWP.ML : Point-to-point window-based flow control with 
 * shared receiver's window. *)
(* Author: Mark Rabotnikov, 8/99 *)
(* Notes:
 * 1. The window-cost of a message here is some number of bytes more 
 *    than the number of bytes in the iov field of the message. 
 *    This # is set by the pt2ptwp_overhead parameter.
 * 2. The protocol assumes the same window size in all processes. 
 * 3. Other threshold measures can be used.
 * 4. control messages - ?
 * 5. heap - ?
 *)

(**************************************************************)
open Buf
open Layer
open View
open Event
open Util
open Trans
(**************************************************************)
let name = Trace.filel "PT2PTWP"
(**************************************************************)

type header = NoHdr | NoFlow | Ack of int

type wmark_state = Hi | Low

module IntSet = Set.Make(struct type t = int let compare = compare end)

(**************************************************************)

type 'abv state = {
  window              : int ;
  min_credit          : int ;
  ack_thresh          : float ;
  overhead            : int ;
  sweep               : Time.t ;
  mutable next_sweep  : Time.t ;
  hi_wmark            : int ;
  wmark               : wmark_state array ;
  send_buf            : (Event.dn * 'abv) Queuee.t array ;
  send_credit         : int array ;
  mutable failed      : bool Arrayf.t ;
  bws                 : float array ;
  sqrt_bws            : float array ;
  received            : int array ;
  scredit             : int array ;
  rcredit             : int array ;
  stolen              : int array ;
  bw_thresh           : int ;
  fast                : bool array ;
  mutable candidates  : IntSet.t ;
(*
  mutable msgs        : int ;
  mutable acks        : int
*)
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

let string_of_int_set s = string_of_list string_of_int (IntSet.elements s)
  
let dump vf s = Layer.layer_dump name (
  fun (ls,vs) s -> [|
    sprintf "send_buf=%s\n"    (string_of_array string_of_queue_len s.send_buf) ;
    sprintf "send_credit=%s\n" (string_of_int_array s.send_credit) ;
    sprintf "bandwidths=%s\n"  (string_of_array (fun f -> sprintf "%.2f" f) s.bws) ;
    sprintf "scredit=%s\n"     (string_of_int_array s.scredit) ;
    sprintf "rcredit=%s\n"     (string_of_int_array s.rcredit) ;
    sprintf "stolen=%s\n"      (string_of_int_array s.stolen) ;
    sprintf "fast=%s\n"        (string_of_bool_array s.fast) ;
    sprintf "candidates=%s\n"  (string_of_int_set s.candidates) ;
    sprintf "failed=%s\n"      (Arrayf.bool_to_string s.failed)  
  |]) vf s

(**************************************************************)

let init _ (ls,vs) = 
  let window     = Param.int vs.params "pt2ptwp_window" in
  let min_credit = Param.int vs.params "pt2ptwp_min_credit" in
  let init_value = if ls.nmembers = 1 then 0 else (min_credit + window / pred ls.nmembers) in
  let candidates = ref IntSet.empty in
  for r = 0 to pred ls.nmembers do
    if r <> ls.rank then
      candidates := IntSet.add r !candidates
  done ;
  { window      = window ;
    min_credit  = min_credit ;
    ack_thresh  = Param.float vs.params "pt2ptwp_ack_thresh" ;
    overhead    = Param.int vs.params "pt2ptwp_overhead" ;
    sweep	= Param.time vs.params "pt2ptwp_sweep" ;
    next_sweep  = Time.invalid ; 
    hi_wmark    = Param.int vs.params "pt2ptwp_hi_wmark" ;
    wmark       = Array.create ls.nmembers Low ;
    send_buf    = Array.init ls.nmembers (fun _ -> Queuee.create ()) ;
    send_credit = Array.init ls.nmembers (fun i -> if i = ls.rank then 0 else init_value) ; 
    failed      = ls.falses ;
    bws         = Array.create ls.nmembers 0.0 ;
    sqrt_bws    = Array.create ls.nmembers 0.0 ;
    received    = Array.create ls.nmembers 0 ;
    scredit     = Array.init ls.nmembers (fun i -> if i = ls.rank then 0 else init_value) ;
    rcredit     = Array.create ls.nmembers 0 ;
    stolen      = Array.create ls.nmembers 0 ;
    bw_thresh   = Param.int vs.params "pt2ptwp_bw_thresh" ;
    fast        = Array.create ls.nmembers false ;
    candidates  = !candidates ;
(*
    msgs        = 0 ;
    acks        = 0
*)
  }

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log  = Trace.log2 name ls.name in
  let logb = Trace.log3 Layer.buffer ls.name name in
  let logc = Trace.log2 (name^"C") ls.name in

  (* Check the water-mark. If it is low, tell the application to 
   * continue sending messages; if it is high, tell it to block.
   *)
  let check_wmark peer = 
    if Queuee.length s.send_buf.(peer) >= s.hi_wmark && s.wmark.(peer) = Low then (
      logc (fun () -> sprintf "Hi") ;
      s.wmark.(peer) <- Hi ;
      let ev = create name EFlowBlock[FlowBlock (Some(peer),true)] in
      upnm ev 
    ) else if Queuee.length s.send_buf.(peer) = 0 && s.wmark.(peer) = Hi then (
      logc (fun () -> sprintf "Low") ;
      s.wmark.(peer) <- Low ;
      let ev = create name EFlowBlock[FlowBlock (Some(peer),false)] in
      upnm ev 
    )
  in

  (* Return the window of a process.
   *)
  let window r =
    s.scredit.(r) - s.stolen.(r) + s.rcredit.(r)
  in

  (* Check if a process has passed threshold of its window.
   *)
  let passed_thresh r =
    let usage = float_of_int s.rcredit.(r) /. float_of_int (window r) in
    usage > s.ack_thresh
  in

  (* Send ack with credit to a process.
   *)
  let send_ack r credit =
    dnlm (sendPeer name r) (Ack(credit)) ;
    array_add s.scredit r credit ;
    s.rcredit.(r) <- 0 ;
    s.stolen.(r) <- 0 ;
    (*s.acks <- succ s.acks*)
  in

  (* Find a process from whom to steal credit and return its rank 
     and the amount of credit to steal. 
  *)
  let find_proc to_r = 
    let to_w = window to_r in
    let to_ratio = s.sqrt_bws.(to_r) /. float_of_int to_w in
    let from_r = ref (-1) in
    let from_w = ref 0 in
    let from_ratio = ref (-1.0) in
    let apply r =
      if r <> to_r then (
	let w = window r in
	let ratio = s.sqrt_bws.(r) /. float_of_int w in
	if !from_r < 0 || ratio < !from_ratio then (
 	  from_r := r ;
	  from_w := w ;
	  from_ratio := ratio
	)
      )
    in
    IntSet.iter (apply) s.candidates ;
    if !from_r < 0 then (-1,0)
    else 
      if !from_ratio > 0.9 *. to_ratio then (-1,0) 
      else (
	let x1 = int_of_float (float_of_int !from_w *. s.ack_thresh *. 0.5)  in
	let x2 = !from_w - s.min_credit in
	let x3 = int_of_float 
	  ((s.sqrt_bws.(to_r) *. float_of_int !from_w -. 
	    s.sqrt_bws.(!from_r) *. float_of_int to_w) /. 
	   (s.sqrt_bws.(!from_r) +. s.sqrt_bws.(to_r))) in
	let x = min (min x1 x2) x3 in
	if x > 0 then (!from_r,x) else (-1,0)
      )
  in

  (* Steal some credit from another process. Send ack if needed.
   *)
  let steal_credit to_r =
    let (from_r,credit) = find_proc to_r in
    if from_r < 0 then 0
    else (
      log (fun () -> 
	let to_w = window to_r in
	let from_w = window from_r in
	sprintf "%d<-%d-%d, (%d,%d)<-(%d,%d)" 
	  to_r credit from_r (to_w + credit) (from_w - credit) to_w from_w
      ) ;
      s.stolen.(from_r) <- s.stolen.(from_r) + credit ;
      if window from_r = s.min_credit then
	s.candidates <- IntSet.remove from_r s.candidates ;
      if passed_thresh from_r then 
	send_ack from_r (s.rcredit.(from_r) - s.stolen.(from_r)) ;
      credit
    )
  in
    
  let up_hdlr ev abv hdr = match getType ev, hdr with
    (* Increase amount of credit to pass back to the sender. If the sender 
     * has passed threshold of its window, try to steal some credit from 
     * another process and send ack to the sender. Finally, deliver 
     * the message.  
     *)
  | ESend iovl, NoHdr ->
      let origin = getPeer ev in
      if Arrayf.get s.failed origin then
	log (fun () -> sprintf "message from failed member") 
      else (
	(*s.msgs <- succ s.msgs ;*)
	let len = msg_len s iovl in
	array_add s.received origin len ;
	array_add s.rcredit origin len ;
	array_sub s.scredit origin len ;
	if passed_thresh origin then (
	  let credit = 
	    if s.fast.(origin) then 
	      if s.stolen.(origin) > 0 
	      then s.rcredit.(origin) - s.stolen.(origin)
	      else (
		let stolen_credit = steal_credit origin in
		if window origin + stolen_credit > s.min_credit then
		  s.candidates <- IntSet.add origin s.candidates ;
		s.rcredit.(origin) + stolen_credit
	      )
	    else s.rcredit.(origin) - s.stolen.(origin) in
	  send_ack origin credit
	)
      ) ;
      up ev abv

(* | ESend, NoFlow -> up ev abv *)

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
	let ev,abv = Queuee.take buf in
	dn ev abv NoHdr ;
      	array_sub s.send_credit origin (msg_len s iovl)
      done ;
      check_wmark origin ;

      Iovecl.free iovl

  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with

    (* EInit: request a first timer alarm to bootstrap bandwidth measuring.
     *)
  | EInit ->
      dnnm (timerAlarm name Time.zero) ;
      upnm ev

    (* ETimer: adjust bandwidth, start next timer alarm.
     *)
  | ETimer -> 
      let time = getTime ev in
      if Time.Ord.ge time s.next_sweep then (
      	let old = s.next_sweep in
	s.next_sweep <- Time.add time s.sweep ;
	dnnm (timerAlarm name s.next_sweep) ;
	
	(* This check prevents calculations immediately after a view.
	 *)
      	if old <> Time.invalid then (
	  for r = 0 to pred ls.nmembers do
	    if r <> ls.rank && not (Arrayf.get s.failed r) then (
	      let bw = float_of_int s.received.(r) /. Time.to_float s.sweep in
	      s.bws.(r) <- 0.6 *. s.bws.(r) +. 0.4 *. bw ;
	      s.fast.(r) <- int_of_float s.bws.(r) > s.bw_thresh ;
	      s.sqrt_bws.(r) <- sqrt s.bws.(r) ;
              s.received.(r) <- 0
	    ) 
	  done ;
	  log (fun () -> sprintf "bandwidths=%s" 
	      (string_of_array (fun f -> sprintf "%.2f" f) s.bws)) ;
	  log (fun () -> sprintf "fast=%s" (string_of_bool_array s.fast))
	)
      ) ;
      upnm ev

    (* EFail: Mark failed members, and pass on up.
     *)
  | EFail ->
      let failed = getFailures ev in
      s.failed <- failed ;
      for r = 0 to pred ls.nmembers do
	if r <> ls.rank && Arrayf.get failed r then (
	  Queuee.iter (fun (ev,_) -> free name ev) s.send_buf.(r) ;
	  Queuee.clear s.send_buf.(r) ;
	  s.send_credit.(r) <- 0 ;
	  s.bws.(r) <- 0.0 ;
	  s.sqrt_bws.(r) <- 0.0 ;
	  s.received.(r) <- 0 ;
	  s.scredit.(r) <- 0 ;
	  s.rcredit.(r) <- 0 ;
	  s.stolen.(r) <- 0 ;
	  s.fast.(r) <- false ;
	  s.candidates <- IntSet.remove r s.candidates
	)
      done ;
      upnm ev

  | EExit -> 
      (* Release all events in the send queues.
       *)
      Array.iter (Queuee.clean (fun (ev,_) -> free name ev)) s.send_buf ;
      (*
      let efficiency = float_of_int s.msgs /. float_of_int (s.msgs + s.acks) in
      eprintf "received %d msgs, sent %d acks, efficiency = %.4f\n" 
	s.msgs s.acks efficiency ;
      *)
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

    (* Send a message. If we don't have any credit, then buffer it. 
     * Note: pt2pt flow control takes effect to reliable application 
     * messages, fragmented messages (see note in Frag layer) and 
     * control messages. Unreliable ones are still sent immediately.  
     *)
  | ESend iovl ->
(*      if getApplMsg ev || getFragment ev then ( *)
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
(*    ) else (
        dn ev abv NoFlow
      )
*)  
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr init hdlrs None (FullNoHdr NoHdr) args vs

let _ = 
  Param.default "pt2ptwp_window"      (Param.Int 150000) ;
  Param.default "pt2ptwp_min_credit"  (Param.Int 6000) ;
  Param.default "pt2ptwp_bw_thresh" (Param.Int 300) ; (* B/s *)
  Param.default "pt2ptwp_ack_thresh"  (Param.Float 0.5) ;
  Param.default "pt2ptwp_sweep"       (Param.Time (Time.of_int 2)) ;
  Param.default "pt2ptwp_hi_wmark"    (Param.Int 100) ;
  Param.default "pt2ptwp_overhead"    (Param.Int 100) ;
  Layer.install name l

(**************************************************************)
