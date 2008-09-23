(**************************************************************)
(* DBM.ML : Database test programs *)
(* Author: Mark Hayden, Zhen Xiao 8/97 *)
(**************************************************************)
open Trans
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "DBM"
(**************************************************************)

module Priq = Time.Priq

(**************************************************************)

type header = Origin of int

type 'abv item =
  | Up of Event.up * 'abv
  | Upnm of Event.up

type 'abv state = {
    mutable num : int ;     (* Database server number *)
    sweep : Time.t ;
    mutable next_sweep : Time.t
}

(**************************************************************)

let dump = unit2

(**************************************************************)

let init _ (ls,vs) = { 
  num = 0 ;
  sweep = Param.time vs.params "dbm_sweep" ;
  next_sweep = Time.invalid
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log name ls.name in

  let up_hdlr ev abv hdr = match hdr with
  | Origin(origin) ->
    if Dbp.connected s.num origin then 
      up ev abv
    else ( 
      free name ev ;
      log (fun () -> sprintf "Drop messages.\n")
    )

  and uplm_hdlr ev _ = failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | EInit ->
      let c = open_in "ser_num" in
      let ser = input_line c in
      printf "Database server: %s\n" ser ;
      s.num <- int_of_string ser ;
      close_in c ;
      Dbp.init () ;
      dnnm (timerAlarm name Time.zero) ;
      upnm ev

  | ETimer ->
      let time = getTime ev in
      if time >= s.next_sweep then (
        s.next_sweep <- Time.add time s.sweep ;
        dnnm (timerAlarm name s.next_sweep) ; (* request next sweep *)
	Dbp.update () ;
      ) ;
      upnm ev
(*
  | EGossipExt -> begin
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
       	if Dbp.ept_con ls.endpt origin then
	  upnm ev
	else (
	  free name ev ;
	  log (fun () -> sprintf "Drop Gossip Messages.\n")
	)
    end
*)
  | EDump -> ( dump vf s ; upnm ev ) 
  | _ -> upnm ev

  and dn_hdlr ev abv =
    dn ev abv (Origin s.num)

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr_state init hdlrs None NoOpt args vs

let _ = 
  Param.default "dbm_sweep" (Param.Time (Time.of_int 1)) ;
  Elink.layer_install name l

(**************************************************************)
