(**************************************************************)
(* DOMAIN.MLI *)
(* Author: Mark Hayden, 4/96 *)
(**************************************************************)
open Trans
open Buf
(**************************************************************)

(* DOMAIN.T: The type of a communication domain.
 *)

type t

(* DEST: this specifies the destinations to use in xmit and
 * xmitv.  For pt2pt xmits, we give a list of endpoint
 * identifiers to which to send the message.  For
 * multicasts, we give a group address and a boolean value
 * specifying whether loopback is necessary.  If no loopback
 * is requested then the multicast may not be delivered to
 * any other processes on the same host as the sender.
 *)

(* Note that significant preprocessing may be done on the
 * destination values.  For optimal performance, apply the
 * destination to an xmit outside of your critical path.
 *)

type loopback = bool

type dest =
| Pt2pt of Addr.set Arrayf.t
| Mcast of Group.id * loopback
| Gossip of Group.id

(**************************************************************)

type handle

(**************************************************************)
(* Operations on domains.
 *)

val name 	: t -> string		(* name of domain *)
val addr	: t -> Addr.id -> Addr.t (* create a local addresss *)
val enable 	: t -> Addr.id -> Group.id -> Addr.set -> View.t -> handle
val disable 	: handle -> unit
val xmit 	: handle -> dest -> Route.xmitf option (* how to xmit *)

(**************************************************************)

(* Create a domain.
 *)
val create : 
  name ->				(* name of domain *)
  (Addr.id -> Addr.t) ->		(* create endpoint address *)
  (Addr.id -> Group.id -> Addr.set -> View.t -> handle) -> (* enable new transport *)
  t

val handle :
  (unit -> unit) -> (* disable transport *)
  (dest -> Route.xmitf option) ->  (* xmit *)
  handle

val string_of_dest : dest -> string

(**************************************************************)
(* Domain management.
 *)

val of_mode : Alarm.t -> Addr.id -> t
val install : Addr.id -> (Alarm.t -> t) -> unit
(**************************************************************)
