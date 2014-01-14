(**************************************************************)
(* PGP: interface to PGP *)
(* Authors: Zhen Xiao, Mark Hayden, 4/97 *)
(* Ohad: Added computation in the background  *)
(*       Fixed "pgp strange-character" syndrome *)
(*       Added support for different versions of PGP *)
(**************************************************************)
type principal   = string
type sender      = principal 
type receiver    = principal 
type cipher_text = string
type clear_text  = string 

(* sign the clear_text with the sender's secret key.
 *)
val sign : sender -> receiver -> clear_text -> cipher_text option
val bckgr_sign : sender -> receiver -> clear_text -> Alarm.t -> (cipher_text option -> unit) -> unit


(* verify the cipher_text. 
 *)
val verify : sender -> receiver -> cipher_text -> clear_text option
val bckgr_verify : sender -> receiver -> cipher_text -> Alarm.t -> (clear_text option -> unit) -> unit 

