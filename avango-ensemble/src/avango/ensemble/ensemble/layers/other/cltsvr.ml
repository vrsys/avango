(**************************************************************)
(* CLTSVR.ML *)
(* Author: Mark Hayden, 6/95 *)
(* Based on code by Robbert vanRenesse *)
(* Supports subcasts to just the servers or just the clients. *)
(**************************************************************)
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "CLTSVR"
let failwith s = Trace.make_failwith name s
(**************************************************************)

type header = NoHdr 
  | ClientOnly 
  | ServerOnly

type state = {
  client : bool
}
  
let init _ (ls,vs) = {
  client = Arrayf.get vs.clients ls.rank
}

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
(*let ack = make_acker name dnnm in*)

  let up_hdlr ev abv hdr = match getType ev, hdr with

    (* Filter out messages not to me.
     *)
  | ECast _, ClientOnly when     s.client -> up ev abv
  | ECast _, ServerOnly when not s.client -> up ev abv
  | ECast _, NoHdr                        -> up ev abv
  | ECast iovl, _ -> Iovecl.free iovl
  | _, NoHdr -> up ev abv
  | _ -> failwith bad_up_event

  and uplm_hdlr ev hdr = failwith unknown_local
  and upnm_hdlr ev = match getType ev with
  | EView ->
      (* First, do a sanity check.
       *)
      let vs = getViewState ev in
      let clients = vs.clients in
      let client = Arrayf.get clients ls.rank in
      if client <> s.client then
        failwith "sanity:client info inconsistent" ;

      upnm ev
  | _ -> upnm ev
  
  and dn_hdlr ev abv = match getType ev with

    (* Check bitfields to see if this is a client
     * or server subcast.
     *)
  | ECast _ ->
      let clt_only = getClientOnly ev
      and svr_only = getServerOnly ev in
      let hdr = match clt_only, svr_only with
      | true,  true  -> failwith "invalid options"
      | true,  false -> ClientOnly
      | false, true  -> ServerOnly
      | _,_	     -> NoHdr
      in
      dn ev abv hdr

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr ev = match getType ev with
  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l
