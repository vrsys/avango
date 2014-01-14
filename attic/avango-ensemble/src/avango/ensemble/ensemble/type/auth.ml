(**************************************************************)
(* AUTH.ML *)
(* Authors: Mark Hayden, Ohad Rodeh, 8/96 *)
(**************************************************************)
(* This file gives a generic interface to authentication
 * services. We assume that the ticket is in base64 format. 
 * The Exchange protocol assumes this property when it does
 * secure marshaling. 
*)
(**************************************************************)
open Util
open Trans
open Buf
(**************************************************************)
let name = Trace.file "AUTH"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
(**************************************************************)

type credentials = string
type clear = string 
type cipher = string 

type t = {
  name   : name ;
  princ  : Addr.id -> credentials -> Addr.t ; 
  seal   : Addr.id -> Addr.set -> Addr.set -> clear -> cipher option ;
  bckgr_seal : Addr.id -> Addr.set -> Addr.set -> clear -> Alarm.t -> 
    (cipher option -> unit) -> unit;
  unseal : Addr.id -> Addr.set -> Addr.set -> cipher -> clear option ;
  bckgr_unseal : Addr.id -> Addr.set -> Addr.set -> cipher -> 
		Alarm.t -> (clear option -> unit) -> unit
}

let handlers = ref [] 

let handler_of_svc svc =
  try
    List.assoc svc !handlers 
  with Not_found -> 
    eprintf "AUTH:did not find the required service\n" ;
    raise Not_found

let create name princ seal bckgr_seal unseal bckgr_unseal = {
  name         = name ;
  princ        = princ ;
  seal         = seal ;
  bckgr_seal   = bckgr_seal ;
  unseal       = unseal ;
  bckgr_unseal = bckgr_unseal
}

let install id t = 
  handlers := (id,t) :: !handlers

let lookup id =
  try 
    List.assoc id !handlers
  with Not_found ->
    eprintf "AUTH:did not find id(%s)\n" (Addr.string_of_id id);
    raise Not_found
	

let principal t = t.princ
let seal t = t.seal
let bckgr_seal t = t.bckgr_seal
let unseal t = t.unseal
let bckgr_unseal t = t.bckgr_unseal

(**************************************************************)

(* This is assumed to be in base64. 
*)
type ticket = cipher

let string_of_ticket = ident 
let ticket_of_string = ident 

type data = 
  | Clear of clear option
  | Ticket  of ticket option

  (* Find intersection of modes.
   *)
let common_mode src dst = 
  let sm = Arrayf.to_list (Addr.ids_of_set src) in
  let dm = Arrayf.to_list (Addr.ids_of_set dst) in
  let m = Lset.intersect sm dm in
  let m = List.filter Addr.has_auth m in
  match m with
  | [] -> None
  | _ -> Some (List.hd m)


let ticket sim src dst clear = 
  if sim then (
    log (fun () -> "simulation");
    Some (hex_of_string clear)
  ) else (
    let id = common_mode src dst in
    match id with 
      | None -> 
	  failwith (sprintf "No authenticated means of contacting src=%s dst=%s" 
	    (Addr.string_of_set src) (Addr.string_of_set dst))
      | Some id -> 
	  let t = lookup id in
	  let info = seal t id src dst clear in
	  match info with
	    | None -> None
	    | Some tck -> Some tck
  )
		
let bckgr_ticket sim src dst clear alarm rep_fun =
  (* Simulation
   *)
  if sim then (
    log (fun () -> "simulation");
    rep_fun (Some (hex_of_string clear))
  ) else (
    let id = common_mode src dst in
    match id with 
    | None -> 
	failwith (sprintf "No authenticated means of contacting src=%s dst=%s" 
	  (Addr.string_of_set src) (Addr.string_of_set dst))
    | Some id -> 
	let t = lookup id in
	bckgr_seal t id src dst clear alarm (function 
	  | None -> rep_fun None
	  | Some tck -> rep_fun (Some tck)
	)
  )

let check sim dst src cipher = 
  if sim then (
    log (fun () -> "simulation");
    Some (hex_to_string cipher)
  ) else (
    let id = common_mode src dst in
    match id with 
      | None -> None
      | Some id -> 
	  let t = lookup id in
	  match unseal t id src dst cipher with
	    | None -> None
	    | Some clear -> Some clear
  )
    
let bckgr_check sim dst src cipher alarm rep_fun =
  (* Simulation
   *)
  if sim then (
    log (fun () -> "simulation");
    rep_fun (Some (hex_to_string cipher))
  ) else
    let id = common_mode src dst in
    match id with
    | None -> 
        log (fun () -> sprintf "src (%s) and dst (%s) have not common authentication mode" (Addr.string_of_set src) (Addr.string_of_set dst));
        rep_fun None
    | Some id -> 
        let t = lookup id in
        bckgr_unseal t id src dst cipher alarm (function 
          | None ->       rep_fun None
          | Some clear -> rep_fun (Some clear)
	)

(**************************************************************)
