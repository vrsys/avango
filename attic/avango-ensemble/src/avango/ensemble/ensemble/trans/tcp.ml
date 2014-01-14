(**************************************************************)
(* TCP.ML *)
(* Author: Mark Hayden, 7/96 *)
(**************************************************************)
open Util
open Trans
open Buf
(**************************************************************)
let name = Trace.file "TCP"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
(**************************************************************)
(* Notes:

 * BUG: need to collect unused sockets.

 *)
(**************************************************************)
(* Set up a TCP socket.
 *)

let init host =
  (* Create a TCP stream socket.
   *)
  let sock =
    try Hsys.socket_stream () with e ->
      eprintf "TCP:error creating socket:%s, exiting\n" (Util.error e) ;
      exit 1
  in

  (* Bind to any port.
   *)
  let port = Hsys.bind_any name sock host in
  log (fun () -> sprintf "TCP server port %d" port) ;
  
  begin
    try Hsys.listen sock 5 with e ->
      eprintf "TCP:error listening to socket:%s, exiting\n" (Util.error e) ;
      exit 1
  end ;

  (sock,port)

(**************************************************************)

type conn = {
  mutable sock : Hsys.socket option ;
  mutable peer : (inet * port) option ;
  mutable send : (Buf.t -> Buf.ofs -> Buf.len -> Iovecl.t -> unit) option ;
  mutable access : Time.t
}

(**************************************************************)

let domain alarm =
  let handlers = Alarm.handlers alarm in
  let conns = 
    let table = Hashtbl.create (*name*) 10 in
    Trace.install_root (fun () ->
      [sprintf "TCP:#conns=%d\n" (hashtbl_size table)]
    ) ;
    table
  in
      
  let nconns = ref 0 in
  let host = Hsys.gethost () in
  let (server, port) = init (Hsys.inet_any()) in

  let break c =
    log (fun () -> "breaking connection") ;
    c.sock <- None ;
    c.send <- None
  in

  let disable c =
    break c ;
    if_some c.peer (fun peer ->
      Hashtbl.remove conns peer
    )
  in

  let find addr =
    let conn =
      match addr with
      |	None -> None
      |	Some addr ->
	  try Some (Hashtbl.find conns addr) 
	  with Not_found -> None
    in
    
    match conn with 
    | Some c -> c
    | None ->
      	let conn = {
	  peer = addr ;
      	  sock = None ;
      	  send = None ;
	  access = Alarm.gettime alarm
      	} in
	if_some addr (fun addr ->
      	  Hashtbl.add conns addr conn ;
	) ;
	conn
  in

  let install c =
    match c.sock with
    | None -> failwith "install_conn:sanity"
    | Some sock ->
	let client info send =
	  let recv buf iovl =
	    Route.deliver handlers buf len0 (Buf.length buf) iovl
	  in
	  let disable () =
	    log (fun () -> sprintf "lost connection to %s"
	        (string_of_option (fun (inet,port) ->
		  sprintf "%s:%d" (Hsys.string_of_inet inet) port) c.peer)) ;
	    break c
	  in

	  if_some c.peer (fun peer ->
	    c.send <- Some send ;
	    Hashtbl.add conns peer c
	  ) ;

	  (recv,disable,())
	in

	Hsyssupp.client name alarm sock client
  in

  let connect c =
    match c.sock,c.peer with
    | None,Some(inet,port) ->
	log (fun () -> sprintf "connecting to {%s:%d}" (Hsys.string_of_inet inet) port) ;
      	let sock = Hsys.socket_stream () in
      	begin try
	  Util.disable_sigpipe () ;
	  Hsys.connect sock inet port ;
	  c.sock <- Some sock ;
	  install c ;
	with e ->
	  log (fun () -> sprintf "error:%s" (Util.error e)) ;
	  Hsys.close sock
	end
    | _,_ -> failwith "connect:sanity" ;
  in

  (* Accept a connection on the server socket.
   *)
  let accept _ =
    try
      log (fun () -> sprintf "accepting connection") ;
      Util.disable_sigpipe () ;
      let conn = find None in
      let (sock,_,_) = Hsys.accept server in
      conn.sock <- Some sock ;
      install conn
    with e ->
      log (fun () -> sprintf "error:%s" (Util.error e)) ;
      failwith "error occurred while accepting"
  in

  let addr = Addr.TcpA(host,port) in
  let addr _ = addr in

  let enable  _ _ _ _ =
    Alarm.add_sock_recv alarm (name^":server") server (Hsys.Handler0 accept) ;

    let disable () =
      Alarm.rmv_sock_recv alarm server

    and xmit dest =
(*
      eprintf "TCP:enable:dest=%s\n" (Domain.string_of_dest dest) ;
*)

      (* Do some preprocessing.
       *)
      let conns =
	match dest with
	| Domain.Mcast _ -> failwith "mcast not supported"
	| Domain.Gossip _ -> failwith "gossip not supported"
	| Domain.Pt2pt dests ->
	    Arrayf.map (fun dest ->
	      match Addr.project dest Addr.Tcp with
	      | Addr.TcpA(inet,port) -> find (Some(inet,port))
	      | _ -> failwith "xmit:sanity"
	    ) dests
      in

      if Arrayf.is_empty conns then (
	None 
      ) else (
	let x hdr hdr2 len iovl =
	  for i = 0 to pred (Arrayf.length conns) do
	    (* Increate the refcount for every connection we send
	     * on (except the first).
	     *)
	    let iovl = 
	      if i=0 then iovl 
	      else Iovecl.copy iovl
	    in
	    match Arrayf.get conns i with
	    | {send=Some send} -> 
		send hdr hdr2 len iovl
	    | {sock=None;peer=Some _} as c ->
		(* If we have a name, but no socket, then
		 * try connecting.
		 *)
		connect c ;
		if_some c.send (fun send ->
		  send hdr hdr2 len iovl) ;

		(*
		 * Hsyssupp already frees the iovecl.
		 *)
		  (* Iovecl.free iovl *) 
	    | _ -> ()
	  done 
	in

	let x =
      	  if Arge.timestamp_check "send" then (
	    let stamp = Timestamp.register "TCP:xmit" in
	    let ts_xmit () = Timestamp.add stamp in
	    let x hdr hdr2 len iovl = ts_xmit () ; x hdr hdr2 len iovl in 
	    x
	  ) else x
	in Some x
      )
    in
    Domain.handle disable xmit
  in

  Domain.create name addr enable

(**************************************************************)

let _ = Domain.install Addr.Tcp domain

(**************************************************************)
