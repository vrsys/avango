(**************************************************************)
(* APPL_INTF.ML: application interface *)
(* Author: Mark Hayden, 8/95 *)
(* See documentation for a description of this interface *)
(**************************************************************)
open Trans
open Util
open View
(**************************************************************)
let name = Trace.file "APPL_INTF"
let failwith s = Trace.make_failwith2 name s
let log = Trace.log name
(**************************************************************)

(* Some type aliases.
 *)
type dests = rank array
    
(**************************************************************)
    
type control =
  | Leave
  | Prompt
  | Suspect of rank list
      
  | XferDone
  | Rekey of bool 
  | Protocol of Proto.id
  | Migrate of Addr.set
  | Timeout of Time.t
      
  | Dump
  | Block of bool
  | No_op
      
      
type ('cast_msg,'send_msg) action =
  | Cast of 'cast_msg
  | Send of dests * 'send_msg
  | Send1 of rank * 'send_msg
  | Control of control
      
(**************************************************************)
      
let string_of_control = Trace.debug "" (function
  | Suspect sus -> sprintf "Suspect(%s)" (string_of_int_list sus)
  | Protocol p -> sprintf "Protocol(_)"
  | Block b -> sprintf "Block(%b)" b
  | Migrate _ -> sprintf "Migrate(_)"
  | Timeout _ -> sprintf "Timeout(_)"
  | Leave -> "Leave"
  | Prompt -> "Prompt"
  | Dump -> "Dump"
  | Rekey _ -> "Rekey(_)"
  | XferDone -> "XferDone"
  | No_op -> "No_op"
)
  
let string_of_action = function
  | Cast _ -> sprintf "Cast(_)"
  | Send(dest,_) -> sprintf "Send(%s,_)" (string_of_int_array dest)
  | Send1(dest,_) -> sprintf "Send1(%d,_)" dest
  | Control c ->
      sprintf "Control(%s)" (string_of_control c)
      
(**************************************************************)
      
let action_map fc fs = function
  | Cast m -> 
      (*printf "marsh(\n";*)
      let m = fc m in
      (*printf ")\n";*)
      Cast(m)
  | Send(d,m) -> Send(d,(fs m))
  | Send1(d,m) -> Send1(d,(fs m))
  | Control o -> Control o
      
let action_array_map fc fs = Array.map (action_map fc fs)
  
(**************************************************************)
  
  
module New = struct
  
  let null _ = [||]
    
  type cast_or_send = C | S
      
  type blocked = U | B
      
  type 'msg naction = ('msg,'msg) action
      
  type 'msg handlers = {
    flow_block : rank option * bool -> unit ;
    block : unit -> 'msg naction array ;
    heartbeat : Time.t -> 'msg naction array ;
    receive : origin -> blocked -> cast_or_send -> 'msg -> 'msg naction array ;
    disable : unit -> unit
  } 
      
  type 'msg full = {
    heartbeat_rate : Time.t ;
    install : View.full -> ('msg naction array) * ('msg handlers) ;
    exit : unit -> unit
  } 
      
  type t = Iovecl.t full
      
  type 'msg power = ('msg * Iovecl.t) full
      
  (**************************************************************)
      
  let string_of_cs = function C -> "C" | S ->"S"
  let string_of_blocked = function B -> "B" | U -> "U"
    
  (**************************************************************)
    
  let failed1 _ = failwith sanity
  let failed4 _ _ _ _ = failwith sanity
  let failed_handlers = {
    flow_block = failed1 ;
    heartbeat = failed1 ;
    receive = failed4 ;
    block = failed1 ;
    disable = failed1
  } 
    
  let full i =
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
	flow_block = handlers.flow_block;
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
end
  
(* These are put here for dynamic linking reasons.
 *)
type appl_lwe_header =
  | LCast of origin
  | LSend of origin * rank array
      
type appl_multi_header = int
    
(**************************************************************)
