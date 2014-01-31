(**************************************************************)
(* ACTUAL.MLI *)
(* Author: Mark Hayden, 11/96 *)
(* Designed with Roy Friedman *)
(**************************************************************)
open Trans
open Mutil
(**************************************************************)

type ('m,'g) t

val create : Alarm.t -> View.full -> (('m,'g) t * View.full * Appl_intf.New.t)

val join : ('m,'g) t -> group -> endpt -> ltime ->
  (coord_msg -> unit) -> (member_msg -> unit)

val set_properties : ('m,'g) t -> 'c -> (group -> 'g -> unit) -> unit

val announce : ('m,'g) t -> group -> 'c -> 'g -> unit

val destroy : ('m,'g) t -> group -> unit

(**************************************************************)
