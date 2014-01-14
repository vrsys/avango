(**************************************************************)
(* ASSERT.ML *)
(* Author: Mark Hayden, 6/95 *)
(* Based on code by: Robbert vanRenesse *)
(* Basic sanity checks on event fields *)
(**************************************************************)
open Layer
open Util
open Event
open View
(**************************************************************)
let name = Trace.filel "ASSERT"
(**************************************************************)

type header = NoHdr

type state = {
  mutable strange 	: string list ;

  mutable up_init 	: bool ;
  mutable dn_view     	: bool ;
  mutable up_view 	: bool ;
  mutable dn_merge 	: bool ;
  mutable up_merge_failed : bool
}

let init _ (ls,vs) = "(???)"

let hdlrs name (ls,vs) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let s = {
    strange 		= [] ;
    up_init 		= false ;
    dn_view 		= false ;
    up_view 		= false ;
    dn_merge 		= false ;
    up_merge_failed 	= false
  } in
  let failwith s = failwith ("ASSERT:"^name^":"^s) in

  let sanitycheck event descr =
    eprintf "ASSERT:sanity check failed: '%s'\n" descr ;
    failwith sanity
  in

  let chk_init () =
    if not s.up_init then
      failwith "EInit not first event"
  in

  let up_hdlr ev abv hdr =
    chk_init () ;
    begin
      match getType ev with
      | EBlock
      | EBlockOk
      | EDump
      | EElect
      | EFail
      | EExit
      | EInit
      | ELeave
      | EView
      | EMergeFailed
      | ELostMessage
      | EStable
      | ESuspect
      | ESystemError
      | ETimer -> failwith "got nomsg event with msg"
      | _ -> ()
    end ;

    if (getPeer ev) >=0 && (getPeer ev) >= ls.nmembers then
      sanitycheck ev "ASSERT:sanity check: bad origin [6]" ;
    up ev abv

  and uplm_hdlr _ _ = failwith "uplm_hdlr"

  and upnm_hdlr ev =
    if getType ev <> EInit then
      chk_init () ;

    begin
      match getType ev with
      | EOrphan
      | EMergeDenied
      | EMergeGranted
      | EMergeRequest
      | ESend _
      | ECast _ -> 
	  failwith "got msg event with nomsg"
      | EMergeFailed ->
      	  if s.up_merge_failed then failwith "2nd EMergeFailed" ;
	  s.up_merge_failed <- true
      | EInit ->
          if s.up_init then failwith "2nd EInit" ;
	  s.up_init <- true
      | EView ->
      	  if s.up_view then failwith "2nd EView" ;
	  s.up_view <- true
      | _ -> ()
    end ;
    upnm ev

  and dn_hdlr ev abv =
    chk_init () ;
    begin
      match getType ev with
  (*  | EAck*)
      | EBlock
      | EBlockOk
      | EDump
      | ELeave
      | EView
      | EFail
      | EStable
      | ETimer -> failwith "got nomsg event with msg"
      | EMergeRequest ->
       	  if s.dn_merge then failwith "2nd EMergeRequest" ;
	  s.dn_merge <- true
      | _ -> ()
    end ;
    dn ev abv NoHdr

  and dnnm_hdlr ev =
    chk_init () ;
    begin
      match getType ev with
      | ECast _
      | EMergeRequest
      | EMergeDenied
      | EMergeGranted
      | ESend _ ->
	  failwith "got msg event with nomsg"
      | EView ->
      	  if s.dn_view then failwith "2nd EView" ;
	  s.dn_view <- true
      | _ -> ()
    end ;
    dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
