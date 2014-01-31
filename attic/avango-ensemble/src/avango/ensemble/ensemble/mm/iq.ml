(**************************************************************)
(* IQ.ML *)
(* Author: Mark Hayden, 7/95 *)
(**************************************************************)
open Util
open Trans
open Buf
open Iovec
let ref = Pervasives.ref
(**************************************************************)
let name = Trace.file "IQ"
let failwith s = Trace.make_failwith name s
(**************************************************************)

let rec log2 i =
  if i <= 0 then 
    failwith "log2:arg <= 0" ;
  if i = 1 then 0 else succ (log2 (i lsr 1))

let pow2 i =
  if i > 30 then
    failwith "pow2:arg > 30" ;
  (1 lsl i)

(**************************************************************)

type ctl =
  | Data
  | Unset

type 'a get =
  | GData of Iovecl.t * 'a
  | GUnset
  | GReset

type 'a item = {
  mutable ctl : ctl ;
  mutable msg : 'a ;
  mutable iov : Iovecl.t ;
} 

type 'a t = {
  mutable lo     : int ;
  mutable hi     : int ;
  mutable read	 : int ;
  mutable arr    : 'a item array ;
  mutable alen   : int ;
  mutable mask   : int ;
  zero           : 'a ;
  name           : debug
}

(**************************************************************)

(* Alloc a zero entry.
 *)
let zeroc zero seqno = {
  ctl = Unset ;
  msg = zero ;
  iov = Iovecl.empty
}

let zero iq i = zeroc iq.zero i

(**************************************************************)

let idx_of_abs iq abs = abs land iq.mask
let maxi iq = iq.lo + iq.alen

(**************************************************************)
(*
let string_of_ctl = function
  | Data -> "D"
  | Unset -> "U"

let dump iq =
  eprintf "IQ:dump:name=%s\n" iq.name ;
  eprintf "  alen=%d lo=%d hi=%d read=%d\n" iq.alen iq.lo iq.hi iq.read ;
  for i = iq.lo to pred (maxi iq) do
    let item = iq.arr.(idx_of_abs iq i) in
    let kind = string_of_ctl item.ctl in
    let pointer = "" in
    let pointer = if i =| iq.lo then pointer ^ "<-lo" else pointer in
    let pointer = if i =| iq.read then pointer ^ "<-read" else pointer in
    let pointer = if i =| iq.hi then pointer ^ "<-hi" else pointer in
    printf "  %d:%d:%s:%s\n" i 0(*item.seqno*) kind pointer ;
  done
*)
(**************************************************************)

let check iq =
()(*
  let failwith s = 
    dump iq ;
    failwith ("check:"^s) 
  in
  if iq.alen <> Array.length iq.arr then failwith "iq.alen" ;
  if iq.mask <> pred iq.alen then failwith "iq.mask" ;
  if iq.read < iq.lo && iq.read <> 0 then failwith "iq.read" ;
  if iq.hi < iq.lo then failwith "iq.hi" ;
  if iq.read > iq.hi then failwith "read > hi" ;
  if iq.lo > iq.hi then failwith "lo > hi" ;
  let arr = iq.arr in
  for i = iq.lo to pred (maxi iq) do
    if arr.(idx_of_abs iq i).seqno <> i then 
      failwith "iq.arr bad seqno"
  done
*)
(* Replaced by the above stuff
  for i = 0 to pred iq.alen do
    let arri = arr.(i) in
    for j = succ i to pred iq.alen do
      if arri == arr.(j) then
	failwith "iq.arr has copied entries"
    done
  done
*)
(**************************************************************)

let get_unsafe iq i = 
  let idx = idx_of_abs iq i in
  assert (idx >= 0) ;
  assert (idx < Array.length iq.arr) ;
  let item = iq.arr.(idx) in
  item

(**************************************************************)

(* Grow to a larger size array.
 *)
let grow iq nlen =
  let olen = iq.alen in
  let omask = iq.mask in
  let nlen = 
    let len = int_max nlen olen in
    let pow = log2 (2*len-1) in
    let len = pow2 pow in		(* always a power of 2 *)
    let len = int_max 16 len in		(* always grow to at least 16 *)
    len
  in
  let nmask = pred nlen in

  (* We try to choose an old value for the init to avoid a minor
   * collection (see byterun/array.c).
   *)
  let init = if olen = 0 then zero iq 0 else iq.arr.(0) in
  let na = Array.create nlen init in
  for abs = iq.lo to pred (maxi iq) do
    let oi = abs land omask in
    let ni = abs land nmask in
    na.(ni) <- iq.arr.(oi)
  done ;
  for abs = maxi iq to pred (iq.lo + nlen) do
    let ni = abs land nmask in
    na.(ni) <- zero iq abs
  done ;
  iq.arr <- na ;
  iq.alen <- nlen ;
  iq.mask <- nmask

(**************************************************************)
(*
let grow iq i = check iq ; grow iq i ; check iq
*)
(**************************************************************)

let create debug zeroa = 
  let iq = {
    arr  = [|zeroc zeroa 0|] ;
    mask = 0 ;
    alen = 1 ;
    lo   = 0 ;
    hi   = 0 ;
    read = 0 ;
    zero = zeroa ;
    name = debug
  } in
(*
  grow iq 8 ;
*)
  iq

(**************************************************************)

let rec(*avoid inline*) overflow iq new_hi =
  assert (new_hi >= maxi iq) ;
  grow iq (new_hi - iq.lo + 1) ;
  assert (new_hi < maxi iq)

(**************************************************************)

let set_hi iq i =
  if i >| iq.hi then
    iq.hi <- i

(**************************************************************)

let get_iov debug item = 
  assert (item.ctl = Data) ;
  item.iov

(**************************************************************)

let get_ctl iq i =
  assert (i >=| iq.lo) ;
  if i >=| maxi iq then
    Unset
  else (
    let item = get_unsafe iq i in
    item.ctl
  )

let get iq i =
  if i <| iq.lo then
    GReset
  else if i >=| maxi iq then
    GUnset
  else (
    let item = get_unsafe iq i in
    match item.ctl with
    | Data ->
    	let iov = get_iov "get" item in
    	GData(iov,item.msg)
    | Unset ->
	GUnset
  )

(**************************************************************)
(* A helper function for set and opt_insert_check_doread.
 *)

let do_set item iov msg =
  if not (msg == item.msg) then
    item.msg <- msg ;
  item.ctl <- Data ;
  item.iov <- Iovecl.copy iov ;
  true

(**************************************************************)

let set iq i iov msg =
  if i <| iq.lo then (
    false
  ) else (
    set_hi iq (succ i) ;
    if i >=| maxi iq then
      overflow iq i ;
    let item = get_unsafe iq i in
    match item.ctl with
    | Unset -> do_set item iov msg
    | _ -> false
  )

let do_set_msg item msg =
  if not (msg == item.msg) then
    item.msg <- msg ;
  true

let msg_update iq i msg =
  if i <| iq.lo then (
    false
  ) else (
    let item = get_unsafe iq i in
    match item.ctl with
    | Data -> do_set_msg item msg
    | _ -> false
  )

(**************************************************************)

let opt_insert_check iq i =
  i =| iq.read && i =| iq.hi

(**************************************************************)

let opt_insert_doread iq i iov msg =
  assert (i >=| iq.lo) ;
  let next = succ i in
  iq.read <- next ;
  iq.hi   <- next ;
  if i >=| maxi iq then
    overflow iq i ;
  let item = get_unsafe iq i in
  assert (item.ctl = Unset) ;
  ignore (do_set item iov msg) ;
  iq

(**************************************************************)

let opt_insert_check_doread iq i iov msg =
  if opt_insert_check iq i then (
    ignore (opt_insert_doread iq i iov msg) ;
    true
  ) else (
    false
  )

(**************************************************************)

let clear_item iq i =
  let item = get_unsafe iq i in
  match item.ctl with
  | Data ->
      Iovecl.free (get_iov "set_lo" item) ;
      item.ctl <- Unset ;
      item.iov <- Iovecl.empty ;
      if not (iq.zero == item.msg) then
	item.msg <- iq.zero
  | Unset ->
      ()

let set_lo iq n =
  if n >| iq.lo then (
    let lo = iq.lo in
    if n >=| maxi iq then
      overflow iq n ;
    for i = lo to pred n do
      clear_item iq i
    done ;
    iq.lo <- n ;
    set_hi iq n
  )
	
(**************************************************************)

let lo iq = iq.lo
let hi iq = iq.hi
let read iq = iq.read

(**************************************************************)

let free iq = set_lo iq iq.hi

(**************************************************************)

let assign = set

let add iq iov msg =
  if not (set iq iq.hi iov msg) then 
    failwith "add:sanity" ;
  iq

(**************************************************************)

let rec get_prefix iq f =
  let lo = iq.lo in
  match get_ctl iq lo with
  | Data ->
      let item = get_unsafe iq lo in
      let iov = get_iov "get_prefix" item in
      f lo iov item.msg ;
      set_lo iq (succ lo) ;
      get_prefix iq f
  | _ -> ()

(**************************************************************)

let rec read_prefix iq f =
  let read = iq.read in
  assert (read >= iq.lo) ;
  if get_ctl iq read = Data then (
    let item = get_unsafe iq read in
    let iov = get_iov "read_prefix" item in
    f read iov item.msg ;
    iq.read <- succ read ;
    read_prefix iq f
  )

(**************************************************************)

let opt_update_check iq i =
  i =| iq.lo && i =| iq.hi

let opt_update_update iq i =
  let i = succ i in
  set_lo iq i ;
  iq.hi <- i ;
  iq

let opt_update_old iq i =
  if opt_update_check iq i then (
    ignore (opt_update_update iq i) ;
    true
  ) else false

(**************************************************************)

let clear_unread iq =
  if iq.hi >= maxi iq then
    overflow iq iq.hi ;
  for i = iq.read to iq.hi do
    clear_item iq i
  done ;
  iq.hi <- iq.read

(**************************************************************)

let list_of_iq_interval iq (lo,hi) =
  let l = ref [] in

  (* Never go below iq.lo
   *)
  let lo = int_max lo iq.lo in

  (* Go in reverse to get the right order for the list.
   *)
  for i = pred hi downto lo do
    match get_ctl iq i with
    | Data -> 
	let item = get_unsafe iq i in
	let iov = get_iov "" item in
	l := (i,iov,item.msg) :: !l
    | _ -> ()
  done ;
  !l

(**************************************************************)

let list_of_iq iq =
  list_of_iq_interval iq (iq.lo,iq.hi)

(**************************************************************)
(* HOLES: Return the index of the start and end of any hole at
 * the beginning of the "window."  Used for NAKs...
 *)

let rec next_set iq i =
  if i >=| iq.hi then 
    i
  else (
    match get_ctl iq i with
    | Data -> i
    | _ -> next_set iq (succ i)
  )

let hole_help iq i =
  let first_set = next_set iq i in
  if first_set <=| iq.lo || i >=| iq.hi then
    None
  else 
    Some (iq.lo,first_set)

let hole iq = hole_help iq iq.lo
let read_hole iq = 
  assert(iq.read >= iq.lo) ;
  hole_help iq iq.read

(**************************************************************)
