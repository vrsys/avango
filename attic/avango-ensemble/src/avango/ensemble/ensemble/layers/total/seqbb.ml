(****************************************************************)
(* SEQBB.ML                                                     *)
(* Author: Lars Hofhansl 5/97                                   *)
(* Bug fixes: Ohad Rodeh, 7/02				        *)
(****************************************************************)
(*
 * An sequencer based broadcast-broadcast total ordering protocol.
 * A sender who wishes to initiate a totally ordered broadcast,
 * sends it immediatly. All receivers buffer the message until
 * they receive a special accept message from the sequencer.
 * The sequencer itself listens for broadcast messages and sends
 * the accept message as soon as it receives a broadcast.
 *
 * Messages are identified by the senders rank and by their position 
 * in the stream of input messages (MNAK provides FIFO).
 * 
 * When a member receives an accept, it delivers the
 * corresponsing message. If the message is not yet 
 * available the accept is buffered.
 * IMPORTANT : This protocol requires a reliable channel that already
 * provides FIFO ordering (per member), I.e. MNAK.
 *
 * The BB protocol has advantages on multicast networks, with
 * with large messages.
 * 

 * Warning, this is my first Program in ocaml (and ensemble),
 * so be patient with me...
 *)
(**************************************************************)
open Layer
open Event
open Util
open View
open Trans
open Buf
(**************************************************************)
let name = Trace.filel "SEQBB"
let failwith s = Trace.make_failwith name s
let log = Trace.log name 
(**************************************************************)

type header = NoHdr
  | Cast             (* an ordered BCast msg *)
  | Acc of rank      (* accept from the sequencer *)

type 'abv state = {
    (* the rank of the sequencer *)
    seq_rank : int;   

    (* array of queues containing all not yet accepted msg for
       each member *)
    to_deliver : ('abv * Event.t) Queue.t array;

    (* list of all accept msgs *)
    accepts : int Queue.t;

    (* is the view in a blocking state ? *)
    mutable blocking : bool
}

(* vs is a viewstate record, s is a state record *)

let dump (ls,vs) s = ()

let init _ (ls,vs) = {
  seq_rank = 0;      (* sequencer is always 0 *)
  blocking = false;
  to_deliver = Array.init ls.nmembers (fun _ -> Queue.create());
  accepts = Queue.create ()
}


let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =

  (* scan through the list of accepts and try to deliver the corresponding
     msgs. If successful try the next one *)

  let rec deliver () =
    try 
      let rank = Queue.peek s.accepts in

      let (abv,ev) = Queue.take s.to_deliver.(rank) in

      ignore (Queue.take s.accepts) ; (* if reached : remove head *)

      up ev abv;             (* deliver the msg            *)
      deliver ();            (* .. and try the next accept *)
	
    with Queue.Empty -> ()   (* either accepts of msg-queue empty...
				do nothing ... *)
  in

  (* rcv evt from below *) 
  let up_hdlr ev abv hdr =

    (* if we receive a normal cast message: 
       - the sequencer casts the accept message,
         and delivers the original message
       - all others store the message, until the
         accept message arrives *)
    match getType(ev),hdr with
    | ECast iovl, Cast ->
	let rank = getPeer ev in 

      	if ls.rank = s.seq_rank then (
	  (*printf "sequencer: got Cast send accept for %d from %d\n"
	    seqno rank;*)

	  (* cast accept message *)
	  dnlm (Event.create name (ECast Iovecl.empty) [Peer ls.rank]) (Acc rank);
	  (* deliver *)
	  up ev abv ;

	) else (
	  (*printf "storing cast %d from %d\n" seqno rank;*)
	  Queue.add (abv,ev) s.to_deliver.(rank);
	  deliver ()
	)

    (* This is an already ordered message from the leader.
     *)
  | ECast iovl ,Acc rank -> 
      assert (rank = s.seq_rank && ls.rank <> rank) ;
      (* deliver the message from rank *)
      (*printf "got accept %d from %d... queueing\n" seqno rank;*)
      Queue.add (abv,ev) s.to_deliver.(rank);
      Queue.add rank s.accepts;
      deliver ();


    | _ -> up ev abv

  (* rcv ev from below, handle msg *)
  and uplm_hdlr ev hdr = match getType ev, hdr with
  | ECast iovl ,Acc(rank) ->
      (* deliver the message from rank *)
      (*printf "got accept %d from %d... queueing\n" seqno rank;*)
      assert (Iovecl.len iovl =|| len0) ;
      Queue.add rank s.accepts;
      deliver ();

  | _ -> failwith "uplm_hdlr... strange"

  (* rcv ev from below *)
  and upnm_hdlr ev = match getType ev with
    (* if I get get a new view I deliver all msg that wait for
       delivery in some deterministic order - here ordered by
       rank -- oops, also take seqno into account !! *)
  | EView ->
      for i = 0 to pred ls.nmembers do
	for j = 1 to Queue.length s.to_deliver.(i) do 
	  let (abv,ev) = Queue.take s.to_deliver.(i) in
	  up ev abv
	done;
	Queue.clear s.to_deliver.(i)
      done;
      Queue.clear s.accepts;
      upnm ev

  (* GC all buffers.
   *)
  | EExit ->
      log (fun () -> sprintf "%s %d EExit(" (View.string_of_id ls.view_id) ls.rank);
      Array.iter (fun q -> 
	Queue.iter (function (abv,ev) -> free name ev) q ;
	Queue.clear q;
      ) s.to_deliver;
      log (fun () -> ")");
      upnm ev

  | _ -> upnm ev

  (* rcv ev from above, forward msg *)  
  and dn_hdlr ev abv = 
    match getType ev with
    | ECast iovl when not (getNoTotal ev) -> 
        (* we got a broadcast request from the application here *)
	if s.blocking then (
	  eprintf "the view is blocked... dropping\n" ;
	  Iovecl.free iovl
	) else (
	  if ls.nmembers = 1 then 
	    up (Event.set name ev [Peer ls.rank]) abv
	  else (
	    (* all members can cast the message immediately ! *)
	    
	    (* Increment the reference count: this message is being sent 
	     * both locally, and to the network. *)
	    ignore (Iovecl.copy iovl);
	    let ev = Event.set name ev [Peer ls.rank] in

	    if( ls.rank = s.seq_rank ) then (
	      
	      (* if I'm the sequencer send the message with the accept
                 - each message is uniqly indentified by
                 the senders rank and the sequence number *)
	      (*printf "sequencer sending accept for %d\n" s.curr_seq_num;*)

	      log (fun () -> sprintf " %s %d: seq multicasting" (View.string_of_id ls.view_id) ls.rank);
	      dn ev abv (Acc ls.rank);
	      
	      (* the sequencer delivers the message immediately.
	       *)
	      up ev abv
	    ) else (
	      log (fun () -> sprintf "%s %d: multicasting" (View.string_of_id ls.view_id) ls.rank);
    	      dn ev abv Cast;
	      (* all others store their message and wait for
	         the accept *)
	      
	      (*printf "storing own cast %d me %d -- %d\n"
		s.curr_seq_num ls.rank (getOrigin ev);*)
	      Queue.add (abv,ev) s.to_deliver.(ls.rank);
	    )
          ) 
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

  (* rcv ev from above *)
  and dnnm_hdlr ev = match getType ev with
  | EBlock ->
      (* make sure we don't accept any broadcast while 
         the system is merging a new group *)
      s.blocking <- true;
      dnnm ev
  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}


let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l
