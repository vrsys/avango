(**************************************************************)
(* DTBL.ML *)
(* Author: Zhen Xiao, 2/98 *)
(**************************************************************)
open Trans
open Util
open Hsys
(**************************************************************)
let name = Trace.file "DTBL"
let log = Trace.log name
let failwith = Trace.make_failwith name
(**************************************************************)

let tbl_size = 5
let tbl = Hashtbl.create tbl_size

let create server port my_name =
  let sock = socket_stream () in
  let server = inet_of_string server in
  begin
    try 
      connect sock server port
    with e -> 
      eprintf "Error: %s, exiting\n" (error e) ;
      exit 1
  end ;
  log (fun () -> sprintf "connected to server\n") ;
  let msg = sprintf "reg %s" my_name in
  ignore (send_p sock (Buf.of_string msg) Buf.len0 (Buf.length (Buf.of_string msg))) ;
  sock


let parse msg =
  let l = string_split " \t" msg in
  match l with
  | ["suicide"] -> failwith "got killed by the server"
  | ["cut"; name] -> 
      if not (Hashtbl.mem tbl name) 
      then Hashtbl.add tbl name ()
  | ["repair"; name] -> Hashtbl.remove tbl name
  | _ -> log (fun () -> sprintf "Unknown msg from server\n")


let get_input sock = fun () ->
  let buf = String.create 1024 in
  let len = read sock buf 0 (String.length buf) in
  if len = 0 then 
    log (fun () -> sprintf "Lost connection to the server\n")
  else (
    let msg = String.sub buf 0 len in
    log (fun () -> sprintf "%s\n" msg) ;
    parse msg
  )


let leave my_name sock =
  let msg = sprintf "leave %s" my_name in
  let buf = Buf.of_string msg in
  ignore (send_p sock buf Buf.len0 (Buf.length buf)) ;
  close sock ;
  ()


let drop origin = Hashtbl.mem tbl origin
