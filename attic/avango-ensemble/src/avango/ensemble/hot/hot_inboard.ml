(**************************************************************)
(* HOT_INBOARD.ML *)
(* Author:  Alexey Vaysburd, 11/96 *)
(* init_view_state and capp_main_loop are based on code by Mark Hayden *)
(* Bug Fixes, Cleanup: Mark Hayden, 4/97 *)
(* More Bug Fixes, Cleanup: Mark Hayden, 5/97 *)
(**************************************************************)
open Ensemble
open Trans
open Buf
open Util
open View
open Appl_intf
open Appl_intf.New
open Protos
open Hot_util
(**************************************************************)
let name = Trace.file "HOT_INBOARD"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
(**************************************************************)
(* External C functions.
 *)

type c_dncall_full = {
  id : id ;
  dncall : Hot_appl.c_dncall 
}

external usleep : int -> unit 
    = "hot_ens_thread_usleep"

external cback_install_view : id -> c_view_state -> unit 
    = "hot_ens_AcceptedView_cbd"

external cback_heartbeat : id -> millisec -> unit
    = "hot_ens_Heartbeat_cbd"

external cback_recv_cast : id -> endpt -> Iovec.raw -> unit 
    = "hot_ens_ReceiveCast_cbd"

external cback_recv_send : id -> endpt -> Iovec.raw -> unit 
    = "hot_ens_ReceiveSend_cbd"

external cback_block : id -> unit 
    = "hot_ens_Block_cbd"

external cback_exit : id -> unit
    = "hot_ens_Exit_cbd"

(* Retrieve pending downcalls from C application.
 *)
external c_get_dncalls : unit -> c_dncall_full array
    = "hot_ens_GetDncalls"

(* Call out to C for printing and exceptions.
 *)
external c_printer : string -> bool
    = "hot_ens_MLDoPrint"

external c_exception_handler : string -> bool
    = "hot_ens_MLHandleException"

(**************************************************************)

let interface id heartbeat_rate =
  let install ((ls,vs) as vf) =
    let cvs = c_view_state vf in
    cback_install_view id cvs ;

    log (fun () -> "after cback_install_view");
    let receive o blk cs =
      let origin = Endpt.string_of_id (Arrayf.get vs.view o) in
      let upcall = 
	match cs with
	| S ->
	    fun iovec ->
	      cback_recv_send id origin iovec
	| C ->
	    fun iovec ->
	      cback_recv_cast id origin iovec
      in
      fun iovl ->
      	let iov = Iovecl.flatten iovl in
	upcall (Iovec.raw_of iov) ;
      	Iovecl.free iovl ;
      	Iovec.free iov ;
      	[||]
    in
    let heartbeat time = 
      cback_heartbeat id (milli_of_time time) ; 
      log (fun () -> "after cback_heartbeat");
      [||] in
    let disable = ident in
    let block () = cback_block id ; [||] in
    [||],{
      flow_block = (fun _ -> ());
      heartbeat = heartbeat ;
      receive = receive ;
      block = block ;
      disable = disable
    } 
  in
  let exit () = cback_exit id in
  { heartbeat_rate = heartbeat_rate ;
    install = install ;
    exit = exit }

let join ctxt id vf hbr dispatch = 
  let interface = interface id hbr in
  Hot_appl.join
    ctxt
    id
    dispatch
    vf
    interface

(**************************************************************)
  
(* Get pending dncalls from C application and dispatch among resp. groups. 
 *)
let rec dispatch_dncalls ctxt () = 
  let dn_arr = c_get_dncalls () in 
  Array.iter (function {id = gctx; dncall = dncall} ->
    (* Dispatch a downcall request to the corresp. group context.
     *)
    match dncall with
    | Hot_appl.C_Join jops -> 
	log (fun () -> "C_Join");
	let vf = init_view_state jops in
	let heartbeat_rate = Time.of_float (float jops.jops_hrtbt_rate /. 1000.0) in
	join ctxt gctx vf heartbeat_rate (dispatch_dncalls ctxt)
    | _ ->
        (* Add request to the list of pending dncalls for the given gctx.
         *)
	log (fun () -> "Hot_appl.get");
 	(Hot_appl.get ctxt gctx) dncall 
  ) dn_arr ;
  
  Array.length dn_arr <> 0

(**************************************************************)

(* Polling function called from C.
 *)
let poll ctxt alarm =
  let sched = Alarm.sched alarm in
  let sched_step = Sched.step sched in
  let count = Arge.get Arge.sched_step in
  let check_msgs = Alarm.poll alarm Alarm.SocksPolls in
  let check_timeouts = Alarm.check alarm in

  (* This is the actual function exported to C.
   *)
  let event_loop () =
    try
      (* Check for messages.
       *)
      let got_msgs = check_msgs () in

      (* Check for application downcalls.
       *)
      log (fun () -> "dispatch_dncalls");
      let got_dncalls = dispatch_dncalls ctxt () in

      (* Schedule some events in the layers.
       *)
      let got_events = sched_step count in

      (* Check for timers.
       *)
      log (fun () -> "check_timeouts");
      let got_alarms = check_timeouts () in

(*
      eprintf "HOT_INBOARD:poll:msgs=%b dncalls=%b events=%b alarms=%b\n"
	got_msgs got_dncalls got_events got_alarms ;
*)

      (* Return true if there is more stuff to do.
       *)
      got_alarms || got_msgs || got_events || got_dncalls
    with e ->
      let e = Util.error e in
      if not (c_exception_handler e) then
	eprintf "HOT_INBOARD:poll:uncaught exception:%s\n" e ;
      exit 1
  in event_loop

(**************************************************************)

let block alarm =
  let block () =
    try
(*
      eprintf "HOT_INBOARD:block\n" ;
*)
      Alarm.block alarm
    with e ->
      let e = Util.error e in
      if not (c_exception_handler e) then
	eprintf "HOT_INBOARD:block:uncaught exception:%s\n" e ;
      exit 1
  in
  block

(**************************************************************)

let export_timeout = (ref Time.zero : Time.t ref)

let run () =
  Arge.parse [
    (*
     * Extra arguments can go here.
     *)
  ] (Arge.badarg name) "libhot:Ensemble HOT tools library" ;

  let alarm = Appl.alarm name in

  (* Force linking of the Udp mode.  This causes
   * a port number to be installed in the 
   * Unique generator.
   *)
  let _ = Domain.of_mode alarm Addr.Udp in

  let ctxt = Hot_appl.context name in
  Appl.start_monitor () ;

  (* Register the function for polling Ensemble.
   *)
  let poll = poll ctxt alarm in
  Callback.register "hot_poll" poll ;
  Callback.register "hot_block" (block alarm) ;

  (* This is the information about timeouts and sockets.
   *)
  Callback.register "hot_sockets" Real.export_socks ;
  Callback.register "hot_timeout" export_timeout ;

  if Sys.os_type = "Win32" then (
    (* For Windows, we do something different.
     *)

    (* First we install a blocking function that goes
     * to sleep.
     *)
    Real.install_blocker (fun timeout ->
      let timeout = Time.to_float timeout in
      let timeout = 
      	if timeout < 0.0 then 10000
	else (
	  let timeout = timeout *. 1000000.0 in
	  let timeout = truncate timeout in
	  let timeout = min timeout 100000 in
	  timeout
	)
      in
      usleep timeout
    ) ;

    (* Then we stay in our own main loop.
     *)
    while true do
      let ret = poll () in
      if not ret then 
	Alarm.block alarm
    done
  ) else (
    (* This overrides blocking calls in the Real module.
     *)
    Real.install_blocker (fun timeout ->
(*
      eprintf "HOT_INBOARD:timeout=%f\n" timeout ;
*)
      export_timeout := timeout
    )
  )

let _ =
  let printer ch s =
    if not (c_printer s) then (
      output_string ch s ;
      flush ch
    )
  in
  Util.fprintf_override printer ;
  try run () with e ->
    let e = Util.error e in
    if not (c_exception_handler e) then
      eprintf "HOT_INBOARD:top:uncaught exception:%s\n" e ;
    exit 1

(**************************************************************)
