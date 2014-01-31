(**************************************************************)
(* PARAM.ML *)
(* Author: Mark Hayden, 12/96 *)
(**************************************************************)
open Util
open Trans
(**************************************************************)
let name = Trace.file "PARAM"
let failwith s = Trace.make_failwith name s
(**************************************************************)

type t =
  | String of string
  | Int of int
  | Bool of bool
  | Time of Time.t
  | Float of float

type tl = (name * t) list

let defaults = Hashtbl.create 100

let rec(*avoid inlining*) default name t =
  Hashtbl.add defaults name t

let print_defaults () =
  eprintf "PARAM:default values:begin\n" ;
  Hashtbl.iter (fun name t ->
    eprintf "  %s=" name ;
    match t with
    | String v -> eprintf "String(%s)\n" v
    | Int v -> eprintf "Int(%d)\n" v
    | Bool v -> eprintf "Bool(%b)\n" v
    | Time v -> eprintf "Time(%s)\n" (Time.to_string v)
    | Float v -> eprintf "Float(%f)\n" v
  ) defaults ;
  eprintf "PARAM:default values:end\n"

(* We use List.mem_assoc first in order to avoid allocation
 * for the exception.
 *)
let lookup l name =
  if List.mem_assoc name l then (
    List.assoc name l
  ) else (
    try Hashtbl.find defaults name with Not_found ->
      eprintf "PARAM:lookup:%s\n" name ;
      failwith "lookup failed"
  )

let string tl name =
  match lookup tl name with
  | String s -> s
  | _ -> failwith "string"

let time tl name =
  match lookup tl name with
  | Time s -> s
  | _ -> failwith "time"

let int tl name =
  match lookup tl name with
  | Int s -> s
  | _ -> failwith "int"

let bool tl name =
  match lookup tl name with
  | Bool s -> s
  | _ -> failwith "bool"

let float tl name =
  match lookup tl name with
  | Float s -> s
  | _ -> failwith "float"

let to_string (name,value) =
  let string_rep,typ =
    match value with
    | Time(t)   -> (string_of_float (Time.to_float t) , "time")
    | Int(i)    -> (string_of_int i, "int")
    | Bool(b)   -> (string_of_bool b, "bool")
    | String(s) -> (s, "string")
    | Float(f)  -> (string_of_float(f), "float")
  in
  sprintf "%s=%s:%s" name string_rep typ
