(**************************************************************)
(* TIMESTAMP.MLI *)
(* Author: Mark Hayden, 4/97 *)
(**************************************************************)

type t

val register : Trans.debug -> t

val add : t -> unit

val print : unit -> unit

