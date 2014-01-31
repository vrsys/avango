(**************************************************************)
(* SIGNED router *)
(* Author: Mark Hayden, 3/97 *)
(* Rewritten by Ohad Rodeh 10/2001 *)
(**************************************************************)

val f : unit -> 
  (Trans.rank -> Obj.t option -> Trans.seqno -> Iovecl.t -> unit) Route.t
