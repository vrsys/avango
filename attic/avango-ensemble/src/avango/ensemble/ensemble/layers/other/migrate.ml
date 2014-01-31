(**************************************************************)
(* MIGRATE.ML : protocol migration management *)
(* Author: Mark Hayden, 3/97 *)
(**************************************************************)
open Layer
open View
open Event
open Util
(**************************************************************)
let name = Trace.filel "MIGRATE"
let failwith = Trace.make_failwith name
(**************************************************************)

type header = NoHdr
  | Migrate of Addr.set

(**************************************************************)

type state = {
  laddress : Addr.set array
}

(**************************************************************)

let dump = ignore2

(**************************************************************)

let init _ (ls,vs) = {
  laddress = Arrayf.to_array vs.address
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm}  =
  let log = Trace.log2 name ls.name in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | _,NoHdr -> up ev abv
  | _ -> failwith bad_up_event

  and uplm_hdlr ev hdr = match getType ev, hdr with
  | ECast iovl, Migrate(address) ->
      let origin = getPeer ev in
      s.laddress.(origin) <- address ;
      if ls.am_coord then
	upnm (create name EPrompt []) ;
      Iovecl.free iovl

  | _ -> failwith "bad uplm event"

  and upnm_hdlr ev = match getType ev with
  | EElect ->
      upnm ev ;
      if Arrayf.of_array s.laddress <> vs.address then
	upnm (create name EPrompt [])
  | EMigrate ->
      log (fun () -> Event.to_string ev) ;
      let addr = getAddress ev in
      s.laddress.(ls.rank) <- addr ;
      dnlm (castEv name) (Migrate(addr)) ;

      (* Trigger a view change if I'm coordinator.
       *)
      if ls.am_coord then
	upnm (create name EPrompt []) ;

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr ev = match getType ev with
  | EView ->
      let old_addr = s.laddress in
      let old_map = Arrayf.combine vs.view (Arrayf.of_array old_addr) in
      let old_map = Arrayf.to_list old_map in

      let new_vs = getViewState ev in
      let new_map = Arrayf.combine new_vs.view new_vs.address in
      let new_map = Arrayf.to_list new_map in
      let new_map =
	List.map (fun ((endpt,_) as it) ->
	  try
	    let addr = List.assoc endpt old_map in (* BUG:PERF *)
	    (endpt,addr)
	  with Not_found -> it
	) new_map
      in
      let new_addr = List.map snd new_map in
      let new_addr = Arrayf.of_list new_addr in
      let new_vs = View.set new_vs [Vs_address new_addr] in
      let ev = set name ev [ViewState new_vs] in
      dnnm ev

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
