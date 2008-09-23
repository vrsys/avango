(**************************************************************)
(* IOVEC.MLI *)
(* Author: Mark Hayden, 3/95 *)
(* Rewritten by Ohad Rodeh 9/2001 *)
(* This file wraps the Socket.Iovec raw interface in a more 
 * type-safe manner, using offsets and length types. 
 *)
(**************************************************************)
let name = "IOVEC"
let failwith s = failwith (name^":"^s)
let log = Trace.log name
(**************************************************************)
open Trans
open Buf
open Printf
(**************************************************************)

type t = Socket.Basic_iov.t
    
(* A raw iovec, no ref-counting.
 *)
type raw = Socket.Basic_iov.raw
let of_raw = Socket.Basic_iov.t_of_raw
let raw_of = Socket.Basic_iov.raw_of_t
  
let num_refs t = Socket.Basic_iov.num_refs t
  
(* allocation, and copy. 
 *)
let buf_of t = 
  Buf.of_string (Socket.Basic_iov.string_of_t t)

let buf_of_full buf ofs t = 
  Socket.Basic_iov.string_of_t_full (Buf.string_of buf) (int_of_len ofs) t
    
let of_buf buf ofs len = 
  Socket.Basic_iov.t_of_string (Buf.string_of buf) (int_of_len ofs) (int_of_len len)
    
let empty = Socket.Basic_iov.empty
  
let len t = len_of_int (Socket.Basic_iov.len t)
  
let alloc ilen = 
  if ilen =|| len0 then 
    empty 
  else 
    let iov = Socket.Basic_iov.alloc (int_of_len ilen) in
    iov
      
let sub t ofs len' =
  assert (ofs +|| len' <=|| len t);
  Socket.Basic_iov.sub t (int_of_len ofs) (int_of_len len')
    
(* Decrement the refount, and free the cbuf if the count
 * reaches zero. 
 *)
let free = Socket.Basic_iov.free
  
(* Increment the refount, do not really copy. 
 *)
let copy = Socket.Basic_iov.copy
  
let really_copy = Socket.Basic_iov.really_copy
  
(* Flatten an iovec array into a single iovec.  Copying only
 * occurs if the array has more than 1 non-empty iovec.
 * In the case of a singlton array, the ref-count is incremented. 
 *
 * [flatten iov ilen] 
 * [ilen] is the length of the iovl.
 *)
let flatten = Socket.Basic_iov.flatten
  
let flatten_w_copy = Socket.Basic_iov.flatten_w_copy 
  
let debug () = (-1,-1)
  
let marshal a flags = Socket.Basic_iov.marshal a flags
  
let unmarshal t = Socket.Basic_iov.unmarshal t
  
module Priv = struct
  let md5_update_iov = Socket.md5_update_iov 
  let sendtov = Socket.sendtov
  let sendtosv = Socket.sendtosv 
    
  let udp_recv_packet s = 
    let str,iov = Socket.udp_recv_packet s in
    Buf.of_string str,iov
      
  let recv_iov = Socket.recv_iov
  let tcp_recv_packet = Socket.tcp_recv_packet
  let sendv_p = Socket.sendv_p
  let sendsv_p = Socket.sendsv_p
  let sends2v_p = Socket.sends2v_p
end
(**************************************************************)
  
