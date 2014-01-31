(**************************************************************)
(* EMV.ML *)
(* Author: Mark Hayden, 12/96 *)
(* Changes: Ohad Rodeh 1/2001 *)
(**************************************************************)
open Printf

(**************************************************************)
type platform =
  | Unix
  | Other

let platform = match Sys.os_type with
  | "Unix" -> Unix
  | _ -> Other

let read_lines c =
  let lines = ref [] in
  try
    while true do
      let line = input_line c in
      lines := line :: !lines
    done ; []
  with End_of_file ->
    List.rev !lines
(**************************************************************)


let path = ref []
let files = ref []
let dest = ref None
let mlfile = ref false
let copy = ref false

let proc_file oc file =
(*
  let file =
    try find_path !path file with Not_found ->
      exit 0
  in
*)
  let modul = Filename.basename file in
  let modul = Filename.chop_extension modul in
  let modul = String.capitalize modul in

  if not (Sys.file_exists file) then
    failwith "file does not exist" ;

  if !mlfile then (
    if not !copy then (
      fprintf oc "module %s = %s\n" modul modul
    ) else (
      fprintf oc "module %s = struct\n" modul ;
      let ch = open_in file in
      let lines = read_lines ch in
      List.iter (fun line ->
	fprintf oc "  %s\n" line
      ) lines ;
      fprintf oc "end\n\n"
    )
  ) else (
    fprintf oc "module %s : sig\n" modul ;

    let ch = open_in file in
    let lines = read_lines ch in
    List.iter (fun line ->
      fprintf oc "  %s\n" line
    ) lines ;

    fprintf oc "end\n\n"
  )

let main () =
  Arg.parse [
    "-I",Arg.String(fun s -> path := s :: !path),": add dir to path" ;
    "-o",Arg.String(fun s -> dest := Some s),"" ;
    "-ml",Arg.Set mlfile,"" ;
    "-mli",Arg.Clear mlfile,"";
    "-copy",Arg.Set copy, "copy ML module bodies"
  ] (fun file -> files := file :: !files) "" ;

  let dest = 
    match !dest with
    | None -> failwith "no destination file given"
    | Some dest -> dest
  in

  if Sys.file_exists dest then
    Sys.remove dest ;
  let oc = open_out_bin dest in
  List.iter (proc_file oc) (List.rev !files) ;
  flush oc ;
  close_out oc ;

  if platform = Unix then (
    let cmd = sprintf "chmod -w %s" dest in
    let _ = Sys.command cmd in
    ()
  )

let _ = main ()
