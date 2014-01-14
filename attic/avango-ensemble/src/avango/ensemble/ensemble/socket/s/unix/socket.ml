(**************************************************************)
(* SOCKET.ML *)
(* Authors: Robbert vanRenesse and Mark Hayden, 4/95 *)
(**************************************************************)
let name = "SSOCKET"
let failwith s = failwith (name^":"^s)
(**************************************************************)
open Printf
open Socksupp
(**************************************************************)
type buf = string
type ofs = int
type len = int

(**************************************************************)
(* Memory management functions. 
*)

(* Debugging
*)
let verbose = ref false

let set_verbose flag =
  printf "Socket verbose\n"; Pervasives.flush Pervasives.stdout;
  verbose := flag

let log f = 
  if !verbose then (
    let s = f () in
    print_string s; print_string "\n"; flush stdout;
  ) else ()

(**************************************************************)
(* Include the native iovector support code
 *)
module Basic_iov = Natiov_imp
open Basic_iov

(* Include common code for all platforms
*)
let heap = Common_impl.heap
let addr_of_obj = Common_impl.addr_of_obj
let minor_words = Common.minor_words
let frames = Common.frames

(* MD5 hash functions. We need to have it work on Iovecs as
 * well as regular strings. 
*)
type md5_ctx = Common_impl.md5_ctx

let md5_init = Common_impl.md5_init
let md5_init_full = Common_impl.md5_init_full
let md5_final = Common_impl.md5_final
let md5_update_iov = Common_impl.md5_update_iov
let sihelp = Common_impl.sihelp
let select_info = Common_impl.select_info

(**************************************************************)

type timeval = {
  mutable sec10 : int ;
  mutable usec : int
} 

external gettimeofday : timeval -> unit 
  = "skt_gettimeofday" "noalloc"

(**************************************************************)

type socket = Unix.file_descr

(**************************************************************)

external start_input : unit -> Unix.file_descr 
  = "skt_start_input"

(* See documentation in stdin.c.
 *)
let stdin =
  let stdin = ref None in
  if is_unix then (
    fun () -> Unix.stdin 
  ) else (
    fun () ->
      (*failwith "stdin not supported --- just for debugging";*)
      match !stdin with
	| None ->
	    let s = start_input () in
	    stdin := Some s ;
	    s
	| Some s -> s
  )

(**************************************************************)

external heap : unit -> Obj.t array 
  = "skt_heap"
external addr_of_obj : Obj.t -> string 
  = "skt_addr_of_obj"
external minor_words : unit -> int 
  = "skt_minor_words" "noalloc"
external frames : unit -> int array array 
  = "skt_frame_descriptors"

(**************************************************************)
external skt_recvfrom : socket -> buf -> ofs -> len -> len * Unix.sockaddr
  = "skt_recvfrom" 

let recvfrom s b o l = 
  if is_unix then Unix.recvfrom s b o l []
  else skt_recvfrom s b o l 


(* READ: Depending on the system, call either read or recv.
 * This is only used for stdin (where on Nt it is a socket).
 *)
external skt_recvfrom2 : socket -> buf -> ofs -> len -> len 
  = "skt_recvfrom2" 

let read =
  if is_unix then 
    fun s b o l -> Unix.read s b o l
  else
    fun s b o l -> skt_recvfrom2 s b o l

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

(**************************************************************)
external skt_socket_mcast : 
  Unix.socket_domain -> Unix.socket_type -> int -> Unix.file_descr
    = "skt_socket_mcast"

external skt_socket : 
  Unix.socket_domain -> Unix.socket_type -> int -> Unix.file_descr
  = "skt_socket"

external skt_connect : Unix.file_descr -> Unix.sockaddr -> unit
  = "skt_connect"

external skt_bind : Unix.file_descr -> Unix.sockaddr -> unit
  = "skt_bind"

external skt_close : Unix.file_descr -> unit
  = "skt_close"

external skt_listen : Unix.file_descr -> int -> unit
  = "skt_listen"

external skt_accept : Unix.file_descr -> Unix.file_descr * Unix.sockaddr 
  = "skt_accept"

let socket dom typ proto = 
  let s = 
    if is_unix then Unix.socket dom typ proto 
    else skt_socket dom typ proto 
  in
  log (fun () -> sprintf "sock=%d\n" (int_of_socket s));
  s

let socket_mcast = 
  if is_unix then Unix.socket
  else skt_socket_mcast

let connect = 
  if is_unix then Unix.connect
  else skt_connect

let bind = 
  if is_unix then Unix.bind
  else skt_bind

(* will work only on sockets on WIN32.
*)
let close = 
  if is_unix then Unix.close
  else skt_close

let listen = 
  if is_unix then Unix.listen
  else skt_listen

let accept = 
  if is_unix then Unix.accept
  else skt_accept

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
external sendtov : sendto_info -> Basic_iov.t array -> unit 
  = "skt_sendtov" "noalloc"

external sendtosv : sendto_info -> buf -> ofs -> len -> Basic_iov.t array -> unit
  = "skt_sendtosv" "noalloc"

external skt_udp_recv_packet : 
  socket -> 
  Basic_iov.cbuf -> (* pre-allocated user-space memory *)
  string * Basic_iov.raw 
    = "skt_udp_recv_packet"

let udp_recv_packet sock = 
  (* check that there is enough memory, and return a cbuf for 
   * udp_recv.
   *)
  let cbuf = Basic_iov.check_pre_alloc () in
  let buf,iov = skt_udp_recv_packet sock cbuf in

  (* Update our position in the cached memory chunk.
   * On WIN32, since we don't have MSG_PEEK, we read everything into
   * user buffers. Hence, we need to copy the ML header into a string, 
   * and advance past 8byte pre-header + ML header + user-buffer.
   *)
  if iov.len >|| 0 then (
    if is_unix then 
      Basic_iov.advance iov.len
    else
      Basic_iov.advance (String.length buf + iov.len + 8);
    buf, Basic_iov.t_of_iovec s.chunk.base iov
  ) else
    buf, empty
  
    
(**************************************************************)
(* Receive messages into preallocated buffers. Exceptions are
 * thrown. 
 *)

external recv : socket -> buf -> ofs -> len -> len 
  = "skt_recv" "noalloc"
external recv_iov : socket -> Basic_iov.raw -> ofs -> len -> len 
  = "skt_recv_iov" "noalloc"
external tcp_recv_packet : socket -> string -> ofs -> len -> Basic_iov.raw -> len
  = "skt_tcp_recv_packet"

let recv_iov sock t ofs len = recv_iov sock t.Basic_iov.iov ofs len

let tcp_recv_packet sock buf ofs len t = 
  tcp_recv_packet sock buf  ofs len t.Basic_iov.iov

(**************************************************************)
(* Operations for sending messages point-to-point. 
 * 1) Exceptions are thrown
 * 2) nothing is allocated
 * 3) The length sent is returned
 * 4) The sendto parameter must contain a single destination. 
 *)
    
external send_p : socket -> buf -> ofs -> len -> len
  = "skt_send_p" "noalloc"

external sendv_p : socket -> Basic_iov.t array -> len
  = "skt_sendv_p" "noalloc"

external sendsv_p : socket -> buf -> ofs -> len -> Basic_iov.t array -> len
  = "skt_sendsv_p" "noalloc"

external sends2v_p : socket-> buf -> buf -> ofs -> len -> Basic_iov.t array -> len
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
let setsockopt_reuse sock onoff = 
  if is_unix then Unix.setsockopt sock Unix.SO_REUSEADDR onoff
  else setsockopt_reuse sock onoff
(**************************************************************)
(* MD5 hash functions. We need to have it work on Iovecs as
 * well as regular strings. 
*)
type md5_ctx = string

external md5_ctx_length : unit -> int
  = "skt_md5_context_length" "noalloc"
external md5_init : md5_ctx -> unit
  = "skt_md5_init" "noalloc"
external md5_init_full : md5_ctx -> string -> unit
  = "skt_md5_init_full" "noalloc"
external md5_update : md5_ctx -> buf -> ofs -> len -> unit
  = "skt_md5_update" "noalloc"
external md5_update_iov : md5_ctx -> Basic_iov.raw -> unit
  = "skt_md5_update_iov" "noalloc"
external md5_final : md5_ctx -> Digest.t -> unit
  = "skt_md5_final" "noalloc"

let md5_init () =
  let ret = String.create (md5_ctx_length ()) in
  md5_init ret ;
  ret

let md5_init_full init_key =
  if String.length init_key <> 16 then 
    raise (Invalid_argument "md5_init_full: initial key value is not of length 16");
  let ret = String.create (md5_ctx_length ()) in
  md5_init_full ret init_key;
  ret


let md5_final ctx =
  let dig = String.create 16 in
  md5_final ctx dig ;
  dig

let md5_update_iov ctx iov_t = 
  md5_update_iov ctx iov_t.Basic_iov.iov

(**************************************************************)

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
type win = 
    Win_3_11
  | Win_95_98
  | Win_NT_3_5
  | Win_NT_4
  | Win_2000

type os_t_v = 
    OS_Unix
  | OS_Win of win

external win_version : unit -> string = "win_version"

let os_type_and_version () = 
  let v = ref None in
  match !v with 
      Some x -> x
    | None -> 
	let ver = 
	  if is_unix then OS_Unix
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
let is_unix = is_unix

(**************************************************************)

