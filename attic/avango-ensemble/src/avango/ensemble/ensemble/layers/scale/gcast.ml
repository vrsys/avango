(**************************************************************)
(* GCAST.ML *)
(* Author: Mark Hayden, 12/96 *)
(* BUG: refcounts are not done correctly here *)
(**************************************************************)
open Trans
open Layer
open Event
open Util
open View
(**************************************************************)
let name = Trace.filel "GCAST"
let failwith = Trace.make_failwith name
(**************************************************************)

type header = NoHdr 
  | Cast of rank

type state = {
  fanout : int ;
  mutable failed : bool Arrayf.t ;
  mutable forward : rank Arrayf.t Arrayf.t
}

(**************************************************************)

(* Fix O'caml mod function to return non-negative values.
 *)
let (mod) a b = 
  let m = a mod b in
  if m >=0 then m else (
    assert (m + b >= 0) ;
    m + b
  )

(**************************************************************)

(* Calculate vector of children for forwarding messages.
 *)
let children nmembers fanout rank failed =
  let is_failed shift rank =
    Arrayf.get failed ((rank - shift) mod nmembers)
  in
  let fan = Util.sequence fanout in
  let fan = Arrayf.of_array fan in
  let rec children shift rank =
    let c = (rank - shift) mod nmembers in
    let c = succ (c * fanout) in
    let c = Arrayf.map ((+) c) fan in
    let c = Arrayf.filter (fun i -> i < nmembers) c in
    let c = Arrayf.map (fun i -> (i + shift) mod nmembers) c in
    let c = 
      Arrayf.map (fun i ->
      	if Arrayf.get failed i then
	  children shift i
      	else Arrayf.singleton i
      ) c
    in
    Arrayf.concat (Arrayf.to_list c)
  in

  Arrayf.init nmembers (fun i -> children i rank)

(* DEBUGGING VERSION
let children nmembers fanout root failed =
  if failed.(root) then failwith "root failed" ;
  let mark = Array.copy failed in
  let rec dft r = 
    if mark.(r) then failwith "marked twice" ;
    if failed.(r) then failwith "send to failed member" ;
    mark.(r) <- true ;
    let ch = children nmembers fanout r failed in
    let ch = ch.(root) in
    List.iter dft ch
  in
  dft root ;

  let nfailed = ref 0 in
  Array.iter (fun b -> if b then incr nfailed) failed ;
  eprintf "GCAST:checking tree: nmembers=%d, root=%d, nfailed=%d\n"
    nmembers root !nfailed ;
  if array_exists (fun _ b -> not b) mark then
    failwith "not all marked" ;
  
  children nmembers fanout root failed
*)
    
(**************************************************************)

let init _ (ls,vs) =
  let fanout = Param.int vs.params "gcast_fanout" in
  { fanout = fanout ;
    failed = ls.falses ;
    forward = children ls.nmembers fanout ls.rank ls.falses
  }

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let log = Trace.log2 name ls.name in
  let up_hdlr ev abv hdr = match getType ev, hdr with
  | (ESend iovl|ESendUnrel iovl), (Cast(origin) as hdr) ->
      if not (Arrayf.get s.failed origin) then (
	let dests = Arrayf.get s.forward origin in
	log (fun () -> sprintf "%d ... -> %d -> %d -> %s" 
	  origin (getPeer ev) ls.rank (Arrayf.int_to_string dests)) ;
	Arrayf.iter (fun dest ->
	  let iovl = Iovecl.copy iovl in
	  dn (sendPeerIov name dest iovl) abv hdr ;
	) dests ;
	let iovl = Iovecl.copy iovl in
	up (castPeerIov name origin iovl) abv
      ) ;
      Iovecl.free iovl

  | _, NoHdr -> up ev abv
  | _, _     -> failwith bad_up_event

  and uplm_hdlr ev hdr = failwith unknown_local
  and upnm_hdlr ev = match getType ev with

  | EFail -> 
      (* Mark failed members.
       *)
      s.failed <- getFailures ev ;

      (* Recalculate forwarding info.
       *)
      s.forward <- children ls.nmembers s.fanout ls.rank s.failed ;
      upnm ev

  | _ -> upnm ev
  
  and dn_hdlr ev abv = match getType ev with
  | (ECast iovl|ECastUnrel iovl) ->
      let dests = Arrayf.get s.forward ls.rank in
      log (fun () -> sprintf "root:%d -> %s" 
	ls.rank (Arrayf.int_to_string dests)) ;
      Arrayf.iter (fun dest ->
	let iovl = Iovecl.copy iovl in
      	dn (sendPeerIov name dest iovl) abv (Cast ls.rank)
      ) dests ;
      Iovecl.free iovl

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

(**************************************************************)

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = 
  Param.default "gcast_fanout" (Param.Int 2) ;
  Layer.install name l
    
(**************************************************************)
