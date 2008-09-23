(**************************************************************)
(* HOT_APPL.ML *)
(* Authors: Alexey Vaysburd, Mark Hayden 11/96 *)
(**************************************************************)
open Ensemble
open Util
open Trans
open View
open Appl_intf
open Appl_intf.New
open Protos
open Hot_util
open Buf
(**************************************************************)
let name = Trace.file "HOT_APPL"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
(**************************************************************)

(* Types of C application's downcalls to Ensemble.  The
 * 'unit's are to force Ocaml to not use constant tags
 * in the representation.
 *)
type c_dncall = 
  | C_Join of Hot_util.join_options 
  | C_Cast of Iovec.raw
  | C_Send of endpt * Iovec.raw
  | C_Suspect of endpt array 
  | C_XferDone of unit
  | C_Protocol of string
  | C_Properties of string
  | C_Leave of unit
  | C_Prompt of unit
  | C_Rekey of unit
  | C_BlockOk of unit
  | C_Void of unit

let string_of_dncall = function
  | C_Join _ -> "C_Join"
  | C_Cast _ -> "C_Cast"
  | C_Send _ -> "C_Send"
  | C_Suspect _ -> "C_Suspect"
  | C_XferDone _ -> "C_XferDone"
  | C_Protocol _ -> "C_Protocol"
  | C_Properties _ -> "C_Properties"
  | C_Leave _ -> "C_Leave"
  | C_Prompt _ -> "C_Prompt"
  | C_Rekey _ -> "C_Rekey"
  | C_BlockOk _ -> "C_BlockOk"
  | C_Void _ -> "C_Void"

let dncall_of_c (ls,vs) = 
  let viewh = Hashtbl.create 10 in
  for rank = 0 to pred ls.nmembers do
    let endpt = Arrayf.get vs.view rank in
    let endpt = Endpt.string_of_id endpt in
    Hashtbl.add viewh endpt rank
  done ;
  
  function dncall -> 
    log (fun () -> string_of_dncall dncall);
    match dncall with 
      | C_Join jops -> failwith "dncall_of_c:C_Join"
      | C_Cast iov ->
	  let iovl = Iovecl.of_iovec (Iovec.of_raw iov) in
	  Cast(iovl)
      | C_Send(dest,iov) -> (
	  let iovl = Iovecl.of_iovec (Iovec.of_raw iov) in
	  try
      	    let rank = Hashtbl.find viewh dest in
	    Send1(rank,iovl)
	  with Not_found -> 
	    (* Drop the message and do a no-op.
	     *)
	    Iovecl.free iovl;
	    eprintf "HOT_APPL:Send:warning:unknown destination '%s'\n" dest ;
  	    Control(No_op)
	)
      | C_Suspect endpts ->
	  let suspects = ref [] in
	  Array.iter (fun endpt ->
	    try
	      let rank = Hashtbl.find viewh endpt in
	      if rank = ls.rank then
		failwith "suspecting myself!" ;
	      suspects := rank :: !suspects
	    with Not_found ->
	      eprintf "HOT_APPL:warning:Suspect:unknown endpt\n"
	  ) endpts ;
	  Control (Suspect !suspects)
      | C_XferDone () -> Control XferDone
      | C_Protocol s -> Control (Protocol (Proto.id_of_string s))
      | C_Properties properties ->
	  let protocol =
	    let props = 
	      let props_l = Util.string_split ":" properties in
	      let props_l = List.map Property.id_of_string props_l in
	      if vs.groupd then
		Property.strip_groupd props_l
	      else 
		props_l
	    in
	    Property.choose props
	  in
	  Control (Protocol protocol)
      | C_Leave () -> Control Leave
      | C_Prompt () -> Control Prompt
      | C_Rekey () -> Control (Rekey false)
      | C_BlockOk () -> Control (Block true)
      | C_Void () -> failwith "dncall_of_c:C_Void"
	  
(**************************************************************)
	  
type context = (id, (c_dncall -> unit)) Hashtbl.t
    
let context name =
  let active = Hashtbl.create 10 in
  active

let get active id dncall =
  try
    let handle_dncall = Hashtbl.find active id in
    handle_dncall dncall
  with Not_found ->
    match dncall with
    | C_BlockOk _ -> 
	eprintf "HOT_UTIL:ignoring BlockOk for id not found in table (should not be a problem)\n"
    | _ ->
(*    	eprintf "HOT_UTIL:get:id=%d:dncall=%s\n" id (string_of_dncall dncall) ;*)
    	failwith "get:not found in table"

(**************************************************************)

(* Join a group.  Use specified group context id.
 *)
let join active id dispatch_dncalls ((init_ls,init_vs) as init_vf) interface =
  (* Initialize the state.
   *)
  let dncalls = Queuee.create () in
  let hb_requested = ref false in
  let in_handler = ref false in
  let exited = ref false in
  let leaving = ref false in
  
  (* Register a function to be invoked in order to request
   * a heartbeat.  
   *)
  let async = Appl.async (init_vs.group,init_ls.endpt) in

  (* Add me to the table.
   *)
  begin
    let handle_dncall dncall = 
      Queuee.add dncall dncalls ;
      if not !in_handler && not !hb_requested then (
      	hb_requested := true ;
      	async ()
      )
    in
    if Hashtbl.mem active id then
      failwith "initialize:already in table" ;
    Hashtbl.add active id handle_dncall
  end ;

  (* Return callback configuration for the given id and
   * heartbeat rate.
   *
   * When defining application configuration for Ensemble,
   * we have to define all callback functions.  Typically a
   * callback function will convert its arguments into
   * arguments for the corresp. C upcall; invoke the upcall;
   * retrieve (from C app.) all enqueued requests (if any),
   * convert them into actions, and add to the actions list;
   * return the action list for the id (if appropriate for
   * this callback type).  
   *)
  let interface =
    (* Got a new view, prepare the message handlers.
     *)
    let install (ls,vs) =
      (* Create a hashtable of the members in this view.
       *)
      let viewh = Hashtbl.create 10 in
      for rank = 0 to pred ls.nmembers do
	let endpt = Arrayf.get vs.view rank in
	let endpt = Endpt.string_of_id endpt in
	Hashtbl.add viewh endpt (rank,[|rank|])
      done ;

      (* Return pending actions for the given id.
       *
       * For send actions, need to convert endp_uid into the rank
       * in the current view.  If there is no member in the
       * current view with the given eid, drop the action.
       *
       * For suspect actions, if a suspected endp is not in the
       * current view, don't include it in the suspect action.  
       *)
      let dncall_of_c = dncall_of_c (ls,vs) in
      let pending_actions () =
	dispatch_dncalls () ;
	let actions = Queuee.to_list dncalls in
	Queuee.clear dncalls ;
	let actions = Array.of_list actions in
	let actions = Array.map (fun c_dncall ->
	  let dncall = dncall_of_c c_dncall in
	  begin
	    match dncall with
	    | Control Leave -> 
	        (* The leave downcall blocks the member. 
		 *)
	      	leaving := true ;
	    | Control (Suspect suspects) ->
	      	if List.mem ls.rank suspects then
		  failwith "suspecting myself!"
	    | _ -> ()
	  end ;
	  dncall
	) actions in
	actions
      in

      (* Deliver the view state to the application.
       *)
      in_handler := true ;
      let actions,handlers = interface.install (ls,vs) in

      (* No actions should be returned by the install handler for HOT.
       *)
      assert (Array.length actions = 0) ;

      let actions = pending_actions () in
      in_handler := false ;

      (* Got a message.  Invoke C callback dispatcher;
       * return a list of pending actions.  This gets
       * handled in two stages.  First, we take the origin,
       * blocked, cast_or_send arguments and do some
       * preprocessing.  Second, we generate functions
       * taking the last argument -- the actual message.
       * This improves performance because the first
       * arguments are applied by Ensemble at the view
       * change and in the normal case we only deal with the
       * last argument.  
       *)
      let receive origin blocked cast_or_send =
	let receive = handlers.receive origin blocked cast_or_send in
	match blocked with
	| U -> 
	    fun iovl ->
	      in_handler := true ;
	      let actions = receive iovl in
	      assert (Array.length actions = 0) ;
	      let ret = pending_actions () in
	      in_handler := false ;
	      ret
	| B -> 
	    fun iovl -> 
	      let actions = receive iovl in
	      assert (Array.length actions = 0) ;
	      [||]
      in

      (* Got a heartbeat callback.  Invoke C app's callback
       * dispatcher; return a list of pending actions.  
       *)
      let heartbeat time = 
	in_handler := true ;
	let actions = handlers.heartbeat time in
	assert (Array.length actions = 0) ;
	hb_requested := false ;		(* should this be here? *)
	let ret = pending_actions () in
	in_handler := false ;
	ret
      in

      (* The group is about to block.  Tell the application it
       * should not request Ensemble's attention until the
       * next view is installed.  Return the list of pending
       * actions.  
       *)
      let block () =
	in_handler := true ;
	let actions = handlers.block () in
	assert (Array.length actions = 0) ;
	let ret = pending_actions () in
	in_handler := false ;
	ret
      in

      (* Disable this set of handlers.  Nothing to be done
       * here.
       *)
      let disable = ident in

      let handlers = {
	flow_block = (fun _ -> ());
        heartbeat = heartbeat ;
	receive = receive ;
	block = block ;
	disable = disable 
      } in

      (actions,handlers)
    in

    (* We have left the group.  Notify the C application.
     * The id is invalidated.
     *)
    let exit () =
      exited := true ;
      interface.exit () ;
      try
        (* Ensure that the link was in the table.
	 *)
    	let _ = Hashtbl.find active id in
    	
        (* Then remove it.
         *)
    	Hashtbl.remove active id
      with Not_found ->
    	failwith "destroy:not found in table"
    in

    { heartbeat_rate = interface.heartbeat_rate ;
      install = install ;
      exit = exit 
    }
  in
  Appl.config_new interface init_vf

(**************************************************************)
