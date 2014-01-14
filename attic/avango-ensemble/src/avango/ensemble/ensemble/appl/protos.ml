(**************************************************************)
(* PROTOS.ML *)
(* Authors: Mark Hayden, Alexey Vaysburd 11/96 *)
(**************************************************************)
(* Create a server process that provides "Ensemble services".
 * Client connect to it and perform application actions: 
 * create,send,cast, ...
*)
(**************************************************************)
open Util
open Trans
open Buf
open View
open Appl_intf
open Appl_intf.New
(**************************************************************)
let name = Trace.file "PROTOS"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
let logc = Trace.log (name^"C")
(**************************************************************)

type id = int
type 'a dncall_gen = ('a,'a) action
    
type 'a upcall_gen = 
  | UInstall of View.full
  | UReceive of origin * cast_or_send * 'a
  | UHeartbeat of Time.t
  | UBlock
  | UExit
      
type endpt = string
    
type 'a message_gen =
  | Create of id * View.state * Time.t * (Addr.id list)
  | Upcall of id * 'a upcall_gen
  | Dncall of id * 'a dncall_gen
  | SendEndpt of id * endpt * 'a
  | SuspectEndpts of id * endpt list
      
type message = Iovecl.t message_gen
type dncall = Iovecl.t dncall_gen
type upcall = Iovecl.t upcall_gen
    
(**************************************************************)

let string_of_dncall = Appl_intf.string_of_action

let string_of_upcall = function
  | UInstall vf -> sprintf "UInstall(%s)" (View.string_of_full vf)
  | UReceive(origin,cs,_) ->
      sprintf "UReceive(%d,%s,_)" origin (string_of_cs cs)
  | UHeartbeat time -> sprintf "UHeartbeat(%s)" (Time.to_string time)
  | UBlock -> sprintf "UBlock"
  | UExit -> sprintf "UExit"

let string_of_message = function
  | Create(id,vs,hbr,modes) ->
      sprintf "Create(%d,%s,%s,%s)" id 
	(View.string_of_state vs) 
	(Time.to_string hbr)
 	(string_of_list Addr.string_of_id modes)
  | Upcall(id,upcall) ->
      sprintf "Upcall(%d,%s)" id (string_of_upcall upcall)
  | Dncall(id,dncall) ->
      sprintf "Dncall(%d,%s)" id (string_of_dncall dncall)
  | SendEndpt(id,endpt,iov) -> "SendEndpt(_)"
  | SuspectEndpts(id,endpts) -> "SuspectEndpts(_)"

(**************************************************************)

(* Create an interface for a new member.
 *)
let interface dncalls deliver_upcall exited id heartbeat_rate disabled =
  log (fun () -> sprintf "heartbeat_rate=%f\n" (Time.to_float heartbeat_rate));
  let write_upcall msg = 
    let msg = Upcall(id,msg) in
    log (fun () -> string_of_message msg) ;
    deliver_upcall msg
  in
  let got_leave = ref false in

  (* Got a new view, prepare the message handlers.
   *)
  let install ((ls,vs) as vf) =
    let am_blocked = ref U in
    let next_heartbeat = ref Time.zero in
    let need_block_ok = ref false in

    (* Create a hashtable of the members in this view.
     *)
    let viewh = Hashtbl.create (2 * ls.nmembers) in
    Arrayf.iteri (fun rank endpt ->
      let endpt = Endpt.string_of_id endpt in
      Hashtbl.add viewh endpt rank
    ) vs.view ;

    let transl_dncall = function
      | Dncall(_,d) ->
	  begin
	    match d with
	    | Control Leave ->
		got_leave := true ;
	    | Control (Block true) ->
		am_blocked := B ;
	    | _ -> ()
	  end ;
	  d
      | SendEndpt(_,e,m) ->
	  let rank = 
	    try Hashtbl.find viewh e
	    with Not_found -> failwith (sanityn 1)
	  in
	  Send1(rank,m)
      | SuspectEndpts(_,s) ->
	  failwith "unimplemented"
      | _ -> failwith (sanityn 2)
    in

    (* Return pending actions.
     *
     * For send actions, need to convert endp_uid into the rank
     * in the current view.  If there is no member in the
     * current view with the given eid, drop the action.
     *
     * For suspect actions, if a suspected endp is not in the
     * current view, don't include it in the suspect action.  
     *)
    let pending_actions () =
      let actions = Queuee.to_array dncalls in
      Array.map transl_dncall actions
    in

    (* Deliver the view state to the application.
     *)
    write_upcall (UInstall vf) ;
    let actions = pending_actions () in

    (* Got a message.  Invoke C callback dispatcher; return
     * a list of pending actions.  This gets handled in two
     * stages.  First, we take the origin, blocked,
     * cast_or_send arguments and do some preprocessing.
     * Second, we generate functions taking the last
     * argument -- the actual message.  This improves
     * performance because the first arguments are applied
     * by Ensemble at the view change and in the normal case
     * we only deal with the last argument.  
     *)
    let receive origin blocked cast_or_send =
      let handler iovl =
	(* The following check is not valid because of the
	 * stuff we do with Control(Block(_)).
	 *)
	(*assert (blocked = !am_blocked) ;*)
	write_upcall (UReceive(origin,cast_or_send,iovl)) ;
	if !am_blocked = U then
	  pending_actions ()
	else [||]
      in handler
    in

    (* Got a heartbeat callback.  Invoke C app's callback
     * dispatcher; return a list of pending actions.  
     *)
    (* NOTE: the heartbeat may have been generated by a call to async,
     * in which case we do *not* deliver it to the application.
     *)
    let heartbeat time = 
      if Time.ge time !next_heartbeat then (
	next_heartbeat := Time.add time heartbeat_rate ;
	write_upcall (UHeartbeat time) ;
      ) ;
      
      let ret = pending_actions () in

      (* If the connection was disabled, we need to continue
       * the blocking protocol for the (failed) client so that
       * the stack will be released.
       *)
      if !disabled && !need_block_ok then (
	need_block_ok := false ;
	Array.append ret [|Control(Block true)|]
      ) else (
	ret
      )
    in

    (* The group is about to block.  Tell the application it
     * should not request Ensemble's attention until the
     * next view is installed.  Return the list of pending
     * actions.  
     *)
    let block () =
      write_upcall UBlock ;
      let ret = pending_actions () in
      
      (* Add a Block(false) action, unless already disabled.
       *)
      if !disabled then
	ret
      else (
	need_block_ok := true ;
	Array.append ret [|Control(Block false)|]
      )
    in

    let handlers = {
      flow_block = (fun _ -> ());
      heartbeat = heartbeat ;
      receive = receive ;
      block = block ;
      disable = ident
    } in

    (actions,handlers)
  in

  (* We have left the group.  Notify the C application.
   * The id is invalidated.
   *)
  let exit () =
    write_upcall UExit ;
    exited ()
  in

  { heartbeat_rate = heartbeat_rate ;
    install = install ;
    exit = exit }

(**************************************************************)

let server alarm addr config debug_ignored deliver_upcall =
  let table = (Hashtbl.create 10 : (id,(message ->unit))Hashtbl.t) in

  let get_handler id =
    try Hashtbl.find table id
    with Not_found -> 
      fun msg ->
	match msg with
	| Dncall(_,Control(Block _)) ->
	    log (fun () -> "PROTOS:ignoring BlockOk for id not found in table (should not be a problem)")
	|  _ ->
	    eprintf "PROTOS:id=%d ids=%s msg=%s\n"
	      id (string_of_int_list (List.map fst (hashtbl_to_list table))) 
	      (string_of_message msg) ;
	    failwith "unknown id"
  in

  let disabled = ref false in

  let disable () = 
    disabled := true ;
    Hashtbl.iter (fun id handler ->
      handler (Dncall(id,Control(Leave)))
    ) table
  and recv msg =
    log (fun () -> string_of_message msg) ;
    match msg with
    | Create(id,vs,heartbeat_rate,modes) ->
	assert (Arrayf.length vs.view = 1) ;
	let endpt = Arrayf.get vs.view 0 in
	let dncalls = 
	  let dncalls = Queuee.create () in

	  (* Register a function to be invoked in order to request
	   * a heartbeat.  
	   *)
	  let async = Async.find (Alarm.async alarm) (vs.group,endpt) in
	  let handle_dncall dncall = 
	    (* Request an async heartbeat if the queue is empty.
	     *)
	    if Queuee.empty dncalls then (
	      async () ;
	    ) ;
	    Queuee.add dncall dncalls ;
	  in

	  if Hashtbl.mem table id then
	    failwith "initialize:already in table" ;
	  Hashtbl.add table id handle_dncall ;
	  logc (fun () -> sprintf "added id=%d #=%d" id (hashtbl_size table)) ;
	  dncalls
	in
	
	let exited () =
          (* Ensure that the link was in the table.
	   *)
	  if not (Hashtbl.mem table id) then
    	    failwith "exited:not found in table" ;
    	    
          (* Then remove it.
	   *)
    	  Hashtbl.remove table id ;
	  logc (fun () -> sprintf "removed id=%d #=%d" id (hashtbl_size table)) ;
	in

        (* Unset the protos bit.  Create a new address
	 * that is local to this process.
	 *)
	if Arrayf.length vs.view <> 1 then 
	  failwith (sanityn 6) ;
	let addr = addr modes in
    	let vs = View.set vs [
	  Vs_protos (Arrayf.singleton false) ;
	  Vs_address (Arrayf.singleton addr)
	] in

	let ls = View.local name endpt vs in
	let interface = interface dncalls deliver_upcall exited id heartbeat_rate disabled in
	config interface (ls,vs)

      (* Dispatch the message to the handler corresponding
       * to the group context.  
       *)
    | Dncall(id,_) as msg -> 
	get_handler id msg
    | SendEndpt(id,_,_) as msg ->
	get_handler id msg
    | SuspectEndpts(id,_) as msg ->
	get_handler id msg
    | _ -> failwith (sanityn 3)
  in (recv,disable,())

(**************************************************************)

(* Marshal messages.  We use the powermarsh module to
 * separately handle the Iovec portion of send and receives
 * to make the payload zero-copy.  
 *)
let make_marsh () =
  let marsh,unmarsh = Buf.make_marsh "protos" in
  let marsh msg =
    let msg,iovl =
      (* Strip the payload portion out.
       *)
      match msg with
      | Upcall(id,msg) ->
	  let msg,iovl = 
	    match msg with
	    | UInstall vf -> UInstall vf, Iovecl.empty
	    | UReceive(o,c,i) -> 
		UReceive(o,c,()),i
            | UHeartbeat t -> UHeartbeat t, Iovecl.empty
	    | UBlock -> UBlock, Iovecl.empty
	    | UExit -> UExit, Iovecl.empty
	  in
	  Upcall(id,msg),iovl
      |	Dncall(id,msg) ->
	  let msg,iovl = 
	    match msg with
	    | Cast(i) -> Cast(),i
	    | Send(d,i) -> Send(d,()),i
	    | Send1(d,i) -> Send1(d,()),i
	    | Control c -> Control c, Iovecl.empty
	  in
	  Dncall(id,msg),iovl
      | SendEndpt(id,endpt,i) ->
	  SendEndpt(id,endpt,()),i
      | SuspectEndpts(id,endpts) ->
	  SuspectEndpts(id,endpts), Iovecl.empty
      | Create(id,vs,hbr,modes) ->
	  Create(id,vs,hbr,modes), Iovecl.empty
    in
    marsh msg, iovl
  in
  
  let unmarsh (buf,iovl) =
    let msg = unmarsh buf len0 in

    (* Shift the payload portion back in.
     *)
    match msg with 
    | Upcall(id,msg) ->
	let msg = 
	  match msg with
	  | UInstall vf -> 
	      assert (Iovecl.is_empty iovl) ;
	      UInstall vf
	  | UReceive(o,c,()) -> 
	      UReceive(o,c,iovl)
          | UHeartbeat t ->
	      assert (Iovecl.is_empty iovl) ;
	      UHeartbeat t
	  | UBlock -> 
	      assert (Iovecl.is_empty iovl) ;
	      UBlock
	  | UExit ->
	      assert (Iovecl.is_empty iovl) ;
	      UExit
	in
	Upcall(id,msg)
    | Dncall(id,msg) ->
	let msg = 
	  match msg with
	  | Cast () ->
	      Cast iovl
	  | Send(d,()) ->
	      Send(d,iovl)
	  | Send1(d,()) ->
	      Send1(d,iovl)
	  | Control c -> 
	      assert (Iovecl.is_empty iovl) ;
	      Control c
	in
	Dncall(id,msg)
    | SendEndpt(id,endpt,()) ->
	SendEndpt(id,endpt,iovl)
    | SuspectEndpts(id,endpts) ->
	assert (Iovecl.is_empty iovl) ;
	SuspectEndpts(id,endpts)
    | Create(id,vs,hbr,modes) ->
	assert (Iovecl.is_empty iovl) ;
	Create(id,vs,hbr,modes)
  in

  (marsh,unmarsh)

(**************************************************************)

type state = {
  id : id ;
  c_send : message -> unit ;
  interface : Appl_intf.New.t ;
  mutable vs : View.state ;
  mutable handlers : Iovecl.t handlers ;
  mutable blocked : blocked ;
  mutable recv_cast : (Iovecl.t -> Iovecl.t naction array) Arrayf.t ;
  mutable recv_send : (Iovecl.t -> Iovecl.t naction array) Arrayf.t
}  

let update_recv_handlers c =
  let nmembers = Arrayf.length c.vs.view in
  c.recv_cast <- Arrayf.init nmembers (fun i -> c.handlers.receive i c.blocked C) ;
  c.recv_send <- Arrayf.init nmembers (fun i -> c.handlers.receive i c.blocked S) 

type t = {
  alarm : Alarm.t ;
  table : (id,state) Hashtbl.t ;
  send : message -> unit ;
  counter : unit -> int
} 

type config = View.full -> Layer.state -> unit

(**************************************************************)

let handle_actions c actions =
  Array.iter (fun dncall ->
    assert (c.blocked = U) ;
    c.c_send (Dncall(c.id,dncall))
  ) actions

let config s (_,vs) state =
  assert (Arrayf.length vs.view = 1) ;
  let id = s.counter () in
  let interface = state.Layer.interface in

  let c = {
    id = id ;
    vs = vs ;
    c_send = s.send ;
    handlers = failed_handlers ;
    interface = interface ;
    blocked = B ;
    recv_cast = Arrayf.empty ;
    recv_send = Arrayf.empty
  } in

  (* Create an async handler for this member.
   *)
  let disable_async =
    let endpt = Arrayf.get vs.view 0 in
    Async.add (Alarm.async s.alarm) (vs.group,endpt) (fun () ->
      if c.blocked = U then
	handle_actions c (c.handlers.heartbeat (Alarm.gettime s.alarm))
    )
  in

  (* Extend the exit handler to also remove the async.
   *)
  let exit () =
    disable_async () ;
    interface.exit ()
  in

  (* Override the exit handler.
   *)
  let interface = { interface with exit = exit } in

  assert (not (Hashtbl.mem s.table id)) ;
  Hashtbl.add s.table id c ;

  (* Project the modes from the view state.  Erase any address info in
   * the view state.  The server will generate addresses using the mode
   * list.
   *)
  let modes = Arrayf.to_list (Addr.ids_of_set (Arrayf.get vs.address 0)) in
  let address = Arrayf.create 1 (Addr.set_of_arrayf Arrayf.empty) in
  let vs = View.set vs [Vs_address address] in

  s.send (Create(id,vs,interface.heartbeat_rate,modes))

(**************************************************************)

let client alarm info send =
  let send msg =
    log (fun () -> string_of_message msg) ;
    send msg
  in
  
  let disable () =
    eprintf "PROTOS:lost connection to server, exiting\n" ;
    exit 1
  in
  
  let table = Hashtbl.create 10 in

  let state = {table=table;send=send;alarm=alarm;counter=counter()} in

  let recv msg =
    log (fun () -> string_of_message msg) ;
    let id,upcall = 
      match msg with
      | Upcall(id,upcall) -> id,upcall
      | _ -> failwith (sanityn 8)
    in
    let c = 
      try Hashtbl.find table id 
      with Not_found ->
	eprintf "PROTOS:get:id=%d:dncall=%s\n" id (string_of_upcall upcall) ;
	failwith "get:not found in table"
    in
    match upcall with
    | UInstall((ls,vs) as vf) ->
	c.blocked <- U ;
	let actions,handlers' = c.interface.install vf in
	c.handlers <- handlers' ;
	c.vs <- vs ;
	update_recv_handlers c ;
	handle_actions c actions
    | UReceive(origin,cs,iovl) ->
	let handlers =
	  match cs with
	  | C -> c.recv_cast
	  | S -> c.recv_send
	in
	assert (origin >= 0 && origin < Arrayf.length handlers) ;
	let handler = Arrayf.get handlers origin in
	handle_actions c (handler iovl)
    | UHeartbeat time ->
	if c.blocked = U then
	  handle_actions c (c.handlers.heartbeat time)
    | UBlock ->
	handle_actions c (c.handlers.block ()) ;
	
	(* Respond with Block true now.
	 *)
	handle_actions c [|Control(Block true)|] ;
	
	c.blocked <- B ; 
	update_recv_handlers c
    | UExit ->
	c.interface.exit () ;
	Hashtbl.remove table id
  in
  
  let config a b = config state a b in
  
  (recv,disable,config)

(**************************************************************)

