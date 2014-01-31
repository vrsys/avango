(**************************************************************)
(* DEBUG.ML *)
(* Author: Mark Hayden, 11/96 *)
(**************************************************************)
open Trans
open Util
open Buf
(**************************************************************)
let name = Trace.file "DEBUG"
let failwith = Trace.make_failwith name
let log = Trace.log name
(**************************************************************)

type message =
  | Add of name
  | Remove of name
  | Logs_request
  | Logs_reply of name list      
  | Data of name * string * string
      
let marsh,unmarsh = Iovecl.make_marsh true

(**************************************************************)

let server alarm port =
  let conns = ref [] in

  let client_init info send =
    let logs = ref [] in
    
    let send msg =
      let iovl = marsh msg in
      send Buf.empty len0 len0 iovl ;
      ()
    in

    let disable () =
      eprintf "DEBUG:lost connection to debugger on %s\n" info ;
      List.iter (fun name -> Trace.log_remove name) !logs
    in
	   
    let recv _ iovl =
      let msg = unmarsh iovl in
      match msg with
      | Add(name) ->
	  let send id data = send (Data(name,id,data)) in
	  Trace.log_add name send ;
	  logs := name :: !logs
      | Remove(name) ->
	  Trace.log_remove name ;
	  logs := except name !logs
      | Logs_request ->
	  let names = Trace.logs_all () in
	  send (Logs_reply names)
      | _ -> failwith sanity
    in
    recv,disable,()
  in

  (* Start the server.
   *)
  Hsyssupp.server name alarm port client_init
	
(**************************************************************)

let read_lines () =
  let stdin = Hsys.stdin () in
  let buf = String.create 10000 in
  let len = Hsys.read stdin buf 0 (String.length buf) in
  if len = 0 then raise End_of_file ;
  let buf = String.sub buf 0 len in
  let rec loop buf =
    try 
      let tok,buf = strtok buf "\n" in
      if tok = "" then (
	loop buf 
      ) else (
	tok :: loop buf
      )
    with Not_found -> []
  in
  loop buf

let client alarm host port =
  let sock = Hsys.socket_stream () in
  eprintf "DEBUG:connecting to process\n" ;
  Hsys.connect sock host port ;
  eprintf "DEBUG:connected\n" ;

  Hsyssupp.client name alarm sock (fun info send ->
    let send msg =
	let iovl = marsh msg in
	send Buf.empty len0 len0 iovl
    in

    let recv _ msg =
      let msg = unmarsh msg in
      match msg with
      | Data(kind,id,data) ->
	  printf "%s:%s:%s\n" kind id data
      | Logs_reply(names) ->
	  printf "DEBUGS:logs:%s\n" (String.concat "," names)
      | _ -> failwith sanity
    in

    let disable () =
      eprintf "DEBUG:lost connection\n" ;
      exit 0
    in

    let stdin = Hsys.stdin () in
    let input_handler () =
      try 
	List.iter (fun line ->
	  let toks = string_split " \t" line in
	  match toks with
	  | ["add";name] -> 
	      let name = String.uppercase name in
	      send (Add(name)) ;
	      printf "DEBUG:added '%s' to debugged names\n" name
	  | ["remove";name] -> 
	      let name = String.uppercase name in
	      send (Remove(name)) ;
	      printf "DEBUG:removed '%s' from debugged names\n" name
	  | ["logs"] ->
	      send Logs_request
	  | _ -> 
	      eprintf "DEBUG:bad command:line='%s'\n" line
	) (read_lines ())
      with
      | End_of_file ->
	  exit 0
      | e ->
	  eprintf "DEBUG:error:%s\n" (Util.error e)
    in

    Alarm.add_sock_recv alarm name stdin (Hsys.Handler0 input_handler) ;
    (recv,disable,())
  )

(**************************************************************)

