(**************************************************************)
(* HANDLER.MLI: handler tables *)
(* Author: Mark Hayden, 12/95 *)
(**************************************************************)

type hash = int
type ('key,'data,'merge) t

val create	: ('data Arrayf.t -> 'merge) -> ((unit -> 'merge) -> 'merge) -> ('key,'data,'merge) t
val add 	: ('key,'data,'merge) t -> hash -> 'key -> 'data -> unit
val remove 	: ('key,'data,'merge) t -> hash -> 'key -> unit
val find 	: ('key,'data,'merge) t -> hash -> 'merge
val size 	: ('key,'data,'merge) t -> int
val info	: ('key,'data,'merge) t -> string
val to_list     : ('key,'data,'merge) t -> ('key * 'data) list
