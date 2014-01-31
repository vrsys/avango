(**************************************************************)
(* ALARM.ML *)
(* Author: Mark Hayden, 4/96 *)
(**************************************************************)
open Util
open Trans
(**************************************************************)
let name = Trace.file "ALARM"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
(**************************************************************)

type gorp = Unique.t * Sched.t * Async.t * Route.handlers

type alarm = {
  disable : unit -> unit ;
  schedule : Time.t -> unit
}

type poll_type = SocksPolls | OnlyPolls

type t = {
  name		: string ;		(* name *)
  gettime	: unit -> Time.t ;	(* get current time *)
  alarm		: (Time.t -> unit) -> alarm ;
  check		: unit -> bool ;	(* check the timers *)
  min		: unit -> Time.t ;	(* get next timer *)
  add_sock_recv	: debug -> Hsys.socket -> Hsys.handler -> unit ;
  rmv_sock_recv	: Hsys.socket -> unit ;
  add_sock_xmit : debug -> Hsys.socket -> (unit->unit) -> unit ;
  rmv_sock_xmit : Hsys.socket -> unit ;
  block		: unit -> unit ;	(* block until next alarm *)
  add_poll	: debug -> (bool -> bool) -> unit ;
  rmv_poll	: debug -> unit ;
  poll		: poll_type -> unit -> bool ;
  handlers      : Route.handlers ;
  sched         : Sched.t ;
  unique        : Unique.t ;
  async         : Async.t ;
  local_xmits   : Route.xmitf
}

(**************************************************************)

let local_xmits sched handlers debug =
  let x buf ofs len iovl = 
    Sched.enqueue_5arg sched debug Route.deliver handlers buf ofs len iovl
  in
  x

(**************************************************************)

let c_alarm dis sch = {
  disable = dis ;
  schedule = sch
}

let disable a = a.disable ()
let schedule a = a.schedule

let create
  name
  gettime
  alarm
  check
  min
  add_sock_recv
  rmv_sock_recv
  add_sock_xmit
  rmv_sock_xmit
  block
  add_poll
  rmv_poll
  poll
  (unique,sched,async,handlers)
= {
    name	= name ;
    gettime     = gettime ;
    check       = check ;
    alarm       = alarm ;
    min         = min ;
    add_sock_recv = add_sock_recv ;
    rmv_sock_recv = rmv_sock_recv ;
    add_sock_xmit = add_sock_xmit ;
    rmv_sock_xmit = rmv_sock_xmit ;
    add_poll	= add_poll ;
    rmv_poll	= rmv_poll ;
    poll	= poll ;
    block	= block ;
    handlers    = handlers ;
    sched       = sched ;
    async       = async ;
    unique      = unique ;
    local_xmits = (local_xmits sched handlers name)
  }

(**************************************************************)

let gettime t = t.gettime ()
let alarm t = t.alarm
let check t = t.check
let min t = t.min ()
let add_sock_recv t = t.add_sock_recv
let rmv_sock_recv t = t.rmv_sock_recv
let add_sock_xmit t = t.add_sock_xmit
let rmv_sock_xmit t = t.rmv_sock_xmit
let block t = t.block ()
let add_poll t = t.add_poll
let rmv_poll t = t.rmv_poll
let poll t = t.poll
let name t = t.name
let handlers t = t.handlers
let sched t = t.sched
let unique t = t.unique
let async t = t.async
let local_xmits t = t.local_xmits

(**************************************************************)

let poll_f = ref None

let update_polls polls =
  let pa = Resource.to_array polls in
  let n = Arrayf.length pa in
  match n with
  | 0 -> poll_f := None
  | 1 -> poll_f := Some (Arrayf.get pa 0)
  | 2 ->
      let p0 = Arrayf.get pa 0 in
      let p1 = Arrayf.get pa 1 in
      poll_f := Some (fun b -> p1 (p0 b))
  | 3 ->
      let p0 = Arrayf.get pa 0 in
      let p1 = Arrayf.get pa 1 in
      let p2 = Arrayf.get pa 2 in
      poll_f := Some (fun b -> p2 (p1 (p0 b)))
  | _ ->
      let rec loop i rm =
	if i >= n then rm else (
	  loop (succ i) ((Arrayf.get pa i) rm)
        ) 
      in
      poll_f := Some (fun b -> loop 0 b)

let polls =
  Resource.create "ALARM:polls" 
    ignore2
    ignore2
    update_polls
    ignore

let alm_add_poll name poll = 
  Resource.add polls name name poll ;
  !poll_f

let alm_rmv_poll name =
  Resource.remove polls name ;
  !poll_f

(*
let alm_poll b = 
  match !poll_f with
  | Some f -> f b
  | None -> b
*)

(**************************************************************)

(* For bypass stuff.
 *)
let wrap wrapper alarm =
  let enable callback =
    alarm.alarm (wrapper callback)
  in { alarm with alarm = enable }

(**************************************************************)

(* Management of alarm tables.
*)

type id = string

let alarms = ref []

let install id a =
  alarms := (id,a) :: !alarms

let chosen = ref None

let choose alarm gorp =
  let alarm = string_uppercase alarm in
  let a = 
    try List.assoc alarm !alarms with Not_found ->
      eprintf "ELINK:alarm %s not installed, exiting" alarm ;
      exit 1
  in
  match !chosen with
  | Some a -> a
  | None ->
      let a = a gorp in
      chosen := Some a ;
      a

let get_hack () = 
  match !chosen with
  | None -> failwith "no alarm chosen"
  | Some a -> a

(**************************************************************)

let install_port port =
  log (fun () -> sprintf "setting Unique port to %d" port) ;
  let alarm = get_hack () in
  let unique = unique alarm in
  Unique.set_port unique port

(**************************************************************)
