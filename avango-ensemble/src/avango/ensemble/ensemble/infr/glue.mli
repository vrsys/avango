(**************************************************************)
(* GLUE.MLI *)
(* Author: Mark Hayden, 10/96 *)
(**************************************************************)
open Trans
(**************************************************************)

type glue = 
  | Imperative 
  | Functional

type ('state,'a,'b) t

val of_string : string -> glue

val convert : glue -> ('bel,'abv,'state) Layer.basic -> ('state,'bel,'abv) t

val compose : ('s1,'abv,'mid) t -> ('s2,'mid,'bel) t -> ('s1*'s2,'abv,'bel) t

type ('state,'top,'bot) init = ('state,'top,'bot) t -> 
  'top ->
  'bot ->
  Alarm.t ->
  (Addr.id -> int) ->
  Layer.state ->
  View.full ->
  (Event.up -> unit) -> 
  (Event.dn -> unit)

(*
val init : ('top,'bot) init
*)
val init : ('state,'top,('a,'b,'c)Layer.msg) init

(**************************************************************)

module type S =
  sig
    type ('state,'a,'b) t
    val convert : ('a,'b,'state) Layer.basic -> ('state,'a,'b) t
    val revert  : ('state,'a,'b) t -> ('a,'b,'state) Layer.basic
    val wrap_msg : ('a -> 'b) -> ('b -> 'a) -> ('c, 'd, 'a) t -> ('c, 'd, 'b) t
    val compose : ('s1,'a,'b) t -> ('s2,'b,'c) t -> ('s1*'s2,'a,'c) t
    type ('state,'top,'bot) init = 
      ('state,'top,'bot) t -> 
      'top -> 'bot ->
      Alarm.t ->
      (Addr.id -> int) ->
      Layer.state ->
      View.full ->
      (Event.up -> unit) -> 
      (Event.dn -> unit)

    val init : ('state,'top,('a,'b,'c) Layer.msg) init
  end

(**************************************************************)

val inject_init : debug -> Sched.t -> (Event.up -> 'a -> unit) -> 'a -> unit

(**************************************************************)

module Functional : S with
  type ('state,'b,'a) t = 
    Layer.state -> 
    View.full -> 
    'state * (('state * ('a,'b) Event.dirm) -> 
              ('state * ('b,'a) Event.dirm Fqueue.t))
