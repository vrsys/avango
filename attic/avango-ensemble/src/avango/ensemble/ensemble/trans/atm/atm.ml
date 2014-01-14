(**************************************************************)
(* ATM.ML: ATM network transport *)
(* Author: Mark Hayden, 12/95 *)
(**************************************************************)
open Util
open Trans
open Buf
(**************************************************************)
let name = Trace.file "ATM"
let failwith s = failwith (Util.failmsg name s)
(**************************************************************)

external atm_init : unit -> unit 
  = "ens_atm_init"
external atm_activate : vci -> unit 
  = "ens_atm_activate"
external atm_send : vci Arrayf.t -> Buf.t -> ofs -> len -> unit 
  = "ens_atm_send" "noalloc"
external atm_recv : Buf.t -> ofs -> len -> len
  = "ens_atm_recv" "noalloc"

(**************************************************************)
(* Hardwire: host <--> VCI table
 *)

let hosts = [
  100, "eclair0" ; 110, "eclair0" ; 120, "eclair0" ;
  101, "eclair1" ; 111, "eclair1" ; 121, "eclair1" ;
  102, "eclair2" ; 112, "eclair2" ; 122, "eclair2" ;
  103, "eclair3" ; 113, "eclair3" ; 123, "eclair3" ;
  104, "eclair4" ; 114, "eclair4" ; 124, "eclair4" ;
  105, "gungnir" ; 115, "gungnir" ; 125, "gungnir" ;
  106, "gunnlod" ; 116, "gunnlod" ; 126, "gunnlod" ;
  107, "snotra"  ; 117, "snotra"  ; 127, "snotra"
]

let vcis = [
  "eclair0", 110 ;
  "eclair1", 111 ;
  "eclair2", 112 ;
  "eclair3", 113 ;
  "eclair4", 114 ;
  "gungnir", 115 ;
  "gunnlod", 116 ;
  "snotra",  117
]

(**************************************************************)
let atm_max_msg = len_of_int (16*1024)

let domain alarm =
  let mbuf = Mbuf.create name (len_of_int 128000) atm_max_msg in
  let handlers = Alarm.handlers alarm in
  let host = Hsys.gethost () in
  let vcis =
    List.map (fun (name,vci) ->
      (Hsys.inet_of_string name),vci
    ) vcis
  in

  let get_my_vci () =
    try List.assoc host vcis with Not_found ->
      eprintf "ATM:can't find my vci in host table\n" ;
      exit 1
  in

  let inited = ref false in
  let init () =
    if not !inited then (
      inited := true ;
      atm_init () ;
      let my_vci = get_my_vci () in
      List.iter (fun (_,vci) ->               (* ??? *)
	if vci <> my_vci then
	  atm_activate vci
      ) vcis
    )
  in

  let poll = Mbuf.alloc_recv name mbuf handlers atm_recv Route.deliver in

  let endpt mode = Addr.AtmA host in

  let enable _ _ _ _ = 
    init () ;
    Alarm.add_poll alarm name poll ;
    
    let disable () =
      Alarm.rmv_poll alarm name

    and xmit dest =
      let dests =
	match dest with
	| Domain.Pt2pt dests -> (
	    Arrayf.map (fun dest ->
	      match Addr.project dest Addr.Atm with 	  
	      | Addr.AtmA inet -> List.assoc inet vcis
	      | _ -> failwith sanity
	    ) dests
	  )
	| Domain.Mcast _ -> failwith "multicast not supported"
	| Domain.Gossip _ -> failwith "gossip not supported"
      in

      let xmit = atm_send dests in
      let xmitv iov = 
	let iov = Mbuf.flatten name mbuf iov in
	Iovec.read name iov xmit
      in
      let xmitvs iovl s =
	let iov = Mbuf.alloc name mbuf s len0 (Buf.length s) in
	let iovl = Iovecl.appendi name iovl iov in
	xmitv iovl ;
	Iovecl.free name iovl
      in Some(xmit,xmitv,xmitvs)
    in
    Domain.handle disable xmit
  in

  Domain.create name endpt enable

let _ = 
  Elink.put Elink.atm_domain domain

(**************************************************************)
