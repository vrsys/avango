(**************************************************************)
(* DEBUG.MLI *)
(* Author: Mark Hayden, 11/96 *)
(**************************************************************)
open Trans

val client : Alarm.t -> inet -> port -> unit
val server : Alarm.t -> port -> unit
