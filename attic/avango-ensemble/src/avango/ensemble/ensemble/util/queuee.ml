
(* Note: this file is modified from the Caml Light 7.1
 * distribution.  Does the above copyright apply?
 *)

type 'a queue_cell =
    Nil
  | Cons of 'a * 'a queue_cell ref

type 'a t =
  { mutable head: 'a queue_cell;
    mutable tail: 'a queue_cell }

let create () =
  { head = Nil; tail = Nil }

let clear q =
  q.head <- Nil; q.tail <- Nil

let add x = function
    { tail = Nil } as q ->    (* if tail = Nil then head = Nil *)
      let c = Cons(x, ref Nil) in
        q.head <- c; q.tail <- c
  | { tail = Cons(_, newtail) } as q ->
      let c = Cons(x, ref Nil) in
        newtail := c; q.tail <- c

let peek q =
  match q.head with
    Nil ->
      failwith "QUEUEE:peek:empty queue"
  | Cons(x,_) ->
      x

let empty = function
| {head = Nil} -> true
| _ -> false

let take q =
  match q.head with
    Nil ->
      failwith "QUEUEE:take:empty queue"
  | Cons(x, rest) ->
      q.head <- !rest;
      begin match !rest with
        Nil -> q.tail <- Nil
      |  _  -> ()
      end;
      x

let takeopt q =
  match q.head with
    Nil ->
      None
  | Cons(x, rest) ->
      q.head <- !rest;
      begin match !rest with
        Nil -> q.tail <- Nil
      |  _  -> ()
      end;
      Some x

let rec length_aux = function
    Nil -> 0
  | Cons(_, rest) -> succ (length_aux !rest)

let length q = length_aux q.head

let rec iter_aux f = function
    Nil ->
      ()
  | Cons(x, rest) ->
      f x; iter_aux f !rest

let iter f q = iter_aux f q.head

let to_list q =
  let l = ref [] in
  iter (fun it ->
    l := it :: !l
  ) q ;
  List.rev !l

let to_array q =
  let len = length q in
  if len = 0 then (
    [||]
  ) else (
    let a = Array.create len (take q) in
    (* Don't forget to skip the first one.
     *)
    for i = 1 to pred len do
      a.(i) <- take q
    done ;
    a
  )

let rec clean f q =
  match takeopt q with
  | None -> ()
  | Some v -> f v ; clean f q

let transfer q0 q1 =
  while not (empty q1) do
    let v = take q1 in
    add v q0
  done
