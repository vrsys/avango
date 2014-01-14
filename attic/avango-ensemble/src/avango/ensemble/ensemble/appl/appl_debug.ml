(**************************************************************)
(* APPL_DEBUG.ML: application interface *)
(* Author: Mark Hayden, 11/97 *)
(**************************************************************)
open Trans
open Util
open View
open Appl_intf 
  open New
(**************************************************************)
let name = Trace.file "APPL_DEBUG"
let failwith = Trace.make_failwith name
let log = Trace.log name
(**************************************************************)

let debug_msg name =
  eprintf "APPL:%s\n" name

let al al =
  Array.iter (function
    | Cast(_)       -> debug_msg "CAST"
    | Send(dests,_) -> debug_msg "SEND"
    | Send1(dest,_) -> debug_msg "SEND1"
    | Control o -> (
	match o with
	| Leave    -> debug_msg "LEAVE"
	| XferDone -> debug_msg "XFER_DONE"
	| Dump     -> debug_msg "DUMP"
	| Prompt   -> debug_msg "PROMPT"
	| Protocol p ->
	    let msg = sprintf "PROTOCOL:%s" (Proto.string_of_id p) in
	    debug_msg msg
	| _ -> ()
    )
  ) al ;
  al

(**************************************************************)

let f debug intf =
  let install (ls,vs) = 
    debug_msg (sprintf "INSTALL:%s" (View.string_of_full (ls,vs))) ;
    let (actions,handlers) = intf.install (ls,vs) in
    ignore (al actions) ;

    let block () =
      debug_msg "BLOCK" ;
      al (handlers.block ())
    and heartbeat time =
      debug_msg (sprintf "HEARTBEAT(%s)" (Time.to_string time)) ;
      al (handlers.heartbeat time)
    and receive origin blocked cs =
      let receive = handlers.receive origin blocked cs in
      let debug = sprintf "RECEIVE:%d:%s:%s" origin 
	  (string_of_blocked blocked)
	  (string_of_cs cs)
      in
      fun msg -> 
	debug_msg debug ;
	al (receive msg)
    and disable () =
      debug_msg "DISABLE" ;
      handlers.disable ()
    in actions,{flow_block=handlers.flow_block;
		 receive=receive;heartbeat=heartbeat;disable=disable;block=block}
  and exit () =
    debug_msg "EXIT" ;
    intf.exit ()
  in {
    install = install ;
    exit = exit ;
    heartbeat_rate = intf.heartbeat_rate
  }   

let view debug intf =
  let install (ls,vs) = 
    let uptime = truncate (Time.to_float (Time.sub (Time.gettimeofday ()) vs.uptime)) in
    let uptime = uptime / 3600 in
    eprintf "%s:view:ltime=%d:uptime=%d hrs:#=%d:%s\n" debug vs.ltime uptime ls.nmembers (View.to_string vs.view) ;
    let (actions,handlers) = intf.install (ls,vs) in
    let block () =
      eprintf "%s:blocking for view\n" debug ;
      handlers.block ()
    in
    (actions,{flow_block=handlers.flow_block;receive=handlers.receive;
	       heartbeat=handlers.heartbeat;disable=handlers.disable;block=block})
  in 
  
  {install=install;exit=intf.exit;heartbeat_rate=intf.heartbeat_rate}

(**************************************************************)

