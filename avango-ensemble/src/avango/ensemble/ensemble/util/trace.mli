(**************************************************************)
(* TRACE.MLI *)
(* Author: Mark Hayden, 3/96 *)
(**************************************************************)

val file : string -> string		(* for modules *)
val filel : string -> string		(* for layers *)
val file_report : bool -> unit

(**************************************************************)

val log_check : string -> bool
val log : string -> (unit -> string) -> unit
val log2 : string -> string -> (unit -> string) -> unit
val log3 : string -> string -> string -> (unit -> string) -> unit
val logl : string -> (unit -> string list) -> unit
val logl2 : string -> string -> (unit -> string list) -> unit
val logl3 : string -> string -> string -> (unit -> string list) -> unit

val logs_all : unit -> string list
val logs_all_status : unit -> (string * bool) list

val log_add : string -> (string -> string -> unit) -> unit
val log_remove : string -> unit

(**************************************************************)

val make_failwith : string -> string -> 'a
val make_failwith2 : string -> string -> 'a

(**************************************************************)

val dump : (unit -> unit) -> unit
val debug : 'b -> ('a -> 'b) -> ('a -> 'b)
val info : string -> string -> string

(**************************************************************)

(* Modules call this in order to register a garbage
 * collection "root".  Calls to print root then call all
 * root callbacks and print out the strings.  This can be
 * helpful in tracking down memory leaks.  
 *)
val install_root : (unit -> string list) -> unit

val print_roots : unit -> unit

(**************************************************************)

(* Modules can call this in order to register information
 * that might be useful to a user.  Usually, this
 * corresponds to strangenesses in the configuration.  
 *)
val comment : string -> unit

(**************************************************************)

(* Modules with an embedded test function in them can
 * register it with test_declare.  Other modules can run a
 * test with test_exec.  
 *)
val test_declare : string -> (unit -> unit) -> unit
val test_exec : string -> unit

(**************************************************************)

(* Prints out information about the configuration.
 *)
val config_print : unit -> unit

(**************************************************************)
