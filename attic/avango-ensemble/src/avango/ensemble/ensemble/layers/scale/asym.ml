(**************************************************************)
(* ASYM.ML *)
(* Author: Mark Hayden, 7/99 *)
(**************************************************************)
open Trans
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "ASYM"
let failwith = Trace.make_failwith name
(**************************************************************)

type header = NoHdr 
  | Direct of int(*src_time*)
  | Route of rank(*src*) * rank(*dest*) * int(*ttl*)
  | Gossip of rank Arrayf.t(*dest0*) * seqno Arrayf.t(*time0*) * bool

type state = {
  sweep		: Time.t ;
  mutable next_gossip : Time.t ;
  mutable last : seqno ;
  mutable failed : bool Arrayf.t ;

  mutable send_time : seqno array ;
  mutable send_dest : rank array ;

  mutable dist : int array ;
  mutable forward : rank array ;
  mutable time1 : int array ;

  mutable gossip : rank Arrayf.t ;
  mutable routed : bool array
}

(**************************************************************)

let init _ (ls,vs) = { 
    sweep = Time.of_float 0.2 ;
    next_gossip = Time.of_int 0 ;
    failed = ls.falses ;
    last = 0 ;
    forward = Array.init ls.nmembers ident ;
    dist = Array.create ls.nmembers 0 ;
    time1 = Array.create ls.nmembers 0 ;

    send_time = Array.create ls.nmembers 0 ;
    send_dest = Array.init ls.nmembers (fun i -> (succ i) mod ls.nmembers) ;
    gossip = Arrayf.gossip ls.falses ls.rank ;
    routed = Array.create ls.nmembers true
  }

(**************************************************************)

let rec calc_dist init me a =
  let rec loop i n =
    if n >= Arrayf.length a then (
      eprintf "init=%d me=%d a=%s[2]\n" init me (Arrayf.int_to_string a) ;
      exit 1 ;
    ) ;
    assert (n < Arrayf.length a) ;
    if i = me then 
      n
    else (
      loop (Arrayf.get a i) (succ n)
    )
  in loop init 0

let rec calc_gorp init me a =
  let rec loop i n =
    if n >= Arrayf.length a then (
      eprintf "init=%d me=%d a=%s[1]\n" init me (Arrayf.int_to_string a) ;
      exit 1 ;
    ) ;
    assert (n < Arrayf.length a) ;
    if i = init then (
      n
    ) else if i = me then (
      (-1)
    ) else (
      loop (Arrayf.get a i) (succ n)
    )
  in loop (Arrayf.get a me) 1

let rec check_root a init me =
  let rec loop i n =
    if n = 0 then (
      eprintf "init=%d me=%d a=%s[4]\n" init me (string_of_int_array a) ;
      exit 1 ;
    ) ;
    assert (n <> 0) ;
    if i = me then 
      ()
    else (
      loop a.(i) (pred n) 
    )
  in loop init (succ (Array.length a))

let check ls s =
  for i = 0 to pred ls.nmembers do
(*
    eprintf "dest0=%s\n" (string_of_int_array s.dest0) ;
*)
    if i <> ls.rank then (
      assert (s.send_dest.(i) <> i) ;
      assert (s.forward.(i) <> ls.rank) ;
      assert (s.dist.(i) >= 0) ;
      assert (s.send_time.(i) >= 0) ;
      check_root s.send_dest i ls.rank
    )
  done

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let log = Trace.log2 name ls.name in
  (*let connected = Elink.get name Elink.partition_connected in*)

  check ls s ;

  let make_gossip info =
    s.dist.(ls.rank) <- 0 ;
    s.send_time.(ls.rank) <- s.last ;
    (* send_dest is set at dest.
     *)
    let dest0 = Arrayf.of_array s.send_dest in
    let time0 = Arrayf.of_array s.send_time in
    Gossip(dest0, time0, info)
  in

  let do_gossip rank info =
    assert (rank <> ls.rank) ;
    assert (s.send_dest.(rank) <> rank) ;
    s.dist.(ls.rank) <- 0 ;
    let gossip = make_gossip info in
    dnlm (sendUnrelPeer name rank) gossip
  in

  let up_hdlr ev abv hdr = match getType ev, hdr with
  | (ESend iovl|ESendUnrel iovl), Route(src,dst,ttl) ->
      let fwd = s.forward.(dst) in
      let peer = getPeer ev in
      if Arrayf.get s.failed src 
      || Arrayf.get s.failed dst
      then (
	log (fun () -> sprintf "%d->..->%d->%d:source/dest failed" src peer ls.rank) ;
      ) else if src = ls.rank then (
	log (fun () -> sprintf "%d->..->%d->%d:failure:cycle:sender" src peer ls.rank) ;
      ) else if dst = ls.rank then (
	log (fun () -> sprintf "%d->..->%d->%d:routing succeeded (TTL=%d)" src peer ls.rank ttl) ;
	up (setPeer name ev src) abv
      ) else if ttl = 0 then (
	log (fun () -> sprintf "%d->..->%d->%d:failure:TTL" src peer ls.rank) ;
      ) else if fwd = src || fwd = peer then (
	(* Don't forward back to original sender or
	 * to who sent it to me.
	 *)
	do_gossip fwd true ;
	log (fun () -> sprintf "%d->..->%d->%d->%d->...->%d:failure:cycle:peer" src peer ls.rank fwd dst) ;
      ) else (
	dn (setSendUnrelPeer name ev fwd) abv (Route(src,dst,pred ttl))
      ) ;

      Iovecl.free iovl

  | (ESend _|ESendUnrel _), Direct(src_time) ->
      let origin = getPeer ev in
(*
      merge "direct" origin ls.rank ls.rank s.last src_time ;
*)
      up ev abv

  | _, NoHdr -> up ev abv
  | _, _     -> failwith bad_up_event

  and uplm_hdlr ev hdr = match getType ev, hdr with
  | (ESend iovl|ESendUnrel iovl|ECast iovl|ECastUnrel iovl),
    Gossip(send_dest,time0,reply) ->
      let origin = getPeer ev in
      let send_dest = Arrayf.fset send_dest origin ls.rank in
      let forward = Arrayf.get send_dest ls.rank in
      assert (forward <> ls.rank) ;
      assert (Arrayf.get send_dest origin <> origin) ;

      begin
	let time0 = Arrayf.get time0 ls.rank in
	for rank = 0 to pred ls.nmembers do
	  if rank <> ls.rank 
	  && not (Arrayf.get s.failed rank)
	  then (
	    assert (Arrayf.get send_dest rank <> rank) ;
	    assert (rank <> ls.rank) ;
	    let time1 = s.time1.(rank) in
	    if time0 >= time1 then (
	      let dist0 = calc_gorp rank ls.rank send_dest in
	      if dist0 >= 0 then (
		s.time1.(rank) <- time0 ;
		s.forward.(rank) <- forward ;
		s.dist.(rank) <- dist0 ;
	      )
	    )
	  )
	done ;
      end ;
      check ls s ;

      begin
	let prev = Arrayf.of_array s.send_dest in
	for i = 0 to pred ls.nmembers do
(*
	  check_root s.send_dest i ls.rank ;(*PERF*)
	  let d0 = calc_dist i ls.rank prev in
	  let d1 = calc_dist i ls.rank send_dest in
*)

	  let t0 = s.send_time.(i) in
	  let t1 = Arrayf.get time0 i in
	  (* Also fill everything forward from here so that
	   * we still have a tree.
	   *)
	  if t1 >= t0 then (
	    let rec loop i =
	      if i <> ls.rank then (
		let dest = Arrayf.get send_dest i in
		s.send_time.(i) <- Arrayf.get time0 i ;
		s.send_dest.(i) <- dest ;
		loop dest
	      ) 
	    in loop i
	  )
	done
      end ;

      if reply then (
	do_gossip origin false
      ) ;
      
      Iovecl.free iovl

  | _ -> failwith unknown_local
  and upnm_hdlr ev = match getType ev with
  | EFail -> 
      (* Mark failed members.
       *)
      s.failed <- getFailures ev ;
      s.gossip <- Arrayf.gossip s.failed ls.rank ;

      (* Should be ok to do nothing else because
       * failed members will already be gone.
       *)
      upnm ev

    (* EInit: request a first timer alarm to bootstrap things.
     *)
  | EInit ->
      dnnm (timerAlarm name Time.zero) ;
      upnm ev

    (* ETimer: every so often ping the other members.
     *)
  | ETimer ->
      let time = getTime ev in
      if time >= s.next_gossip then (
	s.next_gossip <- Time.add time s.sweep ;
	dnnm (timerAlarm name s.next_gossip) ; (* request next gossip *)

	s.last <- succ s.last ;
	begin
	  match Arrayf.choose s.gossip 1 with 
	  | [] -> ()
	  | [dest] ->
	      assert (dest <> ls.rank) ;
	      s.routed.(dest) <- true
	  | _ -> assert false
	end ;

	let gossip = make_gossip true in
	for rank = 0 to pred ls.nmembers do
	  if rank <> ls.rank 
	  && s.routed.(rank) 
	  then (
	    s.routed.(rank) <- false ;
	    dnlm (sendUnrelPeer name rank) gossip
	  )
	done

      ) ;
(*
      log (fun () -> sprintf "   dist=%s" (string_of_int_array s.dist)) ;
      log (fun () -> sprintf "forward=%s" (string_of_int_array s.forward)) ;
*)
      upnm ev

  | _ -> upnm ev
  
  and dn_hdlr ev abv = match getType ev with
  | (ESend _|ESendUnrel _) ->
      let dest = getPeer ev in
      assert (dest <> ls.rank) ;
      let forward = s.forward.(dest) in
      assert (forward <> ls.rank) ;
      if forward = dest then (
	dn ev abv (*NoHdr*)(Direct s.last)
      ) else (
	(* The TTL needs to be set to be large enough for longest
	 * chain of endpoints.  For 2 endpoint group a TTL of 0
	 * is sufficient.
	 *)
	s.routed.(dest) <- true ;
	dn (setSendUnrelPeer name ev forward) abv (Route(ls.rank,dest,ls.nmembers-2))
(*
	dn (setPeer name ev forward) abv (Route(ls.rank,dest,ls.nmembers-2))
*)
      )
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

(**************************************************************)

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = 
  Layer.install name l
    
(**************************************************************)
