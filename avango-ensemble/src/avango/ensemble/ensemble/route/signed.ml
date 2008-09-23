(**************************************************************)
(* SIGNED: MD5 signatures, 16-byte md5 connection ids. *)
(* Author: Mark Hayden, 3/97 *)
(* Rewritten by Ohad Rodeh 10/2001 *)
(**************************************************************)
open Trans
open Buf
open Util
open Shared
(**************************************************************)
let name = Trace.file "SIGNED"
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

let const handler = Route.Signed handler

let unmarsh obj ofs = Marshal.from_string (Buf.string_of obj) (int_of_len ofs)

let f () =
  let zeros = 
    let s = String.create (int_of_len md5len) in
    String.fill s 0 (int_of_len md5len) '0';
    Buf.of_string s
  in
  let scribble = Buf.create md5len in

  let recv pack key secureh insecureh = 
    let key = match key with 
      | Security.NoKey -> failwith "recv: NoKey"
      | Security.Common key' -> Security.buf_of_mac key'.Security.mac in 

    (* Copy the md5 hash to the side [scribble], zero it, and
     * compute the MD5 hash. If the received has is equal to 
     * the computed hash, then return true. Otherwise, return 
     * false.
     *)
    let check_md5 hdr ofs len iovl = 
      let ctx = Hsys.md5_init_full (Buf.string_of key) in
      Buf.blit hdr (ofs +|| md5len_plus_8) scribble len0 md5len;
      Buf.blit zeros len0 hdr (ofs +|| md5len_plus_8) md5len;
      Hsys.md5_update ctx hdr ofs len;
      Hsys.md5_update_iovl ctx iovl;
      let d = Buf.of_string (Hsys.md5_final ctx) in
      let ret = Hsys.substring_eq d len0 scribble len0 md5len in
      log (fun () -> sprintf "check_md5=%b (mo=%d iov=%d)" ret
	(int_of_len len) (int_of_len (Iovecl.len iovl)));
      ret
    in
    
    let upcall hdr ofs len iovl = 
      if len <|| md5len_plus_8 +|| md5len then (
	Iovecl.free iovl ;
	Route.drop (fun () -> sprintf "%s:size below minimum:len=%d\n" name (int_of_len len))
      ) else if not (Buf.subeq16 hdr ofs pack) then (
	Iovecl.free iovl ;
	Route.drop (fun () -> sprintf "%s:rest of Conn.id did not match" name)
      ) else (
	let check = check_md5 hdr ofs len iovl in
	let rank  = Buf.read_int32 hdr (ofs +|| md5len) in
	let seqno = Buf.read_int32 hdr (ofs +|| md5len_plus_4) in
	Route.info (fun () -> sprintf "rank=%d seqno=%d" rank seqno);
	let molen = len -|| (md5len_plus_8 +|| md5len) in
	if molen =|| len0 then (
	  log (fun () -> "deliver None");
	  if check then secureh rank None seqno iovl
	  else insecureh rank None seqno iovl
	) else
	  try 
	    let mo = unmarsh hdr (ofs +|| md5len_plus_8 +|| md5len) in
	    log (fun () -> "deliver Some");
	    if check then secureh rank (Some mo) seqno iovl
	    else insecureh rank (Some mo) seqno iovl
	  with _ -> 
	    Iovecl.free iovl ;
	    Route.drop (fun () -> sprintf "Bad parsing of ML header")
      )
    in upcall
  in
    
  let merge info =
    let upcalls = Arrayf.map (function
      | (_,p,k,(Route.Signed h)) -> ((p,k),h)
      | _ -> failwith sanity
    ) info 
    in
    let upcalls = Route.group upcalls in
    let upcalls =
      Arrayf.map (fun ((pack,key),upcalls) ->
	let secure   = Route.merge4iov (Arrayf.map (fun u -> u true ) upcalls) in
	let insecure = Route.merge4iov (Arrayf.map (fun u -> u false) upcalls) in
	recv pack key secure insecure
      ) upcalls
    in
    upcalls
  in
  
  
  (* Here, we use a preallocated buffer from the Buf module.
   * The idea is to marshal an ml object into a pre-allocated
   * buffer, and then write the rest of the parameters into
   * the beginning of the buffer. 
   *
   * We pass the [f] function into buf. This saves allocating an
   * intermediated (buf,ofs,len) structure. 
   * 
   * The format of the ML part of the message is:
   * [16byte connection_id] [4byte sender] [4byte seqno] [16byte digest]
   * 
   * The iovec-length and ml-object length, are taken into 
   * account implicitly, by the digest function. 
   *)
  let blast xmit key pack conn _ =
    
    let sender =
      let sender,_ = Conn.squash_sender conn in
      match sender with
	| Some rank -> rank
	| None -> -1			(* hack! *)
    in
    let key = match key with 
      | Security.NoKey -> failwith "send: NoKey"
      | Security.Common key' -> Security.buf_of_mac key'.Security.mac in 
    
    fun mo seqno iovl ->
      let f hdr ofs len = 
	Buf.blit pack len0 hdr ofs md5len;
	Buf.write_int32 hdr (ofs +|| md5len) sender ;
	Buf.write_int32 hdr (ofs +|| md5len_plus_4) seqno ;

	if Iovecl.len iovl >|| len0 then 
	  log (fun () -> sprintf "blast: ml_len=%d iovl_len=%d %s %s"
	    (int_of_len len) (int_of_len (Iovecl.len iovl))
	    (Util.hex_of_string (Buf.string_of pack ))
	    (Conn.string_of_id conn)
	  );
	
	(* Handling the md5 hash. 
	 * 1) Zero the designated area, 2) Comupte the md5
	 * 3) write the result.
	 *)
	Buf.blit zeros len0 hdr (ofs +|| md5len_plus_8) md5len; 
	let ctx = Hsys.md5_init_full (Buf.string_of key) in
	Hsys.md5_update ctx hdr ofs len;
	Hsys.md5_update_iovl ctx iovl;
	let d = Buf.of_string (Hsys.md5_final ctx) in
	Buf.blit d len0 hdr (ofs +|| md5len_plus_8) md5len; 
	
	xmit hdr ofs len iovl
      in
      match mo with
	| None -> 
	    Buf.prealloc (md5len_plus_8 +|| md5len) f
	| Some obj -> 
	    Buf.prealloc_marsh (md5len_plus_8 +|| md5len) obj f
  in
  
  Route.create name true const pack_of_conn merge blast
    
(**************************************************************)
  
