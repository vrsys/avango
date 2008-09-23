(**************************************************************)
(* UDP.ML *)
(* Author: Mark Hayden, 5/95 *)
(**************************************************************)
open Util
open Trans
(**************************************************************)
let name = Trace.file "UDP"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
(**************************************************************)
(* NOTE: What about using an extra socket set to
 * no-local-receipt for sending multicasts on?  *)
(**************************************************************)

let init_sock host =
  let sock_buf = Arge.get Arge.sock_buf in
  let sock = Hsys.udp_socket sock_buf in

  let rec loop port =
    try
      Hsys.bind sock host port;
      port
    with e ->
      log (fun () -> Util.error e);
      loop (succ port)
  in

  let port = ref (Arge.check name Arge.port) in
  if !port = 0 then (
    (* Bind to any port and the specified address.
     *)
    port := Hsys.bind_any "UDP:recv" sock host
  ) else (
    port := loop !port
  );
  log (fun () -> sprintf "kernel bound port %d (receiving)" !port) ;
  (sock,!port)

(**************************************************************)

let domain alarm =
  let handlers = Alarm.handlers alarm in
  (* Note that deering port is always decided by the
   * configuration.  We should be able to bind to this port
   * without problems because all Ensemble processes use
   * reusable IP multicast sockets.  
   *)
  let deering_port = do_once (fun () -> Arge.check name Arge.deering_port) in
  let gossip_port = do_once (fun () -> Arge.check name Arge.gossip_port) in
  let gossip_hosts = do_once (fun () ->
      let hosts = Arge.check name Arge.gossip_hosts in
      let hosts = Arge.inet_of_string_list Arge.gossip_hosts hosts in
      Array.of_list hosts
    )
  in

  (* This allows the user to override the host name to use to
   * send messages to me.
   *)
  let host =
    match Arge.get Arge.udp_host with
    | None -> Hsys.gethost ()
    | Some host -> Arge.inet_of_string Arge.udp_host host
  in

  let localhost = Hsys.getlocalhost() in
  let map_localhost inet =
    if inet = host then localhost else inet
  in

  (* Initialize send and receive sockets.  Both are bound
   * to INADDR_ANY so that messages will be accepted on
   * all network interfaces.
   *)
  let (udp_sock, port) = init_sock (Hsys.inet_any ()) in

(*
  let xmit_sock_mc = do_once (fun () ->
    if not (Hsys.has_ip_multicast()) then
      failwith "multicast not supported" ;
    let sock = init_xmit_sock () in
    Hsys.setsockopt sock (Hsys.Multicast true) ;
    sock
  )
  in
*)

  (* Tell the alarm module our unique port number.
   *)
  Alarm.install_port port ;

  (* TODO: I'm having problems with the loopback stuff.
   *)
  let noloop = udp_sock in
(*
  let noloop = socket_dgram () in
  setsock_multicast noloop false ;
*)

  let udp_addr = Addr.UdpA(host,port) in

  (* Note: we give out the same port as for UDP.  This
   * is for sending messages pt2pt to me.
   *)
  let deering_addr () = Addr.DeeringA(host,port) in

  let addr mode = match mode with
  | Addr.Udp     -> udp_addr
  | Addr.Deering -> deering_addr ()
  | _ -> failwith "addr:bad mode"
  in

  let enable mode group endpt view =
    let ipmc_sock, disable =
      match mode with
      | Addr.Udp ->
	  Alarm.add_sock_recv alarm name udp_sock Hsys.Handler1 ;
	  let disable () = Alarm.rmv_sock_recv alarm udp_sock in
	  udp_sock, disable
      | Addr.Deering ->
	  let hash = Group.hash_of_id group in
	  let deering_sock = Ipmc.join (Hsys.deering_addr hash) (deering_port ()) in
	  Alarm.add_sock_recv alarm name udp_sock Hsys.Handler1 ;
	  Alarm.add_sock_recv alarm "DEERING" deering_sock Hsys.Handler1 ;

	  let disable () =
	    let hash = Group.hash_of_id group in
	    let deering_sock = Ipmc.leave (Hsys.deering_addr hash) (deering_port()) in
	    Alarm.rmv_sock_recv alarm udp_sock ;
	    Alarm.rmv_sock_recv alarm deering_sock
	  in

	  deering_sock, disable
      | _ -> failwith "enable:bad mode"
    in

    let xmit dest =
      (* Do some preprocessing.
       *)
      let dests =
	match dest with
	| Domain.Pt2pt dests ->
	    assert (not (Arrayf.is_empty dests)) ;

	    let sock = match mode with
	    | Addr.Deering
	    | Addr.Udp -> udp_sock
	    | _ -> failwith "xmit:sanity"
	    in

	    let dests =
	      Arrayf.map (fun dest ->
		match Addr.project dest mode with 
		| Addr.UdpA    (inet,port) -> (map_localhost inet,port)
		| Addr.DeeringA(inet,port) -> (map_localhost inet,port)
		| _ -> failwith "xmit:sanity"
	      ) dests
	    in
	    let dests = Arrayf.to_array dests in
	    Hsys.sendto_info sock dests
	| Domain.Mcast(dest,loopback) ->
	    (* Multicast communication means that this must
	     * be a Deering address.
	     *)
	    let hash = Group.hash_of_id dest in
	    let inet = Hsys.deering_addr hash in 
	    (*eprintf "UDP:ipmc_sock=%d (Mcast)\n" (Hsys.int_of_socket ipmc_sock) ;*)
	    Hsys.sendto_info ipmc_sock [|inet,deering_port()|]
	| Domain.Gossip(dest) ->
	    match mode with
	      (* For gossip messages with Deering, just broadcast
	       * it as above.
	       *)
	    | Addr.Deering ->
		let hash = Group.hash_of_id dest in
		let inet = Hsys.deering_addr hash in
		(*eprintf "UDP:ipmc_sock=%d (Gossip)\n" (Hsys.int_of_socket ipmc_sock) ;*)
		Hsys.sendto_info ipmc_sock [|inet,deering_port()|]

	      (* For Udp, we send the message to the various 
	       * gossip hosts.
	       *)
	    | Addr.Udp ->
		(* For this, we need to send using the udp_sock because
		 * the gossip server will use recvfrom to learn the
		 * address to reply to.
		 *)
		let gossip_port = gossip_port () in
                  let used_gossip_hosts =
                    if (!Arge.gossip_changed) then (* Added by Roy Friedman *)
                      Arge.get_new_gossip ()
                    else
                      gossip_hosts ()
                  in
	        let dests = Array.map (fun host -> (host,gossip_port)) used_gossip_hosts in

		Hsys.sendto_info udp_sock dests

	    | _ -> failwith "bad mode"
      in

      let x hdr ofs len iovl = 
	Hsys.sendtosv dests hdr ofs len iovl;
	Iovecl.free iovl
      in
      let x =
	if Arge.timestamp_check "send" then (
	  let stamp = Timestamp.register "UDP:xmit" in
	  let ts_xmit () = Timestamp.add stamp in
	  let x hdr ofs len iovl = ts_xmit () ; x hdr ofs len iovl in 
	  x
	) else x
      in Some x
    in

    log (fun () -> sprintf "ipmc_sock=%d" (Hsys.int_of_socket ipmc_sock));
    Domain.handle disable xmit
  in

  Domain.create name addr enable

(**************************************************************)

let _ = Domain.install Addr.Udp domain

(**************************************************************)
