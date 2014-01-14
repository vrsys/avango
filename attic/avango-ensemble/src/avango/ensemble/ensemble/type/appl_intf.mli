(**************************************************************)
(* APPL_INTF.ML: application interface *)
(* Author: Mark Hayden, 8/95 *)
(* See documentation for a description of this interface *)
(**************************************************************)
open Trans
open Util
(**************************************************************)

(* Some type aliases.
 *)
type dests = rank array

(**************************************************************)
(* ACTION: The type of actions an application can take.
 * These are typically returned from callbacks as lists of
 * actions to be taken.

 * [NOTE: XferDone, Protocol, Rekey, and Migrate are not
 * implemented by all protocol stacks.  Dump and Block are
 * undocumented actions used for internal Ensemble
 * services.]

 * Cast(msg): Broadcast a message to entire group

 * Send(dests,msg): Send a message to subset of group.

 * Leave: Leave the group.

 * Prompt: Prompt for a view change.

 * Suspect: Tell the protocol stack about suspected failures
 * in the group.

 * XferDone: Mark end of a state transfer.

 * Protocol(protocol): Request a protocol switch.  Will
 * cause a view change to new protocol stack.

 * Migrate(addresses): Change address list for this member.
 * Will cause a view change.

 * Rekey: Request that the group be rekeyed.

 * Timeout(time): Request a timeout at a particular time.
 * The timeout takes the form of a heartbeat callback.
 * Currently not supported.

 * Dump: Undefined (for debugging purposes)

 * Block: Controls synchronization of view changes.  Not
 * for casual use.
 *)
type control =
  | Leave
  | Prompt
  | Suspect of rank list

  | XferDone
  | Rekey of bool 
  | Protocol of Proto.id
  | Migrate of Addr.set
  | Timeout of Time.t

  | Dump
  | Block of bool
  | No_op

type ('cast_msg,'send_msg) action =
  | Cast of 'cast_msg
  | Send of dests * 'send_msg
  | Send1 of rank * 'send_msg
  | Control of control

val string_of_control : control -> string
val string_of_action : ('a,'b) action -> string

val action_array_map : 
  ('a -> 'b) -> ('c -> 'd) -> 
    ('a,'c) action array -> ('b,'d) action array

(**************************************************************)

(* This is a new version of the application interface.
 * We will be slowly transitioning to it because it
 * provides better performance and is often easier to
 * use.
 * 
 * OR: As of version 1.3, only the new interface is supported.
 *)

module New : sig
  type cast_or_send = C | S
  type blocked = U | B

  type 'msg naction = ('msg,'msg) action

  type 'msg handlers = {
    flow_block : rank option * bool -> unit ;
    block : unit -> 'msg naction array ;
    heartbeat : Time.t -> 'msg naction array ;
    receive : origin -> blocked -> cast_or_send -> 'msg -> 'msg naction array ;
    disable : unit -> unit
  } 

  type 'msg full = {
    heartbeat_rate : Time.t ;
    install : View.full -> ('msg naction array) * ('msg handlers) ;
    exit : unit -> unit
  } 

  type t = Iovecl.t full
  type 'msg power = ('msg * Iovecl.t) full

  val string_of_blocked : blocked -> string
  val string_of_cs : cast_or_send -> string

  (* A Null handler for applications.
   *)
  val null : 'msg -> 'msg naction array

  val full : 'msg full -> t

  val failed_handlers : 'msg handlers
end

type appl_lwe_header =
  | LCast of origin
  | LSend of origin * rank array

type appl_multi_header =
  int

(**************************************************************)
