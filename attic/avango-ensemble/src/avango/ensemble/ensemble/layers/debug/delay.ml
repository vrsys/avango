(**************************************************************)
(*
 *  TransisE, (Version 0)
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
(* DELAY.ML : debugging layer. Delaying messages *)
(* Author: Ohad Rodeh, 12/96 *)
(**************************************************************)
open Trans
open Layer
open Event
open Util
open View
(**************************************************************)
let name = Trace.filel "DELAY"
(**************************************************************)

type header = NoHdr

type state = {
    counter           : int;
    mutable count     : int
}

(**************************************************************)

let dump = ignore2
let debug = false

(**************************************************************)

let init _ (ls,vs) = { 
  counter  = 12 ;
  count    = 0
}
  
(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in

  let up_hdlr ev abv hdr = match getType ev, hdr with
  | ECast _, NoHdr -> 
(*	s.count <- (s.count + 1) mod s.counter;
 	if 0<=s.count & s.count<=3 then up ev abv 
	else let r = (Random.int 10) in 
      	if debug then printf "[DELAY r=%d]" r;
	if 0<=r & r <=3 then up ev abv 
	else free name ev *)
      up ev abv 
  | _, NoHdr -> up ev abv 

  and uplm_hdlr ev hdr = match getType ev,hdr with
  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | EDump -> ( dump vf s ; upnm ev ) 
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr init hdlrs None NoOpt args vs

let _ = Layer.install name l

(**************************************************************)


