(**************************************************************)
(* MCAUSAL.ML : Causal ordering support module *)
(* Author: Ohad Rodeh, 7/97 *)
(*************************************************************************)
open Trans
(*************************************************************************)
(* Module definition.
 *)
module type S = sig
  type t
  type s
      
  val create_state  : int -> t
  val last_of       : t -> seqno array
  val can_deliver   : t -> s -> rank -> bool
  val incr          : t -> rank -> s
  val update        : t -> s -> rank -> unit
end
(**************************************************************)
(* Regular support functions. 
 * No compression, send full acknowledgment vectors. 
 *)
module Regular : S = struct
type t = {
    rlast : seqno array  (* Caually received seqno per member *)
  } 

type s = seqno array     (* Acknowledgment array *)

let create_state nmem = {
  rlast = Array.create nmem 0
} 

let last_of t = t.rlast

(* Is message (org,cs.last.(org)) with acknowledges for ack_vector
 * deliverable?
*)
let can_deliver t ack_vct org = 
  let rec loop i = 
    if (i = (Array.length t.rlast)) then 
      true
    else 
     if i = org then 
       loop (succ i) 
     else
       if (t.rlast.(i) < ack_vct.(i)) then
	 false
       else
	 loop (succ i) 
  in
  loop 0 


(* Increment your own counter. Retrun array of acknowledgments to send.
 *)
let incr t org = 
  t.rlast.(org) <- succ t.rlast.(org);
  Array.copy t.rlast
		    
(* Received a message. Update the state. 
   *)
let update t ack_vct org = 
  t.rlast.(org) <- succ t.rlast.(org)
end

  
(*************************************************************************)
(* Compression support functions
 *)
module Compress : S = struct
type t = {
    clast  : seqno array;		(* Caually received seqno per member *)
    mutable roots : (rank * seqno) list	(* Roots of the dag. *)
  }

type s = (rank * seqno) array		(* Acknowledgment array *)

let create_state nmem = {
  clast = Array.create nmem 0;
  roots = []
} 

let last_of t = t.clast

(* Is message (org,t.last.(org)) with acknowledges for ack_list
 * deliverable?
 *)
let can_deliver t ack_list org = 
  let can = ref true in
  Array.iter (fun (r,seq) -> 
    if t.clast.(r) < seq then 
      can := false
  ) ack_list;
  !can

(* Increment your own counter and set the root list. Retrun a list of 
 * acknowledgments to send.
 *)
let incr t org = 
  (* Remove myself from the root list being sent.
   *)
  let old_roots = ref [] in
  List.iter (fun (r,seq) -> 
    if (r <> org) then 
      old_roots := !old_roots @ [(r,seq)]
  ) t.roots ;

  (* Reset root list.
   *)
  t.roots <- [(org, t.clast.(org))] ;
  t.clast.(org) <- succ t.clast.(org) ;
  Array.of_list !old_roots


(* Received a message. Update the roots and the vector
 * of last messages. 
 *)
let update t ack_list org =
  let ack_list = Array.to_list ack_list in
  (* Remove the ack_list from the roots, for example:
   * exclude [(0,102);(1,103);(2,104)] from roots=[(1,104);(2,103)] 
   * results in: roots = [(1,104)]
   *)
  let exclude ack_list =
      let rec loop l roots_new roots_old =
      	if l = [] || roots_old = [] then 
      	  roots_new @ roots_old
	else
      	  begin
      	    let (r1, seq1) = List.hd l
     	    and (r2, seq2) = List.hd roots_old in 
     	    if r1 = r2 then (
	      if (seq1 >= seq2) then 
	      	loop (List.tl l) roots_new (List.tl roots_old)
	      else
	      	loop (List.tl l) (roots_new @ [(r2,seq2)]) (List.tl roots_old)
	    ) else (
	      if r1 > r2 then
	      	loop (List.tl l) (roots_new @ [(r2,seq2)]) (List.tl roots_old)
	      else
	      	loop (List.tl l) roots_new roots_old
	    )
	   end
	  in
      t.roots <- loop ack_list [] t.roots

    (* Add a message to the roots:
     * add_root (2,101) to roots=[(0,100);(1,117);
     * results in: roots = [(0,100);(1,117);(2,101)]
     * add_root (2,101) to roots=[(0,100);(1,117);(2,90)];
     * results in: roots = [(0,100);(1,117);(2,101)]
     *) 
    and add_root (r,seq) = 
      let rec loop l1 l2 =
      	if l2 = [] then (
	  l1 @ [(r,seq)]
	) else (
	    let (r2,seq2) = List.hd l2 in
	    if r2 < r then
	      loop (l1 @ [(r2,seq2)]) (List.tl l2)
	    else if r2 = r then 
              l1 @ [(r,seq)] @ (List.tl l2)
	    else
              l1 @ [(r,seq)] @ l2
        )
      in 
      t.roots <- loop [] t.roots
    in
  
    t.clast.(org) <- succ t.clast.(org);
    exclude ack_list;
    add_root (org,t.clast.(org))
end

(**************************************************************)

module Export = Regular

type t = Export.t
type s = Export.s

let create_state = Export.create_state  
let last_of      = Export.last_of       
let can_deliver  = Export.can_deliver   
let incr         = Export.incr
let update       = Export.update        

(**************************************************************)
