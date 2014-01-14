(**************************************************************)
(* STABLE.ML : stability detection protocol *)
(* Author: Mark Hayden, 12/95 *)
(* Based on code by: Robbert vanRenesse *)
(* Note: Sequence numbers begin at 0 in each epoch.
 * Acknowlegdements give the number of messages acknowledged
 * so far in the epoch.  Thus the first messge is
 * acknowledged with '1'.  Similarly, the stability numbers
 * correspond to the number of messages that are stable in
 * the current epoch. *)
(**************************************************************)
open Trans
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "STABLE"
let log_iov = Trace.log "IOV"
(**************************************************************)

type header = Gossip of (seqno Arrayf.t) * (bool Arrayf.t)

type state = {
  sweep		   : Time.t ;

  (* The space of time over which to stagger messages 
   *)
  spacing          : Time.t ;  
  acks             : seqno array array ;
  mutable failed   : bool Arrayf.t ;
  mutable next_gossip : Time.t ;

  mutable dbg_maxs : seqno Arrayf.t ;
  mutable dbg_mins : seqno Arrayf.t
}

(**************************************************************)

let dump = Layer.layer_dump name (fun (ls,vs) s -> Array.append [|
  sprintf "failed  =%s\n" (Arrayf.bool_to_string s.failed) ;
  sprintf "dbg_mins=%s\n" (Arrayf.to_string string_of_int s.dbg_mins) ;
  sprintf "dbg_maxs=%s\n" (Arrayf.to_string string_of_int s.dbg_maxs) ;
|] (Array.mapi (fun i a ->
  sprintf "acks(%02d)=%s\n" i (string_of_int_array a)
) s.acks))


(* Stagger out stability events based on the rank and the time
 * frame over which to space things. 
*)
let space_out rank nmembers spacing = 
  let spacing = Time.to_float spacing in
  let rltv = (float rank) /. (float nmembers) in
  let time = mod_float rltv spacing in
  Time.of_float time

(**************************************************************)

let init _ (ls,vs) = {
  sweep	        = Param.time vs.params "stable_sweep" ;
  spacing       = Param.time vs.params "stable_spacing" ;

  failed        = ls.falses ;
  acks          = Array.create_matrix ls.nmembers ls.nmembers 0 ; (* matrix (from,to) *)

  next_gossip = Time.invalid;
  dbg_mins	= ls.zeroes ;
  dbg_maxs  	= ls.zeroes
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let log = Trace.log2 name ls.name in
  let failwith = layer_fail dump vf s name in

  (*
   * DO_STABLE: Recalculate stability.  Send down a EStable event.
   *)
  let do_stable () =
    let mins = Array.copy s.acks.(ls.rank) in
    let maxs = Array.create ls.nmembers 0 in

    (* For each member, update ack entry and find new stability.
     *)
    for i = 0 to pred ls.nmembers do
      if not (Arrayf.get s.failed i) then (
        let row = s.acks.(i) in
        for j = 0 to pred ls.nmembers do
	  let seqno = row.(j) in
	  if seqno >| maxs.(j) then maxs.(j) <- seqno ;
	  if seqno <| mins.(j) then mins.(j) <- seqno
	done
      )
    done ;

    let mins = Arrayf.of_array mins in
    let maxs = Arrayf.of_array maxs in
    s.dbg_mins <- mins ;
    s.dbg_maxs <- maxs ;

    (* Send dn EStable event.
     *)
    dnnm (create name EStable[
      Stability mins ;
      NumCasts maxs
    ]) ;
  in

  let up_hdlr ev abv () = up ev abv
  and uplm_hdlr ev hdr = match getType ev,hdr with

    (* Gossip Message: if from a live member, copy into
     * the origins row in my acknowledgement matrix.
     *)
  | (ECast _ |ESend _ |ECastUnrel _ |ESendUnrel _), Gossip(remote,failed) ->
      let origin = getPeer ev in
      if (not (Arrayf.get failed ls.rank)) (* BUG: could auto-fail him *)
      && (not (Arrayf.get s.failed origin))
      then (
	let local = s.acks.(origin) in
	for i = 0 to pred ls.nmembers do
	  let remote = Arrayf.get remote i in
	  if remote >| local.(i) then
	    local.(i) <- remote
	done ;
      ) ;
      free name ev

  | _ -> failwith unknown_local
   
  and upnm_hdlr ev = match getType ev with

    (* EInit: request a first timer alarm to bootstrap
     * things.
     *)
  | EInit ->
      dnnm (timerAlarm name Time.zero) ;
      upnm ev

    (* EFail: mark the failed members and check if any
     * messages are now stable.
     *)
  | EFail ->
      s.failed <- getFailures ev ;
      dnnm (create name EStableReq[]) ;
      upnm ev

    (* ETimer: every so often:
     *   1. recalculate stability and deliver EStable event
     *   2. gossip about my row in the stability matrix
     *)
  | ETimer ->
      (* Log the state of the refcounts.
       * 
       * BUG: we should put this inside Appl.monitor
       *)
      if Random.int 100 = 0 then
	log_iov (fun () ->
	  let num,lens = Iovec.debug () in
	  sprintf "refcounts = %d,%d\n   GC=%s" num lens 
	    (Util.string_of_list ident
	      (Util.string_list_of_gc_stat (Gc.stat ())))
	);

      let time = getTime ev in
      (*if !verbose then dump vf s ;*)
      if Time.Ord.ge time s.next_gossip then (
      	let old = s.next_gossip in

	(* This check prevents immediate broadcasts after a view.
	 * We stagger heartbeats a little bit. 
	 *)
      	if Time.is_invalid old then
	  s.next_gossip <- space_out ls.rank ls.nmembers s.spacing
	else (
          dnnm (create name EStableReq[]);
	  log (fun () -> sprintf "sending stability %s" 
	    (Time.to_string time))
	);
	  
	s.next_gossip <- Time.add time s.sweep ;
	dnnm (timerAlarm name s.next_gossip) ; (* request next gossip *)
      ) ;
      upnm ev

    (* Got reply to my stability request.
     *)
  | EStableReq ->
      let casts = getNumCasts ev in
      for i = 0 to pred ls.nmembers do
        s.acks.(ls.rank).(i) <- Arrayf.get casts i
      done ;
      do_stable () ;
      let my_row = Arrayf.of_array s.acks.(ls.rank) in
      dnlm (castUnrel name) (Gossip(my_row,s.failed)) ;
      upnm ev

  | EDump -> dump vf s ; upnm ev
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv ()

  and dnnm_hdlr ev = match getType ev with
(*
    (* EAck: update my entry in my row of the acknowledgement
     * matrix.
     *)
  | EAck ->
      begin
	match getAck ev with
	| RankSeqno(rank,seqno) ->
	    if seqno > s.acks.(ls.rank).(rank) then (
	      s.acks.(ls.rank).(rank) <- seqno ;
	    )
        | _ -> failwith "got unknown type of ack"
      end ;
      free name ev
*)
  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr init hdlrs None (FullNoHdr ()) args vs

let _ = 
  Param.default "stable_spacing" (Param.Time (Time.of_float 0.5));
  Param.default "stable_sweep" (Param.Time (Time.of_int 1)) ;
  Param.default "stable_explicit_ack" (Param.Bool false) ;
  Layer.install name l

(**************************************************************)
