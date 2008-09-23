(**************************************************************)
(* RATE.ML : rate based flow control *)
(* Author: Takako M. Hickey, 2/96 *)
(**************************************************************)
open Event
open Util
open Layer
open Trans
open Time
open View
(**************************************************************)
let name = Trace.filel "RATE"
(**************************************************************)


(**************************************************************)

type 'abv state = {
  (* rate related variables *)
  mutable rate_n	: int ;		(* nmsgs allowed to sent per rate interval *)
  mutable rate_t	: Time.t ;	(* rate interval *)
  mutable rate		: Time.t ;	(* rate *)

  mutable start		: Time.t ;	(* start of this rate interval *)
  mutable nsent		: int ;		(* nmsgs sent in this rate interval *)
  mutable next_interval	: Time.t ;

  mutable sweep		: Time.t ;	(* sweep interval *)
  mutable next_sweep	: Time.t ;	(* next sweep time *)

  buffer		: ('abv * Iovecl.t) Queue.t(* msgs yet to be casted *)
}

(**************************************************************)

(* TODO: test out if this val is appropriate.
 *)
(* minimum sweep interval = 1 ms *)
let min_sweep = Time.of_string "0.001"

let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  eprintf "RATE:dump\n" ;
  sprintf "rate_n=%d, rate_t=%f, rate=%f\n"
    s.rate_n
    (Time.to_float s.rate_t)
    (Time.to_float s.rate) ;
  sprintf "start=%f, nsent=%d"
    (Time.to_float s.start)
    s.nsent ;
  sprintf "sweep=%f\n"
    (Time.to_float s.sweep)
|] )

let init (rate_n,rate_t) vs =
  let intermsg	= div rate_t (of_int rate_n) in {
    (* Rate related variables *)
    rate_n	= rate_n ;
    rate_t	= rate_t ;
    rate	= div (of_int rate_n) rate_t ;
    next_interval = Time.zero ;
    start	= Time.zero ;
    nsent	= 0 ;

    (* Set sweep time to be the larger of the minimum sweep time or
     * the inter message sent time if rate was kept perfectly even.
     *)
    sweep	=
    	if intermsg < min_sweep then min_sweep
    	else intermsg ;

    next_sweep	= Time.zero ;
    buffer      = Queue.create ()
  }

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let up_hdlr ev abv () = up ev abv
  and uplm_hdlr ev hdr = failwith "bad uplm event"

  and upnm_hdlr ev = match getType ev with

    (* EInit: request a first timer alarm to bootstrap things.
     *)
  | EInit ->
      dnnm (timerAlarm name Time.zero) ;
      upnm ev

    (* ETimer: every rate interval
     *   send a message
     *)
  | ETimer -> (
      let time = getTime ev in
      let bufsize = Queue.length s.buffer in
      let interval = sub time s.start in

      (* Decide # msgs to send on this sweep so that rate is kept
       * within this interval.
       *
       * TODO: make this more efficient.  e.g., binary search.
       *)
      let rec find_ntosend n =
        if bufsize = n then
          n
        else if div (of_int (s.nsent + n + 1)) interval > s.rate then
          n
        else
          find_ntosend (n + 1)
      in

      if time >= s.next_sweep then (
        (* Request next sweep.
         *)
        s.next_sweep <- add time s.sweep ;
        dnnm (timerAlarm name s.next_sweep) ;

        (* If interval is up, reset interval realted values.
         *)
        if time >= s.next_interval then (
          s.next_interval <- add time s.rate_t ;
          s.start <- time ;
	  s.nsent <- 0
        ) ;

        let ntosend = (find_ntosend 0) in
(* eprintf "RATE: sweep ntosend %d\n" ntosend;*)

        (* Send a buffered msgs.
         *)
	for i = 1 to ntosend do
          let (abv,iov) = Queue.take s.buffer in

(* eprintf "RATE: casting\n";*)
          dn (castIov name iov) abv () ;
	  s.nsent <- succ s.nsent
        done
      ) ;
      upnm ev
    )

  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev

  and dn_hdlr ev abv = match getType ev with
  | ECast -> 
      let iov = getIov ev in
      let iov = Iovecl.copy name iov in
      Queue.add (abv,iov) s.buffer ;
      free name ev
  | _ -> dn ev abv ()

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

(**************************************************************)
