(**************************************************************)
(* ASYNC.MLI *)
(* Author: Mark Hayden, 4/97 *)
(**************************************************************)

type t

val create : Sched.t -> t

(* Async.add: called by "receiver" to register a callback.
 * It returns disable function.
 *)
val add : t -> (Group.id * Endpt.id) -> (unit -> unit) -> (unit -> unit) 

(* Find is called by "sender" to generate new callback.
 *)
val find : t -> (Group.id * Endpt.id) -> (unit -> unit)

