(**************************************************************)
(* IPMC.MLI *)
(* Author: Mark Hayden, 5/95 *)
(**************************************************************)
open Trans
open Hsys
(**************************************************************)

(* ADDR: returns a valid (Class D) IP multicast address.
 *)
val addr  : unit -> inet


(* JOIN: Join a group on some port.
 *)
val join  : inet -> port -> socket


(* LEAVE: Leave a previously joined group.  Note: the file
 * descriptor returned here may be closed so you shouldn't
 * pass it on to anyone else, just remove it from your
 * tables.  
 *)
val leave : inet -> port -> socket

(**************************************************************)
