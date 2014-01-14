(**************************************************************)
(* XFER.ML : determines state tranfer views *)
(* Author: Mark Hayden, 3/96 *)
(* With suggestions by Samuel Weber *)
(**************************************************************)
(* Rules for determining whether a view is a state transfer
 * view.
 *
 * Singleton views are *never* XFER views.
 *
 * Non-singleton views are XFER if (A or (B and not C)) holds.
 * Where A, B, and C are:
 *
 *   A. This view was merged from several previous views.
 *      This property is checked in EView when the new
 *      view is ready.
 *
 *   B. Previous view was a XFER view.  This property is
 *      also checked un EView when the new view is ready.
 *
 *   C. All members in previous view took XferDone action.
 *      This property is checked at the coordinator in EView
 *      when sending new view out.  The coordinator examines
 *      the xfer_done array, which marks which members did
 *      XferDone in the current view (and sent message to
 *      the coordinator.
 *)

(* NOTE: the Primary protocol requires that the xfer field
 * in the view state be set by the time the EView event is
 * going up the protocol stack.  
 *)

(**************************************************************)
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.file "XFER"
(**************************************************************)

type header = NoHdr
 | XferDone

(**************************************************************)

type state = {
  xfer_done : bool array		(* who has EXferDone'd? *)
}

(**************************************************************)

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "xfer_done=%s\n" (string_of_bool_array s.xfer_done)
|])

(**************************************************************)

let init _ (ls,vs) = {
  xfer_done = Array.create ls.nmembers false
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
(*let ack = make_acker name dnnm in*)
  let log = Trace.log2 name ls.name in

  (* CHECK_XFER_DONE: mark the member as being done.  If I'm
   * coordinator and everyone is now done, then prompt a
   * view change.
   *)
  let check_xfer_done rank =
    let prev = s.xfer_done.(rank) in	(* save previous val *)
    s.xfer_done.(rank) <- true ;	(* mark entry as true *)

    if vs.xfer_view			(* this is an XFER view *)
    && ls.am_coord			(* I am coordinator *)
    && (not prev)			(* this entry used to be false *)
    && array_for_all ident s.xfer_done	(* all entries are now true *)
    then (
      log (fun () -> "EPrompt");
      dnnm (create name EPrompt[])	(* start view change *)
    )
  in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | _,NoHdr -> up ev abv
  | _,_ -> failwith bad_up_event

  and uplm_hdlr ev hdr = match getType ev,hdr with

    (* ECast|ESend(XferDone): other member is telling me
     * that its state transfer is complete.
     *)
  | (ECast iovl|ESend iovl), XferDone ->
      check_xfer_done (getPeer ev) ;
      (*ack ev ;*)
      Iovecl.free iovl

  | _ -> failwith "bad uplm event"

  and upnm_hdlr ev = match getType ev with
    (* EView: just for debugging.
     *)
  | EView ->
      let vs = getViewState ev in
      log (fun () -> sprintf "xfer=%b" vs.xfer_view) ;
      upnm ev

    (* EXferDone: Application has completed state transfer.
     * If haven't done so already, tell rest of the group
     * (or only myself if I'm the coordinator).
     *)
  | EXferDone ->
      log (fun () -> Event.to_string ev) ;
      if ls.am_coord then (
	check_xfer_done ls.rank
      ) else (
        (* PERF: check if necessary to send this info out *)
        dnlm (sendPeer name 0) XferDone
      ) ;
   
      free name ev

  | EDump -> dump vf s ; upnm ev
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr ev = match getType ev with
    (* EView: coordinator tells others if everyone did
     * XferDone.
     *)
  | EView ->
      let xfer_all_done = array_for_all ident s.xfer_done in
      let vs_new = getViewState ev in
      let view_old = Arrayf.to_list vs.view in
      let view_new = Arrayf.to_list vs_new.view in
      let mergers = Lset.subtract view_new view_old <> [] in
      let xfer_view =
      	   (Arrayf.length vs_new.view > 1) 
      	&& (mergers || (vs.xfer_view && not xfer_all_done))
      in

      let vs_new = View.set vs_new [Vs_xfer_view xfer_view] in
      let ev = set name ev [ViewState vs_new] in

      dnnm ev

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
