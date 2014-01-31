(**************************************************************)
(* PR_SUSPECT.ML *)
(* Author: Mark Hayden, 11/96 *)
(**************************************************************)
open Trans
open Layer
open Event
open Util
open View
open Arrayop
(**************************************************************)
let name = Trace.filel "PR_SUSPECT"
(**************************************************************)

type header = NoHdr 
  | Gossip of bool Arrayf.t * int Arrayf.t

type state = {
  sweep		: Time.t ;
  max_idle	: int ;
  fanout        : int ;
  mutable gossip : rank Arrayf.t ;
  mutable failed : bool Arrayf.t ;
  idle		: int array ;
  mutable gossiped : bool ;
  mutable next_gossip : Time.t
}

(**************************************************************)

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "nmembers=%d, rank=%d\n" ls.nmembers ls.rank ;
  sprintf "failed   =%s\n" (Arrayf.bool_to_string s.failed) ;
  sprintf "idle     =%s\n" (string_of_int_array s.idle)
|])

(**************************************************************)

let init _ (ls,vs) = {
  max_idle    = Param.int vs.params "pr_suspect_max_idle" ;
  sweep       = Param.time vs.params "pr_suspect_sweep" ;
  fanout      = Param.int vs.params "pr_suspect_fanout" ;

  failed      = ls.falses ;
  idle 	      = Array.create ls.nmembers 0 ;
  gossiped    = true ;
  gossip      = Arrayf.gossip ls.falses ls.rank ;
  next_gossip = Time.invalid
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in

  let do_gossip () =
    let dests = Arrayf.choose s.gossip s.fanout in
    let idle = Arrayf.of_array s.idle in
    List.iter (fun dest ->
      dnlm (sendUnrelPeer name dest) (Gossip(s.failed,idle))
    ) dests
  in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | _, NoHdr -> up ev abv
  | _, _     -> failwith bad_up_event

  and uplm_hdlr ev hdr = match getType ev,hdr with
    (* Gossip Message.
     *)
  | (ECast iovl|ESend iovl|ECastUnrel iovl|ESendUnrel iovl), Gossip(failed,idle) ->
      let origin = (getPeer ev) in
      if (not (Arrayf.get failed ls.rank)) (* BUG: could auto-fail him *)
      && (not (Arrayf.get s.failed origin))
      then (
	minify s.idle (Arrayf.to_array idle) ;
	if s.idle.(ls.rank) <> 0 then
	  failwith sanity ;
	if not s.gossiped then (
	  s.gossiped <- true ;
	  do_gossip ()
	)
      ) ;
      Iovecl.free iovl

  | _ -> failwith unknown_local


  and upnm_hdlr ev = match getType ev with

    (* EFail: mark the failed members and check if any
     * messages are now suspect.
     *)
  | EFail ->
      s.failed <- getFailures ev ;
      s.gossip <- Arrayf.gossip s.failed ls.rank ;
      upnm ev

    (* EInit: request a first timer alarm to bootstrap things.
     *)
  | EInit ->
      dnnm (timerAlarm name Time.zero) ;
      upnm ev

    (* ETimer: every so often:
     *   1. check for suspicions
     *   2. ping the other members
     *)
  | ETimer ->
      let time = getTime ev in
      if time >= s.next_gossip then (
	s.next_gossip <- Time.add time s.sweep ;
	dnnm (timerAlarm name s.next_gossip) ; (* request next gossip *)

	(* Gossip, if haven't done so already.
	 *)
	if not s.gossiped then (
	  s.gossiped <- true ;
	  do_gossip ()
	) ;
	s.gossiped <- false ;

	(* Increment idle counters and check threshhold.
	 *)
	let suspicions = Array.create ls.nmembers false in
	let any = ref false in
	for i = 0 to pred ls.nmembers do
	  if i <> ls.rank 
	  && not (Arrayf.get s.failed i) 
	  then (
	    s.idle.(i) <- succ s.idle.(i) ;
	    if s.idle.(i) > s.max_idle then (
	      suspicions.(i) <- true ;
	      any := true
	    )
	  )
	done ;

	(* Announce suspicions.
	 *)
	if !any then
	  dnnm (suspectReason name (Arrayf.of_array suspicions) "PR_SUSPECT:timeout") ;

	(* If rank 0, start the gossip cascade.
	 *)
	if ls.rank = 0 then (
	  s.gossiped <- true ;
	  do_gossip ()
        )
      ) ;
      upnm ev

  | EDump -> dump vf s ; upnm ev
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ =
  Param.default "pr_suspect_sweep" (Param.Time (Time.of_string "0.5")) ;
  Param.default "pr_suspect_max_idle" (Param.Int 15) ;
  Param.default "pr_suspect_fanout" (Param.Int 3) ;
  Layer.install name l

(**************************************************************)
