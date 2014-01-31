(**************************************************************)
(* HEAP.ML *)
(* Author: Mark Hayden, 11/97 *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "HEAP"
let failwith = Trace.make_failwith name
(**************************************************************)

(*
type code =				(*also in heapc.c*)
| Byte of int
| Native of string

let string_of_code = function
| Byte p -> sprintf "Byte(%08x)" p
| Native p -> sprintf "Native(%s)" p
*)

type code = string

let string_of_code x = x

type tags =
| Infix
| String
| Double_array
| Final
| Double
| Abstract
| Closure
| No_scan of int
| Numbered of int

let tag_of_int = function
| 249 -> Infix
| 250 -> Closure
| 251 -> Abstract
| 252 -> String
| 253 -> Double
| 254 -> Double_array
| tag when tag >= 251 -> No_scan tag
| tag -> Numbered tag

let string_of_tag = function
| Infix -> "Infix"
| Closure -> "Closure"
| Abstract -> "Abstract"
| String -> "String"
| Double -> "Double"
| Double_array -> "Double_array"
| No_scan tag -> sprintf "No_scan(%d)" tag
| Numbered tag -> sprintf "Numbered(%d)" tag
| Final -> "Final"

type taginfo = {
  mutable nwords : int ;
  mutable nitems : int
}

let obj_field_int o i =
  let f = Obj.field o i in
  if Obj.is_block f then
    failwith "obj_field_int:field is a block" ;
  (Obj.magic f : int)

let get_symbols file =
  let table = Hashtbl.create (*name*) 1000 in
  let command = sprintf "nm %s" file in
  eprintf "HEAP:running nm:'%s'\n" command ;
  let (ok,out,err) = Hsys.open_process command [||] "" in
  if not ok then
    failwith "get:couldn't read symbols" ;
  eprintf "HEAP:splitting lines[1]\n" ;
  let lines = string_split "\n" out in
  eprintf "HEAP:splitting lines[2]\n" ;
  let lines = List.map (string_split " \t") lines in
  eprintf "HEAP:building table[2]\n" ;
  List.iter (function
    | [addr;_;symbol] ->
	Hashtbl.add table addr symbol
    | _ -> ()
  ) lines ;
  eprintf "HEAP:done\n" ;
  table

let code_of_closure table c =
  let tag = tag_of_int (Obj.tag c) in
  if tag <> Closure then
    failwith "code_of_closure:not a closure" ;
  let nargs = obj_field_int c 1 in
  let code = Obj.field c 0 in
  let code = Hsys.addr_of_obj code in
  let code = try Hashtbl.find table code with Not_found -> code in
  if nargs = 1 then (
    sprintf "Closure(%s,1)" code
  ) else (
    let direct = Obj.field c 2 in
    let direct = Hsys.addr_of_obj direct in
    let direct = try Hashtbl.find table direct with Not_found -> direct in
    sprintf "Closure(%s,%s,%d)" direct code nargs
  )  

let symbols = ref None

let code_of_closure c =
  let table =
    match !symbols with
    | Some t -> t
    | None ->
	let t = get_symbols "rand" in
	symbols := Some t ;
	t
  in	  
  code_of_closure table c

let do_strings heap =
  let strings = Hashtbl.create (*name*) 10 in
  Array.iter (fun obj ->
    let tagi = Obj.tag obj in
    let tag = tag_of_int tagi in
    if tag = String then (
      let code = (Obj.magic obj : string) in
      let info =
	try
	  Hashtbl.find strings code
	with Not_found ->
	  let info = {nwords=0;nitems=0} in
	  Hashtbl.add strings code info ;
	  info
      in
      let size = Obj.size obj in
      info.nitems <- succ info.nitems ;
      info.nwords <- info.nwords + size ;
    )
  ) heap ;
  let l = ref [] in
  Hashtbl.iter (fun code info ->
    (*if info.nwords > 50 then*) (
      l := (info,code) :: !l
    )		  
  ) strings ;
  let strings = List.sort compare !l in
  List.iter (fun (info,string) ->
    eprintf "String:nitems=%d nwords=%d:'%s'\n"
      info.nitems info.nwords (String.escaped string) ;
  ) strings

let do_closures heap =
  let closures = Hashtbl.create (*name*) 10 in
  Array.iter (fun obj ->
    let tagi = Obj.tag obj in
    let tag = tag_of_int tagi in
    let size = Obj.size obj in
    if tag = Closure then (
      let code = code_of_closure obj in
      let info =
	try
	  Hashtbl.find closures code
	with Not_found ->
	  let info = {nwords=0;nitems=0} in
	  Hashtbl.add closures code info ;
	  info
      in
      let size = Obj.size obj in
      info.nitems <- succ info.nitems ;
      info.nwords <- info.nwords + succ size ;
    )
  ) heap ;

  let l = ref [] in
  Hashtbl.iter (fun code info ->
    if info.nwords > 0 then (
      l := (info,code) :: !l
    )		  
  ) closures ;
  let closures = List.sort compare !l in
  List.iter (fun (info,code) ->
    eprintf "%s:nitems=%d nwords=%d\n"
      (string_of_code code) info.nitems info.nwords ;
  ) closures

let do_biggies heap =
  let biggies = ref [] in
  Array.iter (fun obj ->
    let size = Obj.size obj in
    if size > 20 then
      biggies := ((succ size),obj) :: !biggies
  ) heap ;

  let biggies = List.sort (fun (a,_) (b,_) -> compare a b) !biggies in
  List.iter (fun (size,obj) ->
    let tag = Obj.tag obj in
    eprintf "%3d:%s:nwords=%d\n"
      tag (string_of_tag (tag_of_int tag)) size
  ) biggies

let do_histogram heap =
  let max_size = ref 0 in
  Array.iter (fun obj ->
    let size = Obj.size obj in
    if size > !max_size then
      max_size := size
  ) heap ;
  let hist = Array.create (succ !max_size) 0 in
  Array.iter (fun obj ->
    let size = Obj.size obj in
    hist.(size) <- succ hist.(size)
  ) heap ;
  Array.iteri (fun size nitems ->
    if nitems > 0 then
      eprintf "size=%d:nitems=%d:nwords=%d\n" size nitems (nitems * size)
  ) hist

let do_tags heap =
  let tags = Array.init 256 (fun _ -> {nitems=0;nwords=0}) in
  Array.iter (fun obj ->
    let tag = Obj.tag obj in
    let size = Obj.size obj in
    let info = tags.(tag) in
    info.nitems <- succ info.nitems ;
    info.nwords <- info.nwords + succ size ;
  ) heap ;
  Array.iteri (fun tagi info ->
    let info = tags.(tagi) in
    let tag = tag_of_int tagi in
    if info.nitems > 0 then (
      eprintf "%3d:%s:nitems=%d nwords=%d\n"
      	tagi (string_of_tag tag) info.nitems info.nwords ;
    ) ;
  ) tags

let analyze () =
  Gc.full_major () ;
  (* First, get the heap.
   *)
  let heap = Hsys.heap () in

  do_tags heap ;
(*
  do_histogram heap ;
*)
  do_biggies heap ;
  do_strings heap ;
(*
  do_closures heap ;
*)
  flush stdout

(**************************************************************)

let _ =
  Trace.test_declare "FRAMES" (fun () ->
    let frame_desc_size a =		(* for 32 bit machine *)
      4 + 2 + (Array.length a - 1) * 2
    in
    let frames = Hsys.frames () in
    printf "ndescriptors=%d\n" (Array.length frames) ;
    let size = Array.fold_left (fun i a -> i + frame_desc_size a) 0 frames in
    printf "size=%d\n" size ;
    let t = Hashtbl.create (*name*) 1000 in
    Array.iter (fun a ->
      if Array.length a < 2 then failwith "gorp" ;
      let al = Array.sub a 0 2 in
      let ar = Array.sub a 2 (Array.length a - 2) in
      let ar = Arraye.of_array ar in
      Arraye.sort compare ar ;
      let ar = Arraye.to_array ar in
      let a = Array.append al ar in
      (*if (a <> b) then failwith sanity ;*)
      try
	incr (Hashtbl.find t a)
      with Not_found ->
	Hashtbl.add t a (ref 1)
    ) frames ;
    let cframes = Array.map fst (Array.of_list (hashtbl_to_list t)) in
    let csize = Array.fold_left (fun i a -> i + frame_desc_size a) 0 cframes in
    let cframes = Arraye.of_array cframes in
    Arraye.sort compare cframes ;
    Arraye.iteri (fun i a -> printf "%08x %s\n" i (string_of_int_array a)) cframes ;
    printf "ndescr(compr)=%d\n" (hashtbl_size t) ;
    printf "size(compr)=%d\n"  (csize + Array.length frames * 8 - Arraye.length cframes * 4)
  )

(**************************************************************)
