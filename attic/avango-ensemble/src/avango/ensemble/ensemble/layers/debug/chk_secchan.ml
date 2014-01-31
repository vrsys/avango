(**************************************************************)
(* CHK_SECCHAN.ML *)
(* Author: Ohad Rodeh, 3/99 *)
(* Tests for the secchan/mngchan layers *)
(**************************************************************)
open Layer
open Trans
open Util
open View
open Event
(**************************************************************)
let name = Trace.filel "CHK_SECCHAN"
(**************************************************************)

type header = 
  | ChanList of rank list 
  | Len of Buf.len
  | NoHdr

type state = {
  nonce               : int ;
  ack                 : bool array ;
  mutable first_time  : bool;
  prog                : string ;
  mutable blocked     : bool ;
  mutable recv        : int ;
  chans               : rank list array
}

type msg = Out of int | In of int | String of string

let init s (ls,vs) = {
  nonce       = ls.rank ;
  ack         = Array.init ls.nmembers (function i -> i=ls.rank);
  first_time  = true ;
  prog        = Param.string vs.params "chk_secchan_prog" ;
  blocked     = false ;
  recv        = 0 ;
  chans       = if ls.rank = 0 then Array.create ls.nmembers [] else Array.create 0 []
}

let marshal, unmarshal = Buf.make_marsh "CHK_SECCHAN" 
let check_all a = array_for_all ident a 
let log ls = Trace.log2 name (string_of_int ls.rank)
let log1 ls = Trace.log2 (name^"1") (string_of_int ls.rank)
  
(**************************************************************)
  (* Pad a message to be a multiply of 8 bytes. return the new 
   * message, and the actual length of the message.
   *)
let pad_msg m = 
  let len = Buf.length m in 
  let len = Buf.int_of_len len in
  if len mod 8 <> 0 then 
    let ceil  = ((len /8 ) + 1 ) * 8 in
    let fill = ceil - len in
    let m = Buf.append m (Buf.create (Buf.len_of_int fill)) in
    m,(Buf.len_of_int len)
  else
    m,(Buf.len_of_int len)

(* Strip the padding from the message. 
 *)
let strip_pad m actual = Buf.sub m Buf.len0 actual 


let sendSecureMsg dnlm peer m = 
  let m,actual = pad_msg (marshal m) in
  dnlm (create name ESecureMsg [Peer peer; SecureMsg m]) (Len actual)

module type TEST = sig
  val timer : state -> View.full -> (Event.t -> header -> unit) -> Time.t -> unit
  val handle_secure_msg : state -> View.full -> (Event.t -> header -> unit) ->  msg -> rank -> unit
end
  
module N_to_n : TEST = struct
  let send_all s (ls,vs) dnlm  = 
    if not s.blocked then (
      log ls (fun () -> sprintf "sending %d" s.nonce);
      for i=0 to (pred ls.nmembers) do 
	if ls.rank <> i then 
	  sendSecureMsg dnlm i (Out s.nonce)
      done;
    )

  let timer s (ls,vs) dnlm t = 
    if s.first_time && ls.nmembers > 1 then (
      s.first_time <- false ;
      send_all s (ls,vs) dnlm ;
    )
    
  (* Echo received messages 
   *)
  let handle_secure_msg s (ls,vs) dnlm m src = 
    match m with 
    | Out r -> 
	log1 ls (fun () -> sprintf "Got [%d] msg=%d" src r);
	sendSecureMsg dnlm src (In r)
    | In r -> 
	if r = s.nonce then (
	  log1 ls (fun () -> sprintf "Echo from %d -- OK" src);
	  s.ack.(src) <- true ;
	  if (check_all s.ack) then (
	    Array.fill s.ack 0 (Array.length s.ack) false;
	    s.ack.(0) <- true;
	    log ls (fun () -> "All OK");
	    send_all s (ls,vs) dnlm
	  )
	) else 
	  failwith (sprintf "Echo from %d [val=%d]" src r) 
    | _ -> failwith "N_n, bad message type"
end
  
module One_to_n : TEST = struct
  let send_all s (ls,vs) dnlm  = 
    assert(ls.rank=0);
    log ls (fun () -> sprintf "sending %d" s.nonce);
    for i=1 to (pred ls.nmembers) do 
      sendSecureMsg dnlm i (Out s.nonce) 
    done;
    ()

  let timer s (ls,vs) dnlm t = 
    if s.first_time && ls.nmembers > 1 && ls.rank = 0 then (
      s.first_time <- false ;
      send_all s (ls,vs) dnlm
    )

  (* Echo received messages 
   *)
  let handle_secure_msg s (ls,vs) dnlm m src = 
    match m with 
    | Out r -> 
	log1 ls (fun () -> sprintf "Got [%d] msg=%d" src r);
	sendSecureMsg dnlm src (In r) 
    | In r -> 
	if r = s.nonce then (
	  log1 ls (fun () -> sprintf "Echo from %d -- OK" src);
	  s.ack.(src) <- true ;
	  if (check_all s.ack) then (
	    Array.fill s.ack 0 (Array.length s.ack) false;
	    s.ack.(0) <- true;
	    log ls (fun () -> "All OK");
	    send_all s (ls,vs) dnlm
	  )
	) else 
	  failwith (sprintf "Echo from %d [val=%d]" src r) 
    | _ -> failwith "N_n, bad message type"
end

module Ping : TEST = struct
  let hello = String "hello world"

  let timer s (ls,vs) dnlm t = 
    if s.first_time && ls.nmembers > 1 then (
      log ls (fun () -> "ping");
      s.first_time <- false;
      match ls.rank with 
	| 0 -> sendSecureMsg dnlm 1 hello
	| _ -> ()
    )

  let handle_secure_msg s (ls,vs) dnlm m src = 
    if m <> hello then failwith "bad message";
    match src,ls.rank with 
      | 1,0 -> sendSecureMsg dnlm 1 hello
      | 0,1 -> sendSecureMsg dnlm 0 hello
      | _ -> failwith "Ping, bad message destination"
end

(**************************************************************)
  
  
let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let log = Trace.log2 name ls.name in
  let log2 = Trace.log2 (name^"2") ls.name in
  let failwith m = failwith ("("^name^":"^m^")") in
  let endpt_of_rank r = Endpt.string_of_id_short (Arrayf.get vs.view r) in
  let timer = match s.prog with 
    | "ping" -> Ping.timer
    | "n_n"  -> N_to_n.timer
    | "1_n"  -> One_to_n.timer
    | _ -> failwith "program not supported"
  and handle_secure_msg = match s.prog with
    | "ping" -> Ping.handle_secure_msg
    | "n_n"  -> N_to_n.handle_secure_msg
    | "1_n"  -> One_to_n.handle_secure_msg
    | _ -> failwith "program not supported"
  in

  let check_recv_all () = 
    if s.recv = ls.nmembers then (
      log2 (fun () -> "received all");
      let result = ref true in
      Array.iteri (fun i l -> 
	if not (List.for_all (fun r -> List.mem i s.chans.(r)) l) then
	  result := false
      ) s.chans;
     if not !result then (
	eprintf "%s\n" (string_of_array (fun l -> string_of_list endpt_of_rank l ) s.chans);
	flush stdout;
	failwith "Non-symmetric channels"
      )
    )
  in
      
  let up_hdlr ev abv hdr = match getType ev,hdr with
    | _ -> up ev abv
	  
  and uplm_hdlr ev hdr = 
    match getType ev,hdr with
      (* Echo received messages *)
    | ESecureMsg,Len actual -> 
	if not s.blocked then (
	  let src = getPeer ev in
	  let m = getSecureMsg ev in
	  let m = strip_pad m actual in 
	  let m = unmarshal m Buf.len0 in
	  handle_secure_msg s (ls,vs) dnlm m src
	);
	free name ev

    | ESend iovl, ChanList l -> 
	if not s.blocked then (
	  s.recv <- succ s.recv ;
	  let peer = getPeer ev in
	  s.chans.(peer) <- l ;
	  check_recv_all ()
	);
	Iovecl.free iovl

    | _ -> failwith unknown_local
		
  and upnm_hdlr ev = match getType ev with
    | ETimer -> 
	log (fun () -> "timer");
	timer s (ls,vs) dnlm (getTime ev) ;
	upnm ev	

    | ESecChannelList -> 
	(* log2 (fun () -> "ESecChannelList");*)
	if not s.blocked then (
	  let l = getSecChannelList ev in
	  if ls.rank = 0 then (
	    s.recv <- succ s.recv ;
	    s.chans.(0) <- l
	  ) else
	    dnlm (sendPeer name 0) (ChanList l);
	);
	free name ev

    | _ -> upnm ev
	  
  and dn_hdlr ev abv = dn ev abv NoHdr
	  
  and dnnm_hdlr ev = match getType ev with
    | EBlock -> 
	s.blocked <- true;
	dnnm ev

    | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = 
  Param.default "chk_secchan_prog" (Param.String "ping") ;
  Layer.install name l

(**************************************************************)




