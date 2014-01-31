(**************************************************************)
(* EXCHANGE.ML : key exchange protocol *)
(* Authors: Ohad Rodeh 4/98 *)
(* Based on code by Mark Hayden, with suggestions by Ron Minsky *)
(**************************************************************)

open Buf
open Layer
open View
open Event
open Util
open Trans
open Shared
open Mexchange
open Security

(**************************************************************)
let name = Trace.filel "EXCHANGE"
let failwith s = Trace.make_failwith name s
let log = Trace.log name 
(**************************************************************)
  
(* A dummy type. 
 *)
type none_msg = NoHdr
    
type bckgr = 
  | Idle
  | Computing of Mexchange.inner_msg 
  | Accepted
      
type state = {
  dh                 : DH.t ;
  dhl_key            : DH.key ;
  pub_key            : string ;
  policy             : (Addr.set -> bool) option ;
  sfmrsh             : Mexchange.t;    (* The state for the SafeMarsh module *)
  my_endpt           : string ;        (* A string representation of ls.endpt *)
  mutable key_sug    : Security.key ;
  mutable blocked    : bool;
  mutable nonce      : int ;
  mutable next       : Time.t ;
  mutable bckgr      : bckgr
}
    
let dump = ignore2
  
let check_policy s a = match s.policy with
  | None -> true
  | Some f -> f a
      
let init s (ls,vs) = 
  let dh = DH.lookup "OpenSSL/DH" in
  let dh_key = match !(s.dh_key) with 
    | Some key -> key
    | None -> 
	let len = Param.int vs.params "secchan_key_len" in
	let key = DH.fromFile dh len in
	DH.generate_key dh key;
	s.dh_key := Some key;
	(* DH.show_params dh key;*)
	key
  in
  let pub_key = hex_of_string (DH.string_of_bignum dh (DH.get_pub dh dh_key)) in
  let my_endpt = Endpt.string_of_id ls.endpt in
  let max_diff = Param.int vs.params "sec_diff" in
  {
    dh        = dh ;
    dhl_key   = dh_key ;
    pub_key   = pub_key ;
    key_sug   = vs.key ;
    blocked   = false ;
    policy    = s.exchange;
    sfmrsh    = Mexchange.init max_diff (View.string_of_id ls.view_id);
    my_endpt  = my_endpt ;
    nonce     = 0 ;
    next      = Time.zero ;
    bckgr     = Idle  ;
  }
  
(**************************************************************)
let string_of_bckgr = function
  | Idle -> "Idle"
  | Computing _ -> "Computing" 
  | Accepted -> "Accepted"
      
let src_endpt = function
  | Piece (src,_,_,_,_,_,_) -> src
  | Reply (src,_,_,_,_,_,_) -> src
      
let src_addr = function
  | Piece (_,s_addr,_,_,_,_,_) -> s_addr
  | Reply (_,s_addr,_,_,_,_,_) -> s_addr
      
let dst_endpt = function
  | Piece (_,_,_,dst,_,_,_) -> dst
  | Reply (_,_,_,dst,_,_,_) -> dst
      
let dst_addr = function
  | Piece (_,_,_,_,d_addr,_,_) -> d_addr
  | Reply (_,_,_,_,d_addr,_,_) -> d_addr
      
let dst_nonce = function
  | Piece (_,_,d_nonce,_,_,_,_) -> d_nonce
  | Reply (_,_,d_nonce,_,_,_,_) -> d_nonce
      
let string_of_msg = function
  | Id(_,_,_) -> "Id"
  | Compx(inner_msg,ticket) -> "Compx _" ^ (Auth.string_of_ticket ticket)
      

(**************************************************************)
let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm}  =
  let log = Trace.log2 name ls.name in
  let log1 = Trace.log2 (name^"1") ls.name in
  let log2 = Trace.log2 (name^"2") ls.name in
  let logs = Trace.log2 (name^"S") ls.name in
  let shared = Cipher.lookup "OpenSSL/RC4" in
  let encrypt key = Cipher.single_use shared key true
  and decrypt key = Cipher.single_use shared key false in
  
  let digest m = Digest.string  (Marshal.to_string m [] ) in
  
  let gossip_send msg = Mexchange.string_of_msg msg  in
  
  let gossip_get msg = 
  log1 (fun () -> "Recv: "^ msg );
    let msg = Mexchange.msg_of_string msg in
    (*log (fun () -> sprintf "Recv=%s" (string_of_msg msg));*)
    msg
  in
  
  let agreed_key_of_bignum bignum = 
    let material = DH.compute_key s.dh s.dhl_key bignum in
    if Buf.len_of_int (String.length material) < Security.cipher_len then 
      failwith "key material is too short";
    Security.cipher_of_buf (Buf.of_string  material)
  in

  (* Handle a verified message
   *)
  let verified = function
    | Piece (s_endpt,s_addr,d_nonce, d_endpt, d_addr, s_nonce, bignum) -> (
	log (fun () -> sprintf "got Piece from %s" (Addr.string_of_set s_addr));
	match s.bckgr with 
	  | Idle -> 
	      log (fun () -> "Idle, computing");
	      let bignum =  DH.bignum_of_string s.dh (hex_to_string bignum) in 
	      let tmp_cipher = agreed_key_of_bignum bignum in
	      let key = encrypt tmp_cipher (Security.buf_of_key vs.key) in
	      let msg = Reply (s.my_endpt, ls.addr, s_nonce, s_endpt, s_addr, s.pub_key, key) in
	      log (fun () -> sprintf "sending a Reply -> %s" (Addr.string_of_set s_addr));
	      s.bckgr <- Computing msg;
	      dnlm (create name EAuth [
		AuthData (s_addr, Auth.Clear (Some (digest msg)))
	      ]) NoHdr
	  | Computing _ 
	  | Accepted -> ()
      )  
	
    | Reply (s_endpt, s_addr, d_nonce, d_endpt, d_addr, bignum, key) -> 
	log (fun () -> sprintf "got Reply from %s" (Addr.string_of_set s_addr));
	let bignum = DH.bignum_of_string s.dh (hex_to_string bignum) in 
	let tmp_cipher = agreed_key_of_bignum bignum in
	let key = decrypt tmp_cipher key in
	s.key_sug <- Security.key_of_buf key ;
	s.bckgr <- Accepted ;
	upnm (create name EPrompt [])
  in
  
  let match_msg = function
    | Id (s_endpt, s_addr, s_nonce) -> 
	if s_endpt <= s.my_endpt then 
	  log1 (fun () -> sprintf "discarding id %s" s_endpt)
	else
	  if not (check_policy s s_addr) then
	    log1 (fun () -> "recvd id, but policy rejected it")
	  else (
	    log (fun () -> sprintf "recvd and ok'd id") ;
	    match s.bckgr with 
	      | Idle -> 
		  let msg =
		    Piece (s.my_endpt, ls.addr, s_nonce, s_endpt, s_addr, 
		    create_nonce s.sfmrsh s.nonce , s.pub_key) in
		  s.bckgr <- Computing msg;
		  log (fun () -> sprintf "Sending Piece -> %s" (Addr.string_of_set s_addr));
		  dnlm (create name EAuth [
		    AuthData (s_addr, Auth.Clear (Some (digest msg)))
		  ]) NoHdr;
	      | Computing _ 
	      | Accepted -> ()
	  )
	    
    | Compx (msg, sign) -> 
	if s.my_endpt = dst_endpt msg then 
	  if check_policy s (src_addr msg) then
	    if check_nonce s.sfmrsh s.nonce (dst_nonce msg) then (
	      match s.bckgr with 
		| Idle -> 
		    s.bckgr <- Computing msg;
		    log2 (fun () -> sprintf "recv ticket=%s\n" 
			(Auth.string_of_ticket sign));
		    dnlm (create name EAuth [
		      AuthData (src_addr msg, Auth.Ticket (Some sign))
		    ]) NoHdr
		| Computing _ 
		| Accepted -> ()
	    ) else 
	      log (fun () -> sprintf "old nonce message, discarding diff=%d" 
		  (diff_nonce s.sfmrsh s.nonce (dst_nonce msg)))
	  else
	    log (fun () -> "Member did not pass the ACL check")
	else
	  log (fun () -> "Messge not to me, discarding")
	    
  in    
  
  let gossip_recv msg = 
    try 
      let msg = gossip_get msg in
      if s.blocked then
	log1 (fun () -> sprintf "blocked++")
      else
	match_msg msg
    with _ -> 
      log (fun () -> sprintf "Could not unmarshal a gossip message %s" msg)
  in
  
  let up_hdlr ev abv () = up ev abv
    
  and uplm_hdlr ev hdr =  match getType ev with 
    | EAuth -> (
	let _,op = getAuthData ev in
	match s.bckgr with
	  | Idle -> failwith "Recieved EAuth in Idle state"
	  | Computing msg -> 
	      s.bckgr <- Idle;
	      (match op with 
		| Auth.Clear None -> 
		    log (fun () -> "Could not verify ticket")
		      
		| Auth.Clear (Some clear) -> 
		    if clear = digest msg then 
		      verified msg 
		    else ()
		      
		| Auth.Ticket None -> 
		    log (fun () -> "Could not create ticket to remote address")
		      
		| Auth.Ticket (Some ticket) -> 
		    dnnm (create name EGossipExtDir[
		      Address (dst_addr msg) ;
		      ExchangeGos (gossip_send (Compx (msg,ticket)))
		    ]);
	      );
	  | Accepted -> ()
      )
	
    | _ -> failwith "bad uplm event"
	
  and upnm_hdlr ev = match getType ev with
      (* Decrypt new_key with old_key
       * It is none if it was disseminated using Rekey. 
       *)
  | EView ->
      let new_vs = getViewState ev in
      let new_vs = 
	match new_vs.key with 
	  | Security.NoKey -> new_vs (* Dissemination using Rekey *)
	  | Security.Common _ -> 
	      let key = Cipher.decrypt_key shared (get_cipher vs.key) new_vs.key in
	      logs (fun () -> sprintf "up EView %s" (Security.string_of_key key));
	      View.set new_vs [Vs_key key] 
      in
      s.blocked <- true ;
      upnm (set name ev[ViewState new_vs])
	
  | EBlock ->
      s.blocked <- true ;
      upnm ev
	
  | EInit -> 
      s.next <- Time.add (Time.of_int 1) (getTime ev);
      upnm ev
	
  | ETimer ->
      let now = getTime ev in
      if Time.ge now s.next then (
	s.nonce <- succ s.nonce;
	s.next <- Time.add now  (Time.of_int 1);
      );
      upnm ev
	
  | EGossipExt -> 
      if ls.am_coord then (
    	getExtendOpt ev (function
	  | ExchangeGos(a) -> gossip_recv a ; true
	  | _ -> false
	)
      ) ;
      upnm ev
	
  | EDump -> ( dump vf s ; upnm ev )
      
      
  | _ -> upnm ev
      
  and dn_hdlr ev abv = dn ev abv ()
    
  and dnnm_hdlr ev = match getType ev with
      (* Encrypt new_key with old_key
       * It is none if it was disseminated using Rekey. 
       *)
  | EView ->
      logs (fun () -> "dn EView");
      let new_vs = getViewState ev in
      let new_vs = 
	match new_vs.key with 
          | Security.NoKey -> new_vs (* Dissemination using Rekey *)
	  | Security.Common key -> 
	    match s.key_sug with 
	    | Security.NoKey -> new_vs
	    | Security.Common _ -> 
		let enc_key_sug = Cipher.encrypt_key shared (get_cipher vs.key) s.key_sug in
	    	View.set new_vs [Vs_key enc_key_sug] 
      in
      dnnm (set name ev[ViewState new_vs])
	
  | EGossipExt ->
      let ev =
	if ls.am_coord && not s.blocked then (
	  let msg = gossip_send(Id(s.my_endpt,ls.addr,create_nonce s.sfmrsh s.nonce)) in
	  set name ev [ExchangeGos msg]
	) else ev
      in
      dnnm ev
	
  | _ -> dnnm ev
      
  in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}
       
let l args vf = Layer.hdr init hdlrs None NoOpt args vf
		  
(* [sec_sim] simulate authentication?
 *)
let _ = 
  Param.default "sec_sim" (Param.Bool false) ;
  Param.default "sec_diff" (Param.Int 10) ;
  Layer.install name l
    
(**************************************************************)
