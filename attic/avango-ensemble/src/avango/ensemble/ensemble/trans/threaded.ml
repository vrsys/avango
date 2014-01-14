(**************************************************************)
(* THREADED.ML *)
(* Author: Mark Hayden, 3/96 *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "THREADED"
let failwith s = failwith (Util.failmsg name s)
(**************************************************************)

let alarm sched handlers mbuf =
  let mutex = Mutex.create () in
  Mutex.lock mutex ;

(*let ts_recv = Timestamp.add "THREADED:recv" in*)
  let threads = ref [] in
  let sock_add s h =
    let reader () =
      Hsys.setsock_nonblock s ;		(* from ThreadUnix *)
      while true do
	Thread.wait_read (Obj.magic (Obj.repr s)) ; (* HACK! *)
	(* Acquire mutex before reading & making callback.
	 *)
	Mutex.lock mutex ;
	begin
	  match h with
	  | Hsys.Handler0 f ->
	      f ()
	  | Hsys.Handler1 ->
	      Mbuf.alloc_udp_recv name *** this is broken ***
		mbuf
		(fun buf ofs len -> Hsys.udp_recv s buf ofs len)
		f
		(fun _ -> failwith "error")
		false ;
	      ()
	end ;
	Mutex.unlock mutex ;
      done
    in
    
    let id = Thread.create reader () in
    threads := (s,id) :: !threads ;
  in

  let sock_rmv s =
    let id =
      try List.assoc s !threads with
      |	Not_found -> failwith "couldn't find thread"
    in
    threads := except (s,id) !threads ;
    Thread.kill id
  in      

  let socks = Resource.create name sock_add sock_rmv (fun _ -> ()) in

  let block () = 
    Mutex.unlock mutex ;
    Thread.delay (0.2) ;
    Mutex.lock mutex ;
  in

  let gettime = Time.gettimeofday in

  let min () = failwith sanity

  and check () = false

  and alarm callback =
    let enabled = ref true in
    let thread time =
      (* Sleep until specified time.
       *)
      while gettime () < time do
	if not !enabled then 
	  Thread.exit () ;
	let now = gettime () in
	Thread.delay (Time.to_float (Time.sub time now)) ;
      done ;
      if !enabled then (
	(* Acquire mutex before making callback.
	 *)
	Mutex.lock mutex ;
      	callback (gettime ()) ;
	Mutex.unlock mutex ;
      ) ;
    in
    let disable () = 
      enabled := false 
    in
    let schedule time =
      Thread.create thread time ; ()
    in 
    Alarm.c_alarm disable schedule

  and poll _ =
    (* Release and acquire mutex.
     *)
    Mutex.unlock mutex ;
    Thread.delay 0.0 ;
    Mutex.lock mutex ;
    Alarm.alm_poll false

  and add_sock = Resource.add socks
  and rmv_sock = Resource.remove socks
  and add_poll = Alarm.alm_add_poll
  and rmv_poll = Alarm.alm_rmv_poll

  in Alarm.create
    "THREADED"
    gettime
    alarm
    check
    min
    add_sock
    rmv_sock
    block
    add_poll
    rmv_poll
    poll

let _ =
(*
  Trace.install_resource (fun () ->
    eprintf "THREADED:info\n" ;
    eprintf "  socks:%s\n" (Resource.info socks)
  ) ;
*)
  Arge.alarm_install Alarm.Threaded alarm

(**************************************************************)
