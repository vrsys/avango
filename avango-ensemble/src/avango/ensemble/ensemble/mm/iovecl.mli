(**************************************************************)
(* IOVECL.MLI: operations on arrays of Iovec's. *)
(* Author: Mark Hayden, 3/95 *)
(* Completely rewritten by Ohad Rodeh 9/2001 *)
(**************************************************************)
open Trans
open Buf
(**************************************************************)
(* Type of Iovec arrays.
 *)
type t 

val to_arrayf : t -> Iovec.t Arrayf.t
val of_iovec_arrayf : Iovec.t Arrayf.t -> t
val of_iovec_array : Iovec.t array -> t
val to_iovec_array : t -> Iovec.t array
val to_iovec_raw_array : t -> Iovec.raw array
val of_iovec_raw_array : Iovec.raw array -> t
(**************************************************************)

(* refcount operation to all member iovecs.  See Iovec.mli
 *)
val free : t -> unit
val copy : t -> t

(**************************************************************)

(* Calculate total length of an array of iovecs.
 *)
val len : t -> len

(* Flatten an iovec array into a single iovec.  Copying only
 * occurs if the array has more than 1 non-empty iovec.
 * The reference count is incremented if this is a singleton
 * array.
 *)
val flatten : t -> Iovec.t
val flatten_w_copy : t -> Iovec.t

(* Catenate an array of iovec arrays. Used in conjunction with 
 * flatten.
 *)
val concata : t Arrayf.t -> t


(* Make a marshaller for arrays of iovecs.  
 * 
 * PERF: there is an extra copy here that should be 
 * eliminated. This effects ML applications.
 * 
 * The boolean flags describes whether to free the iovecl's
 * after unmarshaling it into an ML value.
 *)
val make_marsh : bool -> (('a -> t) * (t -> 'a))

(* An empty iovec array.
 *)
val empty : t
val is_empty : t -> bool

(* Wrap a normal iovec.  Takes the reference count.
 *)
val of_iovec : Iovec.t -> t

(* Append two iovec arrays.
 *)
val append : t -> t -> t

(* Prepend a single iovec to an iovec array.
 *)
val prependi : Iovec.t -> t -> t
val appendi : t -> Iovec.t -> t

(* This is used for reading Iovecls from left to right.
 * The loc objects prevent the sub operation from O(n^2)
 * behavior when repeatedly sub'ing an Iovecl.  loc0 is
 * then beginning of the iovecl.
 * 
 * The reference counts of all iovec's that are in the 
 * sub-vector are incremented. 
 *)
type loc = int * ofs (* position [ofs] in the i-th iovec *)
val loc0 : loc
val sub_scan : loc -> t -> ofs -> len -> t * loc

val sub : t -> ofs -> len -> t 

(* Fragment iovec array into an array of iovec arrays of
 * some maximum size.  No copying occurs.  The resulting
 * iovec arrays are at most the specified length.
 *)
val fragment : len -> t -> t Arrayf.t

(* Check if iovecl has only one Iovec.  If it does,
 * Get_singleton will extract it. 
 * get_singleton fails if there is more than one
 * iovec.  
 *)
val singleton : Iovec.t -> t
val is_singleton : t -> bool
val get_singleton : t -> Iovec.t
val count_nonempty : t -> int

(**************************************************************)
(* Summerize the number of references to each part of the iovec.
*)
val sum_refs : t -> string
(**************************************************************)
  
