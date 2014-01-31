(**************************************************************)
(* REKEY_DIAM.ML : switching to a new key using a diamond structure *)
(* Authors: Ohad Rodeh *)
(**************************************************************)

open Util
open Layer
open View
open Event
open Trans

(**************************************************************)
let name = Trace.filel "REKEY_DIAM"
(**************************************************************)
(* The protocol used here is as follows:
 * 1. A rekey request is made by the user. 
 * 2. The request is forwared to the leader.
 * 3. The leader checks if the current diamond contains 
 *    all group members. 
 * 4. If so, then it creates a new set of instructions and a 
 *    a new diamond.
 * 5. The set of instructions is followed by all members. 
 * 6. The last member on the list multicasts a DonePatch.
 * 7. The leader starts constructing the new diamond. 
 * 8. When the last member is done, it multicasts a Commit. 
 * 9. All member that receive the Commit, know that the protocol 
 *    is complete and that the new diamond is the correct one. 
*)
module D = Diamond

type header =
  | MAck
  | MKey
  | Gather
  | Component of D.t
  | CompInst of D.instr * D.t * D.instr
  | Commit 

(* pt-2-pt messages may arrive before the CompInst message.
 * In this case, we must store them for later.
*)
type pend_msg = 
  | PendKey of rank * Buf.t
  | PendAck of rank

(**************************************************************)

type patch_st = {
  mutable comp_l     : D.t list ;
  rcv_comp_vct       : bool array ;
  mutable key_sug    : Security.key ;
  mutable comp_instr : (D.instr * D.t * D.instr) option 
}

type recon_st = { 
  mutable recon_time : Time.t ;
  mutable instr      : D.instr ;
  mutable new_diam   : D.t 
}

type phase = 
  | Norml              (* Normal phase         - do nothing, also -- *)
  | Patch of patch_st  (* Patch phase          - Try a fast rekey *)
  | Recon of recon_st  (* Reconstruction phase - Reconstruct the diamond *)
  | Compl

type state = {
  state_r            : D.appl_state option ref ;
  mutable phase      : phase ;
  mutable diam       : D.t ;
  pend               : pend_msg Queuee.t ;
  mutable children   : (rank * D.simple_cmd) list ;
  mutable fathers    : (rank * D.simple_cmd) list ;
  mutable recvd      : int ;      (* How many messages have I received *)
  mutable blocking   : bool ;
  mutable commit     : bool ;

  (* For PERF measurements. 
   *)
  mutable start_time : Time.t
} 
	       
let string_of_phase = function
  | Norml -> "Norml"
  | Patch _ -> "Patch"
  | Recon _ -> "Recon"
  | Compl ->   "Compl"

(**************************************************************)
	       
let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "phase=%s\n" (string_of_phase s.phase);
  sprintf "diam=%s\n" (D.string_of_t s.diam);
  sprintf "blocking=%b \n" s.blocking;
|])

(**************************************************************)
let init s (ls,vs) = 
  let state = match !(s.Layer.diam) with 
    | None -> D.T (D.singleton ls.rank)
    | Some state -> state in
  let phase,diam = match state with 
    | D.T t -> Norml,t
    | D.Tinstr (t,t_new,instr_new) -> 
	Recon ({
	  recon_time = Time.invalid ;
	  instr      = instr_new ;
	  new_diam   = t_new 
	}), t  
  in { 
      state_r   = s.Layer.diam ;
      phase     = phase ;
      diam      = diam ;
      pend      = Queuee.create ();
      children  = [] ;
      fathers   = [] ;
      recvd     = 0;
      blocking  = false ;
      commit    = false ;
      
      start_time = Time.invalid
    }
    
(**************************************************************)
  
let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith str = layer_fail dump vf s name str in
  let log = Trace.log2 name (sprintf "%d" ls.rank) in
  let logs = Trace.log2 (name^"S") (sprintf "%d" ls.rank) in
  let log1 = Trace.log2 (name^"1") (sprintf "%d" ls.rank) in
  let log2 = Trace.log2 (name^"2") (sprintf "%d" ls.rank) in
  let log3 = Trace.log2 (name^"3") (sprintf "%d" ls.rank) in
  let logm = Trace.log2 (name^"M") "" in
  let dummy = (Buf.of_string "0123456701234567") in
  let alarm = Alarm.get_hack () in

  let subleader (t : D.t) : int = 
    let l = D.members_of_t t in 
    match l with 
      | [] -> failwith "subleader: Diamond is empty"
      | hd::_ -> hd 
  in

  let rec send_key_to_children key = 
    List.iter (function (i,cmd) -> match cmd with 
      | D.Key -> 
	  let ev = (create name ESecureMsg[Peer i; SecureMsg key]) in
	  dnlm ev MKey
      | D.Ack -> 
	  dnlm (sendPeer name i) MAck
    ) s.children;
    (match s.phase with 
      | Norml -> 
	  failwith "send_key_to_children:Norml phase" 
      | Patch p -> 
	  dnnm (create name ERekeyPrcl[AgreedKey (Security.key_of_buf key)]);
	  if s.children = [] then (
	    let end_time = Alarm.gettime alarm in
	    let diff = Time.to_float (Time.sub end_time s.start_time) in
	    logm (fun () -> sprintf "Time for patch = %1.3f %d" diff ls.nmembers);
	    (*dnnm (castEv name) ERekeyCommit;*)
	  )
      | Recon _ -> 
	  if s.children = [] then (
	    logm (fun () -> "Reconstruction Complete");
	    dnlm (castEv name) Commit;
	    recv_commit ()
	  )
      | Compl -> 
	  failwith "send_key_to_children:Compl phase"
    )
    
  and recv_from_father (msg : pend_msg) = 
    let check_sanity peer cmd' =
      List.iter (fun (rank,cmd) -> 
	if rank=peer && cmd<>cmd' then 
	  failwith "Bad message from father"
      )  s.fathers;
      true
    in
    if s.recvd = 0 then 
      Queuee.add msg s.pend 
    else (
      let peer,cmd' = match msg with 
	| PendAck peer-> peer,D.Ack
	| PendKey (peer,key) -> 
	    (match s.phase with 
	      | Norml ->    failwith "recv_from_father: Norml phase"
	      | Patch p -> p.key_sug <- Security.key_of_buf key
	      | Recon _ -> ()
	      | Compl ->   failwith "recv_from_father: Compl phase"
	    );
	    peer,D.Key in
      assert (check_sanity peer cmd');
      s.recvd <- pred s.recvd;
      log1 (fun () -> sprintf "recvd =%d" s.recvd);
      if s.recvd = 0 then 
	match s.phase with 
	  | Norml ->    failwith "recv_from_father: Norml phase"
	  | Patch p -> send_key_to_children (Security.buf_of_key p.key_sug)
	  | Recon _ -> send_key_to_children dummy
	  | Compl ->   failwith "recv_from_father: Cannot be in Compl phase"
    )
      
  and handle_pending () = 
    if not (Queuee.empty s.pend) then (
      Queuee.iter recv_from_father s.pend;
      Queuee.clear s.pend
    )
      
  (* [got_instr instr] perform the instructions described in [instr].
   *)
  and got_instr (s_a,d_a) = 
    s.children <- s_a.(ls.rank);
    s.fathers <- d_a.(ls.rank);
    s.recvd <- List.length s.fathers;
    handle_pending ();
    if s.fathers = [] then (
      match s.phase with 
	| Norml -> 
	    failwith "got_instr:Norml phase"
	| Patch p -> 
	    log (fun () -> "I'm root, creating a new key");
	    p.key_sug <- Shared.Prng.create_key ();
	    send_key_to_children (Security.buf_of_key p.key_sug)
	| Recon _ -> 
	    log (fun () -> "Reconstruction phase, passing a dummy value.");
	    send_key_to_children dummy
	| Compl -> 
	    failwith "got_instr: Compl phase"
    ) 

  and recv_commit () = 
    match s.phase with 
      | Norml  -> 
	  failwith "recv_commit: Norml phase"
      | Patch _ -> 
	  failwith "recv_commit: Patch phase"
      | Recon r -> 
	  log (fun () -> "recv_commit");
	  s.phase <- Compl;
	  s.diam <- r.new_diam;
	  dnnm (create name ERekeyPrcl2[])
      | Compl -> 
	  failwith "recv_commit: Compl phase"
  in


  (* Only the root of the tree should create a new key and send
   * it off. 
   * 
   * If got_comp_inst while in the middle of a Recon, then the Recon state
   * is lost.
   *)
  let got_comp_inst (instr1,diam,instr2) = 
    let create_patch_state () = {
      comp_l       = [];
      rcv_comp_vct = Array.create 0 false;
      key_sug      = Security.NoKey ;
      comp_instr   = None 
    } in

    s.start_time <- Alarm.gettime alarm;
    let p = 
      match s.phase with 
	| Norml -> create_patch_state ()
	| Patch p -> p
	| Recon _ 
	| Compl -> create_patch_state () in
    s.phase <- Patch p;
    p.comp_instr <- Some(instr1,diam,instr2);
    log (fun () -> "CompInst");
    got_instr instr1 
  in

  let got_component peer c = 
    assert (ls.rank = 0);
    match s.phase with 
      | Patch p -> 
	  p.comp_l <- c :: p.comp_l;
	  let l = D.members_of_t c in
	  List.iter (fun i -> p.rcv_comp_vct.(i) <- true) l;
	  logs (fun () -> 
	    sprintf "<- %d Component %s" peer (D.string_of_t c) 
	      (*(string_of_bool_array p.rcv_comp_vct)*)
	  );
	  if array_for_all ident p.rcv_comp_vct then (
	    let instr1,new_diam,instr2 = D.merge p.comp_l ls.nmembers in
	    logs (fun () -> sprintf "merge = \n%s\n" (D.pretty_string_of_t new_diam));
	    dnlm (castEv name) (CompInst (instr1,new_diam,instr2));
	    p.comp_l <- [];
	    got_comp_inst (instr1,new_diam,instr2) 
	  )
      | _ -> failwith "got_compnent not in Norml state"
  in    
  
  
  (* Got a request to start rekeying. 
   * Any Recon state is lost.
   *)
  let init_rekey_prcl () = 
    assert (ls.am_coord);
    s.phase <- Patch( {
      comp_l       = [];
      rcv_comp_vct = Array.create ls.nmembers false;
      key_sug      = Security.NoKey ;
      comp_instr   = None 
    });
    if List.length (D.members_of_t s.diam) = ls.nmembers  then (
      assert (D.fully_connected s.diam);
      logs (fun () -> sprintf "fully_connected=\n%s" (D.pretty_string_of_t s.diam));
      let instr1,diam,instr2 = D.merge [s.diam] ls.nmembers in 
      dnlm (castEv name) (CompInst (instr1,diam,instr2));
      got_comp_inst  (instr1,diam,instr2)
    ) else (
      dnlm (castEv name) Gather;
      if subleader s.diam = ls.rank then 
	got_component ls.rank s.diam 
    )
  in
  
  let up_hdlr ev abv () = up ev abv
    
    
  and uplm_hdlr ev hdr = if not s.blocking then 
  match getType ev,hdr with
  | ECast _, CompInst (instr1,diam,instr2) -> 
      got_comp_inst (instr1,diam,instr2)
	
  | ECast _, Gather ->
      if subleader s.diam = ls.rank then 
	dnlm (sendPeer name 0) (Component s.diam)
      
  | ESend _, Component c-> 
      got_component (getPeer ev) c

  | ECast _, Commit -> 
      recv_commit ()
      
  | ESend _, MAck -> 
      log (fun () -> sprintf "<- Ack %d" (getPeer ev));
      let peer = getPeer ev in 
      recv_from_father (PendAck peer)

  | ESecureMsg, MKey -> 
      log (fun () -> sprintf "<- Key %d" (getPeer ev));
      let peer = getPeer ev in
      let key = getSecureMsg ev in 
      recv_from_father (PendKey(peer,key))

  | _ -> failwith unknown_local
      
  and upnm_hdlr ev = match getType ev with
  | ERekeyPrcl -> 
      log3 (fun () -> "ERekeyPrcl");
      assert (ls.rank = 0);
      if not s.blocking 
	&& ls.nmembers > 1 then
	  init_rekey_prcl ()

  | ERekeyCommit -> 
      s.commit <- true

  | ERekeyCleanup -> 
      s.phase <- Norml ;
      s.diam <- D.singleton ls.rank ;
      dnnm ev
	
  (* EBlock: Mark that we are now blocking.
   *)
  | EBlock ->
      s.blocking <- true;
      (*if ls.rank= 0 then log (fun () -> "block");*)
      upnm ev
	
  | EInit -> 
      (match s.phase with 
	| Norml 
	| Patch _ -> ()
	| Recon r -> r.recon_time <- Time.add (Time.of_int 2) (getTime ev)
	| Compl -> ());
      upnm ev

  | ETimer -> 
      (match s.phase with 
	| Norml 
	| Patch _ -> ()
	| Recon r -> 
	    if r.recon_time <> Time.invalid then (
	      let now = getTime ev in
	      if Time.ge now r.recon_time then (
		r.recon_time <- Time.invalid ;
		got_instr r.instr
	      )
	    )
	| Compl -> ());
      upnm ev
	
  (* Need to set my_component. 
   * If the diamond is currently fully_connected, then we need to do 
   * a Gather stage and rebuild it in the next view. If it isn't, 
   * then we have already rebuild it, and there is no need to do 
   * so in the next view. 
   *)
  | EView -> 
      let vs_new = getViewState ev in
      if vs_new.view <> vs.view then (
	(* First, remove all the members that are leaving. 
	 * split the tree accordingly.
	 *)
	let new_view = Lset.inject vs_new.view
	and cur_view = Lset.inject vs.view in
	let leave = Lset.subtracta cur_view new_view in
	let leave = Lset.project leave in
	let leave = Arrayf.to_array leave in
	let leave = Array.map (function e -> 
	  Arrayf.index e vs.view 
	) leave in
 	let leave = Array.to_list leave in
	(*log1 (fun () -> sprintf "split %s %d %s " 
	    (D.string_of_t s.diam) ls.rank (string_of_int_list leave));*)
	let new_diam = D.split s.diam  ls.rank leave in
	(*logs (fun () -> sprintf "split %s->%s %s" 
	    (D.string_of_t s.diam) (D.string_of_t new_diam)
	    (string_of_int_list leave)
	);*)
	let new_diam = D.map (function i -> 
	  let e = Arrayf.get vs.view i in
	  Arrayf.index e vs_new.view 
	) new_diam in
	s.diam <- new_diam;
	s.state_r := Some (D.T s.diam);
      ) else (
	(* The new view is the same. If we are in patch mode, then 
	 * build the instructions for the next view. 
	 *)
	let new_state = 
	  match s.phase with 
	    | Norml -> D.T (s.diam)
	    | Patch p -> (
		match p.comp_instr with 
		  | None -> D.T (s.diam) 
		  | Some (instr1,t_new,instr2) -> D.Tinstr (s.diam,t_new,instr2)
	      )
	    | Recon _
	    | Compl   -> D.T (s.diam) in
	s.state_r := Some new_state 
      );
      log1 (fun () -> sprintf "EView %s" (D.string_of_t s.diam));
      upnm ev
  
| EDump -> ( dump vf s ; upnm ev )
| _ -> upnm ev
	
  and dn_hdlr ev abv = dn ev abv ()
			 
  and dnnm_hdlr ev = dnnm ev
	  
	  
  in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}
     
let l args vf = Layer.hdr init hdlrs None NoOpt args vf
		  
let _ = Layer.install name l

(**************************************************************)

