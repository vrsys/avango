(**************************************************************)
(* STACKE.ML *)
(* Author: Mark Hayden, 4/96 *)
(**************************************************************)
open Trans
open Util
open View
open Event
(**************************************************************)
let name = Trace.file "STACKE"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
(**************************************************************)

let init_stack glue alarm ranking state (ls,vs) up =
  let stack = Proto.layers_of_id vs.proto_id in
(*
  log (fun () -> sprintf "proto=%s" (Proto.string_of_id vs.proto_id)) ;
  log (fun () -> sprintf "stack=%s" (string_of_list ident stack)) ;
*)
  let procure l = Glue.convert glue (Layer.get l) in
  let stack =
    match stack with
    | [] -> failwith "init_stack:empty protocol stack"
    | hd :: tl ->
	let hd = Proto.string_of_l hd in
      	List.fold_left (fun stk lyr -> 
	  let lyr = Proto.string_of_l lyr in
	  let stk = Glue.compose stk (procure lyr) in
	  
	  (* Hack: this is necessary because otherwise the
	   * states will not type correctly.
	   *)
	  Obj.magic stk
	) (procure hd) tl
  in
  (Glue.init stack) Layer.NoMsg Layer.NoMsg alarm ranking state (ls,vs) up

(**************************************************************)

let config_full glue alarm ranking state (ls,vs) up =
  let log = Trace.log2 name ls.name in
  let rec loop (ls,vs) dn_rr =
(*
    eprintf "STACKE:config_full:vs=%s\n" (View.string_of_state vs) ;
*)
    let next vs =
      match !dn_rr with
      | None -> failwith "2nd EView"
      | Some dn_r ->
	  (* Loop and then zap my reference to the dn_r ref.
	   *)
	  loop vs (ref (Some dn_r)) ;
      	  dn_rr := None
    in

    let up ev = match getType ev with
    | EExit | EBlockOk | EPrompt -> up ev
    | EView ->
	let old_addr = ls.addr in
	let new_vs = getViewState ev in
	let new_ls = View.local name ls.endpt new_vs in 
	log (fun () -> sprintf "view(Up):%s" (View.string_of_full (new_ls,new_vs))) ;
	let new_addr = new_ls.addr in
	if new_addr = old_addr then 
    	  next (new_ls,new_vs)
	else
	  up ev
    | _ -> 
	eprintf "STACKE:bad up event:%s\n" (Event.to_string ev) ;
	failwith (sanityn 1)
    in

    let dn = init_stack glue alarm ranking state (ls,vs) up in

    let dn ev = match getType ev with
    | EFail | EBlock | EExit -> dn ev
    | EView ->
	dn (Event.create name EExit[]) ;
	let new_vs = getViewState ev in
	let new_ls = View.local name ls.endpt new_vs in 
	next (new_ls,new_vs)
    | _ -> failwith (sanityn 2)
    in

    match !dn_rr with
    | None -> failwith (sanityn 3)
    | Some dn_r ->
    	dn_r := dn
  in

  let dn_r = ref (fun _ -> failwith (sanityn 4)) in
  loop (ls,vs) (ref (Some dn_r)) ;
  let dn ev = !dn_r ev in
  dn

(**************************************************************)

let config glue alarm ranking state vf =
  let up _ = () in
  let dn = config_full glue alarm ranking state vf up in
  ()

(**************************************************************)
