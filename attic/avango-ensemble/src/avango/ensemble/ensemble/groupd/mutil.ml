(**************************************************************)
(* MUTIL.ML *)
(* Author: Mark Hayden, 8/97 *)
(* Designed with Roy Friedman *)
(**************************************************************)
open Trans
open Util
(**************************************************************)
let name = Trace.file "MUTIL"
let failwith s = failwith (name^":"^s)
let log = Trace.log name
(**************************************************************)

(* ENDPT: the type of endpoints.
 *)
type endpt = Buf.t

(* GROUP: the type of groups.
 *)
type group = Buf.t

(**************************************************************)

type member_msg =
  | Join of ltime
  | Synced
  | Fail of endpt list
   
(**************************************************************)

type coord_msg =
  | View of ltime * primary * (endpt list)
  | Sync
  | Failed of endpt list

(**************************************************************)

let soer = ref (fun _ -> raise Not_found)
let set_string_of_endpt soe = soer := soe

let string_of_endpt endpt =
  try !soer endpt with Failure _ ->
    (* If soer doesn't work, then just print Id as hex.
     *)
    sprintf "Endpt_nm{%s}" (Buf.to_hex endpt)

(**************************************************************)

let string_of_member_msg = function
  | Join(ltime) -> sprintf "Join(%d)" ltime
  | Synced -> "Synced"
  | Fail(endpts) -> sprintf "Fail(%s)" (string_of_list string_of_endpt endpts)

let string_of_coord_msg = function
  | View(ltime,primary,view) -> sprintf "View(%d,%b,%d)" ltime primary (List.length view)
  | Failed(_) -> "Failed"
  | Sync -> "Sync"


(**************************************************************)

type state =
  | GNormal
  | GSynced
  | GSyncing

let wrap f send =
  let state = ref GSynced in
  let send msg = 
    begin
      match !state,msg with
      |	GSyncing,Synced ->
	  state := GSynced
      |	_,Fail _ -> ()
      |	_,Join _ -> 
	  failwith "bad mesage:Join"
      |	GSynced,Synced ->
	  failwith "Synced:already synced"
      |	GNormal,Synced ->
	  failwith "Synced:in normal state"
(*
      |	_,_ -> 
	  eprintf "PROXY:member_msg=%s\n" (string_of_member_msg msg) ;
	  failwith "invalid msg from client"
*)
    end ;
    send msg
  in

  let recv = f send in
    
  let recv msg = 
    begin
      match !state,msg with
      |	GNormal,Sync ->
	  state := GSyncing
      |	GSynced,View _ ->
	  state := GNormal
      |	(GSyncing|GSynced),Failed _ -> ()
      |	GNormal,Failed _ ->
	  failwith "got Failed while in Normal state"
      |	_,View _ ->
	  failwith "got View while not Synced state"
      |	_,Sync ->
	  failwith "got Sync while not Normal state"
(*      |	_,_ -> failwith "invalid event[2]"*)
    end ;
    recv msg
  in

  recv

(**************************************************************)
