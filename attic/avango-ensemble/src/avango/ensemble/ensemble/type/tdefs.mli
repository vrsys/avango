(**************************************************************)
(* TDEFS.MLI: definitions for the tree modules *)
(* Author: Ohad Rodeh 2/99  *)
(**************************************************************)

module Key  : Tree.OrderedKey
module Member : Tree.OrderedMember 
module Tree : Tree.S with type member = Member.t and type key = Key.t

module type RealKeyType = sig
  type t 
  type set 

  val empty : set 
  val create : Key.t -> Security.cipher option -> t
  val tag : t -> Key.t
  val real : t -> Security.cipher option
  val string_of_t : t -> string
  val add : set -> Key.t -> Security.cipher option -> unit
  val decrypt : Shared.Cipher.t -> set -> ((Key.t * Buf.t) list * Key.t) list -> unit
  val assoc : Key.t -> set -> Security.cipher option
  val translate : set -> (Key.t * Key.t) list -> set
  val purge  : Tree.z -> set -> int -> set 
  val inflate : Tree.z -> set -> int -> set
  val check_all : set -> bool
  val pairs_of : set -> (Key.t * Security.cipher option) list
  val from_pair : Key.t * Security.cipher option -> t
  val from_list : t list -> set
  val hd        : set -> Key.t * Security.cipher option
  val tl        : set -> set
end

module RealKey : RealKeyType

