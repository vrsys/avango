(**************************************************************)
(* PRIQ.MLI: heap-based priority queues *)
(* Author: Mark Hayden, 3/96 *)
(**************************************************************)

type ('key,'data) t

(* The 'key arg is the zero value for the keys.
 * The function is a test for greater-or-equal.
 *)
val create 	: 'key -> 'data -> ('key -> 'key -> int) -> ('key,'data) t

(* Add an item.
 *)
val add 	: ('key,'data) t -> 'key -> 'data -> unit

val get 	: ('key,'data) t -> ('key -> 'data -> unit) -> 'key -> bool

(* GETOPT: specialized form of "get" (note that the callback
 * is made with the value passed in instead of the interned 
 * value.
 *)
val getopt 	: ('key,'key -> unit) t -> 'key -> bool

val size 	: ('key,'data) t -> int
val min 	: ('key,'data) t -> 'key	(* BUGGY *)
val take 	: ('key,'data) t -> 'data
val empty 	: ('key,'data) t -> bool
val to_list	: ('key,'data) t -> ('key * 'data) list
val clear       : ('key,'data) t -> ('key -> 'data -> unit) -> unit

(**************************************************************)
(* Functorized version. 
 *)

module type S =
  sig
    type key
    type 'data t
    val create : 'data -> 'data t
    val add : 'data t -> key -> 'data -> unit
    val get : 'data t -> (key -> 'data -> unit) -> key -> bool
    val getopt : (key -> unit) t -> key -> bool
    val size : 'data t -> int
    val min : 'data t -> key	(* BUGGY *)
    val take : 'data t -> 'data
    val empty : 'data t -> bool
    val to_list	: 'data t -> (key * 'data) list
    val clear : 'data t -> (key -> 'data -> unit) -> unit
  end

module Make(Ord : Trans.OrderedType): (S with type key = Ord.t)

