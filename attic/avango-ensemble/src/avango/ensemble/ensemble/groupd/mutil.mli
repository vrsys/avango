(**************************************************************)
(* MUTIL.MLI *)
(* Author: Mark Hayden, 8/97 *)
(* Designed with Roy Friedman *)
(**************************************************************)
open Trans
(**************************************************************)

type endpt = Buf.t
type group = Buf.t

(**************************************************************)

(* MEMBER_MSG: messages from a member to the membership service.
 * These messages are all prefixed with the Endpt.id and Group.id
 * of the member sending the message.

 * Join: request to join the group.  Replied with a View message.

 * Synced: this member is synchronized.  Is a reply to a Sync
 * message.  Will be replied with a View message.

 * Fail(endpt list): remove these members from the group.
 *) 
type member_msg =
  | Join of ltime
  | Synced
  | Fail of endpt list
   
(**************************************************************)

(* COORD_MSG: messages from the service to a member.  These
 * messages are all prefixed with the Group.id of the group
 * for which these messages are being sent.

 * View(view,ltime): a new view is being installed.  The
 * view is a list of Endpt.id's.  A member who just sent a
 * Join message may not be included in the view, in which
 * case it should await the next View message.  The ltime is
 * the logical time of the view.  The first entry in the
 * view and the ltime uniquely identify the view.  The
 * ltime's a member sees grow monotonicly.

 * Sync: all members should "synchronize" (usually this means
 * waiting for messages to stabilize) and then reply with
 * a SyncOk message.

 * Failed: a member is being reported as having failed.
 * This is done because members may need to know about
 * failures in order to determine when they are
 * synchronized.
 *)
type coord_msg =
  | View of ltime * primary * (endpt list)
  | Sync
  | Failed of endpt list

(**************************************************************)

(* Wrap a groupd client to check that it follows
 * the published state machine transitions correctly.
 *)
val wrap :  
  ((member_msg -> unit) -> (coord_msg -> unit)) ->
  ((member_msg -> unit) -> (coord_msg -> unit))

(**************************************************************)

val string_of_endpt : endpt -> string

(**************************************************************)

val string_of_member_msg : member_msg -> string

val string_of_coord_msg : coord_msg -> string

(**************************************************************)
val set_string_of_endpt : (endpt -> string) -> unit
(**************************************************************)
