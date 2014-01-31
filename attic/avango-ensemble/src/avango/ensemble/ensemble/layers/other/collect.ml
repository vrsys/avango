(**************************************************************)
(* Collect.ML *)
(* Collect layer. Provide vsync infomation for the total ordering *)
(* protocols. Used with Partial_appl layer.                       *)
(* Author: Xiaoming Liu with Robbert vanRenesse 2/98 *)
(**************************************************************)
open Trans
open Layer
open Util
open View
open Event
(**************************************************************)
let name = Trace.filel "COLLECT"
(**************************************************************)

type header = NoHdr
  | NonAppl
  | VsyncMsg

type blocking = 
  | Unblocked
  | DnBlocked
  | UpBlocking
  | UpBlocked

let string_of_blocking = function
  | Unblocked  -> "Unblocked"
  | DnBlocked  -> "DnBlocked"
  | UpBlocking -> "UpBlocking"
  | UpBlocked  -> "UpBlocked"

type state = {
  send_xmit             : seqno array ;
  send_recv             : seqno array ;
  cast_recv             : seqno array ;
  mutable cast_xmit     : seqno ;
  mutable got_expect    : bool ;
  mutable cast_expect   : seqno Arrayf.t ;
  mutable send_expect   : seqno Arrayf.t ;
  mutable failed        : bool Arrayf.t ;
  mutable blocking      : blocking
} 

(**************************************************************)

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "got_expect=%b\n" s.got_expect ;
  sprintf "blocking=%s\n" (string_of_blocking s.blocking) ;
  sprintf "cast_recv=%s\n" (string_of_int_array s.cast_recv) ;
  sprintf "cast_expect=%s\n" (Arrayf.int_to_string s.cast_expect) ;
  sprintf "send_xmit  =%s\n" (string_of_int_array s.send_xmit) ;
  sprintf "send_recv  =%s\n" (string_of_int_array s.send_recv) ;
  sprintf "send_expect=%s\n" (Arrayf.int_to_string s.send_expect)
|])

(**************************************************************)

let init _ (ls,vs) = { 
  got_expect  = false ;
  blocking    = Unblocked ;
  cast_xmit   = 0 ;
  cast_recv   = Array.create ls.nmembers 0 ;
  send_xmit   = Array.create ls.nmembers 0 ;
  send_recv   = Array.create ls.nmembers 0 ;
  cast_expect = ls.zeroes ;
  send_expect = ls.zeroes ;
  failed      = ls.falses
} 

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in
  let logs = Trace.log3 Layer.syncing ls.name name in

  let check_block_ok () =
    if s.blocking = UpBlocking
    && s.got_expect
    then  (
      let all_ok = ref true in
      for rank = 0 to pred ls.nmembers do
	if rank <> ls.rank then (
	  if s.cast_recv.(rank) < Arrayf.get s.cast_expect rank then
	    all_ok := false ;
	  if (not (Arrayf.get s.failed rank))
	  && s.send_recv.(rank) < Arrayf.get s.send_expect rank
	  then 
	    all_ok := false
	)
      done ;
      if !all_ok then (
	logs (fun () -> sprintf "release EBlockOk (cast=%s)" (string_of_int_array s.cast_recv)) ;
	s.blocking <- UpBlocked ;
	upnm (create name EBlockOk[])
      )
    )
  in

  let up_hdlr ev abv hdr = match getType ev,hdr with
  (* Application cast.
   *)
  | ECast _,NoHdr ->			
      let origin = getPeer ev in
      array_incr s.cast_recv origin ;
      if s.got_expect
      && s.cast_recv.(origin) > Arrayf.get s.cast_expect origin
      then
	failwith ("bad cast:"^(Event.to_string ev)) ;
      up ev abv ;
      if s.blocking = UpBlocking then
	check_block_ok ()

  (* Application send.
   *)
  | ESend _,NoHdr ->			
      let origin = getPeer ev in
      array_incr s.send_recv origin ;
      if s.got_expect
      && s.send_recv.(origin) > Arrayf.get s.send_expect origin
      then
	failwith ("bad send:"^(Event.to_string ev)) ;
      up ev abv ;
      if s.blocking = UpBlocking then
	check_block_ok ()

  (* Some control messages need to be sent with a VS guanranty
   *)
    | ECast _, VsyncMsg -> 
	let origin = getPeer ev in
	array_incr s.cast_recv origin ;
	up ev abv ;
	if s.blocking = UpBlocking then
	  check_block_ok ()
	
    | ESend _, VsyncMsg -> 
	let origin = getPeer ev in
	array_incr s.send_recv origin;
	up ev abv ;
	if s.blocking = UpBlocking then
	  check_block_ok ()

  | (ECast _|ESend _), NonAppl
  | _, NoHdr -> up ev abv
  | _        -> failwith bad_header
     
  and uplm_hdlr ev () = failwith "got uplm event"

  and upnm_hdlr ev = match getType ev with
  | EBlock | EFail ->
      if getType ev = EBlock then
	s.blocking <- DnBlocked ;
      let cast_recv = Arrayf.of_array s.cast_recv in
      let cast_recv =
	Arrayf.fset cast_recv ls.rank s.cast_xmit
      in

      logs (fun () -> sprintf "EBlock|EFail cast_recv=%s"
	(Arrayf.int_to_string cast_recv)) ;

      let ev = set name ev [
        ApplCasts cast_recv ;
	ApplSends (Arrayf.of_array s.send_xmit)
      ] in
      upnm ev ;
      check_block_ok ()

  | ESyncInfo ->
      logs (fun () -> "got ESyncInfo") ;
      s.got_expect <- true ;
      s.cast_expect <- getApplCasts ev ;
      s.send_expect <- getApplSends ev ;
      check_block_ok () ;
      upnm ev

  | EBlockOk -> 
      (*assert (s.blocking = DnBlocked) ;*)
      if not (s.blocking = DnBlocked) then failwith "(s.blocking = DnBlocked)";
      s.blocking <- UpBlocking ;
      check_block_ok ()

  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | ECast _ ->
      if getApplMsg ev then (
	(*assert (s.blocking = Unblocked) ;*)
	if not (s.blocking = Unblocked) then failwith "(s.blocking = Unblocked)";
	s.cast_xmit <- succ s.cast_xmit ;
	dn ev abv NoHdr
      ) else (
	if getForceVsync ev then (
	  assert (s.blocking = Unblocked) ;
	  s.cast_xmit <- succ s.cast_xmit ;
	  dn ev abv VsyncMsg
	) else
	  dn ev abv NonAppl
      )

  | ESend _ ->
      if getApplMsg ev then (
	assert (s.blocking = Unblocked) ;
	array_incr s.send_xmit (getPeer ev) ;
	dn ev abv NoHdr
      ) else (
	if getForceVsync ev then (
	  assert (s.blocking = Unblocked) ;
	  array_incr s.send_xmit (getPeer ev) ;
	  dn ev abv VsyncMsg
	) else
	  dn ev abv NonAppl
      )

  | _ -> dn ev abv NoHdr

  and dnnm_hdlr ev = dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = Layer.install name l

(**************************************************************)
