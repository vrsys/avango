(**************************************************************)
(* PARTITION.ML : support for simulating partitions *)
(* Authors: Mark Hayden, Zhen Xiao, 7/97 *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "PARTITION"
let log = Trace.log name
(**************************************************************)

let endpts = ref Arraye.empty

let add_endpt e =
  if not (Arraye.mem e !endpts) then (
    endpts := Arraye.prependi e !endpts
  )

let choose_endpt () =
  let len = Arraye.length !endpts in
  if len > 0 then (
    let idx = Random.int len in
    Arraye.get !endpts idx
  ) else (
    raise Not_found
  )


let hashtbl_mem t k =
  try Hashtbl.find t k ; true with
    Not_found -> false

let failed = (Hashtbl.create (*name*) 10 : ((Endpt.id*Endpt.id),bool) Hashtbl.t)

let update link_drop_rate () =
  let p = Random.float 1.0 in
  if p < (1.0 -. link_drop_rate) then (
    (* Do nothing. *)
  ) else if p < (1.0 +. (1.0 -. link_drop_rate))/.2.0 then (
    (* Add a link failure.
     *)
    try
      let e1 = choose_endpt () in
      let e2 = choose_endpt () in
      if e1 = e2 then raise Not_found ;
      if Random.int 100 < 95 then (
      	Hashtbl.add failed (e1,e2) true ;
      ) else (
      	Hashtbl.add failed (e1,e2) false ;
      )
    with Not_found -> ()
  ) else (
    (* Remove a link failure.
     *)
    let l = hashtbl_to_list failed in
    let len = List.length l in
    if len > 0 then (
      let idx = Random.int len in
      let (key,_) = List.nth l idx in
      Hashtbl.remove failed key
    )
  )

let counter = Util.counter ()
let dropped = ref 0

let info () =
  if ((counter ()) mod 100) = 0 then (
    log (fun () -> sprintf "#link_failures=%d, #endpts=%d, #dropped_msgs=%d" 
      (hashtbl_size failed) (Arraye.length !endpts) !dropped
    )
  )

(* [connected link_drop_rate e1 e2]
   * 
 * Checks whether end points [e1] and [e2] are connected. 
 * The [link_drop_rate] parameter determines the rate
 * at which links are added and dropped. This parameter
 * is intended to be used in a partial evaluation mode. 
 *)
let connected link_drop_rate e1 e2 =
  add_endpt e1 ;
  add_endpt e2 ;
  update link_drop_rate () ;
  if Hashtbl.mem failed (e1, e2)
  || Hashtbl.mem failed (e2, e1) && (Hashtbl.find failed (e2, e1))
  then (
    incr dropped ;
    info () ;
    false
  ) else true




