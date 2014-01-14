(**************************************************************)
(* COMM_IMPL.ML *)
(* Authors: Robbert vanRenesse and Mark Hayden, 4/95 *)
(* Ohad Rodeh :                                       *)
(*         C-Memory mangement support          9/2001 *)
(*         Winsock2 support                   11/2001 *)
(**************************************************************)
let name = "SSOCKET"
let failwith s = failwith (name^":"^s)
(**************************************************************)
open Printf
open Natiov_impl
open Socksupp

(**************************************************************)
(* Including stuff from socksupp
*)
let max_msg_size = Socksupp.max_msg_size

(**************************************************************)

external gettimeofday : timeval -> unit 
  = "skt_gettimeofday" "noalloc"

(**************************************************************)

external start_input : unit -> Unix.file_descr 
  = "skt_start_input"

(* See documentation in stdin.c.
 *)
let stdin =
  let stdin = ref None in
  fun () ->
    (*failwith "stdin not supported --- just for debugging";*)
    match !stdin with
      | None ->
	  let s = start_input () in
	  stdin := Some s ;
	  s
      | Some s -> s

(**************************************************************)

external recvfrom : socket -> buf -> ofs -> len -> len * Unix.sockaddr
  = "skt_recvfrom" 

(* On win32, read from the socket representing stdin.
 *)
external read : socket -> buf -> ofs -> len -> len 
  = "skt_recvfrom2" 

(**************************************************************)

type sock_info = (socket array) * (bool array)
type socket_info = (socket array) * (bool array)
type select_info = socket_info * socket_info * socket_info * int

external select : select_info -> timeval -> int 
  = "skt_select" "noalloc"
external poll : select_info -> int 
  = "skt_poll" "noalloc"

(**************************************************************)

external substring_eq : string -> ofs -> string -> ofs -> len -> bool 
  = "skt_substring_eq" "noalloc"

(**************************************************************)

(* HACK!  It's useful to be able to print these out as ints.
 *)
external int_of_file_descr : Unix.file_descr -> int
  = "skt_int_of_file_descr"


let int_of_socket s =
  int_of_file_descr s

let sihelp (socks,ret) =
  let max = ref 0 in
  Array.iter (fun sock ->
    let sock = int_of_socket sock in
    if sock > !max then max := sock
  ) socks ;
  ((socks,ret),!max)

let select_info a b c = 
  let (a,am) = sihelp a in
  let (b,bm) = sihelp b in
  let (c,cm) = sihelp c in
  let max = succ (max am (max bm cm)) in
  (a,b,c,max)

(**************************************************************)
external socket_mcast : 
  Unix.socket_domain -> Unix.socket_type -> int -> Unix.file_descr
    = "skt_socket_mcast"

external socket : 
  Unix.socket_domain -> Unix.socket_type -> int -> Unix.file_descr
  = "skt_socket"

external connect : Unix.file_descr -> Unix.sockaddr -> unit
  = "skt_connect"

external bind : Unix.file_descr -> Unix.sockaddr -> unit
  = "skt_bind"

external close : Unix.file_descr -> unit
  = "skt_close"

external listen : Unix.file_descr -> int -> unit
  = "skt_listen"

external accept : Unix.file_descr -> Unix.file_descr * Unix.sockaddr 
  = "skt_accept"

(**************************************************************)
(* Optimized socket operations.
 * - unsafe
 * - no exceptions
 * - "noalloc" is enabled
 * - socket, flags, and address are preprocessed
 * - no interrupts accepted
 * - no retransmission attempts on interrupts
 *)

(* Pre-processed information on a socket.
*)
type sendto_info

(* A context structure.
*)
type ctx 

type digest = string (* A 16-byte string *)

external create_ctx : unit -> ctx 
  = "skt_Val_create_ctx"

external sendto_info : 
  Unix.file_descr -> Unix.sockaddr array -> sendto_info
    = "skt_Val_create_sendto_info"

external sendto : sendto_info -> buf -> ofs -> len -> unit
  = "skt_sendto" "noalloc"

(* These two functions are used to send UDP packets. 
 * They add a header describing the size of the ML/Iovec size 
 * of the data. 
 * The format is: [ml_len] [usr_len]
 * 
 * Corresponding code exists in udp_recv_packet. 
*)
external sendtov : sendto_info -> Natiov_impl.t array -> unit 
  = "skt_sendtov" "noalloc"

external sendtosv : sendto_info -> buf -> ofs -> len -> Natiov_impl.t array -> unit
  = "skt_sendtosv" "noalloc"

external skt_udp_recv_packet : 
  socket -> 
  Natiov_impl.cbuf -> (* pre-allocated user-space memory *)
  string * Natiov_impl.raw 
    = "skt_udp_recv_packet"

let udp_recv_packet sock = 
  (* check that there is enough memory, and return a cbuf for 
   * udp_recv.
   *)
  let cbuf = Natiov_impl.check_pre_alloc () in
  let buf,iov = skt_udp_recv_packet sock cbuf in

  (* Update our position in the cached memory chunk.
   * On WIN32, since we don't have MSG_PEEK, we read everything into
   * user buffers. Hence, we need to copy the ML header into a string, 
   * and advance past 8byte pre-header + ML header + user-buffer.
   *)
  if iov.len >|| 0 then (
    Natiov_impl.advance (String.length buf + iov.len + 8);
    buf, Natiov_impl.t_of_iovec s.chunk.base iov
  ) else
    buf, empty
  
    
(**************************************************************)
(* Receive messages into preallocated buffers. Exceptions are
 * thrown. 
 *)

external recv : socket -> buf -> ofs -> len -> len 
  = "skt_recv" "noalloc"
external recv_iov : socket -> Natiov_impl.raw -> ofs -> len -> len 
  = "skt_recv_iov" "noalloc"
external tcp_recv_packet : socket -> string -> ofs -> len -> Natiov_impl.raw -> len
  = "skt_tcp_recv_packet"

let recv_iov sock t ofs len = recv_iov sock t.Natiov_impl.iov ofs len

let tcp_recv_packet sock buf ofs len t = 
  tcp_recv_packet sock buf  ofs len t.Natiov_impl.iov

(**************************************************************)
(* Operations for sending messages point-to-point. 
 * 1) Exceptions are thrown
 * 2) nothing is allocated
 * 3) The length sent is returned
 * 4) The sendto parameter must contain a single destination. 
 *)
    
external send_p : socket -> buf -> ofs -> len -> len
  = "skt_send_p" "noalloc"

external sendv_p : socket -> Natiov_impl.t array -> len
  = "skt_sendv_p" "noalloc"

external sendsv_p : socket -> buf -> ofs -> len -> Natiov_impl.t array -> len
  = "skt_sendsv_p" "noalloc"

external sends2v_p : socket-> buf -> buf -> ofs -> len -> Natiov_impl.t array -> len
  = "skt_sends2v_p_bytecode" "skt_sends2v_p_native" "noalloc"

(**************************************************************)

external has_ip_multicast : unit -> bool 
  = "skt_has_ip_multicast" 
external in_multicast : Unix.inet_addr -> bool 
  = "skt_in_multicast" 
external setsockopt_ttl : socket -> int -> unit 
  = "skt_setsockopt_ttl" 
external setsockopt_loop : socket -> bool -> unit 
  = "skt_setsockopt_loop" 
external setsockopt_join : socket -> Unix.inet_addr -> unit 
  = "skt_setsockopt_join" 
external setsockopt_leave : socket -> Unix.inet_addr -> unit 
  = "skt_setsockopt_leave" 
external setsockopt_sendbuf : socket -> int -> unit
  = "skt_setsockopt_sendbuf"
external setsockopt_recvbuf : socket -> int -> unit
  = "skt_setsockopt_recvbuf"
external setsockopt_nonblock : socket -> bool -> unit
  = "skt_setsockopt_nonblock"
external setsockopt_bsdcompat : socket -> bool -> unit
  = "skt_setsockopt_bsdcompat"
external setsockopt_reuse : socket -> bool -> unit
  = "skt_setsockopt_reuse"
(**************************************************************)
external win_version : unit -> string = "win_version"

let os_type_and_version () = 
  let v = ref None in
  match !v with 
      Some x -> x
    | None -> 
	let ver = 
	  if is_unix then 
	    failwith "Sanity: using win32 socket library on a non-win32 platform"
	  else (
	    let version = 
	      match win_version () with
		  "NT3.5" -> Win_NT_3_5
		| "NT4.0" -> Win_NT_4
		| "2000" -> Win_2000
		| "95/98" -> Win_95_98
		| "3.11" -> Win_3_11
		| _ -> failwith "Sanity: win_version returned bad value"
	    in
	    OS_Win version
	  ) in
	v := Some ver;
	ver

(**************************************************************)

