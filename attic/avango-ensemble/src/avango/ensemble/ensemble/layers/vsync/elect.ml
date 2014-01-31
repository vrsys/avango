(**************************************************************)
(* ELECT.ML *)
(* Author: Mark Hayden, 10/96 *)
(**************************************************************)
open Trans
open Util
open Layer
open View
open Event
(**************************************************************)
let name = Trace.filel "ELECT"
(**************************************************************)

type header = NoHdr

type state = {
  dn_elect : Once.t ;
  up_elect : Once.t ;
  mutable suspects : bool Arrayf.t
}

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "dn_elect=%b, up_elect=%b, suspects=%s\n" 
    (Once.isset s.dn_elect) (Once.isset s.up_elect) 
    (Arrayf.bool_to_string s.suspects)
|])

let init _ (ls,vs) = {
  dn_elect      = Once.create "dn_elect" ;
  up_elect      = Once.create "up_elect" ;
  suspects 	= ls.falses
}

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | _, NoHdr -> up ev abv
(*| _ -> failwith "bad up event [2]"*)

  and uplm_hdlr ev hdr = match getType ev,hdr with
  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | EFail ->
      let failures = getFailures ev in
      s.suspects <- Arrayf.map2 (||) s.suspects failures ;
      upnm ev

  | EElect ->
      Once.set s.up_elect ;
      upnm ev

    (* EInit: If I'm marked as coord in the view state then
     * I start out as coordinator.
     *)
  | EInit ->
      upnm ev ;
      if ls.rank = vs.coord then (
	Once.set s.dn_elect ;
      	dnnm (create name EElect[])
      )

    (* ESuspect: Some members are suspected to be faulty.
     * Check with the policy module about what to do.
     *)
  | ESuspect ->
      let loc = "ELECT:ESuspect:"^(getSuspectReason ev) in
      let suspicions = getSuspects ev in
      s.suspects <- Arrayf.map2 (||) s.suspects suspicions ;
      assert(not (Arrayf.get suspicions ls.rank)) ;

      if Arrayf.min_false s.suspects >= ls.rank 
      && (not (Once.isset s.dn_elect))
      then (
	log (fun () -> sprintf "reporting:'%s':%s" loc (Arrayf.bool_to_string s.suspects)) ;
	Once.set s.dn_elect ;
	dnnm (create name EElect[])
      ) ;

      let ev = set name ev [
	Suspects s.suspects ;
 	SuspectReason loc
      ] in
      upnm ev

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | _   -> dn ev abv NoHdr

  and dnnm_hdlr ev = match getType ev with
  | EElect -> failwith "got Elect event from above"
  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
