(**************************************************************)
(* SECCHAN: Creating secure channels between ensemble processes.*)
(* Author: Ohad Rodeh  2/98 *)
(**************************************************************)
(* Fine points:
 * 1. Two members may setup channels to each other simultaniously. 
 *    To prevent this, we use a two phase protocol.
 * 2. A Channel must be constructed within one view, otherwise, 
 *    it is torn-down. Communication protocols should take this 
 *    into account.
 * 3. This layer allows sending only 8-byte aligned messages. 
*)

open Util
open Layer
open View
open Event
open Trans
open Shared
(**************************************************************)
let name = Trace.filel "SECCHAN"
(**************************************************************)

(* Dummy, place holder for the actual type.
*)  

type header = 
  | NoHdr
  | Piece of string 
  | Data of Buf.t 
      
type con_stat = 
  | Closed
  | SemiOpen
  | Open

type 'abv channel = {
  mutable key    : Security.cipher option; (* buffer *)
  msgs           : (Buf.t * 'abv) Queue.t;
  mutable status : con_stat ;
}	

type 'abv state = {
  dh                   : DH.t ;
  dhl_key              : DH.key ;
  channels             : (rank, 'abv channel) Hashtbl.t; 
  mutable blocked      : bool ;
  saved                : (Endpt.id * Security.cipher) list ref ;

  (* PERF measurement *)
  mutable num          : int ;

  (* Configuration parameters *)
  debug                : bool  
} 
    
let string_of_con_stat = function
  | Closed -> "Closed"
  | SemiOpen -> "SemiOpen"
  | Open -> "Open" 

let string_of_header = function
  | NoHdr -> "NoHdr"
  | Piece _  -> "Piece _"
  | Data _ -> "Data _" 
	
(**************************************************************)
let common a b = 
  let a = Lset.inject a
  and b = Lset.inject b in
  Lset.project (Lset.intersecta a b)

(**************************************************************)
    
let dump (ls,vs) s =
  eprintf "SECCHAN:dump:%s\n" ls.name ;
  let l = hashtbl_to_list s.channels in
  let l = List.map (function (peer,ch) -> 
    (string_of_int peer) ^":"^ string_of_con_stat ch.status
  ) l in
  let l = string_of_list ident l  in
  eprintf "rank=%d Channels=%s\n" ls.rank l 

(**************************************************************)
    


(* unfreeze the channel list.
 *)
let init s (ls,vs) = 
  let channels = Hashtbl.create 7 in
  List.iter (function endpt, key -> 
    let ch = {
      key = Some key;
      msgs = Queue.create ();
      status = Open
    } in
    let peer = Arrayf.index endpt vs.view  in
    Hashtbl.add channels peer ch
  ) !(s.secchan);
  s.secchan := [];

  let dh = DH.lookup "OpenSSL/DH" in
  DH.init dh ();
  let dh_key = match !(s.dh_key) with 
    | Some key -> key
    | None -> 
	let len = Param.int vs.params "secchan_key_len" in
	if len < 128 then 
	  failwith "The Diffie-Hellman key must be at least 128 bits long";
	let key = DH.fromFile dh len in
	DH.generate_key dh key;
	s.dh_key := Some key;
	(* DH.show_params dh key;*)
	key
  in
  { 
    dh            = dh;
    dhl_key       = dh_key;
    channels      = channels;
    blocked       = false ;
    saved         = s.secchan ;
    
    num           = 0 ;
    debug         = Param.bool vs.params "secchan_debug" 
  }
  
(**************************************************************)
  
let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith str = layer_fail dump vf s name str in
  let ls_name = sprintf "%d" ls.rank in
  let logs = Trace.log3 Layer.syncing ls.name name in
  let log = Trace.log2 name ls_name in
  let log1 = Trace.log2 (name^"1") ls_name in
  let log2 = Trace.log2 (name^"2") ls_name in
  let log3 = Trace.log2 (name^"2") ls_name in

  let shared = Cipher.lookup "OpenSSL/RC4" in
  let cryptFun = Cipher.single_use shared in
  let encrypt key msg = 
    match key with 
      | None -> failwith "encrypt: key is empty"
      | Some key -> cryptFun key true msg 
  and decrypt key msg = 
    match key with 
      | None -> failwith "decrypt: key is empty"
      | Some key -> cryptFun key false msg
  in
  
  let sendHardMsg peer m = 
    dnlm (Event.create name (ESend Iovecl.empty) [Peer peer; ForceVsync]) m in
  
  let compute_key bignum = 
    let material = DH.compute_key s.dh s.dhl_key bignum in
    if String.length material < 16 then 
      failwith "Generated DH key is too short";
    let key = String.sub material 0 16 in
    log1 (fun () -> sprintf "key=%s" (hex_of_string key));
    key
  in

  (* We make sure to send the reply g^b as quickly as possible. 
   * If we first compute the local key, this would make us a lot slower.
   *)
  let handle_piece peer bignum = 
    try 
      let ch = Hashtbl.find s.channels peer in
      let key = compute_key bignum in
      match ch.status with 
	| SemiOpen -> 
	    ch.key <- Some (Security.cipher_of_buf (Buf.of_string key));
	    ch.status <- Open;
	    Queue.iter (fun (msg,abv) -> 
	      let msg  = encrypt ch.key msg in
	      dn (sendPeer name peer) abv (Data msg) 
	    ) ch.msgs;
	    Queue.clear ch.msgs;
	| Closed | Open -> failwith "Sanity, channel is Closed/Open"
    with Not_found -> 
      if not s.blocked then (
	log (fun () -> sprintf "DH piece -> %d" peer);
	sendHardMsg peer (Piece (
	  DH.string_of_bignum s.dh (DH.get_pub s.dh s.dhl_key )));

	let key = compute_key bignum in
	let ch = {
	  key   = Some (Security.cipher_of_buf (Buf.of_string key));
	  msgs  = Queue.create ();
	  status = Open
	} in
	Hashtbl.add s.channels peer ch;
      ) else 
	log (fun () -> "blocked, can't send reply. Omitting the channel.")
  in
  
  
  (* Recieve a secure message [msg] from [peer]
   *)
  let handle_data peer msg abv = try 
    let ch = Hashtbl.find s.channels peer in
    let msg = decrypt ch.key msg in 
    let e = (Event.create name ESecureMsg[Peer peer; SecureMsg msg])  in
    up e abv 
  with Not_found -> 
    failwith (sprintf "Sanity, received a SecureMsg from %d before the channel is set up." peer)
  in
  
  (* [open_chan_to_peer peer msg] Open a new channel to [peer] and 
   * enqueue [msg] to send.
   *)
  let open_chan_to_peer peer msg = 
    s.num <- 2 + s.num;
    log1 (fun () -> sprintf "open_chan to %d" peer);
    let ch = {
      key = None;
      msgs = Queue.create ();
      status = SemiOpen
    } in
    Queue.add msg ch.msgs;
    Hashtbl.add s.channels peer ch;
    sendHardMsg peer (Piece(
      DH.string_of_bignum s.dh (DH.get_pub s.dh s.dhl_key )))
  in

  (* Handle a secure message [msg] to be sent to [dst]. 
   * 1.If a channel already exists, then send/enqueue. 
   * 2.If it does not exist, create the channel structure, enqueue the message, 
   *   and create a channel.
   * 3. If the channel is about to be closed then enqueue the message in
   * the pending list.
   * 
   *)
  let send_secure_msg dst msg abv = try 
    let ch = Hashtbl.find s.channels dst in
    (match ch.status with 
      | Closed | SemiOpen -> Queue.add (msg,abv) ch.msgs 
      | Open -> 
	  let msg = encrypt ch.key msg in
	  dn (sendPeer name dst) abv (Data msg) 
    )
  with Not_found -> 
    open_chan_to_peer dst (msg,abv)
  in
  

  let up_hdlr ev abv hdr = match getType ev,hdr with 
    | ESend _, Data msg -> 
	let peer = getPeer ev in
	handle_data peer msg  abv
	  
    | _ -> up ev abv

    
  and uplm_hdlr ev hdr = match getType ev,hdr with
    | ESend _, Piece num -> 
	let peer = getPeer ev in
	log (fun () -> sprintf "DH piece <- %d" peer);
	let bignum = DH.bignum_of_string s.dh num in 
	handle_piece peer bignum
	  
    | _ -> failwith "unknown message type"
	
  and upnm_hdlr ev = match getType ev with
    | EBlock -> 
	s.blocked <- true;
	upnm ev

    (* 1. Remove channels to members that have partitioned away. 
     * 2. Remove channels that are partly open. 
     * 3. Throw away all pending messages. 
     * 4. Freeze the current situation, continue in the next view. 
     *)
    | EView ->
	let new_vs = getViewState ev in
	let common = common vs.view new_vs.view in
	log2 (fun () -> sprintf "freezing common=%s view=%s new_view=%s"
	    (View.to_string common)
	    (View.to_string vs.view)
	    (View.to_string new_vs.view)
	);
	Hashtbl.iter (fun rank ch -> 
	  let e = Arrayf.get vs.view rank in
	  if not (Arrayf.mem e common) 
	    || ch.status <> Open 
	    || ch.key = None then 
	    Hashtbl.remove s.channels rank 
	) s.channels;

	let l = hashtbl_to_list s.channels in
	let l = List.map (fun (rank,ch) -> 
	  let e = Arrayf.get vs.view rank in
	  e,some_of name ch.key
	) l in
	s.saved := l;
	upnm ev
	  
	  
    (* Send up data about the current set of channels. 
     *)
    | EInit -> 
	log2 (fun () -> "EInit");
	if s.debug then (
	  let l = hashtbl_to_list s.channels in
	  let l = List.map (fun (rank,_) -> rank) l in
	  upnm (Event.create name ESecChannelList[SecChannelList l])
	);
	upnm ev
	  
	    
    | EDump -> ( dump vf s ; upnm ev )
	  
    | _ -> upnm ev
	  
  and dn_hdlr ev abv = match getType ev with
      (* Got a secureMsg to send  
       *)
    | ESecureMsg -> 
	if not s.blocked then (
	  let dst = getPeer ev in
	  let msg = getSecureMsg ev in
	  log2 (fun () -> sprintf "dst=%d,msg=_" dst);
	  send_secure_msg dst msg abv
	) else 
	  log2 (fun () -> "can't send SecureMsg while blocked, dropped")
	    
    | _ -> dn ev abv NoHdr
	  

  and dnnm_hdlr ev = match getType ev with 
    | EBlock ->
	s.blocked <- true;
	dnnm ev

    (* Add statistics to the ERekeyPrcl message. Statistics are
     * used by PERFREKEY, for performance measurements. 
     *)
    | ERekeyPrcl 
    | ERekeyPrcl2 -> 
	log2 (fun () -> "ERekeyPrcl");
	let num = s.num in
	let ev = set name ev [SecStat num] in
	upnm ev

    (* Close all channels, and clean all pending data. 
     *)
    | ERekeyCleanup -> 
	Hashtbl.clear s.channels;
	s.saved  := [];
	s.num <- 0 ;
	upnm ev
	
    | _ -> dnnm ev
	  
  in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}
       
let l args vf = Layer.hdr init hdlrs None NoOpt args vf
  
let _ = 
  Param.default "secchan_debug" (Param.Bool false);
  Param.default "secchan_key_len" (Param.Int 768);
  Layer.install name l
    
(**************************************************************)
    
    
