(**************************************************************)
(* DIAMOND.ML : A diamond structure *)
(* Author: Ohad Rodeh, 4/2000 *)
(*************************************************************************)
open Trans

(* This is the regular type used through this module. 
 * There is an option to attach other "material" through the alpha
 * type.
*)
type t 

(* A full tree, contains full messaging information.
 * Used for debuggin purposes only. 
 *)
type ft  

val string_of_t : t -> string
val pretty_string_of_t : t -> string
val members_of_t : t -> rank list

(* Instructions for members. 
*)
type simple_cmd = 
  | Ack
  | Key 

val string_of_simple_cmd : simple_cmd -> string

(* The first array is for sources, the second is for
 * destinations. 
*)
type instr = (int * simple_cmd) list array * (int * simple_cmd) list array

val empty : t
val singleton : rank -> t
val create : rank option -> t -> t -> rank option -> bool -> t

val fully_connected : t -> bool 
val map     : (rank -> rank) -> t -> t 

val split   : t -> rank -> rank list -> t (* remove a list of members from t *)
val merge   : t list -> int -> instr * t * instr

(* Exported only for debugging purposes. 
*)
val two_connect : t -> t

type appl_state  = 
  | T of t
  | Tinstr of t * t * instr 

(*************************************************************************)
