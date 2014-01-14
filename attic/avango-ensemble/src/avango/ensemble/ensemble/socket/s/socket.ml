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
type socket = Unix.file_descr
type timeval = Socksupp.timeval = {
  mutable sec10 : int ;
  mutable usec : int
} 

type win = Socksupp.win = 
    Win_3_11
  | Win_95_98
  | Win_NT_3_5
  | Win_NT_4
  | Win_2000

type os_t_v = Socksupp.os_t_v = 
    OS_Unix
  | OS_Win of win


let max_msg_size = Socksupp.max_msg_size
let is_unix = Socksupp.is_unix
(**************************************************************)

(* Debugging
*)
let verbose = ref false

let set_verbose flag =
  printf "Socket verbose\n"; Pervasives.flush Pervasives.stdout;
  verbose := flag

let log f = 
  if !verbose then (
    let s = f () in
    print_string s; flush stdout;
  ) else ()

let log_nl f = 
  if !verbose then (
    let s = f () in
    print_string s; print_string "\n"; flush stdout;
  ) else ()

let is_unix = Socksupp.is_unix

(**************************************************************)
(* Include the native iovector support code
 *)
module Basic_iov = Natiov_impl
open Basic_iov

(**************************************************************)
module Opt = struct
  (* Include common code for all platforms
   *)
  let heap = Common_impl.heap
  let addr_of_obj = Common_impl.addr_of_obj
  let minor_words = Common_impl.minor_words
  let frames = Common_impl.frames
    
  (* MD5 hash functions. We need to have it work on Iovecs as
   * well as regular strings. 
   *)
  type md5_ctx = Common_impl.md5_ctx
      
  let md5_init = Common_impl.md5_init
  let md5_init_full = Common_impl.md5_init_full
  let md5_final = Common_impl.md5_final
  let md5_update_iov = Common_impl.md5_update_iov
  let md5_update = Common_impl.md5_update
    
    
  (* Including platform dependent stuff
   *)
  let gettimeofday = Comm_impl.gettimeofday
    
  let socket_mcast = Comm_impl.socket_mcast
  let socket = Comm_impl.socket
  let connect = Comm_impl.connect
  let bind = Comm_impl.bind 
  let close = Comm_impl.close 
  let listen = Comm_impl.listen 
  let accept = Comm_impl.accept 
    
  let stdin = Comm_impl.stdin
  let read  = Comm_impl.read 
    
  let int_of_socket = Comm_impl.int_of_socket
    
  type sock_info = Comm_impl.sock_info
  type select_info = Comm_impl.select_info
      
  let select_info = Comm_impl.select_info
  let select  = Comm_impl.select 
  let poll  = Comm_impl.poll 
    
  let substring_eq  = Comm_impl.substring_eq
    
  type sendto_info = Comm_impl.sendto_info
  let sendto_info  = Comm_impl.sendto_info 
    
  let sendto  = Comm_impl.sendto 
  let sendtov  = Comm_impl.sendtov 
  let sendtosv  = Comm_impl.sendtosv 
    
  let recvfrom  = Comm_impl.recvfrom 
  let udp_recv_packet  = Comm_impl.udp_recv_packet 
    
  let recv  = Comm_impl.recv 
  let recv_iov  = Comm_impl.recv_iov 
  let tcp_recv_packet  = Comm_impl.tcp_recv_packet 
    
  let send_p  = Comm_impl.send_p 
  let sendv_p  = Comm_impl.sendv_p 
  let sendsv_p  = Comm_impl.sendsv_p 
  let sends2v_p  = Comm_impl.sends2v_p 
    
  let has_ip_multicast  = Comm_impl.has_ip_multicast 
  let in_multicast  = Comm_impl.in_multicast 
  let setsockopt_ttl = Comm_impl.setsockopt_ttl 
  let setsockopt_loop = Comm_impl.setsockopt_loop 
  let setsockopt_join = Comm_impl.setsockopt_join 
  let setsockopt_leave = Comm_impl.setsockopt_leave 
    
  let setsockopt_sendbuf = Comm_impl.setsockopt_sendbuf 
  let setsockopt_recvbuf = Comm_impl.setsockopt_recvbuf 
  let setsockopt_nonblock = Comm_impl.setsockopt_nonblock 
  let setsockopt_bsdcompat = Comm_impl.setsockopt_bsdcompat 
  let setsockopt_reuse = Comm_impl.setsockopt_reuse 
    
  let os_type_and_version = Comm_impl.os_type_and_version
end

(**************************************************************)

module Debug = struct
  (* Include common code for all platforms
   *)
  let heap = Common_impl.heap
  let addr_of_obj = Common_impl.addr_of_obj
  let minor_words = Common_impl.minor_words
  let frames = Common_impl.frames
    
  (* MD5 hash functions. We need to have it work on Iovecs as
   * well as regular strings. 
   *)
  type md5_ctx = Common_impl.md5_ctx
      
  let md5_init () = 
    log (fun () -> "md5_init(");
    let ctx = Common_impl.md5_init () in
    log_nl (fun () -> "md5_init(");
    ctx

  let md5_init_full s = 
    log (fun () -> "md5_init_full(");
    let ctx = Common_impl.md5_init_full s in
    log_nl (fun () -> ")");
    ctx
    
  let md5_final = 
    log (fun () -> "md5_final(");
    let digest = Common_impl.md5_final in
    log_nl (fun () -> ")");
    digest

  let md5_update_iov ctx iov = 
    log (fun () -> "md5_update_iov(");
    Common_impl.md5_update_iov ctx iov;
    log_nl (fun () -> ")")

  let md5_update ctx buf ofs len = 
    log (fun () -> "md5_update(");
    Common_impl.md5_update ctx buf ofs len;
    log_nl (fun () -> ")")
    
  (* Including platform dependent stuff
   *)
  let gettimeofday tm = 
    log (fun () -> "gettimeofday(");
    Comm_impl.gettimeofday tm;
    log_nl (fun () -> ")")
    
  let socket_mcast dom typ flags = 
    log (fun () -> "socket_mcast(");
    let fd = Comm_impl.socket_mcast dom typ flags in
    log_nl (fun () -> ")");
    fd

  let socket dom typ flags = 
    log (fun () -> "socket(");
    let fd = Comm_impl.socket dom typ flags in
    log_nl (fun () -> ")");
    fd

  let connect fd addr = 
    log (fun () -> "conect(");
    Comm_impl.connect fd addr;
    log_nl (fun () -> ")")

  let bind fd addr = 
    log (fun () -> "bind(");
    Comm_impl.bind fd addr;
    log_nl (fun () -> ")")

  let close fd = 
    log (fun () -> "close(");
    Comm_impl.close fd;
    log (fun () -> ")")

  let listen fd num = 
    log (fun () -> "listen(");
    Comm_impl.listen fd num;
    log_nl (fun () -> ")")
 
  let accept fd = 
    log (fun () -> "accept(");
    let rep = Comm_impl.accept fd in
    log_nl (fun () -> ")");
    rep

  let stdin () = 
    log (fun () -> "stdin(");
    let fd = Comm_impl.stdin () in
    log_nl (fun () -> ")");
    fd

  let read sock buf ofs len = 
    log (fun () -> "read(");
    let len = Comm_impl.read sock buf ofs len in
    log_nl (fun () -> ")");
    len

    
  let int_of_socket sock = 
    log (fun () -> "int_of_sock(");
    let i = Comm_impl.int_of_socket sock in
    log (fun () -> ")");
    i
    
  type sock_info = Comm_impl.sock_info
  type select_info = Comm_impl.select_info
      
  let select_info info1 info2 info3 = 
    log (fun () -> "select_info(");
    let rep = Comm_impl.select_info info1 info2 info3 in
    log_nl (fun () -> ")");
    rep

  let select info timeval = 
    log (fun () -> "select(");
    let i = Comm_impl.select info timeval in
    log_nl (fun () -> ")");
    i

  let poll info = 
    log (fun () -> "poll(");
    let rep = Comm_impl.poll info in
    log_nl (fun () -> ")");
    rep
    
  let substring_eq s1 ofs1 s2 ofs2 len = 
    log (fun () -> "substring_eq(");
    let rep = Comm_impl.substring_eq s1 ofs1 s2 ofs2 len in
    log_nl (fun () -> ")");
    rep
    
  type sendto_info = Comm_impl.sendto_info
  let sendto_info sock addr_a= 
    log (fun () -> "sendto_info(");
    let info = Comm_impl.sendto_info sock addr_a in
    log_nl (fun () -> ")");
    info
    
  let sendto info buf ofs len = 
    log (fun () -> "sendto(");
    Comm_impl.sendto info buf ofs len;
    log_nl (fun () -> ")")

  let sendtov info iov_a = 
    log (fun () -> "sendtov(");
    Comm_impl.sendtov info iov_a;
    log_nl (fun () -> ")")

  let sendtosv info buf ofs len iov_a = 
    log (fun () -> "sendtosv(");
    Comm_impl.sendtosv info buf ofs len iov_a ;
    log_nl (fun () -> ")")
    
  let recvfrom sock buf ofs len = 
    log (fun () -> "recvfrom(");
    let rep = Comm_impl.recvfrom sock buf ofs len in
    log_nl (fun () -> ")");
    rep

  let udp_recv_packet sock= 
    log (fun () -> "udp_recv_packet(");
    let rep = Comm_impl.udp_recv_packet sock in
    log (fun () -> ")");
    rep
    
  let recv sock buf ofs len = 
    log (fun () -> "recv(");
    let len = Comm_impl.recv sock buf ofs len in
    log_nl (fun () -> ")");
    len


  let recv_iov sock iov ofs len = 
    log (fun () -> "recv_iov(");
    let len = Comm_impl.recv_iov sock iov ofs len in
    log_nl (fun () -> ")");
    len

  let tcp_recv_packet sock str ofs len iov = 
    log (fun () -> "tcp_recv_packet(");
    let len = Comm_impl.tcp_recv_packet sock str ofs len iov in
    log_nl (fun () -> ")");
    len
    
  let send_p sock buf ofs len= 
    log (fun () -> "send_p(");
    let len = Comm_impl.send_p sock buf ofs len in
    log_nl (fun () -> ")");
    len

  let sendv_p sock iov = 
    log (fun () -> "sendv_p");
    let len = Comm_impl.sendv_p sock iov in
    log_nl (fun () -> ")");
    len

  let sendsv_p sock buf ofs len iov_a = 
    log (fun () -> "sendsv_p");
    let len = Comm_impl.sendsv_p sock buf ofs len iov_a in
    log_nl (fun () -> ")");
    len

  let sends2v_p sock buf buf ofs len iov_a= 
    log (fun () -> "sends2v_p(");
    let len = Comm_impl.sends2v_p sock buf buf ofs len iov_a in
    log_nl (fun () -> ")");
    len
    
  let has_ip_multicast  = Comm_impl.has_ip_multicast 
  let in_multicast  = Comm_impl.in_multicast 
  let setsockopt_ttl = Comm_impl.setsockopt_ttl 
  let setsockopt_loop = Comm_impl.setsockopt_loop 
  let setsockopt_join = Comm_impl.setsockopt_join 
  let setsockopt_leave = Comm_impl.setsockopt_leave 
    
  let setsockopt_sendbuf = Comm_impl.setsockopt_sendbuf 
  let setsockopt_recvbuf = Comm_impl.setsockopt_recvbuf 
  let setsockopt_nonblock = Comm_impl.setsockopt_nonblock 
  let setsockopt_bsdcompat = Comm_impl.setsockopt_bsdcompat 
  let setsockopt_reuse = Comm_impl.setsockopt_reuse 
    
  let os_type_and_version = Comm_impl.os_type_and_version
end

(**************************************************************)

module Export = Opt
  
(* Include common code for all platforms
 *)
let heap = Export.heap
let addr_of_obj = Export.addr_of_obj
let minor_words = Export.minor_words
let frames = Export.frames
  
(* MD5 hash functions. We need to have it work on Iovecs as
 * well as regular strings. 
 *)
type md5_ctx = Export.md5_ctx
    
let md5_init = Export.md5_init
let md5_init_full = Export.md5_init_full
let md5_final = Export.md5_final
let md5_update_iov = Export.md5_update_iov
let md5_update = Export.md5_update
  
  
(* Including platform dependent stuff
 *)
let gettimeofday = Export.gettimeofday
  
let socket_mcast = Export.socket_mcast
let socket = Export.socket
let connect = Export.connect
let bind = Export.bind 
let close = Export.close 
let listen = Export.listen 
let accept = Export.accept 
  
let stdin = Export.stdin
let read  = Export.read 
  
let int_of_socket = Export.int_of_socket
  
type sock_info = Export.sock_info
type select_info = Export.select_info
    
let select_info = Export.select_info
let select  = Export.select 
let poll  = Export.poll 
  
let substring_eq  = Export.substring_eq
  
type sendto_info = Export.sendto_info
let sendto_info  = Export.sendto_info 
  
let sendto  = Export.sendto 
let sendtov  = Export.sendtov 
let sendtosv  = Export.sendtosv 
  
let recvfrom  = Export.recvfrom 
let udp_recv_packet  = Export.udp_recv_packet 
  
let recv  = Export.recv 
let recv_iov  = Export.recv_iov 
let tcp_recv_packet  = Export.tcp_recv_packet 
  
let send_p  = Export.send_p 
let sendv_p  = Export.sendv_p 
let sendsv_p  = Export.sendsv_p 
let sends2v_p  = Export.sends2v_p 
  
let has_ip_multicast  = Export.has_ip_multicast 
let in_multicast  = Export.in_multicast 
let setsockopt_ttl = Export.setsockopt_ttl 
let setsockopt_loop = Export.setsockopt_loop 
let setsockopt_join = Export.setsockopt_join 
let setsockopt_leave = Export.setsockopt_leave 
  
let setsockopt_sendbuf = Export.setsockopt_sendbuf 
let setsockopt_recvbuf = Export.setsockopt_recvbuf 
let setsockopt_nonblock = Export.setsockopt_nonblock 
let setsockopt_bsdcompat = Export.setsockopt_bsdcompat 
let setsockopt_reuse = Export.setsockopt_reuse 
  
let os_type_and_version = Export.os_type_and_version



