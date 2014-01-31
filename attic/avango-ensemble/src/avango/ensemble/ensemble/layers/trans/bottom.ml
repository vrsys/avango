(**************************************************************)
(* BOTTOM.ML : bottom protocol layer *)
(* Author: Mark Hayden, 4/95 *)
(* Based on code by: Robbert vanRenesse *)
(* With structural changes sugested by Scott Stoller *)
(**************************************************************)
open Layer
open Trans
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "BOTTOM"
(**************************************************************)

type header = 
  | NoHdr
  | Unrel

let string_of_header = function
  | NoHdr -> "NoHdr"
  | Unrel -> "Unrel"

(**************************************************************)

type state = {
  mutable all_alive : bool ;
  alarm		  : Alarm.t ;
  mutable failed  : bool Arrayf.t ;
  mutable enabled : bool
}

(**************************************************************)

let init _ (ls,vs) = {
  alarm	  = Alarm.get_hack () ;
  failed  = ls.falses ;
  all_alive = true ;
  enabled = true
}

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "rank=%d nmembers=%d\n" ls.rank ls.nmembers ;
  sprintf "enabled=%b\n" s.enabled ;
  sprintf "all_alive=%b\n" s.all_alive ;
  sprintf "failed=%s\n" (Arrayf.bool_to_string s.failed)
|])

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in

  let got_merge ev typ vs abv =
    if true || s.enabled then (
      let time = Alarm.gettime s.alarm in
      up (set name ev[Type typ;Time time;ViewState vs]) abv
    ) else (
      free name ev
    )
  in

  let up_hdlr ev abv hdr = match getType ev, hdr with
  | (ECast iov |ESend iov), NoHdr ->
      if s.all_alive 
      || not (Arrayf.get s.failed (getPeer ev)) 
      then
	(* Common case: origin was alive.
	 *)
	up ev abv
      else
	Iovecl.free iov

  | ECast iov,Unrel ->
      if Arrayf.get s.failed (getPeer ev) then
	Iovecl.free iov
      else
	up (set name ev[Type (ECastUnrel iov)]) abv

  | ESend iov,Unrel ->
      if Arrayf.get s.failed (getPeer ev) then
	Iovecl.free iov
      else
	up (set name ev[Type (ESendUnrel iov)]) abv

  | EAuth,_ -> 
      if s.enabled then up ev abv else free name ev 
  | _ -> eprintf "%s\n" (Event.to_string ev) ; failwith bad_up_event

  and uplm_hdlr _ _ = failwith "got uplm event"
  and upnm_hdlr ev = match getType ev with
  | EInit ->
      if s.enabled then (
      	upnm (set name ev [Time (Alarm.gettime s.alarm)])
      ) else (
	free name ev
      )
	
  | ETimer | EAsync | EGossipExt -> 
(*
      eprintf "BOTTOM:%s:%s\n" ls.name (Event.to_string ev) ;
*)
      if s.enabled then upnm ev else free name ev

  | _ -> failwith "bad upnm event"


  and dn_hdlr ev abv =
    if s.enabled then (
      match getType ev with
      | EAuth
      | ECast _
      | ESend _ ->
	  dn ev abv NoHdr
      | ECastUnrel _
      | ESendUnrel _ ->
	  dn ev abv Unrel
      | _ -> failwith "bad down event[1]"
    ) else (
      free name ev
    )
	
  and dnnm_hdlr ev = match getType ev with
  (* EGossipExtDir will be delivered at EGossipExt.
   *)
  | EGossipExt
  | EGossipExtDir ->
      if s.enabled then
      	dnnm ev
      else
	free name ev

  (* ETimer: request a timeout callback from the transport.
   *)
  | ETimer ->
      if s.enabled then (
      	let alarm = getAlarm ev in
      	if Time.is_zero alarm then (
	  upnm (create name ETimer[Time (Alarm.gettime s.alarm)])
      	) else (
      	  dnnm ev
	)
      ) else (
	free name ev
      )

  (* EFail: mark the members as failed and bounce the event.
   *)
  | EFail ->
      s.failed <- getFailures ev ;
      s.all_alive <- Arrayf.for_all not s.failed ;
      upnm ev

  (* EExit: disable the transport and send up an EExit event.
   *)
  | EExit ->
      if s.enabled then (
	(* Mark myself and all members as disabled.
	 *)
	s.enabled <- false ;
	s.all_alive <- false ;
	s.failed <- Arrayf.create ls.nmembers true ;
	      
	(* Pass event down.
	 *)
	dnnm ev ;
	let time = Alarm.gettime s.alarm in
(*
      	eprintf "BOTTOM:%s:%s\n" ls.name (Event.to_string ev) ;
*)
	upnm (create name EExit[Time time]) ;
      ) else (
	log (fun () -> "2nd Exit (ok in some cases)") ;
	free name ev
      )

  (* All of these we bounce up as-is.
   *)
  | EBlock
  | ESuspect
  | EXferDone
  | ERekey
  | EPrompt
  | EProtocol
  | EMigrate
  | EView
  | EElect
  | EStable
  | ELeave
  | EBlockOk
  | EAccount 
  | ESyncInfo
  | EStableReq ->
      if s.enabled then upnm ev else free name ev

  | EPresent -> upnm ev

  | EDump -> dump vf s ; upnm ev

  | _ -> 
      eprintf "%s\n" (Event.to_string ev);
      failwith "bad down event[2]"

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr init hdlrs None (FullNoHdr NoHdr) args vs
let l2 args vs = Layer.hdr_noopt init hdlrs args vs

let _ = Layer.install name l

(**************************************************************)
