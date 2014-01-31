(**************************************************************)
(* CHK_FIFO.ML *)
(* Author: Mark Hayden, 6/95 *)
(* Tests for FIFO ordering properties on casts and sends *)
(**************************************************************)
open Layer
open Trans
open Util
open View
open Event
(**************************************************************)
let name = Trace.filel "CHK_FIFO"
(**************************************************************)

type from = Endpt.id
and id = int
and dest = Endpt.id
and origin = rank

type header = NoHdr
| Cast of from * id * seqno
| Send of from * dest * seqno

(**************************************************************)

type s = {
  debug : string ;
  mutable failed : bool Arrayf.t ;
  mutable cast_dn : seqno ;
  cast_up : seqno array ;
  send_dn : seqno array ;
  send_up : seqno array
} 

(**************************************************************)

let unique = counter ()

(**************************************************************)

let init _ (ls,vs) = {
  debug = "(???)" ;
  cast_dn = 0 ;
  cast_up = Array.create ls.nmembers 0 ;
  send_dn = Array.create ls.nmembers 0 ;
  send_up = Array.create ls.nmembers 0 ;
  failed = ls.falses
} 

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "failed=%s\n" (Arrayf.bool_to_string s.failed) ;
  sprintf "cast_dn=%d\n" s.cast_dn ;
  sprintf "cast_up=%s\n" (string_of_int_array s.cast_up) ;
  sprintf "send_dn=%s\n" (string_of_int_array s.send_dn) ;
  sprintf "send_up=%s\n" (string_of_int_array s.send_up) 
|])

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith m = failwith ("CHK_FIFO:"^s.debug^":"^m) in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | ECast _, Cast(chk_from,unique_id,seqno) ->
      (*eprintf "CHK_FIFO:%s:CAST+:%s:" name (Endpt.string_of_id ls.endpt) ; eprint_members members ;*)
      let origin = getPeer ev in

      (*eprintf "CHK_FIFO:%s:%d->%d:receiving #%d (unique_id=%d,actual=%s,expect=%d)\n"
        ls.name origin ls.rank seqno unique_id (Endpt.string_of_id chk_from) s.cast_up.(origin) ;*)

      let err1 = seqno <> s.cast_up.(origin)
      and err2 = chk_from <> Arrayf.get vs.view origin in

      if err1 or err2 then (
	eprintf "CHK_FIFO:event=%s\n" (Event.to_string ev) ;
        eprintf "CHK_FIFO:%s:sanity[1] %s\n" s.debug (Event.to_string ev) ;
        if err1 then (
          eprintf "CHK_FIFO:%s:multicast message out of order unique_id=%d (origin=%d, my_rank=%d, seqno=%d, expect=%d)\n"
            s.debug unique_id origin ls.rank seqno s.cast_up.(origin)
        ) ;

        if err2 then (
          if err1 then eprintf "CHK_FIFO:***AND***\n" ;
          eprintf "CHK_FIFO:incorrect origin m.mendpt, sent=(%s) <> (%s,%d)=my_info\n"
            (Endpt.string_of_id chk_from)  (Endpt.string_of_id (Arrayf.get vs.view origin)) origin
        ) ;

        dump vf s ;
        dnnm (dumpEv name)
      ) ;

      array_incr s.cast_up origin ;
      up ev abv

  | ESend _ , Send(from,dest,seqno) ->
      if dest <> ls.endpt then (
	eprintf "CHK_FIFO:got send message, but not dest\n" ;
        dnnm (dumpEv name)
      ) ;
(*
  | ESend, Failed -> failwith "delivery to failed member"
*)
      let origin = getPeer ev in
(*
      if m.failed then
        failwith "got send from failed member" ;
*)
      let origine = Arrayf.get vs.view origin in
      let err1 = seqno <> s.send_up.(origin)
      and err2 = from  <> origine in
      if err1 || err2 then (
        dump vf s ;
        eprintf "CHK_FIFO:ESend.Send:sanity check failed\n" ;
        eprintf "  out-of-order=%b, dest-incorrect=%b\n" err1 err2 ;
        eprintf "  my_info: msg %d, %s -> %s\n" s.send_up.(origin) (Endpt.string_of_id origine) (Endpt.string_of_id ls.endpt) ;
(*
        eprintf "  message: msg %d, %s -> %s\n" seqno (Endpt.string_of_id from) (Endpt.string_of_id dest) ;
*)
        dnnm (dumpEv name)
      ) ;

      array_incr s.send_up origin ;
      up ev abv

  | _, NoHdr -> up ev abv
  | _, _     -> failwith bad_up_event
  and uplm_hdlr _ _ = failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | ELostMessage ->
      let origin = getPeer ev in
      array_incr s.cast_up origin ;
      upnm ev

  | EFail ->
      s.failed <- getFailures ev ;
      upnm ev

  | EDump -> dump vf s ; upnm ev
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | ECast _ ->
      dn ev abv (Cast(ls.endpt,(unique()),s.cast_dn)) ;
      s.cast_dn <- succ s.cast_dn

  | ESend _ ->
      let dest = getPeer ev in
      let deste = Arrayf.get vs.view dest in
      let seqno =
	  let seqno = s.send_dn.(dest) in
	  array_incr s.send_dn dest ;
	  seqno
      in
      dn ev abv (Send(ls.endpt,deste,seqno))

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr ev = dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
