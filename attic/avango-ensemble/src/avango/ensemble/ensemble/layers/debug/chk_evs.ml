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
(* CHK_EVS.ML : Check Extended virtual synchrony *)
(* Author: Ohad Rodeh, 12/96 *)
(* Based on code by: Mark Hayden  - CHK_SYNC.ML*)
(**************************************************************)
open Layer
open Event
open Util
open View
(**************************************************************)
let name = Trace.filel "CHK_EVS"
let failwith s = failwith (Util.failmsg name s)
(**************************************************************)

type header = NoHdr 
| Gossip of View.id * seqno array * bool array

let init _ (ls,vs) = ()

let hdlrs () vs {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let casts = array_create name ls.nmembers 0 in
  let up_view = ref false in 
  let fail = array_create name ls.nmembers false in

  let chk_casts = ref None in
  let chk_rmt = ref [] in
  let chk_sync () =
    if_some !chk_casts (fun casts ->
      List.iter (fun (rmt_rank,rmt_casts,rmt_fail) ->
      	if !verbose then
      	  eprintf "CHK_EVS:verifying:%d->%d\n" ls.rank rmt_rank ;
	if rmt_casts <> casts then (
	  eprintf "CHK_EVS:virtual synchrony failure{%s}\n" ls.name ;
	  sprintf "rank: %d rmt_rank: %d\n" ls.rank rmt_rank ;
	  sprintf "view_id=%s\n" (View.string_of_id vs.view_id) ;
	  sprintf " my failed:%s\n" (string_of_bool_array fail) ;
	  sprintf "   my cast:%s\n" (string_of_int_array casts) ;
	  sprintf "  rmt cast:%s\n" (string_of_int_array rmt_casts) ;
	  sprintf "rmt failed:%s\n" (string_of_bool_array rmt_fail) ;
	  failwith "virtual synchrony failed"
	)
      ) !chk_rmt ;
      chk_rmt := []
    )
  in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | ECast,NoHdr ->
      array_incr casts (getPeer ev) ;
      up ev abv
  | _,NoHdr -> up ev abv
  | _,_ -> failwith bad_up_event

  and uplm_hdlr ev hdr = match getType ev,hdr with
  | ECast,Gossip(view_id,rmt_casts,rmt_fail) ->
      chk_rmt := ((getPeer ev),rmt_casts,rmt_fail) :: !chk_rmt ;
      chk_sync () ;
      free name ev
  | _,_ -> failwith "bad uplm event"

  and upnm_hdlr ev = match getType ev with
  | EView ->
      up_view := true;
      let casts = Array.copy casts in
      chk_casts := Some casts ;
      chk_sync () ;
      dnlm (create name ECast[Dn_options xxx_no_total]) 
      (Gossip(vs.view_id,casts,fail)) ; 
      upnm ev
  | EFail ->
      let failed = getFailures ev in
      List.iter (fun rank ->
      	fail.(rank) <- true
      ) failed ;
      upnm ev
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv NoHdr
  and dnnm_hdlr ev = dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Procure.install "chk_evs" (Procure.initChk_evs l)

(**************************************************************)
