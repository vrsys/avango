(**************************************************************)
(* ARRAYF.ML : functional arrays *)
(* Author: Mark Hayden, 4/95 *)
(**************************************************************)

type 'a t = 'a Arraye.t

(**************************************************************)

let append = Arraye.append
let bool_to_string = Arraye.bool_to_string
let combine = Arraye.combine
let concat = Arraye.concat
let copy = Arraye.copy
let create = Arraye.create
let doubleton = Arraye.doubleton
let empty = Arraye.empty
let filter = Arraye.filter
let filter_nones = Arraye.filter_nones
let flatten = Arraye.flatten
let for_all = Arraye.for_all
let for_all2 = Arraye.for_all2
let get = Arraye.get
let index = Arraye.index
let init = Arraye.init
let int_to_string = Arraye.int_to_string
let is_empty = Arraye.is_empty
let iter = Arraye.iter
let iteri = Arraye.iteri
let length = Arraye.length
let map = Arraye.map
let mapi = Arraye.mapi
let map2 = Arraye.map2
let mem = Arraye.mem
let of_array = Arraye.of_array
let of_arraye = Arraye.copy
let of_list = Arraye.of_list
let prependi = Arraye.prependi
let singleton = Arraye.singleton
let split = Arraye.split
let sub = Arraye.sub
let to_array = Arraye.to_array
let to_arraye = Arraye.copy
let to_list = Arraye.to_list
let to_string = Arraye.to_string
let fold = Arraye.fold
let fold_left = Arraye.fold_left
let fold_right = Arraye.fold_right
let ordered = Arraye.ordered
let exists = Arraye.exists

(**************************************************************)

(* If already ordered, then return input array.
 *)
let sort cmp a =
  if ordered cmp a then a else (
    let a = Arraye.copy a in
    Arraye.sort cmp a ;
    a
  )

(**************************************************************)

let fset a i x = 
  let a = Arraye.copy a in
  Arraye.set a i x ;
  a

(**************************************************************)

let max a =
  let len = length a in
  assert (len > 0) ;
  let max = ref (get a 0) in
  for i = 1 to pred len do
    let v = get a i in
    if v > !max then
      max := v
  done ;
  !max

let to_ranks a =
  let r = ref [] in
  for i = 0 to pred (length a) do
    if get a i then 
      r := i :: !r
  done ;
  !r

let of_ranks n l =
  let a = create n false in
  List.iter (fun i ->
    Arraye.set a i true
  ) l ;
  a

let gossip failed rank =
  let l = ref [] in
  for i = 0 to pred (length failed) do
    if (not (Arraye.get failed i)) && i <> rank then 
      l := i :: !l
  done ;
  of_list !l

let super a b =
  let la = length a in
  let lb = length b in
  if la <> lb then 
    failwith "super:mismatched lengths" ;
  let super = ref true in
  for i = 0 to pred la do
    if Arraye.get b i && not (Arraye.get a i) then
      super := false
  done ;
  !super

let min_false a =
  let j = ref (length a) in
  for i = pred (length a) downto 0 do
    if not (Arraye.get a i) then
      j := i
  done ;
  !j

let choose a k =
  let n = Arraye.length a in
  let k = Util.int_min n k in
  let rec loop k l =
    if k = 0 then l else (
      let i = Arraye.get a (Random.int n) in
      if List.mem i l then
      	loop k l
      else
      	loop (pred k) (i::l)
    )
  in loop k []

(**************************************************************)
(* This should be used with care, it is unsafe. 
*)
let to_array_break x = (Obj.magic x : 'a array)
let of_array_break x = (Obj.magic x : 'a t)

let of_array_map = Arraye.of_array_map
let to_array_map = Arraye.to_array_map
(**************************************************************)
