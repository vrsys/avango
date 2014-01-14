(**************************************************************)
(* REALKEY.ML : D_WGL algorithm *)
(* Author: Ohad Rodeh 2/99  *)
(**************************************************************)
(* This layer acts in concert with PERFREKEY and REALKEYS to 
 * implement the DWGL rekey algorithm. Each member receives 
 * instructions on how to create the new keytree. This information
 * is received from the OPTREKEY layer. 
 * 
 * 1. A member receives a ERekeyPrcl event with an (tree,full)
 *    field. It purges out useless keys and goes about creating 
 *    the new keytree. Instructions on the protocol to carry out
 *    are stored in is.p_memrec.
 * 2. It creates and sends the keys in p_memrec.send.
 *    It multicasts the keys in p_memrec.cast1.
 *    It receives the key in p_memrec.recv.
 *    It multicasts the key in p_memrec.cast2. 
*)
(**************************************************************)
open Util
open Layer
open View
open Event
open Trans
open Shared
open Tdefs
open Security
(**************************************************************)
let name = Trace.filel "REALKEYS"
let my_assert (flag,s) = assert (if not flag 
                then (print_string (s^"\n"); false) else true)
(**************************************************************)
  
type xfer_msg = 
  | SharedKey of Key.t
  | Cast1Key of (Key.t * Buf.t) list * Key.t
  | Cast2Key of (Key.t * Buf.t) * Key.t
  | SumCasts of ((Key.t * Buf.t) list * Key.t) list 
      
(**************************************************************)
(* p_memrec follows the advance of the actual protocol run. It
 * records all the steps that have been taken for a specific member. 
 * It is checked against the required steps, that module Tree has computed.
 *)
type istate = {
  tree              : Tree.z ;
  memrec            : Tree.memrec option ;
  mutable sum_casts : ((Key.t * Buf.t) list * Key.t) list ;
  total_mcasts      : (Key.t list * Key.t) list ;
  p_memrec          : Tree.memrec ;
}
    
type state = {
  blocked            : Once.t ;
  got_tree           : Once.t ;
  mutable dly_shared_key: (origin * Key.t * Security.cipher option) option ;
  mutable dly_sum_casts : ((Key.t * Buf.t) list * Key.t) list option ;
  key_list_ref       : (Key.t * Security.cipher option) list ref ;
  mutable key_list   : RealKey.set ;
  mutable is         : istate option
}
    
(*let string_of_pair (k,s) = (Key.string_of_t k) ^ " " ^ (Security.string_of_key s)*)
let string_of_pair (k,s) = (Key.string_of_t k)

let string_of_msg = function	       
  | SharedKey k -> "SharedKey " ^ Key.string_of_t k
  | Cast1Key (rkl,rk) -> 
      "Cast1Key " ^ (string_of_list string_of_pair rkl) ^" "^ (Key.string_of_t rk)
  | Cast2Key (rk1,rk2) -> 
      "Cast2Key (" ^ string_of_pair rk1 ^") "^ Key.string_of_t rk2
  | SumCasts sc -> 
      "SumCasts " ^ string_of_list (function rkl,rk -> 
	(string_of_list string_of_pair rkl) ^" "^ (Key.string_of_t rk)
      ) sc
	
(**************************************************************)
	
let string_of_is = function
  | None -> "None"
  | Some is -> 
      let a = [|
	sprintf "tree=%s\n" (Tree.string_of_z is.tree)
      |] in
      string_of_array Util.ident a
	
let string_of_key = Key.string_of_t 
  
let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "blocked=%s\n" (Once.to_string s.blocked) ;
  sprintf "got_tree=%s\n" (Once.to_string s.got_tree) ;
  sprintf "dly_shared_key=%s\n" (string_of_option (fun _ -> "Some") s.dly_shared_key) ;
  sprintf "dly_sum_casts=%s\n" (string_of_option (fun _ -> "Some") s.dly_sum_casts) ;
  sprintf "key_list=%s\n" (string_of_list string_of_pair (RealKey.pairs_of s.key_list)) ;
  sprintf "is=%s\n" (string_of_is s.is)
|])
  
(**************************************************************)
  
let init s (ls,vs) = 
  try 
    let key_list = RealKey.from_list (
      List.map RealKey.from_pair !(s.Layer.key_list)
    ) in
    {
      blocked        = Once.create "blocked" ;
      got_tree       = Once.create "got_tree" ;
      dly_shared_key = None ;
      dly_sum_casts  = None ;
      key_list_ref   = s.Layer.key_list ;
      key_list       = key_list ;
      is             = None
    } 
  with _ -> eprintf "REALKEYS Error in init\n"; exit 0
      
(**************************************************************)
  
(* Compute the total set of multicasts that I need to receive
 *)
let total_mcasts h = 
  let total = Tree.mcast_sweep h in
  Sort.list (<=) total 
    
(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith msg = layer_fail dump vf s name msg in
  let log = Trace.log2 name ls.name in
  let log1 = Trace.log2 (name^"1") (string_of_int ls.rank) in
  let log2 = Trace.log2 (name^"2") (string_of_int ls.rank) in
  let log3 = Trace.log2 (name^"3") "" in
  let logm = Trace.log2 (name^"m") "" in

  let shared = Cipher.lookup "OpenSSL/RC4" in
  let encrypt = function
    | None -> failwith "encrypt: cannot encrypt with an empty key"
    | Some cipher -> Cipher.single_use shared cipher true in

  let get_is () = match s.is with 
    | None -> dump (ls,vs) s; failwith "s.is is empty"
    | Some is -> is
  in

  let dn_ack () = 
    log (fun () -> "dn_ack");
    let key = try 
      let tag,skey = RealKey.hd s.key_list in 
      match skey with 
	| None -> Security.NoKey
	| Some skey -> 
	    Security.Common({
	      mac = mac_of_buf (buf_of_cipher skey) ;
	      cipher = skey 
	    }) 
    with Not_found -> 
      Security.NoKey
    in
    dnnm (create name ERekeyPrcl[AgreedKey key]) 
  in
  
  (* Has the protocol been completed? i.e., have all messages been sent/received?
   * This function does not check if the multicast has been received. It
   * is used to figure out if the multicast can now be decrypted. 
   *)
  let proto_complete () = 
    let is = get_is () in
    match is.memrec with 
      | None -> true
      | Some memrec -> memrec = is.p_memrec 
  in	

  (* Decrypt the multicast message and add the new keys to the list. Also, 
   * perform a sanity check, is the multicast what I expected? 
   *)
  let recv_sum_casts sc =
    RealKey.decrypt shared s.key_list sc;
    if not (RealKey.check_all s.key_list) then failwith "Didn't decrypt all the keys";
    dn_ack ()
  in

  (* Handle a received sum_casts message.
   *)
  let got_sum_casts sc = 
    if not (proto_complete ()) then 
      s.dly_sum_casts <- Some sc 
    else 
      recv_sum_casts sc
  in

  (* Bundle a (rkl,rk) into the set of messages to multicast. 
   * This is performed only at the leader. 
   *)
  let bundle = 
    let string_of_pair_mcl mcl = 
      (string_of_list (function kl,k -> 
	(string_of_list string_of_pair kl) ^
	(Key.string_of_t k))
      ) mcl
    and string_of_mcl mcl = 
      (string_of_list (function kl,k -> 
	(string_of_list Key.string_of_t kl) ^
	(Key.string_of_t k))
      ) mcl
    and order sc= 
      Sort.list (<=) (List.map (function rkl,tag -> 
	List.map (function tag,buf -> tag) rkl,tag
      ) sc)
    in
    
    let f rkl rk = 
      assert (ls.rank = 0);
      let is = get_is () in
      is.sum_casts <- (rkl,rk) :: is.sum_casts ;
      if List.length is.sum_casts = List.length is.total_mcasts then (
	my_assert (order is.sum_casts = is.total_mcasts, 
	     sprintf "sum_casts is not as expected got %s, need %s"
	       (string_of_pair_mcl is.sum_casts) ( string_of_mcl is.total_mcasts));
	log1 (fun () -> sprintf "Recieved all the multicasts %s"
	      (string_of_mcl is.total_mcasts));
	got_sum_casts is.sum_casts ;
	logm( fun () -> sprintf " %d SumCasts= %d\n" ls.nmembers 
	    (String.length (Marshal.to_string (SumCasts is.sum_casts) [])));
	dnlm (castEv name) (SumCasts is.sum_casts) 
      ) else ()
    in
    function
      | Cast1Key (rkl,rk) -> f rkl rk
      | Cast2Key (rk_enc,rk) -> f [rk_enc] rk
      | _ -> failwith "bundle:bad message type"
  in

  let castmsg msg = 
    if ls.rank=0 then bundle msg
    else dnlm (sendPeer name 0) msg
  in

  (* If there is an entry in the full saying what the member should do,
   * then perform the appropriate actions. 
   * Send an Ack, if there are no more actions to perform.
   *)
  let proto () =
    log (fun () -> "proto");
    let is = get_is () in
    begin match is.memrec with 
      | None -> () 
      | Some memrec -> 
	  let new_keys = List.map (function (m,tag) -> 
	    let sk = Shared.Prng.create_cipher () in
	    RealKey.add s.key_list tag (Some sk);
	    tag,sk
	  ) memrec.Tree.send in
	  
	  let actions = 
	    let sendl = 
	      List.map2 (fun (peer,k) (tag,sk) -> 
		let peer = Member.int_of peer in
		dnlm (create name ESecureMsg[
		  Peer peer; 
		  SecureMsg (Security.buf_of_cipher sk)
		]) (SharedKey tag);
		SharedKey tag
	      ) memrec.Tree.send new_keys in 
	    let castl = 
	      List.map (function keyl,tag -> 
		let enc_skey = RealKey.assoc tag s.key_list in
		let keyl = List.map (function k -> 
		  let skey = 
		    match RealKey.assoc k s.key_list with 
		      | None -> failwith "proto:actions: None"
		      | Some skey -> skey in
		  k,encrypt enc_skey (Security.buf_of_cipher skey)
		) keyl in
		castmsg (Cast1Key (keyl,tag));
		Cast1Key (keyl,tag)
	      ) memrec.Tree.cast1
	    in sendl @ castl
	  in
	  is.p_memrec.Tree.send <- memrec.Tree.send ;
	  is.p_memrec.Tree.cast1 <- memrec.Tree.cast1 ;
	  
	  log1 (fun () -> "Do: " ^ string_of_list string_of_msg actions);
    end
  in  

  let recv_shared_key origin tag skey = 
    log1 (fun () -> sprintf "%d<-%d SharedKey %s" ls.rank origin (Key.string_of_t tag)); 
    let is = get_is () in
    RealKey.add s.key_list tag skey;
    begin match is.memrec with 
      | None -> 
	  failwith (sprintf "Did not find %d in the hashtable, recv case" ls.rank)
      | Some memrec -> 
	  begin match memrec.Tree.recv,memrec.Tree.cast2 with 
	    | None,None -> failwith "recv is empty, but I received a key"
	    | Some _,None -> ()
	    | None, Some _ -> failwith "recv = None and cast2 = Some _"
	    | Some k_recv, Some (k_mcast,k_enc) -> 
		if k_recv <> tag then failwith "recived the wrong key";
		if k_mcast <> tag then 
		  failwith "cast2=Some(key_mcast,key), key_mcast <> received key";
		let skey = 
		  match RealKey.assoc k_mcast s.key_list with 
		    | None -> failwith "recv_shared_key: skey, RealKey.assoc None"
		    | Some skey -> skey in
		let k_enc_key = RealKey.assoc k_enc s.key_list in
		let skey = encrypt k_enc_key (Security.buf_of_cipher skey) in
		let msg = Cast2Key((k_mcast,skey), k_enc) in
		log1 (fun () -> string_of_msg msg );
		castmsg (msg)
	  end;
	  is.p_memrec.Tree.recv <- memrec.Tree.recv ;
	  is.p_memrec.Tree.cast2 <- memrec.Tree.cast2 ;
    end;
  in
  
  (* Handle delayed recv and mcast messages. These can arrive out
   * of order because they are sent in FIFO order (not CAUSAL).
   *)
  let handle_delayed () = 
    begin match s.dly_shared_key with 
      | None -> () 
      | Some (origin,tag,sk) -> 
	  if Once.isset s.got_tree then 
	    recv_shared_key origin tag sk
    end;
    begin match s.dly_sum_casts with 
      | None -> ()
      | Some sc -> 
	  if (proto_complete ()) then recv_sum_casts sc
    end
  in  

  let up_hdlr ev abv () = up ev abv
    
  and uplm_hdlr ev hdr =
    if not (Once.isset s.blocked) then 
      let origin = getPeer ev in 
      match getType ev,hdr with
	  
  | ESecureMsg, SharedKey new_key_tag -> 
      let origin = getPeer ev
      and real = getSecureMsg ev in
      let real = Security.cipher_of_buf real in
      if Once.isset s.got_tree then (
	recv_shared_key origin new_key_tag (Some real);
	if (proto_complete ()) then handle_delayed ()
      ) else 
	s.dly_shared_key <- Some (origin,new_key_tag,(Some real))

  | ESend _, (Cast1Key _ | Cast2Key _)  when ls.rank=0 -> 
      bundle hdr
      
  | ECast _, SumCasts sc when origin=0 -> 
      got_sum_casts sc 
	
  | _ -> failwith "unknown local"
	
  and upnm_hdlr ev = match getType ev with
 (* EBlock: Mark that we are now blocking.
  *)
  | EBlock ->
      Once.set s.blocked ;
      upnm ev
	
  (* Cleanup the layer. pass down to PERFREKEY.
   *)
  | ERekeyCleanup -> 
      s.key_list <- RealKey.empty;
      dnnm ev

  (* Got a new view, remove keys that are dead.
   * Translate key names. 
   *)
  | EView ->
      let _,subtree = getTree ev in
      let key_list = RealKey.purge subtree s.key_list ls.rank in
      s.key_list_ref := RealKey.pairs_of key_list;
      upnm ev
	
  (* Got a rekey command.
   * There may have been some colisions in key names, translate the
   * "bad keys", and insert the new key tags into the key list at
   * the appropriate places. 
   *)
  | ERekeyPrcl -> (
	Once.set s.got_tree ;
	(* If this is an empty view change, then simply remove the
	 * head key.
	 *)
	let view_type, tree = getTree ev in 
	if not view_type then 
	  if s.key_list <> RealKey.empty then 
	    s.key_list <- RealKey.tl s.key_list ;

	let actions_h,bad_set = getTreeAct ev in
	let total_mcasts = Sort.list (<=) (total_mcasts actions_h) in
	assert (List.length total_mcasts > 0);
	let memrec = try 
	  Some (Hashtbl.find actions_h (Member.of_int ls.rank))
	with Not_found -> None in
	s.is <- Some {
	  tree        = tree ;
	  sum_casts   = [] ;
	  total_mcasts= total_mcasts ;
	  memrec      = memrec ;
	  p_memrec    ={Tree.send = []; Tree.cast1 = []; Tree.recv = None; Tree.cast2 = None}
	};
	
	let key_list = RealKey.translate s.key_list bad_set in
	let key_list = RealKey.inflate tree s.key_list ls.rank in
	s.key_list <- key_list;
	proto () ;
	handle_delayed ()
    )

  | EDump -> ( dump vf s ; upnm ev )

  | _ -> upnm ev
	
  and dn_hdlr ev abv = dn ev abv ()
			 
  and dnnm_hdlr ev = dnnm ev
      
in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

     
let l args vf = Layer.hdr init hdlrs None NoOpt args vf
		  
let _ = Layer.install name l

(**************************************************************)
    
