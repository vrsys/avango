(**************************************************************)
(* CLIENT.ML: Client side of RES *)
(* Author: Ohad Rodeh , 9/99 *)
(**************************************************************)
open Aux
open Unix
open Printf

let env_array = Unix.environment ()

let send outchan msg = 
  Marshal.to_channel outchan msg [] ;
  flush outchan; 
  ()
    
(**************************************************************)
  (* A list of channels and appropriate handlers
   *)
let chan_tbl = ((Hashtbl.create 3) : (file_descr,(unit -> unit)) Hashtbl.t)
let sock_l = (ref [] : file_descr list ref)
  
let add_recv_sock sock f = 
  sock_l := sock :: !sock_l ;
  Hashtbl.add chan_tbl sock f
    
    
let rmv_recv_sock sock = 
  close sock;
  sock_l := List.find_all (fun i -> i<>sock) !sock_l;
  Hashtbl.remove chan_tbl sock
    
    
(**************************************************************)
let process_handles = (ref [] : (file_descr * file_descr * in_channel * out_channel * in_channel * re_opts) list ref)
 

let close_single ((sic,sec,ic,oc,ec,opts) as child) =  
  ignore (close_process_full (ic,oc,ec));
  sock_l := List.find_all (fun i -> i<>sic && i<>sec) !sock_l;
  Hashtbl.remove chan_tbl sic;
  Hashtbl.remove chan_tbl sec;
  process_handles := List.find_all (fun c -> c <> child) !process_handles

let close_children () = 
  List.iter (fun (sic,sec,ic,oc,ec,opts) -> 
    close_single (sic,sec,ic,oc,ec,opts)
  ) !process_handles;
  process_handles := []

(**************************************************************)
(* Perform a command [cmd]. Return a reply through [outchan] to 
 * the COORD.
*)
let read_proc_sock outchan sock_chan chan opts closed_channels id () = 
  let buf = String.create 5000 in
  let close_child_if chan = 
    incr closed_channels;
    rmv_recv_sock sock_chan;
    if !closed_channels = 2 (*1*) then (
      close_single id;
      print "Sending finish\n";
      if List.length !process_handles = 0 then 
	send outchan Finish 
    )
  in
  try 
    let len = (input chan buf 0 5000) in
    let s = String.sub buf 0 (len-1) in  (* We assume the last character is '\n' *)
    if len > 0 then 
      match opts with 
	| Local -> print (sprintf "line=%s\n" s)
	| To_coord -> send outchan (Line s)
	| _ -> ()
    else close_child_if chan
  with _ -> close_child_if chan


let handle_coord_msg inchan outchan () =  
  let do_cmd outchan opts = function
    | Do_command c -> 
	let ic,oc,ec = open_process_full c env_array in
	let sic = descr_of_in_channel ic in
	let sec = descr_of_in_channel ec in
	let closed_channels = ref 0 in
	let id = (sic,sec,ic,oc,ec,opts) in 
	add_recv_sock sic (read_proc_sock outchan sic ic opts closed_channels id);
	add_recv_sock sec (read_proc_sock outchan sec ec opts closed_channels id);
	process_handles := (sic,sec,ic,oc,ec,opts) :: !process_handles;
    | Do_finish ->
	close_children ();
	send outchan Finish 
    | Do_unit -> send outchan Finish 
  in
  let (l : (mngr_msg * re_opts) list ) = Marshal.from_channel inchan in
  List.iter (fun (msg,opts) -> 
    print (sprintf "(%s,%s)\n" (string_of_mngr_msg msg) (string_of_re_opts opts));
    do_cmd outchan opts msg
  ) l
    
(* Handle a connection by a COORD. Accept and execute commands. 
 *)
let accept_connection s inchan outchan = 
  add_recv_sock s (handle_coord_msg inchan outchan) ;
  try 
    while true do 
      let sl,_,_ = select !sock_l [] [] (-1.0) in
      List.iter (fun s -> 
	let f = Hashtbl.find chan_tbl s in
	f ()
      ) sl
    done
  with e -> 
    print ("Lost Coord <" ^ (hsys_error e) ^ ">, restarting.\n"); 
    rmv_recv_sock s;
    close_children ()

    
(**************************************************************)
    
(* Wait on a predetermined port [res_port] for a connection
 * to a server. The server will send commands to the client.
 * The client executes the commands and sends the resultant
 * output to the local screen or back to the server.
 *)


let run () = 
  let setup_listening_sock () = 
    let sockaddr  = ADDR_INET (inet_addr_any, res_port) in
    let sock      = socket PF_INET SOCK_STREAM 0 in
    setsockopt sock SO_REUSEADDR true ;
    bind sock sockaddr ;
    listen sock 3;
    print "Started Client\n";
    sock
  in

  let accept_sock = ref (setup_listening_sock ()) in

  let get_new_conn () = 
    try 
      accept !accept_sock
    with e -> 
      print ("Problem in accept socket" ^ (hsys_error e) ^ ", restarting.\n"); 
      accept_sock := setup_listening_sock ();
      accept !accept_sock
  in
  
  while true do 
    while true do 
      let (s,caller) = get_new_conn () in
      let inchan = in_channel_of_descr s in
      let outchan = out_channel_of_descr s in
      
      begin match caller with 
	| ADDR_UNIX _ -> failwith "ADDR_UNIX"
	| ADDR_INET (inet_addr,_) -> 
	    print (sprintf "Connected to =%s\n" (string_of_inet_addr inet_addr))
      end;
      accept_connection s inchan outchan
    done
  done
  
let _ = 
  Sys.set_signal Sys.sigpipe Sys.Signal_ignore;
  run ()
    
