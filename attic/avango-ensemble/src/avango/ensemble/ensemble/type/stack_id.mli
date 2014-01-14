(**************************************************************)
(* STACK_ID.MLI *)
(* Author: Mark Hayden, 3/96 *)
(**************************************************************)

type t =
  | Primary
  | Bypass
  | Gossip
  | Unreliable

val string_of_id : t -> string

val id_of_string : string -> t
