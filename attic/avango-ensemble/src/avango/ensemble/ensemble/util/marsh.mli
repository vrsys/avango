(**************************************************************)
(* MARSH.MLI *)
(* Author: Mark Hayden, 12/96 *)
(* Modified by Ohad Rodeh, 10/2001 *)
(**************************************************************)
(* This now provides by hand marshaling for strings and integers.
 * The usage is for C applications that need to communicate
 * through an agreed protocol with ML. *)
(**************************************************************)
open Trans
open Buf
(**************************************************************)

(* Type of marshalled objects: used at sender.
 *)
type marsh

(* Initialize a marshaller.
 *)
val marsh_init : unit -> marsh

(* Convert a marshaller into a buffer.
 *)
val marsh_done : marsh -> Buf.t

(* Functions for adding data to a string.
 *)
val write_int    : marsh -> int -> unit
val write_bool   : marsh -> bool -> unit
val write_string : marsh -> string -> unit
val write_buf    : marsh -> Buf.t -> unit
val write_list   : marsh -> ('a -> unit) -> 'a list -> unit
val write_array  : marsh -> ('a -> unit) -> 'a array -> unit
val write_option : marsh -> ('a -> unit) -> 'a option -> unit

(**************************************************************)

(* Type of unmarshalling objects: used at receiver.
 *)
type unmarsh

(* This exception is raised if there is a problem
 * unmarshalling the message.  
 *)
exception Error of string

(* Convert a buffer into an unmarsh object.
 *)
val unmarsh_init : Buf.t -> Buf.ofs -> unmarsh

val unmarsh_check_done_all : unmarsh -> unit (* we should have read it all *)

(* Functions for reading from marshalled objects.
 *)
val read_int    : unmarsh -> int
val read_bool   : unmarsh -> bool
val read_string : unmarsh -> string
val read_buf    : unmarsh -> Buf.t
val read_list   : unmarsh -> (unit -> 'a) -> 'a list
val read_option : unmarsh -> (unit -> 'a) -> 'a option

(**************************************************************)
