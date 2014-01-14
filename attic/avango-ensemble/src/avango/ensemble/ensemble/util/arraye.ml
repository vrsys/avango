(**************************************************************)
(* ARRAYE.ML : Non-floating point arrays *)
(* Author: Mark Hayden, 4/95 *)
(**************************************************************)

type 'a t = 'a array

let not_float o =
   Util.tag o <> Obj.double_tag

let create n item = 
  assert (not_float item) ;
  Array.create n item

(**************************************************************)
(* These are hacked to fool the typechecker into knowing the
 * array is not a float array.
 *)

let get a i = 
  Obj.magic ((Obj.magic a : string array).(i))
let set a i x = 
  (Obj.magic a : string array).(i) <- (Obj.magic x : string)
let unsafe_get a i = 
  Obj.magic (Array.unsafe_get (Obj.magic a : string array) i)
let unsafe_set a i x = 
  Array.unsafe_set (Obj.magic a : string array) i (Obj.magic x : string)

let length a = Array.length (Obj.magic a : string array)

(**************************************************************)

(*let copy = Array.copy*)
let append = Array.append
let concat = Array.concat
let flatten = Util.array_flatten
let doubleton a b =
  assert (not_float a) ;
  [|a;b|]
(*
let sub = Array.sub
let fill = Array.fill
let blit = Array.blit
*)
let flatten = Util.array_flatten
let incr = Util.array_incr
(*
let init = Array.init
let iter = Array.iter
let iteri = Array.iteri
let map = Array.map
let mapi = Array.mapi
*)
let to_list = Array.to_list
let of_list = Array.of_list
(*
let index = Util.array_index
let map2 = Util.array_map2
let for_all = Util.array_for_all
let for_all2 = Util.array_for_all2
let filter = Util.array_filter
let mem = Util.array_mem
*)
let is_empty a = length a = 0
let filter_nones = Util.array_filter_nones
let empty = [||]
(*
let combine = Util.array_combine
let split = Util.array_split
let to_array = Array.copy
let of_array = Array.copy
*)
let to_string = Util.string_of_array
let bool_to_string = to_string Util.string_of_bool
let int_to_string = to_string string_of_int
(*
let prependi = Util.array_prependi
let fold_left = Array.fold_left
let fold_right = Array.fold_right
let ordered = Util.array_ordered
let sort = Util.array_sort
let exists = Util.array_exists
*)

(* We do this because create_array has to make
 * an external call to a C function.  This is much
 * faster.
 *)
let singleton x =
  assert (not_float x) ;
  Obj.magic (ref x)
(*let singleton x = Array.create 1 x*)

(**************************************************************)
(* A bunch of these are included below rather than defined
 * in terms of Array or Util.  This is because the Array
 * and Util will do all the crazy float stuff.
 *)

let init l f =
  if l = 0 then [||] else
   let res = create l (f 0) in
   for i = 1 to pred l do
     unsafe_set res i (f i)
   done;
   res 

let copy a =
  let l = length a in
  if l = 0 then [||] else begin
    let res = create l (unsafe_get a 0) in
    for i = 1 to pred l do
      unsafe_set res i (unsafe_get a i)
    done;
    res
  end

let to_array = copy
let of_array = copy

let sub a ofs len =
  if ofs < 0 or len < 0 or ofs + len > length a then invalid_arg "Arraye.sub"
  else if len = 0 then [||]
  else begin
    let r = create len (unsafe_get a ofs) in
    for i = 1 to len - 1 do unsafe_set r i (unsafe_get a (ofs + i)) done;
    r
  end

let fill a ofs len v =
  if ofs < 0 or len < 0 or ofs + len > length a
  then invalid_arg "Arraye.fill"
  else for i = ofs to ofs + len - 1 do unsafe_set a i v done

let blit a1 ofs1 a2 ofs2 len =
  if len < 0 or ofs1 < 0 or ofs1 + len > length a1
             or ofs2 < 0 or ofs2 + len > length a2
  then invalid_arg "Arraye.blit"
  else if ofs1 < ofs2 then
    (* Top-down copy *)
    for i = len - 1 downto 0 do
      unsafe_set a2 (ofs2 + i) (unsafe_get a1 (ofs1 + i))
    done
  else
    (* Bottom-up copy *)
    for i = 0 to len - 1 do
      unsafe_set a2 (ofs2 + i) (unsafe_get a1 (ofs1 + i))
    done

let iter f a =
  for i = 0 to length a - 1 do f(unsafe_get a i) done

let map f a =
  let l = length a in
  if l = 0 then [||] else begin
    let r = create l (f(unsafe_get a 0)) in
    for i = 1 to l - 1 do
      unsafe_set r i (f(unsafe_get a i))
    done;
    r
  end


let of_array_map f a = 
  let l = Array.length a in
  if l = 0 then [||] else begin
    let r = create l (f(Array.unsafe_get a 0)) in
    for i = 1 to l - 1 do
      unsafe_set r i (f(Array.unsafe_get a i))
    done;
    r
  end

let to_array_map f a = 
  let l = length a in
  if l = 0 then [||] else begin
    let r = Array.create l (f(unsafe_get a 0)) in
    for i = 1 to l - 1 do
      Array.unsafe_set r i (f(unsafe_get a i))
    done;
    r
  end



let iteri f a =
  for i = 0 to length a - 1 do f i (unsafe_get a i) done

let mapi f a =
  let l = length a in
  if l = 0 then [||] else begin
    let r = create l (f 0 (unsafe_get a 0)) in
    for i = 1 to l - 1 do
      unsafe_set r i (f i (unsafe_get a i))
    done;
    r
  end

let fold f a =
  let l = length a in
  if l = 0 then
    invalid_arg "Arraye.fold" ;
  let r = ref (unsafe_get a 0) in
  for i = 1 to pred l do
    r := f !r (unsafe_get a i)
  done;
  !r

let fold_left f x a =
  let r = ref x in
  for i = 0 to length a - 1 do
    r := f !r (unsafe_get a i)
  done;
  !r

let fold_right f a x =
  let r = ref x in
  for i = length a - 1 downto 0 do
    r := f (unsafe_get a i) !r
  done;
  !r

(**************************************************************)

let prependi p a =
  let len = length a in
  let b = create (succ len) p in
  for i = 1 to len do
    set b i (get a (pred i))
  done ;
  b

let exists f v = 
  let r = ref false in 
  for i = 0 to pred (length v) do
    if f i (unsafe_get v i) then
      r := true
  done ;
  !r

let index it v =
  let rec loop i =
    if i >= length v then
      raise Not_found
    else if unsafe_get v i = it then
      i
    else loop (succ i)
  in loop 0

let mem it v =
  let rec loop i =
    if i < 0 then false
    else if unsafe_get v i = it then true
    else loop (pred i)
  in loop (pred (Array.length v))

(**************************************************************)
(**************************************************************)

let for_all f a =
  let rec loop i =
    if i < 0 then true
    else if f (unsafe_get a i) then loop (pred i)
    else false
  in loop (pred (length a))

(**************************************************************)

let for_all2 f a1 a2 =
  if length a1 <> length a2 then
    invalid_arg "Arraye.for_all2:mismatched arrays" ;
  let rec loop i =
    if i < 0 then true
    else if f (get a1 i) (get a2 i) then loop (pred i)
    else false
  in loop (pred (length a1))

(**************************************************************)

let map2 f a b =
  if length a <> length b then 
    invalid_arg "Arraye.map2:mismatched lengths" ;
  let l = length a in
  if l = 0 then [||] else (
    let c = create l (f (get a 0) (get b 0)) in
    for i = 1 to pred l do
      set c i (f (get a i) (get b i))
    done ;
    c
  )

(**************************************************************)

let filter f a =
  let len = length a in
  if len = 0 then [||] else (
    let b = create len (get a 0) in
    let j = ref 0 in
    for i = 0 to pred len do
      let it = get a i in
      if f it then (
	set b !j it ;
	Pervasives.incr j
      )
    done ;
    sub b 0 !j
  )

(**************************************************************)
(*
let ordered ge a =
  let len = length a in
  let ret = ref true in
  for i = 1 to pred len do
    if not (ge (get a i) (get a (pred i))) then (
      ret := false
    )
  done ;
  !ret
*)
let ordered cmp a =
  let len = length a in
  let ret = ref true in
  for i = 1 to pred len do
    if cmp (get a (pred i)) (get a i) > 0  then (
      ret := false
    )
  done ;
  !ret

(*
let array_rev_inplace a =
  let l = length a in
  for i = 0 to pred (l / 2) do
    let j = l - i - 1 in
    assert (i < j) ;
    let ai = get a i in
    let aj = get a j in
    set a i aj ;
    set a j ai
  done

let array_rev a =
  let a = copy a in
  array_rev_inplace a

(* In-place O(n^2) sort.
 *)
let insertion_sort ge a =
  let len = length a in
  for i = 0 to pred len do
    let minp = ref i in
    let targ = get a i in
    let min = ref targ in
    for j = succ i to pred len do
      let this = get a j in
      if ge !min this then (
	minp := j ;
	min := this
      )
    done ;
    if !minp <> i then (
      set a !minp targ ;
      set a i !min
    )
  done

(* In-place O(nlogn) sort.
 *)
let sort ge a =
  let len = length a in
  if len < 10 then (
    insertion_sort ge a
  ) else (
    let l = to_list a in
    let l = Sort.list ge l in
    let b = of_list l in
    let b = array_rev b in
    blit b 0 a 0 len
  )

(* Use the Ocaml sort routine.  Note that it
 * wants an "less than or equal" comparison
 * operator.
 *)
let sort ge a =
  Sort.array ge a ;
  array_rev_inplace a ;
  assert (ordered ge a)
*)
let sort cmp a =
  Array.sort cmp a ;
  assert (ordered cmp a)

(**************************************************************)

let combine a b =
  let l = length a in
  if l <> length b then
    invalid_arg "Arraye.combine" ;
  init l (fun i -> ((get a i),(get b i)))

let split c =
  let a = map fst c in
  let b = map snd c in
  (a,b)

(**************************************************************)
