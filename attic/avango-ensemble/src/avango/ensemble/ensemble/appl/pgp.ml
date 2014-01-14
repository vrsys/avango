(**************************************************************)
(* PGP: interface to PGP *)
(* Authors: Zhen Xiao, Mark Hayden, 4/97 *)
(* Ohad: Added computation in the background  *)
(*       Fixed "pgp strange-character" syndrome *)
(*       Added support for different versions of PGP *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "PGP"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
let log1 = Trace.log (name^"1")
let log2 = Trace.log (name^"2")
(**************************************************************)
(* General Support for background computation. Should be somewhere
 * else.
 *)

(* Things are lazy here so as not to disturb the non-secure stack with
 * computation of environment variables that may not be set.
*)
let home = 
  try Unix.getenv "HOME" 
  with Not_found -> failwith "Environment variable HOME does not exist"

let path = 
  try Unix.getenv "PATH" 
  with Not_found -> failwith "Environment variable PATH does not exist"

let pgppass =  lazy (Arge.check name Arge.pgp_pass)
let version =  lazy (
  try 
    let version = Unix.getenv "ENS_PGP_VERSION" in
    log (fun () -> sprintf "request PGP version is %s" version);
    version
  with Not_found -> 
    log (fun () -> 
      "ENS_PGP_VERSION environmet variable is not set. Default is 2.6" );
    "2.6"
)



type comm = 
  | Error of string
  | Reply of string
      
      
let env_array = lazy (
  let pgppass = Lazy.force pgppass in
  [|"PGPPASS="^pgppass;("HOME="^home);("PATH="^path)|]  
)

let verify_cmd sender receiver = 
  match Lazy.force version with 
    | "2.6" -> "pgp -f " ^ receiver ^" -u "^ sender 
    | "5.0" -> "pgpv -f +batch" 
    | "6.5" -> "pgp +COMPATIBLE -f " ^ receiver ^" -u "^ sender 
    | s -> failwith (sprintf "non-supported PGP version (%s)" s)

let sign_cmd sender receiver =
  match Lazy.force version with 
    | "2.6" -> "pgp -feas +batch "^receiver^" -u "^ sender 
    | "5.0" -> "pgpe -fast +batchmode=1 -r "^receiver^" -u "^ sender 
    | "6.5" -> "pgp +COMPATIBLE -fas +batch "^receiver^" -u "^ sender 
    | s -> failwith (sprintf "non-supported PGP version (%s)" s)


let read_sock debug alarm sock f =
  log2 (fun () -> "read_sock");
  let l = ref [] in
  let finalize () = 
    log2 (fun () -> "finalizing socket");
    Alarm.rmv_sock_recv alarm sock ;
    f (String.concat "" (List.rev !l))
  in
  let read () =
    let buf = String.create 10000 in
    try 
      let len = Hsys.read sock buf 0 (String.length buf) in
      log2 (fun () -> sprintf "%s:len=%d" debug len);
      if len = 0 then finalize ();
      l := (String.sub buf 0 len) :: !l
    with End_of_file -> 
      finalize ()
  in
  Alarm.add_sock_recv alarm name sock (Hsys.Handler0 read)

let select_process alarm notify cmd input =
  log2 (fun () -> sprintf "%s %s" cmd input);
  let handle,in_read,err_read = 
    Hsys.background_process cmd (Lazy.force env_array) input in
  let counter = ref 0 in
  let kill_proc () = 
    incr counter;
    if !counter = 2 then ignore (Unix.close_process_full handle)
  in    
  read_sock "get_error" alarm  err_read (fun s -> 
    kill_proc ();
    log2 (fun () -> "get_error");
    notify (Error s) ;
  ) ;
  read_sock "get_reply" alarm in_read (fun s -> 
    kill_proc ();
    log2 (fun () -> "get_reply");
    notify (Reply s)
  )

(**************************************************************)
type principal   = string
type sender      = principal 
type receiver    = principal 
type cipher_text = string 
type clear_text  = string 

let flatten_error s =
  string_map (fun c ->
    if c = '\n' then '|' else c
  ) s

(* sign the clear_text with the sender's secret key. The sender's pass
 * phrase, "pass", is needed to sign the message.
 *)
let sign sender receiver clear_text = 
  let clear_text = (hex_of_string clear_text) in 
  let command = sign_cmd sender receiver in
  log1 (fun () -> command);
  let (exit0,cipher,error) = Hsys.open_process command (Lazy.force env_array) clear_text in
  if exit0 then (
    log (fun () -> "sign succeeded") ;
    Some(cipher)
  ) else (
    log (fun () -> sprintf "sign failed") ;
    log (fun () -> sprintf "pgp info=%s" (flatten_error error)) ;
    None
  )


(* The same -- in the background
 *)
let bckgr_sign sender receiver clear_text alarm rep_fun =
  let clear_text = (hex_of_string clear_text) in 
  log (fun () -> sprintf "sender=%s, receiver=%s text=%s" sender receiver clear_text);
  let command = sign_cmd sender receiver in
  log1 (fun () -> command);
  select_process alarm (function
    | Error e -> 
	log2 (fun () -> sprintf "PGP Error = <%s>" (flatten_error e))
    | Reply cipher -> 
	if cipher="" then (
	  log (fun () -> "[bckground] sign failed") ;
	  rep_fun None
	) else (
	  log (fun () -> "[background] sign succeded");
	  rep_fun (Some cipher)
	)
  ) command clear_text
  

(* verify that the cipher_text comes from the [sender].
*)
let verify sender receiver cipher_text = 
  let command = verify_cmd sender receiver in
  log1 (fun () -> command);
  let (exit0,clear,error) = Hsys.open_process command (Lazy.force env_array) cipher_text in
  if exit0 then (
    log (fun () -> "verify succeeded") ;
    try 
      let clear = hex_to_string clear in
      Some clear
    with _ -> 
      log (fun () -> "String is not in hex format");
      None
  ) else (
    log (fun () -> "verify failed") ;
    log (fun () -> sprintf "pgp info=%s" (flatten_error error)) ;
    None
  )


(* The same -- in the background
 *)
let bckgr_verify sender receiver cipher_text alarm rep_fun =
  let command = verify_cmd sender receiver in
  log1 (fun () -> command);
  select_process alarm (function
    | Error e -> 
	log2 (fun () -> sprintf "PGP Error = <%s>" (flatten_error e))
    | Reply clear -> 
	if clear="" then (
	  log (fun () -> "[bckground] verify failed") ;
	  rep_fun None
	) else (
	  log (fun () -> "[background] verify succeded");
	  log1 (fun () -> sprintf "before hex_to_string %s" clear);
	  try 
	    let clear = hex_to_string clear in
	    rep_fun (Some clear)
	  with _ ->
	    log (fun () -> "String is not in hex format");
	    rep_fun None
	)
  ) command cipher_text


let _ =
  let princ id nm =
    if id <> Addr.Pgp then 
      failwith sanity ;
    Addr.PgpA(nm)
  in

  let sign id src dst clear =
    let src = Addr.project src id in
    let dst = Addr.project dst id in
    match src,dst with
    | Addr.PgpA(src),Addr.PgpA(dst) ->
	sign src dst clear
    | _,_ -> None
  in

  let bckgr_sign id src dst clear alarm rep_fun = 
    let src = Addr.project src id in
    let dst = Addr.project dst id in
    match src,dst with
    | Addr.PgpA(src),Addr.PgpA(dst) ->
	bckgr_sign src dst clear alarm rep_fun 
    | _,_ -> rep_fun None
  in

  let verify id src dst cipher =
    let src = Addr.project src id in
    let dst = Addr.project dst id in
    match src,dst with
    | Addr.PgpA(src),Addr.PgpA(dst) ->
	verify src dst cipher
    | _,_ -> None
  in

  let bckgr_verify id src dst cipher alarm rep_fun = 
    let src = Addr.project src id in
    let dst = Addr.project dst id in
    match src,dst with
    | Addr.PgpA(src),Addr.PgpA(dst) ->
	bckgr_verify src dst cipher alarm rep_fun 
    | _,_ -> rep_fun None
  in

  let auth =
    Auth.create
      name
      princ
      sign
      bckgr_sign
      verify
      bckgr_verify
  in
  
  Auth.install Addr.Pgp auth

(**************************************************************)
