(**************************************************************)
(* MANAGE.MLI *)
(* Author: Mark Hayden, 11/96 *)
(* Designed with Roy Friedman *)
(**************************************************************)
open Trans
open Mutil
(**************************************************************)

(*
 * MANAGE.T: Type of a manager instance.
 *)
type ('member,'group) t

(*
 * CREATE: create a new manager instance within this process.
 * The view state gives information about this member,
 * including the name of the manager group.
 *)
val create : Alarm.t -> View.full -> (('m,'g) t * View.full * Appl_intf.New.t)

(*
 * PROXY: create a proxy for a manager in another process.
 *)
val proxy : Alarm.t -> Hsys.socket -> ('m,'g) t

(* 
 * JOIN: join a group through a manager.
 *)
val join :
  ('m,'g) t -> group -> endpt -> ltime ->
  (coord_msg -> unit) -> (member_msg -> unit)

(*
 * CONFIG: Run a protocol stack through a manager.
 *)
val config : 
  ('m,'g) t ->				(* manager instance *)
  View.full ->				(* view state *)
  (View.full -> (Event.up -> unit) -> (Event.dn -> unit)) -> (* configured stack *)
  unit

(**************************************************************)
(**************************************************************)

(* PROXY_SERVER: creates a TCP server on the given local TCP
 * port.  The instance must be a manager instance created
 * with the manager function.  
 *)
val proxy_server : Alarm.t -> ('m,'g) t -> port -> unit

val create_proxy_server : Alarm.t -> port -> View.full -> View.full * Appl_intf.New.t

(**************************************************************)
(**************************************************************)
(* NOTE: The functions below only work on managers created
 * within this process (not those created by the "proxy"
 * function *)

(* SET_PROPERTIES: Set the properties of a manager. This
 * installs a group announcement handler and specifies the
 * properties of the manager.  The properties of the manager
 * are used for remote filtering of announcements (not
 * currently implemented).  Announced groups cause the
 * handler to be executed at members who are not filtered
 * out.
 *)

val set_properties : 
  ('m,'g) t -> 
  'm ->					(* member's state *)
  (Group.id -> 'g -> unit) ->		(* group announcement handler *)
  unit

(* ANNOUNCE: Announce the creation of a new group.  Members
 * who have not heard of the new group before will have it
 * announced on their handler (if they have installed one).
 * The filter function is used to filter out members who
 * will not be in the group.  The group identifier and the
 * properties are passed to each of the members who are not
 * filtered out so that they can decide whether to join the
 * group.  Groups are persistent and will be announced to
 * new members as they join.  
 *)
val announce : 
  ('m,'g) t ->
  Group.id ->				(* group identifier *)
  ('m -> bool) ->			(* member filter NOT IMPLEMENTED *)
  'g ->					(* group properties *)
  unit

(* DESTROY: Destroy a group.  This causes all members in the
 * group to immediately leave the group.  That group will no
 * longer be announced to new members.  But if it is
 * announced again (or if we merge with another partition
 * that has announced the group), then it will continue to
 * be announced until another destroy operation is made.
 *)
val destroy :				(* NOT IMPLEMENTED *)
  ('m,'g) t ->
  Group.id ->				(* group identifier *)
  unit

(**************************************************************)
(* These are tailored for the groupd.
 *)

type t2 = 
    View.full -> 
    (View.full -> (Event.up -> unit) -> (Event.dn -> unit)) -> 
      unit

val groupd_create : 
  Alarm.t -> View.full -> (t2 * View.full * Appl_intf.New.t)


val groupd_proxy : 
  Alarm.t -> Hsys.socket -> t2

(**************************************************************)


