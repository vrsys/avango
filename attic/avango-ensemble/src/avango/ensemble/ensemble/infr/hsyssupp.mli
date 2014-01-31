(**************************************************************)
(* HSYSSUPP.MLI *)
(* Author: Mark Hayden, 6/96 *)
(**************************************************************)
open Trans
open Buf
(**************************************************************)

(* The send function is of the form: [hdr ofs len iovl].
 * The first header is composed of [ml_len iovl_len]
 *)
type 'a conn =
    debug -> 
      (Buf.t -> ofs -> len -> Iovecl.t -> unit) -> (* A send function *)
	((Buf.t -> Iovecl.t -> unit) (* A receive function *)
	* (unit -> unit)    (* A disable function *)
	* 'a)               (* An application state *)


(* Turn a TCP socket into a send and recv function.
 *)

val server : 
  debug -> 
  Alarm.t ->
  port ->
  unit conn ->
  unit
     
val client : 
  debug -> 
  Alarm.t ->
  Hsys.socket ->
  'a conn ->
  'a


(* 
 * [connect debug port hosts balance repeat]
 * try connecting through a port to one out of a list of hosts waiting
 * for TCP connections. the last two flags control whether to use
 * load-balancing, and if to repeat upon failure.
*)
val connect : debug -> port -> inet list -> bool -> bool -> Hsys.socket
