(**************************************************************)
(* PERFREKEY.ML : D_WGL algorithm *)
(* Author: Ohad Rodeh 2/99 *)
(**************************************************************)
(* This layer accepts rekey actions from the user and initiates
 * the optimized rekey protocol. It acts in concert with the
 * OPTREKEY and REALKEYS layers. 
 * 
 * 1. A rekey action is accepted from below. 
 * 2. It is diverted to the leader. 
 * 3. The leader initiates the optimized rekey sequence by 
 *    passing the request up to optrekey. 
 * 4. Once optrekey is done, it passes a RekeyPrcl event with 
 *    the new group key back down. 
 * 5. Members log the new group key. A tree spanning the group 
 *    is computing through which acks will propogate. The leaves 
 *    sends Acks up tree.
 * 6. When Acks from all the children are received at the root,
 *    it prompts the group for a view change. 
*)
(**************************************************************)
open Util
open Layer
open View
open Event
open Trans
open Shared
(**************************************************************)
let name = Trace.filel "PERFREKEY"
(**************************************************************)

type header =
  | NoHdr
  | Cleanup 
  | AckCleanup
  | Ack of int * int
  | Ack2 of int
  | Rekey of bool 
  | Commit

(**************************************************************)

type state = {
  mutable agreed_key : Security.key ;
  rekeying           : Once.t ;
  mutable blocked    : bool ;
  father             : rank option ;
  children           : rank list ;
  mutable commit     : bool ; (* Set only at the leader *)
  mutable acks       : int ;
  mutable acks2      : int ;

 (* For cleanup sub-protocol *)
  mutable cleanup    : bool ;
  mutable init_time  : float ;

  (* For performance measurments *)
  mutable sum        : int ;
  mutable sum2       : int ;
  mutable mx         : int ;

  (* Cleanup *)
  sweep              : Time.t;
  next_flush         : Time.t ref ;
  mutable cleanup_req: bool 
} 
	       
(**************************************************************)
	       
let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "agreed_key=%s\n" (Security.string_of_key s.agreed_key);
  sprintf "blocking=%b, rekeying=%s\n"
    (s.blocked) (Once.to_string s.rekeying)
|])

(**************************************************************)

let init s (ls,vs) =   
  let degree = Param.int vs.params "perfrekey_ack_tree_degree" in
  let first_child n = degree * n + 1 in
  let father n = 
    if n<=0 then None
    else Some ((n-1)/degree)
  in
  let children =
    let rec loop rank degree =
      if (degree = 0) || (rank >= ls.nmembers) then []
      else rank :: (loop (succ rank) (degree - 1))
    in loop (first_child ls.rank) degree
  in
  let sweep = Param.int vs.params "perfrekey_sweep"  in
  let sweep = Time.of_int sweep in
  let next_flush = s.next_cleanup in
  if !next_flush = Time.zero then (
    let alarm = Alarm.get_hack () in
    next_flush := Time.add (Alarm.gettime alarm) sweep;
  );
  { 
      agreed_key = Security.NoKey ;
      rekeying   = Once.create "rekeying" ;
      blocked    = false ;
      father     = father ls.rank ;
      children   = children ;
      commit     = false ;
      acks       = 0 ;
      acks2      = 0 ;
      
      cleanup    = false ;
      init_time  = 0.0 ;
      
      sum        = 0 ;
      sum2       = 0 ;
      mx         = 0 ;

      sweep      = sweep ;
      next_flush = next_flush ;
      cleanup_req= false 
    }
       
(**************************************************************)
       
let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let ls_name = sprintf "%d" ls.rank in
  let log = Trace.log2 name ls_name in
  let log1 = Trace.log2 (name^"1") ls_name in
  let log2 = Trace.log2 (name^"2") ls_name in
  let logm = Trace.log2 (name^"M") "" in
  
  let init_rekey () = 
    if ls.nmembers > 1 
      && ls.rank = 0 
      && not (Once.isset s.rekeying)
      && not s.blocked then (
	Once.set s.rekeying ;
	log (fun () -> "init_rekey");
	upnm (create name ERekeyPrcl[]) 
      )
  in

  let check_send_ack () = 
    if s.agreed_key <> Security.NoKey then
      match s.children,s.father with 
	| [], None -> 
	    log (fun () -> "leader Commit, no multicasting, just one member.");
	    s.commit <- true ;
	    (*dnlm (castEv name) Commit;*)
	    upnm (create name ERekeyCommit[]);
	    dnnm (create name EPrompt[])
	| [], Some father -> dnlm (sendPeer name father) (Ack (s.sum,s.mx))
	| children, None -> 
	    if s.acks = List.length children then (
	      logm (fun () -> sprintf " %d sum=%d max=%d" ls.nmembers s.sum s.mx);
	      log (fun () -> "leader Commit");
	      s.commit <- true ;
	      dnlm (castEv name) Commit;
	      upnm (create name ERekeyCommit[]);
	      dnnm (create name EPrompt[])
	    )
	| children, Some father -> 
	    if s.acks = List.length children then 
	      dnlm (sendPeer name father) (Ack (s.sum,s.mx))
  in


  let check_done_ack2 () =
    if s.acks2 = ls.nmembers then
      logm (fun () -> sprintf " ACK2 done, %d sum=%d" ls.nmembers s.sum2);
  in

  let all_ack_cleanup () = 
    if not s.blocked 
      && s.acks = List.length s.children then (
	s.acks <- 0;
	match s.father with 
	  | None -> 
	      let now = Unix.gettimeofday () in
	      logm (fun () -> sprintf " %d Done cleanup %1.4f" ls.nmembers (now -. s.init_time));
	      init_rekey () 
	  | Some father ->  dnlm (sendPeer name father) AckCleanup
      )
  in

  let got_cleanup () = 
    if not s.cleanup then (
      s.cleanup <- true;
      dnnm (create name ERekeyCleanup[])
    )
  in
  
  let init_cleanup () = 
    assert (ls.rank = 0);
    log (fun () -> "Started cleanup");
    s.cleanup_req <- true;
    got_cleanup ();
    dnlm (castEv name) Cleanup;
  in

  let init_rekey_full flg = 
    if flg then (
      s.init_time <- Unix.gettimeofday ();  (* HACK, for perf measurements *)
      init_cleanup () 
    ) else init_rekey () 
  in

  let up_hdlr ev abv () = up ev abv
			    
  and uplm_hdlr ev hdr = match getType ev,hdr with
    (* Got a request to start rekeying the group.
     *)
  | ESend _, Rekey flg->
      if ls.rank<>0 then failwith "got a rekey request, but I'm not leader";
      init_rekey_full flg

  | ESend _, Ack (sum,mx) -> 
      let origin = getPeer ev in
      log2 (fun () -> sprintf "%d [%d<-%d] Ack(%d,%d)" ls.nmembers ls.rank origin sum mx);
      assert (List.mem origin s.children);
      s.acks <- succ s.acks ;
      s.sum <- s.sum + sum;
      s.mx <- max s.mx mx;
      check_send_ack ()

  | ESend __, Ack2 sum -> 
      assert (ls.rank = 0);
      let origin = getPeer ev in
      log2 (fun () -> sprintf "%d [%d<-%d] Ack2(%d)" ls.nmembers ls.rank origin sum);
      s.acks2 <- succ s.acks2 ;
      s.sum2 <- s.sum2 + sum;
      check_done_ack2 ()

  | (ECast _|ESend _), Cleanup ->
      got_cleanup ()

  | ESend _, AckCleanup ->
      s.acks <- succ s.acks ;
      all_ack_cleanup ()

  | ECast _, Commit -> 
      upnm (create name ERekeyCommit[]);
      s.commit <-true 

  | _ -> failwith unknown_local
	
  and upnm_hdlr ev = match getType ev with
  | ERekey ->
      if not (Once.isset s.rekeying)
	&& not s.blocked
	&& ls.nmembers > 1 then (
	  let flg = getRekeyFlag ev in 
	  log (fun () -> sprintf "ERekey(%b) event" flg) ;
	  if ls.rank = 0 then 
	    init_rekey_full flg
	  else dnlm (sendPeer name 0) (Rekey flg)
	)

  | ETimer -> 
      let now = getTime ev in
      if Time.ge now !(s.next_flush) then (
	(*log (fun () -> "Should rekey");*)
	if not s.blocked 
	  && not s.cleanup 
	  && not s.cleanup_req 
	  && ls.nmembers > 1 then (
	    s.cleanup_req <- true;
	    log (fun () -> "Timeout is over, flushing the secure-channel cache.");
	    if ls.rank = 0 then init_cleanup ()
	    else dnlm (sendPeer name 0) Cleanup
	  )
      );
      upnm ev
	  
  (* Rekey protocol is complete. The final key, being delivered to 
   * all other group members, is attached. All leaves must send Acks to
   * their respective fathers. 
   *)
  | ERekeyPrcl -> 
      let agreed_key = getAgreedKey ev in
      let num = getSecStat ev in
      s.sum <- s.sum + num;
      s.mx <- max s.mx num;
      s.agreed_key <- agreed_key ;
      log (fun () -> sprintf "AgreedKey=%s" (Security.string_of_key s.agreed_key));
      check_send_ack ()
	

  (* Hack. 
   * 
   * This is used to measure how many secure channels have
   * been built so far. 
   *)
  | ERekeyPrcl2 -> 
      let num = getSecStat ev in
      s.sum2 <- s.sum2 + num;
      if ls.rank <> 0 then 
	dnlm (sendPeer name 0) (Ack2 s.sum2)
      else (
	s.acks2 <- succ s.acks2 ;
	check_done_ack2 ()
      )
	
  (* EBlock: Mark that we are now blocking.
   *)
  | EBlock ->
      s.blocked <- true ;
      upnm ev
	
  (* EView: If a new_key has been selected, then change the appropriate
   * field in the view. 
   *)
  | EView ->
      log1 (fun () -> "up EView");
      let vs_new = getViewState ev in
      let ev = 
	if s.commit
	  && vs_new.key = Security.NoKey then (

	    if s.cleanup then (
	      s.next_flush := Time.add !(s.next_flush) s.sweep;
	      log (fun () -> sprintf "Member cleanup, next_flush=%4.3f" 
		  (Time.to_float !(s.next_flush)))
	    );

	    assert (s.agreed_key <> Security.NoKey);
	    log1 (fun () -> "setting new key (up)");

	    let key = s.agreed_key in
	    let vs_new = View.set vs_new [Vs_key key] in
	    set name ev[ViewState vs_new] 
	  ) else ev
      in
      upnm ev

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev
	
  and dn_hdlr ev abv = dn ev abv ()

  and dnnm_hdlr ev = match getType ev with
  (* If key dissemination is complete then Erase the key from the 
   * outgoing view. Add it on the way up.
   *)
  | EView ->
      log1 (fun () -> "dn EView");
      let vs_new = getViewState ev in
      let ev = 
	if s.commit then (
	  assert (s.agreed_key <> Security.NoKey);
	  log1 (fun () -> "Erasing view key");
	  let vs_new = View.set vs_new [Vs_key Security.NoKey] in
	  set name ev[ViewState vs_new]
	) else ev
      in
      dnnm ev
	
  (* SECCHAN and REKEY/OPTREKEY+REALKEYS finished cleanup
   *)
  | ERekeyCleanup -> all_ack_cleanup ()
      
  | _ -> dnnm ev
      
in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}
     
let l args vf = Layer.hdr init hdlrs None NoOpt args vf
		  
let _ = 
  Param.default "perfrekey_ack_tree_degree" (Param.Int 6);
  Param.default "perfrekey_sweep" (Param.Int (24 * 60 * 60));    (* 24 hours *)
  Layer.install name l

(**************************************************************)
    
