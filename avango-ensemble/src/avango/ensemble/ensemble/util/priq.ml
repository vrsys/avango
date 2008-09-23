(**************************************************************)
(* PRIQ.ML: heap-based priority queues *)
(* Author: Mark Hayden, 3/96 *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "PRIQ"
let failwith s = Trace.make_failwith name s
(**************************************************************)

type ('a,'b) item = {
  key : 'a ;
  data : 'b
} 

type ('key,'data) t = {
  ge             : 'key -> 'key -> bool ;
  cmp            : 'key -> 'key -> int ;
  mutable table  : ('key,'data) item array ;
  mutable nitems : int
}

(**************************************************************)

let create zero nodata cmp = 
  let zero = { key = zero ; data = nodata } in
  { ge     = (fun a b -> cmp a b >= 0) ;
    cmp    = cmp ;
    table  = Array.create 14 zero ;
    nitems = 0
  }

let size pq = pq.nitems
let min pq = pq.table.(1).key
let empty pq = pq.nitems = 0
let zero pq = pq.table.(0)

(**************************************************************)

let double i = i lsl 1
let halve i = i asr 1

let unset pq i =
  pq.table.(i) <- zero pq

let assign pq i k d =
  assert (i <> 0) ;
  pq.table.(i) <- { key = k ; data = d }

let get_key pq i =
  assert (i <> 0) ;
  pq.table.(i).key

let get_data pq i =
  assert (i <> 0) ;
  pq.table.(i).data

let to_list pq =
  let a = Array.sub pq.table 1 pq.nitems in
  let a = Array.map (fun i -> (i.key,i.data)) a in
  Array.to_list a

let clear pq f =
  for i = 1 to pq.nitems do
    let item = pq.table.(i) in
    f item.key item.data ;
    unset pq i
  done ;
  pq.nitems <- 0

(* Grow priority queue arrays by at least one element.
 *)
let grow pq =
  assert (Array.length pq.table > 0) ;
  let add = Array.create (Array.length pq.table) (zero pq) in
  pq.table <- Array.append pq.table add

let set pq i j =
  assert (i <= pq.nitems) ;
  pq.table.(i) <- pq.table.(j)

let add pq key data =
  pq.nitems <- succ pq.nitems ;
  if pq.nitems >=| Array.length pq.table then
    grow pq ;

  let i_r = ref pq.nitems in
  while !i_r > 0 do
    let i = !i_r in
    let j = halve i in
    if j <=| 0 || pq.cmp key (get_key pq j) >= 0 then (
      assign pq i key data ;
      i_r := 0				(* break out *)
    ) else (
      set pq i j ;
      i_r := j
    )
  done

let take pq =
  assert (pq.nitems > 0) ;
  let data = get_data pq 1 in
  let last = pq.nitems in
  let last_key = get_key pq last in
  pq.nitems <- pred last ;
  let i = ref 1 in
  while !i < last do
    let left = double !i in
    let right = succ left in
    if left >=| last then (
      set pq !i last ;
      i := last				(* break out *)
    ) else (
      let nxt =
	if right >=| last
        || pq.cmp (get_key pq right) (get_key pq left) >= 0
	then left
	else right
      in
      if pq.cmp (get_key pq nxt) last_key >= 0 then (
	set pq !i last ;
	i := last			(* break out *)
      ) else (
	set pq !i nxt ;
	i := nxt
      )
    )
  done ;
  unset pq last ;
  data

let get pq f t =
  if pq.nitems =| 0 || not (pq.cmp t (get_key pq 1) >= 0) then (
    false
  ) else (
    while pq.nitems >| 0 && pq.cmp t (get_key pq 1) >= 0 do
      f (get_key pq 1) (take pq)
    done ;
    true
  )

let getopt pq upto =
  if pq.nitems =| 0 || not (pq.cmp upto (get_key pq 1) >= 0) then (
    false
  ) else (
    while pq.nitems >| 0 && pq.cmp upto (get_key pq 1) >= 0 do (
      take pq upto
    )
    done ;
    true
  )

(**************************************************************)
(* Functorized version. 
 * Todo: roll some of the stuff into here.
 *)

module type S =
  sig
    type key
    type 'data t
    val create : 'data -> 'data t
    val add : 'data t -> key -> 'data -> unit
    val get : 'data t -> (key -> 'data -> unit) -> key -> bool
    val getopt : (key -> unit) t -> key -> bool
    val size : 'data t -> int
    val min : 'data t -> key	(* BUGGY *)
    val take : 'data t -> 'data
    val empty : 'data t -> bool
    val to_list	: 'data t -> (key * 'data) list
    val clear : 'data t -> (key -> 'data -> unit) -> unit
  end

module Make(Ord: Trans.OrderedType) : (S with type key = Ord.t) =
  struct
    type key = Ord.t
    type 'a priq = (key,'a) t
    type 'a t = 'a priq
    let create nd = create Ord.zero nd Ord.cmp
    let add = add
    let get = get
    let getopt = getopt
    let size = size
    let min = min
    let take = take
    let empty = empty
    let to_list = to_list
    let clear = clear
  end

(**************************************************************)
(* Stuff for debugging. 
 *)

let _ = Trace.test_declare name (fun () ->
  Random.self_init () ;

  let print pq =
    printf "PRIQ(%d)" pq.nitems ;
    for i = 1 to pq.nitems do
      printf ":%d" (get_key pq i)
    done ;
    printf "\n"
  in

  let check pq =
    for i = succ pq.nitems to pred (Array.length pq.table) do
      if (get_key pq i) <> 0 || (get_data pq i) <> None then (
	printf "size=%d, i=%d\n" pq.nitems i ;
	failwith "unused got modified"
      )
    done ;

    for i = 1 to pq.nitems do
      if (get_key pq i) <> some_of name (get_data pq i) then
	failwith "bad vals"
    done ;

    for i = 2 to pq.nitems do
      let j = halve i in
      if (get_key pq j) > (get_key pq i) then (
	printf "predicate broke: %d,%d > %d,%d\n"
	  j (get_key pq j)
	  i (get_key pq i) ;
	print pq ;
	failwith "predicate broke"
      )
    done
  in

(*
    let pq = create 0 None (>=) in

    let n = 18 in

    for i = 1 to n do
      check pq ;
      let t = Random.int 1000 in
      add pq t (Some t)
    done ;

    for i = 1 to n do
      check pq ;
      print pq ;
      let _ = take pq in ()
    done ;
    check pq ;
    print pq ;

    for i = 1 to 100 do
      check pq ;
      let t = Random.int 1000 in
      add pq t (Some t)
    done ;

    for i = 1 to 100 do
      check pq ;
      let t = i * 10 in
      printf "%d\n" t ;
      let _ = get pq (fun k d -> printf "\t%d\n" k) t in ()
    done ;

    check pq ;
    print pq ;
*)

    let pq = create 0 None compare in
    let l = ref [] in

    for i = 1 to 1000000 do
      if (i mod 100) = 0 then (
	printf "PRIQ:test(%d), size=%d\n" i (size pq)
      ) ;
      
      check pq ;

      let l1 = List.rev (List.sort pq.cmp !l) in
      let l2 = List.rev (List.sort pq.cmp (List.map fst (to_list pq))) in
(*
      printf "1:%s\n" (string_of_list string_of_int l1) ;
      printf "2:%s\n" (string_of_list string_of_int l2) ;
*)
      if l1 <> l2 then (
	printf "PRIQ:bad intersect (i=%d)\n" i ;
	failwith sanity
      ) ;
      check pq ;
      if Random.int 21 <= 9 then (
	let t = Random.int 1000 in
(*
	printf "added:%d\n" t ;
*)
	add pq t (Some t) ;	
	l := t :: !l ;
	()
      ) else (	
	if size pq > 0 then (
(*
          printf "pretake\n" ;
*)
	  let t = take pq in
(*
          printf "took:%s\n" (string_of_option string_of_int t) ;
*)
	  l := except (some_of name t) !l
	)
      )
    done
  )

(**************************************************************)
