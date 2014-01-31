(**************************************************************)
(* COORD.ML: scripting side and UI of RES *)
(* Author: Ohad Rodeh , 9/99 *)
(**************************************************************)
open Aux
open Unix
open Printf

(**************************************************************)
let verbose = ref false

let toggle_verbose () = verbose := not !verbose
(**************************************************************)

(* The type of a connection to a client
 *)
type conn = Connection of string * file_descr * in_channel * out_channel 

(* A list of connections with a hashtable for fast lookup.
 *)
type monitor = {
  mutable domain : string list;   (* Full list of machines. Some are disconnected *)
  mutable num    : int;           (* The number of connections *)
  mutable cl     : conn list ;    (* List of open connections *)
  sock_c_h       : (file_descr, conn) Hashtbl.t  (* Hashtbl between sockets and connections *)
}

type flag_rec = {
  mutable to_coord : bool ;
  mutable file     : string option;
  mutable time     : bool ;
  mutable record   : bool ;
  mutable num_appl : int option   (* The number of applications to run *)
}

(* The type of reply
 *)
type reply = 
  | Done
  | Error of string

(**************************************************************)

let string_of_reply = function
  | Done -> "Done" 
  | Error s -> "Error " ^ s

(* Convert a list of flags into a flag record.
*)
let rec_of_flag_l f_l = 
  let new_rec = {
    to_coord = false ;
    file     = None ;
    time     = false ;
    record   = false ;
    num_appl = None
  } in
  List.iter (function 
    | To_coord -> new_rec.to_coord <- true
    | Local    -> new_rec.to_coord <- false
    | File s   -> new_rec.file <- Some s
    | Time     -> new_rec.time <- true
    | Record   -> new_rec.record <- true
    | Num  i   -> new_rec.num_appl <- Some i
  ) f_l;
  new_rec

let rmv_mach mon m = 
  try 
    let conn = List.find (fun (Connection (mach,_,_,_)) -> mach=m) mon.cl in
    let Connection (mach,fd,_,_) = conn in 
    
    begin try shutdown fd SHUTDOWN_ALL
    with e -> print ("unsuccessful close "^ mach ^ " " ^ (hsys_error e) ^"\n");
    end;
    close fd;
    
    mon.cl <- List.find_all (fun (Connection (x,_,_,_)) -> x <> m) mon.cl ;
    Hashtbl.remove mon.sock_c_h fd;
    mon.num <- mon.num - 1
  with _ -> ()
      
(* [inet_addr_of_machine machine] IP address of [machine]
 *)
let inet_addr_of_machine m = 
  let h_entry = gethostbyname m in
  h_entry.h_addr_list.(0)
    
let establish_conn m = 
  try 
    let sock  = socket PF_INET SOCK_STREAM 0 in
    bind sock   (ADDR_INET (inet_addr_any, 0));
    let rmt_sockaddr = ADDR_INET (inet_addr_of_machine m, res_port) in
    connect sock rmt_sockaddr ;
    let inchan = in_channel_of_descr sock in
    let outchan = out_channel_of_descr sock in
    Some (Connection (m, sock, inchan, outchan))
  with e -> 
    print ("unsuccessful close "^ m ^ " " ^ (hsys_error e) ^"\n");
    None

(* [add_conn monitor machine] try connecting to [machine].
*)
let add_mach mon m = 
  if not (List.mem m mon.domain) then 
    mon.domain <- m :: mon.domain;
  if not (List.exists (fun (Connection (mach,_,_,_)) -> m = mach) mon.cl) then (
    match establish_conn m with 
      | None -> ()
      | Some (Connection (_,sock,_,_) as conn) -> 
	  mon.num <- succ mon.num ;
	  mon.cl <- conn :: mon.cl;
	  Hashtbl.add mon.sock_c_h sock conn
  )
    
let mon_create () = {
  domain   = [];
  num      = 0;
  cl       = [];
  sock_c_h = Hashtbl.create 31
}	

(* Close all the connections
 *)
let close_conn mon = 
  List.iter (fun (Connection(_,s,_,_)) -> 
    begin 
      try shutdown s SHUTDOWN_ALL
      with e -> print ("unsuccessful close "^ (hsys_error e) ^"\n")
    end;
    close s
  ) mon.cl;
  mon.num <- 0;
  mon.cl <- [];
  Hashtbl.clear mon.sock_c_h


(* Send a list of messages on [outchan]
*)
let send_msgs outchan msg_l = 
  Marshal.to_channel outchan msg_l [];
  flush outchan

(**************************************************************)
(* Global monitor.
*)
let mon = mon_create ()
let begin_time = time ()

(* [await_replys timeout time_flag rcrd] Wait for replys from the set of connections.
 * A reply is complete when Finish is received from a connection. The
 * function recursively waits for replies and prints all the output
 * received while waiting. 
 *)
let await_replys timeout flags =
  let init_time = Unix.gettimeofday () in
  let terminate_time = init_time +. (float_of_int timeout) in
  let sl = ref (List.map (function Connection (m,s,inchan,outchan) -> s) mon.cl) in
  let counter = ref (List.length !sl) in
  let replys  = ref [] in
  let replys2 = ref [] in
  let output_f = match flags.file with
    | None -> print
    | Some file -> 
	let output chan s = 
	  output_string chan s; flush chan ;
	in
	output (open_out_gen [Open_creat;Open_append;Open_wronly] 0400 file)
  in
  let add_reply m r s= 
    replys := r :: !replys; 
    decr counter ;
    if !verbose then 
      print (sprintf "%d %s rep=%s\n" !counter m (string_of_reply r));
    sl := List.find_all (fun sock -> sock <> s) !sl
  in
  let rec loop () = 
    let now = time () in
    if !counter = 0 
      || terminate_time <= now 
      || List.length !sl = 0 then (!replys,!replys2) else 
      begin
	let rl,_,_ = select !sl [] [] (terminate_time -. now) in
	List.iter (fun s -> 
	  let Connection(m,_,inchan,_) = Hashtbl.find mon.sock_c_h s in
	  try 
	    let rep = (Marshal.from_channel inchan : res_msg) in
	    match rep with 
	      | Line s -> 
		  if flags.record then replys2 := (m,s) :: !replys2
		  else
		    if flags.time then 
		      let now = time () in
		      output_f (m ^":" ^ string_of_float (now -. begin_time) ^ ":"^ s ^"\n")
		    else
		      output_f (m ^":" ^ s ^"\n")
	      | Finish -> add_reply m Done s
	  with e -> 
	    add_reply m (Error (hsys_error e)) s
	) rl;
	loop () 
      end
  in loop ()
       
let send_command mon cmd_f opts = 
  let cl_a = Array.of_list mon.cl in
  Array.iteri (fun i (Connection(m,s,inchan,outchan)) -> 
    try 
      send_msgs outchan [(cmd_f i m),opts]
    with _ -> 
      print ("Channel failed, removing\n");
      rmv_mach mon m
  ) cl_a

(* A fancy version allowing openning several applications on 
 * each machines. 
*)
let send_command_appl mon cmd_f opts num_appl = 
  let cl_a = Array.of_list mon.cl in
  let len = Array.length cl_a in
  let a = 
    let rec loop array_c counter a = 
      if counter = num_appl then a
      else 
	if array_c = len then 
	  loop 0 counter a 
	else (
	  a.(array_c) <- counter :: a.(array_c);
	  loop (succ array_c) (succ counter) a
	)
    in loop 0 0 (Array.create len [])
  in
  print (sprintf "layout=%s\n" (string_of_array string_of_int_list a));
  Array.iteri (fun i (Connection(m,s,inchan,outchan)) -> 
    try 
      let msg = List.map (fun j -> (cmd_f j m),opts) (a.(i)) in
      send_msgs outchan msg 
    with _ -> 
      print ("Channel failed, removing\n");
      rmv_mach mon m
  ) cl_a

    
(* [rpc_command st cmd_f opts] executes command [cmd_f] on the
 * set of clients [st]. This function takes as long as the 
 * commands are executed on the remote machines.
 * 
 * If break is pressed during this command, a Do_finish is
 * sent to all the clients.
 *)
let rpc_command cmd_f timeout opts_l = 
  let flag_rec = rec_of_flag_l opts_l in
  let break () = 
    send_command mon (fun _ _ -> Do_finish) To_coord ;
    ignore (await_replys 10  {
      to_coord = false ;
      file     = None ;
      time     = false ;
      record   = false ;
      num_appl = None ;
    }
    )
  in
  try 
    let coord_flag = 
      if flag_rec.to_coord then To_coord 
      else Local in
    let num_appl = match flag_rec.num_appl with
      | None -> mon.num 
      | Some num -> num in
    send_command_appl mon cmd_f coord_flag num_appl;

    let replys,replys2 = await_replys timeout flag_rec in
    if List.length replys <> mon.num then (
      if !verbose then 
	print "Didn't get all the replies, killing remote processes\n";
      break ()
    );
    replys2
  with Sys.Break -> 
    print "Break, killing remote processes\n";
    break ();
    []
      
(**************************************************************)
(* Non functional. Uses a global set of clients [client_l].
*)

(* [connect_full ml]  Try concecting to a list of machines [ml].
 * Connect to as many as possible. 
*)
let connect ml = List.iter (fun mach -> add_mach mon mach) ml

(* [close ml] Close specific connections.
*)
let set_live m_l =
  let dead_machines = List.find_all (function m -> not (List.mem m m_l)) mon.domain in
  List.iter (fun mach -> 
    rmv_mach mon mach
  ) dead_machines;
  dead_machines


let rpc cmd_f timeout opts_l = 
  ignore (rpc_command cmd_f timeout opts_l)

(* 
 * Get the uptimes from all the machines. 
 * 
 * A typical lines looks like this:
 * apollo-2,   "2:51pm  up 4 days, 21:04,  1 user,  load average: 1.11, 1.29, 1.35"
 * 
 * Return only (apollo-2, 1.35)
 *)
let rpc_uptime () = 
  let replys = rpc_command (fun _ _ -> Do_command "uptime") 5 [To_coord;Time;Record] in
  if (List.length replys) <> mon.num then 
    printf "Didn't get all the replys [%d/%d] \n" (List.length replys) mon.num;
  let replys = List.map (fun (m,s) ->
    let len = String.length s in
    let s = String.sub s (len-17) 17 in
    let s_l = Str.split (Str.regexp "[:, ]+") s in
    m, float_of_string (List.nth s_l 0)
  ) replys in

  
  
  (* 
   * int_of_string "pd-1" = "pd", "1"
   * int_of_string "apollo-4" = "apollo", "4"
   *)
  
  let int_of_string s = 
    try 
      let i = String.index s '-' in
      let m_name = String.sub s 0 i 
      and m_number = String.sub s (i+1) (String.length s - (i +1)) in
      (m_name,int_of_string m_number)
    with Not_found -> 
      (* This version works for the parpar.
       *)
      let len = String.length s in
      try 
	let m_number = String.sub s (len-1) 1 in
	let m_name = String.sub s 0 (len-1) in
	(m_name,int_of_string m_number)
      with _ -> 
	try 
	  let m_number = String.sub s (len-2) 2 in
	  let m_name = String.sub s (len-2) 2 in
	  (m_name,int_of_string m_number)
	with _ -> 
	  (s,0)
  in
  
  (* The default compare between strings is no good to us. 
   * We use our own brew.
   *)
  let replys = Sort.list (fun (x,_) (y,_) -> 
    let m1,n1 = int_of_string x 
    and m2,n2 = int_of_string y
    in m1 > m2 || m1=m2 && n1 > n2
  ) replys in
  replys
    
(* Print the list of open connections.
 *)
let show_conn_list () = 
  List.map (fun (Connection (m,_,_,_)) -> m) mon.cl
    
(**************************************************************)
(* Remote execution commands
*)
let rexec machine prog = 
  Sys.command ("rsh " ^ machine ^ " " ^ prog ^ " >& /dev/null &")


(* Test main
*)
(*
let _ = 
  connect ["par1"; "par3"];
  rpc_uptime ()
*)
(**************************************************************)
(* Break handler
*)
let _ = 
  printf "Setting break handlers\n"; flush_out ();
  Sys.set_signal Sys.sigpipe Sys.Signal_ignore;
  Sys.catch_break true

(**************************************************************)


