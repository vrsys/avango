(**************************************************************)
(* MERGE.ML : reliable merge protocol *)
(* Author: Mark Hayden, 4/95 *)
(* Based on code by: Robbert vanRenesse *)
(**************************************************************)
(* TODO:

 * BUG: should implement acks for merge_denied

 *)
(**************************************************************)
open Util
open Layer
open Event
open View
open Trans
(**************************************************************)
let name = Trace.filel "MERGE"
let failwith = Trace.make_failwith name
(**************************************************************)

type header = NoHdr

type ('abv) state = {
  policy                : (Addr.set -> bool) option ; (* Authorization policy *)
  mutable merging	: bool ;
  mutable next_sweep	: Time.t ;
  mutable new_view	: bool ;
  mutable seqno 	: int ;
  mutable merge_dn 	: (Time.t * dn) list ;
  merge_up 		: ((Addr.set * Endpt.id),(ltime * seqno)) Hashtbl.t ;
  mutable buf		: dn list ;
  sweep			: Time.t ;
  timeout		: Time.t
}

let check_policy s a =
  match s.policy with
  | None -> true
  | Some f -> f a

let init s (ls,vs) = {
  policy        = s.exchange;
  sweep	        = Param.time vs.params "merge_sweep" ;
  timeout 	= Param.time vs.params "merge_timeout" ;
  merging	= false ;		(* are we merging now? *)
  new_view      = false ;	        (* set on EView *)
  seqno      	= 0 ;			(* seqno for messages sent *)
  merge_dn   	= [] ;			(* messages sent *)
  merge_up   	= Hashtbl.create 10 ;   (* messages recd *)
  buf        	= [] ;			(* temporary buffer *)
  next_sweep 	= Time.zero
}
let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let log = Trace.log2 name ls.name in
  let up_hdlr ev abv NoHdr = up ev abv
  and uplm_hdlr ev hdr = failwith unknown_local
			   
  and upnm_hdlr ev = match getType ev with
  | EGossipExtDir -> failwith "sanity"
  | EGossipExt ->
      let merge = Event.getExtender
	(function Event.MergeGos(a,b,c,d) -> (Some (Some (a,b,c,d))) | _ -> None)
	None ev
      in
      begin match merge with 
      |	None -> ()
      |	Some(contact,seqno,kind,merge_vs) ->
	  if fst (fst contact) = ls.endpt 
	  && merge_vs.proto_id = vs.proto_id
	  then (
	    log (fun () -> "got merge gossip") ;
	    (*let (from,saddr),_=  contact in*)
	    let (from,saddr) = View.coord_full merge_vs in
	    if not (check_policy s saddr) then (
	      free name ev
	    ) else (
	      let last_ltime,last_seqno =
		try Hashtbl.find s.merge_up (saddr,from)
		with Not_found -> -1,-1
	      in
	      if (last_ltime,last_seqno) < (merge_vs.ltime,seqno) then (
		(* Remove old entry.
		 *)
		begin
		  try Hashtbl.remove s.merge_up (saddr,from)
		  with Not_found -> ()
		end ;

		Hashtbl.add s.merge_up (saddr,from) (merge_vs.ltime,seqno) ;
		upnm (create name kind [ViewState merge_vs])
	      ) else (
		free name ev
	      )
	    )
	  )
      end ;
      upnm ev
  | EView ->
      s.new_view <- true ;
      upnm ev
	
  | ETimer ->
      let time = getTime ev in
      if s.merging then (
	(* Copy from temporary buffer into merge_dn.
	 *)
	List.iter (fun ev ->
	  dnnm ev ;
	  let ev = copy name ev in
	  s.merge_dn <- (Time.add time s.timeout,ev) :: s.merge_dn
	) s.buf ;
	s.buf <- [] ;

	if Time.ge time s.next_sweep then (
	  s.next_sweep <- Time.add s.next_sweep s.sweep ;

	  List.iter (fun (timeout,ev) ->
	    let typ = getExtendFail (function MergeGos(a,b,c,d) -> Some c | _ -> None) "MergeGos" ev in
	    if Time.gt timeout time then (
	      let ev = copy name ev in
	      dnnm ev
	     ) else if typ = EMergeRequest
		    && not s.new_view
	     then (
	       log (fun () -> sprintf "failed merge request") ;	
	       upnm (create name EMergeFailed[])
	    )
	  ) s.merge_dn
	)
      ) ;

      upnm ev

  | EExit ->
      List.iter (fun (_,dn) ->
      	free name dn
      ) s.merge_dn ;
      s.merge_dn <- [] ;
      upnm ev

  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr ev = match getType ev with

  | EMergeRequest | EMergeGranted | EMergeDenied ->
      let contact = getContact ev in
      log (fun () -> Event.to_string ev) ;
      s.merging <- true ;
      s.seqno 	<- succ s.seqno ;
      let merge_ev = create name EGossipExtDir [
	Address(snd(fst contact)) ;
	MergeGos(contact,s.seqno,(getType ev),(getViewState ev))
      ] in
      s.buf 	<- merge_ev :: s.buf ;
      dnnm merge_ev ;
      dnnm (timerAlarm name Time.zero)
	
  | _ -> dnnm ev


in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = 
  Param.default "merge_sweep" (Param.Time (Time.of_int 1)) ;
  Param.default "merge_timeout" (Param.Time (Time.of_int 15)) ;
  Layer.install name l

(**************************************************************)
