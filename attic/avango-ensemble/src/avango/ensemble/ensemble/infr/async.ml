(**************************************************************)
(* ASYNC.ML *)
(* Author: Mark Hayden, 7/95 *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "ASYNC"
let failwith = Trace.make_failwith name
let log = Trace.log name
(**************************************************************)

let root_sched = Sched.create "root_sched"

type t = {
  table : ((Group.id * Endpt.id),(unit->unit) option ref) Hashtbl.t ;
  sched : Sched.t 
} 

(* Asynchronous request management.
 *)
(*
let table = 
  let table = Hashtbl.create (*name*) 100 in
  Trace.install_root (fun () ->
    [sprintf "ASYNC:#asyncs=%d" (hashtbl_size table)]
  ) ;
  table
*)

let create s = {
  table = Hashtbl.create (*name*) 100 ;
  sched = s
} 

let string_of_id id =
  string_of_pair Group.string_of_id Endpt.string_of_id id

let add a id f =
  let f () = Sched.enqueue a.sched name f in
  let fro = ref (Some f) in
  let disable () =
    fro := None ;
    try 
      match 
	!(Hashtbl.find a.table id) 
      with 
      |	None ->
	  (* Perhaps installed by me.
	   *)
	  Hashtbl.remove a.table id
      |	Some _ ->
	  (* Must have been installed by someone else.
	   *)
	  ()
    with Not_found -> ()
  in

  begin 
    try
      Hashtbl.remove a.table id
    with Not_found -> () 
  end ;

  Hashtbl.add a.table id fro ;
  log (fun () -> sprintf "adding:%s" (string_of_id id)) ;
  disable

let find a id =
  let fro = ref (ref None) in
  let rec loop () = 
    match !(!fro) with
    | None -> (
 	try 
	  fro := Hashtbl.find a.table id ;
	  loop ()
	with Not_found ->
(*
	  Hashtbl.iter (fun (g,e) _ -> eprintf "ASYNC:async:%s,%s\n"
	    (Group.string_of_id g) (Endpt.string_of_id e)) table ;
	  eprintf "ASYNC:async:key:%s,%s\n"
	    (Group.string_of_id (fst id)) (Endpt.string_of_id (snd id)) ;
*)
	  log (fun () -> sprintf "could not find:async:%s,%s" 
	    (Group.string_of_id (fst id))
	    (Endpt.string_of_id (snd id)))
      )
    | Some f -> f ()
  in loop	  

(**************************************************************)
