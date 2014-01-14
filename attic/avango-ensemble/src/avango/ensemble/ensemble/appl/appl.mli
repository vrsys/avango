(**************************************************************)
(* APPL.MLI *)
(* Author: Mark Hayden, 4/95 *)
(**************************************************************)
open Trans
(**************************************************************)

(* This process' alarm.  This should normally not be called
 * directly.
 *)
val alarm : debug -> Alarm.t

(* Get a list of addresses for this process, based on a set
 * of "modes."
 *)
val addr :
  Addr.id list -> 
  Addr.set

(* Create default view state information for singleton groups.
 *)
val default_info :
  debug ->				(* application name *)
  View.full

(* Take View.full and switch on all the security switches. 
 *)
val set_secure : 
  View.full -> 
  Property.id list -> 
  View.full

(* Create view state information for singleton groups, but
 * you can give more information than default_info.
 *)
val full_info :
  debug ->				(* application name *)
  Endpt.id ->				(* endpoint *)
  bool ->				(* use groupd server? *)
  bool ->				(* use protos server? *)
  Proto.id ->				(* protocol stack to use *)
  (Addr.id list) ->			(* modes to use *)
  Security.key ->			(* key to use *)
  View.full


(* Main loop of Ensemble.  Never returns.
 *)
val main_loop : unit -> unit

(* Execute main function if application has one of the
 * specified names.  Works for the various operating
 * systems supported by Ensemble.
 *)
val exec :
  string list ->			(* application names *)
  (unit -> unit) ->			(* main function *)
  unit

(* Configure a stack.  Supports normal/groupd/protos stacks.
 *)
val config_new :
  Appl_intf.New.t ->			(* application *)
  View.full ->				(* view state *)
  unit

(* Configure a stack.  Supports normal/groupd/protos stacks.
 *)
val config_new_full :
  Layer.state ->			(* application state *)
  View.full ->				(* view state *)
  unit

(* Request an asynchronous event for an application.
 *)
val async :
  (Group.id * Endpt.id) ->		(* Id of application *)
  (unit -> unit)			(* request function *)

(**************************************************************)

val main_loop_opt : unit -> unit
val init_groupd : unit -> Manage.t2 (* for perf tests *)

(* This starts a "thread" that can be used to monitor
 * various resources in the system.  It should only be
 * called once.
 *)
val start_monitor : unit -> unit

(**************************************************************)

open Trans

type cast_info = {
  mutable acct_recd    : seqno ;	  (* # delivered messages *)
  mutable acct_lost    : seqno ;	  (* # lost messages *)
  mutable acct_retrans : seqno ;          (* # retrans messages *)
  mutable acct_bad_ret : seqno ;	  (* # retrans messages not used *)
  mutable acct_sent    : seqno	          (* # retrans messages sent by *)
}

val cast_info : cast_info

(**************************************************************)
