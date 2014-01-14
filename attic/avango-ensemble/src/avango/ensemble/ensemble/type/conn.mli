(**************************************************************)
(* CONN.MLI : communication connection ids *)
(* Author: Mark Hayden, 12/95 *)
(**************************************************************)

(* Identifier used for communication routing.
 *)
type id

type kind = Cast | Send (*| Other*)

(* Set of connection identifiers used to communicate
 * with peers.
 *)
type t
type key
type recv_info = id * kind 

(* Constructor.
 *)
val create : 
  Trans.rank ->
  Version.id ->
  Group.id -> 
  View.id -> 
  Stack_id.t -> 
  Proto.id -> 
  View.t -> 
  t

val key	     	: t -> key
val pt2pt_send 	: t -> Trans.rank -> id
val multi_send 	: t -> id
val gossip 	: t -> id
val all_recv    : t -> recv_info list

(*
val string_of_key : key -> string
val string_of_t : t -> string
*)

(* Display functions.
 *)
val string_of_id : id -> string

val string_of_kind : kind -> string

(**************************************************************)
(* Create a 16-byte MD5 hash of an id.
 *)
val hash_of_id : id -> Buf.t

val hash_of_key : Version.id -> Group.id -> Proto.id -> Buf.t

(* A hack.  Squashes sender field in id to an illegal value.
 * and return the original field.  Used in Router.scale.
 *)
val squash_sender : id -> (Trans.rank option * id)

(**************************************************************)
val ltime : id -> Trans.ltime
(**************************************************************)
