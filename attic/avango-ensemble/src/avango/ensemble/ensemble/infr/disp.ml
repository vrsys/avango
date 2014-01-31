(**************************************************************)
(* DISP.ML *)
(* Author: Mark Hayden, 6/95 *)
(* Based on code by Werner Vogels *)
(**************************************************************)
open Layer
open Util
open View
open Event
(**************************************************************)
let name = Trace.file "DISP"
(**************************************************************)

let l _ (ls,vs) = ((),fun {up_lout=up;dn_lout=dn} ->
  let msg = sprintf "%s{%s}:" name (Endpt.string_of_id ls.endpt) in

  let up_hdlr ev hdr =
    if getType ev <> ETimer then (
      eprintf "%sUp(%s)\n" msg (to_string ev) ;
    ) ;
    up ev hdr
  and dn_hdlr ev hdr =
    if getType ev <> ETimer then (
      eprintf "%sDn(%s)\n" msg (to_string ev) ;
    ) ;
    dn ev hdr
  in {up_lin=up_hdlr;dn_lin=dn_hdlr})

let _ = Layer.install "DISP" l

(**************************************************************)
