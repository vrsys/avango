(**************************************************************)
(* APPL_MULTI.MLI: Multiplexion of interfaces *)
(* Author: Ohad Rodeh, 12/97 *)
(**************************************************************)

(* Wrap a list of application interfaces as one.  The view
 * state is used for the heavy weight group and should
 * contain an endpt not present in the interface list.  The
 * interface list contains interfaces and endpoints for each
 * interface.  The return value is a new view state and the
 * interface for the heavy weight group.  It should be
 * configured with Appl.power and Appl.config. The view state should
 * specify a protocol stack that provides local delivery.
 *)

type header

val f : Appl_intf.New.t array -> header Appl_intf.New.power

