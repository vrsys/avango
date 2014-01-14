(**************************************************************)
(* DTBLBATCH.ML *)
(* Author: Zhen Xiao, 3/98 *)
(**************************************************************)
open Trans
open Util
(**************************************************************)
let name = Trace.file "DTBLBATCH"
let failwith s = Trace.make_failwith name s
(**************************************************************)

let tbl_size = 10

let cuts = Hashtbl.create tbl_size
let drops = Hashtbl.create tbl_size
let delays = Hashtbl.create tbl_size

let group = ref []

let default_drop_rate = ref (-1.0)
let default_delay = ref 0.0
let group_drop_rate = ref (-1.0)
let group_delay = ref 0.0


let read_file c =
  let rec loop c =
    try
      let s = input_line c in
      s :: loop c
    with End_of_file -> []  
  in 
    loop c


let rate_of_string rate = 
  let rate = float_of_string rate in
  if rate < 0.0 || rate > 1.0 
  then failwith "Invalid drop rate"
  else rate


let delay_of_string delay =
  let delay = float_of_string delay in
  if delay < 0.0
  then failwith "Invalid delay time"
  else delay


let parse my_name s =
  let l = string_split " \t" s in
  match l with
  | ["cut"; nd1; nd2] -> 
      if my_name = nd1 then Hashtbl.add cuts nd2 ()
      else if my_name = nd2 then Hashtbl.add cuts nd1 ()

  | ["drop"; nd1; nd2; drop_rate] ->
      let drop_rate = rate_of_string drop_rate in
      if my_name = nd1 
      then Hashtbl.add drops nd2 drop_rate
      else if my_name = nd2 
      then Hashtbl.add drops nd1 drop_rate

  | ["delay"; nd1; nd2; delay] ->
      let delay = delay_of_string delay in 
      if my_name = nd1 
      then Hashtbl.add delays nd2 delay
      else if my_name = nd2 
      then Hashtbl.add delays nd1 delay

  | ["default_drop_rate"; drop_rate] ->
      default_drop_rate := rate_of_string drop_rate ;

  | ["default_delay"; delay] ->
      default_delay := delay_of_string delay 

  | "group" :: ndlist -> 
      if List.mem my_name ndlist then group := ndlist

  | ["group_drop_rate"; drop_rate] ->
      group_drop_rate := rate_of_string drop_rate

  | ["group_delay"; delay] ->
      group_delay := delay_of_string delay

  | "#" :: _ -> ()
  | [] -> ()

  | _ -> failwith ("Syntax error in input file: " ^ s)
 

let init my_name fname =
  try
    let f = open_in fname in
    let l = read_file f in
    List.iter (parse my_name) l
  with Sys_error s -> 
    eprintf "Cannot open data file. %s\n" s ;
    exit 2


let cut origin = Hashtbl.mem cuts origin


let drop_rate origin = 
  if !group = [] || List.mem origin !group 
  then try
    Hashtbl.find drops origin
  with Not_found -> !default_drop_rate
  else !group_drop_rate


let delay origin = 
  if !group = [] || List.mem origin !group
  then try 
    Hashtbl.find delays origin
  with Not_found -> !default_delay
  else !group_delay
