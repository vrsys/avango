(**************************************************************)
(* CHK_TOTAL.ML *)
(* Author: Mark Hayden, 10/96 *)
(* Tests that all members see broadcasts in total ordering.
 * Assumes fifo from members, too. *)
(**************************************************************)
open Trans
open Layer
open Event
open View
open Util
(**************************************************************)
let name = Trace.filel "CHK_TOTAL"
let failwith = Trace.make_failwith name
(**************************************************************)

type header = NoHdr | NoTot | Gossip of View.id * rank Arrayf.t

let init = ignore2

let hdlrs () (ls,vs) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let log = Trace.log2 name ls.name in
  let recd = Queuee.create () in
(*let ack = make_acker name dnnm in*)
  let got_view = ref false in

  let chk_ranks = ref None in
  let chk_rmt = ref [] in
  let chk_total () =
    if_some !chk_ranks (fun (view_id,ranks) ->
      let ranks = Arrayf.to_array ranks in
      List.iter (fun (rmt_rank,rmt_view_id,rmt_ranks) ->
	let rmt_ranks = Arrayf.to_array rmt_ranks in
	log (fun () -> sprintf "verifying:%d->%d" ls.rank rmt_rank) ;
	if view_id = rmt_view_id 
	&& rmt_ranks <> ranks
	then (
	  eprintf "CHK_TOTAL:ordering failure{%s}\n" ls.name ;
	  eprintf "  rank: %d rmt_rank: %d\n" ls.rank rmt_rank ;
	  eprintf "  view_id=%s\n" (View.string_of_id ls.view_id) ;
	  eprintf "     my cast:%s\n" (string_of_int_array ranks) ;
	  eprintf "    rmt cast:%s\n" (string_of_int_array rmt_ranks) ;
	  dnnm (create name EDump[])
(*
	  failwith "ordering failure"
*)
	)
      ) !chk_rmt ;
      chk_rmt := []
    )
  in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | ECast _,NoHdr ->
      if !got_view then
      	failwith "ECast after EView" ;
      Queuee.add (getPeer ev) recd ;
      up ev abv
  | ECast _,NoTot ->
      up ev abv
  | _,NoHdr -> up ev abv
  | _ -> failwith bad_up_event

  and uplm_hdlr ev hdr = match getType ev,hdr with
  | ECast iovl,Gossip(view_id,ranks)  ->
      let origin = getPeer ev in
      chk_rmt := (origin,view_id,ranks) :: !chk_rmt ;
      chk_total () ;
      Iovecl.free iovl
  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | EView ->
      let vs = getViewState ev in
      let view_id = id_of_state vs in
      got_view := true ;
      let ranks = Queuee.to_list recd in
      let ranks = Arrayf.of_list ranks in
      chk_ranks := Some(view_id,ranks) ;
      chk_total () ;
      dnlm (create name (ECast Iovecl.empty) [NoTotal]) (Gossip(view_id,ranks)) ;
      upnm ev
  | _ -> upnm ev
  and dn_hdlr ev abv = match getType ev with
  | ECast _ ->
      if getNoTotal ev || not (getApplMsg ev) then (
      	dn ev abv NoTot
      ) else (
      	if !got_view then failwith "ECast after EView" ;
      	dn ev abv NoHdr
      )
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
