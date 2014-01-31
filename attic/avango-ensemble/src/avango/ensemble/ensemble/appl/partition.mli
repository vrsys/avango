(**************************************************************)
(* PARTITION.MLI : support for simulating partitions *)
(* Authors: Mark Hayden, Zhen Xiao, 7/97 *)
(**************************************************************)

(* [connected link_drop_rate e1 e2]
 * Checks whether end points [e1] and [e2] are connected. 
 * The [link_drop_rate] parameter determines the rate
 * at which links are added and dropped. This parameter
 * is intended to be used in a partial evaluation mode. 
 *)
val connected : float -> Endpt.id -> Endpt.id -> bool

