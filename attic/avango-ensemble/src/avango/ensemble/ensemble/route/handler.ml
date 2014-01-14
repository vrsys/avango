(**************************************************************)
(* HANDLER.ML: handler tables *)
(* Author: Mark Hayden, 12/95 *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "HANDLER"
let failwith = Trace.make_failwith name
(**************************************************************)

type hash = int

(* NOTE: this module is hacked so that the array
 * size of the handlers is not modifiable.
 *)

(* Need to watch out for Monster Ints.
 *)
let power = 10
let init_size = 1 lsl power		(* must be a power of 2 *)
let mask = init_size - 1

(* MH: I've checked that the optimizer inlines all the above.
 *)
let hashint t i = i land mask


type ('key,'data,'merge) t = 
  { merges : 'merge Arraye.t ;
    datas : ('key * 'data) Arrayf.t Arraye.t ;
    table : ('key,'data) Hashtbl.t ;
    merge : 'data Arrayf.t -> 'merge ;
    delay : (unit -> 'merge) -> 'merge ;
    size : int }

let create merge delay = 
  { merge = merge ;
    delay = delay ;
    table = Hashtbl.create (*name*) 437 ;
    size = init_size ;
    merges = Arraye.create init_size (merge Arrayf.empty) ;
    datas = Arraye.create init_size Arrayf.empty }

let to_list t =
  hashtbl_to_list t.table

let add t hash key data =
(*
  eprintf "HANDLER:hash=%04d %04d\n" ((Hashtble.hash_param 10 100 key)mod 9999) ((Hashtble.hash_param 100 1000 key) mod 9999);
*)
  begin
    if Hashtbl.mem t.table key then
      failwith "add:duplicate keys" ;
    Hashtbl.add t.table key data ;
(*  printf "table_size=%d\n" (hashtbl_size t.table) ;*)
  end ;

  let hash = hashint t hash in
  let entries = Arraye.get t.datas hash in
  let entries = Arrayf.prependi (key,data) entries in
  Arraye.set t.datas hash entries ;
  let delay () =
    let merge = t.merge (Arrayf.map snd entries) in
    Arraye.set t.merges hash merge ;
    merge
  in
  Arraye.set t.merges hash (t.delay delay)

let remove t hash key =
  begin
    if not (Hashtbl.mem t.table key) then
      failwith "remove:not found" ;
    Hashtbl.remove t.table key ;
    if Hashtbl.mem t.table key then
      failwith "remove:duplicate keys"
  end ;
	
  let hash = hashint t hash in
  let entries = Arraye.get t.datas hash in
  let entries = Arrayf.filter (fun (sk,_) -> key <> sk) entries in
  Arraye.set t.datas hash entries ; 

  (* If # entries > 1 then delay processing until needed.
   *)
  let nentries = Arrayf.length entries in
  if nentries <= 1 then (
    Arraye.set t.merges hash (t.merge (Arrayf.map snd entries))
  ) else (
    let delay () =
      let entries = Arraye.get t.datas hash in
      let merge = t.merge (Arrayf.map snd entries) in
      Arraye.set t.merges hash merge ;
      merge
    in
    Arraye.set t.merges hash (t.delay delay)
  )

let find t hash =
  Arraye.get t.merges (hashint t hash)

let size t = 
  hashtbl_size t.table

let info t =
  sprintf "{Handler:size=%d}" (size t)
(*
  sprintf "{Handler:size=%d:table=%s}" (size t) (Hashtble.info t.table)
*)

(**************************************************************)
