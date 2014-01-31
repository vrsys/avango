(**************************************************************)
(* MANAGE.ML *)
(* Author: Mark Hayden, 11/96 *)
(* Designed with Roy Friedman *)
(* Ohad Rodeh 11/2001: Ported to the new interface and marhaling code *)
(**************************************************************)
(* BUGS:

 * Does not provide EVS: members can fail and rejoin next view.

 *)
(**************************************************************)
open Util
open View
open Event
open Mutil
open Proxy
open Trans
open Buf
(**************************************************************)
let name = Trace.file "MANAGE"
let failwith s = failwith (name^":"^s)
let log = Trace.log name
(**************************************************************)

let make_marsh_id debug =
  let marsh,unmarsh = Buf.make_marsh (info name debug) in
  let marsh o =
    let o = deepcopy o in
    marsh o
  and unmarsh s =
    unmarsh s len0 
  in (marsh,unmarsh)


(**************************************************************)

type mendpt = {
  mendpt : Endpt.id ;
  maddr : Addr.set ;
  mclient : bool ;
  mood : ltime ;
  mlwe : Endpt.id Arrayf.t
} 

let ma_endpt, um_endpt = make_marsh_id "noshare:endpt"
let ma_group, um_group = make_marsh_id "noshare:group"

(**************************************************************)

let _ = 
  let string_of_endpt e =
    try
      let endpt = um_endpt e in
      Endpt.string_of_id endpt.mendpt
    with _ -> raise Not_found
  in
  Mutil.set_string_of_endpt string_of_endpt

(**************************************************************)

type ('m,'g) t = 
  | Actual of ('m,'g) Actual.t
  | Proxy of Proxy.t

let server_of name = function
  | Actual s -> s
  | Proxy _ -> failwith (name^":can't use proxy")

(**************************************************************)

let create appl (ls,vs) =
  let t,vs,intf = Actual.create appl (ls,vs) in
  Actual t, vs, intf

let proxy_server appl s p = 
  let s = server_of "proxy_server" s in
  Proxy.server appl p (Actual.join s)

let create_proxy_server appl p (ls,vs) =
  let t,vs,intf = Actual.create appl (ls,vs) in
  Proxy.server appl p (Actual.join t) ;
  (vs,intf)
		     
let proxy appl s = Proxy (Proxy.create appl s)

(**************************************************************)

let join s g e ltime send =
  let join = 
    match s with
    | Actual s -> 
      	Actual.join s g e ltime send
    | Proxy s ->
 	Proxy.join s g e ltime send
  in
  log (fun () -> sprintf "Joining %s" (string_of_endpt e));
  join (Join ltime) ;
  join

(**************************************************************)

let split_view vf =
  let vf = Arrayf.of_list vf in
  let view = Arrayf.map (fun i -> i.mendpt) vf in
  let address = Arrayf.map (fun i -> i.maddr) vf in
  let clients = Arrayf.map (fun i -> i.mclient) vf in
  let lwe = Arrayf.map (fun i -> i.mlwe) vf in
  let ood = Arrayf.map (fun i -> i.mood) vf in
  (view,address,clients,lwe,ood)

(**************************************************************)

let config s (ls,vs) stack =
  let log = Trace.log2 name ls.name in
  log (fun () -> "configuring, a new interface");
  let endpt = ls.endpt in

  if Arrayf.length vs.lwe <> ls.nmembers then failwith sanity ;
  let endpt_full = 
    { mendpt = ls.endpt ;
      maddr = ls.addr ; 
      mood = Arrayf.get vs.out_of_date ls.rank ;
      mclient = Arrayf.get vs.clients ls.rank ;
      mlwe = Arrayf.get vs.lwe ls.rank
    }
  in

  let view_r = ref vs.view in
  let to_server_r = ref (fun _ -> failwith sanity) in
  let to_server_o m = !to_server_r m in

  let got_first_view = ref false in
  let to_member_r = ref (fun _ -> failwith sanity) in
  let to_member msg =
    if !got_first_view then (
      !to_member_r msg
    ) else (
      got_first_view := true ;
      let ltime,primary,view = match msg with
      |	View(ltime,primary,view) -> ltime,primary,view
      |	_ -> failwith "first message not View"
      in
      
      let view = List.map um_endpt view in
      let view,address,clients,lwe,ood = split_view view in
      view_r := view ;
      let vs = View.set vs [
	Vs_view view ;
	Vs_address address ;
	Vs_ltime ltime ;
	Vs_primary primary ;
	Vs_clients clients ;
	Vs_out_of_date ood ;
	Vs_lwe lwe
      ] in
      let ls = local name ls.endpt vs in

      let up ev = 
	log (fun () -> sprintf "up:%s" (Event.to_string ev)) ;
	match getType ev with
      	| EPrompt -> 
	    log (fun () -> "Fail") ;
	    to_server_o (Fail[])
      	| EExit -> 
	    log (fun () -> "Fail(leave/exit)") ;
	    to_server_o (Fail([ma_endpt endpt_full]))
      	| EBlockOk -> 
	    log (fun () -> "Synced") ;
	    to_server_o Synced
      	| _ -> failwith sanity
      in

      let dn = stack (ls,vs) up in

      let to_member msg = 
	log (fun () -> sprintf "to_member:%s" (string_of_coord_msg msg)) ;
	match msg with
      	| Sync -> 
	    log (fun () -> "Sync") ;
	    dn (Event.create name EBlock[])
      	| Failed(failed) ->
	    log (fun () -> "Failed()") ;
	    let failed = List.map um_endpt failed in
	    let failede = List.map (fun ef -> ef.mendpt) failed in
	    let exiting = List.mem endpt failede in
	    let ranks = 
	      List.fold_left (fun l endpt -> 
		try
		  (Arrayf.index endpt !view_r) :: l
		with Not_found ->
		  log (fun () -> sprintf "warning:failure(%s) not in view(%s)"
		    (Endpt.string_of_id endpt) (View.to_string !view_r)) ;
		  l
	      ) [] failede
	    in
	    if ranks <> [] then (
	      if exiting then (
		dn (Event.create name EExit[]) ;
		to_server_r := (fun _ -> eprintf "MANAGE:got msg for server after exit\n") ;
	      ) else (
		log (fun () -> sprintf "view=%s" (View.to_string !view_r)) ;
		log (fun () -> sprintf "failed=%s" (string_of_list Endpt.string_of_id failede)) ;
		log (fun () -> sprintf "ranks=%s" (string_of_int_list ranks)) ;
		log (fun () -> sprintf "endpt=%s" (Endpt.string_of_id endpt)) ;
		let failures = Arrayf.of_ranks (Arrayf.length !view_r) ranks in
		dn (Event.create name EFail[Failures failures]) ;
	      )
	    )

	| View(ltime,primary,view) ->
	    log (fun () -> sprintf "View(ltime=%d,nmembers=%d)" ltime (List.length view)) ;
	    let view = List.map um_endpt view in
	    let view,address,clients,lwe,ood = split_view view in
	    view_r := view ;
	    let vs = View.set vs [
	      Vs_view view ;
	      Vs_address address ;
	      Vs_ltime ltime ;
	      Vs_primary primary ;
	      Vs_clients clients ;
	      Vs_lwe lwe ;
	      Vs_out_of_date ood
	    ] in
	    dn (Event.create name EView[ViewState vs])
      in
      
      to_member_r := to_member
    )
  in

  let endpt = ma_endpt endpt_full in
  let group = ma_group vs.group in
  to_server_r := join s group endpt vs.ltime to_member

(**************************************************************)

let set_properties s member handler =
  let handler group g = handler (um_group group) g in
  let s = server_of "properties" s in
  Actual.set_properties s member handler

let announce s group filter properties =
  let group = ma_group group in
  let s = server_of "announce" s in
  Actual.announce s group filter properties

let destroy s group =
  let group = ma_group group in
  let s = server_of "destroy" s in
  Actual.destroy s group

(**************************************************************)
(* Create and configure
*)
let groupd_create appl vf =
  let (m,vf,ai) = create appl vf in
  let m a b = config m a b in
  (m,vf,ai)
  
type t2 = 
    View.full -> 
    (View.full -> (Event.up -> unit) -> (Event.dn -> unit)) -> 
      unit
  
let groupd_proxy appl s =
  let m = proxy appl s in
  let m a b = config m a b in
  m
    
(**************************************************************)
