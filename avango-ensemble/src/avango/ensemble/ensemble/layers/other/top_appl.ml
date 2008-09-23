(**************************************************************)
(* TOP_APPL.ML *)
(* Author: Mark Hayden, 8/95 *)
(**************************************************************)
open Trans
open Layer
open Util
open View
open Event
open Appl_intf
open Appl_intf.New
(**************************************************************)
let name = Trace.filel "TOP_APPL"
let failwith s = Trace.make_failwith name s
let log_iov = Trace.log (name^":IOV")
(**************************************************************)

type header = 
  | NoHdr
  | VsyncMsg

let string_of_header = function
  | NoHdr -> "NoHdr"
  | VsyncMsg -> "VsyncMsg"

type blocking = 
  | Unblocked
  | DnBlocking
  | DnBlocked
  | UpBlocking
  | UpBlocked

let string_of_blocking = function
  | Unblocked  -> "Unblocked"
  | DnBlocking -> "DnBlocking"
  | DnBlocked  -> "DnBlocked"
  | UpBlocking -> "UpBlocking"
  | UpBlocked  -> "UpBlocked"

type state = {
  recv_cast             : (Iovecl.t -> Iovecl.t naction array) array ;
  recv_send             : (Iovecl.t -> Iovecl.t naction array) array ;
  interface		: Appl_intf.New.t ;
  send_xmit             : seqno array ;
  send_recv             : seqno array ;
  cast_recv             : seqno array ;
  mutable cast_xmit     : seqno ;
  mutable got_expect    : bool ;
  mutable cast_expect   : seqno Arrayf.t ;
  mutable send_expect   : seqno Arrayf.t ;
  mutable handlers      : Iovecl.t handlers ;
  mutable leaving	: bool ;
  mutable next_sweep	: Time.t ;
  mutable failed        : bool Arrayf.t ;
  mutable blocking      : blocking
} 

let unblocked s = s.blocking = Unblocked || s.blocking = DnBlocking

(**************************************************************)

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "got_expect=%b blocking=%s\n" s.got_expect (string_of_blocking s.blocking) ;
  sprintf "failed=%s\n" (Arrayf.bool_to_string s.failed) ;
  sprintf "cast_xmit=%d\n" s.cast_xmit ;
  sprintf "cast_recv  =%s\n" (string_of_int_array s.cast_recv) ;
  sprintf "cast_expect=%s\n" (Arrayf.int_to_string s.cast_expect) ;
  sprintf "send_xmit  =%s\n" (string_of_int_array s.send_xmit) ;
  sprintf "send_recv  =%s\n" (string_of_int_array s.send_recv) ;
  sprintf "send_expect=%s\n" (Arrayf.int_to_string s.send_expect)
|])

(**************************************************************)

let init_handlers () = {
  flow_block = (fun _       -> failwith "sanity:flow_block") ;
  receive    = (fun _ _ _ _ -> failwith "sanity:receive") ;
  heartbeat  = (fun _       -> failwith "sanity:heartbeat") ;
  block      = (fun _       -> failwith "sanity:block") ;
  disable    = (fun _       -> failwith "sanity:disable")
} 

(**************************************************************)

let init s (ls,vs) = 
  let interface = s.Layer.interface in
  if Time.ge Time.zero interface.heartbeat_rate then
    failwith "TOP_APPL:non-positive heartbeat rate" ;
  { 
    interface   = interface ;
    handlers    = init_handlers () ;
    recv_cast   = Array.create ls.nmembers (fun _ -> failwith "sanity:recv_cast") ;
    recv_send   = Array.create ls.nmembers (fun _ -> failwith "sanity:recv_send") ;
    send_xmit   = Array.create ls.nmembers 0 ;
    send_recv   = Array.create ls.nmembers 0 ;
    cast_recv   = Array.create ls.nmembers 0 ;
    cast_xmit   = 0 ;
    cast_expect = ls.zeroes ;
    send_expect = ls.zeroes ;
    leaving     = false ;
    next_sweep  = Time.zero ;	
    got_expect  = false ;
    failed      = ls.falses ;
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

  let check_block_ok () =
    if s.blocking = UpBlocking
    && s.got_expect
    then  (
      let all_ok = ref true in
      for rank = 0 to pred ls.nmembers do
	if rank <> ls.rank then (
	  if s.cast_recv.(rank) < Arrayf.get s.cast_expect rank then
	    all_ok := false ;
	  if (not (Arrayf.get s.failed rank))
	    && s.send_recv.(rank) < Arrayf.get s.send_expect rank
	  then 
	    all_ok := false
	)
      done ;
      if !all_ok then (
	logs (fun () -> sprintf "release EBlockOk (cast=%s)" (string_of_int_array s.cast_recv)) ;
	s.blocking <- UpBlocked ;
	upnm (create name EBlockOk[])
      )
    )
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
	  dnnm (create name EBlock[]) ;
	  check_block_ok ()
	)
    | No_op -> ()
  in
  
  let handle_actions actions =
    for i = 0 to pred (Array.length actions) do 
      match actions.(i) with
      | Cast(msg) ->
	  assert (unblocked s) ;
	  s.cast_xmit <- succ s.cast_xmit ;
	  log_iov (fun () -> sprintf "Cast: iovl=%s,len=%d" 
	    (Iovecl.sum_refs msg) (Buf.int_of_len (Iovecl.len msg)));
	  log (fun () -> "cast appl message");
      	  dnlm (castIovAppl name msg) ()

      (* Increment the iovecl count for each destination.
       *)
      | Send(dest,msg) ->
	  assert (unblocked s) ;
	  let len = Array.length dest in
	  if len =| 0 then (
	    eprintf "TOP_APPL:dest=[||],rank=%d,nmembers=%d\n" ls.rank ls.nmembers ;
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
	      eprintf "TOP_APPL:dest=%d,rank=%d,nmembers=%d\n" dest ls.rank ls.nmembers ;
	      failwith "bad destination" ;
	    ) ;
	    array_incr s.send_xmit dest ;
	    dnlm (sendPeerIovAppl name dest msg) ()
	  done
      | Send1(dest,msg) ->
	  log_iov (fun () -> sprintf "Send1: iovl=%s,len=%d" 
	    (Iovecl.sum_refs msg) (Buf.int_of_len (Iovecl.len msg)));
	  assert (unblocked s) ;
	  if dest =| ls.rank then
	    log (fun () -> sprintf "send to myself dest=%d" dest) ;
	  if (* dest =| ls.rank || *) dest <| 0 || dest >=| ls.nmembers then (
	    eprintf "TOP_APPL:dest=%d,rank=%d,nmembers=%d\n" dest ls.rank ls.nmembers ;
	    failwith "bad destination" ;
	  ) ;
	  array_incr s.send_xmit dest ;
	  dnlm (sendPeerIovAppl name dest msg) ()
      | Control ctl -> handle_control ctl
    done
  in

  let up_hdlr ev abv hdr = match getType ev,hdr with
    | ECast _, VsyncMsg -> 
	let origin = getPeer ev in
	array_incr s.cast_recv origin ;
	up ev abv 
	
    | ESend _ , VsyncMsg -> 
	let origin = getPeer ev in
	array_incr s.send_recv origin;
	up ev abv 

    | _ -> up ev abv 

  and uplm_hdlr ev () = match getType ev with
  | ECast iovl ->
      let origin = getPeer ev in
      array_incr s.cast_recv origin ;
(*    log (fun () -> sprintf "%d:cast_recv=%d xmit=%d" origin (s.cast_recv.(origin)) s.cast_xmit) ;*)
      if s.got_expect && s.cast_recv.(origin) >| Arrayf.get s.cast_expect origin then
	failwith ("bad cast:"^(Event.to_string ev)) ;
      log_iov (fun () -> sprintf "recv Cast: iovl=%s,len=%d" 
	(Iovecl.sum_refs iovl) (Buf.int_of_len (Iovecl.len iovl)));
      handle_actions (s.recv_cast.(origin) iovl) ;
      if s.blocking = UpBlocking then
	check_block_ok () ;

  | ESend iovl ->
      let origin = getPeer ev in
      array_incr s.send_recv origin ;
      if s.got_expect && s.send_recv.(origin) >| Arrayf.get s.send_expect origin then
	failwith "bad send" ;
      log_iov (fun () -> sprintf "recv Send: iovl=%s,len=%d" 
	(Iovecl.sum_refs iovl) (Buf.int_of_len (Iovecl.len iovl)));
      handle_actions (s.recv_send.(origin) iovl) ;
      if s.blocking = UpBlocking then
	check_block_ok () ;
     
  | _ -> failwith "got bad uplm event"

  and upnm_hdlr ev = match getType ev with
  | EInit ->
      (* Send the event on.
       *)
      upnm ev ;

      let actions, handlers = s.interface.install (ls,vs) in 
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

    (* Add message count info into Block and Fail events.
     * Other layers should not use this if we have not
     * yet blocked.
     *)
  | EBlock | EFail ->
      if getType ev = EFail then
	s.failed <- getFailures ev ;
      let cast_recv = Arrayf.of_array s.cast_recv in

      let cast_recv =
	Arrayf.fset cast_recv ls.rank s.cast_xmit
      in

      logs (fun () -> sprintf "EBlock|EFail cast_recv=%s"
	(Arrayf.int_to_string cast_recv)) ;
      let ev = set name ev [
	ApplCasts cast_recv ;
	ApplSends (Arrayf.of_array s.send_xmit)
      ]	in
      upnm ev ;
      check_block_ok ()

  | ESyncInfo ->
      logs (fun () -> "got ESyncInfo") ;
      s.got_expect <- true ;
      s.cast_expect <- getApplCasts ev ;
      s.send_expect <- getApplSends ev ;
      for i = 0 to pred ls.nmembers do
	if s.cast_recv.(i) > Arrayf.get s.cast_expect i then
	  failwith sanity
      done ;
      check_block_ok () ;
      upnm ev

  | EBlockOk -> 
      assert (s.blocking = DnBlocked) ;
      if getNoVsync ev then upnm ev
      else (
      	logs (fun () -> sprintf "capture EBlockOk") ;
	s.blocking <- UpBlocking ;
      	check_block_ok () ;
      )

  | EFlowBlock -> 
      let fb = getFlowBlock ev in
      s.handlers.flow_block fb ;

  | EAccount ->
      logb (fun () -> [
  	sprintf "blocking=%s" (string_of_blocking s.blocking) ;
	sprintf "failed     =%s" (Arrayf.bool_to_string s.failed) ;
  	sprintf "cast_recv  =%s" (string_of_int_array s.cast_recv) ;
  	sprintf "cast_expect=%s" (Arrayf.int_to_string s.cast_expect) ;
  	sprintf "send_recv  =%s" (string_of_int_array s.send_recv) ;
  	sprintf "send_expect=%s" (Arrayf.int_to_string s.send_expect)
      ]) ;
      upnm ev

  | EDump -> dump vf s ; upnm ev
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
    (* Hack!!! Add NoTotal flag to all messages from above.
     * (Ohad) added support for VS events above top_appl, by using 
     * the ApplMsg flag.
    *)
  | ECast _ -> 
      let ev = setNoTotal name ev in
      if getForceVsync ev then (
	s.cast_xmit <- succ s.cast_xmit ;
	dn ev abv VsyncMsg
      ) else dn ev abv NoHdr


  | ESend _ -> 
      (* BUG: is no total needed here?
       *)
      let ev = setNoTotal name ev in
      if getForceVsync ev then (
	array_incr s.send_xmit (getPeer ev) ;
	dn ev abv VsyncMsg
      ) else dn ev abv NoHdr

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr ev = match getType ev with
    (* Block this member.
     *)
  | EBlock -> 
      assert (s.blocking = Unblocked) ;

      (* Only deliver this if we are not leaving.
       *)
      let actions =
	if s.leaving then [||] else (
	  s.handlers.block () 
	)
      in
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
let l2 args vs = Layer.hdr_noopt init hdlrs args vs

let _ = Layer.install name l

(**************************************************************)
