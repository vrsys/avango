(**************************************************************)
(* PBCAST.ML : stability detection protocol *)
(* Author: Mark Hayden, 11/96 *)
(* Major Bug fixs:  Zhen Xiao,  3/98
 *  (1) Most-recent-first retransmission
 *  (2) Avoid redundant retransmission: taking into account
 *      the requests in transit.
 *)
(* TODO:                                    
 *   (1) Upon gossip msg, could auto retransmit them msgs they
 *   missed.
 *   (2) The protocol should finish upon view change.
 *)
(**************************************************************)
open Buf
open Trans
open Layer
open Event
open Util
open View
(*module Hashtbl = Hashtble*)
(**************************************************************)
let name = Trace.filel "PBCAST"
(**************************************************************)

type round = seqno

type header = NoHdr
  | Data of rank * seqno * bool   (*retrans?*)
  | Gossip of round * (seqno Arrayf.t)
  | Request of round * ((rank * seqno) Arrayf.t)

type 'abv item =
  | Msg of round * 'abv
  | Lost

type 'abv state = {
  sweep		: Time.t ;
  stale         : round ;
  fanout        : int ;
  reql          : int ;
  max_entropy   : int ;
  msg_overhead  : int ;
  disseminate   : bool ;
  mutable round : round ;
  mutable next_gossip : Time.t ;
  wait          : int ;
  retrans       : ((rank * seqno), round) Hashtbl.t ;

  casts		: 'abv item Iq.t array ;
  recd  	: seqno array ;
  free  	: seqno array ;
  high  	: seqno array ;
  mutable failed : bool Arrayf.t ;
  mutable gossip : rank Arrayf.t ;

  mutable acct_recd : seqno ;		(* # delivered messages *)
  mutable acct_lost : seqno ;		(* # lost messages *)
  mutable acct_retrans : seqno ;	(* # retrans messages *)
  mutable acct_bad_ret : seqno ;	(* # retrans messages not used *)
  mutable acct_sent : seqno             (* # retrans msgs sent by *)
}

let update_appl_info s =
  Appl.cast_info.Appl.acct_recd <- s.acct_recd ;
  Appl.cast_info.Appl.acct_lost <- s.acct_lost ;
  Appl.cast_info.Appl.acct_retrans <- s.acct_retrans ;
  Appl.cast_info.Appl.acct_bad_ret <- s.acct_bad_ret;
  Appl.cast_info.Appl.acct_sent <- s.acct_sent


(**************************************************************)

let dump vf s = Layer.layer_dump_simp name vf s

(**************************************************************)

let init _ (ls,vs) = { 
  sweep	       = Param.time vs.params "pbcast_sweep" ;
  stale        = Param.int vs.params "pbcast_stale" ;
  fanout       = Param.int vs.params "pbcast_fanout" ;
  reql         = Param.int vs.params "pbcast_reql" ;
  max_entropy  = Param.int vs.params "pbcast_max_entropy" ;

  msg_overhead = 100 ;
  disseminate  = Param.bool vs.params "pbcast_disseminate" ;
  round        = 0 ;
  next_gossip  = Time.invalid ;
  wait         = Param.int vs.params "pbcast_wait" ;
  retrans      = Hashtbl.create 107 ;

  casts        = Array.init ls.nmembers (fun _ -> Iq.create name Lost) ;
  recd         = Array.create ls.nmembers 0 ;
  free         = Array.create ls.nmembers 0 ;
  high         = Array.create ls.nmembers 0 ;
  failed       = ls.falses ;
  gossip       = Arrayf.gossip ls.falses ls.rank ;

  acct_recd    = 0 ;
  acct_lost    = 0 ;
  acct_retrans = 0 ;
  acct_bad_ret = 0 ;
  acct_sent    = 0
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
(*let ack = make_acker name dnnm in*)
  let log = Trace.log2 name ls.name in	(* general *)
  let logr = Trace.log2 (name^"R") ls.name in (* reliability, messages retransmission *)
  let logrdd = Trace.log2 (name^"RDD") ls.name in (* redundant retransmission *)
  let logg = Trace.log2 (name^"G") ls.name in (* gossipping status *)
  let loga = Trace.log2 (name^"A") ls.name in (* accounting summaries *)
  let logb = Trace.log2 "PBCASTB"(*"BUFFER"*) ls.name (*(name^":"^ls.name)*) in

  let check_deliver rank =
    if rank = ls.rank then failwith sanity ;
    Iq.read_prefix s.casts.(rank) (fun seqno iov msg ->
      log (fun () -> sprintf "deliver:(%d,%d)" rank seqno) ;
      array_incr s.recd rank ;  
      match msg with
      | Msg(_, abv) ->
	  s.acct_recd <- succ s.acct_recd ;
          update_appl_info s ;
          up (create name (ECast iov) [Peer rank]) abv 

      |	Lost ->
	  s.acct_lost <- succ s.acct_lost ;
          update_appl_info s ;
	  upnm (create name ELostMessage[Peer rank])
    )
  in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | (ECast iov|ESend iov), Data(rank,seqno,retrans) ->
      if rank = ls.rank then 
	failwith "sanity:retransmit of my message" ;
      log (fun () -> sprintf "got:(%d,%d)" rank seqno) ;
      s.high.(rank) <- max s.high.(rank) seqno ;

      if seqno >= s.free.(rank)
      && Iq.assign s.casts.(rank) seqno iov (Msg(s.round, abv))
      then (
	Hashtbl.remove s.retrans (rank, seqno) ;  (* opt to restrict tb size *)
	if retrans then (
	  s.acct_retrans <- succ s.acct_retrans ;
          update_appl_info s
        ) ;
	check_deliver rank ;
      ) else (
	s.acct_bad_ret <- succ s.acct_bad_ret ;
        update_appl_info s ;
	logrdd (fun () -> sprintf "dropping redundant cast (%d,%d)" rank seqno)
      )

  | _, NoHdr -> up ev abv
  | _, _     -> failwith bad_up_event


  and uplm_hdlr ev hdr = match getType ev, hdr with
  | (ECast _|ESendUnrel _), Gossip(round, avail) ->
      if getPeer ev = ls.rank then failwith "gossip from myself" ;
      let requests = ref [] in
      let nrequests = ref 0 in
      for rank = 0 to pred ls.nmembers do 
      	if rank <> ls.rank 
	&& Arrayf.get avail rank > s.recd.(rank) 
	&& !nrequests < s.reql then (
	  s.high.(rank) <- max s.high.(rank) (Arrayf.get avail rank) ;
	  for seqno = s.high.(rank) downto s.recd.(rank) do  (* the last msg likely cause retrans *)
	    if !nrequests < s.reql  then (
	      match Iq.get s.casts.(rank) seqno with
	      | Iq.GData(_, Msg(_, _)) -> ()
	      |	_ ->
		  try 
		    let round = Hashtbl.find s.retrans (rank, seqno) in
		    if round + s.wait < s.round then (
		      Hashtbl.remove s.retrans (rank, seqno) ;
		      Hashtbl.add s.retrans (rank, seqno) s.round ;
		      requests := (rank, seqno) :: !requests ;
		      incr nrequests
		    )
		  with Not_found -> (
		    Hashtbl.add s.retrans (rank, seqno) s.round ;
		    requests := (rank, seqno) :: !requests ;
		    incr nrequests
		  )
	    )
	  done ;
	) ;
      done ;

      if !nrequests > 0 then (
	requests := List.rev !requests ;
      	logr (fun () -> sprintf "requesting=%s"
	    (string_of_list (fun (a,b) -> sprintf "(%d,%d)" a b) !requests)) ;
	let requests = Arrayf.of_list !requests in
      	dnlm (sendPeer name (getPeer ev)) (Request(round, requests)) ;
      ) ;
      
      (*ack ev ;*) free name ev


  | (ECast _ |ESend _), Request(round,requests) ->
      if getPeer ev = ls.rank then failwith "request from myself" ;
      if round <> s.round then (  
	logg (fun () -> sprintf "rank=%d round=%d origin=%d old-gossip=%d"
	    ls.rank s.round (getPeer ev) round)
      ) else  (
      	let origin = getPeer ev in
	let sent = ref 0 in
	let nrequests = Arrayf.length requests in
	let serviced = ref 0 in
	let rec loop i = 
	  if !sent < s.max_entropy && i < nrequests then (
	    let (rank, seqno) = Arrayf.get requests i in
	    begin match Iq.get s.casts.(rank) seqno with
	    | Iq.GData(iov,Msg(_, abv)) ->
		incr serviced ;
	      	dn (sendPeerIov name origin iov) abv (Data(rank,seqno,true)) ;
	      	sent := !sent + succ (int_of_len (Iovecl.len iov)) + s.msg_overhead ;
                s.acct_sent <- succ s.acct_sent ;
	    | _ -> ()
	    end ;
	    loop (succ i)
	  )
	in loop 0 ;
	logr (fun () -> sprintf "Request():(rank=%d) serviced %d/%d" ls.rank !serviced nrequests) ;
      ) ;
      (*ack ev ;*) free name ev

  | _ -> failwith unknown_local


  and upnm_hdlr ev = match getType ev with

    (* EInit: request a first timer alarm to bootstrap things.
     *)
  | EInit ->
      dnnm (timerAlarm name Time.zero) ;
      upnm ev

    (* EFail: mark the failed members and check if any
     * messages are now stable.
     *)
  | EFail ->
      s.failed <- getFailures ev ;
      s.gossip <- Arrayf.gossip s.failed ls.rank ;
      upnm ev

  | EBlockOk ->
      let ev = Event.set name ev [NoVsync] in
      upnm ev

    (* When timer expires:
     * 1. Increment rounds
     * 2. Gossip.
     * 3. Clean out stale messages.
     *)
  | ETimer ->
      let time = getTime ev in
      if time >= s.next_gossip then (
	s.next_gossip <- Time.add time s.sweep ;
	dnnm (timerAlarm name s.next_gossip) ;   (* request next gossip *)
	s.round <- succ s.round ;

	for rank = 0 to pred ls.nmembers do
	  let casts = s.casts.(rank) in
	  
	  (* Figure out the new free pointer for this member.
	   *)
	  let rec loop seqno free =
	    if seqno > s.high.(rank) then free 
	    else (
	      match Iq.get casts seqno with
	      |	Iq.GData(_, Msg(round, _)) ->
		  if round + s.stale >= s.round then free
		  else loop (succ seqno) (succ seqno)
	      |	Iq.GReset -> failwith "got Reset" ; 0
	      |	_ ->
		  loop (succ seqno) free
	    )
	  in
	  let free = loop s.free.(rank) s.free.(rank) in

	  (* Deliver any messages not seen yet.  Others are
	   * considered lost.
	   *)
	  for seqno = s.recd.(rank) to (pred free) do   (* May claim having msgs which are lost *)
	    match Iq.get casts seqno with
	    | Iq.GData(_, Msg(_, _)) -> ()
	    | Iq.GUnset ->
		ignore (Iq.assign casts seqno Iovecl.empty Lost)
	    | _ -> failwith (sanityn 1)
          done ;

	  if rank <> ls.rank then check_deliver rank ;
	  if s.recd.(rank) < free then failwith (sanityn 2) ;

	  (* Set the new values of the free.  High
	   * should not have changed.
	   *)
	  s.free.(rank) <- free ;
	  Iq.set_lo casts free ;
	  log (fun () -> sprintf "head advanced:(%d,%d)" rank free) ;
	done ;

	(* Send out gossip for this round.
	 *)
	let dests = Arrayf.choose s.gossip s.fanout in
	
	if dests <> [] then (
	  logg (fun () -> sprintf "rank=%d round=%d gossipping to %s" 
	    ls.rank s.round (string_of_int_list dests)) ;
	  List.iter (fun dest ->
	    dnlm (sendUnrelPeer name dest) (Gossip(s.round, Arrayf.of_array s.recd)) ;
	  ) dests
	)
      ) ;
      upnm ev

  | EAccount ->
      loga (fun () -> sprintf "(rank=%d) acct_recd=%d acct_lost=%d retrans=%d redundant=%d" 
	  ls.rank s.acct_recd s.acct_lost s.acct_retrans s.acct_bad_ret) ;
      upnm ev

  | EDump -> dump vf s ; upnm ev
  | _ -> upnm ev


  and dn_hdlr ev abv = match getType ev with
    (* ECast: Increment my recd counter.  Then pass the event down.
     *)
  | ECast iov ->
      s.acct_recd <- succ s.acct_recd ;
      update_appl_info s ;
      ignore (Iq.assign s.casts.(ls.rank) s.recd.(ls.rank) iov (Msg(s.round,abv))) ;
      if s.disseminate then 
        dn ev abv (Data(ls.rank, s.recd.(ls.rank), false))
      else free name ev ;
      array_incr s.recd ls.rank ;
      s.high.(ls.rank) <- max s.high.(ls.rank) s.recd.(ls.rank) ;

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = 
  Param.default "pbcast_sweep" (Param.Time (Time.of_string "0.100")) ;
  Param.default "pbcast_stale" (Param.Int 20) ;
  Param.default "pbcast_fanout" (Param.Int 1) ;
  Param.default "pbcast_reql" (Param.Int 100) ;
  Param.default "pbcast_wait" (Param.Int 2) ;
  Param.default "pbcast_disseminate" (Param.Bool true) ;
  Param.default "pbcast_max_entropy" (Param.Int 30000) ;
  Layer.install name l

(**************************************************************)


