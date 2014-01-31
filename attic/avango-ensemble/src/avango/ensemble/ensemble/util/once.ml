(**************************************************************)
(* ONCE.ML : set-once variables *)
(* Author: Mark Hayden, 12/96 *)
(**************************************************************)
open Util
(**************************************************************)

type t = {
  name : string ;
  mutable data : bool
} 

let create name = {
  name = name ;
  data = false
} 

let set t =
  if t.data then (
    eprintf "ONCE:variable set 2nd time:%s\n" t.name ;
    failwith sanity ;
  ) ;
  t.data <- true
  
let isset t = t.data

let to_string t = string_of_bool t.data
