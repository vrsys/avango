(**************************************************************)
(* FRAG.ML *)
(* Author: Mark Hayden, 6/95 *)
(* There is something of a hack for the interaction between
 * this and the Mflow layer.  This layer marks all
 * fragmented messages with the Fragment flag.  This forces
 * the message to be subject to flow control even if the
 * event is not from the application.  Otherwise, the Mflow
 * layer may reorder application and non-application
 * fragments, causing problems. *)
(**************************************************************)
open Trans
open Buf
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "FRAG"
let failwith s = Trace.make_failwith name s
(**************************************************************)

type frag = {
  mutable iov : Iovecl.t array ;
  mutable i : int
}

(* The last fragment also has the protocol headers
 * for the layers above us.
 *)
type header = NoHdr 
  | Frag of seqno * seqno		(* ith of n fragments *)
  | Last of seqno			(* num of fragments *)

type state = {
  max_len	: len ;			(* maximum transmisstion size *)
  cast		: frag array ;		(* recv fragments for Cast's *)
  send		: frag array ;		(* recv fragments for Send's *)
  all_local     : bool ;                (* are all members local? *)
  local         : bool Arrayf.t		(* which members are local to my process *)
}

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "my_rank=%d, nmembers=%d\n" ls.rank ls.nmembers ;
  sprintf "view =%s\n" (View.to_string vs.view) ;
  sprintf "local=%s\n" (Arrayf.to_string string_of_bool s.local)
|])

let init _ (ls,vs) = 
  let local_frag = Param.bool vs.params "frag_local_frag" in
  let addr = ls.addr in
  let local = Arrayf.map (fun a -> (not local_frag) && Addr.same_process addr a) vs.address in
  let all_local = Arrayf.for_all ident local in
  let log = Trace.log2 name ls.name in
  { max_len = len_of_int (Param.int vs.params "frag_max_len") ;
    cast = Array.init ls.nmembers (fun _ -> {iov=[||];i=0}) ;
    send = Array.init ls.nmembers (fun _ -> {iov=[||];i=0}) ;
    all_local = all_local ;
    local = local
  }

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let log = Trace.log2 name ls.name in

  (* Handle the fragments as they arrive.
   *)
  let handle_frag iovl ev i n abv =
    (* First, flatten the iovec.  Normally, the iovec
     * should have only one element anyway.  
     *)
    let origin = getPeer ev in
    
    (* Select the fragment info to use.
     *)
    let c_type = getCompactType ev in
    let frag = 
      match c_type with 
	| C_ECast -> s.cast.(origin)
	| C_ESend -> s.send.(origin)
	| _ -> failwith "Sanity, trying to fragment a non Send/Cast message"
    in

    (* Check for out-of-order fragments: could send up lost
     * message when they come out of order.  
     *)
    if frag.i <> i then (
      log (fun () -> sprintf "expect=%d/%d, got=%d, type=%s\n" 
        frag.i (Array.length frag.iov) i (Event.to_string ev)) ;
      failwith "fragment arrived out of order" ;
    ) ;
    
    (* On first fragment, allocate the iovec array where
     * we will put the rest of the entries.
     *)
    if i = 0 then (
      if frag.iov <> [||] then failwith sanity ;
      frag.iov <- Array.create n Iovecl.empty
    ) ;
    if Array.length frag.iov <> n then
      failwith "bad frag array" ;
    
    (* Add fragment into array.
     *)
    if i >= Array.length frag.iov then
      failwith "frag index out of bounds" ;
    frag.iov.(i) <- iovl ;
    frag.i <- succ i ;
    
    (* On last fragment, send it up.  Note that the ack on
     * this should ack all previous fragments.
     *)
    match abv with
    | None ->
	if i = pred n then failwith sanity 
    | Some abv ->
    	if i <> pred n then failwith sanity ;
        let iov = Iovecl.concata (Arrayf.of_array frag.iov) in
      	frag.iov <- [||] ;
      	frag.i <- 0 ;
	match c_type with 
	  | C_ECast -> up (set_typ name ev (ECast iov)) abv
	  | C_ESend -> up (set_typ name ev (ESend iov)) abv
	  | _ -> failwith "Sanity, trying to fragment a non Send/Cast message"
  in

  let up_hdlr ev abv hdr = match getType ev, hdr with
    (* Common case: no fragmentation.
     *)
  | _, NoHdr -> up ev abv

  | (ECast iovl | ESend iovl), Last(n) ->
      handle_frag iovl ev (pred n) n (Some abv)
  | _, _     -> failwith bad_up_event

  and uplm_hdlr ev hdr = match getType ev, hdr with
  | (ECast iovl | ESend iovl ), Frag(i,n) ->
      handle_frag iovl ev i n None
  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | EExit ->
      (* GC all partially received fragments.
       *)
      let free_frag_array fa =
	Array.iter (fun f ->
	  Array.iter (fun i -> Iovecl.free i) f.iov ;
          f.iov <- [||]
	) fa
      in

      free_frag_array s.send ;
      free_frag_array s.cast ;
      upnm ev

  | _ -> upnm ev
  
  and dn_hdlr ev abv = match getType ev with
  | ECast iovl | ESend iovl ->
      (* Messages are not fragmented if either:
       * 1) They are small.
       * 2) They are tagged as unreliable. (BUG?)
       * 3) All their destinations are within this process.
       *)
      let lenl = Iovecl.len iovl in
      let c_type = getCompactType ev in
      if lenl <=|| s.max_len
      || (c_type = C_ECast && s.all_local)
      || (c_type = C_ESend && Arrayf.get s.local (getPeer ev))
      then (
	(* Common case: no fragmentation.
	 *)
        log (fun () -> sprintf "not-fragmenting") ;
      	dn ev abv NoHdr
      ) else (
        log (fun () -> sprintf "fragmenting") ;

	(* Fragment the message.
	 *)
	let frags = Iovecl.fragment s.max_len iovl in
	log (fun () -> sprintf "total_len=%d frags=%s" 
	  (int_of_len lenl)
	  (Arrayf.to_string 
	    (fun x -> string_of_int (int_of_len (Iovecl.len x))) frags));

	let nfrags = Arrayf.length frags in
	assert (nfrags >= 2) ;
	let frag_fun = match c_type with 
	  | C_ESend -> setSendIovFragment 
	  | C_ECast -> setCastIovFragment 
	  | _ -> failwith "Sanity, a non Send/Cast message"
	in
	for i = 0 to nfrags - 2 do
	  dnlm (frag_fun name ev (Arrayf.get frags i)) (Frag(i,nfrags)) ;
	done ;

	(* The last fragment has the header above us.
         *)
	dn (frag_fun name ev (Arrayf.get frags (pred nfrags))) abv (Last(nfrags)) ;

	(* Here the iovecl will be freed. 
	 *)
	Iovecl.free iovl
      )
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr init hdlrs None (FullNoHdr NoHdr) args vs
let l2 args vs = Layer.hdr_noopt init hdlrs args vs

let _ = 
  let frag_len = int_of_len (Buf.max_msg_len) - 2000 in
  Param.default "frag_max_len" (Param.Int frag_len) ;
  Param.default "frag_local_frag" (Param.Bool false) ;
  Layer.install name l
    
(**************************************************************)
