(**************************************************************)
(* QUEUEA.ML *)
(* Author: Mark Hayden, 10/97 *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "QUEUEA"
let failwith = Trace.make_failwith name
(**************************************************************)

type 'a t = {
  mutable arr : 'a Arraye.t ;
  mutable head : int ;
  mutable len : int ;			(* length of queue *)
  mutable alen : int ;			(* size of array >= len *)
  bottom : 'a ;
  bottomf : unit -> 'a
}

let create_gen bot botf =
  let alen = 11 in
  let arr = Arraye.create alen bot in
  for i = 0 to pred alen do
    Arraye.set arr i (botf ())
  done ;
  { arr = arr ;
    head = 0 ;
    len = 0 ;
    alen = alen ;
    bottom = bot ;
    bottomf = botf
  }

let create bot = create_gen bot (fun () -> bot)
let create_mut bot = create_gen (bot ()) bot

let dump q =
  eprintf "QUEUEA:head=%d, len=%d, alen=%d\n"
    q.head q.len (Arraye.length q.arr) ;
  let bot = Arraye.map (fun b -> b <> q.bottom) q.arr in
  let bot = Arraye.to_array bot in
  eprintf "QUEUEA:live=%s\n" (string_of_bool_array bot)

let set_arr q a =
  q.arr <- a ;
  q.alen <- Arraye.length a

let alen q = q.alen (* size of the array *)
let length q = q.len (* number of live entries in the array *)
let empty q = length q = 0

let check q w =()
(*
  let failwith s = dump q ; failwith (w^":"^s) in
  if q.len > alen q then failwith "length too large" ;
  if q.head > alen q then failwith "head too large" ;
  for i = 0 to pred (alen q) do
    let wrapping = q.head + q.len > alen q in
    let alive =
      (wrapping && (i >= q.head || i < ((q.head + q.len) mod (alen q)))) ||
      ((not wrapping) && (i >= q.head && i < q.head + q.len))
    in
    if alive <> (q.arr.(i) <> q.bottom) then
      failwith "bad array entry" ;
  done
*)

let check_mut q w =
()(*
  let failwith s = dump q ; failwith (w^":"^s) in
  if q.len > alen q then failwith "length too large" ;
  if q.head >= alen q then failwith "head too large" ;
  for i = 0 to pred (alen q) do
    for j = succ i to pred (alen q) do
      if Arraye.get q.arr i == Arraye.get q.arr j then
	printf "QUEUEA:entry %d = %d\n" i j
    done ;
  done
*)

let expand q =
  let oarr = q.arr in
  let oalen = alen q in
  let nalen = oalen * 2 + 10 in
  let narr = Arraye.create nalen q.bottom in
  (* If using a mutable queue, we need distinct elements
   *)
  for i = 0 to pred nalen do
    Arraye.set narr i (q.bottomf ())
  done ;

  for i = 0 to pred q.len do
    Arraye.set narr i (Arraye.get oarr ((q.head + i) mod oalen))
  done ;
  
  q.head <- 0 ;
  set_arr q narr

(* We only contract when at the beginning of the queue's array
 * and only when the size of the array is large enough.
 *)
let contract q =
  if q.alen > 200 && 4 * q.len + 30 < q.alen then (
    if q.head <> 0 then 
      failwith sanity ;
    let oalen = alen q in
    let nalen = (oalen - 10) / 2 in
    let narr = Arraye.sub q.arr 0 nalen in
    set_arr q narr
  )

(* This is called on the second try after
 * the array has been expanded.  We use
 * a separate function because let rec
 * prevents inlining.
 *)
let rec expand_and_add q it =
  expand q ;
  let len = q.len in
  let alen = alen q in
  q.len <- succ len ;
  Arraye.set q.arr ((q.head + len) mod alen) it

(**************************************************************)

let add q it =
  let len = q.len in
  let alen = alen q in
  let idx = q.head + len in
  if idx < alen then (
    Arraye.set q.arr idx it ;
    q.len <- succ len
  ) else if len < alen then (
    Arraye.set q.arr (idx - alen) it ;
    q.len <- succ len
  ) else (
    expand_and_add q it
  )

(**************************************************************)

(* This is called on the second try after
 * the array has been expanded.
 *)
let rec(*avoid inlining*) expand_and_add_mut q =
  expand q ;
  let len = q.len in
  let alen = alen q in
  q.len <- succ len ;
  Arraye.get q.arr ((q.head + len) mod alen)

(**************************************************************)
(* Same as above, but return whatever was there
 * before.
 *)

let add_mut q =
  let len = q.len in
  let alen = alen q in
  let idx = q.head + len in
  if idx < alen then (
    q.len <- succ len ;
    Arraye.get q.arr idx
  ) else if len < alen then (
    q.len <- succ len ;
    Arraye.get q.arr (idx - alen)
  ) else (
    expand_and_add_mut q
  )

(**************************************************************)

let take_unsafe q =
  let len = q.len in
  assert (len > 0) ;
  q.len <- pred len ;

  let head = q.head in
  let arr = q.arr in
  let ret = Arraye.get arr head in
  Arraye.set arr head q.bottom ;
  if succ head >= q.alen then (
    (* Whenever the head gets to the beginning
     * of the array, check if we want to contract
     * it.
     *)
    q.head <- 0 ;
    contract q ;
    ret
  ) else (
    q.head <- succ head ;
    ret
  )

(**************************************************************)

(* Same as above, but don't reset the entries.
 *)
let take_mut_unsafe q =
  let len = q.len in
  q.len <- pred len ;

  let head = q.head in
  let arr = q.arr in
  let ret = Arraye.get arr head in
  let succ_head = succ head in
  if succ_head >= q.alen then (
    (* Whenever the head gets to the beginning
     * of the array, check if we want to contract
     * it.
     *)
    q.head <- 0 ;
    contract q ;
    ret
  ) else (
    q.head <- succ_head ;
    ret
  )

(**************************************************************)
(*
let take q =
  let len = q.len in
  if len <= 0 then
    raise Queue.Empty ;
  take_unsafe q
*)
(**************************************************************)

let clear q =
  while not (empty q) do
    assert (q.len > 0) ;
    take_unsafe q
  done

(**************************************************************)

let iter f q =
  let alen = alen q in
  let arr = q.arr in
  let head = q.head in
  for i = 0 to pred q.len do
    f (Arraye.get arr ((head + i) mod alen))
  done

(**************************************************************)
(*
module S = Queuee
type 'a t = {f : 'a u ; s: 'a S.t}

let checker f s fn a b p =
  if a <> b then (
    eprintf "function=%s\n" fn ;
    eprintf "fast=%s, slow=%s\n" (p a) (p b) ;
    failwith "mismatch returns"
  ) ;
  a

let create bottom =
  { f = create bottom ;
    s = S.create () }
let add {f=f;s=s} a =
  checker f s "add" (add f a) (S.add a s) (fun _ -> "()")
let take {f=f;s=s} =
  checker f s "take" (take f) (S.take s) (fun _ -> "_")
let empty {f=f;s=s} =
  checker f s "empty" (empty f) (S.empty s) string_of_bool
let clear _ = failwith sanity
let iter _ = failwith sanity
let length _ = failwith sanity
*)
(**************************************************************)

let _ = Trace.test_declare name (fun () ->
    let verbose = false in
    let q = create_mut (fun () -> ref (-1)) in
    let counter1 = Util.counter () in
    let counter2 = Util.counter () in

    for i = 1 to 100000 do
      match Random.int 5 with
      | 0 | 1 | 2 ->
	  let r = add_mut q in
	  if !r <> -1 then failwith "add_mut:not bottom" ;
	  r := counter1 () ;
	  if verbose then eprintf "adding %d\n" !r
      | 3 | 4 ->
	  if not (empty q) then (
	    let r = take_mut_unsafe q in
	    if verbose then eprintf "taking %d\n" !r ;
	    if !r = -1 then failwith "take_mut:got bottom" ;
	    if !r <> counter2 () then
	      failwith "take_mut:out-of-order" ;
	    r := (-1)
	  )
      | _ -> failwith sanity
    done
  )

(**************************************************************)
