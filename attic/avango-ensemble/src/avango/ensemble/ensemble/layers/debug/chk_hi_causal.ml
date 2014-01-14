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
(* TCAUSAL.ML : Checking causal ordering properties *)
(* Author: Ohad Rodeh, 12/96 *)
(* Based on code by: Mark Hayden *)
(**************************************************************)
open Layer
open Trans
open Util
open Event
open View
(**************************************************************)
let name = Trace.filel "CHK_HI_CAUSAL"
(**************************************************************)


type header = NoHdr
| Cast of int * Endpt.id * rank * seqno array

type member = {
  mendpt : Endpt.id ;
  mutable failed : bool ;
  mutable cast_up : int
}

type state = {
    mutable uniqueid : int;
    members  : member array
  } 

(**************************************************************)

type ('a,'b) t = (header,'a,'b, state) Layer.t

(**************************************************************)

let init _ (ls,vs) = 
  let mmbs = (Array.of_list vs.view) in {
  uniqueid = 0;
  members = Array.map (fun endpt -> {
      mendpt = endpt ;
      failed = false ;
      cast_up = 0 
    }) mmbs
} 

(**************************************************************)

let print_members members =
  for_array (fun rank m ->
    sprintf "mbr(%d):" rank ;
    if m.failed then
      eprintf "failed\n"
    else
      eprintf "%s,cup=%d\n" (Endpt.string_of_id m.mendpt) m.cast_up 
  ) members

let dump s vs  =
  eprintf "CHK_HI_CAUSAL:dump:%s\n" ls.name ;
  sprintf "rank=%d\n" ls.rank ;
  print_members s.members

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log name ls.name in

  let check_acks ack_vct = 
    let v = ref [] in 
    for i = 0 to ls.nmembers-1 do
      if s.members.(i).cast_up <= ack_vct.(i) then v := (i,ack_vct.(i)) :: !v;
    done;
    !v
  in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | ECast, Cast(unique,chk_from,chk_origin,ack_vct) -> (
      let org = (getPeer ev) in 
      let m = s.members.(org) in
      let {cast_up=cast_up; mendpt=endpt} = m in

      if chk_from <> m.mendpt  then (
      	eprintf "CHK_CAUSAL:incorrect origin m.mendpt.";
     	eprintf "sent=(%s,%d)" (Endpt.string_of_id chk_from) chk_origin;
	eprintf "<> (%s,%d)=my_info\n" (Endpt.string_of_id m.mendpt) org;
        failwith sanity
        ) ;

      let l = check_acks ack_vct in 
      if List.length l <> 0 then (
	eprintf "<CHK_CAUSAL:deliverying (r=%d,s=%d) without predecessor" 
	org s.members.(org).cast_up;
	List.iter (fun (r,s) -> printf "(r=%d,s=%d)" r s) l;
	eprintf ">\n";
        failwith sanity
	);

      s.members.(org).cast_up <- succ s.members.(org).cast_up;
      up ev abv
      )

  | _, NoHdr -> up ev abv
  | _, _     -> failwith bad_up_event
  and uplm_hdlr _ _ = failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | EFail ->
      let suspicions = getFailures ev in
      List.iter (fun i ->
      	s.members.(i).failed <- true
      ) suspicions ;
      upnm ev
  | EDump -> (dump s vs ; upnm ev)
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | ECast when (not (getUnreliable ev))
    && not (getNoTotal ev) -> 
      let ack_vct = Array.map (fun m -> m.cast_up-1) s.members in 
      dn ev abv (Cast(s.uniqueid,ls.endpt,ls.rank,ack_vct)) ;
      s.uniqueid <- succ s.uniqueid

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Elink.layer_install name l

(**************************************************************)
