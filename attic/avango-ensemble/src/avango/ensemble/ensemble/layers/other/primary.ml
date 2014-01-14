(**************************************************************)
(* PRIMARY.ML *)
(* Author: Mark Hayden, Zhen Xiao, 5/97 *)
(**************************************************************)
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "PRIMARY"
(**************************************************************)

(* Nservers takes a view state and a bit-mask-option.  All
 * members who are not clients and not out-of-date and are
 * in the bit mask are counted as servers.  If the bit-mask
 * is None then it defaults to all true.  
 *)
let nservers vs mask =
  try
    let nmembers = Arrayf.length vs.view in
    if Arrayf.length vs.clients <> nmembers then
      failwith "PRIMARY:bad clients field of View.state record" ;

    let mask =
      match mask with
      | None -> Arrayf.create nmembers true
      | Some mask -> mask
    in

    let nservers = ref 0 in
    for i = 0 to pred nmembers do
      if (not (Arrayf.get vs.clients i))
(*
      && (not (Arrayf.get vs.out_of_date i))
      && Arrayf.get vs.out_of_date i = vs.ltime
*)
      && Arrayf.get mask i
      then
	incr nservers
    done ;

    !nservers
  with 
  | Failure(_) as e -> raise e
  | _ -> failwith "PRIMARY:error in nservers"

(**************************************************************)

type state = {
  mutable in_view : bool Arrayf.t ;
  quorum : View.state -> bool Arrayf.t option -> bool
}

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "in_view=%s\n" (Arrayf.bool_to_string s.in_view)
|])

let init _ (ls,vs) =
  let quorum = Param.int vs.params "primary_quorum" in
  if quorum = 0 then (
    eprintf "PRIMARY:use of the PRIMARY protocol layer requires\n" ;
    eprintf "  setting the primary_quorum parameter.  See ref.ps\n" ;
    eprintf "  under 'primary' for more information.  Exiting.\n" ;
    exit 2 ;
  ) ;
  if 2 * quorum <= nservers vs None then 
    eprintf "Warning: primary_quorum is no more than half of the #servers\n" ;
  { in_view = ls.falses ;
    quorum = (fun vs mask -> nservers vs mask >= quorum)
  }

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
(*let ack = make_acker name dnnm in*)
  let log = Trace.log2 name ls.name in

  let up_hdlr ev abv () = up ev abv

  and uplm_hdlr ev hdr = match getType ev,hdr with
  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with

    (* Send a message to the coordinator saying that I'm
     * here.  
     *)
  | EInit ->
      log (fun () -> sprintf "primary partition=%b"  vs.primary) ;
      upnm ev ;

      (* If we have a quorum, but we are not marked as
       * primary, then prompt a view change.  
       *)
      log (fun () -> sprintf "am_coord=%b primary=%b quorum=%b"
	  ls.am_coord vs.primary (s.quorum vs None)) ;
      if ls.am_coord
      && s.quorum vs None
      && not vs.primary 
      then
	dnnm (create name EPrompt[])

    (* Update the info about which members are currently in view.
     *)
  | EPresent ->
      s.in_view <- getPresence ev ;

      (* If we have a quorum, but we are not marked as
       * primary, then prompt a view change.  
       *)
      log (fun () -> sprintf "present am_coord=%b primary=%b quorum=%b"
	  ls.am_coord vs.primary (s.quorum vs (Some s.in_view))) ;
      if ls.am_coord
      && s.quorum vs (Some s.in_view) (*MH:was None*)
      && not vs.primary 
      then
	dnnm (create name EPrompt[]) ;

      upnm ev

    (* At all members, check that the ltime is one more
     * than the current logical time.
     *)
  | EView ->
      let next_vs = getViewState ev in

      if next_vs.prev_ids = [] then (
        eprintf "PRIMARY:view_state=%s\n" (View.string_of_state next_vs) ;
	failwith "zero-length next_vs.prev_ids field" ;
      ) ;

(* This check is invalid.
      if Arrayf.get next_vs.view 0 <> snd (List.hd next_vs.prev_ids) then (
        eprintf "PRIMARY:view_state=%s\n" (View.string_of_state next_vs) ;
	failwith "inconsistent view_id & view" ;
      ) ;
*)

      (* The INTER layer guarantees that the coordinator's
       * previous view id is the first in the list of prev
       * view_ids.  
       *)
      let coord_ltime = fst (List.hd next_vs.prev_ids) in
      let next_ltime = next_vs.ltime in
      let succ_ltime = next_ltime = succ coord_ltime in

      (* We leave the primary bit set if it was already
       * set and the one additional criteria is met.
       *)
      let primary = 
	next_vs.primary && succ_ltime
      in

      (* We set the out_of_date field to be all-false when
       * we find that there is a view with the primary
       * bit set and the xfer bit is unset.  This means
       * that all members must have completed a successful
       * state transfer.
       *)
      (* At this point, the xfer field should have been
       * set by the XFER layer.
       *)
      let out_of_date =
        if primary && not next_vs.xfer_view then (
	  Arrayf.create (Arrayf.length next_vs.view) next_vs.ltime
	) else next_vs.out_of_date
      in

      log (fun () -> sprintf "next_vs.primary=%b succ_ltime=%b -> primary=%b"
	next_vs.primary succ_ltime primary
      ) ;

      let next_vs = View.set next_vs [
	Vs_primary primary ;
	Vs_out_of_date out_of_date
      ] in
      upnm (set name ev [ViewState next_vs])

  | EDump -> dump vf s ; upnm ev
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv ()

  and dnnm_hdlr ev = match getType ev with

    (* EView: decide whether next view is 
     * primary.
     *)
  | EView ->
      let next_vs = getViewState ev in

      (* Check whether we have a quorum in our view.
       *)
      let in_view_quorum = s.quorum vs (Some s.in_view) in

      (* Check whether the next view has a quorum.
       *)
      let next_quorum = s.quorum next_vs None in

      (* Check if the next coord is the same as current one.
       *)
      let this_coord = Arrayf.get vs.view vs.coord in
      let next_coord = Arrayf.get next_vs.view next_vs.coord in
      let same_coord = next_coord = this_coord in

      (* These are the first three conditions for primariness.
       * The next one is in upnm EView.
       *)
      let primary =
	in_view_quorum &&
	next_quorum &&
	same_coord
      in

      log (fun () -> sprintf "in_view_quorum=%b next_quorum=%b same_coord=%b -> primary=%b"
	in_view_quorum next_quorum same_coord primary
      ) ;

      let next_vs = View.set next_vs [Vs_primary primary] in
      dnnm (set name ev [ViewState next_vs])

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ =
  Param.default "primary_quorum" (Param.Int 0) ;
  Layer.install name l
