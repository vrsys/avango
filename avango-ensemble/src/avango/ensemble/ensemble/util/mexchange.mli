(**************************************************************)
(* MEXCHANGE.MLI : Utilities for the Exchange protocol *)
(* Author: Ohad Rodeh, 6/2000 *)
(*************************************************************************)

type src = (*Endpt.id*) string
type dst = (*Endpt.id*) string
type bignum = (*Shared.DH.bigum*) string 
type key = string
type nonce = int * string

(* Used to record the message we are currently working on.
*)
type inner_msg = 
  | Piece of src * Addr.set * nonce * dst * Addr.set * nonce * bignum
  | Reply of src * Addr.set * nonce * dst * Addr.set * bignum * Buf.t

(* My id is used just to discard messages from myself. 
 *)
type msg =  
  | Id  of src * Addr.set * nonce
  | Compx of inner_msg * Auth.ticket


exception Bad_format of string
  
type t 

(* [init max_diff view_id_str]
 * Initialize a t structure with the maximal allowed difference
 * in acceptable nonces, and the current view_id in string format. 
*)    
val init : int -> string -> t
    
val create_nonce : t -> int -> nonce
val check_nonce  : t -> int -> nonce -> bool 
val diff_nonce   : t -> int -> nonce -> int
    
val string_of_msg : msg -> string
val msg_of_string : string -> msg

