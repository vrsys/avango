(**************************************************************)
(* ENDPT.MLI *)
(* Author: Mark Hayden, 7/95 *)
(**************************************************************)
open Trans
(**************************************************************)

(* Type of endpoints.
 *)
type id

type full = id * Addr.set

(* Constructors.  The named endpoints are only used for
 * debugging purposes.  Named endpoints (as opposed to named
 * groups which just have a string) still contain a unique
 * identifier along with the name.  
 *)
val id		        : Unique.t -> id (* anonymous endpoints *)
val named               : Unique.t -> string -> id (* named endpoints *)

(* Yet another way to create an endpoint.  In this case, the
 * string given must provide the uniqueness of the endpoint.
 * In other words, the string passed here must be
 * system-wide unique.  Also, the string_of_id function
 * returns the string value used here.  This function is used
 * in implementing external interfaces where it may be useful
 * to be able to generate the endpoint information elsewhere
 * instead of requesting it from this module.  Because of this,
 * the string input here should usually not contain spaces or
 * non-printable characters.
 *)
val extern              : string -> id

(* Display functions.
 *)
val string_of_id	: id -> string
val string_of_id_short	: id -> string
val string_of_id_very_short : id -> string
val string_of_full      : full -> string

val string_of_id_list	: id list -> string

(**************************************************************)
