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
(* CHK_AGREE.ML *)
(* Author: OHAD RODEH, 12/96 *)
(* Tests that all members see agree broadcasts in total order. *)
(* Based on code by mark hayden CHK_TOTAL *)
(**************************************************************)
(*
Overview
--------
  This layer traces messages sent in a partition by the process.
When a view change occurs, layers exchange logs, and check for
consistency. At the moment, consistency is checked between 
processes that passed through the same 
  view-V 
and
  transitional view T(V)
*)
(**************************************************************)
open Layer
open Event
open View
open Util
(**************************************************************)
let name = Trace.filel "CHK_AGREE"
(**************************************************************)

type header = NoHdr | Appl | Gossip of rank list * bool Arrayf.t

type chk = Rmt of origin * rank list * bool Arrayf.t

type state = {
    mutable got_view        : bool;
    mutable trans           : bool array;
    mutable msg_log         : rank list;
    mutable chk_rmt         : chk list
   }

(**************************************************************)

type ('a,'b) t = (header,'a,'b, state) Layer.t

(**************************************************************)

let init _ (ls,vs) = 
  let n = ls.nmembers in {
  trans       = array_create name n true;
  got_view    = false;
  msg_log     = [];
  chk_rmt     = []
}
    
(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let log = Trace.log name ls.name in
  let failwith s = (dnnm (create name EDump[])) in (* BUG? *)

  let string_of_rk_list l = 
    string_of_list (function r -> sprintf "%d" r) l
  in

  let chk_total () =
      if s.got_view then  
	begin
      	  List.iter (fun (Rmt(rmt_rank,rmt_log,trans_view)) ->
	    if trans_view = s.trans then 
	      begin
    		log (fun () -> sprintf "verifying:%d->%d" ls.rank rmt_rank) ;
	    	if s.msg_log <> rmt_log then (
	      	  eprintf "CHK_AGREE:ordering failure{%s}\n" ls.name ;
		  eprintf "  rank: %d rmt_rank: %d\n" ls.rank rmt_rank ;
		  sprintf "view_id=%s\n" (View.string_of_id vs.view_id) ;
		  sprintf "   my cast:%s\n" (string_of_rk_list s.msg_log) ;
		  sprintf "  rmt cast:%s\n" (string_of_rk_list rmt_log) ;
		  failwith "ordering failure")
	      end
	      ) s.chk_rmt ;
	  s.chk_rmt <- [] 
	end
  in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | ECast,Appl -> 
      if ev.flags land xxx_tcausal <> xxx_tcausal then
	failwith "SANITY";
      if ev.flags land xxx_no_total <> 0 then 
	failwith "SANITY";
      if not s.got_view then
	failwith "ECast after EView" ;
      s.msg_log <- s.msg_log @ [(getPeer ev)];
      up ev abv
  | _ -> up ev abv

  and uplm_hdlr ev hdr = match getType ev,hdr with
  | ECast,Gossip(rmt_log, trans_view) -> 
      if (getPeer ev) <> ls.rank 
      then (
	s.chk_rmt <- (Rmt((getPeer ev),rmt_log,trans_view)) :: s.chk_rmt ;
      	chk_total ()
	);
      free name ev
  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with
   | EView -> 
       s.got_view <- true;
       chk_total () ;
       dnlm (create name ECast[Dn_options xxx_no_total]) (Gossip (s.msg_log,s.trans)) ;
       upnm ev
   | ETransView -> 
       s.trans <- getTransitional ev;
       s.msg_log <- s.msg_log @ [(-1)];
       upnm ev
   | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | ECast ->
      if getApplMsg ev then 
	dn ev abv Appl
      else 
	dn ev abv NoHdr
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Elink.layer_install name l

(**************************************************************)
