(**************************************************************)
(* MCREDIT.MLI *)
(* Author: Mark Hayden, Zhen Xiao, 3/97 *)
(**************************************************************)
open Trans

type credit = int

type t

(* [create rank nmembers credit send_credit recv_credit]
 * initializes a credit info for member [rank] with initial
 * send_credit of [send_credit] and recv_credit of
 * [recv_credit].  
 *)
val create : rank -> nmembers -> credit -> credit array -> credit array -> t

(* [got_credit t rank] is called when I receive a credit
 * message from member [rank].  
 *)
val got_credit : t -> rank -> unit

(* [got_msg t rank cr] is called when I receive a message
 * of worth [cr] credit from member [rank]. It returns the
 * total amount of credits received from that member so far.
 *)
val got_msg : t -> rank -> credit -> credit

(* [set_credit t rank int] is called to reset the
 * recv_credit when I have acknowledged previous credits
 * from member [rank].  
 *) 
val set_credit : t -> rank -> credit -> unit

(* [fail t rank] is called when I am notified the failure of 
 * member [rank]
 *)
val fail : t -> rank -> unit

(* [check t] is called to check if I can send more stuff.
 *)
val check : t -> bool 

(* Ohad. How much credit do I have?
*)
val left : t -> credit

(* [take t credit] is called to remove [credit] credits from all
 * members.  The credit record is returned for the functional
 * optimizations.
 *)
val take : t -> credit -> t

(* [clear t] is called to periodically clear the send_credit
 * array and the msg_len so that they won't overflow.  
 *)
val clear : t -> unit

(* [to_string_list t] prints out information.
 *)
val to_string_list : t -> string list
