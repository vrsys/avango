(**************************************************************)
(* APPL_HANDLE.ML *)
(* Author: Mark Hayden, 10/97 *)
(**************************************************************)
open Trans
open Util
open View
(**************************************************************)
let name = Trace.file "APPL_HANDLE"
let failwith = Trace.make_failwith name
let log = Trace.log name
(**************************************************************)

type handle = { mutable endpt : Endpt.id ; mutable rank : int }
type handle_gen = unit
type origin 	= handle
type rank       = handle
type dests 	= handle array

(**************************************************************)

type ('cast_msg, 'send_msg) action =
  | Cast of 'cast_msg
  | Send of dests * 'send_msg
  | Control of Appl_intf.control

(**************************************************************)

let handle _ debug endpt = { 
  endpt = endpt ;
  rank = 0
}

let handle_hack = handle ()

let is_valid handle vs =
  Arrayf.mem handle.endpt vs.View.view

let endpt_of_handle handle = handle.endpt

let string_of_handle h =
  sprintf "{HANDLE:endpt=%s}" (Endpt.string_of_id h.endpt)

let handle_eq h h' =
  h.endpt = h'.endpt

(**************************************************************)

module Old = struct

type (
  'cast_msg,
  'send_msg,
  'merg_msg,
  'view_msg
) full = {
  recv_cast 		: origin -> 'cast_msg ->
    ('cast_msg,'send_msg) action array ;

  recv_send 		: origin -> 'send_msg ->
    ('cast_msg,'send_msg) action array ;

  heartbeat_rate	: Time.t ;

  heartbeat 		: Time.t ->
    ('cast_msg,'send_msg) action array ;

  block 		: unit ->
    ('cast_msg,'send_msg) action array ;

  block_recv_cast 	: origin -> 'cast_msg -> unit ;
  block_recv_send 	: origin -> 'send_msg -> unit ;
  block_view            : View.full -> (Endpt.id * 'merg_msg) list ;
  block_install_view    : View.full -> 'merg_msg list -> 'view_msg ;
  unblock_view 		: View.full -> 'view_msg ->
    ('cast_msg,'send_msg) action array ;

  exit 			: unit -> unit
}

end

(**************************************************************)

module New = struct
  type cast_or_send = Appl_intf.New.cast_or_send = C | S
  type blocked = Appl_intf.New.blocked = U | B
  type 'msg naction = ('msg,'msg) action

  type 'msg handlers = {
    block : unit -> 'msg naction array ;
    heartbeat : Time.t -> 'msg naction array ;
    receive : origin -> blocked -> cast_or_send -> 'msg -> 'msg naction array ;
    disable : unit -> unit
  } 

  type 'msg full = {
    heartbeat_rate : Time.t ;
    install : View.full -> handle Arrayf.t -> ('msg naction array) * ('msg handlers) ;
    exit : unit -> unit
  } 
end

(**************************************************************)
