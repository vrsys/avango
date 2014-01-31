(***********************************************************************)
(*                                                                     *)
(*                           Objective Caml                            *)
(*                                                                     *)
(*            Xavier Leroy, projet Cristal, INRIA Rocquencourt         *)
(*                                                                     *)
(*  Copyright 1996 Institut National de Recherche en Informatique et   *)
(*  Automatique.  Distributed only by permission.                      *)
(*                                                                     *)
(***********************************************************************)
open Printf

(* Hash tables *)

(*
let info = Hashtbl.create 7
*)

external hash_param : int -> int -> 'a -> int = "hash_univ_param" "noalloc"

let hash x = hash_param 10 100 x

(* We do dynamic hashing, and resize the table and rehash the elements
   when buckets become too long. *)

type ('a, 'b) t =
  { ctr: int ref;
    name: string;
    mutable max_len: int;                     (* max length of a bucket *)
    mutable data: ('a, 'b) bucketlist array } (* the buckets *)

and ('a, 'b) bucketlist =
    Empty
  | Cons of 'a * 'b * ('a, 'b) bucketlist

let cnt = ref 0
let trace t name =
(*
  incr cnt ;
  incr t.ctr ;
  if (!cnt mod 1000) = 0 then (
    eprintf "HASHTBLE:%07d:%s:%s\n" !cnt name t.name ;
    Hashtbl.iter (fun name ctr ->
      eprintf "  %s:%07d\n" name !ctr
    ) info ;
    flush stderr
  )
*)()

let create name initial_size =
(*
  eprintf "Hashtble.create:name=%s:size=%d\n" name initial_size ;
*)
  let ctr = cnt in
(*
  let ctr = 
    try Hashtbl.find info name 
    with Not_found ->
      let ctr = ref 0 in
      Hashtbl.add info name ctr ;
      ctr
  in
*)
 
  if initial_size <= 0 then invalid_arg "Hashtbl.create" else
  { ctr = ctr ; name = name; max_len = 3; data = Array.create initial_size Empty }

let clear h =
  for i = 0 to Array.length h.data - 1 do
    h.data.(i) <- Empty
  done

let resize hashfun tbl =
  let odata = tbl.data in
  let osize = Array.length odata in
  let nsize = 2 * osize + 1 in
  let ndata = Array.create nsize Empty in
  let rec insert_bucket = function
      Empty -> ()
    | Cons(key, data, rest) ->
        insert_bucket rest; (* preserve original order of elements *)
        let nidx = (hashfun key) mod nsize in
        ndata.(nidx) <- Cons(key, data, ndata.(nidx)) in
  for i = 0 to osize - 1 do
    insert_bucket odata.(i)
  done;
  tbl.data <- ndata;
  tbl.max_len <- 2 * tbl.max_len
          
let rec bucket_too_long n bucket =
  if n < 0 then true else
    match bucket with
      Empty -> false
    | Cons(_,_,rest) -> bucket_too_long (n - 1) rest

let add h key info =
  trace h "add" ;
  let i = (hash key) mod (Array.length h.data) in
  let bucket = Cons(key, info, h.data.(i)) in
  h.data.(i) <- bucket;
  if bucket_too_long h.max_len bucket then resize hash h

let remove h key =
  trace h "remove" ;
  let rec remove_bucket = function
      Empty ->
        Empty
    | Cons(k, i, next) ->
        if k = key then next else Cons(k, i, remove_bucket next) in
  let i = (hash key) mod (Array.length h.data) in
  h.data.(i) <- remove_bucket h.data.(i)

let find h key =
  trace h "find" ;
  match h.data.((hash key) mod (Array.length h.data)) with
    Empty -> raise Not_found
  | Cons(k1, d1, rest1) ->
      if key = k1 then d1 else
      match rest1 with
        Empty -> raise Not_found
      | Cons(k2, d2, rest2) ->
          if key = k2 then d2 else
          match rest2 with
            Empty -> raise Not_found
          | Cons(k3, d3, rest3) ->
(*
	      eprintf "HASHTBLE:deepbucket:%06d:%d:%s\n" ((hash key) mod (Array.length h.data)) h.max_len h.name ; flush stderr ;
*)
              if key = k3 then d3 else begin
                let rec find = function
                    Empty ->
                      raise Not_found
                  | Cons(k, d, rest) ->
                      if key = k then d else find rest
                in find rest3
              end

let find_all h key =
  let rec find_in_bucket = function
    Empty ->
      []
  | Cons(k, d, rest) ->
      if k = key then d :: find_in_bucket rest else find_in_bucket rest in
  find_in_bucket h.data.((hash key) mod (Array.length h.data))

let iter f h =
  let rec do_bucket = function
      Empty ->
        ()
    | Cons(k, d, rest) ->
        f k d; do_bucket rest in
  let d = h.data in
  for i = 0 to Array.length d - 1 do
    do_bucket d.(i)
  done

let rec bucket_length = function
  | Empty -> 0
  | Cons(_,_,rest) -> succ (bucket_length rest)

let info h =
  let t = Array.map bucket_length h.data in
  let max = ref 0 in
  Array.iter (fun len -> if len > !max then max := len) t ;
  let max = succ !max in
  let l = Array.create max 0 in
  Array.iter (fun len -> l.(len) <- succ l.(len)) t ;
  let l = Array.map string_of_int l in
  let l = Array.to_list l in
  let l = String.concat ";" l in
  sprintf "[%s]" l
  
    
(**************************************************************)

let size t =
  let n = ref 0 in
  iter (fun _ _ -> incr n) t ;
  !n

(**************************************************************)

let mem t k =
  try find t k ; true with
    Not_found -> false

(**************************************************************)

let to_list h =
  let l = ref [] in
  iter (fun k d ->
    l := (k,d) :: !l
  ) h ;
  !l

(**************************************************************)

let clean h =
  try
    iter (fun k _ ->
      remove h k
    ) h
  with Not_found -> 
    failwith "clean:Not_found"

(**************************************************************)
