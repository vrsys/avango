(**************************************************************)
(* PT2PT.ML : FIFO, reliable pt2pt sends *)
(* Author: Mark Hayden, 3/96 *)
(**************************************************************)
open Layer
open View
open Event
open Util
open Trans
open Buf
(**************************************************************)
let name = Trace.filel "PT2PT"
(**************************************************************)
(* Data(seqno): sent with message number 'seqno'

 * Ack(seqno): acks first seqno pt2pt messages.

 * Nak(lo,hi): request for retransmission of messages number
 * 'lo' to 'hi'.
 * BUG: comment should say, inclusive/exclusive
 *)
type header = NoHdr
  | Data of seqno
  | Ack of seqno
  | DataAck of (*data:*)seqno * (*ack:*)seqno
  | Nak of seqno * seqno

let string_of_header = function
  | NoHdr -> "NoHdr"
  | Data seqno -> sprintf "Data(%d)" seqno
  | Ack seqno -> sprintf "Ack(%d)" seqno
  | DataAck(seqno,ack) -> sprintf "DataAck(%d,%d)" seqno ack
  | Nak(lo,hi) -> sprintf "Nak(%d,%d)" lo hi

(**************************************************************)
(* These are for optimizations in the Layer module.
 * They take advantage of the similar representation
 * between headers and options.
 *)

let _ =
  if Some 24 <> Obj.magic (Data 24) then
    failwith "PT2PT:tag sanity"

let extractor = function 
  | (Data _) as d -> (Obj.magic d : seqno option)
  | _ -> None

let constructor = function
  | (Some _) as s -> (Obj.magic s : header)
  | None -> failwith sanity

(**************************************************************)

type 'abv state = {
  sweep         : Time.t ;
  mutable next_sweep : Time.t ;
  mutable failed : bool Arrayf.t ;

  sends		: 'abv Iq.t Arrayf.t ;
  recvs		: 'abv Iq.t Arrayf.t ;
  naked         : seqno array ;
  acked		: seqno array ;
  retrans       : seqno array
}

(**************************************************************)

let iq_size iq =
  let l = Iq.list_of_iq iq in
  List.fold_left (fun s (_,iov,_) -> s + (int_of_len (Iovecl.len iov))) 0 l

(**************************************************************)

let dump vf s = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "failed =%s\n" (Arrayf.bool_to_string s.failed) ;
  sprintf "send_lo=%s\n" (Arrayf.int_to_string (Arrayf.map Iq.lo s.sends)) ;
  sprintf "send_hi=%s\n" (Arrayf.int_to_string (Arrayf.map Iq.hi s.sends)) ;
  sprintf "recv_lo=%s\n" (Arrayf.int_to_string (Arrayf.map Iq.lo s.recvs)) ;
  sprintf "recv_hi=%s\n" (Arrayf.int_to_string (Arrayf.map Iq.hi s.recvs)) ;
  sprintf "recv_size=%s\n" (Arrayf.int_to_string (Arrayf.map iq_size s.recvs)) ;
  sprintf "send_size=%s\n" (Arrayf.int_to_string (Arrayf.map iq_size s.sends)) ;
  sprintf "retrans=%s\n" (string_of_int_array s.retrans)
|]) vf s

(**************************************************************)

let init _ (ls,vs) = {
  sweep		= Param.time vs.params "pt2pt_sweep" ;
  next_sweep	= Time.zero ;
  sends		= Arrayf.init ls.nmembers (fun _ -> Iq.create name Local_nohdr) ;
  recvs		= Arrayf.init ls.nmembers (fun _ -> Iq.create name Local_nohdr) ;
  acked		= Array.create ls.nmembers 0 ;
  naked		= Array.create ls.nmembers 0 ;
  retrans       = Array.create ls.nmembers 0 ;
  failed	= ls.falses
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in
  let logb = Trace.log3 Layer.buffer ls.name name in

  (* Code for handling acks and data.  Note that handle_ack
   * does not modify the event, but handle_data does.
   *)
  let handle_ack ev ack =
    let origin = getPeer ev in
    let sends = Arrayf.get s.sends origin in
    Iq.set_lo sends ack ;
    log (fun () -> sprintf "got ack from %d for %d (new lo=%d)" origin ack (Iq.lo sends)) ;
  and handle_data iov ev seqno abv =
    let origin = getPeer ev in
    let recvs = Arrayf.get s.recvs origin in

    (* Check for fast-path.
     *)
    if Iq.opt_update_check recvs seqno then (
      (* The assignment below is unnecessary, but is
       * added for the functional optimizations.
       *)
      (*Arraye.set s.recvs origin*) 
      ignore (Iq.opt_update_update recvs seqno) ;
      up ev abv
    ) else (
      log (fun () -> sprintf "slow path origin=%d seqno=%d hi=%d" origin seqno (Iq.hi recvs)) ;
      if Iq.assign recvs seqno iov abv then (
	Iq.get_prefix recvs (fun seqno iov abv ->
	  let iov = Iovecl.copy iov in
	  up (sendPeerIov name origin iov) abv
	)
      ) else (
	log (fun () -> sprintf "slow path, redundant trans origin=%d seqno=%d lo=%d hi=%d" 
	      origin seqno (Iq.lo recvs) (Iq.hi recvs))
      ) ;

      (* If seqno seems old, then maybe a previous ack did
       * not get through.  Shift ack seqno back.  This will
       * cause an ack to be sent on next timeout.
       *)
      if seqno <| s.acked.(origin) then (
	log (fun () -> sprintf "setting acked=%d back to %d"
	    s.acked.(origin) seqno) ;
	s.acked.(origin) <- seqno
      ) ;

      (* If there are holes out of order
       * then send a nak.
       *)
      if seqno >| s.naked.(origin) then (
	Iq.set_hi recvs seqno ; (*BUG?*)
	if_some (Iq.hole recvs) (fun (lo,hi) ->
	  if seqno >=| lo then (
	    (* Keep track of highest msg # we've naked.
	     *)
	    s.naked.(origin) <- max s.naked.(origin) hi ;
	    dnlm (sendPeer name origin) (Nak(lo,hi)) ;
	  )
	)
      ) ;

      Iovecl.free iov
    )
  in

  let up_hdlr ev abv hdr = match getType ev, hdr with

    (* ESend:Data: Got a data message from other
     * member.
     *)
  | ESend iovl , Data(seqno) ->
      handle_data iovl ev seqno abv

    (* ESend:DataAck: Got a data message from other
     * member as well as an acknowledgement.
     *)
  | ESend iovl, DataAck(seqno,ack) ->
      handle_ack ev ack ;
      handle_data iovl ev seqno abv

  | _, NoHdr -> up ev abv
  | _        -> failwith bad_header

  and uplm_hdlr ev hdr = match getType ev,hdr with

    (* Nak: got a request for retransmission.  Send any
     * messages I have in the requested interval, lo..hi.
     *)
  | ESend iov, Nak(lo,hi) ->
      let origin = getPeer ev in
      let sends = Arrayf.get s.sends origin in
      log (fun () -> sprintf "got NAK for %d..%d from %d" lo hi origin) ;

      (* Ack: Unbuffer any acked messages.
       *)
      Iq.set_lo sends lo ;

      (* Retransmit any of the messages asked for that I have.
       *)
      let list = Iq.list_of_iq_interval sends (lo,hi) in
      List.iter (fun (seqno,iov,abv) ->
	let iov = Iovecl.copy iov in
        dn (sendPeerIov name origin iov) abv (Data seqno)
      ) list ;

      Iovecl.free iov

    (* Ack: Unbuffer any acked messages.
     *)
  | ESend iovl, Ack(seqno) ->
      handle_ack ev seqno ;
      Iovecl.free iovl

  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with

    (* EFail: Mark failed members, and pass on up.
     *)
  | EFail ->
      s.failed <- getFailures ev ;
      upnm ev

    (* ETimer: check for any messages that need to be
     * retransmitted.  Acknowledge any unacknowledged
     * messages.  
     *)
  | ETimer ->
      let time = getTime ev in
      if Time.ge time s.next_sweep then (
        s.next_sweep <- Time.add time s.sweep ;
	dnnm (timerAlarm name s.next_sweep) ;

	(* Go through all live members other than myself.
	 *)
      	for i = 0 to pred ls.nmembers do
	  if i <>| ls.rank && not (Arrayf.get s.failed i) then (
	    (* Send out acknowledgements.
	     *)
	    let recvs = Arrayf.get s.recvs i in
	    let sends = Arrayf.get s.sends i in
	    let head = Iq.lo recvs in
	    if head >| s.acked.(i) then (
	      log (fun () -> sprintf "sending ack to %d for %d, acked was %d" i head s.acked.(i)) ;
	      dnlm (sendPeer name i) (Ack head) ;
	      s.acked.(i) <- head
	    ) ;

	    (* Retransmit any unacknowledged messages I've sent up to the previous
	     * rounds.  This gives the other endpoints a chance to acknowledge
	     * my messages.
	     *)
	    let hi = s.retrans.(i) in
	    s.retrans.(i) <- Iq.hi sends ;
	    let lo = Iq.lo sends in
	    let list = Iq.list_of_iq_interval sends (lo,hi) in
	    List.iter (fun (seqno,iov,abv) ->
	      log (fun () -> sprintf "retransmitting %d to %d" seqno i) ;
	      let iov = Iovecl.copy  iov in
	      dn (sendPeerIov name i iov) abv (Data seqno)
	    ) list
	  )
	done
      ) ;

      upnm ev

  | EExit ->
      (* GC all buffers.
       *)
      Arrayf.iter Iq.free s.sends ;
      Arrayf.iter Iq.free s.recvs ;
      upnm ev

  | EAccount ->
      (* Dump buffering information if requested.
       *)
      logb (fun () -> sprintf "recv(bytes)=%s" (Arrayf.int_to_string (Arrayf.map iq_size s.recvs))) ;
      logb (fun () -> sprintf "send(bytes)=%s" (Arrayf.int_to_string (Arrayf.map iq_size s.sends))) ;
      upnm ev

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with

    (* ESend: buffer a copy and send it out.
     *)
  | ESend iov ->
      let dest = getPeer ev in
      if dest =| ls.rank then (
	eprintf "PT2PT:%s\n" (Event.to_string ev) ;
	eprintf "PT2PT:%s\n" (View.string_of_full (ls,vs)) ;
      	failwith "send to myself" ;
      ) ;
      let sends = Arrayf.get s.sends dest in
      let seqno = Iq.hi sends in

      (* The assignment below is unnecessary, but is
       * added for the functional optimizations.
       *)
      (*Arraye.set s.sends origin*) let _ = (Iq.add sends iov abv) in

      (* Non-application messages piggyback an acknowledgement.  The key
       * here is for the Pt2ptw ack messages to cause an ack to be piggy
       * backed by this layer.
       *)
      let hdr = 
	if getApplMsg ev then (
	  Data seqno
	) else (
	  let recvs = Arrayf.get s.recvs dest in
	  let head = Iq.lo recvs in
	  if head <=| s.acked.(dest) then (
	    Data seqno
	  ) else (
	    log (fun () -> sprintf "sending ack to %d for %d, acked was %d" dest head s.acked.(dest)) ;
	    s.acked.(dest) <- head ;
	    DataAck(seqno,head)
	  )
	)
      in
      dn ev abv hdr

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr init hdlrs None (LocalSeqno(NoHdr,C_ESend,extractor,constructor)) args vs
let l2 args vs = Layer.hdr_noopt init hdlrs args vs

let _ = 
  Param.default "pt2pt_sweep" (Param.Time (Time.of_int 1)) ;
  Layer.install name l

(**************************************************************)
