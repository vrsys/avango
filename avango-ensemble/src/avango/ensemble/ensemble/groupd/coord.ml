(**************************************************************)
(* COORD.ML *)
(* Author: Mark Hayden, 8/97 *)
(* Designed with Roy Friedman *)
(**************************************************************)
open Trans
open Util
open Mutil
(**************************************************************)
let name = Trace.file "COORD"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
(**************************************************************)

let insert i l = Lset.union l [i]

let foreach l f = List.iter f l

let list_union l = List.fold_left Lset.union [] l

let list_filter2 f l1 l2 = 
  let l =
    List.fold_left2 (fun l a b ->
      if f a then b :: l else l
    ) [] l1 l2
  in Lset.sort l

(**************************************************************)

(* LaTeX:begin *)
(* COORD: Record containing the state of a coordinator.
 * There is one such coordinator per group.  
 *)

type t = {
  mutable syncing : bool ;		(* am I sync'ing? *)
  mutable ltime : ltime ;               (* view's logical time *)
  mutable primary : primary ;		(* are we in the primary view? *)
  mutable alive : endpt list ;		(* endpoints in the group *)
  mutable syncd : endpt list ;		(* who is sync'ed *)
  broadcast : coord_msg -> unit	(* send a message to members **)
}

(**************************************************************)

let create primary cast = {
  syncing = true ;
  syncd = [] ;
  ltime = 0 ;
  primary = primary ;
  alive = [] ;
  broadcast = cast
}

(**************************************************************)
(* CHECK_VIEW: Helper function for receive.  Check if it is
 * time for the coordinator to install a new view.  
 *)

let check_view c =
  log (fun () -> sprintf "#syncd=%d, #alive=%d"  (**)
    (List.length c.syncd) (List.length c.alive)) ;(**)
      
  if c.syncing && Lset.super c.syncd c.alive then ( 
    log (fun () -> sprintf "check_view:Casting(View)") ;(**)
    c.syncd <- [] ;
    c.ltime <- succ c.ltime ;
    c.syncing <- false ;
    c.broadcast (View(c.ltime,c.primary,c.alive)) ;
  )
(* LaTeX:end *)

(**************************************************************)

(* LaTeX:begin *)
(* RECEIVE: Handler for coordinator receiving a
 * membership message from a member.  
 *)

let receive c (endpt,msg) =    
  (* Synchronize, if haven't done so already.
   *)
  if not c.syncing then (
    c.syncing <- true ;
    log (fun () -> sprintf "receive:Casting(Sync)") ;(**)
    c.broadcast Sync
  ) ;

  (* Process the message.
   *)
  log (fun () -> sprintf "receive:%s" (string_of_member_msg msg)) ;
  begin match msg with
  | Join(ltime) ->
      (* Add the member to the group.
       *)
      c.alive <- insert endpt c.alive ;
      c.syncd <- insert endpt c.syncd ;

      (* Logical time is the max of current and of joiners.
       *)
      c.ltime <- max c.ltime ltime

  | Synced ->
      (* Mark the member as being synchronized.
       *)
      c.syncd <- insert endpt c.syncd ;

  | Fail(failed) ->
      (* Remove the members from the group.
       *)
      c.alive <- Lset.subtract c.alive failed ;
      c.syncd <- Lset.subtract c.syncd failed ;
      if failed <> [] then
      	c.broadcast (Failed failed) ;
  end ;                                 (**)

  (* Check if the view is now ready.
   *)
  check_view c
(* LaTeX:end *)

(**************************************************************)

(* LaTeX:begin *)
(* RECONSTRUCT: Given lists of the fields of the members,
 * reinitialize a coordinator, and send out messages to
 * update members on the state of the group.
 *)

let reconstruct c primary (endpts,states,ltimes,views) =
  (* This is set according to the primary bit of the
   * groupd group.
   *)
  c.primary <- primary ;

  (* Reconstruct fields as follows:
   *
   *  alive: sorted list of endpoints
   *  ltime: maximum of ltimes
   *  syncd: all members in Syncd state
   *  syncing: if any members is not in normal state
   *    or any member disagrees about the view.
   *)
  c.alive <- Lset.sort endpts ;
  c.ltime <- List.fold_left max 0 ltimes ;
  c.syncd <- list_filter2 (fun state -> state = Member.Syncd) states endpts ;
  c.syncing <- 
     List.exists (fun state -> state <> Member.Normal) states ||
     List.exists (fun view -> view <> c.alive) views ||
     List.exists (fun ltime -> ltime <> c.ltime) ltimes ;

  (* Send out Sync if necessary.
   *)
  if c.syncing then (
    log (fun () -> sprintf "recon:Sync") ;
    c.broadcast Sync ;
  ) ;
  
  (* Take the union of all views.
   *)
  let total = list_union views in
  log (fun () -> sprintf "recon:alive=%s" (string_of_list string_of_endpt c.alive)) ;
  log (fun () -> sprintf "recon:total=%s" (string_of_list string_of_endpt total)) ;

  (* Any of members not alive are failed.
   *)
  let failed = Lset.subtract total c.alive in
  if failed <> [] then (
    log (fun () -> sprintf "recon:Failed") ;
    c.broadcast (Failed failed) ;
  ) ;

  (* Check if view is ready.
   *)
  check_view c

(* LaTeX:end *)
(**************************************************************)
