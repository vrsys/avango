(**************************************************************)
(* STACKE.MLI *)
(* Author: Mark Hayden, 4/96 *)
(**************************************************************)

val config_full : 
  Glue.glue ->				(* glue *)
  Alarm.t ->
  (Addr.id -> int) ->
  Layer.state ->			(* application *)
  View.full ->				(* view state *)
  (Event.up -> unit) ->			(* events out of top *)
  (Event.dn -> unit)			(* events into top *)

(**************************************************************)

val config :
  Glue.glue ->				(* glue *)
  Alarm.t ->
  (Addr.id -> int) ->
  Layer.state ->			(* application *)
  View.full ->				(* view state *)
  unit

(**************************************************************)
