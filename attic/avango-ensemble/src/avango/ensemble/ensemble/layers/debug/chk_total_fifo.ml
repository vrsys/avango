(**************************************************************)
(* CHK_TOTAL_FIFO.ML *)
(* Author: Mark Hayden, 6/95 *)
(* Tests for FIFO ordering properties on casts and sends *)
(**************************************************************)
open Layer
open Trans
open Util
open View
open Event
(**************************************************************)
let name = Trace.filel "CHK_TOTAL_FIFO"
(**************************************************************)

type from = Endpt.id
and id = int
and dest = Endpt.id
and origin = rank

type header = NoHdr
| Cast of from * id * seqno
| Send of from * (dest * (seqno option)) list

(**************************************************************)

type member = {
  mendpt : Endpt.id ;
  mutable failed : bool ;
  mutable send_up : int ;
  mutable send_dn : int ;
  mutable cast_up : int
}

(**************************************************************)

let print_members members =
  for_array (fun rank m ->
    sprintf "mbr(%d):" rank ;
    if m.failed then
      eprintf "failed\n"
    else
      eprintf "%s,sup=%d,cup=%d,sdn=%d\n"
      	(Endpt.string_of_id m.mendpt) m.send_up m.cast_up m.send_dn
  ) members

(**************************************************************)

let unique_id = ref 0

(**************************************************************)

let init name (ls,vs) = name

let hdlrs name (ls,vs) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let cast_dn = ref 0
  and members = Array.map (fun endpt -> {
      mendpt = endpt ;
      failed = false ;
      send_up = 0 ;
      send_dn = 0 ;
      cast_up = 0
    }) vs.view
  and await_view = ref false in
  let dump () =
    eprintf "FIFO_CHK:dump:%s\n" ls.name ;
    sprintf "rank=%d\n" ls.rank ;
    print_members members
  in
  let failwith s = failwith ("CHK_TOTAL_FIFO:"^name^":"^s) in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  | ECast, Cast(chk_from,unique_id,seqno) ->
      (*eprintf "CHK_TOTAL_FIFO:%s:CAST+:%s:" name (Endpt.string_of_id ls.endpt) ; eprint_members members ;*)
      let origin = getPeer ev in
      let m = members.(origin) in
      let {cast_up=cast_up; mendpt=endpt} = m in
(*
      eprintf "CHK_TOTAL_FIFO:%s:%s->%s:receiving #%d (unique_id=%d,actual=%s)\n"
        name (Endpt.string_of_id m.mendpt) (Endpt.string_of_id ls.endpt) seqno unique_id (Endpt.string_of_id chk_from) ;
*)
      let err1 = seqno <> m.cast_up
      and err2 = chk_from <> m.mendpt in

      if err1 or err2 then (
        dump () ;
        eprintf "CHK_TOTAL_FIFO:%s:sanity[1] %s\n" name (Event.to_string ev) ;
        if err1 then (
          eprintf "CHK_TOTAL_FIFO:%s:multicast message out of order unique_id=%d (origin=%d(%s), my_rank=%d, seqno=%d, expect=%d)\n"
            name unique_id origin (Endpt.string_of_id m.mendpt) ls.rank seqno m.cast_up
        ) ;

        if err2 then (
          if err1 then eprintf "CHK_TOTAL_FIFO:***AND***\n" ;
          eprintf "CHK_TOTAL_FIFO:incorrect origin m.mendpt, sent=(%s) <> (%s,%d)=my_info\n"
            (Endpt.string_of_id chk_from) (Endpt.string_of_id m.mendpt) origin
        ) ;

        failwith sanity
      ) ;

      m.cast_up <- succ m.cast_up ;
      up ev abv

  | ESend, Send(from,dest_info) ->
      let seqno = 
	try
	  List.assoc ls.endpt dest_info
	with Not_found ->
	  eprintf "CHK_TOTAL_FIFO:got send message, but not in dest list\n" ;
	  failwith sanity
      in
(*
  | ESend, Failed -> failwith "delivery to failed member"
*)
      let seqno = some_of seqno in
      let origin = (getPeer ev) in
      let m = members.(origin) in
(*
      if m.failed then
        failwith "got send from failed member" ;
*)
      let {mendpt=endpt; send_up=send_up} = m in
      let err1 = seqno <> send_up
      and err2 = from  <> m.mendpt in
      if err1 || err2 then (
        dump () ;
        eprintf "CHK_TOTAL_FIFOy:ESend.Send:sanity check failed\n" ;
        sprintf "out-of-order=%b, dest-incorrect=%b\n" err1 err2 ;
        sprintf "my_info: msg %d, %s -> %s\n" send_up (Endpt.string_of_id m.mendpt) (Endpt.string_of_id ls.endpt) ;
(*
        sprintf "message: msg %d, %s -> %s\n" seqno (Endpt.string_of_id from) (Endpt.string_of_id dest) ;
*)
        failwith sanity
      ) ;

      m.send_up <- succ m.send_up ;
      up ev abv

  | _, NoHdr -> up ev abv
  | _, _     -> failwith bad_up_event
  and uplm_hdlr _ _ = failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | ELostMessage ->
      let m = members.((getPeer ev)) in
      m.cast_up <- succ m.cast_up ;
      upnm ev

  | EFail ->
      let suspicions = getFailures ev in
      List.iter (fun i ->
      	members.(i).failed <- true
      ) suspicions ;
      upnm ev
  | EDump -> dump () ; upnm ev
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | (ECast|ESend) when (getUnreliable ev) or (getNoTotal ev) ->
      dn ev abv NoHdr

  | ECast ->
      dn ev abv (Cast(ls.endpt,!unique_id,!cast_dn)) ;
      incr unique_id ;
      incr cast_dn

  | ESend ->
      let dest_info =
      	List.map (fun dest ->
          let m = members.(dest) in
          let seqno =
            if m.failed then None else (
	      let seqno = Some(m.send_dn) in
              m.send_dn <- succ m.send_dn ;
              seqno
            )
	  in (m.mendpt,seqno)
	) (Array.to_list (getPeers ev))
      in

      dn ev abv (Send(ls.endpt,dest_info))

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr ev = match getType ev with
  | EView ->
      await_view := true ;
      dnnm ev
  | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Elink.layer_install name (Layer.initNamed l)

(**************************************************************)
