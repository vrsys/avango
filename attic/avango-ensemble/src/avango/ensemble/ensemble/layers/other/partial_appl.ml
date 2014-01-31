(**************************************************************)
(* PARTIAL_APPL.ML *)
(* Author: Mark Hayden, 8/95 *)
(* Modified: Xiaoming Liu with Robbert vanRenesse 2/98 *)
(**************************************************************)
open Trans
open Layer
open Util
open View
open Event
open Appl_intf
open Appl_intf.New
(**************************************************************)
let name = Trace.filel "PARTIAL_APPL"
(**************************************************************)

let string_of_header = string_of_unit

type blocking = 
  | Unblocked
  | DnBlocking
  | DnBlocked

let string_of_blocking = function
  | Unblocked  -> "Unblocked"
  | DnBlocking -> "DnBlocking"
  | DnBlocked  -> "DnBlocked"

type state = {
  recv_cast             : (Iovecl.t -> Iovecl.t naction array) array ;
  recv_send             : (Iovecl.t -> Iovecl.t naction array) array ;
  interface		: t ;
  mutable handlers      : Iovecl.t handlers ;
  mutable leaving	: bool ;
  mutable next_sweep	: Time.t ;
  mutable blocking      : blocking
} 

let unblocked s = s.blocking = Unblocked || s.blocking = DnBlocking

(**************************************************************)

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "blocking=%s\n" (string_of_blocking s.blocking)
|])

(**************************************************************)

let init_handlers () = {
  flow_block = (fun _ -> failwith "sanity:flow_block") ;
  receive = (fun _ _ _ _ -> failwith "sanity:receive") ;
  heartbeat = (fun _ -> failwith "sanity:heartbeat") ;
  block = (fun _ -> failwith "sanity:block") ;
  disable = (fun _ -> failwith "sanity:disable")
} 

(**************************************************************)

let init s (ls,vs) = 
  let interface = s.Layer.interface in
  if Time.ge Time.zero interface.heartbeat_rate then
    failwith "PARTIAL_APPL:non-positive heartbeat rate" ;
  { 
    interface   = interface ;
    handlers    = init_handlers () ;
    recv_cast   = Array.create ls.nmembers (fun _ -> failwith "sanity:recv_cast") ;
    recv_send   = Array.create ls.nmembers (fun _ -> failwith "sanity:recv_send") ;
    leaving     = false ;
    next_sweep  = Time.zero ;	
    blocking    = Unblocked
  }

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in
  let logs = Trace.log3 Layer.syncing ls.name name in
  let logb = Trace.logl3 Layer.block ls.name name in

  let set_blocked blocked =
    for rank = 0 to pred (ls.nmembers) do 
      s.recv_cast.(rank) <- s.handlers.receive rank blocked C ;
      s.recv_send.(rank) <- s.handlers.receive rank blocked S ;
    done
  in

  let handle_control = function
    | Leave ->
	s.leaving <- true ;
	dnnm (create name ELeave[])
    | XferDone           -> dnnm (create name EXferDone[])
    | Protocol(proto_id) -> dnnm (create name EProtocol[ProtoId proto_id])
    | Migrate(addrs)     -> dnnm (create name EMigrate[Address addrs])
    | Rekey flg          -> dnnm (create name ERekey[RekeyFlag flg])
    | Dump               -> dnnm (create name EDump[])
    | Prompt             -> dnnm (create name EPrompt[])
    | Suspect ranks ->
	if List.mem ls.rank ranks then
	  failwith "I'm listed as a suspect" ;
	let suspicions = Arrayf.of_ranks ls.nmembers ranks in
	dnnm (suspectReason name suspicions "Application")
    | Timeout t ->
      	failwith "Timeout not supported"
    | Block b -> 
      (* Blocked(false) is handled in the block() case.
       *)
	if s.blocking = DnBlocking && b then (
	  s.blocking <- DnBlocked ;
	  set_blocked B ;
	  dnnm (create name EBlock[])
	)
    | No_op -> ()
  in
  
  let handle_actions actions =
(*    if Array.length actions > 0 then 
      log (fun () -> "actions");    *)
    for i = 0 to pred (Array.length actions) do 
      match actions.(i) with
      | Cast(msg) ->
	  assert (unblocked s) ;
      	  dnlm (castIovAppl name msg) ()
      | Send(dest,msg) ->
	  assert (unblocked s) ;
	  let len = Array.length dest in
	  if len =| 0 then (
	    eprintf "PARTIAL_APPL:dest=[||],rank=%d,nmembers=%d\n" ls.rank ls.nmembers ;
	    raise (Invalid_argument "Empty send list");
	  );
	  for i = 0 to pred len do
	    let msg = 
	      if i >| 0 then Iovecl.copy msg
	      else msg
	    in
	    let dest = dest.(i) in
	    if dest =| ls.rank then
	      log (fun () -> sprintf "send to myself dest=%d" dest) ;
	    if (* dest =| ls.rank || *) dest <| 0 || dest >=| ls.nmembers then (
	      eprintf "PARTIAL_APPL:dest=%d,rank=%d,nmembers=%d\n" dest ls.rank ls.nmembers ;
	      failwith "bad destination" ;
	    ) ;
	    dnlm (sendPeerIovAppl name dest msg) ()
	  done
      | Send1(dest,msg) ->
	  if dest =| ls.rank then
	    log (fun () -> sprintf "send to myself dest=%d" dest) ;
	  if (* dest =| ls.rank || *) dest <| 0 || dest >=| ls.nmembers then (
	    eprintf "PARTIAL_APPL:dest=%d,rank=%d,nmembers=%d\n" dest ls.rank ls.nmembers ;
	    failwith "bad destination" ;
	  ) ;
	  dnlm (sendPeerIovAppl name dest msg) ()
      | Control ctl -> handle_control ctl
    done
  in

  let up_hdlr ev abv () = up ev abv

  and uplm_hdlr ev () = match getType ev with
  | ECast iovl ->
      let origin = getPeer ev in
      handle_actions (s.recv_cast.(origin) iovl)

  | ESend iovl ->
      let origin = getPeer ev in
      handle_actions (s.recv_send.(origin) iovl)
     
  | _ -> failwith "got bad uplm event"

  and upnm_hdlr ev = match getType ev with
  | EInit ->
      (* Send the event on.
       *)
      upnm ev ;

      let actions,handlers = s.interface.install vf in 
      s.handlers <- handlers ;
      set_blocked U ;
      handle_actions actions ;

      (* Request first alarm.
       *)
      dnnm (timerAlarm name Time.zero)

    (* Wake up the application.
     *)
  | EAsync ->
      if unblocked s then
      	handle_actions (s.handlers.heartbeat (getTime ev)) ;
      upnm ev

  | ETimer ->
      let time = getTime ev in
      if Time.Ord.ge time s.next_sweep
      && unblocked s then (
      	(* Schedule a new heartbeat.
      	 *)
	s.next_sweep <- Time.add time s.interface.heartbeat_rate ;
	dnnm (timerAlarm name s.next_sweep) ;

      	(* Wake up the application.
	 *)
	handle_actions (s.handlers.heartbeat time)
      ) ;

      upnm ev

  | EExit ->
      s.handlers.disable () ;
      if s.leaving then (
	s.interface.exit ()
      ) ;
      upnm ev

  | EFlowBlock -> 
      let fb = getFlowBlock ev in
      s.handlers.flow_block fb

  | EAccount ->
      logb (fun () -> [
  	sprintf "blocking=%s" (string_of_blocking s.blocking)
      ]) ;
      upnm ev

  | EDump -> dump vf s ; upnm ev
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
    (* Hack!!! Add NoTotal flag to all messages from above.
     *)
  | ECast _ -> 
      let ev = setNoTotal name ev in
      dn ev abv ()

  | _ -> dn ev abv ()

  and dnnm_hdlr ev = match getType ev with
    (* Block this member.
     *)
  | EBlock -> 
      assert (s.blocking = Unblocked) ;

      let actions = s.handlers.block () in
      handle_actions actions ;

      (* This is set after the above statement so
       * that Block(true) in the actions will not
       * generate an EBlock event.
       *)
      s.blocking <- DnBlocking ;

      (* If Blocked(false) is one of the actions, do
       * not actually block.
       *)
      if not (array_mem (Control(Block false)) actions) then (
	set_blocked B ;
	s.blocking <- DnBlocked ;
	dnnm (create name EBlock[])
      )

  | _ -> dnnm ev


in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr init hdlrs None (LocalNoHdr ()) args vs

let _ = Layer.install name l

(**************************************************************)
