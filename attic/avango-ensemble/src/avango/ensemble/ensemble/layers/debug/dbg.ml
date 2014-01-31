(**************************************************************)
(* DBG.ML : on-line modification of network topology *)
(* Author:  Zhen Xiao, 3/98 *)
(**************************************************************)
open Trans
open Layer
open View
open Event
open Util
open Hsys
open Printf
(**************************************************************)
let name = Trace.filel "DBG"
(**************************************************************)

type header = Origin of string

type state = {
  sock : Hsys.socket ;
  my_name : string ;
  dropall : bool
}

(**************************************************************)

let dump = ignore2

let init _ (ls,vs) = 
  let server = Param.string vs.params "dbg_server" in
  let port = Param.int vs.params "dbg_port" in
  let my_name = Param.string vs.params "dbg_name" in
  let dropall = Param.bool vs.params "dbg_dropall" in
  if my_name = "" then (
    eprintf "DBG: use of DBG protocol layer requires setting\n" ;
    eprintf "  the dbg_name parameter.  Exiting\n" ;
    exit 2 ;
  ) ;
  let sock = Dtbl.create server port my_name in
  { sock = sock ;
    my_name = my_name ;
    dropall = dropall
  }

(**************************************************************)

let stdout = Pervasives.stdout
let flush = Pervasives.flush

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in

  let up_hdlr ev abv hdr = match hdr with
  | Origin(origin) ->
      if Dtbl.drop origin || s.dropall
      then (
	free name ev ;
	log (fun () -> sprintf "dropping msg from %s\n" origin) ;
      ) else up ev abv

  and uplm_hdlr ev _ = failwith unknown_local
    
  and upnm_hdlr ev = match getType ev with
  | EInit ->
      let alarm = Alarm.get_hack () in
      Alarm.add_sock_recv alarm name s.sock (Handler0 (Dtbl.get_input s.sock)) ;
      upnm ev
	
  | EGossipExt ->
      let origin = getDbgName ev in
      if Dtbl.drop origin || s.dropall 
      then (
	free name ev ;
	log (fun () -> sprintf "dropping Gossip msg from %s\n" origin)
      ) else upnm ev

  | EView ->
      Dtbl.leave s.my_name s.sock ;
      upnm ev

  | _ -> upnm ev

  and dn_hdlr ev abv = dn ev abv (Origin s.my_name)

  and dnnm_hdlr ev = match getType ev with
  | EGossipExt ->
    let ev = set name ev[(DbgName s.my_name)] in
    dnnm ev

  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = 
  Param.default "dbg_server" (Param.String "gulag") ;
  Param.default "dbg_port" (Param.Int 2345) ;
  Param.default "dbg_name" (Param.String "") ;
  Param.default "dbg_dropall" (Param.Bool false) ;
  Layer.install name l

(**************************************************************)


