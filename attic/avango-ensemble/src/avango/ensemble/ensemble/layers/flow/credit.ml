(**************************************************************)
(* CREDIT.ML : credit based flow control *)
(* Author: Takako M. Hickey, 2/96 *)
(**************************************************************)
open Event
open Util
open Layer
open Trans
open View
(**************************************************************)
let name = Trace.filel "CREDIT"
(**************************************************************)

type 'a state = {
  (* credit related variables *)
  mutable rtotal	: int ;		(* total resource *)
  mutable rremain	: int ;		(* unallocated resources *)

  mutable ntogive	: int array;	(* # credit to give at a time *)
  mutable whentogive	: int array;	(* give more credit when this # credit remaning *)
  mutable pntogive	: int array;	(* # credit to piggyback at a time *)
  mutable pwhentogive	: int array;	(* piggyback more credit when this # credit remaning *)
  crgiven		: int array;	(* credit given to other members *)

  mutable ntoask	: int ;		(* # credit to ask at a time *)
  mutable whentoask	: int ;		(* ask for more credit when this # of credit remaining *)
  mutable pntoask	: int ;		(* # credit to ask at a time *)
  mutable pwhentoask	: int ;		(* ask for more credit when this # of credit remaining *)
  credit		: int array ;	(* credit given by other members *)

  buffer		: 'a Queue.t ;	(* msgs yet to be casted *)
  mutable nbuffered	: int ;		(* # msgs in buffer *)
  sweep         	: Time.t ;
  mutable next_sweep 	: Time.t ;

  (* Statistics *)
  mutable msent_data	: int ;		(* # data msgs sent *)
  mutable nblocked	: int ;		(* # data msgs blocked *)
  msent			: int array ;	(* # credit reply sent *)
  mrcvd			: int array ;	(* # credit reply rcvd *)
  csent 		: int array 	(* # credit given via reply *)
}

(**************************************************************)

type header = NoHdr
| CR_Request of int * int * int * int	(* ntogive, whentogive *)
| CR_One of int 		(* ncredit *)
| CR_Many of (int array)		(* ncredit for multiple members *)

(* How credit was given *)
let ct_piggy = 0
let ct_ind = 1
let ct_timeout = 2

(**************************************************************)

let dump (ls,vs) s =
  eprintf "CREDIT:dump (nothing)\n"

let print_array array =
  Array.map (function (i) -> eprintf "%d " i) array ;
  eprintf "\n"

let init (rtotal,ntoask,whentoask,pntoask,pwhentoask,sweep) (ls,vs) =
  let nmembers= ls.nmembers in
  let gwhen = if nmembers > 1 then rtotal / (nmembers - 1) else 0
  in {
    (* Credit related variables *)
    rtotal	= rtotal ;
    rremain	= rtotal ;

    ntogive	= array_create name nmembers 0 ;
    whentogive	= array_create name nmembers 0 ;
    pntogive	= array_create name nmembers 0 ;
    pwhentogive	= array_create name nmembers 0 ;
    crgiven	= array_create name nmembers 0 ;

    ntoask	= ntoask ;
    whentoask	= whentoask;
    pntoask	= pntoask ;
    pwhentoask	= pwhentoask;
    credit	= array_create name nmembers 0 ;

    buffer      = Queue.create () ;
    nbuffered   = 0 ;
    sweep       = sweep ;
    next_sweep  = Time.zero ;

    (* Statistics *)
    msent_data	= 0 ;
    nblocked	= 0 ;
    msent	= array_create name 3 0 ;
    mrcvd	= array_create name 3 0 ;
    csent	= array_create name 3 0
  }

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let ack = make_acker name dnnm in

  let print_stat () = (
    eprintf "CR msent_data: %d\n" s.msent_data ;
(*    eprintf "CR nblocked: %d\n" s.nblocked ; *)
    eprintf "CR msent piggy: %d\n" s.msent.(ct_piggy) ;
    eprintf "CR msent ind: %d\n" s.msent.(ct_ind) ;
    eprintf "CR msent timeout: %d\n" s.msent.(ct_timeout) ;
    eprintf "CR csent piggy: %d\n" s.csent.(ct_piggy) ;
    eprintf "CR csent ind: %d\n" s.csent.(ct_ind) ;
    eprintf "CR csent timeout: %d\n" s.csent.(ct_timeout) ;
    eprintf "CR mrcvd piggy: %d\n" s.mrcvd.(ct_piggy) ;
    eprintf "CR mrcvd ind: %d\n" s.mrcvd.(ct_ind) ;
    eprintf "CR mrcvd timeout: %d\n" s.mrcvd.(ct_timeout)
  )


  (* If enough resource remaining, give out credit to potential sender.
   *)
  and give_credit sender style = (
    let ntogive = s.ntogive.(sender)
    and whentogive = s.whentogive.(sender)
    and pntogive = s.pntogive.(sender)
    and pwhentogive = s.pwhentogive.(sender)
    and crgiven = s.crgiven.(sender)
    and remain = s.rremain

    and send_credit n = (
      s.rremain <- s.rremain - n ;
      s.crgiven.(sender) <- s.crgiven.(sender) + n ;
      s.csent.(style) <- s.csent.(style) + n ;
      n
    )
    in

    (* The following allows piggyback of credits whenever it can.
     * This makes more credits to be sent piggy, but also causes a
     * large number of individual credit messages via ECast event.
     *)
(*
    if remain > 0 & ntogive > 0
	& (style = ct_piggy or crgiven <= whentogive) then (
      if s.rremain >= ntogive then
        send_credit ntogive
      else
        send_credit remain
    )
*)

    (* The following sends credits only if senders are almost
     * out of credits, piggy or not piggy.  This causes most of
     * credits not to be piggybacked via ECast event, however,
     * the number of indiviual credit messages stay relatively constant.
     *)
(*
    if remain > 0 & ntogive > 0 & crgiven <= whentogive then (
      if s.rremain >= ntogive then
        send_credit ntogive
      else
        send_credit remain
    )
*)

    if remain > 0 & ntogive > 0
	& ((style = ct_piggy & crgiven <= pwhentogive)
	or crgiven <= whentogive) then (
      if style = ct_piggy & s.rremain >= pntogive then
        send_credit pntogive
      else if (not (style = ct_piggy)) & s.rremain >= ntogive then
        send_credit ntogive
      else
        send_credit remain
    )

    else
      0
  )
  in

  (* Give out credit to all members.
   *)
  let give_gcredit style = (
    let crarray = array_create name ls.nmembers 0
    and max = ref 0 in

    for i = 0 to pred ls.nmembers do
      if i <> ls.rank then (
        let cr = (give_credit i style) in
        crarray.(i) <- cr ;
        if cr > !max then
          max := cr
      )
    done ;
    if !max > 0 then (
(*
eprintf "giving arrayor credit from %d: " vs.ran;
print_array crarray ;
*)
      CR_Many(crarray)
    )
    else
      NoHdr
  )

  (* Find minimum of credit given by all other members.
   *)
  and avail_credit () = (
    let rec check_credit answer n =
      if n >= ls.nmembers then
        answer
      else if (not (n = ls.rank)) & (s.credit.(n) < answer) then
        check_credit s.credit.(n) (n + 1)
      else
        check_credit answer (n + 1)
    in
      if ls.rank = 0 then (
        if ls.nmembers > 1 then
          check_credit s.credit.(1) 2
        else
          0
      )
      else
        check_credit s.credit.(0) 1
  )

  (* Use c credits.
   *)
  and use_credit c = (
    let rec use c i =
      if i >= ls.nmembers then ()
      else (
        if (not (i = ls.rank)) then
          s.credit.(i) <- s.credit.(i) - c ;
        use c (i + 1)
      )
    in use c 0
  )
  in

  let do_cast msg = (
      use_credit 1 ;
      let hdr = (give_gcredit ct_piggy) in
      ( match hdr with
        | CR_Many _  ->
           s.msent.(ct_piggy) <- s.msent.(ct_piggy) + 1
        | _  -> ()
      ) ;
      dn (castEv name) msg hdr
(*     dn (castEv name) msg NoHdr *)
  )

  and cast_buffered ntosend = (
(*
    let last = ref (Socket.gettimeofday ())
    and t = ref 0.0 in
    if ntosend > 0 then
      eprintf "sending %d buffered msgs\n" ntosend ;
*)

    for i = 1 to ntosend do
      let msg = Queue.take s.buffer in

      if i = 1 then (
        let hdr = (give_gcredit ct_piggy) in
        ( match hdr with
          | CR_Many _  ->
             s.msent.(ct_piggy) <- s.msent.(ct_piggy) + 1
          | _  -> ()
        ) ;
        dn (castEv name) msg hdr
      )
      else
        dn (castEv name) msg NoHdr

(*
      t := Socket.gettimeofday () ;
      sprintf "%d: %f\n" i (!t -. !last) ;
      last := !t ;
*)
    done ;

    use_credit ntosend ;
    s.nbuffered <- s.nbuffered - ntosend
  )
  in

  let got_credit receiver ngiven = (
    s.credit.(receiver) <- s.credit.(receiver) + ngiven ;
(*eprintf "CR: got %d credits from %d (cr total: %d)\n" ngiven receiver s.credit.(receiver) ;*)

    let cr = (avail_credit ())
    and bufsize = Queue.length s.buffer in
    let ntosend = if (cr < bufsize) then cr else bufsize
    in

    cast_buffered ntosend
(*eprintf "CR: sent %d buffered msg (nbuffered: %d rremain: %d)\n" ntosend s.nbuffered s.rremain*)
  )
  in


  let up_hdlr ev abv hdr = match getType ev with
  | ECast -> (
      (* reclaim credit *)
      s.rremain <- s.rremain + 1 ;
      s.crgiven.((getPeer ev)) <- s.crgiven.((getPeer ev)) - 1 ;
(* eprintf "CR: reclaiming credit (rremain: %d)\n" s.rremain ;*)

      ( match hdr with
        | CR_One(ngiven)  -> (
            s.mrcvd.(ct_piggy) <- s.mrcvd.(ct_piggy) + 1 ;
            got_credit (getPeer ev) ngiven
          )
        | CR_Many(crvec)  -> (
            s.mrcvd.(ct_piggy) <- s.mrcvd.(ct_piggy) + 1 ;
            got_credit (getPeer ev) crvec.(ls.rank)
          )
        | _ -> ()
      ) ;

      let cr = (avail_credit ())
      and bufsize = Queue.length s.buffer in
      let ntosend = if (cr < bufsize) then cr else bufsize in
      if ntosend > 0 then
        cast_buffered ntosend
      else (
        let n = (give_credit (getPeer ev) ct_ind) in
        if n > 0 then (
          s.msent.(ct_ind) <- s.msent.(ct_ind) + 1 ;
          dnlm (sendPeer name (getPeer ev)) (CR_One(n))
        )
      ) ;
      up ev abv
    )

  | _ ->
      up ev abv

  and upnm_hdlr ev = match getType ev with
  | EInit -> (
      (* Ask for some credit *)
      dnlm (castEv name) (CR_Request(s.ntoask,s.whentoask,s.pntoask,s.pwhentoask)) ;

      (* Schedule periodic credit giving *)
      dnnm (timerAlarm name Time.zero) ;
      upnm ev
    )

  | ETimer -> (
      let time = getTime ev in
      (* Request next credit giving *)
      if time >= s.next_sweep then (
        s.next_sweep <- Time.add time s.sweep ;
        dnnm (timerAlarm name s.next_sweep) ;
(*
        for i = 0 to pred ls.nmembers do
          if not (i = ls.rank) then (
            let n = (give_credit i ct_ind) in
            if n > 0 then (
              s.msent.(ct_ind) <- s.msent.(ct_ind) + 1 ;
              dnlm (sendPeer name i) (CR_One(n))
            )
          )
        done
*)
        let hdr = (give_gcredit ct_timeout) in
        ( match hdr with
        | CR_Many(crarray) -> (
            s.msent.(ct_timeout) <- s.msent.(ct_timeout) + 1 ;
            dnlm (castEv name) hdr
        )
        | _ -> ())
      ) ;
      upnm ev
    )

  | EDump -> ( dump vf s ; upnm ev )

  | EExit -> (
      print_stat () ;
      upnm ev
    )
  | _ -> upnm ev

  and uplm_hdlr ev hdr = (match getType ev,hdr with
  | (ECast|ESend), CR_Request(ntogive, whentogive, pntogive, pwhentogive) -> (
      (* Save credit giving policy *)
      s.ntogive.((getPeer ev)) <- ntogive ;
      s.whentogive.((getPeer ev)) <- whentogive ;

      s.pntogive.((getPeer ev)) <- pntogive ;
      s.pwhentogive.((getPeer ev)) <- pwhentogive ;

      let n = (give_credit (getPeer ev) ct_ind) in
        if n > 0 then (
          s.msent.(ct_ind) <- s.msent.(ct_ind) + 1 ;
          dnlm (sendPeer name (getPeer ev)) (CR_One(n))
        )
    )
  | (ECast|ESend), CR_One(ngiven) -> (
      s.mrcvd.(ct_ind) <- s.mrcvd.(ct_ind) + 1 ;
      got_credit (getPeer ev) ngiven
    )
  | (ECast|ESend), CR_Many(crvec) -> (
      s.mrcvd.(ct_timeout) <- s.mrcvd.(ct_timeout) + 1 ;
      got_credit (getPeer ev) crvec.(ls.rank)
    )
  | _ -> failwith unknown_local) ;
  ack ev

  and dn_hdlr ev abv = match getType ev with
  | ECast -> (
      (* If enough credit, send the msg.  Otherwise, buffer msg.
       *)
      let cr = (avail_credit ()) in
      if cr > 0 then (
(*eprintf "CR: casting msg. cr left: %d\n" (cr - 1);*)
         do_cast abv
      )
      else (
        Queue.add abv s.buffer ;
        s.nbuffered <- s.nbuffered + 1 ;
        s.nblocked <- s.nblocked + 1
(*eprintf "CR: buffering msg.  (nbuffered: %d)\n" s.nbuffered*)
      ) ;

      s.msent_data <- s.msent_data + 1
    )
  | _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

(**************************************************************)
