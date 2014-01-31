(**************************************************************)
(* IQ.MLI *)
(* Author: Mark Hayden, 4/95 *)
(**************************************************************)
open Trans
(**************************************************************)
(*
               	    lo       	read   	  hi
                   \/          \/        \/
     [][][][][][][][][][][][][][][][][][][][][][][]
     0<-- reset -->                      <---- unset ...

*)
(**************************************************************)

type 'a t

type 'a get =
  | GData of Iovecl.t * 'a
  | GUnset
  | GReset

val create	: debug -> 'a -> 'a t
val grow	: 'a t -> int -> unit

val lo          : 'a t -> seqno
val hi  	: 'a t -> seqno
val set_lo	: 'a t -> seqno -> unit
val set_hi     	: 'a t -> seqno -> unit

(* ASSIGN: if this returns true then the slot was empty
 * and the Iovecl ref was appropriately incremented.
 *)
val assign		: 'a t -> seqno -> Iovecl.t -> 'a -> bool
val msg_update          : 'a t -> seqno ->'a -> bool
val opt_insert_check_doread : 'a t -> seqno -> Iovecl.t -> 'a -> bool

(* Split the above into two pieces.
 *)
val opt_insert_check    : 'a t -> seqno -> bool
val opt_insert_doread   : 'a t -> seqno -> Iovecl.t -> 'a -> 'a t

(* Add returns the Iq.  This is done for the functional
 * optimizations.
 *)
val add			: 'a t -> Iovecl.t -> 'a -> 'a t

val get                 : 'a t -> seqno -> 'a get
val get_prefix		: 'a t -> (seqno -> Iovecl.t -> 'a -> unit) -> unit
val list_of_iq		: 'a t -> (seqno * Iovecl.t * 'a) list
val list_of_iq_interval : 'a t -> (seqno * seqno) -> (seqno * Iovecl.t * 'a) list
val hole		: 'a t -> (seqno * seqno) option

val read		: 'a t -> seqno
val read_hole		: 'a t -> (seqno * seqno) option
val read_prefix		: 'a t -> (seqno -> Iovecl.t -> 'a -> unit) -> unit

(* The opt_update functions are used where the normal
 * case insertion does not actually insert anything
 * into the buffer.
 *)
val opt_update_old	: 'a t -> seqno -> bool

(* Split the above into two pieces.
 *)
val opt_update_check	: 'a t -> seqno -> bool
val opt_update_update	: 'a t -> seqno -> 'a t

val free                : 'a t -> unit

(* Clear entries that haven't been read yet.
 *)
val clear_unread        : 'a t -> unit

(**************************************************************)
