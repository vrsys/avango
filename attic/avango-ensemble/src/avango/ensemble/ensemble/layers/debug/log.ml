(**************************************************************)
(* LOG.ML *)
(* Author: Mark Hayden, 1/97 *)
(**************************************************************)
open Layer
open Event
open Util
open View
(**************************************************************)
let name = Trace.filel "LOG"
let failwith s = failwith (Util.failmsg name s)
(**************************************************************)
(*
type header = NoHdr
  | Info of seqno * len			(* sequence # & total bytes on channel *)

type out_channel = {
    mutable messages : int ;
    mutable bytes : int
  } 

type in_channel = {
    mutable messages : int ;
    mutable bytes : int ;
    mutable max_message : int ;
    mutable max_byte : int ;
    mutable out_of_order : int ;
  }     

type state = {
    send_out : out_channel array ;
    send_in : in_channel array ;
    cast_out : out_channel ;
    cast_in : in channel array
} 

let update_out ch iovl =
  let info = Info(ch.messages,ch.bytes) in
  let len = Iovecl.len name iovl in
  ch.bytes <- ch.bytes + len ;
  ch.messages <- ch.message + 1 ;
  info

let update_in ch hdr iov =
  match hdr with
  | Info(seqno,len) ->
      if seqno > ch.max_messages then (
	ch.out_of_order <- ch.out_of_order + seqno - ch.max_messages ;
	ch.max_messages <- seqno ;
	
      let len = Iovecl.len name iov in
      ch.messages <- ch.messages + 1 ;
      ch.bytes <- ch.bytes + len ;
      if seqno = 
  


let init _ (ls,vs) = ()

let hdlrs () vs {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let up_hdlr ev abv hdr = up ev abv
  and uplm_hdlr ev hdr = failwith "uplm_hdlr"
  and upnm_hdlr = upnm

  and dn_hdlr ev abv = match getType ev with
  | ECast ->
      
      up (create name ECast[Origin ls.rank; Iov (getIov ev)]) abv ;
      dn ev abv NoHdr

  | ESend ->
      if List.mem ls.rank (getPeers ev) then (
      	up (create name ESend[Origin ls.rank; Iov (getIov ev)]) abv ;
	if List.length (getPeers ev) = 1 then
  	  free name ev
	else
	  dn (set name ev[Ranks (except ls.rank (getPeers ev))]) abv NoHdr
      ) else (
	dn ev abv NoHdr
      )

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Elink.layer_install name l

(**************************************************************)
*)
