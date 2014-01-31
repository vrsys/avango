(**************************************************************)
(* TRANSPORT.MLI *)
(* Author: Mark Hayden, 7/95 *)
(**************************************************************)
open Trans
(**************************************************************)

(* This is the type of object used to send messages.
 *)
type 'msg t

(* Construct and enable a new transport instance.
 *)
val f :
  Alarm.t ->
  (Addr.id -> int) ->
  View.full ->
  Stack_id.t ->				(* stack id *)
  'msg Route.t ->			(* connection table *)
  (Conn.kind -> bool -> 'msg) -> (* message handler *)
  'msg t

val f2 :
  Alarm.t ->
  (Addr.id -> int) ->
  View.full ->
  Stack_id.t ->				(* stack id *)
  'msg Route.t ->			(* connection table *)
  ('msg t -> ((Conn.kind -> bool -> 'msg) * 'a)) -> (* message handler *)
  'a
	
(**************************************************************)

val disable	: 'msg t -> unit	(* disable the transport*)
val send	: 'msg t -> rank -> 'msg (* send on the transport *)
val cast	: 'msg t -> 'msg	(* cast on the transport *)
val gossip	: 'msg t -> Addr.set option -> 'msg (* gossip on the transport *)

(**************************************************************)
