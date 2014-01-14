(**************************************************************)
(* COORD.MLI *)
(* Author: Mark Hayden, 8/97 *)
(* Designed with Roy Friedman *)
(**************************************************************)
open Trans
open Mutil
(**************************************************************)

(* COORD.T: The type for coordinators.
 *)
type t

(* CREATE: create a new coordiator instance.
 *)
val create : primary -> (coord_msg -> unit) -> t

(* RECEIVE: handle a message from a member.
 *)
val receive : t -> (endpt * member_msg) -> unit

(* RECONSTRUCT: reconstruct a coordinator after a view
 * change.  
 *)
val reconstruct : t -> primary -> 
  (endpt list) * (Member.state list) * (ltime list) * (endpt list list) -> 
  unit

(**************************************************************)
