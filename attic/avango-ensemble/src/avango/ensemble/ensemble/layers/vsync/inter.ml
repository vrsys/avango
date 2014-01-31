(**************************************************************)
(* INTER.ML *)
(* Author: Mark Hayden, 10/96 *)
(**************************************************************)
open Util
open Layer
open View
open Event
open Trans
(**************************************************************)
let name = Trace.filel "INTER"
(**************************************************************)

type header = NoHdr
	
type states =
  | Normal
  | Merging
  | FailedMerge
  | InstalledView

type state = {
  mutable state		: states ;	(* automata state *)
  mutable blocked	: bool ;	(* have I seen an EBlockOk? *)
  mutable endpts_seen 	: Endpt.id Lset.t ; (* endpts I've seen anywhere *)
  mutable mergers       : View.state list (* views merging with me *)
}

let string_of_state = function
  | Normal  	  -> "Normal"
  | Merging 	  -> "Merging"
  | FailedMerge   -> "FailedMerge"
  | InstalledView -> "InstalledView"

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "am_coord=%b, blocked=%b\n" ls.am_coord s.blocked ;
  sprintf "view_id=%s\n" (View.string_of_id ls.view_id) ;
  sprintf "view=%s\n" (View.to_string vs.view) ;
  sprintf "endpts_seen=%s\n" (Lset.to_string Endpt.string_of_id s.endpts_seen)
|])

let init _ (ls,vs) = {
  endpts_seen  	= Lset.inject vs.view ;
  state		= Normal ;
  blocked	= false ;
  mergers       = []
}


let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith   = layer_fail dump vf s name in
  let log        = Trace.log2 name ls.name in
  let assertions l = Array.iter (fun (v,s) -> if not v then failwith s) l in

  let up_hdlr ev abv NoHdr = up ev abv
  and uplm_hdlr _ _ = failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | EView ->
      s.state <- InstalledView ;
      upnm ev

    (* EBlockOk: mark state and pass on.
     *)
  | EBlockOk -> 
      s.blocked <- true ; 
      upnm ev

    (* EMergeRequest (Coordinator): merge request from remote
     * coordinator.
     *)
  | EMergeRequest ->
      let merge_vs = getViewState ev in
      let merge_view = Lset.inject merge_vs.view in
      assert (merge_vs = getViewState ev) ;

      if ls.am_coord			(* I'm coord *)
      && not s.blocked			(* haven't blocked yet *)
      && Lset.disjointa merge_view s.endpts_seen (* no endpt that I've seen before *)
      then (
      	(* Accept request.  Update logical time, add to
	 * lists of possibly previous view_ids and the endpts
	 * we've seen so far.  Then send on up.  
      	 *)
	log (fun () -> sprintf "EMergeRequest:accepted:%s" (View.to_string merge_vs.view)) ;
	s.endpts_seen <- Lset.uniona s.endpts_seen merge_view ;
        s.mergers     <- merge_vs :: s.mergers ;
	upnm (set name ev [ViewState merge_vs])
      ) else (
      	(* Deny request.
	 *)
	log (fun () -> sprintf "EMergeRequest:rejected:%s%s%s%s" 
	  (View.to_string merge_vs.view)
	  (if not ls.am_coord then ":not_coord" else "")
	  (if s.blocked then ":blocked" else "")
	  (if Lset.disjointa merge_view s.endpts_seen then "" else ":seen_before")) ;
	
	let merge_con = View.coord_full merge_vs in
	let merge_vid = View.id_of_state merge_vs in
      	dnnm (create name EMergeDenied[
      	  Contact(merge_con,Some merge_vid) ;
	  MergeGos((merge_con,Some merge_vid),0,EMergeDenied,merge_vs) ;
	  ViewState merge_vs
      	]) ;
      	free name ev
      )

    (* EMergeGranted(Coordinator): Remote merging
     * coordinator: new view arrived from coordinator.
     * bounce off bottom.  
     *)
  | EMergeGranted ->
      let new_vs = getViewState ev in
      assert (new_vs = getViewState ev) ;
      if ls.am_coord
      && s.state = Merging 
      && List.mem ls.view_id new_vs.prev_ids
      (* BUG: also check that correct members are in the view. *)
      then (
      	log (fun () -> "EMergeGranted:accepted") ;
	assert (new_vs.ltime > vs.ltime) ;

	(* Install the view locally.
	 *)
	s.state <- InstalledView ;
	dnnm (create name EView[ViewState new_vs])
      ) else (
      	log (fun () -> "EMergeGranted:rejected")
      ) ;
      free name ev

    (* EMergeDenied: if the view state matches mine,
     * then pass up an EMergeFailed event.
     *)
  | EMergeDenied ->
      let merge_vs = getViewState ev in
      assert (merge_vs = getViewState ev) ;
      if ls.am_coord
      && s.state = Merging 
      && vs = merge_vs
      then (
        log (fun () -> "EMergeDenied(this view)") ;
      	s.state <- FailedMerge ;
	upnm (create name EMergeFailed[])
      ) ;
      free name ev

    (* EMergeFailed: my EMerge timed out.  If the merge
     * was still in progress, send it on up.
     *)
  | EMergeFailed ->
      if ls.am_coord && s.state = Merging then (
        log (fun () -> "EMergeFailed") ;
      	s.state <- FailedMerge ;
      	upnm ev
      ) else (
      	free name ev
      )

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv NoHdr

  and dnnm_hdlr ev = match getType ev with
    (* EView: Forward view to merging groups.
     *)
  | EView ->
      let new_vs = getViewState ev in
      assert (s.state <> InstalledView) ;

      (* Calculate the new view id and add to event.
       *)
      s.state <- InstalledView ;

      (* Send view state to merging coordinators (who will forward
       * it for me).  
       *)
      log (fun () -> sprintf "EView:view=%s" (View.to_string new_vs.view)) ;
      List.iter (fun merge_vs ->
	let contact = View.coord_full merge_vs in
 	let view_id = View.id_of_state merge_vs in
        log (fun () -> sprintf "EView:with:%s" (Endpt.string_of_full contact)) ;
        dnnm (create name EMergeGranted[
          Contact(contact,Some view_id) ;
	  MergeGos((contact,Some view_id),0,EMergeGranted,new_vs) ;
	  ViewState new_vs
        ])
      ) s.mergers ;

      (* Pass view to my partition.
       *)
      dnnm ev

    (* EMergeRequest. Sends merge request.  Note that EMerge
     * should only be done after zero failures and by the
     * original coordinator of the group.  
     *)
  | EMergeRequest ->
      (* Check a bunch of assertions. *)
      let contact,contact_vid = getContact ev in
      assert ls.am_coord ;
      assert (not (Arrayf.mem (fst contact) vs.view)) ;
      assert (s.state = Normal) ;

      log (fun () -> sprintf "EMergeRequest:with:%s" (Endpt.string_of_full contact)) ;
      s.state <- Merging ;
      let ev = set name ev [ViewState vs;MergeGos((getContact ev),0,EMergeRequest,vs)] in
      dnnm ev

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
