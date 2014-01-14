(**************************************************************)
(* TIME.ML *)
(* Author: Mark Hayden, 8/96 *)
(**************************************************************)
open Util
open Socksupp
open Hsys
(**************************************************************)
let name = Trace.file "TIME"
let failwith = Trace.make_failwith name
(**************************************************************)

type t = Hsys.timeval

let onemillion = 1000000
let tenmillion = 10000000

let rec of_correct_ints sec10 usec = 
  if usec < 0 then (
    of_correct_ints (pred sec10) (usec + tenmillion)
  ) else if usec >= tenmillion then (
    let sec10 = sec10 + (usec / tenmillion) in
    let usec = usec mod tenmillion in
    assert (usec >= 0 && usec < tenmillion) ;
    {sec10=sec10;usec=usec}
  ) else {sec10=sec10;usec=usec}

let of_correct_ints sec10 usec = 
  if usec < 0 || usec >= tenmillion then
    of_correct_ints sec10 usec
  else {sec10=sec10;usec=usec}

let of_ints sec usec = 
  let usec = usec + ((sec mod 10) * onemillion) in
  let sec10 = sec / 10 in
  of_correct_ints sec10 usec
  
let of_int sec = of_ints sec 0

let zero = {sec10=0;usec=0}
let neg_one = {sec10=(-1);usec=0;}
let invalid = neg_one

let is_zero t = t.usec = 0 && t.sec10 = 0
let is_invalid t = t.usec = 0 && t.sec10 = -1

let to_timeval {sec10=sec10;usec=usec} = {sec10=sec10;usec=usec}
let of_timeval {sec10=sec10;usec=usec} = {sec10=sec10;usec=usec}

(* Print out time with millisecond accuracy
 *)
let to_string t = 
  if (t.sec10 > 0) then
    sprintf "%d%d.%03d" t.sec10 (t.usec / onemillion) ((t.usec mod onemillion) / 1000)
  else
    sprintf "%d.%03d" (t.usec / onemillion) ((t.usec mod onemillion) / 1000)

(* Convert with full accuracy.
 *)
let to_string_full t = 
  sprintf "%d%d.%06d" t.sec10 (t.usec / onemillion) (t.usec mod onemillion)

let add a b =
  of_correct_ints (a.sec10 + b.sec10) (a.usec + b.usec)

let sub a b = 
  of_correct_ints (a.sec10 - b.sec10) (a.usec - b.usec)

let of_float f =
  let usec,sec10 = modf (f /. 10.) in
  let usec = usec *. 1.0E7 in
  let sec10 = truncate sec10 in
  let usec = truncate usec in
  of_correct_ints sec10 usec

let safe_int_of_string s =
  try
    int_of_string s
  with _ -> 0

let of_string s =
(*
  eprintf "TIME:of_string:%s\n" s ;
*)
  try
    let point = strchr '.' s in
    let secs,sec10 = 
      if point = 0 then 
	0,0 
      else
      	(int_of_string (String.sub s (pred point) 1)),(safe_int_of_string (String.sub s 0 (pred point))) 
    in

    let usec = 
      let usec = String.sub s (succ point) (String.length s - point - 1) in
      let useclen = String.length usec in
      if useclen = 0 then 0 else (
	let usec = int_of_string usec in
	let rec loop usec len =
	  if len < 6 then
	    loop (usec * 10) (succ len)
	  else if len > 6 then
	    loop (usec / 10) (pred len)
	  else usec
	in
	let usec = loop usec useclen in
	usec
      )
    in
    of_correct_ints sec10 (usec + (secs * onemillion))
  with Not_found ->
    let pos = pred (String.length s) in
    of_correct_ints (safe_int_of_string (String.sub s 0 pos)) (int_of_string (String.sub s pos 1))

(*
let of_string s =
  let time = float_of_string s in
  let time = of_float time in
  let new_time = of_string s in
  if new_time.sec <> time.sec 
  || abs (new_time.usec - time.usec) > 10 
  then
    failwith "new time is bad" ;
  new_time
*)

let to_float tv =
  ((float tv.sec10) *. 10.) +. ((float tv.usec) /. 1.0E6)

let ge a b =
  if a.sec10 = b.sec10 then
    a.usec >= b.usec
  else 
    a.sec10 > b.sec10

let gt a b =
  if a.sec10 = b.sec10 then
    a.usec > b.usec
  else 
    a.sec10 > b.sec10

(* Check that subtracting two integers
 * does not cause overflow.
 *)
let check_overflow i j =
  assert (
    let diff = i - j in
    if (i > j) then
      diff > 0
    else if i < j then
      diff < 0
    else true
  )

let cmp a b =
  if a.sec10 = b.sec10 then (
    check_overflow a.usec b.usec ;
    a.usec - b.usec
  ) else (
    check_overflow a.sec10 b.sec10 ;
    a.sec10 - b.sec10
  )

module Ord =
  struct
    type time = t
    type t = time
    let zero = zero
    let ge = ge
    let cmp = cmp
  end

let gettimeofday = Hsys.gettimeofday
let select = Hsys.select

(**************************************************************)
(* Another version of the above stuff with in-place 
 * modification.  Use at your own risk.
 *)
let rec set t sec10 usec = 
  if usec >= 0 && usec < tenmillion then (
    t.sec10 <- sec10 ; 
    t.usec <- usec
  ) else if usec < 0 then (
    set t (pred sec10) (usec + tenmillion)
  ) else (
    set t (succ sec10) (usec - tenmillion)
  )

type m = t
let mut_copy {sec10=sec10;usec=usec} = {sec10=sec10;usec=usec}
let mut () = mut_copy zero
let mut_set m t = m.sec10 <- t.sec10 ; m.usec <- t.usec
let mut_gettimeofday = Hsys.gettimeofdaya
let mut_ge = ge
let mut_add m t = set m (m.sec10 + t.sec10) (m.usec + t.usec)
let mut_sub m t = set m (m.sec10 - t.sec10) (m.usec - t.usec)
let mut_sub_rev t m = set m (t.sec10 - m.sec10) (t.usec - m.usec)
let mut_select = Hsys.select

(*
type m' = m
module Mut_hide = struct
  type m = m'
  let copy = mut_copy
  let zero = mut_zero
  let gettimeofday = mut_gettimeofday
  let ge = mut_ge
  let of_int = mut_of_int
  let add = mut_add
  let sub = mut_sub
  let sub_rev = mut_sub_rev
  let select = mut_select
end
*)
(**************************************************************)
