(**************************************************************)
(* APPL_LWE.MLI: lwe application interface *)
(* Author: Mark Hayden, 11/97 *)
(**************************************************************)

(* Wrap a list of application interfaces as one.  The view
 * state is used for the heavy weight group and should
 * contain an endpt not present in the interface list.  The
 * interface list contains interfaces and endpoints for each
 * interface.  The return value is a new view state and the
 * interface for the heavy weight group.  It should be
 * configured with Appl.config.  The view state should
 * specify a protocol stack that provides local delivery.
 *)

type header

val f : Alarm.t -> View.full -> 
  (Endpt.id * Appl_intf.New.t) list -> 
    (View.full * ((header * unit) * Iovecl.t)Appl_intf.New.full)

val pf : Alarm.t -> View.full -> 
  (Endpt.id * ('a * Iovecl.t) Appl_intf.New.full) list -> 
    (View.full * ((header *'a) * Iovecl.t) Appl_intf.New.full)
