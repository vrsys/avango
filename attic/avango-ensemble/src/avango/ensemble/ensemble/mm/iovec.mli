(**************************************************************)
(* IOVEC.MLI *)
(* Author: Mark Hayden, 3/95 *)
(* Rewritten by Ohad Rodeh 9/2001 *)
(* This file wraps the Socket.Iovec raw interface in a more 
 * type-safe manner, using offsets and length types. 
 *)
(**************************************************************)
open Trans
open Buf
(**************************************************************)
(* The type of a C memory-buffer. It is opaque.
 *)
type t 
    
(* A raw iovec, no ref-counting.
*)
type raw
val of_raw : raw -> t
val raw_of : t -> raw

(* allocation, and copy. 
 *)
val buf_of : t -> Buf.t
val buf_of_full : Buf.t -> len -> t -> unit
val of_buf : Buf.t -> ofs -> len -> t

val empty : t

(* Length of an iovec
*)  
val len : t -> len

(* May throw an Out_of_iovec_memory exception, if the
 * user has no more free memory to hand out.
 *)
val alloc : len -> t
  
  
val sub : t -> ofs -> len -> t
  
(* Decrement the refount, and free the cbuf if the count
 * reaches zero. 
 *)
val free : t -> unit 
  
(* Increment the refount, do not really copy. 
 *)
val copy : t -> t 

(* Actually copy into a fresh iovec.
 *)
val really_copy : t -> t
  
(* Flatten an iovec array into a single iovec.  Copying only
 * occurs if the array has more than 1 non-empty iovec.
 *
 * Throws an Out_of_iovec_memory exception, if 
 * user memory has run out.
 *)
val flatten : t array -> t

(* Copy even in the case of a singleton.
 *)
val flatten_w_copy : t array -> t
  
(**************************************************************)  
(* For debugging. Return the number of references to the iovec.
*)
val num_refs : t -> int

(* Return the number of iovecs currrently held, and the total length.
 *)
val debug : unit -> int * int

(* Marshal directly into an iovec.
 *
 * Unmarshal directly from an iovec. Raises an exception if there
 * is a problem, and checks for out-of-bounds problems. 
 *)
val marshal : 'a -> Marshal.extern_flags list -> t
val unmarshal : t-> 'a

(**************************************************************)
(* This is a hack to remove the need to copy iovec arrays, for the
 * sole need of changing their type, to Socket.Iovec.t.
 *)
module Priv : sig
  val md5_update_iov : Socket.md5_ctx -> t -> unit

  val sendtov : Socket.sendto_info -> t array -> unit
  val sendtosv : Socket.sendto_info -> string -> int -> int -> t array -> unit
  val udp_recv_packet : Socket.socket -> Buf.t * t

  val recv_iov : Socket.socket -> t -> int -> int -> int
  val tcp_recv_packet : Socket.socket -> string -> int -> int -> t -> int
    
  val sendv_p : Socket.socket -> t array -> int
  val sendsv_p : Socket.socket -> string -> int -> int -> t array -> int
  val sends2v_p : Socket.socket -> string -> string -> int -> int -> t array -> int
end
  
(**************************************************************)  
  
  
