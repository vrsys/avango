(**************************************************************)
(* VSYNC.ML *)
(* Author: Mark Hayden, 11/97 *)
(**************************************************************)
open Event
open Layer
open View
open Trans
open Util
(**************************************************************)
let name = Trace.filel "VSYNC"
(**************************************************************)

type header =
  | NoHdr
  | General of bool Arrayf.t * (rank * seqno Arrayf.t * seqno Arrayf.t) Arrayf.t

type state = {
  mutable failed : bool Arrayf.t ;
  mutable delay : (rank * bool Arrayf.t * seqno Arrayf.t * seqno Arrayf.t) list ;
  mutable sends : seqno Arrayf.t array ;
  mutable casts : seqno Arrayf.t array ;
  mutable heard : bool array ;
  mutable donesync : bool ;
  mutable blocked : bool ;

(* Following fields added by TC for new protocol, 12/12/97 *)
  mutable coord : rank;	  (* who do I think is coord? *)
(* ;
  mutable f : bool Arrayf.t array
*)
} 

(**************************************************************)

let dump = Layer.layer_dump name (fun (ls,vs) s -> Array.concat [[|
  sprintf "blocked=%b coord=%d donesync=%b\n" s.blocked s.coord s.donesync ;
  sprintf "failed=%s\n" (Arrayf.bool_to_string s.failed) ;
  sprintf "heard=%s\n" (string_of_bool_array s.heard) ;
|];(Array.mapi (fun i c ->
  sprintf "casts(%02d)=%s\n" i (Arrayf.int_to_string c)
) s.casts);(Array.mapi (fun i s ->
  sprintf "sends(%02d)=%s\n" i (Arrayf.int_to_string s)
) s.sends)])
(* ;
  for i = 0 to pred ls.nmembers do
    sprintf "    f(%02d)=%s\n" i (Arrayf.bool_to_string s.f.(i))
  done
*)

(**************************************************************)

let init _ (ls,vs) = {
  blocked = false ;
  donesync = false ;
  coord = 0 ;
  delay = [] ;
  failed = ls.falses ;
  heard = Arrayf.to_array ls.falses ;
  sends = Array.create ls.nmembers ls.zeroes ;
  casts = Array.create ls.nmembers ls.zeroes
(* ;
  f= Array.create ls.nmembers (ls.falses)
*)
}  

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in
  let logs = Trace.log3 Layer.syncing ls.name name in
  let logb = Trace.logl3 Layer.block ls.name name in
(*let ack = make_acker name dnnm in*)

  let check_ready () =
    if not s.donesync
    && array_for_all ident s.heard
    then (
      s.donesync <- true ;
(*
      dump vf s ;
*)

      let sends =
	Array.map (fun row ->
	  Arrayf.get row ls.rank
	) s.sends 
      in

      let casts = Array.create ls.nmembers 0 in
      for i = 0 to pred ls.nmembers do
	if not (Arrayf.get s.failed i) then (
	  for j = 0 to pred ls.nmembers do
	    casts.(j) <- int_max casts.(j) (Arrayf.get s.casts.(i) j)
	  done
	)
      done ;

      logs (fun () -> sprintf "delivering ESyncInfo (casts=%s)" (string_of_int_array casts)) ;
      dnnm (create name ESyncInfo [
	ApplCasts (Arrayf.of_array casts) ;
	ApplSends (Arrayf.of_array sends)
      ]) ;
    
      if ls.rank = s.coord then (
        (* Create an array with info from all non-failed members.
         *)
	let info =
	  Arrayf.mapi (fun i failed ->
	    if failed then None else Some(i, s.casts.(i), s.sends.(i))
	  ) s.failed
	in
	let info = Arrayf.filter_nones info in

	log (fun () -> sprintf "COORD casting info") ;

        (* Send it out.
         *)
	dnlm (castEv name) (General(s.failed, info)) ;
      )
    )
  in

(*
    s.f.(origin) <- failed ;
*)

  let got_info origin failed casts sends =
    (* Add to my list of info.
     *)
    s.delay <- (origin,failed,casts,sends) :: s.delay ;

    (* Go through the list to see if there is anything
     * to be done.
     *)
    s.delay <- List.filter (fun (origin,failed,casts,sends) ->
      (* If the failures do not include at least as much
       * as what I've heard so far, then ignore them.
       *)
      if s.failed = failed then (
	s.heard.(origin) <- true ;
	s.casts.(origin) <- casts ;
	s.sends.(origin) <- sends ;
        log (fun () -> sprintf "got_info");     
	check_ready () ;
	false				(* remove *)
      ) else (
	true				(* keep *)
      )
    ) s.delay
  in

  let got_local ev =
    logs (fun () -> sprintf "got_local:%s" (Event.to_string ev)) ;

    (* Get the info out of the event, broadcast it
     * to the other members, update my state, and
     * pass it on.
     *)
    let casts = getApplCasts ev in
    let sends = getApplSends ev in

    (* If not coord, send info pt2pt to coord.
     *)
    if ls.rank <> s.coord then (
      let info = Arrayf.singleton (ls.rank, casts, sends) in
      dnlm (sendPeer name s.coord) (General(s.failed, info)) ;
      log (fun () -> sprintf "EFail NOT coord, sending cast/send info to coord")
    ) else (
      log (fun () -> sprintf "EFail AM coord, not sending cast/send info!")
    ) ;

    (* Call receive function for this info.
     *)
    got_info ls.rank s.failed casts sends ;
  in
  
  let up_hdlr ev abv () = up ev abv
  and uplm_hdlr ev hdr = match getType ev, hdr with

  (* Receive vector info in General hdr.
   *)
  | (ESend iovl|ECast iovl), General(failed, info) ->
      (* Ignore messages from myself.
       *)
      if getPeer ev <> ls.rank then (
	Arrayf.iter (fun (origin, casts, sends) ->
	  log (fun () -> sprintf "ECast|ESend - got info from %d" origin);

	  (* Call the receive funtion for this info.
	   *)
	  got_info origin failed casts sends ;
	) info 
      ) ;

      Iovecl.free iovl

  | _ -> failwith unknown_local
	
  and upnm_hdlr ev  = match getType ev with
  | EFail ->
      let failed = getFailures ev in

      (* When my failure information changes, reset all the
       * info.
       *)
      let old_failed = s.failed in
      s.failed <- Arrayf.map2 (||) failed s.failed ;
      if s.failed <> old_failed then (
	s.donesync <- false ;
      	s.heard <- Arrayf.to_array s.failed ;
      ) ;

      (* Recompute the rank of the coordinator.  Hope to do
       * this in elect layer later on.  TC 12/12/97.
       *)
      log (fun () -> sprintf "EFail:recomputing coord");
      s.coord <- Arrayf.min_false s.failed ;

      (* If we are not blocked then the data in the event
       * is no good.  Only continue if we're blocked.
       *)
      if s.blocked then
	got_local ev ;

      upnm ev

  | EBlock ->
      s.blocked <- true ;
      got_local ev ;
      upnm ev

  | EAccount -> 
      logb (fun () -> [
  	sprintf "failed=%s" (Arrayf.bool_to_string s.failed) ;
  	sprintf "heard=%s" (string_of_bool_array s.heard)
      ]) ;
      upnm ev 

  | EDump -> dump vf s ; upnm ev 
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv ()
  and dnnm_hdlr ev = dnnm ev


in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
