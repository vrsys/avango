(**************************************************************)
(* APPL_POWER.ML *)
(* Author: Mark Hayden, 12/98 *)
(**************************************************************)
open Util
open View
open Appl_intf
(**************************************************************)
let name = Trace.file "APPL_POWER"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
(**************************************************************)

open New
let newf i =
  let ma,um = Iovecl.make_marsh true in
  let ta = action_array_map ma ma in

  let install vf =
    let actions,handlers = i.install vf in
    let actions = ta actions in
    let receive o b cs =
      let receive = handlers.receive o b cs in
      fun m -> ta (receive (um m))
    in
    
    let block () =
      ta (handlers.block ())
    in

    let heartbeat t =
      ta (handlers.heartbeat t)
    in

    let handlers = { 
      flow_block = handlers.flow_block ;
      heartbeat = heartbeat ;
      receive = receive ;
      block = block ;
      disable = handlers.disable
    } in
    
    actions,handlers
  in 
  
  { heartbeat_rate = i.heartbeat_rate ;
    install = install ;
    exit = i.exit }

(**************************************************************)

