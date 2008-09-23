(**************************************************************)
(* ONCE.MLI : set-once variables *)
(* Author: Mark Hayden, 12/96 *)
(**************************************************************)

type t

val create : string -> t

val set : t -> unit

val isset : t -> bool

val to_string : t -> string
