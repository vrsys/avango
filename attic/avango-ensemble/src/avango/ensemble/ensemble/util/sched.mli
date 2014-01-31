(**************************************************************)
(* SCHED.MLI: call-back scheduling *)
(* Author: Mark Hayden, 12/95 *)
(**************************************************************)
open Trans

type t

(* Create a scheduling queue.
 *)
val create : debug -> t

(* Enqueue a function and its arguments.
 *)
val enqueue : t -> debug -> (unit -> unit) -> unit
val enqueue_1arg : t -> debug -> ('a -> unit) ->  'a -> unit
val enqueue_2arg : t -> debug -> ('a -> 'b -> unit) -> 'a -> 'b -> unit
val enqueue_3arg : t -> debug -> ('a -> 'b -> 'c -> unit) -> 'a -> 'b -> 'c -> unit
val enqueue_4arg : t -> debug -> ('a -> 'b -> 'c -> 'd -> unit) -> 'a -> 'b -> 'c -> 'd -> unit
val enqueue_5arg : t -> debug -> ('a -> 'b -> 'c -> 'd -> 'e -> unit) -> 'a -> 'b -> 'c -> 'd -> 'e -> unit

(* Is it empty?
 *)
val empty : t -> bool

(* What's its size?
 *)
val size : t -> int

(* Execute this number of steps.  Returns true if
 * some steps were made.  The #steps is assumed to
 * be > 0.
 *)
val step : t -> int -> bool

(**************************************************************)

(* Global variable: #of scheduling steps taken so far.
 *)
val steps : int ref

(**************************************************************)
