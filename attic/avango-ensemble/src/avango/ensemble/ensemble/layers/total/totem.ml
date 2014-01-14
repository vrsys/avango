(**************************************************************)
(* TOTEM.ML                                                   *)
(* Author: Roy Friedman, 3/96                                 *)
(* Bug fixes & major changes: Mark Hayden, 10/96              *)
(* Bug fixes : Ohad Rodeh, 9/02                               *)
(**************************************************************)
(* Original C code was done by Roy Friedman as a hack on the
 * C Total Later.  This is a hack on Mark Hayden's ML Total
 * Layer.basically, there is a token constantly rotating
 * within the group.   Anyone that wishes to send, must buffer
 * its messages until it gets the token.  Once a member
 * receives the token, it sends everything it has to send,
 * and pass the tokan to the next member in the membership
 * list.  This protocol was originally developed as part of
 * the Totem project.  
 * 
 * 
 * MH: does not observe causality
 * 
 * BUG (RF): The token is an unbounded counter. To prevent it
 * from wrapping around, a view must be installed every so
 * often. If messages are sent at a rate of 1,000 per
 * second, then at least every 24 days.  If the rate is at
 * least 10,000 per second, then every 2.4 days etc.  
 *)
(**************************************************************)
open Trans
open Layer
open Event
open Util
open View
(**************************************************************)
let name = Trace.filel "TOTEM"
let failwith = Trace.make_failwith name
(**************************************************************)
(* Headers

 * NoHdr: all non-cast messages

 * TokenSend(token): Assigns next token holder to the next
 * member in the cycle (as determined by the origin of the
 * mesage).  This header type never comes with attached
 * data.  The new token holder will send his first message
 * with sequence number 'token.'

 * Ordered(seqno,has_token): Deliver this message with
 * sequence number 'seqno.'  If has_token is true then the
 * message also passes token to the next member in the group
 * (as determined by the origin).

 * Unordered: (ie., not ordered by a token holder) Sent at
 * the end of the view.  All members deliver unordered
 * messages in fifo order in order of the sending members
 * rank.

 *)

type header = NoHdr
  | TokenSend of seqno
  | Ordered   of seqno * bool
  | Unordered

(**************************************************************)

type 'abv state = {
  prev		: rank ;		(* prev member in cycle *)
  next		: rank ;		(* next member in cycle *)
  mutable got_view : bool ;
  mutable blocking : bool ;		(* are we blocking? *)
  waiting 	: ('abv * Iovecl.t) Queuee.t ;(* waiting for token *)
  order 	: (rank * 'abv) Iq.t ;	(* ordered by token *)
  unord 	: ('abv * Iovecl.t) Queuee.t array ; (* unordered *)
  mutable token	: seqno option
}

(**************************************************************)

let dump vf s = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "blocking=%b \n" s.blocking ;
  sprintf "waiting=%d\n" (Queuee.length s.waiting) ;
  sprintf "order=%d..%d\n" (Iq.lo s.order) (Iq.hi s.order) ;
  let l = Iq.list_of_iq s.order in
  let l = string_of_list (fun (s,_,(r,_)) -> sprintf "(%d,%d)" r s) l in
  sprintf "order=%s\n" l
|]) vf s

(**************************************************************)

let init await_ack (ls,vs) = {
  got_view	= false ;
  blocking 	= false ;
  next		= (succ ls.rank) mod ls.nmembers ;
  prev		= (pred ls.rank + ls.nmembers) mod ls.nmembers ;(* Hack! *)
  waiting 	= Queuee.create () ;
  order 	= Iq.create name (0,NoMsg) ;
  token		= None ;
  unord 	= Array.init ls.nmembers (fun _ -> Queuee.create ())
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith m = dump vf s ; failwith m in

  (* Check for any requests.  If there are requests and we
   * have no messages buffered, then just send the token on.
   * If we have messages, then send them, with the last message
   * being sent as an OrderedTokenSend.  
   *)
  let check_token () =
    if_some s.token (fun token ->
      if Iq.lo s.order >= token then (
	s.token <- None ;
	let len = Queuee.length s.waiting in
	if len = 0 then (
	  dnlm (sendPeer name s.next) (TokenSend(token))
	) else (
	  if !verbose then
      	    eprintf "TOTEM:%d->[%d..%d]\n" ls.rank token (token+len-1) ;
	  for i = 0 to pred len do
	    let abv,iov = Queuee.take s.waiting in
	    let pass = (i = pred len) in
	    dn (castIovAppl name (Iovecl.copy iov)) abv (Ordered(token + i,pass)) ;
	    up (castPeerIovAppl name ls.rank iov) abv ;
	    if not (Iq.opt_update_old s.order (token + i)) then
	      failwith sanity
	  done
	)
      )
    )
  in

  let got_token token =
    if not s.blocking then (
      s.token <- Some token ;
      check_token ()
    )
  in

  let up_hdlr ev abv hdr = match getType ev, hdr with
  | ECast iov, Ordered(seqno,token) ->
      if s.got_view then
      	failwith "ECast(Ordered) after EView" ;
      if !verbose then
	eprintf "TOTEM:%d:Ordered:seqno=%d\n" ls.rank seqno ;
      if Iq.opt_update_old s.order seqno then (
        up ev abv
      ) else (
	let origin = getPeer ev in
	if not (Iq.assign s.order seqno iov (origin,abv)) then
	  failwith "2nd seqno of same seqno" ;
	Iq.get_prefix s.order (fun seqno iov (origin,abv) ->
	  up (castPeerIovAppl name origin (Iovecl.copy iov)) abv
	) ;
        Iovecl.free iov
      ) ;

      check_token () ;

      (* Check if the token is for me.
       *)
      if token & (getPeer ev) = s.prev then
        got_token (succ seqno)

  | ECast iovl, Unordered ->
      if s.got_view then
      	failwith "ECast(Unordered) after EView" ;
      Queuee.add (abv,iovl) s.unord.((getPeer ev))

  | _, NoHdr      -> up ev abv (* all other events have NoHdr *)
  | _             -> failwith "non-NoHdr on non ECast"

  and uplm_hdlr ev hdr = match getType ev,hdr with
  | ESend iovl, TokenSend(token) ->
      got_token token ;
      Iovecl.free iovl

  | _ -> failwith "non-NoHdr on non ECast"

  and upnm_hdlr ev = match getType ev with
  | EInit ->
      upnm ev;
      if ls.rank = 0 && ls.nmembers > 1 then
	dnlm (sendPeer name s.next) (TokenSend(0))
  | EView ->
      s.got_view <- true ;

      (* Deliver unordered messages.
       *)
      for i = 0 to pred ls.nmembers do
        Queuee.clean (fun (abv,iov) ->
	  if !verbose then
	    eprintf "TOTEM:unordered:%d->%d\n" i ls.rank ;
          up (castPeerIovAppl name i iov) abv
        ) s.unord.(i)
      done ;
      upnm ev

  | EExit -> 
      Queuee.clean (function (_,iovl) -> Iovecl.free iovl ) s.waiting ;
      Iq.free s.order;
      Array.iter (function q -> 
	Queuee.clean (function (_,iovl) -> Iovecl.free iovl ) q
      ) s.unord ;
      upnm ev

  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | ECast iovl when not (getNoTotal ev) && getApplMsg ev -> 
      if s.blocking then (
      	eprintf "TOTEM:warning dropping ECast after EBlockOk\n" ;
	Iovecl.free iovl
      ) else if ls.nmembers =| 1 then (
	up (castPeerIovAppl name ls.rank iovl) abv
      ) else (
	Queuee.add (abv,iovl) s.waiting ;
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
  | EBlock ->
      if s.blocking then 
      	failwith "blocking twice" ;
      if s.got_view then 
	failwith "EBlock, but already got EView";
      s.blocking <- true ;
      if !verbose then
	eprintf "TOTEM:sending %d unordered\n" (Queuee.length s.waiting) ;
      Queuee.clean (fun (abv,iov) ->
      	Queuee.add (abv,iov) s.unord.(ls.rank) ;
	dn (castIovAppl name (Iovecl.copy iov)) abv Unordered
      ) s.waiting ;
      dnnm ev

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
