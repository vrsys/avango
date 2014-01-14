(**************************************************************)
(* LOCAL.ML *)
(* Author: Mark Hayden, 6/95 *)
(**************************************************************)
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "LOCAL"
let failwith = Trace.make_failwith name
(**************************************************************)

let init _ (ls,vs) = ()

let hdlrs () (ls,vs) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let up_hdlr ev abv () = up ev abv
  and uplm_hdlr ev hdr = failwith "uplm_hdlr"
  and upnm_hdlr = upnm

  and dn_hdlr ev abv = match getType ev with
  | ECast iovl ->
      dn ev abv () ;
      up (castPeerIov name ls.rank (Iovecl.copy iovl)) abv

  | ESend iovl ->
      let rank = getPeer ev in
      if ls.rank =| rank then (
      	up (sendPeerIov name ls.rank iovl) abv ;
      ) else (
	dn ev abv ()
      )

  | _ -> dn ev abv ()

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None (FullNoHdr ()) args vf

let _ = Layer.install name l

(**************************************************************)
