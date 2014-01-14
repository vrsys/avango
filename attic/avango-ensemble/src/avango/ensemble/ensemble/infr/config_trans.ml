(**************************************************************)
(* CONFIG_TRANS.ML *)
(* Author: Mark Hayden, 10/96 *)
(**************************************************************)
open Trans
open Buf
open Util
open Layer
open View
(**************************************************************)
let name = Trace.file "CONFIG_TRANS"
let failwith s = Trace.make_failwith name s
let log = Trace.log name 
(**************************************************************)
let name_send = "CONFIG_TRANS(send)"
let name_cast = "CONFIG_TRANS(cast)"
let name_recv = "CONFIG_TRANS(recv)"
let name_goss = "CONFIG_TRANS(goss)"
(**************************************************************)

let gossip_marsh () =
  let pack,unpack = Util.make_magic () in
  let pack_gossip ev =
    let exchange = Event.getExtender
      (function Event.ExchangeGos(a) -> (Some (Some (a))) | _ -> None)
      None ev
    in

    let switch = Event.getExtender
      (function Event.SwitchGos(a,b,c) -> (Some (Some (a,b,c))) | _ -> None)
      None ev
    in

    let heal = Event.getExtender
      (function Event.HealGos(a,b,c,d,e) -> (Some (Some (a,b,c,d,e))) | _ -> None)
      None ev
    in

    let merge = Event.getExtender
      (function Event.MergeGos(a,b,c,d) -> (Some (Some (a,b,c,d))) | _ -> None)
      None ev
    in

    let dbg_name = Event.getExtender
      (function Event.DbgName name -> (Some (Some name)) | _ -> None)
      None ev
    in

    match exchange,switch,heal,merge,dbg_name with
    | None,None,None,None,None -> 
	None
    | _ ->
	Some (pack (exchange,switch,heal,merge,dbg_name))


  and unpack_gossip secure mlobj =
    let exchange,switch,heal,merge,dbg_name =
      match mlobj with 
	  None -> 
	    (None,None,None,None,None)
	| Some buf -> 
	    if not (
	      Obj.is_block buf
	      && Obj.tag buf = 0
	      && Obj.size buf = 5
	    ) then (
	      eprintf "CONFIG_TRANS:unpack_gossip: not a correct block (tag=%d, size=%d is_block=%b) (continuing)\n" (Obj.tag buf) (Obj.size buf) (Obj.is_block buf);
		(None,None,None,None,None)
	    ) else
	      let incorrect mo = 
		eprintf "CONFIG_TRANS:unpack_gossip: not a correct exchange block (tag=%d, size=%d is_block=%b) (continuing)\n" 
		  (Obj.tag mo) (Obj.size mo) (Obj.is_block mo);
		None
	      in
	      let incorrect_int mo = 
		eprintf "CONFIG_TRANS:unpack_gossip: not a correct exchange block: is_int (continuing)\n";
		None
	      in

	      (* exchange is supposed to be a 
	       *        Some(string) value : string option.
	       * We need to decompose it through first opening the option
	       * and then the string.
	       *)
	      let exchange = Obj.field buf 0 in
	      let exchange = 
		(* Check the option
		 *)
		if Obj.is_int exchange then 	
		  (* This is a none value.
		   *)
		  None
		else
		  if not (
		    Obj.is_block exchange 
		    && Obj.size exchange = 1
		    && Obj.tag exchange = 0) then 
		    incorrect exchange
		  else
		    (* Check the string
		     *)
		    let exchange = Obj.field exchange 0 in
		    if Obj.is_int exchange then 	
		      incorrect_int exchange
		    else
		      if not (
			Obj.is_block exchange
			&& Obj.tag exchange = Obj.string_tag) then
			incorrect exchange
		      else
			(* All is well, a safe case is in order.
			 *)
			let exchange = (Obj.magic exchange : string) in
			Some exchange
	      in
	      
	      (* Only read the other fields if things are secure.
	       *)
	      let switch,heal,merge,dbg_name = 
		if secure then (
		  let buf = unpack buf in
		  let (_,switch,heal,merge,dbg_name) = buf in
		  (switch,heal,merge,dbg_name)
		) else (None,None,None,None)
	      in
	      
	      (exchange,switch,heal,merge,dbg_name)
    in
    
    let exchange = option_map (fun (a)         -> Event.ExchangeGos(a)    ) exchange in
    let switch   = option_map (fun (a,b,c)     -> Event.SwitchGos(a,b,c)  ) switch in
    let heal     = option_map (fun (a,b,c,d,e) -> Event.HealGos(a,b,c,d,e)) heal in
    let merge    = option_map (fun (a,b,c,d)   -> Event.MergeGos(a,b,c,d) ) merge in
    let dbg_name = option_map (fun (a)         -> Event.DbgName(a)        ) dbg_name in
    let fields = filter_nones [exchange;switch;heal;merge;dbg_name] in
    match fields with
      | [] -> None
      | _ ->
	  let ev = Event.create name Event.EGossipExt fields in
	  Some ev

  in pack_gossip,unpack_gossip

(**************************************************************)

let f alarm ranking bot_nomsg ((ls,vs) as vf) up_hdlr =
  let pack,unpack = Util.make_magic () in
  let pack_gossip,unpack_gossip = gossip_marsh () in

  (* OR support for 
   *)
  let sec_sim = lazy (Param.bool vs.params "sec_sim") in

  let schedule = Alarm.alarm alarm
    (fun time -> up_hdlr (Event.timerTime name time) bot_nomsg)
  in

  let async_disable = 
    Async.add (Alarm.async alarm) ls.async (fun () ->
      up_hdlr (Event.create name Event.EAsync[
	(Event.Time (Alarm.gettime alarm))
      ]) bot_nomsg
    )
  in

  let router =
    if vs.key = Security.NoKey then Unsigned.f () else Signed.f ()
  in

  let (gossip_gossip,gossip_disable) =
    if ls.am_coord then (
      let receive _ secure =
	let handler _ mlobj _ _ =
	  match unpack_gossip secure mlobj with
	      None -> ()
	    | Some ev -> up_hdlr ev bot_nomsg
	in handler
      in

      let trans = Transport.f alarm ranking vf Stack_id.Gossip router receive in

      let gossip = Transport.gossip trans in
      let disable () = Transport.disable trans in
      (gossip,disable)
    ) else (
      let gossip _ _ _ _ _ = failwith "non-coord gossipping" ; () in
      (gossip,ident)
    )
  in

  let (primary_cast,primary_send,primary_disable) =
    let receive kind secure =
      if secure then (
	let handler rank msg seqno iov =
	  let msg = match msg with
	  | None -> Local_seqno seqno	(*MEM:alloc*)
	  | Some d -> unpack d
	  in
	  match kind with 
	    | Conn.Send  -> up_hdlr (Event.sendPeerIov "ESend" rank iov) msg
	    | Conn.Cast  -> up_hdlr (Event.castPeerIov "ECast" rank iov) msg
	in handler
      ) else (
	fun _ _ _ _ ->
	  Route.drop (fun () -> "CONFIG_TRANS:insecure message being dropped")
      )
    in

    let trans = Transport.f alarm ranking vf Stack_id.Primary router receive in

    let cast = Transport.cast trans in
    let send = Transport.send trans in
    let disable () = Transport.disable trans in
    (cast,send,disable)
  in
  let sends = Array.create ls.nmembers None in
  
  let dn ev msg = match Event.getType ev with
  | Event.ECastUnrel iov
  | Event.ECast iov -> (
      (* Note: we do not free the event because
       * the cast operation will take the iov reference.
       *)
      match msg with
      | Local_seqno seqno -> 
	  primary_cast ls.rank None seqno iov
      | _ ->
	  primary_cast ls.rank (Some (pack(msg))) (-1) iov 
    )
  | Event.ESendUnrel iov
  | Event.ESend iov -> (
      (* Compute the send function if not done already.
       * Note: we do not free the event because
       * the send operation will take the iov reference.
       *)
      let send =
      	let rank = Event.getPeer ev in
	match sends.(rank) with
      	| Some send -> send
      	| None ->
	    let send = primary_send rank in
	    sends.(rank) <- Some(send) ;
	    send
      in

      match msg with
      | Local_seqno seqno -> 
	  send ls.rank None seqno iov
      | _ ->
	  send ls.rank (Some (pack(msg))) (-1) iov 
    )
  | Event.EExit -> 
      primary_disable () ;
      gossip_disable () ;
      async_disable ()
  | Event.ETimer -> 
      Alarm.schedule schedule (Event.getAlarm ev)
  | Event.EGossipExt ->
      begin
	match pack_gossip ev with
	| None -> ()
	| Some _ as s_buf ->
	    gossip_gossip None (-1) s_buf (-1) Iovecl.empty
      end ;
      Event.free name ev

  | Event.EGossipExtDir ->
      let dest = Event.getAddress ev in
      begin
	match pack_gossip ev with
	| None -> ()
	| Some _ as s_buf ->
	    gossip_gossip (Some dest) (-1) s_buf (-1) Iovecl.empty
      end ;
      Event.free name ev

  | Event.EAuth -> (
      log (fun () -> "EAuth");
      let dst_addr,op = Event.getAuthData ev in
      match op with 
	| Auth.Clear None -> failwith "Can't sign None clear_text"
	| Auth.Clear (Some clear_text) -> 
	    if Sys.os_type = "Unix" then 
	      Auth.bckgr_ticket (Lazy.force sec_sim) ls.addr dst_addr clear_text alarm 
  		(function cipher_text -> 
		  up_hdlr (Event.create name Event.EAuth [
		    Event.AuthData (dst_addr, Auth.Ticket cipher_text)]
		  ) msg)
	    else
	      let cipher_text = Auth.ticket (Lazy.force sec_sim) ls.addr dst_addr clear_text  in
	      up_hdlr (Event.create name Event.EAuth [
		    Event.AuthData (dst_addr, Auth.Ticket cipher_text)]
	      ) msg
	      
	| Auth.Ticket None -> failwith "Can't sign a None ticket"
	| Auth.Ticket (Some cipher_text) -> 
	    if Sys.os_type = "Unix" then 
	      Auth.bckgr_check (Lazy.force sec_sim) ls.addr dst_addr cipher_text alarm
		(function clear_text -> 
		  up_hdlr (Event.create name Event.EAuth [
		    Event.AuthData (dst_addr, Auth.Clear clear_text)
		  ]) msg) 
	    else
	      let clear_text = Auth.check (Lazy.force sec_sim) ls.addr dst_addr cipher_text in
	      up_hdlr (Event.create name Event.EAuth [
		Event.AuthData (dst_addr, Auth.Clear clear_text)
	      ]) msg 
    )	      

  | _ -> failwith "sanity:bad dn"
  in

  dn

(**************************************************************)
