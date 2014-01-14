(**************************************************************)
(* MCAUSAL.MLI : Causal ordering support module. *)
(* Author: Ohad Rodeh, 7/97 *)
(**************************************************************)
open Trans

type t
type s

val create_state : int -> t
val last_of      : t -> seqno array
val can_deliver  : t -> s -> seqno -> bool
val incr         : t -> seqno -> s
val update       : t -> s -> seqno -> unit



