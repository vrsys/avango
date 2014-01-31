(**************************************************************)
(* RESOURCE.MLI *)
(* Author: Mark Hayden, 4/96 *)
(**************************************************************)
open Trans
(**************************************************************)

type ('a,'b) t

val create : 
  name -> 
  ('a -> 'b -> unit) ->			(* add *)
  ('a -> 'b -> unit) ->			(* remove *)
  (('a,'b) t -> unit) ->		(* change *)
  (('a,'b) t -> unit) ->		(* minimal change *)
  ('a,'b) t

val simple 	: name -> ('a,'b) t
val add       	: ('a,'b) t -> debug -> 'a -> 'b -> unit
val remove 	: ('a,'b) t -> 'a -> unit

val to_array	: ('a,'b) t -> 'b Arrayf.t

val info	: ('a,'b) t -> string
val to_string   : ('a,'b) t -> string
