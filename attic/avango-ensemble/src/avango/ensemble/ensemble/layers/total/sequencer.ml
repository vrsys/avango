(****************************************************************)
(* SEQUENCER.ML                                                 *)
(* Author: Roy Friedman, 3/96                                   *)
(* Bug fixes: Mark Hayden, 10/96				*)
(* Bug fixes: Ohad Rodeh, 9/02        	  			*)
(* Based on the C implementation of dynseq but without rotating	*)
(* the sequencer.					        *)
(****************************************************************)
(*                                                              
 * The coordinator of the group acts as a sequencer; in order   
 * to send messages, processes must send the messages pt2pt to  
 * the sequencer, who then broadcasts these messages to         
 * the rest of the group.                                       
 *)
(****************************************************************)
open Layer
open View
open Event
open Util
open Trans
(**************************************************************)
let name = Trace.filel "SEQUENCER"
let failwith = Trace.make_failwith name
(**************************************************************)
(* Headers

 * NoHdr: all non-totally ordered cast messages.

 * Ordered(rank): deliver this message as though from member
 * 'rank'.

 * Unordered: (ie., not ordered by a token holder) Sent at
 * the end of the view.  All members deliver unordered
 * messages in fifo order in order of the sending members
 * rank.
 *)

type header = NoHdr
  | ToSeq
  | Ordered of rank
  | Unordered of seqno

(**************************************************************)

type ('abv,'b) state = {
  seq_rank		: rank ;        (* who is the sequencer *)
  ordered		: seqno array ;
  mutable got_view	: bool ;	(* have we got an EView? *)
  mutable blocking 	: bool ;	(* are we blocking? *)

  (* buffered send messages (waiting for delivery from coordinator) *)
  casts 		: 'abv Iq.t ;

  (* buffered recv messages waiting for the end of the view *)
  up_unord 		: (seqno * 'abv * Iovecl.t)  Queuee.t array
}

(**************************************************************)

let dump (ls,vs) s =
  eprintf "SEQUENCER:dump:%s\n" ls.name ;
  sprintf "blocking=%b\n" s.blocking

(**************************************************************)

let init _ (ls,vs) = {
  seq_rank      = 0 ;			(* always rank 0 *)
  ordered	= Array.create ls.nmembers 0 ;
  blocking 	= false ;
  casts 	= Iq.create name NoMsg ;
  got_view	= false ;
  up_unord 	=
    Array.init ls.nmembers (fun _ -> Queuee.create ())
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let log = Trace.log2 name ls.name in

  let up_hdlr ev abv hdr = match getType ev, hdr with
    | ESend iovl, ToSeq ->
	if ls.rank <> s.seq_rank then
	  failwith "send when not sequencer" ;

	(* Bounce the message back out and deliver locally. 
	 *)
	if not s.blocking then (
	  let origin = getPeer ev in
	  up (castPeerIov name origin iovl) abv ;
	  if ls.nmembers > 1 then (
	    let iovl = Iovecl.copy iovl in
	    dn (castIovAppl name iovl) abv (Ordered origin)
	  ) ;
	  array_incr s.ordered origin ;
	) else 
	  Iovecl.free iovl

    | ECast _, Ordered rank ->
      	if s.got_view then
	  failwith "got ordered after view" ;
	if s.seq_rank = ls.rank then
	  failwith "sequencer got ordered message" ;

      	(* If it's from me, then remove from my iq.
	 *)
	if rank = ls.rank then
	  Iq.set_lo s.casts s.ordered.(rank) ;

      	(* Deliver ordered messages.  Keep track of how many
	 * I've got from each member.
       	 *)
(*	log (fun () -> sprintf "delivering ordered msg from %d: #ordered=%d" rank s.ordered.(rank)) ;*)
	array_incr s.ordered rank ;
	up (set name ev[Peer rank]) abv

    | ECast iovl, Unordered seqno ->
      	(* Buffer unordered messages until the view.
	 *)
	Queuee.add (seqno,abv,iovl) s.up_unord.(getPeer ev) ;

    | _, NoHdr      -> up ev abv (* all other events have NoHdr *)
    | _             -> failwith "non-NoHdr on non ECast"

  and uplm_hdlr ev hdr = failwith "local message event"

  and upnm_hdlr ev = match getType ev with
  | EView ->
      log (fun () -> "EView") ;
      s.got_view <- true ;
      
      (* Deliver unordered messages and then the view.
       *)
      for i = 0 to pred ls.nmembers do
	Queuee.clean (fun (seqno,abv,iov) ->
	  if seqno >= s.ordered.(i) then (
(*	    log (fun () -> sprintf "delivering unordered msg:seqno=%d, #ordered=%d" seqno s.ordered.(i)) ;*)
	    up (castPeerIov name i iov) abv
	  ) else (
	    Iovecl.free iov
	  )
	) s.up_unord.(i)
      done ;
      upnm ev

  | EExit -> 
      log (fun () -> "EExit") ;
      Iq.free s.casts ;
      Array.iter (fun q -> 
	Queuee.clean (fun (_,_,iovl) -> Iovecl.free iovl) q
      ) s.up_unord ;
      upnm ev

  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | ECast iov when not (getNoTotal ev) && getApplMsg ev -> 
      if s.blocking then (
      	eprintf "SEQUENCER:warning dropping ECast after EBlockOk\n" ;
	Iovecl.free iov
      ) else if ls.rank =| s.seq_rank then (
	(* If I'm the sequencer, send out immediately as an ordered message.
	 * Otherwise, send pt2pt to sequencer and stash a copy of it in
	 * case the sequencer fails and we need to resend it.
	 *)
	if ls.nmembers >| 1 then (
	  let iov = Iovecl.copy iov in
	  dn ev abv (Ordered(ls.rank))
	);
	up (castPeerIov name ls.rank iov) abv ;
	array_incr s.ordered ls.rank
      ) else (
	ignore (Iq.add s.casts iov abv) ;
	dn (sendPeerIov name s.seq_rank iov) abv ToSeq
      )

    (* Handle local delivery for sends.
     *)
  | ESend iovl ->
      let dest = getPeer ev in
      if dest = ls.rank then (
      	up (sendPeerIov name ls.rank iovl) abv
      ) else (
	dn ev abv NoHdr
      )
	
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr ev = match getType ev with
    (* Send any messages we have buffered as 'unordered.'  
     *)
  | EBlock ->
      if not s.blocking then (
	s.blocking <- true ;
	List.iter (fun (seqno,iov,abv) ->
	  Queuee.add (seqno,abv,Iovecl.copy iov) s.up_unord.(ls.rank) ;
	  dn (castIovAppl name (Iovecl.copy iov)) abv (Unordered seqno)
	) (Iq.list_of_iq s.casts);
	Iq.free s.casts;
      ) ;
      dnnm ev

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
