(**************************************************************)
(* TRANS.MLI *)
(* Author: Mark Hayden, 12/95 *)
(**************************************************************)

type mux	= int
type port	= int
type incarn	= int
type vci	= int
type rank	= int
type nmembers   = int
type data_int	= int
type seqno	= int
type ltime      = int
type bitfield	= int
type fanout     = int
type name       = string
type debug      = string
type inet       = Unix.inet_addr
type origin     = rank
type primary    = bool
type pid        = int

module type OrderedType =
  sig
    type t
    val zero : t
    (*val ge : t -> t -> bool*)
    val ge : t -> t -> bool
    val cmp : t -> t -> int
  end

(**************************************************************)
