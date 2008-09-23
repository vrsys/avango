(**************************************************************)
(* REQUEST.MLI *)
(* Author: Mark Hayden, 4/95 *)
(**************************************************************)
(* Request Queues : Maintain linked list of requests in the
 * order that they were made.  Supports insert, remove, take
 * operations with amortized O(1) time and no memory
 * allocation. *)
(**************************************************************)

type t

val create: int -> t
val add: t -> int -> unit
val remove: t -> int -> unit
val take: t -> int
val to_string : t -> string
