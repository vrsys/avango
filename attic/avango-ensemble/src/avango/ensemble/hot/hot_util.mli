(**************************************************************)
(* HOT_UTIL.MLI *)
(* Authors: Alexey Vaysburd, Mark Hayden 11/96 *)
(**************************************************************)
open Ensemble
open Trans
open Buf
(**************************************************************)

type endpt = string
type msg = string
type millisec = int

val milli_of_time : Time.t -> millisec

(**************************************************************)
(* Types shared with the C application.
 *
 * NOTE: Changes to type definitions below require
 * corresponding modifications to be done in the C stubs.
 *)


(* View state structure passed to the application. 
 *)
type c_view_state = {
  c_version	: string ;
  c_group	: string ;
  c_view	: endpt array ; 
  c_rank	: int ;
  c_protocol    : string ;
  c_groupd	: bool ;
  c_view_id	: Trans.ltime * endpt ;
  c_params	: string ;
  c_xfer_view	: bool ; 
  c_primary	: bool ;
  c_clients     : bool array;
  c_key         : string 
}


(* These options are passed with application's join()
 * downcalls.  
 *)
type join_options = {
  jops_hrtbt_rate	: millisec ;
  jops_transports	: string ;
  jops_protocol	        : string ;
  jops_group_name	: string ;
  jops_properties	: string ;
  jops_use_properties	: bool ;
  jops_groupd	        : bool ;
  jops_params	        : string ;
  jops_client           : bool ;
  jops_debug            : bool ;
  jops_endpt            : string ;
  jops_princ            : string ;
  jops_key              : string ;
  jops_secure           : bool
}
(*
val marsh_upcall : Mbuf.t -> View.state option -> Protos.id -> Protos.upcall -> Iovecl.t
*)
(**************************************************************)

val init_view_state : join_options -> View.full

(**************************************************************)

val c_view_state : View.full -> c_view_state

(**************************************************************)
