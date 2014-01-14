(**************************************************************)
(* SUBCAST.ML *)
(* Author: Mark Hayden, 4/95 *)
(* Sends pt2pt messages as casts *)
(**************************************************************)
open Layer
open View
open Event
open Trans
open Util
(**************************************************************)
let name = Trace.filel "SUBCAST"
let failwith = Trace.make_failwith name
(**************************************************************)

type header = NoHdr | Send of rank

let init = ignore2

let hdlrs () (ls,vs) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =

  let up_hdlr ev abv hdr = match getType ev, hdr with
  | ECast iovl, Send(dest) ->
      if ls.rank = dest then (
        up (set name ev [Type (ESend iovl)]) abv
      ) else (
      	Iovecl.free iovl
      )

  | _, NoHdr -> up ev abv
  | _, _     -> failwith bad_up_event

  and uplm_hdlr ev hdr = failwith unknown_local
  and upnm_hdlr = upnm

  and dn_hdlr ev abv = match getType ev with
  | ESend iovl ->
      let rank = getPeer ev in
      let ev = set name ev [Type (ECast iovl);Peer (-1)] in
      dn ev abv (Send(rank))

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
