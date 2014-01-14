(**************************************************************)
(* HTK.ML *)
(* Author: Mark Hayden, 3/96 *)
(**************************************************************)
open Tk
open Ensemble
open Util
open Trans
open Hsys
open Buf
(**************************************************************)
let name = Trace.file "HTK"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
(**************************************************************)

let exec_events sched =
  while Sched.step sched 100 do
    log (fun () -> sprintf "looping")
  done

let alarm ((unique,sched,async,handlers) as gorp) =
  let gettime = Time.gettimeofday

  and alarm callback =
    let disable () = () in
    let schedule time =
      let timef	= Time.to_float time in
      let now 	= Time.gettimeofday () in
      let now   = Time.to_float now in
      let milli	= (timef -. now) *. 1000.0 in

      (* We sometimes get timers that are set to go off in
       * in the past!
       *)
      let milli 	= max 0 (truncate milli) in

      ignore (Timer.add milli (fun () ->
	try 
	  log (fun () -> sprintf "timer awake{") ;

	  (* Tk sometimes delivers alarms early, according
	   * got gettimeofday and our calculations above.
	   *)
	  let now = max time (Time.gettimeofday ()) in

	  (* Make the callback and execute all events to
	   * completion.
	   *)
	  callback now ;
	  exec_events sched ;

	  log (fun () -> "timer asleep") ;
	with e -> 
	  eprintf "Uncaught exception in HTK\n";
	  raise e
      ))
    in Alarm.c_alarm disable schedule
  in

  let poll_f = ref None in
  let alm_poll b = 
    match !poll_f with
    | Some f -> f b
    | None -> b
  in

  let sockets = Resource.create "HTK:sockets"

    (* Add a callback for a socket.
     *)
    (fun _ (sock,hdlr) -> 
      Fileevent.add_fileinput sock (fun () ->
	try 
	  log (fun () -> sprintf "sock awake{") ;
	  begin
	    match hdlr with
	    | Hsys.Handler0 f -> f ()
	    | Hsys.Handler1 ->
		let (buf,iov) = Hsys.udp_recv_packet sock in
		if Buf.length buf <>|| len0 || Iovec.len iov <>|| len0 then
		  let iovl = Iovecl.singleton iov in
		  Route.deliver handlers buf Buf.len0 (Buf.length buf) iovl
	  end ;
          ignore (alm_poll false) ;
	  exec_events sched ;
	  log (fun () -> sprintf "sock asleep}") ;
	with e -> 
	  eprintf "Uncaught exception in HTK\n";
	  raise e

      )
    )

    (* Remove a socket's callback.
     *)
    (fun _ (sock,_) -> 
       Fileevent.remove_fileinput sock)

    ignore
    ignore
  in

  let add_sock_recv d s h = Resource.add sockets d (Hsys.int_of_socket s) (s,h)
  and rmv_sock_recv s = Resource.remove sockets (Hsys.int_of_socket s)
  and add_sock_xmit _ = failwith "add_sock_xmit"
  and rmv_sock_xmit _ = failwith "rmv_sock_xmit"
  and add_poll n p = poll_f := Alarm.alm_add_poll n p
  and rmv_poll n = poll_f := Alarm.alm_rmv_poll n
  and min _ 	= failwith "min"
  and check _ 	= failwith "check"
  and block _ 	= failwith "block"
  and poll _	= failwith "poll"

  in Alarm.create
    name
    gettime
    alarm
    check
    min
    add_sock_recv
    rmv_sock_recv
    add_sock_xmit
    rmv_sock_xmit
    block
    add_poll
    rmv_poll
    poll
    gorp

(**************************************************************)

let init alarm =
  exec_events (Alarm.sched alarm)

(* BUG: This is an ugly hack, we should try to add initialization
 * to the alarm.
*)
let init_hack () = 
  init (Alarm.get_hack ())
(**************************************************************)

let _ = Alarm.install "TK" alarm

(**************************************************************)
