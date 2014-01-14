(**************************************************************)
(* LAYER.MLI *)
(* Author: Mark Hayden, 4/95 *)
(**************************************************************)
open Trans
open Tdefs

(**************************************************************)

type 'a saved = 'a option ref

(**************************************************************)

type ('a,'b,'c) handlers_out = {
  up_out	: Event.up -> 'c -> unit ;
  upnm_out	: Event.up -> unit ;
  dn_out 	: Event.dn -> 'c -> 'b -> unit ;
  dnlm_out	: Event.dn -> 'a -> unit ;
  dnnm_out	: Event.dn -> unit
}

type ('a,'b,'c) handlers_in = {
  up_in 	: Event.up -> 'c -> 'b -> unit ;
  uplm_in 	: Event.up -> 'a -> unit ;
  upnm_in 	: Event.up -> unit ;	
  dn_in 	: Event.dn -> 'c -> unit ;
  dnnm_in 	: Event.dn -> unit
}

type ('local,'hdr,'abv) full =
  ('local,'hdr,'abv) handlers_out ->
  ('local,'hdr,'abv) handlers_in

(**************************************************************)
(* What messages look like in this architecture.
 *)
type ('local,'hdr,'abv) msg =
  (* These come first in order to enable some hacks.
   *)
  | Local_nohdr
  | Local_seqno of Trans.seqno

  | NoMsg
  | Local of 'local
  | Full of 'hdr * 'abv
  | Full_nohdr of 'abv

val string_of_msg :
  ('local -> string) -> ('hdr -> string) -> ('abv -> string) ->
    ('local,'hdr,'abv) msg -> string

(**************************************************************)

type ('local,'hdr) optimize =
  | NoOpt
  | LocalNoHdr of 'local
  | FullNoHdr of 'hdr
  | LocalSeqno of 'hdr * Event.compact_typ * ('hdr -> Trans.seqno option) * (Trans.seqno option -> 'hdr)

(**************************************************************)
(* Message composition functions (for bypass code).
 *)
val compose_msg : 'abv -> 'hdr -> ('local,'hdr,'abv) msg
val local_msg : 'local -> ('local,'hdr,'abv) msg
val no_msg : unit -> ('local,'hdr,'abv) msg

(**************************************************************)

type state = {
  interface        : Appl_intf.New.t ;
  switch	   : Time.t saved ;
  exchange         : (Addr.set -> bool) option ;
  secchan          : (Endpt.id * Security.cipher) list ref ; (* State for SECCHAN *)
  tree             : Tree.z ref ;          (* State for OPTREKEY *)
  key_list         : (Key.t * Security.cipher option) list ref ; (* State for REALKEYS *)
  dyn_tree         : Mrekey_dt.t ref ;   (* State for REKEY_DT *)
  diam             : Diamond.appl_state option ref ; (* State for REKEY_DIAM *)
  dh_key           : Shared.DH.key option ref ;
  next_cleanup     : Time.t ref
}

val new_state : Appl_intf.New.t -> state

val set_exchange : (Addr.set -> bool) option -> state -> state

val reset_state : state -> unit

(**************************************************************)
(* Type of exported layers. (Exported for bypass code.)
 *)

type ('bel,'abv) handlers_lout = {
  up_lout	: Event.up -> 'bel -> unit ;
  dn_lout 	: Event.dn -> 'abv -> unit
}

type ('bel,'abv) handlers_lin  = {
  up_lin 	: Event.up -> 'abv -> unit ;
  dn_lin 	: Event.dn -> 'bel -> unit
}

type ('bel,'abv,'state) basic =
  state -> View.full -> 'state * (('bel,'abv) handlers_lout -> ('bel,'abv) handlers_lin)

(**************************************************************)
(* Alias for common layers in this architecture.
 *)

type ('local,'hdr,'state,'abv1,'abv2,'abv3) t = 
  (('abv1,'abv2,'abv3) msg, ('local,'hdr,('abv1,'abv2,'abv3)msg) msg,'state) basic

val hdr :
  (state -> View.full -> 'state) ->
  ('state -> View.full -> ('local,'hdr,('abv1,'abv2,'abv3)msg) full) ->
  (*Typedescr.t*)int option ->
  ('local,'hdr) optimize ->
  ('local,'hdr,'state,'abv1,'abv2,'abv3) t

val hdr_noopt :
  (state -> View.full -> 'state) ->
  ('state -> View.full -> ('local,'hdr,('abv1,'abv2,'abv3)msg) full) ->
  ('local,'hdr,'state,'abv1,'abv2,'abv3) t

(**************************************************************)
(* These are strings used in lots of layers, so we share
 * them all here.
 *)
val syncing : string
val block : string
val buffer : string
val unknown_local : string
val bad_header : string
val bad_up_event : string
val layer_fail : (View.full -> 'a -> unit) -> View.full -> 'a -> debug -> string -> 'b
val layer_dump : debug -> (View.full -> 'a -> string array) -> View.full -> 'a -> unit
val layer_dump_simp : debug -> View.full -> 'a -> unit
(**************************************************************)
(* Layer management.
 *
 * [OR]: This has been moved back here, away from the elink 
 * module. I think dynamic linking should be done by the caml
 * folk, not us. 
 *)
type name = string

val install : name -> ('a,'b,'c) basic -> unit
val get : name -> ('a,'b,'c) basic

(**************************************************************)

