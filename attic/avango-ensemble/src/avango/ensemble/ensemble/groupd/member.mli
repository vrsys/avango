(**************************************************************)
(* MEMBER.MLI *)
(* Author: Mark Hayden, 8/97 *)
(* Designed with Roy Friedman *)
(**************************************************************)
open Trans
open Mutil
(**************************************************************)
(* STATE: A member is in one of 3 states:
 * Normal: normal state
 * Syncing: in the process of synchronizing.
 * Syncd: finished synchronizing.
 *)

type state = Normal | Syncing | Syncd

(* MEMBER.T: Type of members.
 *)
type t          

(* CREATE: create a new member.
 *)
val create :
  endpt ->
  (coord_msg -> unit) ->
  (member_msg -> unit) ->
  (unit -> unit) ->
  rank ->
  ltime ->
  t

(* ENDPT: project endpt field from member's state.
 *)
val endpt : t -> endpt

(* SUMMARY: get tuple summarizing member's state.
 *)
val summary : t -> (endpt * state * ltime * (endpt list))  

(* RECIEVE_CLIENT: handle a message from a client.
 *)
val receive_client : t -> member_msg -> unit

(* RECEIVE_COORD: handle a message from the coordinator.
 *)
val receive_coord : t -> coord_msg -> unit

(**************************************************************)
