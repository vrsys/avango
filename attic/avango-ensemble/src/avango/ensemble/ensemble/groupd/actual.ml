(**************************************************************)
(* ACTUAL.ML *)
(* Author: Mark Hayden, 11/96 *)
(* Designed with Roy Friedman *)
(* Ohad Rodeh: Converted from Old to New application interface, 11/2001. *)
(**************************************************************)
(* BUGS:
 
 *  Orders members in views lexicographicly

 *)
(**************************************************************)
open Trans
open Util
open View
open Mutil
open Appl_intf open New
(**************************************************************)
let name = Trace.file "ACTUAL"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
(**************************************************************)

let list_split4 l =
  List.fold_left (fun (la,lb,lc,ld) (a,b,c,d) ->
    (a::la,b::lb,c::lc,d::ld)
  ) ([],[],[],[]) l

let list_join l =
  let l = Lset.sort l in
  let l = 
    List.fold_left (fun cur (k,d) ->
      match cur with
      | [] -> [k,[d]]
      | (k',l) :: tl ->
          if k = k' then 
            (k',(d::l)) :: tl
          else
            (k,[d]) :: cur
    ) [] l
  in
  l

(**************************************************************)

type 'g message = 
  | ToCoord of group * endpt * rank(*of coord*) * member_msg
  | ToMembers of group * coord_msg
  | Announce of group * 'g
  | Destroy of group


type xfer_stage = 
    Null
  | Gather 

type 'g info = 
    (Mutil.group *
      (Mutil.endpt * Member.state * Trans.ltime * Mutil.endpt list) list)
    list * 
    (Mutil.group * 'g) list

type 'g xfer_message = 
    M_Reg of 'g message
  | M_Gather of 'g info
  | M_State of 'g info 

type ('m, 'g) t = {
  async : unit -> unit ;
  mutable ls : View.local ;
  mutable vs : View.state ;
  mutable announce : (group -> 'g -> unit) option ; (* hack! *)
  mutable blocking : bool ;
  ops : ('g xfer_message, 'g xfer_message) Appl_intf.action Queuee.t ;
  members : (group, Member.t list) Hashtbl.t ;
  coords : (group, Coord.t) Hashtbl.t ;
  groups : (group, 'g) Hashtbl.t ;
  mutable xfer_stage : xfer_stage ;
  mutable comb : 'g info list 
}

(**************************************************************)
(**************************************************************)

let string_of_msg = function
  | ToMembers(_,msg) ->
      let msg = string_of_coord_msg msg in
      sprintf "ToMembers(%s)" msg
  | ToCoord(_,_,rank,msg) -> 
      let msg = string_of_member_msg msg in
      sprintf "ToCoord(%s)" msg
  | Announce _ -> "Announce"
  | Destroy _ -> "Destroy"

let string_of_xfer_stage = function
    Null -> "Null"
  | Gather -> "Gather"

let string_of_xfer_msg = function
    M_Reg  msg -> "M_Reg " ^ string_of_msg msg
  | M_Gather _ -> "M_Gather" 
  | M_State _ -> "M_State"

(**************************************************************)
(**************************************************************)

let coord s group f =
  let coord = 
    try Hashtbl.find s.coords group with Not_found ->
(*
      if s.vs.rank <> 0 then 
        failwith "non-coord is default" ;
*)
      let cast msg =
	log (fun () -> sprintf "coord:cast:%s" (string_of_coord_msg msg)) ;
	let msg = ToMembers(group,msg) in
	Queuee.add (Cast (M_Reg msg)) s.ops ;
	if not s.blocking 
	  && s.xfer_stage = Null then 
	    s.async ()
      in

      let coord = Coord.create s.vs.primary cast in
      Hashtbl.add s.coords group coord ;
      coord
  in
  f coord
  
let members s group f = 
  let members = 
    try Hashtbl.find s.members group
    with Not_found -> []
  in List.iter f members

(**************************************************************)

let join s group endpt ltime to_client =
  (* BUG: there is a dependency below on having the coordinator
   * manage all the groups.
   *)
  let coord = 0 in

  let to_coord msg =
    let msg = ToCoord(group,endpt,coord,msg) in
    Queuee.add (Send1(coord,(M_Reg msg))) s.ops ;    (*BUG:PERF*)(* CAREFUL!! *)
    if not s.blocking 
      && s.xfer_stage = Null then 
	s.async ()
  in

  let disable () =
    log (fun () -> sprintf "disable:removing member %s" (string_of_endpt endpt)) ;
    let members = 
      try Hashtbl.find s.members group with Not_found ->
	eprintf "ACTUAL:join:disable:uncaught Not_found\n" ;
	exit 1
    in
    Hashtbl.remove s.members group ;
    let members = List.filter (fun m -> Member.endpt m <> endpt) members in
    Hashtbl.add s.members group members ;
  in
  
  let m = Member.create endpt to_client to_coord disable coord ltime in
  
  let members =
    try 
      let ret = Hashtbl.find s.members group in
      Hashtbl.remove s.members group ;
      ret
    with Not_found -> []
  in
  let members = m :: members in
  Hashtbl.add s.members group members ;
  log (fun () -> sprintf "#groups=%d" (hashtbl_size s.members)) ;
  Member.receive_client m
  
(**************************************************************)
  
let create alarm (ls_init,vs_init) =
  Util.disable_sigpipe () ;
  
  (* Ensure that we are using Total protocol.
   * Need local delivery.
   *)
  let properties = 
(*
    if vs_init.groupd then
      Property.Total :: Property.fifo
    else
*)
    Property.total
  in

  let vs_init = View.set vs_init [
    Vs_groupd false ;
    Vs_proto_id (Property.choose properties)
  ] in
  let ls_init = View.local name ls_init.endpt vs_init in
  
  (* Create the state of the group.
   *)
  let s = {
    vs = vs_init ;
    ls = ls_init ;
    announce = None ;
    async = Async.find (Alarm.async alarm) (vs_init.group,ls_init.endpt) ;
    blocking = false ;
    ops = Queuee.create () ;
    coords = Hashtbl.create 10 ;
    members = Hashtbl.create 10 ;
    groups = Hashtbl.create 10 ;
    xfer_stage = Null ;
    comb = []
  } in
  
  let announce group prop =
    try 
      (* BUG: should check that properties are the same. *)
      Hashtbl.find s.groups group ; ()
    with Not_found ->
      Hashtbl.add s.groups group prop ;
      if_some s.announce (fun handler ->
        handler group prop
      ) 
  in

  (* Handle a message receipt.
   *)
  let handle_msg msg = 
    log (fun () -> sprintf "got:%s\n" (string_of_msg msg)) ;
    match msg with
      | Announce(group,prop) ->
          announce group prop
      | Destroy(group) ->
          (try Hashtbl.remove s.groups group with Not_found -> ())
      | ToMembers(group,msg) ->
          (* Just pass message directly to the member.
           *)
	  (*log (fun () -> "ToMembers");*)
          members s group (fun m -> Member.receive_coord m msg)
      | ToCoord(group,endpt,rank,msg) ->
          (* Only handle message if I'm the requested rank.
           *)
          if s.ls.rank = rank then
            coord s group (fun c -> Coord.receive c (endpt,msg))
  in
  
  let clean_ops () =
    if s.xfer_stage = Null
      && not s.blocking then (
	let msgs = Queuee.to_list s.ops in
	Queuee.clear s.ops ;
	Array.of_list msgs
      ) else [||]
  in

  
  let install (ls,vs) = 
    let log = Trace.log2 name ls.name in

    (* Receive a the final state. 
      *)
    let recv_state (info,groups) = 
      log (fun () -> "recv_state");
      s.ls <- ls ;
      s.vs <- vs ;
      s.blocking <- false ;
      s.xfer_stage <- Null;
      if not !quiet then
	eprintf "ACTUAL:stable view:%d:%s\n" ls.nmembers (View.to_string s.vs.view) ;
      
      (* Strip all groups from the table.
       *)
      Hashtbl.clear s.coords ;
      (*hashtbl_clean s.coords ;*)
      
      (* Reconstruct coordinators.
       *)
      List.iter (fun (group,members) ->
	if ls.rank = 0 then (
	  coord s group (fun c -> 
	    let lists = list_split4 members in
	    Coord.reconstruct c vs.primary lists ; ()
	  )
	)
      ) info ;
      
      List.iter (fun (group,prop) ->
	announce group prop ; ()
      ) groups ;
      
      clean_ops ()
    in
    
    (* Recevied a message from another coord, describing
     * its list of groups and members.
     *)
    let recv_gather_msg origin (info,groups) = 
      assert (ls.rank = 0);
      log (fun () -> sprintf "%d->%d recv_gather_msg (nmembers=%d)" 
	origin 0 ls.nmembers);
      s.comb <- (info,groups) :: s.comb;
      
      if List.length s.comb = ls.nmembers then (
	let (members,groups) = List.split s.comb in
	s.comb <- [];
	
	(* Merge, sort, & organize the group information.
	 *)
	let members = List.flatten members in
	let members = 
	  List.map (fun (group,members) -> 
	    List.map (fun member -> group,member) members
	  ) members 
	in
	let members = List.flatten members in
	let members = list_join members in
	
	(* Combine all the groups' property lists.
	 *)
	let groups = List.fold_right Lset.union groups [] in
	
	log (fun () -> "Casting final M_State");
	let actions = recv_state (members,groups) in
	Array.append [|Cast(M_State(members,groups))|] actions
      ) else
	[||]
    in	      
    
    let receive origin _ _ msg = 
      log (fun () -> sprintf "%d->%d %s, msg=%s" 
	origin ls.rank
	(string_of_xfer_stage s.xfer_stage)
	(string_of_xfer_msg msg)
      );
      match s.xfer_stage,msg with
	  Null, M_Reg msg -> 
	    handle_msg msg;
	    clean_ops ()

	(* Gather messages are sent pt-2-pt. The leader
	 * does not send its local state to itself via Ensemble, 
	 * as this would be a bug (cannot send a message to oneself.
	 *)
	| Gather, M_Gather (info,groups) -> 
	    assert (ls.rank = 0);
	    recv_gather_msg origin (info,groups)

	(* The leader receives the new state first.
	 * It performs a local receive, and does not need the 
	 * sent message.
	 *)
	| Gather, M_State(info,groups) -> 
	    if ls.rank <> 0 then 
	      recv_state (info,groups)
	    else
	      [||]

	(* The leader receives its own state message.
	 *)
	| Null, M_State _ -> 
	    [||]

	| _,_ -> 
	    eprintf "Sanity, impossible (message,state) combination\n" ;
	    eprintf "origin=%d->%d %s,%s\n" 
	      origin ls.rank
	      (string_of_xfer_stage s.xfer_stage)
	      (string_of_xfer_msg msg);
	    exit 0

    and block () =
      let res = clean_ops () in
      s.blocking <- true ;
      res

    and heartbeat time =  clean_ops ()
    in
      
    let actions = 
      log (fun () -> sprintf "ACTUAL:install:%d:%s\n" 
	ls.nmembers (View.to_string vs.view)) ;

      (* Remove all membership messages from the send
       * queue.  The data we're sending now should replace them.
       * We leave in failure notifications, however.
       *)
      let l = Queuee.to_list s.ops in
      let l = List.filter (fun msg -> 
	match msg with
	  | Cast(M_Reg(ToCoord(_,_,_,Fail(_)))) -> true
	  | Send(_,M_Reg(ToCoord(_,_,_,Fail(_)))) -> true
	  | Cast(M_Reg(ToCoord _)) | Cast(M_Reg(ToMembers _))
	  | Send(_,M_Reg(ToCoord _)) | Send(_,M_Reg(ToMembers _)) -> false
	  | _ -> true
      ) l in
      Queuee.clear s.ops ;
      List.iter (fun m -> Queuee.add m s.ops) l ;
      
      let members = hashtbl_to_list s.members in
      let members = 
	List.map (fun (group,members) ->
          let members = List.map (fun m -> Member.summary m) members in
          (group, members)
	) members
      in
      
      (* Collect all the group properties.
       *)
      let groups = hashtbl_to_list s.groups in
      let groups = Lset.sort groups in
      
      (* Send everything to the group leader, who will 
       * merge everything together. 
       *)
      s.comb <- [];
      
      if ls.nmembers = 1 then ( 
	s.xfer_stage <- Null ;
	recv_state (members,groups)
      ) else (
	s.xfer_stage <- Gather ;
	if ls.rank <> 0 then (
	  log (fun () -> "sending gather to 0");
	  [|Send1(0, M_Gather(members,groups))|]
	) else (
	  recv_gather_msg 0 (members,groups)
	)
      )
    in
    
    let handlers = { 
      flow_block = (fun _ -> ());
      receive = receive ;
      block = block ;
      heartbeat = heartbeat ;
      disable = Util.ident
    } in
    actions,handlers
  in
  
  let interface = 
    full { 
      heartbeat_rate      = Time.of_int 
       (* (if Arge.get Arge.aggregate then 0.05 else 10.0) ;*)10 (* BUG *) ;
      install             = install ;
      exit                = (fun _ -> ())
    } in
(*
  let interface =
    if Arge.get Arge.aggregate then aggr interface else full interface
  in
*)
  
  (s,(ls_init,vs_init(* BUG? *)),interface)
  
let set_properties s member handler =
  s.announce <- Some handler ;
  ()
  
let announce s group filter properties =
  let msg = Announce(group,properties) in
  Queuee.add (Cast (M_Reg msg)) s.ops ;
  if not s.blocking 
  && s.xfer_stage = Null then 
    s.async ()

  
let destroy s group =
  let msg = Destroy(group) in
  Queuee.add (Cast (M_Reg msg)) s.ops ;
  if not s.blocking 
  && s.xfer_stage = Null then 
    s.async ()
  
(**************************************************************)
