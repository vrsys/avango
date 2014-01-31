(**************************************************************)
(* COORD.ML: scripting side and UI of RES *)
(* Author: Ohad Rodeh , 9/99 *)
(**************************************************************)

(* Toggle the verbose flag
*)
val toggle_verbose : unit -> unit

(* [connect ml]  Try concecting to a list of machines [ml].
 * Connect to as many as possible. 
*)
val connect : string list -> unit

(* [set_live ml] Set the list of live machines to [ml]. 
 * Return the list of dead machines. 
*)
val set_live : string list -> string list

(* Get uptime from all the clients. 
*)
val rpc_uptime : unit -> (string * float) list

(* [rpc f time opts time_flag], send (Do_command (f i machine_name opts)) to 
 * all clients. Wait [time] for all replys.
*)
val rpc : (int -> string -> Aux.mngr_msg) -> int -> Aux.re_opts list -> unit

(* Printf the list of open connections.
*)
val show_conn_list : unit -> string list 

(* [run_remote prog machine] run a process [prog] on a remote
 * [machine]
*)
val rexec : string -> string -> int







