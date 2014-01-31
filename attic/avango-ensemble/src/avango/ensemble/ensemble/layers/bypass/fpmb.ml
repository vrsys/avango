(**************************************************************)
(* FPMB.ML *)
(* Author: Mark Hayden, 12/97 *)
(**************************************************************)
open Util
open Layer
open Trans
open Event
open View
(**************************************************************)
let name = Trace.filel "FPMB"
let failwith s = Trace.make_failwith name s
(**************************************************************)

let (|||) = Glue.Functional.compose

type 'a msg =
  | Opt0 of seqno
  | Opt1 of seqno
  | Normal of 'a

let compress = function
  | NoMsg -> NoMsg
  | Full(Bottom.NoHdr,Full(Mnak.Data seqno,Full(Pt2pt.NoHdr,Local ()))) -> Local (Opt0 seqno)
  | Full(Bottom.NoHdr,Full(Mnak.NoHdr,Full(Pt2pt.Data seqno,Local ()))) -> Local (Opt1 seqno)
  | msg -> Local (Normal msg)

let string_of_msg msg = 
  let rest = Layer.string_of_msg (fun _ -> "_") (fun _ -> "_") (fun _ -> "_") in
  let top_appl = Layer.string_of_msg Top_appl.string_of_header Top_appl.string_of_header rest in 
  let pt2pt = Layer.string_of_msg Pt2pt.string_of_header Pt2pt.string_of_header top_appl in
  let mnak = Layer.string_of_msg Mnak.string_of_header Mnak.string_of_header pt2pt in 
  let bottom = Layer.string_of_msg Bottom.string_of_header Bottom.string_of_header mnak in 
  bottom msg

let expand = function
  | NoMsg -> NoMsg
  | Local (Opt0 seqno) -> Full(Bottom.NoHdr,Full(Mnak.Data seqno,Full(Pt2pt.NoHdr,Local ())))
  | Local (Opt1 seqno) -> Full(Bottom.NoHdr,Full(Mnak.NoHdr,Full(Pt2pt.Data seqno,Local ())))
  | Local (Normal msg) -> msg
  | _ -> failwith sanity

(*
let compress msg =
  eprintf "FPMB:%s\n" (string_of_msg msg) ;
  compress msg
*)
(*
let expand msg =
  let msg = expand msg in
  eprintf "FPMB:%s\n" (string_of_msg msg) ;
  msg
*)

let abv_mnak = Full(Pt2pt.NoHdr,Local ())
let abv_pt2pt = Local ()

let l state vf =
  let init l =
    let sr = ref None in
    let l state vs = 
      let (s,_) as pair = l state vs in
      sr := Some s ;
      pair
    in
    sr,(Glue.Functional.convert l)
  in
  
  let appl_s,appl = init Top_appl.l2 in
(*
  let frag_s,frag = init Frag.l2 in
*)
  let pt2pt_s,pt2pt = init Pt2pt.l2 in
  let mnak_s,mnak = init Mnak.l2 in
  let bottom_s,bottom = init Bottom.l2 in
  let stack = appl ||| (*frag |||*) pt2pt ||| mnak ||| bottom in
  let stack = Glue.Functional.wrap_msg compress expand stack in
  let stack = Glue.Functional.revert stack in

  let (s,h) = stack state vf in
  let appl_s = some_of name !appl_s in
(*let frag_s = some_of name !frag_s in*)
  let pt2pt_s = some_of name !pt2pt_s in
  let mnak_s = some_of name !mnak_s in
  let bottom_s = some_of name !bottom_s in

  let (ls,vs) = vf in
  let h {up_lout=up;dn_lout=dn} =
    let {up_lin=up;dn_lin=dn} = h {up_lout=up;dn_lout=dn} in

    let get_transport bypass_trans = 
      let (fast_cast : rank -> seqno -> Iovecl.t -> unit) = 
	Transport.cast bypass_trans in
      let fast_send = Array.init ls.nmembers (Transport.send bypass_trans) in

      let mnak_buf = Arrayf.get mnak_s.Mnak.buf ls.rank in

      let handle_actions actions =
	for i = 0 to pred (Array.length actions) do
	  match actions.(i) with
	  | Appl_intf.Cast iov ->
	      let seqno = Iq.read mnak_buf in
	      fast_cast ls.rank seqno iov ;
	      ignore (Iq.opt_insert_doread mnak_buf seqno iov abv_mnak) ;
	      appl_s.Top_appl.cast_xmit <- succ appl_s.Top_appl.cast_xmit
	  | Appl_intf.Send(dest,iov) ->
	      assert (Array.length dest = 1) ;
	      let dest = dest.(0) in
	      assert (dest <> ls.rank) ;
	      let sends = Arrayf.get pt2pt_s.Pt2pt.sends dest in
	      let seqno = Iq.hi sends in
	      fast_send.(dest) ls.rank seqno iov ;
	      ignore (Iq.add sends iov abv_pt2pt) ;
	      array_incr appl_s.Top_appl.send_xmit dest ;
	  | Appl_intf.Send1(dest,iov) ->
	      assert (dest <> ls.rank) ;
	      let sends = Arrayf.get pt2pt_s.Pt2pt.sends dest in
	      let seqno = Iq.hi sends in
	      fast_send.(dest) ls.rank seqno iov ;
	      ignore (Iq.add sends iov abv_pt2pt) ;
	      array_incr appl_s.Top_appl.send_xmit dest ;
	  | Appl_intf.Control _ -> failwith "Control"
	done
      in

      let cast_bypass peer seqno iov =
	let buf = Arrayf.get mnak_s.Mnak.buf peer in
	  if bottom_s.Bottom.all_alive 
	  && appl_s.Top_appl.blocking = Top_appl.Unblocked
	  && Iq.opt_insert_check buf seqno
	  then (
	    handle_actions (appl_s.Top_appl.recv_cast.(peer) iov) ;
	    ignore (Iq.opt_insert_doread buf seqno iov abv_mnak) ;
	    array_incr appl_s.Top_appl.cast_recv peer
	  ) else (
	    up (bodyCore name (ECast iov) peer) (Local(Opt0 seqno))
	  )
      and send_bypass peer seqno iov =
	let recvs = Arrayf.get pt2pt_s.Pt2pt.recvs peer in
	if bottom_s.Bottom.all_alive 
	  && appl_s.Top_appl.blocking = Top_appl.Unblocked
	  && Iq.opt_update_check recvs seqno
	then (
	  handle_actions (appl_s.Top_appl.recv_send.(peer) iov) ;
	  ignore (Iq.opt_update_update recvs seqno) ;
	  array_incr appl_s.Top_appl.send_recv peer
	) else (
	  up (bodyCore name (ESend iov) peer) (Local(Opt1 seqno))
	)
      in
  
      let fast_receive kind _ =
	match kind with
	| Conn.Cast -> cast_bypass 
	| Conn.Send -> send_bypass 
(*
	| Conn.Other -> fun _ _ -> failwith sanity
*)
      in
    
      let up ev msg =
	let peer = getPeer ev in
	match getType ev,msg with
	| ECast iovl,Local(Opt0 seqno) -> cast_bypass peer seqno iovl
	| ESend iovl,Local(Opt1 seqno) -> send_bypass peer seqno iovl
	| _ -> up ev msg
      in

      (fast_receive,{up_lin=up;dn_lin=dn})
    in

    let alarm = Appl.alarm name in	(*HACK*)


    Transport.f2
      alarm
      Addr.default_ranking		(*BUG*)
      (ls,vs)
      Stack_id.Bypass
      (Bypassr.f ())
      get_transport
(*
    bypass_disable_r := (fun () -> Transport.disable bypass_trans) ;
*)

  in
  (s,h)

let _ = Layer.install name l

(**************************************************************)
