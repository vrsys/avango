(**************************************************************)
(* SCHED.ML: call-back scheduling *)
(* Author: Mark Hayden, 12/95 *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "SCHED"
let failwith = Trace.make_failwith name
(**************************************************************)

(* Count of # of scheduling steps so far.
 *)
let steps = ref 0

(**************************************************************)
(*
(* ITEM: Always visible
 *)
type item =
| Bottom of unit
| Arg0 of (unit -> unit)
| Arg1 of (Obj.t -> unit) *
    Obj.t
| Arg2 of (Obj.t -> Obj.t -> unit) *
    Obj.t * Obj.t
| Arg3 of (Obj.t -> Obj.t -> Obj.t -> unit) *
    Obj.t * Obj.t * Obj.t
| Arg4 of (Obj.t -> Obj.t -> Obj.t -> Obj.t -> unit) *
    Obj.t * Obj.t * Obj.t * Obj.t


(* TYPED_ITEM: Always visible
 *)
type ('a,'b,'c,'d) typed_item =
| T_Bottom of unit
| T_Arg0 of (unit -> unit)
| T_Arg1 of ('a -> unit) 			* 'a
| T_Arg2 of ('a -> 'b -> unit) 			* 'a * 'b
| T_Arg3 of ('a -> 'b -> 'c -> unit) 		* 'a * 'b * 'c
| T_Arg4 of ('a -> 'b -> 'c -> 'd -> unit) 	* 'a * 'b * 'c * 'd
*)
(**************************************************************)
(*
type t = (unit -> unit) Queuee.t

let create _ = Queuee.create ()
and enqueue s debug f =
  Queuee.add f s
and enqueue_1arg s debug f a =
  Queuee.add (fun () -> f a) s
and enqueue_2arg s debug f a b =
  Queuee.add (fun () -> f a b) s
and enqueue_3arg s debug f a b c =
  Queuee.add (fun () -> f a b c) s
and enqueue_4arg s debug f a b c d =
  Queuee.add (fun () -> f a b c d) s
and empty = queue_empty
and size = Queuee.length
and step s n =
  try
    for i = 1 to n do
      (Queuee.take s) ()
    done ;
    true
  with Queuee.Empty -> false
*)
(**************************************************************)
(*
type t = item Queuee.t

let create _ = Queuee.create ()
let enqueue s debug f =
  Queuee.add (Arg0(f)) s
let enqueue_1arg s debug f a =
  Queuee.add (Obj.magic (Obj.repr (T_Arg1(f,a)))) s
let enqueue_2arg s debug f a b =
  Queuee.add (Obj.magic (Obj.repr (T_Arg2(f,a,b)))) s (*MEM:alloc*)
let enqueue_3arg s debug f a b c =
  Queuee.add (Obj.magic (Obj.repr (T_Arg3(f,a,b,c)))) s
let enqueue_4arg s debug f a b c d =
  Queuee.add (Obj.magic (Obj.repr (T_Arg4(f,a,b,c,d)))) s
let empty = Queuee.empty
let size = Queuee.length

let step s n =				(*PERF*)
  if Queuee.empty s then (
    false
  ) else (
    (* MH: I've verified that this ref does not cause allocation
     * when compiled as native code.
     *)
    let n = ref n in
    while !n > 0 && (not (Queuee.empty s)) do
      begin
	match Queuee.take s with
	| Arg0(f) 	  -> f ()
	| Arg1(f,a) 	  -> f a
	| Arg2(f,a,b)     -> f a b
	| Arg3(f,a,b,c)   -> f a b c
	| Arg4(f,a,b,c,d) -> f a b c d
        | Bottom() -> failwith "bottom"
      end ;
      decr n
    done ;
    true
  )
*)
(**************************************************************)
(* This version is optimized for scheduling 2 argument
 * functions and uses the new Queuea module.
 *)

type item = {
  mutable f : Obj.t -> Obj.t -> unit ;
  mutable a : Obj.t ;
  mutable b : Obj.t
}

let bottomfun = Obj.magic (fun _ _ -> failwith "bottom")
let bottomnew = Obj.magic (fun _ _ -> failwith "bottomnew")
let bottomarg = Obj.repr ()

(* Create a new record for the queue.
 *)
let bottom () = {
  f = bottomfun ;
  a = bottomarg ;
  b = bottomarg
}

(* Clear all the fields in an item.  Before, I tried to hack
 * this so that 'modify' would not get called, but there
 * appears to be a problem with that approach: it caused
 * segfaults, I guess this was because of some strange GC
 * requirement that you darken old objects that are being
 * written over.  
 *)

(* I still do try the hack of checking for physical equality
 * before assigning.  This will often work out true for 
 * the 2nd argument.
 *)
let bottomize i =
  i.f <- bottomfun ;
  i.a <- Obj.repr () ;
  if not (i.b == Obj.repr ()) then i.b <- Obj.repr ()

type t = item Queuea.t

let helper f a = f a

let c1 = counter () 
let c2 = counter ()

let create _ = Queuea.create_mut bottom
let enqueue_2arg s debug (f : 'a->'b->unit) (a : 'a) (b : 'b) =
(*  eprintf "enqueuing: %s %d\n" debug (c1()) ;*)
  let item = Queuea.add_mut s in
  item.f <- Obj.magic f ;
  item.a <- Obj.repr a ; 
  (*if not (item.b == Obj.repr b) then*) item.b <- Obj.repr b

let enqueue      s debug f           = enqueue_2arg s debug helper f ()
let enqueue_1arg s debug f a         = enqueue_2arg s debug helper f a
let enqueue_3arg s debug f a b c     = enqueue s debug (fun () -> f a b c)
let enqueue_4arg s debug f a b c d   = enqueue s debug (fun () -> f a b c d)
let enqueue_5arg s debug f a b c d e = enqueue s debug (fun () -> f a b c d e)
let empty = Queuea.empty
let size = Queuea.length

let step s n =
  let i = ref n in
  while (not (Queuea.empty s)) && !i > 0 do
    let item = Queuea.take_mut_unsafe s in
    let {f=f;a=a;b=b} = item in
    (* Note: need to zonk the fields immediately before we loop
     * around.
     *)
    bottomize item ;
(*
    eprintf "step1 %d\n" (c2());
*)
    f a b ;
(*
    eprintf "step3\n" ;
    Gc.full_major () ;
    eprintf "step2\n" ;
*)
    decr i
  done ;
  !i < n

(**************************************************************)
(*
type t = {
  q 		: item Queuee.t ;
  name 		: string
}

let create name = {
    q 	 = Queuee.create () ;
    name = name
  }

let enqueue s debug f            = check s ; Queuee.add (Arg0(f)) s.q
and enqueue_1arg s debug f a     = check s ; Queuee.add (Arg1(Obj.magic (Obj.repr f), (Obj.repr a))) s.q
and enqueue_2arg s debug f a b   = check s ; Queuee.add (Arg2((Obj.magic (Obj.repr f)), (Obj.repr a), (Obj.repr b))) s.q
and enqueue_3arg s debug f a b c = check s ; Queuee.add (Arg3((Obj.magic (Obj.repr f)), (Obj.repr a), (Obj.repr b), (Obj.repr c))) s.q
and enqueue_4arg s debug f a b c d = check s ; Queuee.add (Arg4((Obj.magic (Obj.repr f)), (Obj.repr a), (Obj.repr b), (Obj.repr c), (Obj.repr c))) s.q
and empty s  = Queuee.empty s.q
and size s   = Queuee.length s.q
and step s n =
  try
    for i = 1 to n do
      match Queuee.take s.q with
      | Arg0(f) -> f ()
      | Arg1(f,a) -> f a
      | Arg2(f,a,b) -> f a b
      | Arg3(f,a,b,c) -> f a b c
      | Arg4(f,a,b,c,d) -> f a b c d
    done ; true
  with Queue.Empty -> false		(*BUG*)
*)
(**************************************************************)
