(**************************************************************)
(* LSET.ML *)
(* Author: Mark Hayden, 11/96 *)
(**************************************************************)
open Util
(**************************************************************)
let ltoa = Arrayf.of_list
let atol = Arrayf.to_list
(**************************************************************)

(* Arrayf.sort checks first if the array is ordered.
 *)
let sorta a = Arrayf.sort compare a
let sort a = atol (sorta (ltoa a))

(**************************************************************)
(* Debugging versions of some of these functions.
 *)
(*
let subtract l1 l2 =
  let old_subtract f = function
    | [] -> f
    | e  ->
      	let rec loop = function
          | [] -> []
       	  | elem::l -> if List.mem elem e then loop l else elem :: loop l
      	in loop f
  in
  let ret = subtract l1 l2 in
  if ret <> (sort (old_subtract l1 l2)) then
    failwith "subtract:check" ;
  ret
  
let union l1 l2 =      
  let old_union l1 l2 =
    let rec loop = function
      | [] -> l2
      | a::l -> if List.mem a l2 then loop l else a :: loop l
    in loop l1
  in
  let ret = union l1 l2 in
  if ret <> (sort (old_union l1 l2)) then
    failwith "union:check" ;
  ret

let intersect l1 l2 =  
  let old_intersect l1 l2 =
    let rec loop = function
      | [] -> []
      | a::l -> if List.mem a l2 then a :: loop l else loop l
    in loop l1
  in
  let ret = intersect l1 l2 in
  if ret <> (sort (old_intersect l1 l2)) then
    failwith "union:check" ;
  ret
*)

(**************************************************************)
(**************************************************************)

type 'a t = 'a Arrayf.t

let collapsea a = 
  assert (Arrayf.ordered compare a) ;
  let l = Arrayf.length a in
  
  (* Handle empty case first.
   *)
  if l = 0 then (
    Arrayf.empty
  ) else (
    (* Count up # of unique items.
     *)
    let n = ref 1 in
    for i = 1 to pred l do
      if Arrayf.get a (pred i) <> Arrayf.get a i then
	incr n
    done ;
    let n = !n in

    (* If all are unique, return original array.
     *)
    if n = l then (
      a					(* Already collapsed *)
     ) else (
      (* Only now do we actually allocate the new array.
       *)
      assert (n <> 0) ;			(* Always true *)
      let b = Arraye.create n (Arrayf.get a 0) in
      let j = ref 1 in
      for i = 1 to pred l do
	let v = Arrayf.get a i in
	if Arrayf.get a (pred i) <> v then (
	  assert (!j < n) ;
	  Arraye.set b !j v ;
	  incr j
	) ;
      done ;
      assert (!j = n) ;
      Arrayf.of_arraye b
    )
  )

let inject a =
  let a = sorta a in
  assert (Arrayf.ordered compare a) ;
  collapsea a

let project = Util.ident
let is_empty = Arrayf.is_empty
let to_string = Arrayf.to_string

(* Helper function for both subtract and intersect.
 *)
let helper op get_rest a1 a2 =
  let l1 = Arrayf.length a1 in
  let l2 = Arrayf.length a2 in
  
  (* Handle empty case first.
   *)
  if l1 = 0 then (
    Arrayf.empty
  ) else (
    (* Result array is at most the size of a1
     *)
    let b = Arraye.create l1 (Arrayf.get a1 0) in

    let j = ref 0 in
    let k = ref 0 in
    for i = 0 to pred l1 do
      let v = Arrayf.get a1 i in

      (* Scan through 2nd array looking for v.
       *)
      while !j < l2 && v > Arrayf.get a2 !j do
	incr j
      done ;

      (* This is where we use 'op'.
       *)
      if (!j < l2 && op v (Arrayf.get a2 !j)) ||
         (!j = l2 && get_rest) then (
	Arraye.set b !k v ;
	incr k
      )
    done ;
    
    (* If size is same as first array, it must equal
     * first array.
     *)
    if !k = l1 then (
      assert (a1 = Arrayf.of_arraye b) ;
      a1 
    ) else (
      Arrayf.of_arraye (Arraye.sub b 0 !k)
    )
  )

let intersecta a1 a2 =
  helper (=) false a1 a2

let subtracta a1 a2 =
  helper (<>) true a1 a2

(*
let _ =
  assert ((Arrayf.length (subtracta (Arrayf.singleton 1) (Arrayf.singleton 0))) = 1) ;
  assert ((Arrayf.length (subtracta (Arrayf.singleton 1) Arrayf.empty)) = 1)
*)

let uniona a1 a2 =
  let l1 = Arrayf.length a1 in
  let l2 = Arrayf.length a2 in
  if l1 = 0 then (
    a2
  ) else (
    let b = Arraye.create (l1 + l2) (Arrayf.get a1 0) in

    let j = ref 0 in
    let k = ref 0 in
    for i = 0 to pred l1 do
      let v1 = Arrayf.get a1 i in

      (* Add in all items in the 2nd array not in the first.
       *)
      while !j < l2 && v1 > Arrayf.get a2 !j do
	Arraye.set b !k (Arrayf.get a2 !j) ;
	incr k ;
	incr j ;
      done ;

      (* Add this guy in.
       *)
      Arraye.set b !k v1 ;
      incr k ;

      (* Skip if v1 is also in the 2nd array.
       *)
      if !j < l2 && v1 = Arrayf.get a2 !j then (
	incr j
      )
    done ;

    (* Add in rest of 2nd array.
     *)
    for j = !j to pred l2 do
      Arraye.set b !k (Arrayf.get a2 j) ;
      incr k ;
    done ;

    let b = Arraye.sub b 0 !k in
    Arrayf.of_arraye b
  )

(*
let subtracta a b = 
  let o = ltoa (subtract (atol a) (atol b)) in
  let n = subtracta a b in
  assert (o = n) ;
  n
*)

(*
let uniona a b = 
  let o = ltoa (union (atol a) (atol b)) in
  let n = uniona a b in
  assert (o = n) ;
  n
*)

let supera a b = is_empty (subtracta b a)
let disjointa a b = is_empty (intersecta a b)

(**************************************************************)

let helper op a b = 
  let a = inject (Arrayf.of_list a) in
  let b = inject (Arrayf.of_list b) in
  op a b

let super a b = helper supera a b
let disjoint a b = helper disjointa a b

let helper op a b = 
  let res = helper op a b in
  Arrayf.to_list (project res)

let subtract a b = helper subtracta a b
let union a b = helper uniona a b
let intersect a b = helper intersecta a b

let collapse a = 
  let a = inject (Arrayf.of_list a) in
  let a = Arrayf.to_list (project a) in
  a

(**************************************************************)
