(**************************************************************)
(* REFLECT.ML: server side of gossip transport *)
(* Author: Mark Hayden, 8/95 *)
(**************************************************************)
open Util
open View
open Appl_intf open New
(**************************************************************)
let name = Trace.file "REFLECT"
let failwith = Trace.make_failwith name
let log = Trace.log name
(**************************************************************)

(* This is the amount of time for which we will forward
 * gossip to another process without having heard from it.
 *)
let remove_timeout = Time.of_int 20

(* This is time before we will forward the same message
 * another time.
 *)
let repeat_timeout = Time.of_string "0.1"

let max_msg_len = Buf.int_of_len Buf.max_msg_len
  
  
let create_server_sock port = 
  (* Create datagram socket.
   *)
  let sock = Hsys.socket_dgram () in
  
  (* Try to disable error ICMP error reporting.
   *)
  begin try
    Hsys.setsockopt sock (Hsys.Bsdcompat true)
  with e ->
    log (fun () -> sprintf "warning:setsockopt:Bsdcompat:%s" (Util.error e))
  end ;
  
  (* Bind it to the gossip port.
   *)
  begin
    try Hsys.bind sock (Hsys.inet_any()) port with e ->
      (* Don't bother closing the socket.
       *)
      if not !quiet then (
	printf "REFLECT:error when binding to port\n" ;
	printf "  (this probably means that a gossip server is already running)\n" ;
      ) ;
      raise e
  end ;
  sock

(* This function returns the interface record that defines
 * the callbacks for the application.
 *)
let intf alarm initial_sock port = 
  let clients = Hashtbl.create (*name*) 10 in

  let recv_gossip_r = ref (fun () -> ()) in
  let recv_gossip () = !recv_gossip_r () in

  let sock_r = ref initial_sock in
  Alarm.add_sock_recv alarm name !sock_r (Hsys.Handler0 recv_gossip) ;

  let replace_socket () = 
    printf "REFLECT:server socket closed due to errors, creating a new one\n"; 
    Alarm.rmv_sock_recv alarm !sock_r ;
    Hsys.close !sock_r;
    let new_sock = create_server_sock port in
    sock_r := new_sock ;
    Alarm.add_sock_recv alarm name new_sock (Hsys.Handler0 recv_gossip) ;
  in

  let install (ls,vs) =
    if not !quiet then
      printf "REFLECT:view:%d:%s\n" ls.nmembers (View.to_string vs.view) ;
    let async = Appl.async (vs.group,ls.endpt) in
    let actions = Queuee.create () in
    let reflect (inet,port) msg =
      if ls.am_coord then (
	let time = Alarm.gettime alarm in
	let _,time',digest' = 
	  try Hashtbl.find clients (inet,port) with Not_found ->
	    let sendinfo = Hsys.sendto_info !sock_r [|inet,port|] in
	    let digest = Buf.digest_sub Buf.empty Buf.len0 Buf.len0 in
	    let info = (sendinfo,(ref time),(ref digest)) in
	    Hashtbl.add clients (inet,port) info ;
	    info
	in
	let digest = Buf.digest_sub msg Buf.len0 (Buf.length msg) in
	
	if digest <> !digest' 
        || Time.ge time (Time.add !time' repeat_timeout)
	then (
	  digest' := digest ;
	  time'   := time ;
	  let dests = hashtbl_to_list clients in
	  List.iter (fun ((inet,port),(info,time',_)) ->
	    if Time.ge time (Time.add !time' remove_timeout) then (
	      if !verbose then
		printf "REFLECT:removing          (%s,%d)\n" 
		  (Hsys.string_of_inet inet) port ;
	      Hashtbl.remove clients (inet,port)
	    ) else (
	      if !verbose then
		printf "REFLECT:send_gossip: to   (%s,%d)\n" 
		  (Hsys.string_of_inet inet) port ;
	      ignore (Hsys.sendto info msg Buf.len0 (Buf.length msg))
	    )
	  ) dests
	)
      ) else (
	if Queuee.empty actions then
	  async () ;
	Queuee.add (Send([|vs.coord|],((inet,port),msg))) actions ;
      )
    in
      
    let recv_gossip =
      let buf = Buf.create Buf.max_msg_len in
      fun () ->
	if !verbose then 
	  printf "REFLECT:#clients=%d\n" (hashtbl_size clients) ;
	try
          let (len,inet,port) = Hsys.recvfrom !sock_r buf Buf.len0 Buf.max_msg_len in
	  if !verbose then (
	    printf "REFLECT:recv_gossip: from (%s,%d)\n" 
	      (Hsys.string_of_inet inet) port ;
	    ) ;
	  let msg = Buf.sub buf Buf.len0 len in
	  (*printf "REFLECT:sending msg_len=%d\n" (Buf.int_of_len len);*)
	  reflect (inet,port) msg
	with (Unix.Unix_error(err,s1,s2) as exn)-> 
	  match err with 
	    | Unix.ECONNREFUSED 
	    | Unix.ECONNRESET 
	    | Unix.EHOSTDOWN	
	    | Unix.ENOENT	
	    | Unix.ENETUNREACH
	    | Unix.EHOSTUNREACH	
	    | Unix.EPIPE 
	    | Unix.EINTR	
	    | Unix.EAGAIN 
	    | Unix.EFAULT              (** Bad address *)
	    | Unix.EBADF ->
		if !verbose then
		  printf "REFLECT:warning:%s\n" (Util.error exn)
	    | _ -> 
		printf "Unix.error=%s" (Util.error exn);
		replace_socket ()
    in
    recv_gossip_r := recv_gossip ;

    let receive origin bl cs =
      match bl,cs with
      |	U,S -> (
	  fun (from,msg) ->
	    if !verbose then
	      printf "REFLECT:forwarded:origin=%s gossiper=(%s,%d)\n" 
	      	(Endpt.string_of_id (Arrayf.get vs.view origin))
	      	(Hsys.string_of_inet (fst from)) (snd from) ;
	    reflect from msg ;
	    [||]			(*BUG*)
	)
      |	_ -> null
    in
    let block () = [||] in
    
    let heartbeat time =
      let sactions = Queuee.to_list actions in
      Queuee.clear actions ;
      let actions = Array.of_list sactions in
      actions
    in
    
    let handlers = { 
      flow_block = (fun _ -> ());
      heartbeat = heartbeat ;
      receive = receive ;
      block = block ;
      disable = Util.ident
    } in
    [||],handlers
  in

  let exit () = () in

  {
    heartbeat_rate = Time.of_int 1 ;
    install = install ;
    exit = exit
  }

(**************************************************************)

let init alarm (ls,vs) port force =
  (* Figure out port information and bind to it.
   *)
  let host = Hsys.gethost () in
  let localhost = Hsys.inet_of_string "localhost" in
  if not !quiet then
    printf "REFLECT:server starting on %s\n" (Hsys.string_of_inet host) ;

  let hosts = Arge.check name Arge.gossip_hosts in
  let hosts = Arge.inet_of_string_list Arge.gossip_hosts hosts in

  (* Check that I'm in my list of gossip hosts.
   *)
  if (not force) 
  && (not (List.mem host hosts)) 
  && (not (List.mem localhost hosts)) 
  then (
    if not !quiet then (
      printf "REFLECT:I'm not in the list of gossip hosts, exiting\n" ;
      printf "  (the hosts are %s)\n" 
        (string_of_list Hsys.string_of_inet hosts) ;
    ) ;
    exit 1
  ) ;

  if not !quiet then
    printf "REFLECT:server binding to port %d\n" port ;


  if not !quiet then
    printf "REFLECT:server ready\n" ;

  let sock = create_server_sock port in

  (*
   * Initialize the application interface.
   *)
  let interface = intf alarm sock port in

  (* Ensure that groupd is unset.
   *)
  let properties = Property.vsync in
  let protocol = Property.choose properties in
  let vs = View.set vs [
    Vs_groupd false;
    Vs_proto_id protocol
  ] in
  let ls = View.local name ls.endpt vs in
  let interface = full interface in
  ((ls,vs),interface)

(**************************************************************)

