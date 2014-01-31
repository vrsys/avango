(**************************************************************)
(* ARGE.MLI *)
(* Author: Mark Hayden, 4/96 *)
(**************************************************************)
open Trans
(**************************************************************)

type 'a t

(* Get and set parameters.
 *)
val get : 'a t -> 'a
val set : 'a t -> 'a -> unit
val set_string : 'a t -> string -> unit

(* Check an option parameter to make sure it has been set.
 *)
val check : debug -> 'a option t -> 'a

(**************************************************************)
val bool : (name -> bool -> 'a) -> 'a -> name -> string -> 'a t
val int : (name -> int -> 'a) -> 'a -> name -> string -> 'a t
val string : (name -> string -> 'a) -> 'a -> name -> string -> 'a t
(**************************************************************)
(**************************************************************)
(* These are the parameter variables set by this module.
 *)

val aggregate    : bool t		(* aggregate messages *)
val alarm	 : string t		(* alarm to use *)
val force_modes	 : bool t		(* force modes, no matter what *)
val gossip_hosts : string list option t	(* where to find gossip servers *)
val gossip_port  : port option t	(* where to find gossip server *)
val port         : port option t	(* ensemble default port *)
val group_name	 : string t		(* default group name *)
val groupd       : bool t		(* use groupd server? *)
val groupd_balance : bool t		(* load balance groupd servers? *)
val groupd_hosts : string list option t (* where to find groupd servers *)
val groupd_port  : port option t	(* TCP port to use for groupd *)
val groupd_repeat : bool t      	(* try multiple times to reach groupd *)
val protos       : bool t	        (* use protos server *)
val protos_port  : port option t	(* TCP port to use for protos *)
val protos_test  : bool t               (* used for testing protos *)
val id           : string t		(* user id for application *)
val key		 : string option t	(* security key to use *)
val log          : bool t		(* use log server? *)
val log_port     : port option t	(* TCP port to use for log *)
val modes	 : Addr.id list t	(* default modes to use *)
val deering_port : port option t	(* deering port *)
val properties   : Property.id list t	(* default protocol properties *)
val refcount     : bool t		(* use reference counts for iovecs *)
val roots	 : bool t		(* output resource info? *)
val glue         : Glue.glue t		(* selected layer glue. *)
val pgp          : string option t	(* are we using pgp? *)
val pgp_pass     : string option t	(* pass phrase for pgp *)
val pollcount    : int t		(* number of failed polls before blocking *)
val multiread    : bool t		(* do we read all data from sockets? *)
val ranking      : (Addr.id -> int) t	(* "ranking" of communication modes *)
val sched_step   : int t		(* number of events to schedule per step *)
val udp_host     : string option t	(* hostname override to use for UDP communication *)
val udp_port     : port option t	(* port override for UDP communication *)
val netsim_socks : bool t		(* allow socks with Netsim *)
val debug_real   : bool t		(* use real time for debugging logs *)
val short_names  : bool t		(* use short names for endpoints *)
val sock_buf     : int t                (* size of kernel socket buffers *)

(**************************************************************)
val gc_compact  : int -> unit           (* Set GC compaction rate *)
val gc_verb     : unit -> unit          (* Set GC verbosity *)
(**************************************************************)

val inet_of_string : 'a t -> string -> inet
val inet_of_string_list : 'a t -> string list -> inet list

(**************************************************************)

(* Returns sorted list of default Ensemble command-line arguments.
 *)
val args 	: unit -> (string * Arg.spec * string) list

(* Takes application arguments and parses them along with
 * the Ensemble arguments.
 *)
val parse	: (string * Arg.spec * string) list -> (string -> unit) -> string -> unit

(* Can be used for capturing command-line problems.
 *)
val badarg 	: string -> string -> unit

(**************************************************************)

val timestamp_check : string -> bool

(**************************************************************)
val arg_filter : (string array -> string array) -> unit
(**************************************************************)

val gossip_changed : bool ref           (* Did the gossip hosts list change *)
val get_new_gossip : unit -> Hsys.inet array (* Return the new gossip hosts *)
val set_new_gossip : Hsys.inet array -> unit (* set the new gossip hosts *)
val set_servers : Hsys.inet list -> unit (* set servers in the cluster *)
val get_servers : unit -> Hsys.inet list (* get servers in the cluster *)
