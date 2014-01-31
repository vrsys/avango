(**************************************************************)
(* TRACE.ML *)
(* Author: Mark Hayden, 3/96 *)
(**************************************************************)
open Util
(**************************************************************)

(* This is the non-debugging version of this module.
 * Everything is basically turned into no-ops.
 *)

(*
let file_report = ignore
let ensemble = "ENSEMBLE"
let file _ = ensemble
let filel = ident
let print_sources = ignore
let info = Util.info

let test_declare = ignore2
let test_exec _ = failwith "TRACE:test_exec:disabled"
let test_print = ignore

let install_root = ignore
let print_roots = ignore

let log _ = ignore
let log2 _ _ = ignore
let log3 _ _ _ = ignore
let logl = log
let logl2 = log2
let logl3 = log3
let log_check _ = false
let log_add = ignore2
let log_remove = ignore
let logs_all () = []
let logs_all_status = logs_all
let nlogs () = 0

let ncomments = nlogs
let comment = ignore
let config_print () = failwith "TRACE:config_print:disabled"

let dump = ignore
let helper a = let f _ = a in f
let debug a _ = helper a

(*
let rec(*avoid inline*) make_failwith a _ = failwith a
let _ = Trace.comment "using bad make_failwith"
let rec my_failwith a = failwith a
let make_failwith a b =
  my_failwith a
let make_failwith a _ = failwith a
let make_failwith _ _ = raise Division_by_zero
let rec(*avoid inline*) make_failwith a b = failwith (a^":"^b)
let make_failwith2 = make_failwith
*)
let rec inner _ = raise Division_by_zero
let make_failwith _ = inner
let make_failwith2 _ _ = inner ()
*)

(**************************************************************)
(* This is the debugging version of this module.
 *)


let dump f = f ()

let debug _ = ident
let always_info a b = a^":"^b
(*let info = always_info*)
let info a b = b

let source_report = ref (try let _ = Sys.getenv "ENS_TRACE" in true with _ -> false)
let sources = ref []
let file_report v =
  source_report := v

let file file =
  sources := file :: !sources ;
  if !source_report then (
    eprintf "TRACE:entering:%s\n" file ;	
    flush stderr
  ) ;
  file

let filel = file

let print_sources () =
  eprintf "  Source Files (that reported)\n" ;
  List.iter (fun file -> 
    eprintf "    %s\n" file) (List.rev !sources)

let tests = ref []

let test_declare name f =
  tests := (name,f) :: !tests

let test_exec name = 
  eprintf "TRACE:executing test '%s'\n" name ; flush stdout ;
  try
    let f = List.assoc name !tests in
    begin
      try f () with e ->
        eprintf "TRACE:test raised error:%s\n" (Util.error e)
    end ;
    exit 0
  with Not_found ->
    eprintf "TRACE:couldn't find test '%s'\n" name ;
    exit 0

let test_print () =
  eprintf "  Internal Tests\n" ;
  List.iter (fun (name,_) -> 
    eprintf "    %s\n" name) !tests

let roots = Queuee.create ()

let install_root f = Queuee.add f roots

let print_roots () = 
  eprintf "\n" ;
  Queuee.iter (fun f -> 
    let l = f () in
    List.iter (fun line ->
      eprintf "%s\n" line
    ) l
  ) roots ;
  eprintf "\n" ;
  flush stdout

let logs = Hashtbl.create 100

let get_log name =
  let name = string_uppercase name in
  try Hashtbl.find logs name with Not_found ->
    let ret = ref None in
    Hashtbl.add logs name ret ;
    ret

let log_check name =
  not (is_none !(get_log name))

let log name =
  let x = get_log name in
  fun f ->
    match !x with
    | None -> ()
    | Some g -> g "" (f ())

let log2 name info =
  let x = get_log name in
  fun f ->
    match !x with
    | None -> ()
    | Some g -> g info (f ())

let log3 name info0 info1 =
  let x = get_log name in
  fun f ->
    match !x with
    | None -> ()
    | Some g -> g (always_info info0 info1) (f ())

let logl name =
  let x = get_log name in
  fun g ->
    match !x with
    | Some f -> List.iter (f "") (g())
    | None -> ()

let logl2 name info =
  let x = get_log name in
  fun g ->
    match !x with
    | Some f -> List.iter (f info) (g())
    | None -> ()

let logl3 name info0 info1 =
  let x = get_log name in
  fun g ->
    match !x with
    | Some f -> List.iter (f (always_info info0 info1)) (g())
    | None -> ()

let log_add name f =
(*  let name = string_uppercase name in
  if name = "HSYS" then (
    Hsys.set_error_log (fun g -> f "" (g ()))
  ) else ( *)
  let x = get_log name in
  x := Some f

let log_remove name =
  let name = string_uppercase name in
  let x = get_log name in
  x := None

let logs_all_status () =
  let l = hashtbl_to_list logs in
  let l = List.map (fun (name,v) -> (name, (!v<>None))) l in
  l

let logs_all () =
  List.map fst (logs_all_status ())

let nlogs () = hashtbl_size logs

let comments = ref []

let ncomments () = List.length !comments

let comment s = comments := s :: !comments

let print_comments () =
  eprintf "Embedded comments:\n" ;
  List.iter (fun s -> eprintf "  %s\n" s) !comments

let _ =
  install_root (fun () ->
    [sprintf "TRACE:#logs=%d, #comments=%d" 
    (nlogs ()) (ncomments ())]
  )

let config_print () =
  eprintf "TRACE:config_print:begin\n" ;
  print_sources () ;
  test_print () ;
  eprintf "TRACE:config_print:end\n" ;
  flush stdout

(*
let rec(*avoid inline*) make_failwith a _ = failwith a
let _ = Trace.comment "using bad make_failwith"
let rec my_failwith a = failwith a
let make_failwith a b =
  my_failwith a
let make_failwith a _ = failwith a
let make_failwith _ _ = raise Division_by_zero
*)
let rec(*avoid inline*) make_failwith a =
  let failwith b = failwith (a^":"^b) in
  failwith
let make_failwith2 = make_failwith


(**************************************************************)
