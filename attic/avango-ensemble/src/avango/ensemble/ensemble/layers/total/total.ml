(**************************************************************)
(* TOTAL.ML *)
(* Author: Mark Hayden, 4/95 *)
(* Based on code by: Robbert vanRenesse *)
(* Bug fixes: Ohad Rodeh, 9/2002 *)
(**************************************************************)
open Layer
open Event
open Util
open View
open Trans
open Buf
(**************************************************************)
let name = Trace.filel "TOTAL"
let failwith = Trace.make_failwith name
(**************************************************************)
(* Headers

 * NoHdr: all non-cast messages

 * TokenRequest: request for token

 * TokenSend(token,next): Assigns next token holder to
 * member 'next'.  This header type never comes with
 * attached data.  The new token holder will send his first
 * message with sequence number 'token.'  All members can
 * forget any token requests by the sender of this message.

 * Ordered(token): Deliver this message with sequence number
 * 'token.'

 * OrderedTokenSendReq(token,next): Combination of previous
 * 3 headers.  The attached message is to be delivered with
 * sequence number 'token-1'.  The next token holder is
 * 'next' who will begin broadcasting with token number
 * 'token'.  In addition the sender requests to be sent
 * token again.

 * Unordered: (ie., not ordered by a token holder) Sent at
 * the end of the view.  All members deliver unordered
 * messages in fifo order in order of the sending members
 * rank.
 *)
type token = seqno
type next  = rank

type header = NoHdr
  | TokenRequest
  | TokenSend           of token * next
  | Ordered             of token
  | OrderedTokenSendReq of token * next
  | Unordered

(**************************************************************)

type 'abv state = {
  mutable requested	: bool ; 	(* have I requested the token? *)
  mutable blocking 	: bool ;	(* are we blocking? *)
  mutable token 	: token option ;(* the token, if I have it *)
  request : Request.t ;			(* token requests I've gotten *)
  waiting : ('abv * Iovecl.t) Queuee.t ;(* buffered xmit waiting for token *)
  order : (rank * 'abv ) Iq.t ;	(* buffered recv messages ordered by the token  *)

  (* buffered recv messages waiting for the end of the view *)
  unord : ('abv * Iovecl.t) Queuee.t array ;
}

(**************************************************************)

let dump vf s = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "blocking=%b, request=%s\n" s.blocking (Request.to_string s.request) ;
  sprintf "token=%s waiting_len=%d\n" (string_of_option string_of_int s.token) (Queuee.length s.waiting) ;
  sprintf "ordered=%d..%d\n" (Iq.lo s.order) (Iq.hi s.order) ;
|]) vf s

(**************************************************************)

let init _ (ls,vs) = {
  blocking 	= false ;
  token 	= if ls.rank = 0 then Some 0 else None ;
  request 	= Request.create ls.nmembers ;
  requested	= false ;
  waiting 	= Queuee.create () ;
  order 	= Iq.create name (0,NoMsg) ;
  unord 	= Array.init ls.nmembers (fun _ -> Queuee.create()) ;
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let dbg = sprintf "(%d,%s)" ls.rank (View.string_of_id ls.view_id) in
  let log = Trace.log2 name dbg in

  (* Put a message in my buffer and deliver any messages now
   * in order.
   *)
  let add_and_deliver token origin abv iovl =
    log (fun () -> sprintf "add_and_deliver token=%d (sum_refs=%s)" 
      token (Iovecl.sum_refs iovl));
    if not (Iq.assign s.order token iovl (origin,abv)) then 
      failwith "add_and_deliver assign failure";
    Iovecl.free iovl;
    log (fun () -> 
      let l = Iq.list_of_iq s.order in
      let l = List.map (function (seqno,_,(origin,_)) -> 
	sprintf "seq=%d,org=%d" seqno origin
      ) l in
      sprintf "lo=%d hi=%d %s" (Iq.lo s.order) (Iq.hi s.order) 
	(string_of_list ident l)
    );
    Iq.get_prefix s.order (fun seqno iov (origin,abv) -> 
      log (fun () -> sprintf "[B(org=%d)], (sum_refs=%s)" origin
	(Iovecl.sum_refs iov));
      let iov = Iovecl.copy iov in
      log (fun () -> sprintf "copied, (sum_refs=%s)" (Iovecl.sum_refs iov));
      up (castPeerIovAppl name origin iov) abv
    )
  in

  let got_token token =
    log (fun () -> sprintf "got_token %d" token);
    if s.token <> None then
      failwith (sprintf "%s token holder got another token" dbg) ;

    (* Check for any requests.  If there are requests and
     * we have no messages buffered, then just send the
     * token on.  If we have messages, then send them, with the
     * last message being sent as an OrderedTokenSendReq.
     *)
    let next = Request.take s.request in
    let len = Queuee.length s.waiting in
    if next >= 0 then (
      if len = 0 then (
	log (fun () -> sprintf "send TokenSend(%d,%d)" token next);
        dnlm (castEv name) (TokenSend(token,next)) ;
	s.token     <- None ;
	s.requested <- false
      ) else (
      	for i = 0 to len-2 do
	  let abv,iovl = Queuee.take s.waiting in
	  ignore (Iovecl.copy iovl);
          add_and_deliver (token+i) ls.rank abv iovl ;
	  dn (castIovAppl name iovl) abv
 	    (Ordered (token+i))
        done ;
        let abv,iovl = Queuee.take s.waiting in
	ignore (Iovecl.copy iovl);
        add_and_deliver (token+len-1) ls.rank abv iovl;
	dn (castIovAppl name iovl) abv
       	  (OrderedTokenSendReq(token+len,next)) ;
	s.requested <- true
      )
    ) else (
      (* No further requests, send messages and keep the
       * token.
       *)
      for i = 0 to pred len do
        let abv,iovl = Queuee.take s.waiting in
	(*log (fun () -> sprintf "self delivery [2], rank=%d" ls.rank);*)
	ignore (Iovecl.copy iovl);
        add_and_deliver (token+i) ls.rank abv iovl;
	dn (castIovAppl name iovl) abv
      	   (Ordered (token+i))
      done ;
      s.token <- Some(token+len)
    )
  in

  let up_hdlr ev abv hdr = match getType ev, hdr with
  | ECast iovl, Ordered token ->
      log (fun () -> sprintf "recv Ordered <- %d, (lo=%d, hi=%d) token=%d" 
	(getPeer ev) (Iq.lo s.order) (Iq.hi s.order) token);
      add_and_deliver token (getPeer ev) abv iovl ;
	
  | ECast iovl, OrderedTokenSendReq(token,next) ->
      let origin = getPeer ev in
      log (fun () -> sprintf "recv OrderedTokenSendReq <- %d (tok=%d,nxt=%d)" 
	origin token next);
      add_and_deliver (pred token) origin abv iovl ;

      if not s.blocking then (
	(* Remove and add the sender from the requests
	 *)
	Request.remove s.request origin ;
	Request.add    s.request origin ;
	
	(* Check if the token is for me.
	 *)
	if next = ls.rank then
	  got_token token ;
      )
	
  | ECast iovl, Unordered ->
      log (fun () -> "recv Unordered");
      let origin = getPeer ev in
      Queuee.add (abv,iovl) s.unord.(origin)
	
  | _, NoHdr -> up ev abv
  | _ -> failwith bad_up_event

  and uplm_hdlr ev hdr = match getType ev, hdr with
  | ECast iovl, TokenRequest ->
      assert(iovl = Iovecl.empty);
      log (fun () -> sprintf "recv TokenRequest <- %d" (getPeer ev));
      if not s.blocking then (
	match s.token with
	  | Some t ->
	      log (fun () -> sprintf "sending token to %d" (getPeer ev)) ;
	      dnlm (castEv name) (TokenSend(t,(getPeer ev))) ;
	      s.token     <- None ;
	      s.requested <- false
	  | None -> 
	      Request.add s.request (getPeer ev)
      )

    (* Could use sends to pass the token around... (with
     * buffering, is that more efficient?)
     *)
  | ECast iovl, TokenSend(token,next) ->
      assert(iovl = Iovecl.empty);
      log (fun () -> sprintf "recv TokenSend(tok=%d,nxt=%d) <- %d" token next (getPeer ev));
      if ls.rank  = next 
	&& not s.blocking then 
	  got_token token 

  | _ -> failwith "bad uplm message"

  and upnm_hdlr ev = match getType ev with
  | EView ->
      log (fun () -> "EView");
      (* Deliver pending ordered messages, inspite of possible holes. 
       *)
      let l = Iq.list_of_iq s.order in
      List.iter (fun (seqno,iovl,(origin,abv)) ->
	log (fun () -> sprintf "[C(org=%d, seq=%d)]" origin seqno) ; 
	up (castPeerIovAppl name origin (Iovecl.copy iovl)) abv
      ) l;
      Iq.free s.order;

      (* Deliver unordered messages.
       *)
      Array.iteri (fun i q ->
	Queuee.clean (fun (abv,iov) ->
	  up (castPeerIovAppl name i iov) abv
	) q
      ) s.unord ;
      upnm ev

  | EExit -> 
      Queuee.clean (fun (_,iovl) -> Iovecl.free iovl) s.waiting;
      Iq.free s.order;
      Array.iter (fun q -> 
	Queuee.clean (fun (_, iovl) -> Iovecl.free iovl) q
      ) s.unord;
      upnm ev
	
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | ECast iovl when not (getNoTotal ev) && getApplMsg ev -> 
      if s.blocking then (
      	eprintf "TOTAL:warning dropping ECast after EBlockOk\n" ;
	Iovecl.free iovl
      ) else if ls.nmembers =| 1 then (
	up (castPeerIovAppl name ls.rank iovl) abv
      ) else (
	match s.token with
	  | Some t ->
	      log (fun () -> sprintf "Cast from Leader, token=%d" t);
	      add_and_deliver t ls.rank abv (Iovecl.copy iovl) ;
	      dn ev abv (Ordered t) ;
	      s.token <- Some(succ t)
	  | None ->
	      log (fun () -> "Cast");
	      if not s.requested then (
		log (fun () -> sprintf "requesting token") ;
		s.requested <- true ;
		dnlm (castEv name) TokenRequest
	      ) ;
	      Queuee.add (abv,iovl) s.waiting
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
	log (fun () -> "blocking");
	Queuee.clean (fun (abv,iovl) ->
	  log (fun () -> "Cast unordered");
      	  dn (castPeerIovAppl name ls.rank (Iovecl.copy iovl)) abv Unordered;
	  Queuee.add (abv,iovl) s.unord.(ls.rank)
	) s.waiting
      ) ;
      log (fun () -> "done blocking funcall");
      dnnm ev

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = 
  Layer.install name l

(**************************************************************)
