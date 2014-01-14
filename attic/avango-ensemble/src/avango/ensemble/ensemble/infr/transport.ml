(**************************************************************)
(* TRANSPORT.ML *)
(* Author: Mark Hayden, 7/95 *)
(**************************************************************)
open Util
open Trans
open View
(**************************************************************)
let name = Trace.file "TRANSPORT"
let failwith = Trace.make_failwith name
let log = Trace.log name
(**************************************************************)

let array_split3 abc =
  let a = Arrayf.map (fun (x,_,_) -> x) abc in
  let b = Arrayf.map (fun (_,x,_) -> x) abc in
  let c = Arrayf.map (fun (_,_,x) -> x) abc in
  (a,b,c)

(**************************************************************)

type dest =
  | XSend of rank
  | XCast
  | XGossip of Addr.set option

let string_of_dest = function
  | XSend rank -> sprintf "Send(%d)" rank
  | XCast -> "Cast"
  | XGossip _ -> "Gossip"

(**************************************************************)

type 'msg t = {
  disable : unit -> unit ;
  xmit	  : dest -> 'msg
}

(**************************************************************)

let disable m 	  = m.disable ()
let send m rank   = m.xmit (XSend rank)
let cast m 	  = m.xmit XCast
let gossip m dest = m.xmit (XGossip dest)

(**************************************************************)

let send_modes ranking addr addrs dest =
  log (fun () -> sprintf "src=%s dst=%s" 
    (Addr.string_of_set addr) (Addr.string_of_set dest)) ;
  
  let modes = Addr.modes_of_view (Arrayf.doubleton addr dest) in
  
  log (fun () -> sprintf "modes.123=%s" 
    (Arrayf.to_string Addr.string_of_id modes)) ;
  
  let modes = Arrayf.filter Addr.has_pt2pt modes in
  if Arrayf.is_empty modes then
    failwith "route:Send:no pt2pt modes in address" ;
  Arrayf.singleton (Addr.prefer ranking modes)


let find_modes ranking addr addrs = function
  | XSend dest ->
      let dest = Arrayf.get addrs dest in
      send_modes ranking addr addrs dest 
  | XCast ->
      let modes = Addr.modes_of_view addrs in
      let modes = Arrayf.filter Addr.has_pt2pt modes in
      if Arrayf.is_empty modes then
        failwith "route:Cast:no pt2pt modes in address" ;
      Arrayf.singleton (Addr.prefer ranking modes)
  | XGossip None ->                (* all available cast modes *)
      let modes = Addr.modes_of_view (Arrayf.singleton addr) in
      let modes = Arrayf.filter Addr.has_mcast modes in
      if Arrayf.is_empty modes then
        failwith "route:Gossip:no mcast modes in address" ;
      modes
  | XGossip(Some dest) ->
      send_modes ranking addr addrs dest 
(*
  | XMerge(_,dest) ->
      let modes = Addr.modes_of_view (Arrayf.doubleton addr (snd dest)) in
      let modes = Arrayf.filter Addr.has_pt2pt modes in
      if Arrayf.is_empty modes then
        failwith "route:Merge:no pt2pt modes in address" ;
      Arrayf.singleton (Addr.prefer ranking modes)
*)

(**************************************************************)

let find_dests endpt group addrs view = function
  | XSend(dest) ->
      let dest = Arrayf.get addrs dest in
      Domain.Pt2pt(Arrayf.singleton dest)
  | XCast ->
      (* Send to all the addresses but mine.
       *)
      if Arrayf.length addrs <> Arrayf.length view then
	failwith "mismatched view and address" ;
      let addrs = Arrayf.combine view addrs in
      let addrs = Arrayf.filter (fun (e,a) -> e <> endpt) addrs in
      let addrs = Arrayf.map snd addrs in
      Domain.Pt2pt(addrs)
  | XGossip None -> Domain.Gossip(group)
  | XGossip(Some dest) ->
      Domain.Pt2pt(Arrayf.singleton dest)
(*
  | XMerge(_,dest) -> Domain.Pt2pt(Arrayf.singleton (snd dest))
*)

(**************************************************************)

let get_xmits alarm handles dests modes =
  (* Check for empty pt2pt destinations.
   *)
  let empty = 
    match dests with
    | Domain.Pt2pt dests -> Arrayf.is_empty dests
    | _ -> false
  in

  if empty then (
    Arrayf.empty 
  ) else (
    let xmits = 
      Arrayf.map (fun (mode,handle) ->
	if Arrayf.mem mode modes then
	  Domain.xmit handle dests
	else
	  None
      ) handles
    in
    Arrayf.filter_nones xmits
  )

(**************************************************************)

let is_just_udp a =
  Arrayf.length a = 1 &&
  (Arrayf.get a 0 = Addr.Udp)

let route alarm endpt addr group view addrs handles dest modes =
  log (fun () -> sprintf "endpt=%s,group=%s,dest.0=%s"
    (Endpt.string_of_id endpt)
    (Group.string_of_id group)
    (string_of_dest dest)) ;

  (* Calculate the destinations.
   *)
  let dests = find_dests endpt group addrs view dest in

  log (fun () -> sprintf "dests.1=%s" (Domain.string_of_dest dests)) ;

  (* For point-to-point:
   * (I've already been stripped)
   * 1. Strip local destinations
   * 2. Strip duplicate destinations
   * 3. Determine if any destinations are local.
   * 4. Use Mcast if we have it and there is more than 1
   *    effective destination (but not if the mode is Udp).
   *
   * For gossip: set local.
   *)
  let local,dests = match dests with
  | Domain.Pt2pt(dests) ->
      let local,dests = Addr.compress addr dests in
      let dests = 
	if Arrayf.length dests > 1
	  && (not (is_just_udp modes))
	  && Arrayf.for_all Addr.has_mcast modes
	then (
	  let loopback = true in
	  Domain.Mcast(group,loopback)
        ) else (
	  Domain.Pt2pt dests
        )
      in (local,dests)
  | Domain.Gossip _ -> true,dests
  | _ -> false,dests
  in

  log (fun () -> sprintf "local=%b, dests.2=%s" local 
      (Domain.string_of_dest dests)) ;

  (* Collect all xmits.
   *)
  let xmits = get_xmits alarm handles dests modes in

  log (fun () -> sprintf "nxmits=%d" (Arrayf.length xmits)) ;

  (* Add in local xmits, if needed.  They are put at the end
   * so that messages are sent on the wire first.
   *)
  let xmits =
    if local then (
      let local = Alarm.local_xmits alarm in
      Arrayf.append xmits (Arrayf.singleton local)
    ) else (
      xmits
    )
  in

  (* merge together all xmit functions. 
   *)
  Route.merge4iov xmits

(**************************************************************)

(*
let enabled = 
  let table = Hashtbl.create name 100 in
  Trace.install_root (fun () ->
    [sprintf "TRANSPORT:#enabled=%d" (hashtble_size table)]
  ) ;
  table
*)
(*
    Hashtbl.iter (fun id () ->
      eprintf "  %s\n" (Conn.string_of_id_field id)
    ) enabled
*)

let f2 alarm ranking (ls,vs) stack_id router gen_recv =

  (* Create record of connections.
   *)
  let conns = Conn.create ls.rank vs.version vs.group 
      ls.view_id stack_id vs.proto_id vs.view 
  in

  (* Check for security problems.
   *)
  Route.security_check router ;

  (* Enable all potential domains.
   *)
  let modes = Addr.ids_of_set ls.addr in
  let handles = 
    Arrayf.map (fun mode ->
      if Addr.has_mcast mode || Addr.has_pt2pt mode then
	let domain = Domain.of_mode alarm mode in
	let handle = Domain.enable domain mode vs.group ls.addr vs.view in
	Some(mode,handle)
      else None
    ) modes
  in
  let handles = Arrayf.filter_nones handles in

  (* Transmit a message.
   *)
  let xmit dest =
    let conn = match dest with
    | XCast -> 
	Conn.multi_send conns
    | XSend dest -> 
        Conn.pt2pt_send conns dest
    | XGossip _ ->			(* Same either way *)
	Conn.gossip conns
    in

    log (fun () -> sprintf "conn=%s" (Conn.string_of_id conn)) ;

    (* Find modes to use.
     *)
    let modes = find_modes ranking ls.addr vs.address dest in

    log (fun () -> sprintf "modes=%s" 
	(Arrayf.to_string Addr.string_of_id modes)) ;

    (* Determine how the message will be sent.
     *)
    let xmits = route alarm ls.endpt ls.addr 
	vs.group vs.view vs.address handles dest modes
    in

    (* In the end, pass everything off to router's blast function.
     *)
    Route.blast router xmits vs.key conn
  in

  (* Disable this transport instance.
   *)
  let disable () =
(*
    Hashtbl.remove enabled conns.Conn.id ;
*)
    Arrayf.iter (fun (_,handle) ->
      Domain.disable handle
    ) handles ;
    Route.remove router (Alarm.handlers alarm) (Conn.key conns) 
      (Conn.all_recv conns)
  in

  let t = {
    xmit    = xmit ;
    disable = disable 
  } in

  (* This is a hack for the bypass protocols.
   *)
  let handler,ret = gen_recv t in
(*
    Hashtbl.add enabled conns.Conn.id () ;
*)
  Route.install router (Alarm.handlers alarm) (Conn.key conns) 
    (Conn.all_recv conns) vs.key handler ;
  ret

let f alarm ranking vf i j receive =
   f2 alarm ranking vf i j (fun trans -> (receive,trans))

(**************************************************************)
