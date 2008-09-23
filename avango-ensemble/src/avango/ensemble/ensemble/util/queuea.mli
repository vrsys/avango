(**************************************************************)
(* QUEUEA.ML *)
(* Author: Mark Hayden, 10/97 *)
(**************************************************************)
(* Queues *)

(* This module implements queues (FIFOs), with in-place modification. *)

type 'a t
        (* The type of queues containing elements of type ['a]. *)

val create: 'a -> 'a t
        (* Return a new queue, initially empty. *)
val add: 'a t -> 'a -> unit
        (* [add x q] adds the element [x] at the end of the queue [q]. *)
val empty : 'a t -> bool
        (* Take a guess *)
(*
val take: 'a t -> 'a
        (* [take q] removes and returns the first element in queue [q],
           or raises [Empty] if the queue is empty. *)
*)
val take_unsafe: 'a t -> 'a (* Assumes queue length > 0 *)
val clear : 'a t -> unit
        (* Discard all elements from a queue. *)
val length: 'a t -> int
        (* Return the number of elements in a queue. *)
val iter: ('a -> 'b) -> 'a t -> unit
        (* [iter f q] applies [f] in turn to all elements of [q], from the
           least recently entered to the most recently entered.
           The queue itself is unchanged. *)

(**************************************************************)

(* Operations for managing mutable queues.  These are the
 * same as above except that me maintain a pool of active
 * data elements for the user.  This means that the user
 * needs to "zero" the records after a take_mut and to fill
 * in the records after an add_mut.  
 *)
(* Note that the "zeroing" must occur immediately otherwise
 * we have a race condition.
 *)
val create_mut : (unit -> 'a) -> 'a t
val add_mut : 'a t -> 'a
val take_mut_unsafe : 'a t -> 'a

(**************************************************************)
