(**************************************************************)
(* PROTOS.MLI *)
(* Authors: Mark Hayden, Alexey Vaysburd 11/96 *)
(* Cleanup by Ohad Rodeh 11/2001, removed direct dynamic linking *)
(* support. Hopefully Caml will support it natively.*)
(**************************************************************)
open Trans
open Appl_intf open New
(**************************************************************)

type id = int
type 'a dncall_gen = ('a,'a) action
    
type 'a upcall_gen = 
  | UInstall of View.full
  | UReceive of origin * cast_or_send * 'a
  | UHeartbeat of Time.t
  | UBlock
  | UExit
      
type endpt = string
    
type 'a message_gen =
  | Create of id * View.state * Time.t * (Addr.id list)
  | Upcall of id * 'a upcall_gen
  | Dncall of id * 'a dncall_gen
  | SendEndpt of id * endpt * 'a
  | SuspectEndpts of id * endpt list
      
type message = Iovecl.t message_gen
type dncall = Iovecl.t dncall_gen
type upcall = Iovecl.t upcall_gen
    
val string_of_message : 'a message_gen -> string

val string_of_dncall : 'a dncall_gen -> string

val string_of_upcall : 'a upcall_gen -> string

(**************************************************************)

(* The type of a protos connection.
 *)
type config = View.full -> Layer.state -> unit

(**************************************************************)

(* Generate a message marshaller.
 * marshal a message into an ML buffer, and an iovecl.
  *)
val make_marsh :
  unit -> 
    (message -> Buf.t * Iovecl.t) *
    (Buf.t * Iovecl.t -> message)

(* Handle a new client connection.
 *)
val server :
  Alarm.t ->
  (Addr.id list -> Addr.set) ->
  (Appl_intf.New.t -> View.full -> unit) ->
  debug -> (message -> unit) ->
  ((message -> unit) * (unit -> unit) * unit)

(* Create a client instance given message channels.
 *)
val client :
  Alarm.t -> 
  debug -> (message -> unit) ->
  ((message -> unit) * (unit -> unit) * config)

(**************************************************************)
