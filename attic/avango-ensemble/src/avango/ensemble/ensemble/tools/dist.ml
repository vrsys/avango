open Unix
open Printf
open Mkutil

let flush = Pervasives.flush
let stdout = Pervasives.stdout

let port = 3460

(**************************************************************)

let open_process_in cmd =
  let shell,arg =
    if platform = Unix then ("/bin/sh","-c")
    else
      try ((Sys.getenv "COMSPEC"),"/c")
      with Not_found -> 
	raise(Unix_error(ENOEXEC, "open_proc", cmd)) 
  in
  let (in_read, in_write) = pipe () in
  let in_write2 = dup in_write in
  let inchan = in_channel_of_descr in_read in
  let pid = create_process shell [|shell;arg;cmd|] stdin in_write in_write2 in
  close in_write ;
  close in_write2 ;
  (pid,inchan)
  
let close_process_in (pid,inchan) =
  close_in inchan ;
  snd(waitpid [] pid)

(**************************************************************)

let strchr c s =
  let n = String.length s in
  let rec loop i =
    if i >= n then 
      raise Not_found
    else if s.[i] = c then
      i
    else loop (succ i)
  in loop 0

(**************************************************************)

let strstr c s =
  let n = String.length s in
  let rec loop i =
    if i >= n then 
      raise Not_found
    else if List.mem s.[i] c then
      i
    else loop (succ i)
  in loop 0

let chars_of_string s =
  let l = ref [] in
  for i = 0 to pred (String.length s) do
    l := s.[i] :: !l
  done ;
  !l

(**************************************************************)

let string_split c s =
  let c = chars_of_string c in
  let rec loop s =
    try
      let i = strstr c s in
      let hd = String.sub s 0 i in
      let tl = String.sub s (i+1) (String.length s - i - 1) in
      hd::(loop tl)
    with Not_found -> [s]
  in loop s

(**************************************************************)

let read_lines ch f =
  try 
    while true do
      f (input_line ch)
    done
  with End_of_file -> ()

(**************************************************************)

type message =
  | Read of string * int
  | Print of string * string
  | Contents of string * int

let server ic oc =
  printf "DIST:got connection\n" ;
  flush stdout ;
  try while true do
    let msg = input_value ic in
    match msg with
    | Read(file,time) ->
  	printf "DIST:request for file %s\n" file ;
	while (not (Sys.file_exists file))
	   || (stat file).st_mtime <= time
	do
	  sleep 10
	done ;

	printf "DIST:sending file\n" ; flush stdout ;
	begin
	  let time = (stat file).st_mtime in
	  let file = open_in_bin file in
	  let len = in_channel_length file in
	  let contents = String.create len in
	  really_input file contents 0 len ;
	  printf "DIST:read file\n" ; flush stdout ;
	  output_value oc (Contents(contents,time)) ;
	  flush oc ;
	end ;
	printf "DIST:done sending file\n" ; flush stdout ;

    | Print(host,msg) ->
	printf "%s:%s\n" host msg ;
	flush stdout ;
(*
	let file = Filename.concat "log" host in
	printf "hello\n" ; flush stdout ;
	let oc = open_out_gen [Open_creat;Open_append;Open_text] 0o644 file in
	printf "hello\n" ; flush stdout ;
	fprintf oc "%s\n" msg ;
	printf "hello3\n" ; flush stdout ;
	close_out oc ;
	printf "hello\n" ; flush stdout ;
*)
    | _ -> failwith sanity
  done with End_of_file ->
    printf "DIST:lost connection\n" ;
    flush stdout ;
    exit 0

let client file commands ic oc =
  let my_host = gethostname () in
  let time = ref 0 in
  let dir = Sys.getcwd () in
  while true do
    output_value oc (Read(file,!time)) ;
    flush oc ;

    let msg = input_value ic in
    match msg with
    | Contents(contents,time') ->
	printf "DIST:cleaning latest directory\n" ; flush stdout ;
	Sys.chdir dir ;
	(match platform with
	| Unix ->
	  Sys.command "rm -r -f latest"
	| Nt ->
	  Sys.command "rmdir /s /q latest"
	) ;
	Sys.command "mkdir latest" ;
	Sys.chdir "latest" ;
	printf "DIST:got file\n" ; flush stdout ;
	time := time' ;
	begin
	  let file = open_out_bin file in
	  output file contents 0 (String.length contents) ;
	  close_out file
	end ;
	let rec loop = function
	  | [] -> ()
	  | command :: tl ->
	      output_value oc (Print(my_host,command)) ;
	      flush oc ;
	      printf "DIST:exec'%s'\n" command ; flush stdout ;
	      let pid,child = open_process_in command in
	      read_lines child (fun line ->
	      	output_value oc (Print(my_host,line)) ;
	      	flush oc ;
	      ) ;
	      match close_process_in (pid,child) with
	      | WEXITED(i) when i = 0 ->
		  loop tl
	      | WEXITED(i) when i = 0 ->
		  let line = sprintf "*** ERROR: exit %d ***" i in
	    	  output_value oc (Print(my_host,line)) ;
	    	  flush oc
	      | _ ->
		  let line = sprintf "***Bad process_status ***" in
	    	  output_value oc (Print(my_host,line)) ;
	    	  flush oc
	in loop commands
    | _ -> failwith sanity
  done


let run_client host =
  let commands =
    let ext =
      match Sys.os_type with
      | "Unix" -> [
	  "gunzip -f ensemble.ext.tar.gz" ;
	  "tar xf ensemble.ext.tar" ;
	  "cd ensemble ; ocamlrun setup accepted" ;
	  "cd ensemble ; gunzip -f ensemble.tar.gz" ;
	  "tar xf ensemble/ensemble.tar" ;
	  "cd ensemble/demo ; make" ;
	  "cd ensemble/def ; make"
	]
      | "Win32" -> [
	  "gunzip -f ensemble.ext.tar.gz" ;
	  "tar xf ensemble.ext.tar" ;
	  "cd ensemble & ocamlrun setup accepted" ;
	  "cd ensemble & gunzip -f ensemble.tar.gz" ;
	  "tar xf ensemble\ensemble.tar" ;
	  "cd ensemble\demo & nmake -f Makefile.nt" ;
	  "cd ensemble\def & nmake -f Makefile.nt"
	]
      | _ -> failwith "bad config"
    and tmp =
      match Sys.os_type with
      | "Unix" -> [
	  "gunzip -c ensemble.tmp.tar.gz | tar xf - " ;
	  "cd ensemble/demo ; make" ;
	  "cd ensemble/def ; make"
	]
      | "Win32" -> [
	  "gunzip -f ensemble.tmp.tar.gz" ;
	  "tar xf ensemble.tmp.tar" ;
	  "cd ensemble\demo & nmake -f Makefile.nt" ;
	  "cd ensemble\def & nmake -f Makefile.nt"
	]
      | _ -> failwith "bad config"
    in
    
    tmp
  in
  
  let sa = 
    let host = gethostbyname host in
    let inet = host.h_addr_list.(0) in
    ADDR_INET(inet,port)
  in

  let (ic,oc) = open_connection sa in
  client "ensemble.tmp.tar.gz" commands ic oc ;
  exit 0
  

let run_server () =
(*
  if not (Sys.file_exists "log") then
    (Sys.command "mkdir log" ; ()) ;
*)
  let sa =
    let host = gethostname () in
    let host = gethostbyname host in
    let inet = host.h_addr_list.(0) in
    ADDR_INET(inet,port)
  in
  establish_server server sa ;
  exit 0
    
let run () =
  Arg.parse [
    "-client",Arg.String(fun s -> run_client s),"";
    "-server",Arg.Unit(run_server),""
  ] (fun _ -> failwith "bad arg") "" ;
  exit 0

let _ = 
  handle_unix_error run ()
