(**************************************************************)
(* FQUEUE.MLI : functional queues *)
(* Author: Mark Hayden, 4/95 *)
(**************************************************************)

type 'a t

(* Check if empty.
 *)
val is_empty : 'a t -> bool
    
(* An empty queue.
 *)
val empty: 'a t

(* [add x q] adds the element [x] at the end of the queue [q]. 
 *)
val add: 'a -> 'a t -> 'a t

(* [take q] removes and returns the first element in queue
 * [q], or fails if the queue is empty. 
 *)
val take: 'a t -> 'a * 'a t

(* [peek q] returns the first element in queue [q], without
 * removing it from the queue, or fails if the
 * queue is empty.  
 *)
val peek: 'a t -> 'a

(* Return the number of elements in a queue. 
 *)
val length: 'a t -> int

(* [iter f q] applies [f] in turn to all elements of [q],
 * from the least recently entered to the most recently
 * entered.  The queue itself is unchanged.  
 *)
val iter: ('a -> unit) -> 'a t -> unit

val map: ('a -> 'b) -> 'a t -> 'b t

(* [fold f i [a;b;c]] is (f a (f b (f c i))).
 *)
val fold : ('a -> 'b -> 'a) -> 'a -> 'b t -> 'a

(* Loop is similar to fold except that each time an entry is
 * passed in, the remainder of the queue is also passed in
 * and returned with the new "state".  If the queue is still
 * not empty then the function is called again.  
 *)
val loop : ('a -> 'b t -> 'b -> 'a * 'b t) -> 'a -> 'b t -> 'a

(* Conversions to/from lists.
 *)
val to_list : 'a t -> 'a list
val of_list : 'a list -> 'a t

val of_array : 'a array -> 'a t
