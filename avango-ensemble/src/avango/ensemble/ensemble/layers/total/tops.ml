(**************************************************************)
(*
 *  TransisE, (Version 1.00)
 *  Hebrew University
 *  Copyright Notice
 *
 *  The contents of this file are subject to copyright by
 *  the Hebrew University, which reserves all rights.  Use,
 *  distribution and copying of this material is expressly
 *  prohibited except by prior written permission from
 *  the Hebrew University or from its appropriately authorized 
 *  agents and licensors.
 *)
(**************************************************************)
(* TCAUSAL.ML : TOPS total ordering protocol *)
(* Author: Ohad Rodeh, 2/97 *)
(**************************************************************)
(* 
Total Order Per Se
------------------
This simple version enforces a strict regime on received fifo
messages. Messages are delivered in a predetermined order -
   msg from 0
   msg from 1
   msg from 2
and so on.

If a process does not send regular messages, then if it blocks
the ordering protocol, it sends a null message within a 1/2second
delay.

*)
(**************************************************************)
open Trans
open Layer
open Event
open Util
open View
(**************************************************************)
let name = Trace.filel "TOPS"
(**************************************************************)

type msg_type = bitfield

type header = NoHdr | YesTops | Filler

type 'abv msg =   
  | Full of 'abv * Iovecl.t
  | Null    

type 'abv state = {
    buf		: 'abv msg Queue.t array;(* The buffer of delayed messages *)
    mutable pointer   : int;               (* Current place in buf  *)
    mutable num_msgs  : int;               (* Number of messages in buf *)
    mutable blocked   : bool               (* Am I blocked? *)
  }

(**************************************************************)

let dump vf s = Layer.layer_dump_simp name vf s
  
(**************************************************************)

let init _ (ls,vs) = {
  buf      = Array.init ls.nmembers (fun _ -> Queue.create ()) ;
  pointer  = 0;
  num_msgs = 0;
  blocked  = false
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in

  (* UpCast a message locally. 
   *)
  let up_cast org msg = 
    match msg with 
    | Full(abv,dn_iov) -> 
	up (castPeerIov name org dn_iov) abv
    | Null -> ()
  in

  (* UpCast next message in line.
   *)
  let next_up () = 
    let msg = Queue.take s.buf.(s.pointer) in
    s.num_msgs <- pred s.num_msgs;
    up_cast s.pointer msg;
    s.pointer <- (succ s.pointer) mod ls.nmembers
  in

  let handle_msg msg org =
    Queue.add msg s.buf.(org);
    s.num_msgs <- succ s.num_msgs;
    try 
      while true do next_up () done;
    with Queue.Empty -> ()
  in

  let up_hdlr ev abv hdr = match getType ev, hdr with

  (* UpCast:Data: Got a data message.
   *)
  | ECast iovl, YesTops -> 
      handle_msg (Full(abv, iovl)) (getPeer ev);

  | _  -> up ev abv 

  and uplm_hdlr ev hdr = match getType ev,hdr with
  | ECast iovl, Filler -> 
      handle_msg Null (getPeer ev) ;
      Iovecl.free iovl

  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  |  EBlock -> 
      s.blocked <- true;
      upnm ev

  (* GC all buffers.
   *)
  | EExit ->
      Array.iter (fun q -> 
	Queue.iter (function
	  | Null -> ()
	  | Full(abv, iov) -> Iovecl.free iov
	) q ;
	Queue.clear q;
      ) s.buf ;
      upnm ev

  (* Send a Filler message 
   *)
  | ETimer ->
      if s.num_msgs > 0 
      	&& not s.blocked
      	&& s.pointer = ls.rank then
	begin
	  log (fun () -> "Filler");
	  dnlm (castEv name) Filler;
	  handle_msg Null ls.rank;
   	end;
      upnm ev

  (* Flush up all messages that are still in the buffer
   *)
  | EView -> 
      while s.num_msgs > 0 do
      	try 
      	  while true do next_up () done;
	with Queue.Empty -> 
	  s.pointer <- (succ s.pointer) mod ls.nmembers
      done;
      upnm ev

  | EDump -> ( dump vf s ; upnm ev ) 
  | _ -> upnm ev 

  and dn_hdlr ev abv = match getType ev with

    (* Increment the reference count: this message is being sent 
     * both locally, and to the network. 
     *)
  | ECast iovl when not (getNoTotal ev) -> 
      log (fun () -> "multicasting") ;
      if ls.nmembers > 1 then (
	dn ev abv YesTops;
	handle_msg (Full(abv,(Iovecl.copy iovl))) ls.rank;
      ) else
	up (Event.set name ev [Peer ls.rank]) abv

    (* Handle local delivery for sends.
     *)
  | ESend iovl ->
      let dest = getPeer ev in
      if dest = ls.rank then (
      	up (sendPeerIov name ls.rank iovl) abv
      ) else (
	dn ev abv NoHdr
      )
	
  |  _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr init hdlrs None NoOpt args vs

let _ = Layer.install name l

(**************************************************************)
