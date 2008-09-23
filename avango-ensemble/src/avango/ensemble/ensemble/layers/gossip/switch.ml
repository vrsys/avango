(**************************************************************)
(* SWITCH.ML : protocol switch management *)
(* Author: Mark Hayden, 3/96 *)
(* Does several things:

 * 1.  Keeps track of most recent EProtocol protocol request.

 * 2.  On views, the coordinator sends out his protocol
 * suggestion with the view.

 * 3.  Coordinator gossips with other coordinators about
 * recent protocol requests.  When it hears of one, it does
 * a protocol switch in its own partition.

 *)
(* BUG: should use elect events to determine if I'm coord. *)
(**************************************************************)
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "SWITCH"
let failwith = Trace.make_failwith name
(**************************************************************)

type header = Proto of Proto.id

type saved_state = Time.t

(**************************************************************)

type state = {
  saved_state		: Time.t saved ;
  mutable bcasted       : bool ;
  mutable cur_time 	: Time.t ;
  mutable timestamp 	: Time.t ;
  mutable proto_id_sug 	: Proto.id option
}

(**************************************************************)

let dump (ls,vs) s = ()

(**************************************************************)

let init s vs =
  let state = s.switch in
  let timestamp =
    match !state with
    | None -> Time.invalid
    | Some(time) -> time
  in
  state := None ;
{
  saved_state 	= state ;
  cur_time	= Time.zero ;
  bcasted	= false ;
  proto_id_sug  = None ;
  timestamp	= timestamp
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm}  =
  let log = Trace.log2 name ls.name in
(*let ack = make_acker name dnnm in*)
  let gossip_send () =
    (* Used to send out suggested protocol.
     *)
    (SwitchGos(vs.proto_id,ls.view_id,s.timestamp))
  in

  let gossip_recv (proto_id,view_id,timestamp(*,key*)) =
    if view_id <> ls.view_id		(* not from my view *)
(*
    && key = vs.key			(* has same key HACK!! *)
*)
    && ls.am_coord			(* I'm coordinator *)
    && proto_id <> vs.proto_id		(* not my current protocol *)
    && s.proto_id_sug = None		(* I'm not looking for another *)
    then (
      (* If more recent timestamp & view_id.
       *)
      if (timestamp,view_id) > (s.timestamp,ls.view_id) then (
    	log (fun () -> sprintf "gossip_recv:accept protocol:%s" (Proto.string_of_id proto_id)) ;
	s.proto_id_sug <- Some proto_id ; (* save suggested protocol *)
	s.timestamp <- timestamp ;	(* and timestamp *)

	(* Trigger a new view.
	 *)
	upnm (create name EPrompt [])
      ) else if not s.bcasted then (
    	log (fun () -> sprintf "gossip_recv:bcasting") ;
	s.bcasted <- true ;
	dnnm (create name EGossipExt[(gossip_send())])
      )
    ) else (
(*
      log (fun () -> sprintf "gossip_recv:dropping view_id=%b am_coord=%b proto_id=%b proto_id_sug=%b"
	  (view_id <> ls.view_id)	(* not from my view *)
	  ls.am_coord			(* I'm coordinator *)
	  (proto_id <> vs.proto_id)     (* not my current protocol *)
	  (s.proto_id_sug = None)	(* I'm not looking for another *))
*)
    )
  in

  let up_hdlr ev abv () = up ev abv

  and uplm_hdlr ev hdr = match getType ev, hdr with
  | (ECast iovl|ESend iovl), (Proto proto_id) ->
      s.proto_id_sug <- Some proto_id ;

      (* Trigger a view change if I'm coordinator.
       *)
      if ls.am_coord then (
	upnm (create name EPrompt []) ;
      ) ;

      Iovecl.free iovl

  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | EView ->
      (* Save my state.
       *)
      s.saved_state := Some(s.timestamp) ;
      upnm ev
  | ETimer ->
      s.cur_time <- getTime ev ;		(* keep the most recent time *)
      upnm ev
  | EProtocol ->
      log (fun () -> Event.to_string ev) ;
      let proto_id = getProtoId ev in
      s.proto_id_sug <- Some proto_id ;
      s.timestamp <- s.cur_time ;

      dnlm (castEv name) (Proto proto_id) ;

      (* Trigger a view change if I'm coordinator.
       *)
      if ls.am_coord then (
	upnm (create name EPrompt []) ;
      ) ;
      free name ev

  | EGossipExt ->
      if ls.am_coord then (
	getExtendOpt ev (function
	  | SwitchGos(a,b,c) -> gossip_recv (a,b,c) ; true
	  | _ -> false
      	)
      ) ;
      upnm ev

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv ()

  and dnnm_hdlr ev = match getType ev with
  | EView ->
      (* Set the protocol id appropriately.
       *)
      let proto_id = match s.proto_id_sug with 
      |	None -> vs.proto_id
      |	Some id -> id
      in

      let vs = getViewState ev in
      let vs = View.set vs [Vs_proto_id proto_id] in
      let ev = set name ev [ViewState vs] in
      
      dnnm ev
  | EGossipExt ->
      if ls.am_coord then (
        s.bcasted <- false ;
      	let ev = set name ev[(gossip_send())] in
	dnnm ev
      ) else (
        dnnm ev
      )

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
