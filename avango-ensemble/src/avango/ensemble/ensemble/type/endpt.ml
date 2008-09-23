(**************************************************************)
(* ENDPT.ML *)
(* Author: Mark Hayden, 7/95 *)
(**************************************************************)
open Trans
open Util
(**************************************************************)
let name = Trace.file "ENDPT"
let failwith = Trace.make_failwith name
(**************************************************************)

type id	= 
  | Anon of Unique.id			(* anonymous endpoints *)
  | Named of Unique.id * string 	(* named endpoints *)
  | External of string			(* externally named *)

type full = id * Addr.set

let id u = Anon(Unique.id u)
let named u s = Named((Unique.id u),s)
let extern unique_string = External unique_string

let string_of_id = function
  | Anon id -> 
      sprintf "{Endpt:%s}" (Unique.string_of_id_short id)
  | Named(id,debug) ->
      sprintf "{Endpt:%s:%s}" debug (Unique.string_of_id_short id)
  | External unique_string ->
      unique_string


let string_of_id_short = function
  | Anon id -> 
      sprintf "{Endpt:%s}" (Unique.string_of_id_short id)
  | Named(id,debug) ->
      sprintf "{Endpt:%s}" (Unique.string_of_id_short id)
  | External unique_string ->
      unique_string


let string_of_id_very_short = function
  | Anon id -> 
      sprintf "{E:%s}" (Unique.string_of_id_very_short id)
  | Named(id,debug) ->
      sprintf "{E:%s}" (Unique.string_of_id_very_short id)
  | External unique_string ->
      unique_string

let string_of_full e =
  string_of_pair string_of_id Addr.string_of_set e

let string_of_id_list l = 
  string_of_list string_of_id l

(**************************************************************)
