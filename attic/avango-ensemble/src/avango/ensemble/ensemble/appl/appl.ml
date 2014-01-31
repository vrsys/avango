 (**************************************************************)
 (* APPL.ML *)
 (* Author: Mark Hayden, 4/95 *)
 (**************************************************************)
 open Util
 open View
 open Layer
 open Appl_intf
 open Buf
 (**************************************************************)
 let name = Trace.file "APPL"
 let failwith s = Trace.make_failwith name s
 let log = Trace.log name
 (**************************************************************)

 let unique_generator () = 
     let get_incarn () =
       (* This is an additional incarnation number.  It can be set
	* by the application.  If not, a 3-digit number is chosen
	* randomly based on the system clock.  
	*)
       let i = Time.gettimeofday () in
       let i = Time.to_string_full i in
       let i = Digest.string i in
       let i = Hashtbl.hash i in
       abs (i mod 1000)
     in

     (* The host is the IP address of this machine.  This should be
      * unique across processes on the system when used in
      * combination with a UDP port number that this process has
      * bound to.  
      *)
     let (host,incarn) =
       if Arge.get Arge.short_names then
	 (Hsys.inet_any (),0)
       else
	 (Hsys.gethost (),get_incarn ())
     in
     Unique.create host incarn

 let alarm_ref = ref None
 let alarm debug =
   log (fun () -> sprintf "appl state requested from %s" debug) ;
   match !alarm_ref with
   | Some a -> a
   | None -> 
       let handlers = Route.handlers () in
       let sched = Sched.create name in
       let async = Async.create sched in
       let unique = unique_generator () in
       let gorp = (unique,sched,async,handlers) in
       let alarm = Alarm.choose (Arge.get Arge.alarm) gorp in
       alarm_ref := Some alarm ;
       alarm

 (**************************************************************)

 let addr modes =
   let alarm = alarm name in
   let addr = 
     List.map (fun mode ->
       let dom = Domain.of_mode alarm mode in
       Domain.addr dom mode
     ) modes
   in 
   let addr = Arrayf.of_list addr in
   Addr.set_of_arrayf addr

 (**************************************************************)

 let full_info name endpt groupd protos protocol modes key =
   let force_modes = Arge.get Arge.force_modes in
   let pgp = Arge.get Arge.pgp in
   let id = Arge.get Arge.id in

   (* If ENS_ID environment variable is set, add that as
    * prefix of name so that users do not interact with each
    * other unless using same id.  
    *)
   let group = 
     let name = 
       if id = "" then name 
       else sprintf "%s:%s" id name
     in
     Group.named name 
   in

   let modes = 
     if not (List.mem Addr.Deering modes) 
     || Hsys.has_ip_multicast ()
     then modes else (
       eprintf "APPL:warning:DEERING mode unavailable (removing from modes)\n" ;
       except Addr.Deering modes
     )
   in

   if not force_modes then (
     if List.filter Addr.has_mcast modes = [] then (
       eprintf "APPL:no mcast modes were selected\n" ;
       Addr.error (Arrayf.of_list modes)
     ) ;

     if List.filter Addr.has_pt2pt modes = [] then (
       eprintf "APPL:no pt2pt modes were selected\n" ;
       Addr.error (Arrayf.of_list modes)
     )
   ) ;

   let addr = addr modes in
   let addr = Addr.arrayf_of_set addr in

   let addr = match Arge.get Arge.pgp with
   | None -> addr
   | Some nm ->
       let auth = Auth.lookup Addr.Pgp in
       let addr = Arrayf.append (Arrayf.singleton (Auth.principal auth Addr.Pgp nm)) addr in
       addr
   in

   let addr = Addr.set_of_arrayf addr in

   let alarm = alarm name in
   let uptime = Alarm.gettime alarm in

   let protos = Arrayf.singleton protos in

   let (ls,vs) = View.singleton key protocol group endpt addr uptime in
   let vs = View.set vs [Vs_groupd groupd;Vs_protos protos] in
   (ls,vs)

 (**************************************************************)
 (* Print out a warning once when stripping properties.
  *)
 let groupd_warning = do_once (fun () ->
   if not !quiet then
     eprintf "APPL:warning:stripping unnecessary properties for '-groupd'\n" ;
 )

 let default_info name =
   let alarm = alarm name in
 (*
   Trace.log "CONFIG" "default_info" (fun () -> Arge.get Arge.alarm_str) ;
 *)
   let groupd = Arge.get Arge.groupd in
   let protos = Arge.get Arge.protos in
   let modes = Arge.get Arge.modes in
   let properties = Arge.get Arge.properties in
   let key = Arge.get Arge.key in

   let key = match key with
   | None -> Security.NoKey
   | Some s -> Security.key_of_buf (Buf.of_string s)
   in

   (* Force linking of the Udp mode.  This causes
    * a port number to be installed in the 
    * Unique generator.
    *)
   let _ = Domain.of_mode alarm Addr.Udp in
   let endpt = Endpt.id (Alarm.unique alarm) in

   let properties =
     if groupd then (
       groupd_warning () ;
       Property.strip_groupd properties
     ) else properties
   in      

   let protocol = Property.choose properties in
   full_info name endpt groupd protos protocol modes key

 (* Same as above, but set all secure switches. 
 *)
 let set_secure (ls,vs) props = 
   if vs.groupd then 
     failwith "Cannot create a secure group that uses the groupd";
   if Arrayf.get vs.protos ls.rank then 
     failwith "Cannot create a secure group that uses the protos server";

   let key = match vs.key with
     | Security.NoKey -> 
	 Shared.Prng.create_key ()
     | _ -> vs.key 
   in
   let props  = props @ [Property.Auth;Property.Rekey;Property.Privacy] in
   let proto  = Property.choose props in
   let vs = View.set vs [Vs_key key; Vs_proto_id proto; 
			  Vs_params ["sec_sim", (Param.Bool false)]
   ] in
   ls,vs


 (**************************************************************)

 let exec names run =
   let appl_name = Sys.argv.(0) in            (* get executable's name *)
   let appl_name = Filename.basename appl_name in  (* chop directory name *)
   let appl_name =                            (* chop extension for nt *)
     try Filename.chop_extension appl_name 
     with Invalid_argument _ -> appl_name
   in
   let appl_name = String.lowercase appl_name in
   if List.mem appl_name names then (
     try
 (*
       Gc.full_major () ;
       List.iter (fun s ->
	 printf "APPL:exec:%s\n" s
       ) (string_list_of_gc_stat (Gc.stat ())) ;
 *)
 (*
       Sys.catch_break true ;
 *)
       run () ;
       exit 0 
     with 
     | Sys.Break ->
	 exit 0
     | exc ->
       let error_msg = Util.error exc in
       eprintf "ENSEMBLE:uncaught exception:%s\n" error_msg ;
       eprintf "  (Ensemble Version.id is %s)\n" (Version.string_of_id Version.id) ;
       raise exc
	 (*exit 1*)
   )

 (**************************************************************)

 let init_groupd = do_once (fun () ->
   let alarm = alarm name in
   let port = Arge.check name Arge.groupd_port in
   let hosts = Arge.check name Arge.groupd_hosts in
   let hosts = Arge.inet_of_string_list Arge.groupd_hosts hosts in
   let balance = Arge.get Arge.groupd_balance in
   let repeat = Arge.get Arge.groupd_repeat in
   let connect = Hsyssupp.connect in
   let sock = connect "groupd" port hosts balance repeat in
   Manage.groupd_proxy alarm sock
 )

 (**************************************************************)

 let socketpair () =
   let alarm = alarm name in
   let sched = Alarm.sched alarm in
   let q0 = Queuee.create () in
   let q1 = Queuee.create () in
   let r0_r = ref (fun _ -> failwith sanity) in
   let r1_r = ref (fun _ -> failwith sanity) in
   let r0 m = !r0_r m in
   let r1 m = !r1_r m in
   let s0 m =
     Sched.enqueue_1arg sched name r1 m
   in
   let s1 m =
     Sched.enqueue_1arg sched name r0 m
   in

   let c0 f =
     let r,_,s = f name s0 in
     r0_r := r ;
     s
   in

   let c1 f =
     let r,_,s = f name s1 in
     r1_r := r ;
     s
   in
   c0,c1

 (**************************************************************)

 let init_protos = do_once (fun () ->
   if Arge.get Arge.protos_test then (
     let alarm = alarm name in
     let client = Protos.client alarm in
     let config s vf =
       let glue = Arge.get Arge.glue in
       let ranking = Arge.get Arge.ranking in
       let s = Layer.new_state s in
       Stacke.config glue alarm ranking s vf
     in
     let server = Protos.server alarm addr config in
     let client_conn,server_conn = socketpair () in
     server_conn server ;
     client_conn client
   ) else (
     let alarm = alarm name in
     let port = Arge.check name Arge.protos_port in

     (* BUG: using groupd hosts, repeat, balance.
      *)
     let hosts = Arge.check name Arge.groupd_hosts in
     let hosts = Arge.inet_of_string_list Arge.groupd_hosts hosts in
     let balance = Arge.get Arge.groupd_balance in
     let repeat = Arge.get Arge.groupd_repeat in
     let connect = Hsyssupp.connect in
     let sock = connect "protos" port hosts balance repeat in
     let chan = Hsyssupp.client name alarm sock in
     let client = Protos.client alarm in
     let marsh,unmarsh = Iovecl.make_marsh true in

     chan (fun info send ->
       let send msg = send Buf.empty len0 len0 (marsh msg)
       in

       let recv,disable,state = client info send in

       let recv _ msg =
	 recv (unmarsh msg)
       in
       recv,disable,state
     )
   )
 )

 (**************************************************************)

 let config_new_full state ((ls,vs) as vf) =
   let alarm = alarm name in
   let protos = Arrayf.get vs.protos ls.rank in
   let glue = Arge.get Arge.glue in
   let ranking = Arge.get Arge.ranking in
 (* MH: I'm not sure that this is still needed.
   if vs.groupd && vs.protos. then
     failwith "config_new_full: both groupd and protos fields are set in view state" ;
 *)
   if vs.groupd then (
     let member = Stacke.config_full glue alarm ranking state in
     (init_groupd ()) vf member
   ) else if protos then (
     (init_protos ()) vf state
 (*
     (init_protos_test ()) vf state
 *)
   ) else (
 (*
     Gc.full_major () ;
     List.iter (fun s ->
       printf "APPL:config:b:%s\n" s
     ) (string_list_of_gc_stat (Gc.stat ())) ;
 *)    
     Stacke.config glue alarm ranking state vf ;
 (*
     Gc.full_major () ;
     List.iter (fun s ->
       printf "APPL:config:a:%s\n" s
     ) (string_list_of_gc_stat (Gc.stat ())) ;
 *)
   )

 let config_new interface vf =
   let interface = 
     if Trace.log_check "APPL_TRACE" then
       (Appl_debug.f name interface)
     else
       interface
   in
   let state = Layer.new_state interface in
   config_new_full state vf

 (**************************************************************)

 let start_monitor () =
   let alarm = alarm name in
   let log_gc = Trace.logl "GC" in
   let log_refcnt = Trace.logl "REFCNTM" in
   let log_heap = Trace.logl "HEAP" in
   let sched_r = ref None in
   let roots = Arge.get Arge.roots in
   let next = ref Time.zero in

   let do_monitor () =

     log_gc (fun () ->
 (*
       Gc.full_major () ;
 *)
       string_list_of_gc_stat (Gc.stat ())
     ) ;

     log_heap (fun () ->
       Heap.analyze () ;
       []
     ) ;

     if roots then
       Trace.print_roots ()
   in

   let loop time =
     if Arge.get Arge.debug_real then (
       let now = Time.gettimeofday () in
       if Time.ge now !next then (
	 next := Time.add now (Time.of_int 20) ;
	 do_monitor ()
       )
     ) else if Time.gt time Time.zero then (
       do_monitor ()
     ) ;
     Alarm.schedule (some_of name !sched_r) (Time.add time (Time.of_int 1))
   in

   sched_r := Some (Alarm.alarm alarm loop) ;
   loop Time.zero

 (**************************************************************)

 let async id =
   let alarm = alarm name in
   let async = Alarm.async alarm in
   Async.find async id

 (**************************************************************)

 let main_loop () =
   let alarm = alarm name in
   let sched = Alarm.sched alarm in
   let check_msgs = Alarm.poll alarm Alarm.SocksPolls in
   let check_timeouts = Alarm.check alarm in
   let pollcount = max 0 (Arge.get Arge.pollcount) in
   let count = Arge.get Arge.sched_step in
   if count <= 0 then failwith "non-positive scheduler counter" ;

   (* Compact the ML heap if it gets too large. This needs to be
    * further investigated.
    *)
   Arge.gc_compact 300;
  start_monitor () ;

  let counter = ref 0 in

  while true do
    while !counter <= pollcount do
      incr counter ;

      (* Schedule events in the layers.
       *)
      if Sched.step sched count then (
        counter := 0 ;
      ) ;

      (* Check for timeouts.
       *)
      if check_timeouts () then (
        counter := 0 ;
      ) ;

      (* Check for messages.
       *)
      if check_msgs () then (
        counter := 0 ;
      ) ;
    done ;
    assert (Sched.empty sched) ;
    counter := 0 ;
    
    (* If nothing to do then block.
     *)
    Alarm.block alarm ;
  done

(**************************************************************)

let main_loop_opt () =
  let alarm = alarm name in
  let sched = Alarm.sched alarm in
  let check_msgs = Alarm.poll alarm Alarm.OnlyPolls in
  let check_timeouts = Alarm.check alarm in

  let f () =
    while true do
      (* Check for messages.
       *)
      ignore (check_msgs ()) ;

      (* Schedule some events in the layers.
       *)
      ignore (Sched.step sched 200) ;

      (* Check for timeouts.
       *)
      ignore (check_timeouts ())
    done
  in
  
  Util.catch f ()

(**************************************************************)
open Trans

type cast_info = {
  mutable acct_recd : seqno ;		(* # delivered messages *)
  mutable acct_lost : seqno ;		(* # lost messages *)
  mutable acct_retrans  : seqno ;     	(* # retrans messages *)
  mutable acct_bad_ret  : seqno ;      	(* # retrans messages not used *)
  mutable acct_sent : seqno    	        (* # retrans messages sent by *)
}

let cast_info = {
  acct_recd    = 0 ;
  acct_lost    = 0 ;
  acct_retrans = 0 ;
  acct_bad_ret = 0 ;
  acct_sent    = 0
} 

(**************************************************************)

