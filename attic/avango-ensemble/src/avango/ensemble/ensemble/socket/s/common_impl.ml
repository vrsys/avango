(**************************************************************)
(* COMMON_IMPL.ML *)
(* Author: Ohad Rodeh 10/2002 *)
(* Common functions for win32 and Unix *)
(**************************************************************)
open Socksupp
(**************************************************************)

external heap : unit -> Obj.t array 
  = "skt_heap"
external addr_of_obj : Obj.t -> string 
  = "skt_addr_of_obj"
external minor_words : unit -> int 
  = "skt_minor_words" "noalloc"
external frames : unit -> int array array 
  = "skt_frame_descriptors"

(**************************************************************)

(* MD5 hash functions. We need to have it work on Iovecs as
 * well as regular strings. 
*)
type md5_ctx = string

external md5_ctx_length : unit -> int
  = "skt_md5_context_length" "noalloc"
external md5_init : md5_ctx -> unit
  = "skt_md5_init" "noalloc"
external md5_init_full : md5_ctx -> string -> unit
  = "skt_md5_init_full" "noalloc"
external md5_update : md5_ctx -> buf -> ofs -> len -> unit
  = "skt_md5_update" "noalloc"
external md5_update_iov : md5_ctx -> Natiov_impl.raw -> unit
  = "skt_md5_update_iov" "noalloc"
external md5_final : md5_ctx -> Digest.t -> unit
  = "skt_md5_final" "noalloc"

let md5_init () =
  let ret = String.create (md5_ctx_length ()) in
  md5_init ret ;
  ret

let md5_init_full init_key =
  if String.length init_key <> 16 then 
    raise (Invalid_argument "md5_init_full: initial key value is not of length 16");
  let ret = String.create (md5_ctx_length ()) in
  md5_init_full ret init_key;
  ret


let md5_final ctx =
  let dig = String.create 16 in
  md5_final ctx dig ;
  dig

let md5_update_iov ctx iov_t = 
  md5_update_iov ctx iov_t.Natiov_impl.iov

(**************************************************************)


