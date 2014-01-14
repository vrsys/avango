(**************************************************************)
(* GROUP.ML *)
(* Author: Mark Hayden, 4/96 *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "GROUP"
let failwith = Trace.make_failwith name
(**************************************************************)

type id	= 
  | Anon of Unique.id			(* anonymous groups *)
  | Named of string			(* named groups *)

let id u = Anon u
let named name = Named name

let string_of_id = function
  | Anon id    -> sprintf "{Group:Anon:%s}" (Unique.string_of_id_short id)
  | Named name -> sprintf "{Group:Named:%s}" name

let hash_of_id = function
  | Anon id -> Unique.hash_of_id id
  | Named name -> Hashtbl.hash (Digest.string name)

(**************************************************************)
