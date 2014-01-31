(**************************************************************)
(* HSYS.MLI *)
(* Author: Mark Hayden, 5/95 *)
(**************************************************************)
open Buf
(**************************************************************)
type debug = string
type port = int
type inet = Unix.inet_addr
type socket = Socket.socket

type timeval = {
  mutable sec10 : int ;
  mutable usec : int
} 

(**************************************************************)
(* The following are essentially the same as the functions of
 * the same name in the Unix library.
 *)

val accept : socket -> socket * inet * port
val bind : socket -> inet -> port -> unit
val close : socket -> unit
val connect : socket -> inet -> port -> unit
val gethost : unit -> inet
val getlocalhost : unit -> inet
val gethostname : unit -> string
val getlogin : unit -> string
val getpeername : socket -> inet * port
val getsockname : socket -> inet * port
val gettimeofday : unit -> timeval
val inet_any : unit -> inet
val inet_of_string : string -> inet
val listen : socket -> int -> unit
val read : socket -> string -> int (*ofs*) -> int (*len*) -> int
val recvfrom : socket -> Buf.t -> Buf.ofs -> Buf.len -> Buf.len * inet * port
val string_of_inet : inet -> string	(* i.e. gethostbyname *)
val string_of_inet_nums : inet -> string
val getenv : string -> string option
val getpid : unit -> int

(**************************************************************)

(* Bind to any port.  Returns port number used.
 *)
val bind_any : debug -> socket -> inet -> port

(**************************************************************)

(* This only returns the host name and none of the
 * domain names (everything up to the first '.'.
 *)
val string_of_inet_short : inet -> string

(**************************************************************)

(* Create a random deering address given an integer 'seed'.
 *)
val deering_addr : int -> inet

(**************************************************************)

(* Same as Unix.gettimeofday, except that a timeval record
 * is passed in and the return value is written there.  See
 * ensemble/socket/socket.mli for an explanation.  
 *)
val gettimeofdaya : timeval -> unit

(**************************************************************)

(* Was Ensemble compiled with ip multicast and does this host
 * support it.
 *)
val has_ip_multicast : unit -> bool

(**************************************************************)

(* Integer value of a socket.  For debugging only.  On Unix,
 * this gives the file descriptor number.  On Windows, this
 * gives the handle number, which is somewhat different from
 * Unix file descriptors.  
 *)
val int_of_socket : socket -> int

(**************************************************************)

(* A variation on fork().  [open_process command input]
 * creates a process of the given name and writes [input] to
 * its standard input.  Returns a tuple, where the first
 * entry specifies if the process exited successful.  The
 * second value is the standard output, and the third value
 * is the standard error.  
 *)

val open_process : string -> string array -> string -> (bool * string * string)

val background_process : string -> string array -> string -> 
  (in_channel * out_channel * in_channel) * socket * socket


(**************************************************************)

(* Handlers that are used for the pollinfo/polling
 * functions.  The first kind of handler specifies a
 * callback to make when there is data ready to read on the
 * socket.  The second kind of handler specifies that the
 * polling function is to recv() data from the socket into
 * its own buffer and pass the data up a central handler
 * (specified in the call to the pollinfo() function).  
 *)
type handler =
  | Handler0 of (unit -> unit)
  | Handler1

(**************************************************************)

(* This is an optimized version of the select function.  You
 * passing in select_info arguments for the read, write, and
 * exception conditions, as with normal select.  However,
 * this function specifies which sockets have data by
 * modifying the boolean values in the array that are
 * associated with the appropriate socket.  
 *)

(* Note: both arrays here should be of the same length.
 *)
type sock_info = ((socket array) * (bool array)) option
type select_info

val select_info : sock_info -> sock_info -> sock_info -> select_info

val select : select_info -> timeval -> int

(* This is the same as select, except that the timeout is
 * always 0.0.  
 *)
val poll : select_info -> int

(**************************************************************)

(* Optimized sending functions.  You first call sendto_info
 * with the socket to send on, and an array of destinations.
 * This returns a preprocessed data structure that can be
 * used later to actually do the sends with.  The sendto and
 * sendtov functions do not check for errors.
 *)
type sendto_info

val tcp_info : socket -> sendto_info
val sendto_info : socket -> (inet * port) array -> sendto_info

val sendto : sendto_info -> Buf.t -> ofs -> len -> unit
val sendtov : sendto_info -> Iovecl.t -> unit
val sendtosv : sendto_info -> Buf.t -> ofs -> len -> Iovecl.t -> unit
val udp_recv_packet : socket -> Buf.t * Iovec.t

val recv : socket -> Buf.t -> ofs -> len -> len

(* In the TCP functions, exceptions are caught, logged, and len0 
 * is returned instead.
*)
val tcp_recv : socket -> Buf.t -> ofs -> len -> len
val tcp_recv_iov : socket -> Iovec.t -> ofs -> len -> len
val tcp_recv_packet : socket -> Buf.t -> ofs -> len -> Iovec.t -> len

val send_p : socket -> Buf.t -> ofs -> len -> len
val sendv_p : socket -> Iovecl.t -> len
val sendsv_p : socket -> Buf.t -> ofs -> len -> Iovecl.t -> len
val sends2v_p : socket -> Buf.t -> Buf.t -> ofs -> len -> Iovecl.t -> len

(**************************************************************)

(* Options passed to setsockopt.
 *)
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

(* Set one of the above options on a socket.
 *)
val setsockopt : socket -> socket_option -> unit

(* Is this a class D address ?
*)
val in_multicast : Unix.inet_addr -> bool
(**************************************************************)

(* Create a datagram (UDP) or a stream (TCP) socket.
 *)
val socket_dgram : unit -> socket
val socket_stream : unit -> socket
val socket_mcast : unit -> socket

(**************************************************************)

(* Check if portions of two strings are equal.
 *)
val substring_eq : Buf.t -> ofs -> Buf.t -> ofs -> len -> bool

(**************************************************************)

(* MD5 support.
 *)
type md5_ctx
val md5_init : unit -> md5_ctx
val md5_init_full : string -> md5_ctx
val md5_update : md5_ctx -> Buf.t -> ofs -> len -> unit
val md5_final : md5_ctx -> Digest.t

val md5_update_iovl : md5_ctx -> Iovecl.t -> unit
(**************************************************************)

(* Create a socket connected to the standard input.
 * See ensemble/socket/stdin.c for an explanation of
 * why this is necessary.
 *)
val stdin : unit -> socket

(**************************************************************)

val heap : unit -> Obj.t array
val addr_of_obj : Obj.t -> string
val minor_words : unit -> int
val frames : unit -> int array array

(**************************************************************)

(* These simply returns the text readable value the
 * inet (XXX.YYY.ZZZ.TTT). Used for safe marshaling routines.
*)
val simple_string_of_inet  : inet -> string 
val simple_inet_of_string  : string -> inet 

(**************************************************************)

(* Do whatever is necessary to create and initialize a UDP
 * socket.
 * 
 * The [int] argument is the size of the socket buffers to
 * ask the kernel to reserve. This can be gotten by using
 * Arge.get Arge.sock_buf. 
 * 
 * This can't be done here, because Hsys is below Arge.
 *)
val udp_socket : int  -> socket
val multicast_socket : int -> socket
(**************************************************************)

