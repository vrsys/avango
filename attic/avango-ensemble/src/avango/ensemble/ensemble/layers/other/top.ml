(**************************************************************)
(* TOP.ML *)
(* Top-most layer.  Handles default group behavior *)
(* Author: Mark Hayden, 7/95 *)
(* Ohad Rodeh : added authorization check 1/99 *)
(* Based on code by Robbert vanRenesse *)
(**************************************************************)
open Layer
open View
open Event
open Trans
open Util
(**************************************************************)
let name = Trace.filel "TOP"
(**************************************************************)
(* State transitions for this layer:

  OLD_STATE:
    NEW_STATE: event

  Normal:
    Merging: EBlockOk
    NextView: EView, EBlockOk

  Merging:
    NextView: EView, EMergeFailed, EMergeDenied

  NextView:
    *sink*
*)
(**************************************************************)

type header = NoHdr

type states =
  | Normal
  | Merging
  | NextView

type state = {
  policy                : (Addr.set -> bool) option ; (* Authorization policy *)
  sweep			: Time.t ;	(* gossip sweep interval (if any) *)
  gossip                : bool ;	(* initiate EGossipExt events? *)
  account               : bool ;	(* initiate EAccount events? *)
  dump_linger           : bool ;	(* dump when this stack has been lingering *)
  dump_fail             : bool ;	(* fail on dump events *)
  mutable state		: states ;	(* state of the protocol *)
  mutable elected	: bool ;	(* have I been elected *)
  mutable next_sweep	: Time.t ;	(* time of next sweep *)
  mutable suspects      : bool Arrayf.t ; (* suspected members *)
  mutable failed        : bool Arrayf.t ;(* failed members *)
  mutable mergers	: View.state list ; (* Mergers for next view *)
  mutable dn_block	: bool ;	(* I've done a EBlock *)
  mutable up_prompt     : bool ;        (* I've been prompted *)
  mutable leaving       : bool ;	(* Are we leaving? *)
  dn_block_abv	        : Once.t ;	(* EBlock was from above! *)
  up_block_ok           : Once.t ;	(* I've recd an EBlockOk *)
  mutable dbg_block_time : Time.t ;	(* time when I blocked *)
  mutable dbg_block_time_first : Time.t	(* time when I blocked *)
}

(**************************************************************)

let string_of_state = function
| Normal	-> "Normal"
| Merging	-> "Merging"
| NextView	-> "NextView"

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "state=%s leaving=%b\n" (string_of_state s.state) s.leaving ;
  sprintf "dn_block=%b elected=%b\n" s.dn_block s.elected ;
  sprintf "failed=%s\n" (Arrayf.bool_to_string s.failed) ;
  sprintf "dbg_block_time_first=%s\n" (Time.to_string s.dbg_block_time_first)
|])

let check_policy s a =
  match s.policy with
  | None -> true
  | Some f -> f a

let init s (ls,vs) = {
  policy        = s.exchange;
  sweep         = Param.time vs.params "top_sweep" ;
  gossip        = Param.bool vs.params "top_gossip" ;
  account       = Param.bool vs.params "top_account" ;
  dump_linger   = Param.bool vs.params "top_dump_linger" ;
  dump_fail     = Param.bool vs.params "top_dump_fail" ;
  state		= Normal ;
  failed	= ls.falses ;
  suspects      = ls.falses ;
  leaving       = false ;
  dn_block	= false ;
  dn_block_abv  = Once.create "TOP:dn_block_abv" ;
  up_prompt     = false ;
  up_block_ok   = Once.create "TOP:up_block_ok" ;
  mergers	= [] ;
  next_sweep	= Time.invalid ;
  elected	= false ;
  dbg_block_time = Time.zero ;
  dbg_block_time_first = Time.invalid
}

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let log = Trace.log2 name ls.name in
  let logs = Trace.log3 Layer.syncing ls.name name in
  let logb = Trace.logl3 Layer.block ls.name name in
(*let ack = make_acker name dnnm in*)
  let failwith = layer_fail dump vf s name in

  let do_block () =
    (* Checking am_coord is a hack for singleton's leaving
     * a group.
     *)
    if (ls.am_coord || s.elected) && not s.dn_block then (
      if s.state = Merging then
	failwith "Merging w/o EBlock" ;
      s.dn_block <- true ;
      dnnm (create name EBlock[])
    )
  in

  let do_fail () =
    if s.elected && not (Arrayf.super s.failed s.suspects) then (
      do_block () ;
      s.failed   <- Arrayf.map2 (||) s.failed s.suspects ;
      s.suspects <- s.failed ;
      dnnm (create name EFail[(Failures s.failed)])
    )
  in

  let do_view () =
    logs (fun () -> "delivering EView") ;
    if not s.elected then failwith "do_view when not coord" ;

    (* Remove failed members from the view.
     *)
    let new_vs = View.fail vs s.failed in

    (* Merge new members into the view.
     *) 
    let new_vs = View.merge (new_vs::s.mergers) in

    dnnm (create name EView[ViewState new_vs]) ;
    s.state <- NextView ;
    s.mergers <- []
  in
  
  let up_hdlr ev abv hdr = failwith "up_hdlr"

  and uplm_hdlr ev hdr = match getType ev with
  | _ -> failwith unknown_local

  and upnm_hdlr ev = (match getType ev with
  | EView ->
      logs (fun () -> "got EView") ;
      log (fun () -> Event.to_string ev) ;
      s.state <- NextView ;
      upnm ev

    (* Don't pass the leave event up.  The do_block
     * is necessary in case we are a singleton group.
     * The suspicions will not necessarily cause a
     * view change.  The Exit is needed in case we
     * are running with the groupd.
     *)
  | ELeave ->
      do_block () ;
      s.leaving <- true ;
      dnnm (create name EExit[]) ;
      free name ev

  | EFail ->
      log (fun () -> Event.to_string ev) ;
      s.failed   <- Arrayf.map2 (||) s.failed (getFailures ev) ;
      s.suspects <- Arrayf.map2 (||) s.suspects s.failed ;

  | EMergeRequest ->
      if s.state <> NextView then (
	let mergers = getViewState ev in
	let mview = Lset.inject mergers.view in
	let check_disjoint = 
	  List.for_all (fun vs ->
	    let view = Lset.inject vs.view in
	    Lset.disjointa view mview
	  ) s.mergers
	in
	if not check_disjoint then
	  failwith "repeating mergers" ;
	s.mergers <- mergers :: s.mergers ;
	log (fun () -> Event.to_string ev) ;
	do_block () 
      ) ;
      free name ev

  | EMergeFailed | EMergeDenied ->
      (* Should do this only if it was the coordinator,
       * otherwise, should just fail the member. [MH: ???]
       *)
      if s.state = Merging then
	do_view ()

  | ESuspect ->
      log (fun () -> Event.to_string ev) ;
      let suspects = getSuspects ev in
      s.suspects <- Arrayf.map2 (||) s.suspects suspects ;
      if s.elected then 
	do_fail ()

  | EBlock ->
      dnnm (create name EBlockOk[])

  | EBlockOk ->
      log (fun () -> Event.to_string ev) ;
      Once.set s.up_block_ok ;

      (* If we got a EBlock from above (from the groupd),
       * then we pass the EBlockOk up.  Otherwise we
       * generate an EView or an EMerge event.  
       *)
      if Once.isset s.dn_block_abv then (
	upnm ev
      ) else (
	if not s.dn_block then 
	  failwith "EBlockOk w/o EBlock" ;
	
	if s.state = Normal then (
	  (* Check if merging.	
	   *)
	  try 
	    let contact,view_id = getContact ev in
	    match view_id, (Arrayf.to_ranks s.failed), s.mergers with
	    | Some(view_id), [], [] ->
		log (fun () -> sprintf "sending MergeRequest") ;
	      	dnnm (create name EMergeRequest[
		  Contact(contact,Some view_id)
		]) ;
	      	s.state <- Merging
	    | _ -> raise (Failure "escape out")
	  with _ ->
	    do_view ()
        )
      )

  | ETimer ->
      let time = getTime ev in
      if Time.ge time s.next_sweep then (
	s.next_sweep <- Time.add time s.sweep ;
	dnnm (timerAlarm name s.next_sweep) ;
	if ls.am_coord && s.gossip then
	  dnnm (create name EGossipExt []) ;
	if s.account then
	  dnnm (create name EAccount []) ;
      ) ;

      if s.state = Normal then (
	s.dbg_block_time <- time
      ) else (
	if s.dump_linger && Time.gt time (Time.add s.dbg_block_time (Time.of_int 200)) then (
	  if s.dbg_block_time_first = Time.invalid then
	    s.dbg_block_time_first <- s.dbg_block_time ;
	  eprintf "TOP:lingering stack, time=%s\n" (Time.to_string time) ;
	  s.dbg_block_time <- time ;
	  dump vf s ;
	  dnnm (create name EDump[])
	)
      )

  | EProtocol ->			(*Just here for view change perf tests*)
      upnm (create name EPrompt[]) ;
      free name ev

  | EPrompt ->
      do_block () ;
      s.up_prompt <- true ;
      upnm ev

  | EElect ->
      s.elected <- true ;
      dnnm (timerAlarm name Time.zero) ;
      
      (* If prompted, then start blocking.
       *)
      if s.up_prompt then
	do_block () ;

      do_fail ()

  | EAccount ->
      logb (fun () -> [
  	sprintf "view=%s" (View.to_string vs.view) ;
  	sprintf "state=%s dn_block=%b elected=%b leaving=%b" 
	  (string_of_state s.state) s.dn_block s.elected s.leaving ;
  	sprintf "failed=%s" (Arrayf.bool_to_string s.failed)
      ]) ;
      free name ev

  | EDump ->
      dump vf s ;
      if s.dump_fail then (
	eprintf "TOP:got EDump:exiting\n" ;
	exit 1 ;
      )

  | EInit ->
      (* If there are any unauthorized members then remove them.
       * Return true if this is the case --- we remove them.
       *)
      let suspicions =
	Arrayf.map2 (fun e addr -> not (check_policy s addr)) 
	  vs.view vs.address
      in
      if Arrayf.exists (fun i x -> x) suspicions then (
	log (fun () -> sprintf "removing untrusted members:%s" 
	  (string_of_bool_array (Arrayf.to_array suspicions))
	) ;
	dnnm (suspectReason name suspicions name)
      )

(*
  | EStable ->
      eprintf "TOP:stable:%s\n" (string_of_int_array (getStability ev)) ;
*)
  | EExit ->
      if s.leaving then
	upnm ev
      else
	free name ev

  | EAsync 
  | EStable 
  | EGossipExt 
  | EXferDone 
  | ELostMessage 
  | EStableReq
  | ESyncInfo
  | EPresent
  | ERekey -> free name ev
  | _ -> eprintf "TOP:dropping:%s\n" (to_string ev )
  ) (*; free name ev: BUG VIEW *)

  and dn_hdlr _ _ = failwith "got down event"
  and dnnm_hdlr ev = match getType ev with
  | EExit ->
      dnnm ev

  | EFail ->
      (* The group daemon is not so good about sending
       * monotonically increasing failures.
       *)
      let failures = getFailures ev in
      s.failed <- Arrayf.map2 (or) s.failed failures ;
      if Arrayf.get s.failed ls.rank then
	failwith "dnnm:failure of myself" ;
      dnnm (set name ev [Failures s.failed])

  | EBlock ->
      log (fun () -> sprintf "EBlock") ;
      (* This was causing problems with big pbcast tests...
       *)
      if Once.isset s.dn_block_abv then (
	eprintf "TOP:warning:got 2nd EBlock from above, sending EBlockOk (hack!)\n" ;
	upnm (create name EBlockOk[]) ; (* hack! *)
      ) else (
	Once.set s.dn_block_abv ;
        if s.dn_block then 
	  failwith "EBlock from above when blocked" ;
        s.dn_block <- true ;
        dnnm (create name EBlock[]) ;	(*???*)
(*
        if Once.isset s.dn_block_abv then ( (*???*)
	  upnm (create name EBlockOk[]) ;	(*???*)
        )
*)
      )

  | _ -> failwith "unknown dnnm event"

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let _ = Trace.comment "TOP:broken (???)"

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = 
  Param.default "top_sweep" (Param.Time (Time.of_int 5)) ;
  Param.default "top_gossip" (Param.Bool true) ;
  Param.default "top_account" (Param.Bool true) ;
  Param.default "top_dump_linger" (Param.Bool false) ;
  Param.default "top_dump_fail" (Param.Bool false) ;
  Layer.install name l

(**************************************************************)
