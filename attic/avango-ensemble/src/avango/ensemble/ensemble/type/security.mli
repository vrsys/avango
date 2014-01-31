(**************************************************************)
(* SECURITY *)
(* Authors: Mark Hayden, 6/95 *)
(* Refinements: Ohad Rodeh 10/98 7/2000 *)
(**************************************************************)
open Trans
open Buf
(**************************************************************)

(* Type of keys in Ensemble.
 * A key has two parts: 
 * 1. mac-key, used for keyed-hashing. 
 * 2. encyrption key, used for encryption functions.
 * Each of the subkeys are 16 bytes long. 
 *)
type mac 
type cipher 

type inner_key = {
  mac     : mac ;
  cipher  : cipher 
}
    
type key = 
  | NoKey
  | Common of inner_key

val mac_len  : Buf.len
val cipher_len : Buf.len
val key_len : Buf.len  (* The standard length of keys. They are currently 32 bytes long *)


val buf_of_cipher : cipher -> Buf.t
val cipher_of_buf : Buf.t -> cipher 
val buf_of_mac :  mac -> Buf.t
val mac_of_buf :  Buf.t -> mac

(* Accessor functions
*)
val get_mac : key -> mac
val get_cipher : key -> cipher

(* Get the Buf.t contents of a key.
 *)
val buf_of_key : key -> Buf.t

(* Get a textual representation of the key for printing out
 * to the user.  
 *)
val string_of_key_short : key -> string
val string_of_key : key -> string

(* Convert a 32byte buf into a key. 
*)
val key_of_buf : Buf.t -> key
