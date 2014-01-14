(**************************************************************)
(* HOT_APPL.MLI *)
(* Authors: Alexey Vaysburd, Mark Hayden 11/96 *)
(**************************************************************)
open Ensemble
open Trans
open Protos
(**************************************************************)

(* Types of C application's downcalls to Ensemble.  The
 * 'unit's are to force Ocaml to not use constant tags
 * in the representation.
 *)
type c_dncall = 
  | C_Join of Hot_util.join_options 
  | C_Cast of Iovec.raw
  | C_Send of endpt * Iovec.raw
  | C_Suspect of endpt array 
  | C_XferDone of unit
  | C_Protocol of string
  | C_Properties of string
  | C_Leave of unit
  | C_Prompt of unit
  | C_Rekey of unit
  | C_BlockOk of unit
  | C_Void of unit

type context

val context : debug -> context

val get : context -> id -> c_dncall -> unit

val join : 
  context ->
  id ->					(* group ctx id *)
  (unit -> bool) ->			(* get downcalls *)
  View.full ->				(* initial view state *)
  Appl_intf.New.t ->			(* application interface *)
  unit

