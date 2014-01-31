(**************************************************************)
(* MCREDIT.ML *)
(* Author: Mark Hayden, Zhen Xiao, 3/97 *)
(**************************************************************)
open Trans
open Util
(**************************************************************)
let name = Trace.file "MCREDIT"
let failwith = Trace.make_failwith name
(**************************************************************)

type credit = int
(*
module type S = sig
  type t
  val create : rank -> nmembers -> credit -> credit array -> credit array -> t
  val got_credit : t -> rank -> unit
  val got_msg : t -> rank -> int -> credit
  val set_credit : t -> rank -> int -> unit
  val fail : t -> rank -> unit
  val check : t -> bool 
  val left : t -> credit
  val take : t -> credit -> t
  val clear : t -> unit
  val to_string_list : t -> string list
end
*)
(*module Fast : S = struct*)
type t = {
    rank : rank ;
    nmembers : nmembers ;
    ack_thresh : credit ;
    send_credit : credit array ;
    recv_credit : credit array ;
    mutable msg_len : int ;
    ack_que : int array ;
    mutable waitfor : rank ;
    failed : bool array ;
    mutable all_failed : bool
  } 

(* Note: we require that initially send_credit satisfies
 * send_credit.(i) < send_credit.(i+1) for i = 0 to n-2
 *)
let init_check rank ack_thresh send_credit =
  let ok = ref true in
  let nmembers = Array.length send_credit in
(* Problem here with skipping my own entry.

  for i = 0 to nmembers - 2 do
    if i <> rank 
    && send_credit.(i) >= send_credit.(succ i) 
    then (
      eprintf "MCREDIT:send_credit=%s, rank=%d\n" 
        (string_of_int_array send_credit) rank ;
      failwith "send_credit not increasing"
    )
  done ;
*)
  if ack_thresh < send_credit.(nmembers-1) - send_credit.(0) then
    failwith "ack threshhold too small"

let create rank nmembers ack_thresh send_credit recv_credit = 
  let send_credit = Array.copy send_credit in
  let recv_credit = Array.copy recv_credit in
  init_check rank ack_thresh send_credit ;
  let waitfor =
    if nmembers = 1 || rank <> 0 then 0 else 1
  in {
    rank = rank ;
    nmembers = nmembers ;
    ack_thresh = ack_thresh ;
    send_credit = send_credit ;
    recv_credit = recv_credit ;
    msg_len = 0 ;
    ack_que = Array.create nmembers 0 ;
    waitfor = waitfor ;
    failed = Array.create nmembers false ;
    all_failed = (nmembers = 1)
  } 

let wait_next m =
  let init = m.waitfor in
  m.waitfor <- (succ m.waitfor) mod m.nmembers ;
  while m.rank =| m.waitfor || m.failed.(m.waitfor) do
    m.waitfor <- (succ m.waitfor) mod m.nmembers
  done

(* Note: wait_next would go into indefinite loop if all members in
   the group have failed.
 *)
let increase m =
  if not m.all_failed then (
(*
    failwith "increase:but all members are failed" ;
*)
    
    wait_next m ;
    if m.waitfor < 0 || m.waitfor >= m.nmembers then
      failwith sanity ;
    
    while m.ack_que.(m.waitfor) > 0 do
      array_decr m.ack_que m.waitfor ;
      array_add m.send_credit m.waitfor m.ack_thresh ;
      wait_next m
    done
  )

(* Note: we require credit > send_credit.(n-1) - send_credit.(0) for
   the initial values of send_credit. This way after any member gets a
   credit message, its credit becomes the greatest in the group.
 *)
let got_credit m origin =
  if m.failed.(origin) then 
    failwith "got_credit from failed member" ;
  if m.waitfor = origin then (
    array_add m.send_credit m.waitfor m.ack_thresh ;
    increase m
  ) else (
    array_incr m.ack_que origin
  )
	  

let got_msg m origin msg_len =
(*
  eprintf "fast: m.recv_credit.(%d)=%d\n" origin m.recv_credit.(origin) ;
*)
  array_add m.recv_credit origin msg_len ;
(*
  eprintf "msg_len=%d  m.recv_credit.(%d)=%d\n" msg_len origin m.recv_credit.(origin) ;
  flush stdout ;
*)
  m.recv_credit.(origin)

let set_credit m origin remainder =
  m.recv_credit.(origin) <- remainder

let fail m rank =
  if rank = m.rank then
    failwith "I'm dead?" ;
  m.failed.(rank) <- true ;
  m.send_credit.(rank) <- 0 ;
  m.recv_credit.(rank) <- 0 ;
  m.ack_que.(rank) <- 0 ;

  let alive = ref false in
  for i = 0 to pred m.nmembers do
    if i <> m.rank && (not m.failed.(i)) then
      alive := true
  done ;
  m.all_failed <- not !alive ;

  if (not m.all_failed) && m.waitfor = rank then 
    increase m

let check m =
(*
  eprintf "fast: m.send_credit.(%d) = %d  msg_len = %d\n" m.waitfor m.send_credit.(m.waitfor) m.msg_len ;
*)
  m.all_failed || (m.send_credit.(m.waitfor) > m.msg_len)


(* Ohad. How much credit do I have?
 *)
let left m = m.send_credit.(m.waitfor) - m.msg_len

(* We may want to change the name "take" since now we actually add
 * something.
 *)

let take m credit =
  m.msg_len <- m.msg_len + credit ;
  m


(* Note the complexity of this function is not constant. However,
 * since it is called only periodically, this may not be a problem.
 *)

let clear m =
  let credit = int_min m.msg_len m.send_credit.(m.waitfor) in
  for rank = 0 to pred m.nmembers do
    if not m.failed.(rank) then
      array_sub m.send_credit rank credit ;
  done ;
  m.msg_len <- m.msg_len - credit


let string_of_queue_len q = string_of_int (Queuee.length q)

let to_string_list m = [
  sprintf "rank=%d nmembers=%d msg_len=%d waitfor=%d all_failed=%b"
    m.rank m.nmembers m.msg_len m.waitfor m.all_failed ;
  sprintf "send_credit=%s" 
    (string_of_int_array m.send_credit) ;
  sprintf "ack_que=%s"
    (string_of_int_array m.ack_que) ;
  sprintf "recv_credit=%s"
    (string_of_int_array m.recv_credit) ;
  sprintf "failed=%s"
    (string_of_bool_array m.failed)
]  
(*end*)

(**************************************************************)
(*
module Slow : S = struct

type t = {
  rank : rank ;
  nmembers : nmembers ;
  ack_thresh : credit ;
  send_credit : credit array ;
  recv_credit : credit array ;
  failed : bool array
} 

let create rank nmembers ack_thresh send_credit recv_credit = 
  let send_credit = Array.copy send_credit in
  let recv_credit = Array.copy recv_credit in
  { rank = rank ;
    nmembers = nmembers ;
    ack_thresh = ack_thresh ;
    send_credit = send_credit ;
    recv_credit = recv_credit ;
    failed = array_create name nmembers false
  } 

(* Note: we require credit > send_credit.(n-1) - send_credit.(0) for
   the initial values of send_credit. This way after any member gets a
   credit message, its credit becomes the greatest in the group.
 *)
let got_credit m origin =
  if m.failed.(origin) then 
    failwith "got_credit from failed member" ;
  array_add m.send_credit origin m.ack_thresh
	  
let got_msg m origin msg_len =
(*
  eprintf "slow: m.recv_credit.(%d)=%d\n" origin m.recv_credit.(origin) ;
*)
  array_add m.recv_credit origin msg_len ;
(*
  eprintf "msg_len=%d  m.recv_credit.(%d)=%d" msg_len origin m.recv_credit.(origin) ;
*)
  m.recv_credit.(origin)

let set_credit m origin remainder =
  m.recv_credit.(origin) <- remainder

let fail m rank =
  if rank = m.rank then
    failwith "I'm dead?" ;
  m.failed.(rank) <- true ;
  m.send_credit.(rank) <- 0 ;
  m.recv_credit.(rank) <- 0

let check m =
  let good = ref true in
  for i = 0 to pred m.nmembers do
(*
    eprintf "slow: m.send_credit.(%d) = %d\n" i m.send_credit.(i) ;
*)
    if i <> m.rank
    && (not m.failed.(i))
    && m.send_credit.(i) <= 0
    then good := false
  done ;
  !good

let take m credit =
  for i = 0 to pred m.nmembers do
    if i <> m.rank
    && (not m.failed.(i))
    then
      array_sub m.send_credit i credit
  done ;
  m

(* Note the complexity of this function is not constant. However,
 * since it is called only periodically, this may not be a problem.
 *)

let clear _ = ()

let to_string_list m = [
  sprintf "rank=%d  nmembers=%d"
    m.rank m.nmembers ;
  sprintf "send_credit=%s" 
    (string_of_int_array m.send_credit) ;
  sprintf "recv_credit=%s"
    (string_of_int_array m.recv_credit) ;
  sprintf "failed=%s"
    (string_of_bool_array m.failed) 
]  
end

module Check : S = struct
  module F = Fast
  module S = Slow

  type t = { f : Fast.t ; s : Slow.t }
	
  let checker f s fn a b p =
    if a <> b then (
      eprintf "function=%s\n" fn ;
      eprintf "fast=%s, slow=%s\n" (p a) (p b) ;
      List.iter (fun s -> eprintf "MCREDIT:fast:%s\n" s) (F.to_string_list f) ;
      List.iter (fun s -> eprintf "MCREDIT:slow:%s\n" s) (S.to_string_list s) ;
      failwith "mismatch returns"
    ) ;
    a

  let create r m a sc rc =
    { f = F.create r m a sc rc ;
      s = S.create r m a sc rc }
  let got_credit {f=f;s=s} o =
    checker f s "got_credit" (F.got_credit f o) (S.got_credit s o) (fun _ -> "()")
  let got_msg {f=f;s=s} o l =
    checker f s "got_msg" (F.got_msg f o l) (S.got_msg s o l) string_of_int
  let set_credit {f=f;s=s} o r =
    checker f s "set_credit" (F.set_credit f o r) (S.set_credit s o r) (fun _ -> "()")
  let fail {f=f;s=s} r =
    checker f s "fail" (F.fail f r) (S.fail s r) (fun _ -> "()")
  let check {f=f;s=s} =
    checker f s "check" (F.check f) (S.check s) string_of_bool
  let clear {f=f;s=s} =
    checker f s "clear" (F.clear f) (S.clear s) (fun _ -> "()")
  let take {f=f;s=s} c =
    checker f s "take" (F.take f c) (S.take s c) (fun _ -> "()")
  let to_string_list {f=f;s=s} =
    ["\nFast:"]@(F.to_string_list f)@["\n\nSlow:"] @ (S.to_string_list s)
end  
*)
(**************************************************************)
(*
module Export = Fast

type t = Export.t
let create = Export.create
let got_credit = Export.got_credit
let got_msg = Export.got_msg
let set_credit = Export.set_credit
let fail = Export.fail
let check = Export.check
let left = Export.left
let clear = Export.clear
let take = Export.take
let to_string_list = Export.to_string_list
*)
(**************************************************************)
