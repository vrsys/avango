(**************************************************************)
(* INTRA.ML *)
(* Author: Mark Hayden, 10/96 *)
(**************************************************************)
open Trans
open Util
open Layer
open Event
open View
(**************************************************************)
let name = Trace.filel "INTRA"
(**************************************************************)

type header =
  | View of View.state
  | Fail of bool Arrayf.t

type state = {
  mutable elected 	: Once.t ;	(* am I the coordinator? *)
  mutable new_view 	: Once.t ;	(* have I done a EView? *)
  mutable failed 	: bool Arrayf.t	(* failed members *)
}

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "new_view=%b elected=%b failed=%s\n" 
    (Once.isset s.new_view) (Once.isset s.elected)
    (Arrayf.bool_to_string s.failed)
|])

let init _ (ls,vs) = {
  elected   	= Once.create "elected" ;
  new_view      = Once.create "new_view" ;
  failed    	= ls.falses
}

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
(*let ack = make_acker name dnnm in*)
  let log = Trace.log2 name ls.name in

  let up_hdlr ev abv () = up ev abv
  and uplm_hdlr ev hdr = match getType ev,hdr with

  (* New view arrived.  If not from me, check it out.  If
   * accepted, bounce off bottom.  
   *)
  | ECast iov, View(new_vs) -> (
      let origin = getPeer ev in
      if origin <> ls.rank then (
	if Once.isset s.elected		(* I'm coordinator *)
        || Once.isset s.new_view	(* I've already accepted a view *)
	|| ls.rank < origin		(* my rank is lower *)
	|| Arrayf.get s.failed origin	(* coord is failed *)
	|| not (Arrayf.mem ls.endpt new_vs.view) (* I am not in the new view *)
  (*    || subtract remaining view' <> [] (* some members are missing *)*)
	then (
	  log (fun () -> sprintf "View:rejected:%s" (View.to_string new_vs.view)) ;
	  dnnm (suspectReason name (Arrayf.of_ranks ls.nmembers [origin]) name) ;
	) else (
	  log (fun () -> sprintf "View:accepted:%s" (View.to_string new_vs.view)) ;
	  Once.set s.new_view ;
	  dnnm (create name EView[ViewState new_vs])
	)
      ) ;
      Iovecl.free iov
    )

  (* New failure announced.  If not from me, check it out.
   * If accepted, bounce off bottom. 
   *)
  | ECast iov, Fail(failures) ->
      let origin = getPeer ev in
      if origin <> ls.rank then (
	if Once.isset s.elected		(* I've been elected *)
	|| ls.rank < origin		(* my rank is lower *)
	|| Arrayf.min_false failures < origin (* failures don't include all lower ranked members *)
	|| not (Arrayf.super failures s.failed) (* he doesn't include failures I've seen *)
	|| Arrayf.get s.failed origin	(* coord is failed *)
	|| Arrayf.get failures origin	(* he is failing himself *)
	|| Arrayf.get failures ls.rank	(* I am being failed *)
	then (
	  log (fun () -> sprintf "Fail:rejected:%s" (Arrayf.bool_to_string failures)) ;
	  dnnm (suspectReason name (Arrayf.of_ranks ls.nmembers [origin]) name) ;
	) else (
	  log (fun () -> sprintf "Fail:accepted:%s" (Arrayf.bool_to_string failures)) ;
	  s.failed <- failures ;
	  dnnm (create name EFail[(Failures failures)])
	)
      ) ;
      Iovecl.free iov

  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  (* EElect: I've been elected.
   *)
  | EElect ->
      Once.set s.elected ;
      upnm ev

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv ()

  and dnnm_hdlr ev = match getType ev with
  (* EView: send out view and bounce locally.
   *)
  | EView ->
      let vs = getViewState ev in
      if Once.isset s.new_view then (
	log (fun () -> "dropping EView because view is already accepted") ;
	free name ev
      ) else (
      	Once.set s.new_view ;
      	log (fun () -> sprintf "EView:%s" (View.to_string vs.view)) ;
      	assert (Once.isset s.elected) ;
      	dnlm (castEv name) (View vs) ;
      	dnnm ev
      )

  (* EFail: send out failures and bounce locally.
   *)
  | EFail ->
      let failures = getFailures ev in
      log (fun () -> sprintf "EFail:%s" (Arrayf.bool_to_string failures)) ;
      assert (Once.isset s.elected) ;
      s.failed <- failures ;
      dnlm (castEv name) (Fail(s.failed)) ;
      dnnm ev

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
