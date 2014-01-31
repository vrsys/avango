(**************************************************************)
(* ROUTE.MLI *)
(* Author: Mark Hayden, 12/95 *)
(**************************************************************)
open Trans
open Buf
(**************************************************************)

type pre_processor =
  | Unsigned of (rank -> Obj.t option -> seqno -> Iovecl.t -> unit)
  | Signed of (bool -> rank -> Obj.t option -> seqno -> Iovecl.t -> unit)

(**************************************************************)

type handlers
val handlers : unit -> handlers

(* The central delivery functions of Ensemble.  All incoming
 * messages go through here. the format is: (len,buf,iovec).
 * buf - a preallocated ML buffer.
 * iovec - a user-land iovec.
 *)
val deliver : handlers -> Buf.t -> Buf.ofs -> Buf.len -> Iovecl.t -> unit
(**************************************************************)

(* transmit an Ensemble packet, this includes the ML part, and a
 * user-land iovecl.
 *)
type xmitf = Buf.t -> Buf.ofs -> Buf.len -> Iovecl.t -> unit

(* Type of routers. ['xf] is the type of a message send function. 
 *)
type 'xf t

val debug : 'xf t -> debug
val secure : 'xf t -> bool
val blast : 'xf t -> xmitf -> Security.key -> Conn.id -> 'xf
val install : 'xf t -> handlers -> Conn.key -> Conn.recv_info list -> 
    Security.key -> (Conn.kind -> bool -> 'xf) -> unit
val remove : 'xf t -> handlers -> Conn.key -> Conn.recv_info list -> unit

(**************************************************************)

(* Security enforcement.
 *)
val set_secure : unit -> unit

val security_check : 'a t -> unit

(**************************************************************)

type id =
  | UnsignedId
  | SignedId

val id_of_pre_processor : pre_processor -> id

(**************************************************************)

(* Create a router.
 *)
val create :
  debug ->
  bool -> (* secure? *)
  ((bool -> 'xf) -> pre_processor) ->
  (Conn.id -> Buf.t) ->			(* packer *)
  ((Conn.id * digest * Security.key * pre_processor) Arrayf.t -> 
    (Buf.t -> Buf.ofs -> Buf.len -> Iovecl.t -> unit) Arrayf.t) -> (* merge *)
  (xmitf -> Security.key -> digest -> Conn.id -> 'xf) ->  (* blast *)
  'xf t

(**************************************************************)

(* Logging functions.  Called from various places.
 *)
val drop : (unit -> string) -> unit (* For tracing message drops: ROUTED *)
val info : (unit -> string) -> unit (* For tracing connection ids: ROUTEI *)

(**************************************************************)
(* Helper functions for the various routers.
 *)
val merge1 : ('a -> unit) Arrayf.t -> 'a -> unit
val merge2 : ('a -> 'b -> unit) Arrayf.t -> 'a -> 'b -> unit
val merge3 : ('a -> 'b -> 'c -> unit) Arrayf.t -> 'a -> 'b -> 'c -> unit
val merge4 : ('a -> 'b -> 'c -> 'd -> unit) Arrayf.t -> 'a -> 'b -> 'c -> 'd -> unit

(* See comments in code.
 *)
val group : ('a * 'b) Arrayf.t -> ('a * 'b Arrayf.t) Arrayf.t
val merge1iov : (Iovecl.t -> unit) Arrayf.t -> Iovecl.t -> unit
val merge2iov : ('a -> Iovecl.t -> unit) Arrayf.t -> 'a -> Iovecl.t -> unit
val merge2iovr : (Iovecl.t -> 'a -> unit) Arrayf.t -> Iovecl.t -> 'a -> unit
val merge3iov : ('a -> 'b -> Iovecl.t -> unit) Arrayf.t -> 'a -> 'b -> Iovecl.t -> unit
val merge4iov : ('a -> 'b -> 'c -> Iovecl.t -> unit) Arrayf.t -> 'a -> 'b -> 'c -> Iovecl.t -> unit

(**************************************************************)

val pack_of_conn : Conn.id -> Buf.t

(**************************************************************)
