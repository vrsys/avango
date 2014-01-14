(**************************************************************)
(* UTIL.MLI *)
(* Author: Mark Hayden, 4/95 *)
(**************************************************************)
open Trans
(**************************************************************)
type ('a,'b,'c) fun2arg = 'a -> 'b -> 'c
type ('a,'b,'c,'d) fun3arg = 'a -> 'b -> 'c -> 'd

(*
external arity2 : ('a,'b,'c) fun2arg -> ('a,'b,'c) fun2arg = "%arity2"
external arity3 : ('a,'b,'c,'d) fun3arg -> ('a,'b,'c,'d) fun3arg = "%arity3"
*)
val arity2 : ('a,'b,'c) fun2arg -> ('a,'b,'c) fun2arg
val arity3 : ('a,'b,'c,'d) fun3arg -> ('a,'b,'c,'d) fun3arg
(**************************************************************)

external (=|) : int -> int -> bool = "%eq"
external (<>|) : int -> int -> bool = "%noteq"
external (>=|) : int -> int -> bool = "%geint"
external (<=|) : int -> int -> bool = "%leint"
external (>|) : int -> int -> bool = "%gtint"
external (<|) : int -> int -> bool = "%ltint"
val int_max : int -> int -> int
val int_min : int -> int -> int

(**************************************************************)
(* Really basic things.
 *)

(* The first application creates a counter.  Each time the
 * second unit is applied the next integer (starting from 0)
 * is returned.
 *)
val counter 		: unit -> unit -> int

(* The identity function.
 *)
val ident               : 'a -> 'a

(* This is used to discard non-unit return values from
 * functions so that the compiler does not generate a
 * warning.  
 *)
(*
val unit : 'a -> unit
val unit2 : 'a -> 'b -> unit
*)
val ignore2 : 'a -> 'b -> unit

val info : string -> string -> string

(* The string sanity.
 *)
val sanity : string
val sanityn : int -> string

(**************************************************************)
(* Debugging stuff.
 *)

val verbose		: bool ref
val quiet		: bool ref
val addinfo             : string -> string -> string
val failmsg             : string -> string -> string

(**************************************************************)
(* Export printf and sprintf.
 *)

val printf		: ('a, unit, unit) format -> 'a
val eprintf		: ('a, unit, unit) format -> 'a
val sprintf		: ('a, unit, string) format -> 'a

(**************************************************************)
val fprintf_override    : (out_channel -> string -> unit) -> unit
(**************************************************************)
(* Some list/array operations.
 *)

val sequence		: int -> int array
val index               : 'a -> 'a list -> int
val except              : 'a -> 'a list -> 'a list
val array_is_empty	: 'a array -> bool
val array_filter	: ('a -> bool) -> 'a array -> 'a array
val array_index 	: 'a -> 'a array -> int
val array_mem 	        : 'a -> 'a array -> bool
val array_filter_nones  : 'a option array -> 'a array
val array_exists	: (int -> 'a -> bool) -> 'a array -> bool
val array_incr 		: int array -> int -> unit
val array_decr 		: int array -> int -> unit
val array_add           : int array -> int -> int -> unit
val array_sub           : int array -> int -> int -> unit
val array_for_all 	: ('a -> bool) -> 'a array -> bool
val array_flatten       : 'a array array -> 'a array
val matrix_incr 	: int array array -> int -> int -> unit
val do_once		: (unit -> 'a) -> (unit -> 'a)
val hashtbl_size	: ('a,'b) Hashtbl.t -> int
val hashtbl_to_list     : ('a,'b) Hashtbl.t -> ('a * 'b) list
val string_check        : debug -> string -> int(*ofs*) -> int(*len*) -> unit
val deepcopy            : 'a -> 'a

(**************************************************************)
(* Some string conversion functions.
 *)

val string_of_unit      : unit -> string
val string_map          : (char -> char) -> string -> string
val string_of_pair      : 
  ('a -> string) -> ('b -> string) -> ('a * 'b -> string)
val string_of_list 	: ('a -> string) -> 'a list -> string
val string_of_array 	: ('a -> string) -> 'a array -> string
val string_of_int_list 	: int list -> string
val string_of_int_array : int array -> string
val bool_of_string      : string -> bool
val string_of_bool	: bool -> string
val string_of_bool_list : bool list -> string
val string_of_bool_array : bool array -> string
val string_split	: string -> string -> string list
val hex_to_string	: string -> string
val hex_of_string	: string -> string
val string_uppercase    : string -> string
val strchr              : char -> string -> int

(**************************************************************)
(* Some additional option operations.
 *)

(* Calls the function if the option is a Some()
 *)
val if_some 		: 'a option -> ('a -> unit) -> unit

(* Call the function if the option is None.
 *)
val if_none 		: 'a option -> (unit -> 'a option) -> 'a option

(* Extract the contents of an option.  Fails on None.
 *)
val some_of 		: debug -> 'a option -> 'a

(* Returns true if the option is None.
 *)
val is_none 		: 'a option -> bool

(* String representation of an option.
 *)
val string_of_option 	: ('a -> string) -> 'a option -> string
val option_map		: ('a -> 'b) -> 'a option -> 'b option
val filter_nones        : 'a option list -> 'a list
val once                : debug -> 'a option list -> 'a

(**************************************************************)

val make_magic : unit -> (('a -> Obj.t ) * (Obj.t  -> 'a))

(**************************************************************)

val disable_sigpipe : unit -> unit

(**************************************************************)
(*
val average   : int -> unit
val gc_profile : string -> ('a -> 'b) -> 'a -> 'b
val gc_profile3 : string -> ('a -> 'b -> 'c -> 'd) -> 'a -> 'b -> 'c -> 'd
*)
(**************************************************************)

val strtok : string -> string -> string * string

(**************************************************************)

val string_of_id : debug -> (string * 'a) array -> 'a -> string
val id_of_string : debug -> (string * 'a) array -> string -> 'a

(**************************************************************)

val string_list_of_gc_stat : Gc.stat -> string list

(**************************************************************)
(* Get the tag value of an object's representation.
 *)

val tag : 'a -> int

(**************************************************************)

val sample : int -> 'a array -> 'a array

(**************************************************************)

(* Generate a string representation of an exception.
 *)
val error : exn -> string

(* Same as Printexc.catch.
 *)
val catch : ('a -> 'b) -> 'a -> 'b


(* Called to set a function for logging information about
 * this module.
 *)
val set_error_log : ((unit -> string) -> unit) -> unit

val install_error : (exn -> string) -> unit
(**************************************************************)

