(**************************************************************)
(* LAYER.ML *)
(* Author: Mark Hayden, 4/95 *)
(**************************************************************)
open Util
open Tdefs
open Trans

(**************************************************************)
let name = Trace.file "LAYER"
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
  (* This comes first in order to enable some hacks.
   *)
  | Local_nohdr
  | Local_seqno of Trans.seqno

  | NoMsg
  | Local of 'local
  | Full of 'hdr * 'abv
  | Full_nohdr of 'abv

let string_of_msg local hdr abv = function
  | Local_nohdr -> "Local_nohdr"
  | Local_seqno seqno -> sprintf "Local_seqno(%d)" seqno
  | NoMsg -> "NoMsg"
  | Local a -> sprintf "Local(%s)" (local a)
  | Full(a,b) -> sprintf "Full(%s,%s)" (hdr a) (abv b)
  | Full_nohdr a -> sprintf "Full_nohdr(%s)" (abv a)

external local_seqno_coerce  : ('a,'b,'c)msg -> ('d,'e,'f)msg = "%identity"

let local_seqno_of_some = function
  | (Some _) as s -> (Obj.magic s : ('a,'b,'c)msg)
  | None -> failwith sanity

let option_of_local_seqno = function
  | (Local_seqno _) as s -> (Obj.magic s : Trans.seqno option)
  | _ -> failwith sanity
    
(**************************************************************)
(* Record containing overall fields for protocol stacks.
 * Assumes no layers are repeated.  
 *)
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
	       
let new_state interface = {
  interface     = interface ;
  exchange      = None ;
  switch	= ref None ;
  secchan       = ref [] ;
  tree          = ref Tree.zempty ;
  key_list      = ref [] ;
  dyn_tree      = ref Mrekey_dt.empty ;
  diam          = ref None ;
  dh_key        = ref None ;
  next_cleanup  = ref Time.zero
}
			    
let set_exchange exchange s = {
  interface     = s.interface ;
  exchange      = exchange ;
  switch	= s.switch ;
  secchan       = s.secchan ;
  tree          = s.tree ;
  key_list      = s.key_list ;
  dyn_tree      = s.dyn_tree ;
  diam          = s.diam ;
  dh_key        = s.dh_key ;
  next_cleanup  = s.next_cleanup
}

let reset_state s =
  s.switch := None

(**************************************************************)
(* Layers can specify some headers to optimize against.
 *)
type ('local,'hdr) optimize =
  | NoOpt
  | LocalNoHdr of 'local
  | FullNoHdr of 'hdr
  | LocalSeqno of 'hdr * Event.compact_typ * ('hdr -> Trans.seqno option) * (Trans.seqno option -> 'hdr)

(**************************************************************)

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
type ('local,'hdr,'state,'abv1,'abv2,'abv3) t = (('abv1,'abv2,'abv3) msg, ('local,'hdr,('abv1,'abv2,'abv3)msg) msg,'state) basic

(**************************************************************)
let compose_msg abv hdr = Full(hdr,abv)
let local_msg hdr = Local(hdr)
let no_msg () = NoMsg
(**************************************************************)

(* Layer conversion functions.
 *)

let hdr init hdlrs hdr_t opt args vs =
  (* This check allows us to use '==' below instead of '='.
   *)
  begin 
    match opt with
    | LocalNoHdr hdr ->
	if Obj.is_block (Obj.repr hdr) then
	  failwith "hdr_state:optimization attempted on block header"
    | FullNoHdr hdr ->
	if Obj.is_block (Obj.repr hdr) then
	  failwith "hdr_state:optimization attempted on block header"
    | LocalSeqno(hdr,_,_,_) ->
        if Obj.is_block (Obj.repr hdr) then
	  failwith "hdr_state:optimization attempted on block header"
    | _ -> ()
  end ;
    
  let state = init args vs in

  let l {up_lout=up;dn_lout=dn} =
    let {up_in=up;uplm_in=uplm;upnm_in=upnm;dn_in=dn;dnnm_in=dnnm} =
      let up        = up
      and upnm ev   = up ev NoMsg
      and dnnm ev   = dn ev NoMsg
      and dn =
	(* Check if we have a header to optimize against. 
	 *)
	match opt with
	| FullNoHdr nohdr ->
	    fun ev abv hdr ->
	      if hdr == nohdr then
		match abv with
		| Local_nohdr -> dn ev Local_nohdr
		| Local_seqno _ -> 
		    (* This is a hack to reuse the message generated above.
		     * The types are different, but it is ok to do this here.
		     *)
		    dn ev (local_seqno_coerce abv)
		| _ -> dn ev (Full_nohdr abv)
	      else		  
		dn ev (Full(hdr,abv))
	| LocalSeqno(nohdr,_,det,get) ->
	    fun ev abv hdr ->
	      if hdr == nohdr then (
		match abv with
		| Local_nohdr -> dn ev Local_nohdr
		| Local_seqno _ -> 
		    (* Same hack as above.
		     *)
		    dn ev (local_seqno_coerce abv)
		| _ -> dn ev (Full_nohdr abv)
	      ) else (
		match abv with
		| Local_nohdr -> (
		    match det hdr with
		    | (Some _) as s -> 
		      	dn ev (Obj.magic s : ('a,'b,'c) msg) (* Hack! *)
		    | None ->
		      	dn ev (Full(hdr,abv))
		  )
		| _ ->
		    dn ev (Full(hdr,abv))
	      )
	| _ -> 
	    fun ev abv hdr ->
	      dn ev (Full(hdr,abv))
      and dnlm =
	match opt with
	| LocalNoHdr nohdr ->
	    fun ev hdr ->
	      if hdr == nohdr then
		dn ev Local_nohdr
	      else dn ev (Local hdr) 
	| _ ->
	    fun ev hdr ->
	      dn ev (Local hdr)
      in
    
      hdlrs state vs {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} 
    in

    let up =
      match opt with
      |	FullNoHdr nohdr ->
	  (fun ev msg -> 
	    match msg with
	    | Local_seqno _  -> up ev (local_seqno_coerce msg) nohdr(* same hack again*)
	    | Local_nohdr    -> up ev Local_nohdr nohdr
	    | Full_nohdr abv -> up ev abv nohdr
	    | Local hdr      -> uplm ev     hdr
	    | Full(hdr,abv)  -> up   ev abv hdr
	    | NoMsg	     -> upnm ev)
      |	LocalNoHdr nohdr ->
	  (fun ev msg -> 
	    match msg with
	    | Local_nohdr   -> uplm ev nohdr
	    | Local hdr     -> uplm ev     hdr
	    | Full(hdr,abv) -> up   ev abv hdr
	    | NoMsg	    -> upnm ev
	    | _ -> failwith "hdr_state:LocalNoHdr:sanity")
      | LocalSeqno(nohdr,typ,_,cons) ->
	  (fun ev msg ->
	    match msg with
	    | Local_seqno _ -> 
		if Event.getCompactType ev == typ then
		  up ev Local_nohdr (cons (Obj.magic msg : int option)) (* Hack! *)
		else 
		  up ev (local_seqno_coerce msg) nohdr (* same hack yet again *)
	    | Local_nohdr   -> up ev Local_nohdr nohdr
	    | Full_nohdr abv -> up ev abv nohdr
	    | Local hdr     -> uplm ev     hdr
	    | Full(hdr,abv) -> up   ev abv hdr
	    | NoMsg	    -> upnm ev
(*
	    | _ -> failwith "hdr_state:LocalSeqno:sanity"
*))
      |	NoOpt ->
	  (fun ev msg -> 
	    match msg with
	    | Local hdr     -> uplm ev     hdr
	    | Full(hdr,abv) -> up   ev abv hdr
	    | NoMsg	    -> upnm ev
	    | _ -> failwith "hdr_state:NoOpt:sanity")
    and dn ev abv =
      match abv with
      | NoMsg -> dnnm ev
      | _ -> dn ev abv
    in

    {up_lin=up;dn_lin=dn}
  in (state,l)

(**************************************************************)
(* Layer conversion function with no optimizations.  This is
 * the same as the above, but without the header
 * optimizations.  
 *)

let hdr_noopt init hdlrs args vs =
  let state = init args vs in
  let l {up_lout=up;dn_lout=dn} =
    let up ev abv       = up ev abv
    and upnm ev	        = up ev NoMsg
    and dnnm ev         = dn ev NoMsg
    and dn ev abv hdr   = dn ev (Full(hdr,abv))
    and dnlm ev hdr     = dn ev (Local hdr)
    in
    
    let {up_in=up;uplm_in=uplm;upnm_in=upnm;dn_in=dn;dnnm_in=dnnm} =
      hdlrs state vs {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} 
    in

    let up ev msg = match msg with
    | Local hdr     -> uplm ev     hdr
    | Full(hdr,abv) -> up   ev abv hdr
    | NoMsg	    -> upnm ev
    | _ -> failwith "hdr_noopt:sanity"
    and dn ev abv = match abv with
    | NoMsg         -> dnnm ev
    | _             -> dn ev abv
    in

    {up_lin=up;dn_lin=dn}
  in (state,l)

(**************************************************************)
(* These are strings used in lots of layers, so we share
 * them all here.
 *)
let syncing = "SYNCING"
let block = "BLOCK"
let buffer = "BUFFER"
let unknown_local = "unknown local message"
let bad_header = "bad header"
let bad_up_event = "bad up event"
let layer_fail dump vf s debug =
  let failwith m = dump vf s ; Trace.make_failwith debug m in
  failwith
let layer_dump name f vf s =
  eprintf "%s:dump:%s\n" name (fst vf).View.name ;
  Array.iter (fun s -> eprintf "  %s" s) (f vf s)
let layer_dump_simp name = layer_dump name (fun _ _ -> [||])

(**************************************************************)
(* Layer management.  
 * 
 * We need to do an ugly cast here, so that all layers 
 * will look the same, although they actually all have different
 * headers. 
 *
 * Each layer installs itself in the table upon initialization
 * time. When stacks are built, the table is searched for layer
 * names. The layer table is of type:
 *   (string, ('a,'b,'c) basic) Hashtbl.t
 *)

(* A layer name is just a string.
*)
type name = string

let layer_table = 
  let (table : (string, ('a,'b,'c) basic) Hashtbl.t) = Hashtbl.create 47 in
  Trace.install_root (fun () ->
    [sprintf "ELINK:#layers=%d" (hashtbl_size table)]
  ) ;
  table

let rec(*no inline*) install name l = 
  let name = string_uppercase name in
  let l = (Obj.magic l : ('a,'b,'c) basic) in
  Hashtbl.add layer_table name l

let get name = 
  let name = string_uppercase name in
  let l = Hashtbl.find layer_table name in
  (Obj.magic l : ('a,'b,'c) basic)
  

let list () =
  eprintf "  ELINK:Installed Layers\n" ;
  Hashtbl.iter (fun name _ ->
    eprintf "	%s\n" name
  ) layer_table

(**************************************************************)
