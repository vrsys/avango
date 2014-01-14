(**************************************************************)
(* UNSIGNED: 16-byte MD5'd connection IDs. *)
(* Author: Mark Hayden, 3/97 *)
(* Rewritten by Ohad Rodeh 10/2001 *)
(**************************************************************)
open Trans
open Util
open Conn
open Route
open Transport
open Buf
(**************************************************************)
let name = Trace.file "UNSIGNED"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
(**************************************************************)

(* This is used so that the hash of a connection identifier
 * will not include specific information on the source. 
 * Hence, all members of a group will have identical connection
 * identifiers.
*)
let pack_of_conn c =
  Conn.hash_of_id (snd (Conn.squash_sender c))

let f () =
  let const handler = Route.Unsigned(handler true) in

  let unmarsh obj ofs = Marshal.from_string (Buf.string_of obj) (int_of_len ofs) in

  let merge info =
    let upcalls = Arrayf.map (function
      | (_,p,_,(Route.Unsigned h)) -> (p,h)
      | _ -> failwith sanity
      ) info
    in
    let upcalls = Route.group upcalls in

    Arrayf.map (fun (pack,upcalls) ->
      let upcalls = Route.merge4iov upcalls in
     let upcall hdr ofs len iovl = 
	if len <|| md5len_plus_8 then (
	  Iovecl.free iovl ;
	  Route.drop (fun () -> sprintf "%s:size below minimum:len=%d\n" name (int_of_len len))
	) else if not (Buf.subeq16 hdr ofs pack) then (
	  Iovecl.free iovl ;
	  Route.drop (fun () -> sprintf "%s:rest of Conn.id did not match" name)
	) else (
	  let rank  = Buf.read_int32 hdr (ofs +|| md5len) in
	  let seqno = Buf.read_int32 hdr (ofs +|| md5len_plus_4) in
	  Route.info (fun () -> sprintf "rank=%d seqno=%d" rank seqno);
	  let molen = len -|| md5len_plus_8 in
	  if molen =|| len0 then (
	    upcalls rank None seqno iovl
	  ) else (
	    try 
	      let mo = unmarsh hdr (ofs +|| md5len_plus_8) in
	      Route.info (fun () -> "before upcalls");
	      upcalls rank (Some mo) seqno iovl
	    with _ -> 
	      Iovecl.free iovl ;
	      Route.drop (fun () -> sprintf "Bad parsing of ML header")
	  ) 
	)
      in upcall
    ) upcalls
  in


  (* Here, we use a preallocated buffer from the Buf module.
   * The idea is to marshal an ml object into a pre-allocated
   * buffer, and then write the rest of the parameters into
   * the beginning of the buffer. 
   *
   * We pass the [f] function into buf. This saves allocating an
   * intermediated (buf,ofs,len) structure. 
   *)
  let blast xmit _ pack conn _ =
    let sender =
      let sender,_ = Conn.squash_sender conn in
      match sender with
      | Some rank -> rank
      |	None -> -1			(* hack! *)
    in

    fun mo seqno iovl ->
      let f hdr ofs len = 
	Buf.blit pack len0 hdr ofs md5len;
	Buf.write_int32 hdr (ofs +|| md5len) sender ;
	Buf.write_int32 hdr (ofs +|| md5len_plus_4) seqno ;
	log (fun () -> sprintf "blast: ml_len=%d iovl_len=%d" 
	  (int_of_len len) (int_of_len (Iovecl.len iovl)));
	xmit hdr ofs len iovl
      in
      match mo with
	| None -> 
	    Buf.prealloc md5len_plus_8 f
	| Some obj -> 
	    Buf.prealloc_marsh md5len_plus_8 obj f
  in
  Route.create name false const pack_of_conn merge blast

(**************************************************************)
