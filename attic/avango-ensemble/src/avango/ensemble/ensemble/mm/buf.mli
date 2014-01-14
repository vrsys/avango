(**************************************************************)
(* BUF.ML *)
(* Author: Mark Hayden, 8/98 *)
(* Rewritten by Ohad Rodeh 9/2001 *)
(* Buffers are simply immutable strings, where several 
   operations are available on them *)
(**************************************************************)
open Trans
(**************************************************************)

(* Type of lengths and offsets.  
 *)
type len (*= int*) (* should be opaque *)
type ofs = len

external int_of_len : len -> int = "%identity"
external len_of_int : int -> len = "%identity"
external (=||) : len -> len -> bool = "%eq"
external (<>||) : len -> len -> bool = "%noteq"
external (>=||) : len -> len -> bool = "%geint"
external (<=||) : len -> len -> bool = "%leint"
external (>||) : len -> len -> bool = "%gtint"
external (<||) : len -> len -> bool = "%ltint"
external (+||) : len -> len -> len = "%addint"
external (-||) : len -> len -> len = "%subint"
external ( *||) : len -> int -> len = "%mulint"

val len0 : len
val len4 : len
val len8 : len
val len12 : len
val len16 : len

val md5len : len
val md5len_plus_4 : len
val md5len_plus_8 : len
val md5len_plus_12 : len
val md5len_plus_16 : len
(**************************************************************)

(* An immutable string
*)
type t
type digest = t (* A 16-byte immutable string *)

val of_string : string -> t
val string_of : t -> string
val blit : t -> ofs -> t -> ofs -> len -> unit
val blit_str : string -> int -> t -> ofs -> len -> unit
val check : t -> ofs -> len -> unit
val concat : t list -> t
val fragment : len -> t -> t array (* fragment a buffer into parts smaller
				      than len *)
val copy : t -> t
val append : t -> t -> t
val create : len -> t
val digest_sub : t -> ofs -> len -> digest
val digest_substring : string -> int(*ofs*) -> int(*len*) -> digest
val empty : t
val int16_of_substring : t -> ofs -> int
val length : t -> len
val sub : t -> ofs -> len -> t
val subeq8 : t -> ofs -> t -> bool
val subeq16 : t -> ofs -> t -> bool
val to_hex : t -> string

(* Write integers in standard network format. 
 * This is compatible with the C htonl functions. 
*)
val write_int32 : t -> ofs -> int -> unit
val read_int32  : t -> ofs -> int
(**************************************************************)

(* Make matched marshallers to/from buffers.
 *)
val make_marsh : debug -> 
  (('a -> t) * (t -> ofs -> 'a))

(**************************************************************)
(* The maximum length of messages on the network. 
 *)
val max_msg_len : len
(**************************************************************)
(* We use a preallocated buffer pool, into which objects are
 * marshaled. 
 * 
 * The basic abstraction is a substring (buf,ofs,len). However,
 * currently CAML does not optimize returning tuples from
 * functions, therefore, we pass a continuation function. 
 *
 * The usage is by the routers. An xmit function is passed
 * that sends the substring to the network. 
*)

  
(* Allocate a substring, and send it. 
*)
val prealloc : len -> (t -> ofs -> len -> unit) -> unit

(* Mashal an ML object into a substring and send it. 
*)  
val prealloc_marsh : len -> 'a -> (t -> ofs -> len -> unit) -> unit
  
(**************************************************************)
