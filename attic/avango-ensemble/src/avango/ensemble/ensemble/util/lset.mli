(**************************************************************)
(* LSET.MLI *)
(* Author: Mark Hayden, 11/96 *)
(**************************************************************)
(* Efficient set-like operations for lists.

 * All functions that return lists, return ordered lists.

 * Set operations take linear time when input lists are in order.

 *) 
(**************************************************************)

(* [sort l] returns the list [l] where all items are
 * sorted.  If the list is already sorted, the same list is
 * returned. 
 *)
val sort : 'a list -> 'a list

(* [subtract l1 l2] returns the list [l1] where all elements
 * structurally equal to one of the elements of [l2] have
 * been removed. 
 *)
val subtract : 'a list -> 'a list -> 'a list

(* [union l1 l2] returns list with all of the items in [l1]
 * and [l2].  Duplicates are stripped. 
 *)
val union : 'a list -> 'a list -> 'a list

(* [intersect l1 l2] returns list of elements in both [l1] and
 * [l2].  
 *)
val intersect : 'a list -> 'a list -> 'a list

(* [super l1 l2] determines whether the items in [l1] are a
 * superset of those in [l2] (they may be equal). 
 *)
val super : 'a list -> 'a list -> bool

(* [disjoin l1 l2] determines whether [l1] and [l2] are
 * disjoint.  
 *)
val disjoint : 'a list -> 'a list -> bool

val collapse : 'a list -> 'a list

(**************************************************************)
type 'a t
val inject : 'a Arrayf.t -> 'a t
val project : 'a t -> 'a Arrayf.t

val sorta : 'a t -> 'a t
val subtracta : 'a t -> 'a t -> 'a t
val uniona : 'a t -> 'a t -> 'a t
val intersecta : 'a t -> 'a t -> 'a t
val supera : 'a t -> 'a t -> bool
val disjointa : 'a t -> 'a t -> bool

val is_empty : 'a t -> bool
val to_string : ('a -> string) -> 'a t -> string
(**************************************************************)
