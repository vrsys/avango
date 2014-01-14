(**************************************************************)
(* MEXCHANGE.ML : Utilities for the Exchange protocol *)
(* Author: Ohad Rodeh, 6/2000 *)
(*************************************************************************)
open Shared
open Util 
open Trans
(*************************************************************************)
let name = Trace.filel "MEXCHANGE"
let failwith s = Trace.make_failwith name s
let log = Trace.log name 
(*************************************************************************)

type src = (*Endpt.id*) string
type dst = (*Endpt.id*) string
type bignum = (*Shared.DH.bigum*) string 
type key = string
type nonce = int * string 

(* The message types we are working on. 
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
  
type t = {
  max_diff    : int ;
  encrypt     : Buf.t -> Buf.t ;
  nonce_init  : Buf.t ;
  view_id_buf : Buf.t    (* Used in the computation of nonces *)
}


let init max_diff view_id_str = 
  let nonce_init = Prng.create_buf (Buf.len_of_int 16) in 
  let idea_key = Prng.create_cipher () in
  let shared = Cipher.lookup "OpenSSL/RC4" in
  let encrypt = Cipher.single_use shared idea_key true in
  let view_id_buf = Buf.of_string view_id_str in
  {
    max_diff    = max_diff ;    
    encrypt     = encrypt ;
    nonce_init  = nonce_init ;
    view_id_buf = view_id_buf
  }
       
(* Create a nonce of the type : (time,f(time)).
 * Inflate the localtime to 128bits by concatenation, concatenate that
 * to a random string. Concatentate the result to the view_id. 
 * Digest the random pattern, and encrypt it under IDEA.
 *)
let create_nonce t ct = 
  let s1 = string_of_int ct in
  let rec loop s = 
    if String.length s >= 16 then String.sub s 0 16 
    else loop (s^s) 
  in
  let s1 = loop s1 in
  let s1 = Buf.append t.nonce_init (Buf.of_string s1) in
  let s1 = Buf.append t.view_id_buf s1 in
  let res = Buf.digest_sub s1 Buf.len0 (Buf.length s1) in
  (ct, hex_of_string (Buf.string_of (t.encrypt res)))
  
(* Check if a received nonce is from me. 
 *)
let check_nonce t my_time (ctime,nonce) = 
  if my_time - ctime < t.max_diff then (
    let (_,recreate) = create_nonce t ctime in
    recreate = nonce
  ) else 
    false 
      
let diff_nonce t my_time (ctime,nonce) = my_time - ctime 

(* Safe marshaling.
*)
let sep = String.make 1 (Char.chr 255)
  
let string_split c s = 
  let l = string_split c s in
  let l = List.find_all (fun x -> x <> "") l in
  l 
    
let string_of_nonce (ct,nonce) = 
  sprintf "(%d,%s)" ct nonce
    
let nonce_of_string m = 
  let l = string_split "(,)" m in
  let a,b = match l with
    | [a;b] -> a, b
    | _  -> raise (Bad_format "nonce_of_string")
  in
  let a = try int_of_string a with _ -> 
    raise (Bad_format "nonce_of_string:int_of_string")
  in (a,b)
       
       
let string_of_msg msg = 
  let f = function
    | Id (src,addr,nonce) -> 
	["Id"; 
	src ; 
	Addr.safe_string_of_set addr ; 
	string_of_nonce nonce ] 
    | Compx (inner,ticket) -> (
	match inner with 
	  | Piece (s_endpt,s_addr,d_nonce, d_endpt, d_addr, s_nonce, bignum) ->
	      [ "Piece"; 
	      s_endpt ; 
	      (Addr.safe_string_of_set s_addr); 
	      (string_of_nonce d_nonce) ; 
	      d_endpt ; 
	      (Addr.safe_string_of_set d_addr) ; 
	      (string_of_nonce s_nonce) ; 
	      bignum ; 
	      (Auth.string_of_ticket ticket) ]
	  | Reply (s_endpt, s_addr, d_nonce, d_endpt, d_addr, bignum, key) -> 
	      ["Reply" ; 
	      s_endpt ; 
	      (Addr.safe_string_of_set s_addr) ; 
	      (string_of_nonce d_nonce) ; 
	      d_endpt ; 
	      (Addr.safe_string_of_set d_addr) ; 
	      bignum ; 
	      (hex_of_string (Buf.string_of key)) ; 
	      (Auth.string_of_ticket ticket) ] 
      ) in
  String.concat sep (f msg)
    
let msg_of_string s = 
  let l = string_split sep s in
  if List.length l = 0 then 
    raise (Bad_format "msg_of_string: list length = 0");
  match l with 
    | ["Id" ; src; addr; nonce] -> 
	Id( src, Addr.safe_set_of_string addr, nonce_of_string nonce)
    | [ "Piece" ; s_endpt; s_addr; d_nonce;  d_endpt;  d_addr;  s_nonce;  bignum; ticket]  -> 
	let s_endpt = s_endpt 
	and s_addr = Addr.safe_set_of_string s_addr 
	and d_nonce = nonce_of_string d_nonce 
	and d_endpt = d_endpt 
	and d_addr = Addr.safe_set_of_string d_addr 
	and s_nonce = nonce_of_string s_nonce 
	and bignum = bignum 
	and ticket = Auth.ticket_of_string ticket in
	Compx (Piece (s_endpt,s_addr,d_nonce, d_endpt, d_addr, s_nonce, bignum), ticket)
	  
    | [ "Reply" ; s_endpt; s_addr;  d_nonce;  d_endpt;  d_addr;  bignum;  key; ticket] ->  
	let s_endpt = s_endpt 
	and s_addr = Addr.safe_set_of_string s_addr 
	and d_nonce = nonce_of_string d_nonce 
	and d_endpt = d_endpt 
	and d_addr = Addr.safe_set_of_string d_addr 
	and bignum = bignum 
	and key = Buf.of_string (hex_to_string key)
	and ticket = Auth.ticket_of_string ticket in
	Compx (Reply (s_endpt, s_addr, d_nonce, d_endpt, d_addr, bignum, key), ticket)
    | _ -> 
	raise (Bad_format "msg_of_string: bad list")
	  
	  
	  
	  
