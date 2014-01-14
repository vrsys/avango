(**************************************************************)
(* SLANDER.ML *)
(* Author: Zhen Xiao, Mark Hayden, 02/97 *)
(**************************************************************)
open Trans
open Util
open Layer
open Event
open View
(**************************************************************)
let name = Trace.filel "SLANDER"
(**************************************************************)

type header = Slander of bool Arrayf.t

type state = {
  mutable suspects : bool Arrayf.t
}

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "suspects=%s\n" (Arrayf.bool_to_string s.suspects)
|])

let init _ (ls,vs) = {
  suspects = ls.falses
}

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
(*let ack = make_acker name dnnm in*)
  let log = Trace.log2 name ls.name in

  let up_hdlr ev abv () = up ev abv

  and uplm_hdlr ev hdr = match getType ev,hdr with
  (* Got slander message from another member.
   *)    
  | (ECast iovl|ESend iovl), Slander(suspects) ->
      let origin = getPeer ev in
      log (fun () -> sprintf "got Slander(%s) from %d" 
        (Arrayf.bool_to_string suspects) origin) ;

      (* If it cast slander on me and I haven't suspected it yet,
       * then suspect it and cast slander on it.
       *) 
      if Arrayf.get suspects ls.rank then (
	if not (Arrayf.get s.suspects origin) then (
          s.suspects <- Arrayf.fset s.suspects origin true ;
          dnnm (suspectReason name s.suspects name) ;
          dnlm (castEv name) (Slander s.suspects)
	)
      ) else (
      (* If it doesn't cast slander on me and there are some
       * new members in its suspicion list, then suspect those new 
       * members. But there is no need to relay this slander message.
       *)
        if not (Arrayf.super s.suspects suspects) then (
          s.suspects <- Arrayf.map2 (||) s.suspects suspects ;
          dnnm (suspectReason name s.suspects name) ;
        )
      ) ;
      Iovecl.free iovl

  | _ -> failwith unknown_local
   
  (* Got suspicion event.
   *)
  and upnm_hdlr ev  = match getType ev  with
  | ESuspect ->
      let suspicions = getSuspects ev in
      (* If there are new suspicious members, slander
       * those members.
       *)
      if not (Arrayf.super s.suspects suspicions) then (
        s.suspects <- Arrayf.map2 (||) s.suspects suspicions ;  
        dnlm (castEv name) (Slander s.suspects) ;
	log (fun () -> sprintf "cast Slander on %s"
	  (Arrayf.bool_to_string s.suspects)) ;
      ) ;
      upnm ev

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv ()

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
