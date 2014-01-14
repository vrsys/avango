(**************************************************************)
(* FRAG_ABV.ML *)
(* Author: Ohad Rodeh, 10/2000 *)
(* Fragment large non-application messages. This allows
 * large groups with 60+ members, where the view state 
 * becomes large. *)
(**************************************************************)
open Trans
open Buf
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "FRAG_ABV"
let failwith = Trace.make_failwith name
let log = Trace.log name 
let log2 = Trace.log (name^"2")
(**************************************************************)

type frag = {
  mutable iov : Buf.t array ;
  mutable i : int
}

(* The last fragment also has the protocol headers
 * for the layers above us.
 *)
type header = 
    NoHdr
  | Short of Buf.t
  | Frag of seqno * seqno * Buf.t	(* ith of n fragments *)

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
  { max_len = len_of_int (Param.int vs.params "frag_max_len") ;
    cast = Array.init ls.nmembers (fun _ -> {iov=[||];i=0}) ;
    send = Array.init ls.nmembers (fun _ -> {iov=[||];i=0}) ;
    all_local = all_local ;
    local = local
  }

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let marsh,unmarsh = Buf.make_marsh name in

  (* Handle the fragments as they arrive.
   *)
  let handle_frag ev i n buf =
    (* First, flatten the iovec.  Normally, the iovec
     * should have only one element anyway.  
     *)
    let origin = getPeer ev in
    
    (* Select the fragment info to use.
     *)
    let frag = 
      match getType ev with 
	| ECast _ -> s.cast.(origin)
	| _ -> s.send.(origin)
    in

    (* Check for out-of-order fragments: could send up lost
     * message when they come out of order.  
     *)
    if frag.i <> i then (
      log (fun () -> sprintf "expect=%d/%d, got=%d, type=%s\n" 
        frag.i (Array.length frag.iov) i (Event.to_string ev)) ;
      failwith "fragment arrived out of order" ;
    ) ;
    
    (* On first fragment, allocate the buf array where
     * we will put the rest of the entries.
     *)
    if i = 0 then (
      if frag.iov <> [||] then failwith sanity ;
      frag.iov <- Array.create n Buf.empty
    ) ;
    if Array.length frag.iov <> n then
      failwith "bad frag array" ;
    
    (* Add fragment into array.
     *)
    if i >= Array.length frag.iov then
      failwith "frag index out of bounds" ;
    frag.iov.(i) <- buf ;
    frag.i <- succ i ;
    
    (* On last fragment, send it up.  Note that the ack on
     * this should ack all previous fragments.
     *)
    if i = pred n then (
      let buf = Buf.concat (Array.to_list frag.iov) in
      frag.iov <- [||] ;
      frag.i <- 0 ;
      let abv = unmarsh buf len0 in
      up ev abv
    )
  in


  let up_hdlr ev abv hdr = match hdr with
  | NoHdr -> up ev abv
  | _ -> failwith bad_up_event

  and uplm_hdlr ev hdr = match getType ev, hdr with
  | (ECast iovl | ESend iovl), Short(buf) ->
      assert(Iovecl.len iovl =|| len0);
      let abv = unmarsh buf len0 in
      up ev abv
  | (ECast iovl| ESend iovl), Frag(i,n,buf) ->
      assert(Iovecl.len iovl =|| len0);
      handle_frag ev i n buf
  | _ -> failwith unknown_local

  and upnm_hdlr = upnm 
  
  and dn_hdlr ev abv = match getType ev with
  | ECast iovl | ESend iovl ->
      (* Make sure that the iovec is empty
       *)
      assert (Iovecl.len iovl =|| len0);

      (* Messages are not fragmented if either:
       * 1) They are tagged as unreliable. (BUG?)
       * 2) All their destinations are within this process.
       *)
      let buf = marsh abv in
      let len_buf = Buf.length buf in
      if len_buf <=|| s.max_len
      || (getCompactType ev = C_ECast && s.all_local)
      || (getCompactType ev = C_ESend && Arrayf.get s.local (getPeer ev))
      then (
	(* Common case: no fragmentation.
	 *)
        log2 (fun () -> sprintf "not-fragmenting") ;
      	dnlm ev (Short buf)
      ) else (
        log (fun () -> sprintf "fragmenting") ;

	(* Fragment the message.
	 *)
	let frags = Buf.fragment s.max_len buf in
	log (fun () -> sprintf "total_len=%d frags=%s" 
	  (int_of_len len_buf)
	  (string_of_array (fun buf -> 
	    string_of_int (int_of_len (Buf.length buf))
	  ) frags)
	);

	let nfrags = Array.length frags in
	assert (nfrags >= 2) ;
	for i = 0 to pred nfrags do
	  dnlm ev (Frag(i, nfrags, frags.(i))) ;
	done ;
      )
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr_noopt init hdlrs args vs

let _ = Layer.install name l
    
(**************************************************************)
