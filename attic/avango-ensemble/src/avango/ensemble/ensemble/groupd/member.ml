(**************************************************************)
(* MEMBER.ML *)
(* Author: Mark Hayden, 8/97 *)
(* Designed with Roy Friedman *)
(**************************************************************)
open Util
open Trans
open Mutil
(**************************************************************)
let name = Trace.file "MEMBER"
let failwith s = failwith (name^":"^s)
let log = Trace.log name
(**************************************************************)
(* STATE: A member is in one of 3 states:
 * Normal: normal state
 * Syncing: in the process of synchronizing.
 * Syncd: finished synchronizing.
 *)

(* LaTeX:begin *)
type state = Normal | Syncing | Syncd

(* MEMBER: Record containing the state of a member.  Each
 * client has one such record.  
 *)

type t = {
  endpt : endpt ;                     (* my endpoint *)
  mutable state : state ;             (* my state *)
  mutable ltime : ltime ;               (* my logical time *)
  mutable view : endpt list ;         (* view of group *)
  mutable coord : rank ;              (* who do I think is coordinator **)
  send_to_client : coord_msg -> unit ;(* send message to client **)
  send_to_coord : member_msg -> unit ;(* send message to coord **)
  disable : unit -> unit              (* disable this member **)
}               
(* LaTeX:end *)

(**************************************************************)

let create endpt to_client to_coord disable coord ltime = {
  endpt = endpt ;
  send_to_client = to_client ;
  send_to_coord = to_coord ;
  disable = disable ;
  view = [endpt] ;
  coord = coord ;
  state = Syncd ;
  ltime = ltime 
} 

let endpt m = m.endpt

let summary m =
  (m.endpt,m.state,m.ltime,m.view)

(**************************************************************)

(* LaTeX:begin *)
(* MEMBER_RECV_CLIENT: Handler for a member to receive a
 * message from its client.  It just passes the message on
 * to the coordinator and updates the member's state.  
 *)

let receive_client m msg =
  m.send_to_coord msg ;
  match msg with
  | Join(ltime) -> 
      (* I don't think this ever gets executed.
       *)
      m.state <- Syncd ;
      m.ltime <- ltime ;
  | Synced ->
      m.state <- Syncd
  | Fail(failed) -> 
      if List.mem m.endpt failed then
        m.disable ()
(* LaTeX:end *)
 
(**************************************************************)
(*let loge = Trace.f "ACTUAL" (Proxy.string_of_endpt m.endpt)*)

(* LaTeX:begin *)
(* MEMBER_RECV_COORD: Handler for a member to receive a message
 * from coordinator.  The member checks the data, and passes
 * it on to its client.
 *)

let receive_coord m msg =
  log (fun () -> sprintf "receive_coord:%s:%s" 
    (string_of_endpt m.endpt)
    (string_of_coord_msg msg)) ;
  match msg with
  | Sync ->
(* I think this case must be allowed, though I'm not sure. --MH
      if m.state = Syncing then
        failwith "Sync when state is Syncing" ;  (* Syncd is Ok *)
*)

      (* If in Normal state, then start synchronizing.
       *)
      if m.state = Normal then (
        log (fun () -> sprintf "Sync(accepted)") ;(**)
        m.state <- Syncing ;
        m.send_to_client msg
      )

  | Failed(failed) ->
      if m.state = Normal then
        failwith "Fail when state is Normal" ;

      (* If the member being failed is in my view,
       * then pass the message to the client.
       *)
      let in_view = Lset.intersect failed m.view in
      if in_view <> [] then (
      	log (fun () -> sprintf "Failed()") ;(**)
        m.send_to_client (Failed(in_view)) ;
      ) ;

      (* If I'm the one being failed, then disable me.
       *)
      (* I'm not sure that this gets called because
       * we normally do this diabling stuff above.
       *)
      if List.mem m.endpt failed then
        m.disable ()

  | View(ltime,primary,view) ->
      if m.state <> Syncd then        (**)
        failwith "View when state isn't Syncd" ;(**)

      (* If I'm listed in the view, then install the
       * information.
       *)
      if List.mem m.endpt view then (
        if m.state <> Syncd then      (**)
          failwith "got View but not Syncd" ;(**)
        m.state <- Normal ;
        m.ltime <- ltime ;
        m.view <- view ;
        m.send_to_client msg ;
      	log (fun () -> sprintf "View accepted:%s" (string_of_endpt m.endpt)); (**)
      ) else (
      	log (fun () -> sprintf "View rejected:%s" (string_of_endpt m.endpt)); (**)
      )	
(* LaTeX:end *)
