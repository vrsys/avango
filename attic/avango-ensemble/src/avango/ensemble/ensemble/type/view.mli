(**************************************************************)
(* VIEW.MLI *)
(* Author: Mark Hayden, 3/96 *)
(**************************************************************)
open Trans
(**************************************************************)

(* VIEW.T: an array of endpt id's.  These arrays should
 * be treated as though they were immutable.
 *)
type t = Endpt.id Arrayf.t

val to_string : t -> string

(**************************************************************)

(* VIEW.ID: a logical time and an endpoint id.  View
 * identifiers have a lexicographical total ordering.  
 *)
type id = ltime * Endpt.id

(* Display function for view id's.
 *)
val string_of_id : id -> string

(**************************************************************)

(* VIEW.STATE: a record of information kept about views.
 * This value should be common to all members in a view.
 *)
type state = {
  (* Group information.
   *)
  version       : Version.id ;		(* version of Ensemble *)
  group		: Group.id ;		(* name of group *)
  proto_id	: Proto.id ;		(* id of protocol in use *)
  coord         : rank ;		(* initial coordinator *)
  ltime         : ltime ;		(* logical time of this view *)
  primary       : primary ;		(* primary partition? (only w/some protocols) *)
  groupd        : bool ;		(* using groupd server? *)
  xfer_view	: bool ;		(* is this an XFER view? *)
  key		: Security.key ;	(* keys in use *)
  prev_ids      : id list ;             (* identifiers for prev. views *)
  params        : Param.tl ;		(* parameters of protocols *)
  uptime        : Time.t ;		(* time this group started *)

  (* Per-member arrays.
   *)
  view 		: t ;			(* members in the view *)
  clients	: bool Arrayf.t ;	(* who are the clients in the group? *)
  address       : Addr.set Arrayf.t ;	(* addresses of members *)
  out_of_date   : ltime Arrayf.t	; (* who is out of date *)
  lwe           : Endpt.id Arrayf.t Arrayf.t ; (* for light-weight endpoints *)
  protos        : bool Arrayf.t  	(* who is using protos server? *)
}

(* FIELDS: these correspond to the fields of a View.state.
 *)
type fields =
  | Vs_coord        of rank
  | Vs_group	    of Group.id
  | Vs_view 	    of t
  | Vs_ltime        of ltime
  | Vs_params	    of Param.tl
  | Vs_prev_ids     of id list
  | Vs_proto_id	    of Proto.id
  | Vs_xfer_view    of bool
  | Vs_key	    of Security.key
  | Vs_clients	    of bool Arrayf.t
  | Vs_groupd       of bool
  | Vs_protos       of bool Arrayf.t
  | Vs_primary      of primary
  | Vs_address      of Addr.set Arrayf.t
  | Vs_out_of_date  of ltime Arrayf.t
  | Vs_lwe          of Endpt.id Arrayf.t Arrayf.t
  | Vs_uptime       of Time.t


(* SET: Construct new view state based on previous view state.
 *)
val set : state -> fields list -> state

val string_of_state : state -> string

val id_of_state : state -> id

(**************************************************************)

(* VIEW.LOCAL: information about a view that is particular to 
 * a member.
 *)
type local = {
  endpt	        : Endpt.id ;		(* endpoint id *)
  addr	        : Addr.set ;		(* my address *)
  rank 	        : rank ;		(* rank in the view *)  
  name		: string ;		(* my string name *)
  nmembers 	: nmembers ;		(* # members in view *)
  view_id 	: id ;			(* unique id of this view *)
  am_coord      : bool ;  		(* rank = vs.coord? *)
  falses        : bool Arrayf.t ;       (* all false: used to save space *)
  zeroes        : int Arrayf.t ;        (* all zero: used to save space *)
  loop          : rank Arrayf.t ;      	(* ranks in a loop, skipping me *)
  async         : (Group.id * Endpt.id) (* info for finding async *)
}  

(* LOCAL: create local record based on view state and endpt.
 *)
val local : debug -> Endpt.id -> state -> local

val redirect_async : debug -> Group.id * Endpt.id -> local -> local

(**************************************************************)

(* VIEW.FULL: a full view state contains both local and common
 * information.
 *)
type full = local * state

(* SINGLETON: Create full view state for singleton view.
 *)
val singleton :
  Security.key ->			(* security key *)
  Proto.id ->				(* protocol *)
  Group.id ->				(* group *)
  Endpt.id ->				(* endpt *)
  Addr.set ->				(* addresses *)
  Time.t ->				(* current time *)
  full

(* STRING_OF_FULL: Display function for view state records.
 *)
val string_of_full : full -> string

(* CHECK: check some conditions on the sanity of the 
 * view state.
 *)
val check : debug -> full -> unit

(**************************************************************)

val endpt_full : full -> Endpt.full 

val coord_full : state -> Endpt.full

(**************************************************************)

(* MERGE: merge a list of view states together.  The fields
 * view, client, address, and out_of_date are taken from the
 * constituent views.  The prev_ids are taken from each of
 * view states (in order).  The ltime is the successor of the
 * maximum of the ltimes.  The other fields are from the view
 * state at the head of the list. 
 *)
val merge : state list -> state

(* FAIL: takes a view state and list of failed ranks and
 * strips the failed members from the 4 array fields.
 *)
val fail : state -> bool Arrayf.t -> state

(**************************************************************)
