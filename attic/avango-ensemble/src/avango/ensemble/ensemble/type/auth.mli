(**************************************************************)
(* AUTH.MLI *)
(* Authors: Mark Hayden, Ohad Rodeh, 8/96 *)
(**************************************************************)
(* This file gives a generic interface to authentication
 * services. *)
(**************************************************************)
open Trans
(**************************************************************)

type t

type clear = string 			(* cleartext *)
type cipher = string 			(* ciphertext *)

val lookup : Addr.id -> t

val principal : t -> Addr.id -> string -> Addr.t
(**************************************************************)

(* This is assumed to be in base64. 
*)
type ticket

val string_of_ticket : ticket -> string
val ticket_of_string : string -> ticket

val ticket : bool (*simulation?*)-> Addr.set(*me*) -> Addr.set(*him*) -> clear -> ticket option

val bckgr_ticket : bool (*simulation?*)-> Addr.set(*me*) -> Addr.set(*him*) ->
 	clear -> Alarm.t -> (ticket option -> unit) -> unit

val check : bool (*simulation?*)-> Addr.set(*me*) -> Addr.set (* him *) -> ticket -> clear option

val bckgr_check : bool (*simulation?*)-> Addr.set(*me*) -> Addr.set (*him*) -> 
	ticket -> Alarm.t -> (clear option -> unit) -> unit

type data = 
  | Clear of clear option
  | Ticket  of ticket option
(**************************************************************)

val create : 
  name ->
  (Addr.id -> string -> Addr.t) ->
  (Addr.id -> Addr.set -> Addr.set -> clear -> cipher option) ->
  (Addr.id -> Addr.set -> Addr.set -> clear -> Alarm.t -> 
    (cipher option -> unit) -> unit) -> 
  (Addr.id -> Addr.set -> Addr.set -> cipher -> clear option) ->
  (Addr.id -> Addr.set -> Addr.set -> cipher -> Alarm.t -> 
    (clear option -> unit) -> unit) -> 
  t

val install : Addr.id -> t -> unit

(**************************************************************)
