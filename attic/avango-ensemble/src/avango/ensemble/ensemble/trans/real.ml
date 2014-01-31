(**************************************************************)
(* REAL.ML *)
(* Author: Mark Hayden, 3/96 *)
(**************************************************************)
open Util
open Trans
open Buf
(**************************************************************)
let name = Trace.file "REAL"
let failwith = Trace.make_failwith name
let log = Trace.log name
(**************************************************************)

let squash_polls pa =
  let n = Arrayf.length pa in
  match n with
  | 0 -> ident
  | 1 -> Arrayf.get pa 0
  | 2 ->
      let p0 = Arrayf.get pa 0 in
      let p1 = Arrayf.get pa 1 in
      fun b -> p1 (p0 b)
  | 3 ->
      let p0 = Arrayf.get pa 0 in
      let p1 = Arrayf.get pa 1 in
      let p2 = Arrayf.get pa 2 in
      fun b -> p2 (p1 (p0 b))
  | _ ->
      let rec loop i rm =
	if i >= n then rm else (
	  loop (succ i) ((Arrayf.get pa i) rm)
        ) 
      in
      fun b -> loop 0 b
(*
let squash_polls = function
  | [] -> ident
  | [p1] -> p1
  | [p1;p2] -> fun b -> p2 (p1 b)
  | [p1;p2;p3] -> fun b -> p3 (p2 (p1 b))
  | pl ->
      let rec loop rm = function
        | poll::tl -> loop (poll rm) tl
        | [] -> rm
      in 
      fun b -> loop b pl
*)

let unroll_loops flags handlers =
  match Array.length flags with 
  | 0 -> 
      eprintf "REAL:unroll_loops:warning:0 case should not occur\n" ;
      ident
  | 1 -> 
      let handler0 = Arrayf.get handlers 0 in
      fun () ->
      	if flags.(0) then handler0 ()
  | 2 -> 
      let handler0 = Arrayf.get handlers 0 in
      let handler1 = Arrayf.get handlers 1 in
      fun () ->
      	if flags.(0) then handler0 () ;
      	if flags.(1) then handler1 ()
  | 3 -> 
      let handler0 = Arrayf.get handlers 0 in
      let handler1 = Arrayf.get handlers 1 in
      let handler2 = Arrayf.get handlers 2 in
      fun () ->
      	if flags.(0) then handler0 () ;
      	if flags.(1) then handler1 () ;
      	if flags.(2) then handler2 ()
  | 4 -> 
      let handler0 = Arrayf.get handlers 0 in
      let handler1 = Arrayf.get handlers 1 in
      let handler2 = Arrayf.get handlers 2 in
      let handler3 = Arrayf.get handlers 3 in
      fun () ->
      	if flags.(0) then handler0 () ;
      	if flags.(1) then handler1 () ;
      	if flags.(2) then handler2 () ;
      	if flags.(3) then handler3 ()
  | 5 -> 
      let handler0 = Arrayf.get handlers 0 in
      let handler1 = Arrayf.get handlers 1 in
      let handler2 = Arrayf.get handlers 2 in
      let handler3 = Arrayf.get handlers 3 in
      let handler4 = Arrayf.get handlers 4 in
      fun () ->
      	if flags.(0) then handler0 () ;
      	if flags.(1) then handler1 () ;
      	if flags.(2) then handler2 () ;
      	if flags.(3) then handler3 () ;
      	if flags.(4) then handler4 ()
  | n ->
      let flags_hi = pred n in
      fun () ->
	for i = 0 to flags_hi do
	  if flags.(i) then
	    Arrayf.get handlers i ()
	done

let helper route_handlers handler info = 
  if Arrayf.is_empty info then 
    None
  else (
    let socks,handlers = Arrayf.split info in
    let flags = Array.create (Arrayf.length socks) false in
    let error _ = failwith "error[1]" in
    let handlers =
      Arrayf.map (function
	| (_,Hsys.Handler0 f) -> f
	| (sock,Hsys.Handler1) ->
	    fun () -> 
	      let buf,iov = Hsys.udp_recv_packet sock in
	      if Buf.length buf <>|| len0 || Iovec.len iov <>|| len0 then (
		let iovl = Iovecl.singleton iov in
		handler route_handlers buf Buf.len0 (Buf.length buf) iovl
	      )
      ) (Arrayf.combine socks handlers)
    in
    let socks = Arrayf.to_array socks in
    let info = Some(socks,flags) in
    let handle = unroll_loops flags handlers in
    Some(info,handle)
  )

let build_block_poll route_handlers handler recv_info xmit_info polls =
  let (select_info,handle) =
    let recv = helper route_handlers handler recv_info in
    let xmit = helper route_handlers handler xmit_info in
    match recv,xmit with
    | None,None ->
	(Hsys.select_info None None None),ident
    | Some(info,handle),None ->
	(Hsys.select_info info None None),handle
    | None,Some(info,handle) ->
	(Hsys.select_info None info None),handle
    | Some(recv_info,recv_handle),
      Some(xmit_info,xmit_handle) ->
	(Hsys.select_info recv_info xmit_info None),
	(fun () -> recv_handle () ; xmit_handle ())
  in

  let block time =
    let nready = Time.mut_select select_info time in
    if nready > 0 then
      handle ()
  and poll =
    if Arrayf.is_empty polls then (
      fun () ->
	let nready = Hsys.poll select_info in
	if nready > 0 then (
	  handle () ;
	  true
	) else false
    ) else (
      let polls = squash_polls polls in
      fun () ->
	let nready = Hsys.poll select_info in
	if nready > 0 then (
	  handle () ;
	  polls true
	) else (
	  polls false
        )
    )
  and onlypolls =
    squash_polls polls
  in (block,poll,onlypolls)

(**************************************************************)

module Priq = Priq.Make ( Time.Ord )

let blocker = ref None

let install_blocker f =
  blocker := Some(f)

let export_socks = ref (Arrayf.empty : Hsys.socket Arrayf.t)

let alarm ((unique,sched,async,handlers) as gorp) =
  let deliver = 
    if Arge.timestamp_check "recv" then (
      let stamp = Timestamp.register"UDP:recv" in
      let ts_add () = Timestamp.add stamp in
      let deliver h buf ofs len iovl =
	ts_add () ;
    	Route.deliver h buf ofs len iovl
      in deliver
    ) else Route.deliver
  in

  let update_r = ref (fun () -> failwith sanity) in
  let update () = !update_r () in

  let socks_recv = 
    Resource.create "REAL:socks_recv"
    ignore2
    ignore2
    (fun _ -> update ())
    (fun socks ->
      log (fun () -> sprintf "socks_recv=%s" (Resource.to_string socks)))
  in

  let socks_xmit = 
    Resource.create "REAL:socks_xmit"
    ignore2
    ignore2
    (fun _ -> update ())
    (fun socks ->
      log (fun () -> sprintf "socks_xmit=%s" (Resource.to_string socks)))
  in

  let polls =
    Resource.create "REAL:polls" 
    ignore2
    ignore2
    (fun _ -> update ())
    (fun polls ->
      log (fun () -> sprintf "polls=%s" (Resource.to_string polls)))
  in

  let blockr = ref (fun _ -> ()) in
  let pollr = ref (fun () -> false) in
  let onlypollr = ref (fun () -> false) in

  update_r := (fun () ->
    export_socks := (Arrayf.map fst (Resource.to_array socks_recv): Hsys.socket Arrayf.t) ;
(*
    eprintf "export_socks=%s\n" (string_of_int_array (Array.map Hsys.int_of_socket !export_socks)) ;
*)

    if Arge.get Arge.multiread then (
      let deliver handlers buf ofs len iovl =
	Sched.enqueue_5arg sched name deliver handlers buf ofs len iovl
      in
      let (block,poll,onlypoll) = 
	build_block_poll 
	  handlers 
	  deliver
	  (Resource.to_array socks_recv)
	  (Resource.to_array socks_xmit)
	  (Resource.to_array polls)
      in
      let block time =
	block time ;
	while poll () do () done
      and poll () =
	if poll () then (
	  while poll () do () done ;
	  true
	) else false
      and onlypoll () =
	if onlypoll false then (
	  while onlypoll false do () done ;
	  true
	) else false
      in
      blockr := block ;
      pollr := poll ;
      onlypollr := onlypoll
    ) else (
      let (block,poll,onlypoll) = 
	build_block_poll 
	  handlers
	  deliver
	  (Resource.to_array socks_recv)
	  (Resource.to_array socks_xmit)
	  (Resource.to_array polls)
      in
      blockr := block ;
      pollr := poll ;
      onlypollr := (fun _ -> onlypoll false)
    )
  ) ;
  
(*
  let ts_recv = Timestamp.add "REAL:recv" in
*)

  Trace.install_root (fun () -> [
    sprintf "REAL:recv:%s" (Resource.info socks_recv) ;
    sprintf "REAL:xmit:%s" (Resource.info socks_xmit) ;
    sprintf "REAL:poll:%s" (Resource.info polls)
  ]) ;

  let space = Time.mut () in

  let alarms = Priq.create (fun _ -> failwith "priq:sanity") in

  let block () =
    if Priq.size alarms = 0 then (
      Time.mut_set space Time.neg_one
    ) else (
      Time.mut_gettimeofday space ;
      let next = Priq.min alarms in
      if Time.mut_ge space next then (
      	Time.mut_set space Time.zero
      ) else (
      	Time.mut_sub_rev next space
      )
    ) ;
    match !blocker with
    | None -> !blockr space
    | Some f -> f (Time.mut_copy space)
  in

  let gettime = Time.gettimeofday

  and min () = Priq.min alarms

  and check () =
    (* This is a hack to avoid memory allocation
     * in normal case.
     *)
    Time.mut_gettimeofday space ;
    let min = Priq.min alarms in
    if not (Time.mut_ge space min) then
      false
    else
      (* We need to copy the timeval in case
       * there is an alarm because Priq.getopt
       * passes this value into the function.
       *)
      Priq.getopt alarms (Time.mut_copy space)

  and alarm callback =
    let disable = ident in
    let schedule time =
(*
      eprintf "REAL:Priq.size alarms = %d\n" (Priq.size alarms) ;
*)
      Priq.add alarms time callback
    in Alarm.c_alarm disable schedule

  and poll kind = 
    let fr =
      match kind with
      | Alarm.SocksPolls -> pollr
      | Alarm.OnlyPolls -> onlypollr
    in
    let actual_poll () = !fr () in
    actual_poll

  and add_sock_recv d s h = Resource.add socks_recv d (Hsys.int_of_socket s) (s,h)
  and rmv_sock_recv s = Resource.remove socks_recv (Hsys.int_of_socket s)
  and add_sock_xmit d s h = Resource.add socks_xmit d (Hsys.int_of_socket s) (s,(Hsys.Handler0 h))
  and rmv_sock_xmit s = Resource.remove socks_xmit (Hsys.int_of_socket s)
  and add_poll name poll = Resource.add polls name name poll
  and rmv_poll = Resource.remove polls

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
(*
Note: This has been moved to the Elink.module.
let _ = Alarm.install Alarm.Real alarm
*)

let _ = Alarm.install "REAL" alarm

(**************************************************************)
