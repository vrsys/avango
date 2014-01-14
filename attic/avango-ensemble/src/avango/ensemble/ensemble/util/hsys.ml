(**************************************************************)
(* HSYS.ML *)
(* Author: Mark Hayden, 5/95 *)
(**************************************************************)
open Buf
open Printf
open Socket
(**************************************************************)
let name = "HSYS"
let failwith s = failwith (name^":"^s)
let log = Trace.log name
(**************************************************************)

let fprintf ch fmt =
  let f s = 
    output_string ch s ;
    flush ch
  in
  Printe.f f fmt

let ident a = a
let sprintf fmt = Printe.f ident fmt
let eprintf fmt = fprintf stderr fmt

(**************************************************************)

type debug = string
type port = int
type socket = Socket.socket

type inet = Unix.inet_addr
type sendto_info = Socket.sendto_info
type sock_info = ((socket array) * (bool array)) option
type select_info = Socket.select_info

type timeval = Socket.timeval = {
  mutable sec10 : int ;
  mutable usec : int
} 

type handler =
  | Handler0 of (unit -> unit)
  | Handler1

type socket_option =
  | Nonblock of bool
  | Reuse
  | Join of inet
  | Leave of inet
  | Ttl of int 
  | Loopback of bool
  | Sendbuf of int
  | Recvbuf of int
  | Bsdcompat of bool

(**************************************************************)

(* Wrap uses of TCP, so that exceptions will be logged, and a len0
 * will be returned. This simplifies handling TCP recv's at
 * higher levels. 
*)
let rec unix_wrap_again debug f =
  try f () with Unix.Unix_error(err,s1,s2) as exc ->
    match err with 
    | Unix.EINTR
    | Unix.EAGAIN ->
	log (fun () -> sprintf "warning:%s:%s" debug (Util.error exc)) ;
	unix_wrap_again debug f
    | Unix.ECONNREFUSED 
    | Unix.ECONNRESET 
    | Unix.EHOSTDOWN			(* This was reported on SGI *)
    | Unix.ENETUNREACH
    | Unix.ENOENT
    | Unix.EPIPE 
    | Unix.ECONNABORTED         
    | Unix.ESHUTDOWN
    | Unix.EBADF -> 
	log (fun () -> sprintf "warning:%s:%s" debug (Util.error exc)) ;
	len0
    | _ ->
	eprintf "HSYS:%s:%s\n" debug (Util.error exc) ;
	flush stderr ;
	raise exc
	  
(**************************************************************)

let bind sock inet port = Socket.bind sock (Unix.ADDR_INET(inet,port))
let close sock		= Socket.close sock
let connect sock inet port = Socket.connect sock (Unix.ADDR_INET(inet,port))
let getlogin 		= Unix.getlogin
let getpid              = Unix.getpid
let gettimeofdaya 	= Socket.gettimeofday
let has_ip_multicast 	= Socket.has_ip_multicast
let inet_any () 	= Unix.inet_addr_any
let int_of_socket 	= Socket.int_of_socket
(*let int_of_substring	= Socket.int_of_substring*)
let listen sock i	= Socket.listen sock i
let select      	= Socket.select
let poll        	= Socket.poll

(* Convert errors on stdin into a return value of 0.
*)
let read s buf ofs len  = 
  try Socket.read s buf ofs len 
  with e -> 
    log (fun () -> sprintf "error in stdin %s\n" (Util.error e));
    (*printf "error in stdin %s\n" (Util.error e); Pervasives.flush Pervasives.stdout;*)
    0
(**************************************************************)
let sendto_info s a 	= Socket.sendto_info s (Array.map (fun (i,p) -> Unix.ADDR_INET(i,p)) a)

let tcp_info s = 
(*  let sockname = Unix.getsockname s in
  let inet,port = match sockname with 
      Unix.ADDR_UNIX _ -> failwith "socket is a UNIX socket, not a TCP/IP one"
    | Unix.ADDR_INET (inet,port) -> inet,port
  in*)
  sendto_info s [||] (*[|inet,port|]*)

let sendto sendto_info buf ofs len =
  Socket.sendto sendto_info (Buf.string_of buf) (int_of_len ofs) (int_of_len len) 

let sendtov info iovl = 
  Iovec.Priv.sendtov info (Iovecl.to_iovec_array iovl)
    
let sendtosv info buf ofs len iovl = 
  Iovec.Priv.sendtosv info 
    (Buf.string_of buf) (Buf.int_of_len ofs) (Buf.int_of_len len) 
    (Iovecl.to_iovec_array iovl) 
    
(* PERF: I hope this is removed by ocamlopt !
*)
let udp_recv_packet sock = 
  let hdr,iovec = Iovec.Priv.udp_recv_packet sock in
  hdr, iovec

let recv s b o l 	= 
    len_of_int (Socket.recv s (Buf.string_of b) (int_of_len o) (int_of_len l))

let tcp_recv s b o l 	= 
  unix_wrap_again "recv_iov" (fun () -> 
    len_of_int (Socket.recv s (Buf.string_of b) (int_of_len o) (int_of_len l))
  )

let tcp_recv_iov s iov o l 	= 
  unix_wrap_again "recv_iov" (fun () -> 
    len_of_int (Iovec.Priv.recv_iov s iov (int_of_len o) (int_of_len l))
  )

let tcp_recv_packet sock buf ofs len iov = 
  unix_wrap_again "tcp_recv_packet" (fun () -> 
    len_of_int (Iovec.Priv.tcp_recv_packet sock 
      (Buf.string_of buf) (int_of_len ofs) (int_of_len len) iov)
  )

let send_p s b o l = 
  unix_wrap_again "send_p" (fun () -> 
    len_of_int (Socket.send_p s (Buf.string_of b) (int_of_len o) (int_of_len l)))

let sendv_p s iovl = 
  unix_wrap_again "sendv_p" (fun () -> 
    len_of_int (Iovec.Priv.sendv_p s (Iovecl.to_iovec_array iovl))
  )

let sendsv_p s buf ofs len iovl = 
  unix_wrap_again "sendsv_p" (fun () -> 
    len_of_int (Iovec.Priv.sendsv_p s (Buf.string_of buf) 
      (Buf.int_of_len ofs) (Buf.int_of_len len) (Iovecl.to_iovec_array iovl))
  )

let sends2v_p s buf1 buf2 ofs len iovl = 
  unix_wrap_again "sends2v_p" (fun () -> 
    len_of_int (Iovec.Priv.sends2v_p s
      (Buf.string_of buf1) 
      (Buf.string_of buf2) (Buf.int_of_len ofs) (Buf.int_of_len len) 
      (Iovecl.to_iovec_array iovl))
  )
(**************************************************************)

let socket_dgram () 	= Socket.socket Unix.PF_INET Unix.SOCK_DGRAM 0
let socket_stream () 	= Socket.socket Unix.PF_INET Unix.SOCK_STREAM 0
let socket_mcast () 	= Socket.socket_mcast Unix.PF_INET Unix.SOCK_DGRAM 0
let stdin 		= Socket.stdin
let string_of_inet_nums = Unix.string_of_inet_addr
let substring_eq s1 ofs1 s2 ofs2 len = 
  Socket.substring_eq 
    (Buf.string_of s1) (int_of_len ofs1)
    (Buf.string_of s2) (int_of_len ofs2) (int_of_len len)

(**************************************************************)
type md5_ctx = Socket.md5_ctx

let md5_init            = Socket.md5_init
let md5_init_full       = Socket.md5_init_full
let md5_update ctx buf ofs len = 
  Socket.md5_update ctx (Buf.string_of buf) (Buf.int_of_len ofs) (Buf.int_of_len len)

let md5_update_iov      = Iovec.Priv.md5_update_iov
let md5_final           = Socket.md5_final

(**************************************************************)

let md5_update_iovl ctx il = 
  let il = Iovecl.to_arrayf il in
  for i = 0 to pred (Arrayf.length il) do
    md5_update_iov ctx (Arrayf.get il i) 
  done

(**************************************************************)

let getenv key =
  try Some(Sys.getenv key)
  with Not_found -> None

(**************************************************************)

let empty = ([||],[||])
let sihelp = function
  | None -> empty
  | Some a -> a

let select_info a b c = Socket.select_info (sihelp a) (sihelp b) (sihelp c)

(**************************************************************)

let getsockname sock =
  match Unix.getsockname sock with
  | Unix.ADDR_INET(inet,port) -> (inet,port)
  | _ -> raise Not_found

(**************************************************************)

let getpeername sock =
  match Unix.getpeername sock with
  | Unix.ADDR_INET(inet,port) -> (inet,port)
  | _ -> raise Not_found

(**************************************************************)

let gethostname () =
  let name = Unix.gethostname () in
  String.lowercase name

(**************************************************************)

let gettimeofday () =
  let tv = { sec10 = 0 ; usec = 0 } in
  Socket.gettimeofday tv ;
  tv

(**************************************************************)
(* The Nonblock option uses the socket library because the
 * Ocaml Unix library implements setsocket non-blocking
 * operations as no-ops.
 *)

let setsockopt sock = function
  | Reuse       -> Socket.setsockopt_reuse sock true
  | Nonblock f  -> Socket.setsockopt_nonblock sock f
  | Join inet   -> Socket.setsockopt_join sock inet
  | Leave inet  -> Socket.setsockopt_leave sock inet
  | Ttl i       -> Socket.setsockopt_ttl sock i
  | Loopback onoff -> Socket.setsockopt_loop sock onoff
  | Sendbuf len -> Socket.setsockopt_sendbuf sock len
  | Recvbuf len -> Socket.setsockopt_recvbuf sock len
  | Bsdcompat b  -> Socket.setsockopt_bsdcompat sock b

let in_multicast = Socket.in_multicast 
(**************************************************************)

let recvfrom s b o l = 
  let l,sa = Socket.recvfrom s (Buf.string_of b) (Buf.int_of_len o) (Buf.int_of_len l) in
  match sa with 
  | Unix.ADDR_INET(i,p) -> (Buf.len_of_int l,i,p)
  | _ -> failwith "recv_from:non-ADDR_INET"

(**************************************************************)

(* This version is wrapped below with a caching version.
 *)
let inet_of_string inet =
  try 
    Unix.inet_addr_of_string inet
  with _ ->
    try
      let hostentry = Unix.gethostbyname inet in
      if Array.length hostentry.Unix.h_addr_list < 1 then (
      	eprintf "HSYS:inet_of_string: got empty list for host %s, raising Not_found\n" inet ;
	raise Not_found
      ) ;
      hostentry.Unix.h_addr_list.(0)
    with Not_found ->
      if inet = gethostname () then (
      	eprintf "HSYS:inet_of_string: host %s not found; defaulting to localhost\n" inet ;
      	Unix.inet_addr_of_string "127.0.0.1"
      ) else (
      	raise Not_found
      )

(**************************************************************)
(* Hashtable for caching gethostbyname results.
 * And an int ref for keeping track of the size.
 *)
let inet_table = (Hashtbl.create 10 : (string, (int * Unix.inet_addr option) ref) Hashtbl.t)
let inet_table_size = ref 0

(* Time constants used below.  We use the gettimeofday directly.
 *)
(* let timeout_good = 15 * 60             (* 15 minutes *) *)
(* let timeout_bad = 12 * 60 * 60         (* 12 hours *) *)
let timeout_good = 15 * 60 / 10        (* 15 minutes *)
let timeout_bad = 12 * 60 * 60 / 10  (* 12 hours *)
let timeout_init = -1                  (* hack *)

(**************************************************************)

let inet_of_string name = 
  (* Get the current time and use this to check for expired
   * entries.  
   *)
  (* let now = ((gettimeofday ()).sec10 * 10) + ((gettimeofday ()).usec / 1000000) in *)
  let now = (gettimeofday ()).sec10 in
  if now < 0 then 
    failwith "sanity:gettimeofday < 0" ;

  let memo = 
    try
      Hashtbl.find inet_table name
    with Not_found ->
      (* If not in table already, create an entry and add it
       * to the table.  If size ever goes above 1000 then
       * clear the table to prevent a space leak.  
       *)
      incr inet_table_size ;
      if !inet_table_size > 1000 then (
	eprintf "HSYS:inet_of_string:size > 1000, clearing cache" ;
	Hashtbl.clear inet_table ;
	inet_table_size := 1
      ) ;
      let memo = ref (timeout_init,None) in
      Hashtbl.add inet_table name memo ;
      memo
  in
  let expir,entryopt = !memo in

  (* If the expiration time is still ok, then just use the
   * cached entry.  
   *)
  if now < expir then (
    match entryopt with
    | None -> raise Not_found
    | Some entry -> entry
  ) else (
    (* Print a warning when about to do a gethostbyname to
     * a bad entry.  This is because we may end up
     * blocking the process.  
     *)
    if entryopt = None && expir <> timeout_init then (
      eprintf "HSYS:inet_of_string:warning:trying again for previously bad entry (%s)\n" name ;
      flush stderr
    ) ;
    
    (* Calculate the new entry and override the previous value.
     *)
    try
      let entry = inet_of_string name in
      let expir = now + timeout_good in
      memo := (expir,(Some entry)) ;
      entry
    with Not_found ->
      let expir = now + timeout_bad in
      memo := (expir,None) ;
      raise Not_found
  )

(**************************************************************)

let gethost () =
  try
    inet_of_string (gethostname ())
  with _ ->
    failwith "HSYS:gethost:unable to get IP address for this host"

(**************************************************************)

let getlocalhost () =
  try
    inet_of_string "127.0.0.1"
  with _ ->
    failwith "HSYS:getlocalhost:unable to get localhost IP address"

(**************************************************************)

let accept sock =
  match Socket.accept sock with
  | (fd,Unix.ADDR_INET(inet,port)) -> (fd,inet,port)
  | _ -> failwith "accept:non-ADDR_INET"

(**************************************************************)

(** using a public routing addr, instead of a private one **)
(** let deering_prefix = "239.255.0." **)
let deering_prefix = "233.4.250."

let deering_addr i =
  let i = (abs i) mod 248 in
  let inet_s = sprintf "%s%d" deering_prefix i in
  Unix.inet_addr_of_string inet_s

(**************************************************************)

let string_of_inet inet =
  try
    let {Unix.h_name=long_name} =
      Unix.gethostbyaddr inet 
    in
    let name = String.lowercase long_name in
    name
  with _ ->
    let inet_s = Unix.string_of_inet_addr inet in
    let ofs = String.length deering_prefix
    and len = String.length inet_s in
    if deering_prefix = String.sub inet_s 0 (min ofs len) then
      sprintf "IPM.%s" (String.sub inet_s ofs (len-ofs))
    else
      inet_s

let string_of_inet_short inet =
  let inet = string_of_inet inet in
  try
    let i = String.index inet '.' in
    String.sub inet 0 i
  with Not_found ->
    inet

(**************************************************************)

(* Ohad 
 *)
let inet_of_unix_inet_addr inet = inet
let unix_inet_addr_of_inet inet = inet

(* These simply returns the text readable value the
 * inet (XXX.YYY.ZZZ.TTT). Used for safe marshaling routines.
*)
let simple_string_of_inet inet = Unix.string_of_inet_addr inet
let simple_inet_of_string inet = Unix.inet_addr_of_string inet 

(**************************************************************)
(* Bind to any port.  This causes the kernel to select an
 * arbitrary open port to bind to.  We then call getsockname
 * to find out which port it was.  
 *)

let bind_any debug sock host =
  if Socket.is_unix then (
    begin 
      try bind sock host 0 with _ ->
	try bind sock host 0 with _ ->
      	  try bind sock host 0 with e -> (
  	    eprintf "HSYS:error:binding socket to port 0 (tried 3 times):%s\n" (Util.error e) ;
  	    eprintf "  (Note: this should not fail because binding to port 0 is\n" ;
  	    eprintf "   a request to bind to any available port.  However, on some\n" ;
  	    eprintf "   platforms (i.e., Linux) this occasionally fails and trying\n" ;
  	    eprintf "   again will often work)\n" ;
	    exit 1
	  )
    end ;
    
    let port =
      try
	let (_,port) = getsockname sock in
	port
      with e ->
	eprintf "HSYS:error:getsockname:%s\n" (Util.error e) ;
	exit 1
    in
    
    port
  ) else (
    (* On win32, getsockname does not work properly. 
     *)
    let rec loop i = 
      if i=0 then (
  	eprintf "HSYS:error:binding socket to random port (tried 10 times)\n";
	exit 1
      ) else (
	let port = 1024 + Random.int 10000 in
	try 
	  bind sock host port; 
	  port 
	with Unix.Unix_error _ -> loop (pred i)
      )
    in
    loop 10 
  )
    
(**************************************************************)

let heap = Socket.heap
let addr_of_obj = Socket.addr_of_obj
let minor_words = Socket.minor_words
let frames = Socket.frames

(**************************************************************)

let read_ch ch =
  let l = ref [] in
  begin 
    try while true do
      let buf = String.create 10000 in
      let len = input ch buf 0 (String.length buf) in
      if len = 0 then 
      	raise End_of_file ;
      l := (String.sub buf 0 len) :: !l
    done with End_of_file -> () 
  end ;
  String.concat "" (List.rev !l)

let open_process cmd env input =
  let ((in_read,out_write,in_err) as handle) = 
    Unix.open_process_full cmd env in
  output_string out_write input ; 
  close_out out_write ;

  let output = read_ch in_read in
  let error = read_ch in_err in
  close_in in_read ;
  close_in in_err ;

  let stat = Unix.close_process_full handle in
  let stat = stat = Unix.WEXITED(0) in
  (stat,output,error)


let background_process cmd env input =
  let ((in_read,out_write,in_err) as handle) = 
    Unix.open_process_full cmd env in
  output_string out_write input ; 
  close_out out_write ;
      
  (* sockets to listen for replies on.
   *)
  (handle,
   Unix.descr_of_in_channel in_read, 
   Unix.descr_of_in_channel in_err)

(**************************************************************)
let set_udp_options sock sock_buf = 
  (* Set the socket to be nonblocking.
   *)

  begin try
    setsockopt sock (Nonblock true)
  with e ->
    log (fun () -> sprintf "warning:setsockopt:Nonblock:%s" (Util.error e))
  end ;
  
  (* Try to disable error ICMP error reporting.
   *)
  begin try
    setsockopt sock (Bsdcompat true)
  with e ->
    log (fun () -> sprintf "warning:setsockopt:Bsdcompat:%s" (Util.error e))
  end ;
  
  (* Try to set the size of the send buffer.
   *)
  begin try
    setsockopt sock (Sendbuf sock_buf)
  with e ->
    log (fun () -> sprintf "warning:setsockopt:Sendbuf(%d):%s" sock_buf (Util.error e))
  end ;
  
  (* Try to set the size of the receive buffer.
   *)
  begin try
    setsockopt sock (Recvbuf sock_buf)
  with e ->
    log (fun () -> sprintf "warning:setsockopt:Recvbuf(%d):%s" sock_buf (Util.error e))
  end ;

  (* Children don't get access to it and make it non-blocking.
   *)
  (*set_close_on_exec sock ;*)
  
  sock


(* Open a datagram socket.
 *)
let udp_socket sock_buf =
  let sock =
    try socket_dgram () with e ->
      eprintf "HSYS:error creating socket:%s, exiting\n" (Util.error e) ;
      exit 1
  in
  log (fun () -> sprintf "udp_socket: sock=%d" (int_of_socket sock)) ;
  set_udp_options sock sock_buf


(* Open a multicast socket
*)
let multicast_socket sock_buf =
  let sock =
    try socket_mcast () with e ->
      eprintf "HSYS:error creating socket:%s, exiting\n" (Util.error e) ;
      exit 1
  in
  log (fun () -> sprintf "multicast_sock: sock=%d" (int_of_socket sock)) ;
  set_udp_options sock sock_buf

(**************************************************************)

