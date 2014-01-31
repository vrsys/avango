(**************************************************************)
(* APPL_DEBUG.MLI: debug application interface *)
(* Author: Mark Hayden, 11/97 *)
(**************************************************************)
open Trans

val f : debug -> 'msg Appl_intf.New.full -> 'msg Appl_intf.New.full
val view : debug -> 'msg Appl_intf.New.full -> 'msg Appl_intf.New.full

(**************************************************************)
