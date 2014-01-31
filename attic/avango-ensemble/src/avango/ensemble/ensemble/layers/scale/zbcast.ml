(**************************************************************)
(* ZBCAST.ML : Gossip style broadcast protocol *)
(* Author: Zhen Xiao, 10/98 *)
(**************************************************************)
open Trans
open Layer
open Buf
open Event
open Util
open View
(*module Hashtbl = Hashtble*)
(**************************************************************)
let name = Trace.filel "ZBCAST"
(**************************************************************)

type round = seqno

type header = NoHdr
  | Data of rank * seqno * bool (*retrans?*) * bool (*cast?*)
  | Gossip of round * (seqno Arrayf.t)
  | Request of round * ((rank * seqno) Arrayf.t)

type 'abv item =
  | Msg of round * round * 'abv
  | Lost

type 'abv state = {
  mutable npolls  : int ;
  max_polls       : int ;
  mutable naked   : int array ;

  req_limit     : int ;
  req_tbl       : ((rank * seqno), int) Hashtbl.t ;

  reply_limit   : int ;
  reply_tbl     : ((rank * seqno), int) Hashtbl.t ;

  mutable datasent : int ;
  max_entropy      : int ;

  mutable nreqs : int ;
  max_reqs      : int ;

  fanout : int ;
  sweep	 : Time.t ;
  mutable next_gossip : Time.t ;
  mutable failed : bool Arrayf.t ;
  mutable gossip : rank Arrayf.t ;

  stale         : round ;
  idle          : round ;
  new_gc        : bool ;
  msg_overhead  : int ;
  disseminate   : bool ;
  mutable round : round ;
  latency       : int ;

  casts		: 'abv item Iq.t array ;
  recd  	: seqno array ;
  free  	: seqno array ;
  high  	: seqno array ;

  mutable acct_recd : seqno ;		(* # delivered messages *)
  mutable acct_lost : seqno ;		(* # lost messages *)
  mutable acct_retrans : seqno ;      	(* # retrans messages *)
  mutable acct_bad_ret : seqno ;        (* # retrans messages not used *)
  mutable acct_sent : seqno             (* # retrans msgs sent by *)
}

let update_appl_info s =
  Appl.cast_info.Appl.acct_recd <- s.acct_recd ;
  Appl.cast_info.Appl.acct_lost <- s.acct_lost ;
  Appl.cast_info.Appl.acct_retrans   <- s.acct_retrans ;
  Appl.cast_info.Appl.acct_bad_ret   <- s.acct_bad_ret;
  Appl.cast_info.Appl.acct_sent <- s.acct_sent


(**************************************************************)

let dump vf s = Layer.layer_dump_simp name vf s

(**************************************************************)

let init _ (ls,vs) = 
  let req_limit = Param.int vs.params "zbcast_req_limit" in
  let reply_limit = Param.int vs.params "zbcast_reply_limit" in
  let max_entropy = Param.int vs.params "zbcast_max_entropy" in
  let latency = Param.int vs.params "zbcast_latency" in
  let reql = Param.int vs.params "zbcast_reql" in
  { 
    npolls    = 0 ;
    max_polls = Param.int vs.params "zbcast_max_polls" ;
    naked     = Array.create ls.nmembers 0 ;

    req_limit    = req_limit ;
    req_tbl      = Hashtbl.create (*name*) 107 ;

    reply_limit  = reply_limit ;
    reply_tbl    = Hashtbl.create (*name*) 107 ;

    datasent     = 0 ;
    max_entropy  = max_entropy ;

    nreqs        = 0 ;
    max_reqs     = Param.int vs.params "zbcast_max_reqs" ;

    fanout       = Param.int vs.params "zbcast_fanout" ;
    sweep	 = Param.time vs.params "zbcast_sweep" ;
    next_gossip  = Time.invalid ;
    failed       = ls.falses ;
    gossip       = Arrayf.gossip ls.falses ls.rank ;

    stale        = Param.int vs.params "zbcast_stale" ;
    idle         = Param.int vs.params "zbcast_idle" ;
    new_gc       = Param.bool vs.params "zbcast_new_gc" ;
    msg_overhead = 100 ;
    disseminate  = Param.bool vs.params "zbcast_disseminate" ;
    round        = 0 ;
    latency      = latency ;
    
    casts        = Array.init ls.nmembers (fun _ -> Iq.create name Lost) ;
    recd         = Array.create ls.nmembers 0 ;
    free         = Array.create ls.nmembers 0 ;
    high         = Array.create ls.nmembers 0 ;
    
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
  let logb = Trace.log2 "ZBCASTB"(*"BUFFER"*) ls.name (*(name^":"^ls.name)*) in

  let check_deliver rank =
    if rank = ls.rank then failwith sanity ;
    Iq.read_prefix s.casts.(rank) (fun seqno iov msg ->
      log (fun () -> sprintf "deliver:(%d,%d)" rank seqno) ;
      array_incr s.recd rank ;  
      match msg with
      | Msg(_, _, abv) ->
	  s.acct_recd <- succ s.acct_recd ;
          update_appl_info s ;
          up (create name (ECast iov) [Peer rank]) abv 

      |	Lost ->
	  s.acct_lost <- succ s.acct_lost ;
          update_appl_info s ;
	  upnm (create name ELostMessage[Peer rank])
    )
  in

  let check_gap rank =
    let casts = s.casts.(rank) in
    let reqSends = ref [] in
    let reqCasts = ref [] in
    for seqno = s.recd.(rank) to s.high.(rank) do
      if seqno > s.naked.(rank) && s.npolls < s.max_polls then (
	match Iq.get casts seqno with
	| Iq.GData(_, Msg(_, _, _)) -> ()
	| _ -> 
	    s.naked.(rank) <- int_max s.naked.(rank) seqno ;
	    s.npolls <- succ s.npolls ;
	    begin	    
	      try
	      	let counter = Hashtbl.find s.req_tbl (rank, seqno) in
	      	let counter = succ counter in
	      	Hashtbl.remove s.req_tbl (rank, seqno) ;
	      	if counter < s.req_limit then (
		  Hashtbl.add s.req_tbl (rank, seqno) counter ;
		  reqSends := (rank, seqno) :: !reqSends ;
		) else (
		  reqCasts := (rank, seqno) :: !reqCasts ;
	        )
	      with Not_found ->
	      	Hashtbl.add s.req_tbl (rank, seqno) 1 ;
	      	reqSends := (rank, seqno) :: !reqSends
	    end
      )
    done ;
    if !reqSends <> [] then (
      let dests = Arrayf.choose s.gossip s.fanout in
      let reqSends = Arrayf.of_list !reqSends in
      List.iter (fun dest ->
	dnlm (sendPeer name dest) (Request(s.round, reqSends)) ;
      ) dests
    ) ;
    if !reqCasts <> [] then (
      let reqCasts = Arrayf.of_list !reqCasts in
      dnlm (castEv name) (Request(s.round, reqCasts)) ;
    )
  in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | (ECast iov|ESend iov), Data(rank, seqno, retrans, is_cast) ->
      if rank <> ls.rank then (
	log (fun () -> sprintf "got:(%d,%d)" rank seqno) ;
	if is_cast then (
	  Hashtbl.remove s.reply_tbl (rank, seqno) ;
	) ;
	s.high.(rank) <- max s.high.(rank) seqno ;
	check_gap rank ;
	if seqno >= s.free.(rank)
	&& Iq.assign s.casts.(rank) seqno iov (Msg(s.round, s.round, abv))
	then (
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
      )

  | _, NoHdr -> up ev abv
  | _, _     -> failwith bad_up_event


  and uplm_hdlr ev hdr = match getType ev, hdr with
  | (ECast _|ESendUnrel _), Gossip(round, avail) ->
      if getPeer ev = ls.rank then failwith "gossip from myself" ;
      let requests = ref [] in
      for rank = 0 to pred ls.nmembers do 
      	if rank <> ls.rank 
	&& Arrayf.get avail rank > s.recd.(rank) 
	&& s.nreqs < s.max_reqs then (
	  s.high.(rank) <- max s.high.(rank) (Arrayf.get avail rank) ;
	  for seqno = s.high.(rank) downto s.recd.(rank) do  (* the last msg likely cause retrans *)
	    if s.nreqs < s.max_reqs  then (
	      match Iq.get s.casts.(rank) seqno with
	      | Iq.GData(_, Msg(_, _, _)) -> ()
	      |	_ -> 
		  requests := (rank, seqno) :: !requests ;
		  s.nreqs <- succ s.nreqs ;
	    )
	  done ;
	) ;
      done ;

      (* For most-recent-first retransmission, reverse reqSends and reqCasts lists *)
      if !requests <> [] then (
	let requests = Arrayf.of_list !requests in
      	dnlm (sendPeer name (getPeer ev)) (Request(round, requests)) ;
      ) ;

      (*ack ev ;*) free name ev


  | (ECast _|ESend _), Request(round, requests) ->
      if getPeer ev = ls.rank then failwith "request from myself" ;
      if round <> s.round then (  
	logg (fun () -> sprintf "rank=%d round=%d origin=%d old-gossip=%d"
	    ls.rank s.round (getPeer ev) round)
      ) else  (
      	let origin = getPeer ev in
	let nreqs = Arrayf.length requests in
	let serviced = ref 0 in
	let rec loop i = 
	  if s.datasent < s.max_entropy && i < nreqs then (
	    let (rank, seqno) = Arrayf.get requests i in
	    (* Suppress my cast request *)
	    if getCompactType ev = C_ECast then (
	      Hashtbl.remove s.req_tbl (rank, seqno) ;
	    ) ;
	    begin match Iq.get s.casts.(rank) seqno with
	    | Iq.GData(iov,Msg(round, _, abv)) -> 
		incr serviced ;
		if s.new_gc then (
		  ignore (Iq.msg_update s.casts.(rank) seqno (Msg(round, s.round, abv)))
		) ;
		(* If the request is casted, so is the reply *)
		if getCompactType ev = C_ECast then (
		  dn (castPeerIov name rank iov) abv (Data(rank, seqno, true, true)) ;
		  Hashtbl.remove s.reply_tbl (rank, seqno) ;
		) else (
		  begin
		    try
		      let counter = Hashtbl.find s.reply_tbl (rank, seqno) in
		      let counter = succ counter in
		      Hashtbl.remove s.reply_tbl (rank, seqno) ;
		      if counter < s.reply_limit then (
		      	Hashtbl.add s.reply_tbl (rank, seqno) counter ;
	      	      	dn (sendPeerIov name origin iov) abv (Data(rank, seqno, true, false))
		      ) else (
		      	dn (castPeerIov name rank iov) abv (Data(rank, seqno, true, true)) ;
		      )
		    with Not_found -> 
		      Hashtbl.add s.reply_tbl (rank, seqno) 1 ;
	      	      dn (sendPeerIov name origin iov) abv (Data(rank, seqno, true, false))
	    	  end ;
		) ;
	      	s.datasent <- s.datasent + succ (int_of_len (Iovecl.len iov)) + s.msg_overhead ;
                s.acct_sent <- succ s.acct_sent ;
	    | _ -> ()
	    end ;
	    loop (succ i)
	  )
	in loop 0 ;
	logr (fun () -> sprintf "Request():(rank=%d) serviced %d/%d" ls.rank !serviced nreqs) ;
      ) ;
      (*ack ev ;*) free name ev

  | _ -> failwith unknown_local


  and upnm_hdlr ev = match getType ev with

    (* EInit: request a first timer alarm to bootstrap things.
     *)
  | EInit ->
      dnnm (timerAlarm name Time.zero) ;
      upnm ev

    (* EFail: mark the failed members so that we don't gossip to them.
     *)
  | EFail ->
      s.failed <- getFailures ev ;
      s.gossip <- Arrayf.gossip s.failed ls.rank ;
      upnm ev

  | ETimer ->
      let time = getTime ev in
      if time >= s.next_gossip then (
	s.next_gossip <- Time.add time s.sweep ;
	dnnm (timerAlarm name s.next_gossip) ;   (* request next gossip *)
	
	s.round <- succ s.round ;
	s.nreqs <- 0 ;
	s.npolls <- 0 ;
	s.datasent <- 0 ;
	s.naked <- Array.create ls.nmembers 0 ;
	for rank = 0 to pred ls.nmembers do
	  let casts = s.casts.(rank) in
	  
	  (* Figure out the new free pointer for this member.
	   *)
	  let rec loop seqno free =
	    if seqno > s.high.(rank) then free 
	    else (
	      match Iq.get casts seqno with
	      |	Iq.GData(_, Msg(round, visit, _)) ->
		  if s.new_gc then (
		    if visit + s.idle >= s.round then free
		    else loop (succ seqno) (succ seqno)
		  ) else (
		    if round + s.stale >= s.round then free
		    else loop (succ seqno) (succ seqno)
		  )
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
	    | Iq.GData(_, Msg(_, _, _)) -> 
		Hashtbl.remove s.reply_tbl (rank, seqno) ;
	    | Iq.GUnset ->
		ignore (Iq.assign casts seqno Iovecl.empty Lost)
	    | _ -> failwith (sanityn 2)
          done ;

	  if rank <> ls.rank then check_deliver rank ;
	  if s.recd.(rank) < free then failwith (sanityn 3) ;

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
	    let recd = Arrayf.of_array s.recd in
	    dnlm (sendUnrelPeer name dest) (Gossip(s.round, recd)) ;
	  ) dests
	) ;
      ) ;
      upnm ev

  | EBlockOk ->
      let ev = Event.set name ev [NoVsync] in
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
      ignore (Iq.assign s.casts.(ls.rank) s.recd.(ls.rank) iov (Msg(s.round, s.round, abv))) ;
      if s.disseminate then 
        dn ev abv (Data(ls.rank, s.recd.(ls.rank), false, true))
      else free name ev ;
      array_incr s.recd ls.rank ;
      s.high.(ls.rank) <- max s.high.(ls.rank) s.recd.(ls.rank) ;

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr ev = dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = 
  Param.default "zbcast_fanout" (Param.Int 1) ;
  Param.default "zbcast_sweep" (Param.Time (Time.of_string "0.100")) ;
  Param.default "zbcast_stale" (Param.Int 20) ;
  Param.default "zbcast_idle" (Param.Int 20) ;
  Param.default "zbcast_new_gc" (Param.Bool true) ;
  Param.default "zbcast_max_polls" (Param.Int 5) ;
  Param.default "zbcast_max_reqs" (Param.Int 20) ;
  Param.default "zbcast_max_entropy" (Param.Int 20000) ;
  Param.default "zbcast_req_limit" (Param.Int 20) ;
  Param.default "zbcast_reply_limit" (Param.Int 6) ;
  Param.default "zbcast_latency" (Param.Int 0) ;
  Param.default "zbcast_reql" (Param.Int 5) ;
  Param.default "zbcast_disseminate" (Param.Bool true) ;
  Layer.install name l

(**************************************************************)
