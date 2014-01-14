(**************************************************************)
(* REQUEST.ML *)
(* Author: Mark Hayden, 4/95 *)
(**************************************************************)
(* Request Queues : Maintain linked list of requests in the
 * order that they were made.  Supports insert, remove, take
 * operations with amortized O(1) time and no memory
 * allocation. *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "REQUEST"
let failwith = Trace.make_failwith name
(**************************************************************)

type t = {
  q : int array ;
  skip : bool array ;
  mutable head : int ;
  mutable tail : int
}

let debug r =
  eprintf "head=%d, tail=%d\n" r.head r.tail ;
  for i = 0 to pred (Array.length r.q) do
    eprintf "%3d:%b:%d\n" i r.skip.(i) r.q.(i)
  done

let create n = {
  q    = Array.create n (-1) ;
  skip = Array.create n true ;
  head = (-1) ;
  tail = (-1)
}

let remove r i =
  r.skip.(i) <- true

let add r i =
  let next = r.q.(i) in
  r.skip.(i) <- false ;

  if i = r.tail or next >= 0 then (
    (* do nothing *)
  ) else if r.head < 0 then (
    r.head <- i ;
    r.tail <- i
  ) else (
    r.q.(r.tail) <- i ;
    r.tail <- i
  )

let rec take r =
  let i = r.head in
  if i < 0 then (-1) else (
    if i = r.tail then (
      r.tail <- (-1) ;
      r.head <- (-1)
    ) else (
      r.head <- r.q.(i)
    ) ;

    r.q.(i) <- (-1) ;
    if r.skip.(i) then (
      take r
    ) else (
      r.skip.(i) <- true ;
      i
    )
  )

let to_string r =
  let rec loop i =
    if i < 0 then ""
    else if r.skip.(i) then loop r.q.(i)
    else "->"^(string_of_int i)^loop r.q.(i)
  in "["^(loop r.head)^"]"

(**************************************************************)

let _ = Trace.test_declare name (fun () ->
    let pred r =
      let n = Array.length r.skip in
      if r.head < 0 & r.q <> Array.create n (-1) then
	failwith "neg head, but not all -1" ;
      if r.head < 0 & r.skip <> Array.create n true then
	failwith "neg head, but not all skipped" ;
      if r.head >= 0 &
	 r.head <> r.tail &
	 r.q = Array.create n (-1) then
	failwith "neg body, but pos head"
    in

    let verbose = false in
    for i = 1 to 1000 do
      let n = Random.int 100 + 1 in
      let v = Array.create n false in
      Printf.eprintf "test #%d, n=%d\n" i n ; 
      let r = create n in
      for j = 1 to Random.int 500 + 500 do
  (*    debug r ;
	pred r ;
  *)
	let k = Random.int n in
	match Random.int 6 with
	|0|1|2 -> (
	    v.(k) <- true ;
	    add r k ;
	    if verbose then eprintf "add %d\n" k
	  )
	| 3 -> (
	    let l = take r in
	    if l >= 0 then (
	      v.(l) <- false ;
	      if verbose then eprintf "take=%d\n" l
	    ) else (
	      if verbose then eprintf "take failed\n"
	    )
	  )
	|4|5 -> (
	    if verbose then eprintf "remove %d\n" k ;
	    remove r k ;
	    v.(k) <- false
	  )
	| _ -> failwith sanity
      done ;

      for j = 1 to n do
	let l = take r in
	if l >= 0 then
	  v.(l) <- false
      done ;
      if take r >= 0 then
	failwith sanity ;

      if v <> Array.create n false then (
	debug r ;
	failwith "not negated"
      )
    done
  )

(**************************************************************)
