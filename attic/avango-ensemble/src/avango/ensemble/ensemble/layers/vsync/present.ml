(**************************************************************)
(* PRESENT.ML *)
(* Author: Mark Hayden, Zhen Xiao, 9/97 *)
(* Note: this layer generates an EPresent event (which contains
 * a bool array) whenever a member reports its presence in the 
 * view. This could be quite inefficient if the group size is 
 * large.  We can either reduce the frequency of generating 
 * such events or not use an array.
 *)
(**************************************************************)
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "PRESENT"
(**************************************************************)

type header = InView

type state = {
  in_view : bool array 
}   

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "in_view=%s\n" (string_of_bool_array s.in_view)
|])

let init _ (ls,vs) = {
  in_view = Array.create ls.nmembers false
} 
  

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
(*let ack = make_acker name dnnm in*)
  let log = Trace.log2 name ls.name in

  let up_hdlr ev abv () = up ev abv

  and uplm_hdlr ev hdr = match getType ev,hdr with
  (* ECast|ESend(InView): other member is telling me
   * that he is in the view.
   *)
  | (ECast iovl|ESend iovl), InView ->
      let origin = getPeer ev in
      s.in_view.(origin) <- true ;
      let in_view = Arrayf.of_array s.in_view in
      log (fun () -> sprintf "present=%s" (Arrayf.bool_to_string in_view)) ;
      dnnm (create name EPresent[Presence in_view]) ;
      Iovecl.free iovl

  | _ -> failwith "bad uplm event"


  and upnm_hdlr ev = match getType ev with
  (* Send a message to the coordinator saying that I'm
   * here.  
   *)
  | EInit ->
      if ls.am_coord then (
	s.in_view.(ls.rank) <- true ;
	let in_view = Arrayf.of_array s.in_view in
      	dnnm (create name EPresent[Presence in_view]) ;
      )
      else
	dnlm (sendPeer name vs.coord) InView ;
      upnm ev

  | EDump -> dump vf s ; upnm ev
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv ()

  and dnnm_hdlr ev = dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
