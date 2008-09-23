(**************************************************************)
(* PROTO.ML *)
(* Author: Mark Hayden, 3/96 *)
(**************************************************************)
open Util
(**************************************************************)

type l = string
type id = string

let string_of_id = ident
let id_of_string = ident
let layers_of_id i = string_split ":" i
let l_of_string = ident
let string_of_l = ident

(**************************************************************)
