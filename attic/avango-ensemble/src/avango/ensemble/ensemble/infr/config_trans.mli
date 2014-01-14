(**************************************************************)
(* CONFIG_TRANS.MLI *)
(* Author: Mark Hayden, 10/96 *)
(**************************************************************)

val f :
  Alarm.t ->
  (Addr.id -> int) ->
  ('local,'abv,'hdr) Layer.msg -> View.full ->
  (Event.t -> ('local,'abv,'hdr) Layer.msg -> unit) ->
  (Event.t -> ('local,'abv,'hdr) Layer.msg -> unit)
