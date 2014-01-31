(**************************************************************)
(* DBGBATCH.ML : endpoint level emulation *)
(* Author:  Zhen Xiao, 3/98 *)
(**************************************************************)
open Trans
open Layer
open View
open Event
open Util
open Printf
(**************************************************************)
let name = Trace.filel "DBGBATCH"
(**************************************************************)

module Priq = Priq.Make ( Time.Ord )

(**************************************************************)

type header = Origin of string

type 'abv item =
  | Up of Event.up * 'abv
  | Upnm of Event.up
  | Zero

type 'abv state = {
  alarm : Alarm.t ;
  buf : 'abv item Priq.t ;
  my_name : string ;
  fname : string ;
}

(**************************************************************)

let dump = ignore2

let init _ (ls,vs) = 
  let my_name = Param.string vs.params "dbgbatch_name" in
  let fname = Param.string vs.params "dbgbatch_fname" in
  if my_name = "" then (
    eprintf "DBGBATCH: use of DBGBATCH protocol layer requires setting\n" ;
    eprintf "  the dbgbatch_name parameter.  Exiting\n" ;
    exit 2 ;
  ) ;
  { my_name = my_name ;
    fname = fname ;
    buf = Priq.create Zero ;
    alarm = Alarm.get_hack ()
  }

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in

  let check origin =
    if Dtblbatch.cut origin then None
    else (
      let p = Random.float 1.0 in
      let drop_rate = Dtblbatch.drop_rate origin in
      if p < drop_rate then None
      else Some (Dtblbatch.delay origin)
    )
  in

  let up_hdlr ev abv hdr = match hdr with
  | Origin(origin) -> begin
      match check origin with
      |	None ->
	  free name ev ;
	  log (fun () -> sprintf "dropping msg from %s\n" origin)
      |	Some delay ->
	  let delay = Time.of_float delay in
	  if delay <= Time.zero then up ev abv
	  else (
	    let time = Alarm.gettime s.alarm in
	    let deliver = Time.add time delay in
	    Priq.add s.buf deliver (Up(ev,abv)) ;
	    dnnm (timerAlarm name deliver)
	  )
  end

  and uplm_hdlr ev _ = failwith unknown_local
    
  and upnm_hdlr ev = match getType ev with
  | EInit ->
      Dtblbatch.init s.my_name s.fname ;
      upnm ev
	
  | ETimer ->
      let time = getTime ev in
      ignore (
	Priq.get s.buf (fun _ e ->
	  match e with
	  | Up(ev,abv) -> up ev abv
	  | Upnm ev -> upnm ev
	  | _ -> failwith "priq sanity"
	) time ;
      ) ;
      upnm ev

  | EExit ->
      Priq.clear s.buf (fun _ it ->
	match it with
	| Up(ev,_) -> free name ev
	| Upnm ev -> free name ev
	| _ -> failwith "priq sanity"
      ) ;
      upnm ev

  | EGossipExt -> begin
      let origin = getDbgName ev in
      match check origin with
      |	None -> 
	  free name ev ;
	  log (fun () -> sprintf "dropping Gossip msg from %s\n" origin)
      |	Some delay ->
	  let delay = Time.of_float delay in
	  if delay <= Time.zero then upnm ev
	  else (
	    let time = Alarm.gettime s.alarm in
	    let deliver = Time.add time delay in
	    Priq.add s.buf deliver (Upnm ev) ;
	    dnnm (timerAlarm name deliver)
	  )
  end

  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv (Origin s.my_name)

  and dnnm_hdlr ev = match getType ev with
  | EGossipExt ->
    let ev = set name ev[(DbgName s.my_name)] in
    dnnm ev

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = 
  Param.default "dbgbatch_name" (Param.String "") ;
  Param.default "dbgbatch_fname" (Param.String "data") ;
  Layer.install name l

(**************************************************************)


