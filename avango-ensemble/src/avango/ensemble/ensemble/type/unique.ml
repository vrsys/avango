(**************************************************************)
(* UNIQUE.ML *)
(* Author: Mark Hayden, 8/96 *)
(**************************************************************)
open Util
open Trans
(**************************************************************)
let name = Trace.file "UNIQUE"
(**************************************************************)

(* An id consists of an IP address, UDP port number, randomized
 * incarnation number, and a multiplexing number (mux).
 *)
type id	= {
  mux 		: mux ;
  host 		: Hsys.inet ;
  port 		: port ;
  incarn 	: incarn
}

(* This is a generator for ids.  It keeps a skeleton id
 * that it uses to generate the next id by inserting
 * then next mux value into it.
 *)
type t = {
  mutable id       : id ;
  mutable gen_mux  : mux ;
  mutable warned   : bool
} 

let create inet incarn = 
  let id = {
    mux = 0 ;
    port = -1 ;
    incarn = incarn ;
    host = inet ;
  } in
  { id = id ; warned = false ; gen_mux = 0 }

(**************************************************************)

let id gen =
  (* A warning is printed out once if a port is requested before
   * one has been installed.
   *)
  if gen.id.port = -1 && not gen.warned then (
    gen.warned <- true ;
    eprintf "UNIQUE:warning:Unique.id called prior to Unique.set_port\n" ;
    eprintf "  (this may prevent some Ensemble identifiers from being unique)\n"
  ) ;

  let mux = gen.gen_mux in
  gen.gen_mux <- succ mux ;
  { gen.id with mux = mux }

(**************************************************************)

let hash_of_id = Hashtbl.hash

let string_of_id_short id =
  let port = 
    if id.port < 0 then "" else
      string_of_int id.port
  in

  let host =
    if id.host = Hsys.inet_any () then "" else
      Hsys.string_of_inet_nums id.host
(*
      Hsys.string_of_inet_short id.host
*)
  in

  let incarn =
    if id.incarn = 0 then "" else
      string_of_int (id.incarn mod 1000)
  in

  let mux = string_of_int id.mux in
  
  let l = [host;port;incarn;mux] in
  let l = List.filter (fun s -> s <> "") l in
  String.concat ":" l


let string_of_id_very_short id =
  let host =
    if id.host = Hsys.inet_any () then "" else
      Hsys.string_of_inet_nums id.host
  in

  let incarn =
    if id.incarn = 0 then "" else
      string_of_int (id.incarn mod 1000)
  in

  let mux = string_of_int id.mux in
  
  let l = [host;incarn;mux] in
  let l = List.filter (fun s -> s <> "") l in
  String.concat ":" l


let string_of_id id =
  sprintf "{%s}" (string_of_id_short id)

(**************************************************************)

let set_port gen port =
  if gen.id.port = -1 then
    gen.id <- { gen.id with port = port }

(**************************************************************)
