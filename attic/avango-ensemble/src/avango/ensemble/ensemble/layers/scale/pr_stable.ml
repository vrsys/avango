(**************************************************************)
(* PR_STABLE.ML : stability detection protocol *)
(* Author: Mark Hayden, 11/96 *)

(* Note: Sequence numbers begin at 0 in each epoch.
 * Acknowlegdements give the number of messages acknowledged
 * so far in the epoch.  Thus the first messge is
 * acknowledged with '1'.  Similarly, the stability numbers
 * correspond to the number of messages that are stable in
 * the current epoch. *)

(* Note: Protocol can quiet itself by not starting cascades
 * at beginning or end of a round.  *)

(**************************************************************)
open Trans
open Layer
open View
open Event
open Util
open Arrayop
(**************************************************************)
let name = Trace.filel "PR_STABLE"
(**************************************************************)

type gossip = {
  g_round   : seqno ;			(* Current round *)
  g_failed  : bool Arrayf.t ;		(* Who I think is failed *)
  g_repr    : bool Arrayf.t ;		(* Who I've heard from *)
  g_mins    : seqno Arrayf.t ;		(* Current approximated mins *)
  g_maxs    : seqno Arrayf.t ;		(* Current approximated maxs *)
  g_st_mins : seqno Arrayf.t ;		(* Previous minimums *)
  g_st_maxs : seqno Arrayf.t		(* Previous maximums *)
} 

type header = NoHdr | Gossip of gossip

type state = {
  sweep		: Time.t ;
  explicitack	: bool ;
(*fanout        : fanout ;*)
  mutable next_gossip : Time.t ;
  mutable gossip : rank Arrayf.t ;

  mutable cast    : seqno Arrayf.t ;
  mutable round   : seqno ;
  mutable failed  : bool Arrayf.t ;
  mutable repr    : bool Arrayf.t ;
  mutable maxs    : seqno Arrayf.t ;
  mutable mins    : seqno Arrayf.t ;
  mutable st_mins : seqno Arrayf.t ;
  mutable st_maxs : seqno Arrayf.t

  (* Fields used to keep track of tree-based gossiping.
   *)
(* ;
  mutable gossiped : bool ;
  optimize      : bool ;
  parent        : rank option ;
  mutable parented : bool ;
  children      : rank array ;
  descend       : rank list
*)
}

let repr nmembers rank = 
  Arrayf.init nmembers (fun r -> r = rank)

(**************************************************************)

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "rank=%d, round=%d\n" ls.rank s.round ;
  sprintf "failed   =%s\n" (Arrayf.bool_to_string s.failed) ;
  sprintf "cast     =%s\n" (Arrayf.int_to_string s.cast) ;
  sprintf "repr     =%s\n" (Arrayf.bool_to_string s.repr) ;
  sprintf "mins     =%s\n" (Arrayf.int_to_string s.mins) ;
  sprintf "maxs     =%s\n" (Arrayf.int_to_string s.maxs) ;
  sprintf "st_mins  =%s\n" (Arrayf.int_to_string s.st_mins) ;
  sprintf "st_maxs  =%s\n" (Arrayf.int_to_string s.st_maxs)
|])

(**************************************************************)

let child1 r = pred ((succ r) * 2)
let child2 r = succ (child1 r)

let init _ (ls,vs) = 
  let parent = if ls.rank = 0 then None else Some (pred ((succ ls.rank) / 2)) in
  let children =
    Array.append
      (if child1 ls.rank < ls.nmembers then [|child1 ls.rank|] else [||])
      (if child2 ls.rank < ls.nmembers then [|child2 ls.rank|] else [||])
  in
  let descend = 
    let rec loop rank =
      if rank >= ls.nmembers then []
      else rank :: ((loop (child1 rank)) @ loop (child2 rank))
    in loop ls.rank
  in
  let array v = Array.create ls.nmembers v in
  {
    sweep	= Param.time vs.params "pr_stable_sweep" ;
    explicitack	= Param.bool vs.params "pr_stable_explicit_ack" ;
(*  fanout      = Param.int vs.params "pr_stable_fanout" ;*)

    next_gossip = Time.invalid ;
    
    cast	= ls.zeroes ;

    round       = 0 ;
    failed      = ls.falses ;
    gossip      = Arrayf.gossip ls.falses ls.rank ;
    
    repr        = repr ls.nmembers ls.rank ;
    st_mins  	= ls.zeroes ;
    st_maxs  	= ls.zeroes ;
    mins	= ls.zeroes ;
    maxs  	= ls.zeroes

(*;    
    gossiped    = true ;
    optimize    = false (*true*) ;
    parent      = parent ;
    parented    = true ;
    children    = children ;
    descend     = descend
*)
  }

(**************************************************************)

let ready repr = Arrayf.for_all ident repr

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let log = Trace.log2 name ls.name in
  let failwith = layer_fail dump vf s name in

  let do_gossip () =
    let dests = 
      Array.of_list (Arrayf.choose s.gossip 1(*s.fanout*))
    in
(*
      let dests = 
	if s.optimize then (
	  let children = 
	    if down then s.children else [||]
	  in
	  let parent =
	    if s.parent <> None
	    && (not s.parented)
	    && List.for_all (fun rank -> s.repr.(rank)) s.descend 
	    then (
	      s.parented <- true ;
	      [|some_of s.parent|]
	    ) else [||]
	  in
	  Array.concat [dests;parent;children]
        ) else dests
      in
*)

    if dests <> [||] then (
      let hdr = Gossip {
	g_round = s.round ;
	g_failed = s.failed ;
	g_repr = s.repr ;
	g_mins = s.mins ;
	g_maxs = s.maxs ;
	g_st_mins = s.st_mins ;
	g_st_maxs = s.st_maxs
      } in

      Array.iter (fun dest ->
      	dnlm (sendUnrelPeer name dest) hdr
      ) dests
    )
  in

  let deliver_stable () =
    (* Send down EStable event.
     *)
    dnnm (create name EStable[
      Stability s.st_mins ;
      NumCasts s.st_maxs
    ])
  in
  
  let check_stable () =
    (* Max in my recd.  Always safe to do (and necessary).
     *)
    s.maxs <- ffmaxify s.maxs s.cast ;

    (* Check if we have full representation for the group.
     *)
    if Arrayf.for_all ident s.repr then (
      s.round <- succ s.round ;
      s.repr <- ffmergify (repr ls.nmembers ls.rank) s.failed ;

      s.st_mins <- s.mins ;
      s.st_maxs <- s.maxs ;
      
      s.mins <- s.cast ; 
      s.maxs <- s.cast ;
      deliver_stable ()
    )
  in

  let up_hdlr ev abv () = up ev abv

  and uplm_hdlr ev hdr = match getType ev,hdr with

    (* Gossip Message: if from a live member, copy into
     * the origins row in my acknowledgement matrix.
     *)
  | (ECast iovl|ESend iovl|ESendUnrel iovl|ECastUnrel iovl), Gossip(g) ->
      let origin = getPeer ev in

      if (not (Arrayf.get g.g_failed ls.rank)) (* he doesn't think I'm failed *)
      && (not (Arrayf.get s.failed origin))    (* I don't think he's failed *)
      then (
      	s.maxs <- ffmaxify s.maxs g.g_maxs ;
	
	begin (* TODO: need to cleanup *)
	  let tmp = Arrayf.to_array s.st_maxs in
	  fmaxify tmp g.g_maxs ;
	  s.st_maxs <- Arrayf.of_array tmp
	end ;

	if s.failed = g.g_failed then (
	  (* If he is from an advanced round, then advance my info.
	   *)
	  if s.round < g.g_round then (
	    s.round <- g.g_round ;
	    s.repr <- ffmergify (repr ls.nmembers ls.rank) s.failed ;
	    
	    s.st_mins <- g.g_st_mins ;
	    s.st_maxs <- g.g_st_maxs ;
	    
	    s.mins <- s.cast ; 
	    s.maxs <- s.cast ;
    
	    deliver_stable ()
	  ) ;

	  (* Merge mins, maxs, & repr.
	   *)
	  if s.round = g.g_round then (	(* he is in my round or later *)
	    s.repr <- ffmergify s.repr g.g_repr ;
      	    s.maxs <- ffmaxify s.maxs g.g_maxs ;
	    s.mins <- ffminify s.mins g.g_mins ;
	    check_stable () ;
	  ) ;
	)
      ) ;
      Iovecl.free iovl

  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with

    (* EInit: request a first timer alarm to bootstrap things.
     *)
  | EInit ->
      dnnm (timerAlarm name Time.zero) ;
      upnm ev

    (* EFail: mark the failed members and check if any
     * messages are now stable.
     *)
  | EFail ->
      s.failed <- getFailures ev ;
      s.gossip <- Arrayf.gossip s.failed ls.rank ;
      s.repr <- ffmergify s.repr s.failed ;
      check_stable () ;
      upnm ev

    (* ETimer: every so often:
     *   1. recalculate stability and deliver EStable event
     *   2. gossip about my row in the stability matrix
     *)
  | ETimer ->
      let time = getTime ev in
      if time >= s.next_gossip then (
	s.next_gossip <- Time.add time s.sweep ;
	dnnm (timerAlarm name s.next_gossip) ; (* request next gossip *)

	(* Request info from the MNAK layer.
	 *)
        dnnm (create name EStableReq[]) ;

	(* Pass down stability info for Mnak layer.  This is
	 * done here in case the maxs have increased.
	 *)
	check_stable () ;
	deliver_stable ()
      ) ;
      upnm ev

    (* Got reply to my stability request.
     *)
  | EStableReq ->
      let casts = getNumCasts ev in
      s.cast <- casts ;
      do_gossip () ;
      upnm ev

(*| EAccount*)
  | EDump -> dump vf s ; upnm ev
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv ()
  and dnnm_hdlr ev = dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = 
  Param.default "pr_stable_sweep" (Param.Time (Time.of_string "0.3")) ;
  Param.default "pr_stable_explicit_ack" (Param.Bool false) ;
(*Param.default "pr_stable_fanout" (Param.Int 3) ;*)
  Layer.install name l

(**************************************************************)
