(**************************************************************)
(* CHK_CAUSAL.ML : Checking causal ordering properties *)
(* Author: Ohad Rodeh, 12/96 *)
(* Last update: 7/97 *)
(**************************************************************)
(* 
 * General description
 * -------------------

 * This layer checks the causal order.  It can work with or
 * without self delivery. It ignores messages recieved which
 * are self-delivered.

 * It performs an Independent check via the addition
 * acknowledgment vector to each message ECast.  Upon
 * receite, these headers are checked for consistency.

 * The Sequencer+Tops adher guarantie causal order. 
 *)
(**************************************************************)
open Layer
open Trans
open Util
open Event
open View
(**************************************************************)
let name = Trace.filel "CHK_CAUSAL"
(**************************************************************)


type header = NoHdr | Cast of seqno Arrayf.t

type state = {
  acks : seqno array			(* locally generated ack vct's *)
} 

(**************************************************************)

let init name (ls,vs) = 
  let n = ls.nmembers in 
  { acks = Array.create n 0 } 

(**************************************************************)

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "rank=%d\n" ls.rank ;
  sprintf "up_cast=%s\n" (string_of_int_array s.acks)
|])

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let stop_exec m = dnnm (create name EDump[]); eprintf "%s\n" (name^":"^m) in
  let log = Trace.log2 name ls.name in

  let check_acks ack_vct org = 
    let l = ref [] in 
    Array.iteri (fun i v -> 
      if s.acks.(i) < v
	&& i <> org 
      then l := (i,v) :: !l;
      ) ack_vct;
    !l
  in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | ECast _, (Cast ack_vct) -> 
      let ack_vct = Arrayf.to_array ack_vct in
      let org = getPeer ev in
      if org <> ls.rank then (
	  s.acks.(org) <- succ s.acks.(org);

	  let l = check_acks ack_vct org in 

	  if List.length l <> 0 then (
	    eprintf "<CHK_CAUSAL:delivering (r=%d,s=%d) without predecessor" 
	    org s.acks.(org);
	    List.iter (fun (r,s) -> printf "(r=%d,s=%d)" r s) l;
	    eprintf ">\n";
            stop_exec sanity
	  )
      ) ;
      up ev abv

  | _, NoHdr -> up ev abv
  | _, _     -> failwith bad_up_event
  and uplm_hdlr _ _ = failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | EDump -> (dump vf s; upnm ev)
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | ECast _ when not (getNoTotal ev) -> 
      s.acks.(ls.rank) <- succ s.acks.(ls.rank);
      dn ev abv (Cast(Arrayf.of_array s.acks));
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
