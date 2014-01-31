(**************************************************************)
(* APPL_HANDLE.MLI *)
(* Author: Mark Hayden, 10/97 *)
(**************************************************************)
open Trans
open Util
(**************************************************************)

(* BUG: handle and handle_gen should be opaque.
 *)
type handle     = { mutable endpt : Endpt.id ; mutable rank : int }
type origin 	= handle
type rank       = handle
type dests 	= handle array
type handle_gen = unit

(**************************************************************)

type ('cast_msg, 'send_msg) action =
  | Cast of 'cast_msg
  | Send of dests * 'send_msg
  | Control of Appl_intf.control

(**************************************************************)

(* Create a new handle give a handle generator and endpoint.
 *)
val handle : handle_gen -> debug -> Endpt.id -> handle

(* Check if handle is valid in a view.
 *)
val is_valid : handle -> View.state -> bool

val handle_hack : debug -> Endpt.id -> handle

val string_of_handle : handle -> string

val endpt_of_handle : handle -> Endpt.id

val handle_eq : handle -> handle -> bool

(**************************************************************)

module Old : sig
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
    block_view          : View.full -> (Endpt.id * 'merg_msg) list ;
    block_install_view  : View.full -> 'merg_msg list -> 'view_msg ;
    unblock_view 	: View.full -> 'view_msg ->
      ('cast_msg,'send_msg) action array ;

    exit 		: unit -> unit
  }

  (*val f : ('a,'b,'c,'d) full -> handle_gen * ('a,'b,'c,'d) Appl_intf.Old.full*)
end

(**************************************************************)

module New : sig
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

  (*val f : 'msg full -> handle_gen * 'msg Appl_intf.New.full*)
end

(**************************************************************)
