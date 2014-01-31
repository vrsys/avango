(**************************************************************)
(* DROP.ML : randomly drops and delays messages at the 
 * reciever . *)
(* Author: Mark Hayden, 1/96 *)
(* Similar to Delay layer by Ohad Rodeh *)
(**************************************************************)
open Trans
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "DROP"
(**************************************************************)

module Priq = Priq.Make ( Time.Ord )

(**************************************************************)

type header = Origin of Endpt.id

type 'abv item =
  | Up of Event.up * 'abv
  | Upnm of Event.up
  | Zero

type 'abv state = {
  alarm : Alarm.t ;
  buf : 'abv item Priq.t ;
  partition : bool ;
  drop_link_rate : float ;
  mutable failed : bool Arrayf.t ;
  mutable acct_delivered : int ;
  mutable acct_dropped : int
}

(**************************************************************)

let dump (ls,vs) s = ()

(**************************************************************)

let init _ (ls,vs) = { 
  buf = Priq.create Zero ;
  failed = ls.falses ;
  partition = Param.bool vs.params "drop_partition" ;
  drop_link_rate = Param.float vs.params "drop_link_rate" ;
  acct_dropped = 0 ;
  acct_delivered = 0 ;
  alarm = Alarm.get_hack ()
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in
  let connected = Partition.connected s.drop_link_rate in

  let distrib =
    let drop_rate = Param.float vs.params "drop_rate" in
    let drop_delay = Param.time vs.params "drop_delay" in
    let drop_delay = 2.0 *. (Time.to_float drop_delay) in
    fun () ->
      let p = Random.float 1.0 in
      if p < drop_rate then (
	s.acct_dropped <- succ s.acct_dropped ;
	None
      ) else (
	let delay = p *. drop_delay in
	let delay = Time.of_float delay in
	Some(delay)
      )
  in

  let up_hdlr ev abv hdr = match hdr with
  | Origin(origin) ->
      if s.partition && not (connected ls.endpt origin) then (
	free name ev
      ) else (
	match distrib () with
	| None ->
	    free name ev
	| Some delay ->
	    (* Deliver after a certain delay....
	     *)
	    if Time.ge Time.zero delay then (
	      up ev abv
	    ) else (
	      let time = Alarm.gettime s.alarm in
	      let deliver = Time.add time delay in
	      Priq.add s.buf deliver (Up(ev,abv)) ;
	      dnnm (timerAlarm name deliver)
	    )
      )
(*  | _ -> up ev abv *)

  and uplm_hdlr ev _ = failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | EFail ->
      s.failed <- getFailures ev ;
      upnm ev

  | ETimer ->
      let time = getTime ev in
      ignore (
	Priq.get s.buf (fun _ e ->
	  s.acct_delivered <- succ s.acct_delivered ;
	  match e with
	  | Up(ev,abv) ->
	      let origin = getPeer ev in
	      if origin >= 0 
              && Arrayf.get s.failed origin 
	      then (
		free name ev
	      ) else (
		up ev abv
	      )
	  | Upnm ev -> upnm ev
	  | _ -> failwith sanity
	) time
      ) ;
      upnm ev

  | EExit ->
      Priq.clear s.buf (fun _ -> function
	| Up(ev,_) -> free name ev
	| Upnm ev  -> free name ev
	| _ -> failwith sanity
      ) ;
      upnm ev

  | EGossipExt -> (
      let origin =
      	getExtender (function
	  | HealGos(_,(_,endpt),_,_,_) -> Some (Some endpt)
	  | SwitchGos(_,(_,endpt),_) -> Some (Some endpt)
	  | _ -> None
        ) None ev 
      in

      match origin with 
      |	None ->
	  upnm ev
      |	Some origin ->
       	  if s.partition && not (connected ls.endpt origin) then (
	    free name ev
	  ) else (
	    match distrib () with
	    | None ->
		free name ev
	    | Some delay ->
	      (* Deliver after a certain delay....
	       *)
	      if delay <= Time.zero then (
		upnm ev
	      ) else (
		let time = Alarm.gettime s.alarm in
		let deliver = Time.add time delay in
		Priq.add s.buf deliver (Upnm(ev)) ;
		dnnm (timerAlarm name deliver)
	      )
	  )
    )

  | EAccount ->
      log (fun () -> sprintf "delivered=%d dropped=%d" s.acct_delivered s.acct_dropped) ;
      upnm ev

  | EDump -> ( dump vf s ; upnm ev ) 
  | _ -> upnm ev

  and dn_hdlr ev abv =
    dn ev abv (Origin ls.endpt)

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr init hdlrs None NoOpt args vs

let _ = 
  Param.default "drop_rate" (Param.Float 0.01) ;
  Param.default "drop_delay" (Param.Time (Time.of_string "0.001")) ;
  Param.default "drop_partition" (Param.Bool false) ;
  Param.default "drop_link_rate" (Param.Float 0.001) ;
  Layer.install name l

(**************************************************************)
