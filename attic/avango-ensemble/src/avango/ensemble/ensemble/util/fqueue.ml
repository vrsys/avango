(**************************************************************)
(* FQUEUE.ML : functional queues *)
(* Author: Mark Hayden, 4/95 *)
(**************************************************************)
let name = Trace.file "FQUEUE"
let failwith s = Trace.make_failwith name s
(**************************************************************)
(*
type 'a t = 
  | Empty
  | Single of 'a
  | Full of 'a list(*hd*) * 'a list(*tl*)

let empty = Empty

let add x = function
  | Empty -> Single x
  | Single y -> Full([y;x],[])
  | Full(hd,tl) -> Full(hd,x::tl)

let is_empty = function
  | Empty | Full([],[]) -> true
  | _ -> false
      
let take = function
  | Empty -> failwith "empty queue"
  | Single x -> (x,Empty)
  | Full(a::hd,tl) -> (a,(Full(hd,tl)))
  | Full([],tl) ->
      match tl with
      |	[] -> failwith "empty queue"
      |	[a] -> (a,Empty)
      |	[b;a] -> (a,(Single b))
      |	tl -> 
	  let hd = List.rev tl in
	  (List.hd hd),(Full((List.tl hd),[]))

let peek = function
  | Empty -> failwith "empty queue"
  | Single a -> a
  | Full([],[]) -> failwith "empty queue"
  | Full(a::_,_) -> a
  | Full([],tl) ->
      List.nth tl (pred (List.length tl))
  
let length = function
  | Empty -> 0
  | Single _ -> 1
  | Full(hd,tl) ->
      List.length hd + List.length tl

let iter f = function
  | Empty -> ()
  | Single a -> f a
  | Full(hd,tl) ->
      List.iter f hd;
      List.iter f (List.rev tl)

let to_list = function
  | Empty -> []
  | Single a -> [a]
  | Full(hd,tl) ->
      hd @ List.rev tl

let of_list = function
  | [] -> Empty
  | [a] -> Single a
  | l -> Full(l,[])

let of_array a = 
  let l = Array.to_list a in
  of_list l

let rec fold f i = function
  | Empty -> i
  | Full([],[]) -> i
  | q ->
      let x,q = take q in
      fold f (f i x) q

let rec loop f i = function
  | Empty -> i
  | Full([],[]) -> i
  | Single x ->
      let i,q = f i Empty x in
      loop f i q
  | q ->
      let x,q = take q in
      let i,q = f i q x in
      loop f i q

let map f q =
  let l = to_list q in
  let l = List.map f l in
  let q = of_list l in
  q
*)

(* Slower version using only pairs of lists.
 *)
type 'a t = 
  { head : 'a list ;
    tail : 'a list }

let empty = { head = [] ; tail = [] }

let add x q = { head = q.head ; tail = x :: q.tail }

let is_empty q = 
  q.head = [] && q.tail = []

let take q =
  match q.head with
  | hd :: tl ->
      hd , { head = tl ; tail = q.tail }
  | [] ->
      match q.tail with
      | [] -> failwith "empty queue"
      | [a] -> (a,empty)
      | tl ->
 
      match List.rev q.tail with
      |	hd :: tl -> hd, { head = tl ; tail = [] }
      |	[] -> failwith "empty queue"

let peek q =
  match q.head with
  | hd :: _ -> hd
  | [] ->
      if q.tail = [] then 
	failwith "empty queue"
      else 
	List.nth q.tail (pred (List.length q.tail))
  
let length q =
  List.length q.head + List.length q.tail

let iter f q =
  List.iter f q.head ;
  List.iter f (List.rev q.tail)

let to_list q =
  q.head @ List.rev q.tail

let of_list l = 
  { head = l; tail = []}

let of_array a = 
  let l = Array.to_list a in
  of_list l

let map f q =
  let l = to_list q in
  let l = List.map f l in
  let q = of_list l in
  q

let rec fold f i q =
  if is_empty q then i else (
    let x,q = take q in
    fold f (f i x) q
  )      

let rec loop f i q =
  if is_empty q then i else (
    let x,q = take q in
    let i,q = f i q x in
    loop f i q
  )
