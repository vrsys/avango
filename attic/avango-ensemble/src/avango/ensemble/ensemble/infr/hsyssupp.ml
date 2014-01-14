(**************************************************************)
(* HSYSSUPP.ML *)
(* Author: Mark Hayden, 6/96 *)
(* Rewritten by Ohad Rodeh 10/2001 *)
(**************************************************************)
(* This modules is used for managing TCP connections.
 * It provides support for sending and receiving packets
 * over streams.  It takes into account the possibility
 * that the send and receive operations will not send
 * data in increments of complete packets. 
 * 
 * The following state-machine is used:
 * 1. Read the first 8 bytes, convert into [ml_len] [iovec_len]
 * 2. Allocate the ml string, and the iovec. 
 * 3. Read into them. 
 * 4. deliver the packet (buf,iovec).
 *
 * The code is optimized for the normal case where
 * the ML header is short, and the iovecl is non-empty. 
 *
 * The send function take a precomputed header, that is of the form
 *  [ml_len iovl_len], and is 8 bytes long.
 *)
(**************************************************************)
open Trans
open Util
open Buf
(**************************************************************)
let name = Trace.file "HSYSSUPP"
let failwith = Trace.make_failwith name
let log = Trace.log name
let log2 = Trace.log (name^"2")
(**************************************************************)

type 'a conn =
    debug -> 
      (Buf.t -> ofs -> len -> Iovecl.t -> unit) -> (* A send function *)
	((Buf.t -> Iovecl.t -> unit) (* A receive function *)
	* (unit -> unit)    (* A disable function *)
	* 'a)               (* An application state *)

type recv_state = 
    R_Init
  | R_Len                        (* Read the first 8 bytes *)
  | R_Hdr of (Buf.t * Iovec.t)   (* Read the complete ML header *)
  | R_Iovec of (Buf.t * Iovec.t) (* Read the Iovec *)

type send_state = 
    S_Null
  | S_Init of Buf.t * Iovecl.t (* Send the full packet *)
  | S_Iovec of Iovecl.t        (* Just the Iovec *)


type state = {
  mutable recv_s             : recv_state ;
  mutable recv_total_hdr_len : len ;
  mutable recv_total_iov_len : len ;
  mutable recv_cur_hdr_len   : len ;
  mutable recv_cur_iov_len   : len ;
  scribble                   : Buf.t ;  (* Temporary scribbling area. *)

  mutable send_s             : send_state ;

  (* Queue of messages to send *)
  q                          : (Buf.t * Buf.t * ofs * len * Iovecl.t) Queuee.t; 
  mutable connected : bool 
}

let create_state () = {
  recv_s = R_Init ;
  recv_total_hdr_len = len0;
  recv_total_iov_len = len0;
  recv_cur_hdr_len   = len0;
  recv_cur_iov_len   = len0;
  scribble = Buf.create len8;

  send_s = S_Null ;

  q      = Queuee.create ();
  connected = true 
} 

let max_msg_len =  Buf.max_msg_len

let tcp debug alarm sock deliver disable =
  Util.disable_sigpipe () ;
  let debug = "tcp" in 
  let s = create_state () in

  let select_info_write =
    let sa = [|sock|] in
    let ba = [|false|] in
    Hsys.select_info None (Some(sa,ba)) None
  in
  let select_info_read =
    let sa = [|sock|] in
    let ba = [|false|] in
    Hsys.select_info (Some(sa,ba)) None None
  in

  let disable () =
    log (fun () -> "Disable");
    s.connected <- false ;
    Queuee.iter (fun (_,_,_,_,iovl) -> Iovecl.free iovl) s.q;
    begin match s.recv_s with 
	R_Hdr (_,iov)
      | R_Iovec(_,iov) -> Iovec.free iov
      | _ -> ()
    end;
    begin match s.send_s with 
	S_Null -> ()
      | S_Init (_,iovl)
      | S_Iovec iovl -> Iovecl.free iovl
    end;
    Alarm.rmv_sock_recv alarm sock ;
    if s.send_s <> S_Null then (
      Alarm.rmv_sock_xmit alarm sock
    ) ;
    Hsys.close sock;
    disable ()
  in


  (* Deliver a packet, and cleanup.
   *)
  let wrap_deliver hdr iov = 
    log2 (fun () -> sprintf "wrap_deliver hdr_len=%d iov_len=%d" 
      (int_of_len (Buf.length hdr)) (int_of_len (Iovec.len iov)));
    s.recv_total_hdr_len <- len0 ;
    s.recv_total_iov_len <- len0 ;
    s.recv_cur_hdr_len <- len0 ;
    s.recv_cur_iov_len <- len0 ;	
    s.recv_s <- R_Init;
    deliver hdr (Iovecl.singleton iov)
  in
      
  let check_len0 len = 
    if len =|| len0 then (
      log2 (fun () -> sprintf "HSYSSUPP:0 return on receive[1]") ;
      disable ()
    ) 
  in

  let  recv () =
    if s.connected then (
      let read_all_first_8_bytes () = 
	let ml_len = len_of_int (Buf.read_int32 s.scribble len0) in
	let iov_len = len_of_int (Buf.read_int32 s.scribble len4) in
	log2 (fun () -> 
	  sprintf "read_all_first_8_bytes (ml_len=%d, iov_len=%d)" 
	  (int_of_len ml_len) (int_of_len iov_len));
	if ml_len +|| iov_len >|| max_msg_len then 
	  failwith (sprintf 
	    "sanity: TCP Message too long (%d+%d)" 
	    (int_of_len ml_len) (int_of_len iov_len)
	  );
	let hdr = Buf.create ml_len in
	let iov = Iovec.alloc iov_len  in

	(* Try to receive a full packet at once. 
	 *
	 * PERF: we may need to add a special case for when
	 *      the ml header is empty.
	 *)
	if Hsys.poll select_info_read > 0 then (
	  let len =  Hsys.tcp_recv_packet sock hdr len0 ml_len iov in
	  log2 (fun () -> sprintf "tcp_recv_pakcet len=%d" (int_of_len len));
	  if len =|| len0 then (
	    Iovec.free iov;
	    check_len0 len;
	  ) else
	    if len =|| ml_len +|| iov_len then (
	      wrap_deliver hdr iov
	    ) else (
	      if len <|| ml_len then (
		s.recv_total_hdr_len <- ml_len ;
		s.recv_total_iov_len <- iov_len ;
		s.recv_cur_hdr_len <- len ;
		s.recv_cur_iov_len <- len0 ;	
		s.recv_s <- R_Hdr(hdr,iov);
	      ) else (
		s.recv_total_iov_len <- iov_len ;
		s.recv_cur_iov_len <- len -|| ml_len ;	
		s.recv_s <- R_Iovec(hdr,iov);
	      )
	    )
	) else (
	  s.recv_total_hdr_len <- ml_len ;
	  s.recv_total_iov_len <- iov_len ;
	  s.recv_cur_hdr_len <- len0 ;
	  s.recv_cur_iov_len <- len0 ;	
	  s.recv_s <- R_Hdr(hdr,iov);
	)
      in
      match s.recv_s with 
	  R_Init -> 
	    log (fun () -> "R_Init");
	    let len = Hsys.tcp_recv sock s.scribble len0 len8 in
	    check_len0 len;
	    if len =|| len8 then (
	      read_all_first_8_bytes () 
	    ) else (
	      s.recv_total_hdr_len <- len8 ;
	      s.recv_cur_hdr_len <- len ;
	      s.recv_s <- R_Len;
	    )
	| R_Len -> 
	    log (fun () -> "R_Len");
	    let remain = len8 -|| s.recv_cur_hdr_len in
	    let len = Hsys.tcp_recv sock s.scribble s.recv_cur_hdr_len remain in
	    check_len0 len;
	    if len =|| remain then (
	      s.recv_total_hdr_len <- len0 ;
	      s.recv_cur_hdr_len <- len0 ;
	      read_all_first_8_bytes ()
	    ) else
	      s.recv_cur_hdr_len <- s.recv_cur_hdr_len +|| len
	| R_Hdr(hdr,iov) -> 
	    log (fun () -> "R_Hdr");
	    let remain = s.recv_total_hdr_len -|| s.recv_cur_hdr_len in
	    let len = Hsys.tcp_recv sock hdr s.recv_cur_hdr_len remain in
	    check_len0 len;
	    if len =|| remain +|| Iovec.len iov then (
	      wrap_deliver hdr iov
	    ) else (
	      assert (len<=||remain);
	      if len =|| remain then (
		s.recv_s <- R_Iovec(hdr,iov) ;
	      ) else (
		s.recv_cur_hdr_len <- s.recv_cur_hdr_len +|| len 
	      )
	    )
	| R_Iovec(hdr,iov) -> 
	    log (fun () -> "R_Iovec");
	    let remain = s.recv_total_iov_len -|| s.recv_cur_iov_len in
	    let len = Hsys.tcp_recv_iov sock iov s.recv_cur_iov_len remain in
	    check_len0 len;
	    if len =|| remain then (
	      wrap_deliver hdr iov
	    ) else (
	      s.recv_cur_iov_len <- s.recv_cur_iov_len +|| len 
	    )
    )
  in

  
  (* Append two strings, in a slightly more efficient way
   *)
  let stitch buf1 buf2 ofs len = 
    let len1 = Buf.length buf1 in
    let sum = Buf.create (len1 +|| len) in
    Buf.blit buf1 len0 sum len0 len1 ;
    Buf.blit buf2 ofs sum len1 len;
    sum
  in
  
  (* Try to send a complete packet. If unseccussful, set
   * the s.send_s (send_state) to the packet remains.
   * hdr is of length 8. Return true if the packet was 
   * send successfuly, false otherwise.
   *)
  let try_send hdr buf ofs len iovl = 
    (* Optimistic case, try to send everything at once. 
     *)
    log (fun () -> sprintf "try_send ml_len=%d iov_len=%d" (int_of_len len)
      (int_of_len (Iovecl.len iovl)));
    let sent_len = Hsys.sends2v_p sock hdr buf ofs len iovl in

    if sent_len =|| len8 +|| len +|| Iovecl.len iovl then (
      Iovecl.free iovl;
      true
    ) else (
      log (fun () -> sprintf "try_send: Exceptional condition, len=%d+%d+%d sent=%d"
	(int_of_len len8) (int_of_len len) (int_of_len (Iovecl.len iovl)) 
	(int_of_len sent_len));
      let total_hdr_len = len8 +|| len in
      if sent_len <|| total_hdr_len then (
	let hdr = stitch hdr buf ofs len in
	let hdr = Buf.sub hdr sent_len (total_hdr_len -|| sent_len) in
	s.send_s <- S_Init(hdr, iovl)
      ) else (
	let sent_len = sent_len -|| total_hdr_len in
	let iovl' = Iovecl.sub iovl sent_len (Iovecl.len iovl -|| sent_len) in
	Iovecl.free iovl;
	s.send_s <- S_Iovec iovl'
      );
      false
    )
    in
      
  (* A function that is woken up when we can write to the socket. 
   * Complete any pending writes. 
   *
   * We do not optimized this case too eagerly. We only optimize
   * the simple case, where transmissions are successful the first
   * try. 
   *)
  let writer () = 
    let rec send_loop () = 
      if Queuee.empty s.q then (
	log2 (fun () -> sprintf "rmv_sock_xmit") ;
	s.send_s <- S_Null ;
	Alarm.rmv_sock_xmit alarm sock
      ) else (
	let (hdr1,hdr2,ofs2,len2,iovl) = Queuee.take s.q in
	if try_send hdr1 hdr2 ofs2 len2 iovl then send_loop ()
      ) 
    in
    if s.connected then (
      match s.send_s with
	  S_Null -> 
	    log (fun () -> sprintf "HSYSSUPP:warning")
	| S_Init (hdr,iovl) ->
	    let hdr_len = Buf.length hdr in
	    let len = Hsys.sendsv_p sock hdr len0 hdr_len iovl in
	    if len =|| hdr_len +|| Iovecl.len iovl then (
	      (* Success, xmit next packet 
	       *)
	      Iovecl.free iovl;
	      s.send_s <- S_Null ;
	      send_loop ()
	    ) else (
	      if len <|| hdr_len then (
		let hdr' = Buf.sub hdr len (hdr_len -|| len) in
		s.send_s <- S_Init(hdr', iovl)
	      ) else (
		let len = len -|| hdr_len in
		let iovl' = Iovecl.sub iovl len (Iovecl.len iovl -|| len) in
		Iovecl.free iovl;
		s.send_s <- S_Iovec iovl'
	      )
	    )
	| S_Iovec iovl -> 
	    let len = Hsys.sendv_p sock iovl in
	    if len =|| Iovecl.len iovl then (
	      (* xmit next packet 
	       *)
	      Iovecl.free iovl;
	      s.send_s <- S_Null ;
	      send_loop ()
	    ) else (
	      let iovl' = Iovecl.sub iovl len (Iovecl.len iovl -|| len) in
	      Iovecl.free iovl;
	      s.send_s <- S_Iovec(iovl')
	    )
    )
  in

  (* Prepare a sending header. Format: [ml_len] [iovecl_len]
   *)
  let prepare_header molen iovl = 
    let scribble = Buf.create len8 in
    Buf.write_int32 scribble len0 (int_of_len molen);
    Buf.write_int32 scribble len4 (int_of_len (Iovecl.len iovl));
    scribble
  in
    
  let send buf ofs len iovl =
    if not s.connected then (
      Iovecl.free iovl
    ) else (
      let hdr = prepare_header len iovl in
      match s.send_s with
	  S_Null -> 
	    (* Optimistic case, try to send everything at once. 
	     *)
	    if Hsys.poll select_info_write > 0 then (
	      if try_send hdr buf ofs len iovl then (
		(* We managed to write the complete packet. Do 
		 * nothing
		 *)
	      ) else (
		(* Put a watch-dog, so that when we are able
		 * to write to the socket, the operation will contine.
		 *)
		s.send_s <- S_Init(stitch hdr buf ofs len, iovl);
		Alarm.add_sock_xmit alarm debug sock writer
	      )
	    ) else (
	      s.send_s <- S_Init(stitch hdr buf ofs len, iovl);
	      Alarm.add_sock_xmit alarm debug sock writer
	    ) 
	| _ -> 
	    Queuee.add (hdr,buf,ofs,len,iovl) s.q	    
    )
  in
  Alarm.add_sock_recv alarm debug sock (Hsys.Handler0 recv) ;
  send

(**************************************************************)

let server debug alarm port client =
  let sock = Hsys.socket_stream () in
  Hsys.setsockopt sock Hsys.Reuse ;

  if not !quiet then
    eprintf "%s:server binding to port %d\n" debug port ;
  begin
    try Hsys.bind sock (Hsys.inet_any()) port with e ->
      (* Don't bother closing the socket.
       *)
      if not !quiet then (
	eprintf "%s:error when binding to port\n" debug ;
	eprintf "  (this probably means that a groupd server is already running)\n" ;
      ) ;
      Hsys.close sock ;
      raise e
  end ;
  Hsys.listen sock 5 ;
  if not !quiet then
    eprintf "%s:server installed\n" debug ;

  let svr_handler () =
    let sock,inet,port = Hsys.accept sock in
    let info = sprintf "{inet=%s;port=%d}"
	(Hsys.string_of_inet_nums inet) port
    in
    let connected = ref true in

    let recv_r = ref (fun _ _ -> failwith sanity) in
    let recv hdr iov = !recv_r hdr iov in
    let disable_r = ref (fun _ -> failwith sanity) in
    let disable () = !disable_r () in

    let send = tcp debug alarm sock recv disable in

    let recv,disable,() = client info send in

    recv_r := recv ;
    disable_r := disable
  in
  
  let debug = sprintf "%s(server)" debug in
  Alarm.add_sock_recv alarm debug sock (Hsys.Handler0 svr_handler)

(**************************************************************)

let client debug alarm sock serv_init =
  let recv_r = ref (fun _ _ -> failwith sanity) in
  let recv hdr iov = !recv_r hdr iov in
  let disable_r = ref (fun _ -> failwith sanity) in
  let disable () = !disable_r () in

  let send = tcp debug alarm sock recv disable in
  let recv,disable,state = serv_init debug send in
  recv_r := recv ;
  disable_r := disable ;
  state

(**************************************************************)

let random_list l =
  let l = Array.of_list l in
  let n = Array.length l in
  for i = 1 to pred n do
    let j = Random.int i in 
    let tmp = l.(i) in
    l.(i) <- l.(j) ;
    l.(j) <- tmp
  done ;
  Array.to_list l

let connect debug port hosts balance repeat =
  if !verbose then
    eprintf "HSYSSUPP:connect:using port %d for %s server\n" port debug ;
  let try_connect host =
    let sock = Hsys.socket_stream () in
    if !verbose then
      eprintf "HSYSSUPP:connect:attempting connection to %s server at %s\n" debug (Hsys.string_of_inet host) ; 
    try
      Hsys.connect sock host port ;
      if !verbose then
        eprintf "HSYSSUPP:connect:connected to %s server\n" debug ;
      Some sock
    with e ->
      if !verbose then
        eprintf "HSYSSUPP:connect:connection to %s server failed (%s)\n" debug (Util.error e) ;
      Hsys.close sock ;
      None
  in

  let host = Hsys.gethost () in
  let localhost = Hsys.inet_of_string "localhost" in

  (* Randomize the list of hosts for load balancing.
   *)
  let hosts = 
    if balance then
      random_list hosts
    else 
      hosts
  in
  
  (* Strip out my host name from the list and then add it to
   * the front.
   *)
  let hosts = localhost :: (except host hosts) in
  let sock =
    let rec loop = function
    | [] -> None
    | hd::tl ->
        if_none (try_connect hd) (fun () -> loop tl)
    in loop hosts
  in

  (* Try locally some more, if requested to do so.
   *)
  let sock = 
    match sock with
    | None ->
	if repeat then (
	  eprintf "HSYSSUPP:connect:no %s server found, trying locally some more times\n" debug ;

	  let rec loop attempts =
	    if attempts <= 0 then None else (
	      (* Sleep for 1 second between tries.
	       *)
	      ignore (Time.select (Hsys.select_info None None None) (Time.of_int 1)) ;
	      match try_connect localhost with
	      | None -> loop (pred attempts)
	      | s -> s
	    )
	  in loop 20 (* Try 20 times. *)
	) else None
    | s -> s
  in
                  
  match sock with 
  | None ->
      let hosts = List.map (Hsys.string_of_inet) (host::hosts) in
      let hosts = string_of_list ident hosts in
      eprintf "HSYSSUPP:connect:no %s server found at %s, exiting\n" debug hosts ;
      exit 1
  | Some sock ->
      sock

(**************************************************************)

