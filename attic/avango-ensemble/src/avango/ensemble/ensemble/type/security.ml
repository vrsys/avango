(**************************************************************)
(* SECURITY *)
(* Author: Mark Hayden, 6/95 *)
(* Refinements: Ohad Rodeh 10/98 7/2000 *)
(**************************************************************)
open Trans
open Buf
open Util
(**************************************************************)
let name = Trace.file "SECURITY"
let log = Trace.log name
let failwith s = Trace.make_failwith name s
(**************************************************************)

(* Type of keys in Ensemble.
 * A key has two parts: 
 * 1. mac-key, used for keyed-hashing. 
 * 2. encyrption key, used for encryption functions.
 * Each of the subkeys are 16 bytes long. 
 *)
type mac = Buf.t
type cipher = Buf.t

type inner_key = {
  mac    : mac ; 
  cipher : cipher
}
    
type key = 
  | NoKey
  | Common of inner_key

let mac_len = len16
let cipher_len = len16
let key_len =  mac_len +|| cipher_len


let buf_of_cipher = ident

let cipher_of_buf buf = 
  if (Buf.length buf) < cipher_len then 
    failwith "cipher_of_buf: buf is too short";
  Buf.sub buf len0 cipher_len

let buf_of_mac = ident

let mac_of_buf buf = 
  if (Buf.length buf) < mac_len then 
    failwith "mac_of_buf: buf is too short";
  Buf.sub buf len0 mac_len

let get_cipher = function
  | NoKey -> failwith "get_cipher:NoKey"
  | Common key -> key.cipher

let get_mac = function
  | NoKey -> failwith "get_cipher:NoKey"
  | Common key -> key.mac
      
let buf_of_key = function
  | NoKey -> raise (Failure "Key=NoKey")
  | Common s -> Buf.append s.mac s.cipher

let string_of_key_short = function
  | NoKey -> "None"
  | Common key -> 
      let key = Buf.append key.mac key.cipher in
      let key = Buf.string_of key in
      let key = Digest.string key in
      let key = Util.hex_of_string key in
      let len = String.length key in
      let len = int_min 16 len in
      let key = String.sub key 0 len in
      sprintf "%s(sig)" key
      
let string_of_key = function
  | NoKey -> "{Key:None}"
  | Common key -> 
      let key = Buf.append key.mac key.cipher in
      let key = Buf.string_of key in
      let key = Digest.string key in
      let key = Util.hex_of_string key in
      sprintf "{Key:Shared:%s(sig)}" key

let key_of_buf s = 
  if Buf.length s < key_len then 
    failwith "Key is too short. must be (at least) 32 bytes long";
  let mac = Buf.sub s len0 mac_len in
  let cipher = Buf.sub s mac_len cipher_len in
  Common ({
    mac = mac ;
    cipher = cipher
  })

(**************************************************************)
