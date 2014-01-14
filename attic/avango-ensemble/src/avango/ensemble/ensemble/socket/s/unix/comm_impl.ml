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
let is_unix = Socksupp.is_unix
let max_msg_size = Socksupp.max_msg_size

(**************************************************************)

external gettimeofday : timeval -> unit 
  = "skt_gettimeofday" "noalloc"

(**************************************************************)

(* For compatibility with win32
*)
let stdin () = Unix.stdin 

(**************************************************************)
external skt_recvfrom : socket -> buf -> ofs -> len -> len * Unix.sockaddr
  = "skt_recvfrom" 

(* For compatibility with win32
*)
let recvfrom s b o l = Unix.recvfrom s b o l []

(* READ: Depending on the system, call either read or recv.
 * This is only used for stdin (where on Nt it is a socket).
 *)
external skt_recvfrom2 : socket -> buf -> ofs -> len -> len 
  = "skt_recvfrom2" 

let read s b o l = Unix.read s b o l

(**************************************************************)

(* HACK!  It's useful to be able to print these out as ints.
 *)
external int_of_file_descr : Unix.file_descr -> int
  = "skt_int_of_file_descr"

let int_of_socket s =
  int_of_file_descr s

(**************************************************************)

type sock_info = (socket array) * (bool array)
type socket_info = (socket array) * (bool array)
type select_info = socket_info * socket_info * socket_info * int

external select : select_info -> timeval -> int 
  = "skt_select" "noalloc"
external poll : select_info -> int 
  = "skt_poll" "noalloc"

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

external substring_eq : string -> ofs -> string -> ofs -> len -> bool 
  = "skt_substring_eq" "noalloc"

(**************************************************************)
let socket dom typ proto = 
  let s = Unix.socket dom typ proto in
  log (fun () -> sprintf "sock=%d\n" (int_of_socket s));
  s

let socket_mcast = Unix.socket
let connect = Unix.connect
let bind = Unix.bind
let close = Unix.close
let listen = Unix.listen
let accept =  Unix.accept

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
    Natiov_impl.advance iov.len;
    buf, Natiov_impl.t_of_iovec s.chunk.base iov
  ) else (
    buf, empty
  )
    
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

let setsockopt_sendbuf s size = Unix.setsockopt_int s Unix.SO_SNDBUF size 
let setsockopt_recvbuf s size = Unix.setsockopt_int s Unix.SO_RCVBUF size 
let setsockopt_nonblock s b =
  if b then Unix.set_nonblock s 
  else Unix.clear_nonblock s
let setsockopt_reuse sock onoff = Unix.setsockopt sock Unix.SO_REUSEADDR onoff

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
external setsockopt_bsdcompat : socket -> bool -> unit
  = "skt_setsockopt_bsdcompat"

(**************************************************************)

let os_type_and_version () = 
  let v = ref None in
  match !v with 
      Some x -> x
    | None -> 
	let ver = 
	  if is_unix then OS_Unix
	  else failwith "Sanity: using Unix library on a non-unix platform"
	in
	v := Some ver;
	ver

(**************************************************************)

