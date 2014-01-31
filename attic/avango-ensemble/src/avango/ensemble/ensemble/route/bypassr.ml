(**************************************************************)
(* BYPASSR: Optimized bypass router *)
(* Author: Mark Hayden, 3/97 *)
(**************************************************************)
(*
open Trans
open Buf
open Util
(**************************************************************)
let name = Trace.file "BYPASSR"
let failwith = Trace.make_failwith name
(**************************************************************)

let pack_of_conn conn =
  let pack = Route.pack_of_conn conn in
  Buf.sub name pack len0 len8

let const handler = Route.Bypass(handler true)

let rec(*no inline*) bad_message rbuf ofs len =
  Refcnt.free name rbuf ;
  if len < len12 then
    Route.drop (fun () -> sprintf "%s:short message" name)
  else
    Route.drop (fun () -> sprintf "%s:bad conn id" name)

let merge info =
  let upcalls = 
    Arrayf.map (function 
      | (_,p,_,(Route.Bypass u)) -> (p,u)
      | _ -> failwith sanity
    ) info 
  in
  let upcalls = Route.group upcalls in
  
  Arrayf.map (fun (pack,upcalls) ->
    let upcalls = Route.merge2iov upcalls in
    let upcalls = arity2 upcalls in
    let upcall rbuf ofs len = 
      let buf = Refcnt.read name rbuf in
      let len_iov = len -|| len12 in
      if len_iov >=|| len0
      && Buf.subeq8 buf (ofs +|| len_iov +|| len4) pack 
      then (
	let mi = Buf.read_net_int buf (ofs +|| len_iov) in
	let mv = Iovecl.alloc_noref name rbuf ofs len_iov in
	upcalls mi mv
      ) else (
	bad_message rbuf ofs len
      )
    in upcall
  ) upcalls

let blast mbuf (_,_,xmitvs) _ pack _ =
  let suffix = Buf.create len12 in
  Buf.blit name pack len0 suffix len4 len8 ;
  let xmit mi mv =
    Buf.write_net_int suffix len4 mi ;
    xmitvs mv suffix
  in xmit
*)

let f mbuf =
  failwith "bypass needs to be reimplemented"
(*  Route.create name false false const pack_of_conn merge (blast mbuf)*)

(**************************************************************)
