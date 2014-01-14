(**************************************************************)
(* TIME.MLI *)
(* Author: Mark Hayden, 8/96 *)
(**************************************************************)

type t

val invalid 	: t
val zero 	: t
val neg_one     : t

val of_int	: int -> t		(* secs *)
val of_ints     : int -> int -> t	(* secs -> usecs *)
val of_string   : string -> t
val of_float 	: float -> t

val to_float 	: t -> float
val to_string 	: t -> string
val to_string_full : t -> string
val to_timeval  : t -> Hsys.timeval
val of_timeval  : Hsys.timeval -> t

val add 	: t -> t -> t
val sub 	: t -> t -> t

val gettimeofday : unit -> t
val select      : Hsys.select_info -> t -> int

val is_zero     : t -> bool
val is_invalid  : t -> bool
val ge          : t -> t -> bool
val gt          : t -> t -> bool
val cmp         : t -> t -> int

module Ord : (Trans.OrderedType with type t = t)

(**************************************************************)
(* Another version of the above stuff with in-place 
 * modification.  Use at your own risk.
 *)

(* I wanted to export this stuff in a separate module
 * but the compiler wasn't inlining the calls.
 *)
type m
val mut         : unit -> m
val mut_set     : m -> t -> unit
val mut_copy    : m -> t
val mut_ge      : m -> t -> bool
val mut_add     : m -> t -> unit
val mut_sub     : m -> t -> unit
val mut_sub_rev : t -> m -> unit
val mut_gettimeofday : m -> unit
val mut_select  : Hsys.select_info -> m -> int

(*
type m' = m
module Mut_hide : sig
  type m = m'
  val zero    : unit -> m
  val of_int  : m -> int -> unit
  val copy    : m -> t
  val ge      : m -> t -> bool
  val add     : m -> t -> unit
  val sub     : m -> t -> unit
  val sub_rev : t -> m -> unit
  val gettimeofday : m -> unit
  val select  : Hsys.select_info -> m -> int
end
*)
(**************************************************************)
