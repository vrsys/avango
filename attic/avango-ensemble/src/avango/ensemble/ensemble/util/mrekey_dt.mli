(**************************************************************)
(* MREKEY_DT.MLI : Dynamic Tree rekey support module *)
(* Author: Ohad Rodeh, 4/2000 *)
(*************************************************************************)
open Trans

type t 

(* An empty tree. Used so that we would not have to reveal the 
 * inner structure of type t.
*)
val empty : t

val string_of_t : t -> string
val singleton :   rank -> t
val children : t -> rank -> rank list
val father   : t -> rank -> rank option
val members_of_t : t -> rank list
val map   : (rank -> rank) -> t -> t

val split : t -> rank -> rank list -> t
val merge : t list -> t

val pretty_string_of_t : t -> string


(*************************************************************************)
