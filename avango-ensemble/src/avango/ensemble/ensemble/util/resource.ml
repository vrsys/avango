(**************************************************************)
(* RESOURCE.ML *)
(* Author: Mark Hayden, 4/96 *)
(**************************************************************)
open Trans
open Util
(**************************************************************)
let name = Trace.file "RESOURCE"
let failwith m = Trace.make_failwith name m
(**************************************************************)

type 'a entry = {
  item : 'a ;				(* actual data *)
  mutable count : int ;			(* reference count *)
  debug : debug				(* debug information *)
}

type ('a,'b) t = {
  name 		: name ;
  first 	: 'a -> 'b -> unit ;
  last 		: 'a -> 'b -> unit ;
  change	: ('a,'b) t -> unit ;
  change_min	: ('a,'b) t -> unit ;
  mutable array : 'b Arrayf.t ;
  table 	: ('a,'b entry) Hashtbl.t
}

(**************************************************************)

let create name first last change change_min = {
  name 	= name ;
  first = first ;
  last 	= last ;
  table = Hashtbl.create (*name*) 10 ;
  array = Arrayf.empty ;
  change = change ;
  change_min = change_min
}

(**************************************************************)

let simple name =
  create name ignore2 ignore2 ignore ignore

(**************************************************************)

let update e =
  let l = hashtbl_to_list e.table in
  let a = Arrayf.of_list l in
  e.array <- Arrayf.map (fun (_,d) -> d.item) a ;
  e.change e

(**************************************************************)

let add e debug k d =
  let data =
    try Hashtbl.find e.table k with Not_found ->
      e.first k d ;
      let data = {
	item = d ;
	count = 0 ;
	debug = debug
      } in
      Hashtbl.add e.table k data ;
      update e ;
      data
  in
  data.count <- succ data.count ;
  e.change_min e

(**************************************************************)

let remove e v =
  let data = 
    try Hashtbl.find e.table v with Not_found ->
      failwith "remove:no such item in table"
  in
  assert (data.count >= 1) ;
  data.count <- pred data.count ;
  if data.count = 0 then (
    e.last v data.item ;
    Hashtbl.remove e.table v ;
    update e
  ) ;
  e.change_min e

(**************************************************************)

let to_array r = r.array

(**************************************************************)

let info r =
  sprintf "RESOURCE:name=%s:size=%d" 
    r.name (hashtbl_size r.table)

let string_of_entry e = 
  string_of_pair ident string_of_int (e.debug, e.count)  

let to_string r =
  let table = hashtbl_to_list r.table in
  string_of_list (fun (_,d) -> string_of_entry d) table

(**************************************************************)
