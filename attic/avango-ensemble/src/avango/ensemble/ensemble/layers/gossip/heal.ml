(**************************************************************)
(* HEAL.ML *)
(* Author: Mark Hayden, 6/95 *)
(* Based on code by Robbert vanRenesse & Dalia Malki *)
(* Originally named merge.ml *)
(**************************************************************)
(* Notes:

 * Only merge with higher numbered views to prevent cycles.

 * Only communicate when I think I will be able to merge
 * quickly.

 * Tries to prevent merge attempts that may fail.  Be
 * conservative.

 *)
(**************************************************************)
open Layer
open Util
open View
open Event
open Trans
(**************************************************************)
let name = Trace.filel "HEAL"
let failwith = Trace.make_failwith name
(**************************************************************)

type header = NoHdr

type state = {
  mutable wait_view  : bool ;
  all_present        : Once.t ;
  present            : bool array ;
  mutable merge_nmem : nmembers ;
  mutable merge_vid  : View.id ;
  mutable merge_con  : Endpt.full ;
  mutable bcasted    : bool
}

(**************************************************************)

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "wait_view=%b\n" s.wait_view ;
  sprintf "present=%s\n" (string_of_bool_array s.present) ;
  sprintf "view_id=%s con_view_id=%s\n"
    (View.string_of_id ls.view_id)
    (View.string_of_id s.merge_vid)
|])

(**************************************************************)

let init _ (ls,vs) = {
  wait_view  = false ;
  all_present = Once.create "all_present" ;
  present    = Array.create ls.nmembers false ;
  merge_vid  = ls.view_id ;
  merge_nmem = ls.nmembers ;
  merge_con  = ((Arrayf.get vs.view 0) , (Arrayf.get vs.address 0)) ;(*BUG?*)
  bcasted    = false
}

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
(*let ack = make_acker name dnnm in*)
  let log = Trace.log2 name ls.name in
  let my_con = (ls.endpt,ls.addr) in
  let gossip_send () =
    let msg = HealGos(vs.proto_id,ls.view_id,my_con,vs.view,Arge.get_servers()) in
    msg
  in

  let all_present () =
    Once.isset s.all_present
  in

  let gossip_recv (bc_proto_id,bc_view_id,bc_con,bc_view,bc_servers) =
    log (fun () -> "gossip_recv") ;
    let bc_coord = fst bc_con in
    let my_servers = Arge.get_servers() in

(* Removed: because prevented cases that were legal.
    (* If I hear of some of my members then remove them.
     *)
    if ls.am_coord
    && (not s.wait_view)
    && snd bc_view_id <> snd ls.view_id
    && bc_view_id > ls.view_id 
    && intersect bc_view (except ls.endpt vs.view) <> []
    then (
      let suspects = intersect bc_view vs.view in
      let suspects = except ls.endpt vs.view in
      let suspects = List.map (fun e -> index e vs.view) suspects in
      dnnm (suspectReason name suspects name) ;
    ) ;
*)

    (* Check if I'm interested in the other partition at all.
     *)
    let bc_nmem = Arrayf.length bc_view in
    let view = Lset.inject vs.view in
    let bc_view = Lset.inject bc_view in

    if ls.am_coord 			(* I'm coordinator *)
    && bc_proto_id = vs.proto_id	(* protocols are the same *)
    && all_present ()			(* everyone is present *)
    && (not (Arrayf.mem bc_coord vs.view)) (* he is not in my view *)
    && Lset.disjointa view bc_view       (* in fact, no members are shared *)
    && (my_servers = [] or bc_servers = [] or not (Lset.disjoint bc_servers my_servers))
    then (
      log (fun () -> sprintf "recd interesting bcast from %s (curr contact=%s)"
	(Endpt.string_of_id bc_coord) 
	(Endpt.string_of_id (fst s.merge_con))) ;

      (* First check to see if I want to merge with him.  This decision
       * is based on several criteria.  First, if I'm a primary partition,
       * then I never merge with other members.  Second, I only merge with
       * partitions that have more members or who have large view ids.
       *)
      if not vs.primary
      && (bc_nmem, bc_view_id) > (s.merge_nmem, s.merge_vid)
      then (
	log (fun () -> sprintf "suggesting merge with %s" (View.string_of_id bc_view_id)) ;
	s.merge_vid <- bc_view_id ;
	s.merge_con <- bc_con ;
	s.merge_nmem <- bc_nmem ;

	(* If I'm not installing a view currently, start a
	 * view change.
	 *)
	if not s.wait_view then (
	  s.wait_view <- true ;
	  dnnm (create name EPrompt [])
	)
      ) ;

      (* Otherwise, he might think my partition is interesting.
       *)
      if ls.view_id = s.merge_vid	(* I'm not planning to merge myself *)
      && not s.wait_view		(* I'm not waiting for a view *)
      && not s.bcasted			(* don't bcast all the time *)
      && (bc_nmem, bc_view_id) < (ls.nmembers, ls.view_id)(* check that I am 'older' than this one *)
      && (my_servers = [] or bc_servers = [] or not (Lset.disjoint bc_servers my_servers))
      then (
	s.bcasted <- true ;
	log (fun () -> "bcasting(recv)") ;
	dnnm (create name EGossipExtDir[Address(snd bc_con) ; gossip_send ()])
      )
    ) else (
      log (fun () -> sprintf "gossip dropped from %s" (Endpt.string_of_id bc_coord)) ;
    )
  in

  let up_hdlr ev abv NoHdr = up ev abv

  and uplm_hdlr ev NoHdr = match getType ev with
  | ESend iovl ->
      s.present.(getPeer ev) <- true ;
      if array_for_all ident s.present then
	Once.set s.all_present ;
      log (fun () -> "bcasting(all_present)") ;
      dnnm (create name EGossipExt[(gossip_send ())]) ;
      Iovecl.free iovl

  | _ ->
      failwith "bad local message"

  and upnm_hdlr ev = match getType ev with
  | EInit ->
      if not ls.am_coord then
	dnlm (sendPeer name vs.coord) NoHdr
      else (
        s.present.(ls.rank) <- true ;
	if array_for_all ident s.present then
	  Once.set s.all_present
      ) ;
      upnm ev

  | EGossipExt ->
      getExtendOpt ev (function
	| HealGos(a,b,c,d,e) -> gossip_recv (a,b,c,d,e) ; true
	| _ -> false) ;
      upnm ev

  | EBlock ->
      s.wait_view <- true ;
      upnm ev

  | EBlockOk ->
      if s.merge_con = my_con then
	upnm ev
      else (
	log (fun () -> sprintf "EBlock(suggest->%s)" (Endpt.string_of_id (fst s.merge_con))) ;
	upnm (set name ev [Contact(s.merge_con,Some s.merge_vid)])
      )

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv NoHdr

  and dnnm_hdlr ev = match getType ev with
  | EGossipExt ->
      s.bcasted <- false ;
      let ev =
	if ls.am_coord			(* I'm elected *)
	&& not s.wait_view		(* I'm not waiting for view *)
	&& ls.view_id = s.merge_vid	(* I'm not planning to merge *)
	&& all_present ()               (* everyone is present *)
	then
	  set name ev [gossip_send ()]
	else ev
      in
      dnnm ev

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
