(**************************************************************)
(* MPI.ML: MPI network transport *)
(* Author: Mark Hayden, 12/95 *)
(* Originally based on code by Jocelyn Serot. *)
(**************************************************************)
open Util
open Trans
open Buf
open View
(**************************************************************)
let name = Trace.file "MPI"
let failwith s = failwith (Util.failmsg name s)
(**************************************************************)

external init : string array -> string array
    = "mpi_init"
external buffer : int -> unit
    = "mpi_buffer"
external finalize : unit -> unit 
    = "mpi_finalize"
external abort : unit -> unit 
    = "mpi_abort"
external nmembers : unit -> int 
    = "mpi_nmembers"
external rank : unit -> int 
    = "mpi_rank"
external get_processor_name : unit -> string 
    = "mpi_get_processor_name"
external send : rank array -> Buf.t -> ofs -> len -> unit 
    = "mpi_send" "noalloc"
external recv : Buf.t -> ofs -> len -> len
    = "mpi_recv" "noalloc"

(**************************************************************)
(* When the args are parsed, we step in and call MPI_Init and
 * install the finalization function.
 *)

let arrayf_split5 abcde =
  let a = Arrayf.map (fun (x,_,_,_,_) -> x) abcde in
  let b = Arrayf.map (fun (_,x,_,_,_) -> x) abcde in
  let c = Arrayf.map (fun (_,_,x,_,_) -> x) abcde in
  let d = Arrayf.map (fun (_,_,_,x,_) -> x) abcde in
  let e = Arrayf.map (fun (_,_,_,_,x) -> x) abcde in
  (a,b,c,d,e)

let mpi_info nmembers rank =
  let (_,vs) = Appl.default_info name in
  let info = Arrayf.init nmembers (fun rank ->
    let endpt = Endpt.extern (sprintf "{MPI:%d}" rank) in
    let addr = Addr.set_of_arrayf (Arrayf.singleton (Addr.MpiA(rank))) in
    let client = false in
    let out_of_date = false in
    let lwe = Arrayf.empty in
    (endpt,addr,client,out_of_date,lwe)
  ) in
  let view,address,client,out_of_date,lwe = arrayf_split5 info in
  let vs = View.set vs [
    Vs_view view ;
    Vs_address address ;
    Vs_clients client ;
    Vs_out_of_date out_of_date ;
    Vs_lwe lwe
  ]	in
  let ls = View.local name (Arrayf.get view rank) vs in
  (ls,vs)
      
let _ =
  Arge.arg_filter (fun argv ->
    eprintf "MPI:prearg:%s\n" (string_of_array ident argv) ;
    let argv = init argv in
    eprintf "MPI:postarg:%s\n" (string_of_array ident argv) ;
    at_exit finalize ;

    (* Allocate a megabyte send buffer.
     * If it overflows then we are in trouble.
     *)
    buffer 1000000 ;
    Appl.mpi_info := Some(mpi_info (nmembers()) (rank())) ;
    argv
  )

(**************************************************************)
(*
let recv a b c =
  eprintf "MPI:calling recv\n" ;
  let ret = recv a b c in
  if ret > 0 then
    eprintf "MPI:recd %d bytes\n" ret ;
  ret
*)


let domain alarm =
  let mbuf = Alarm.mbuf alarm in
  let handlers = Alarm.handlers alarm in
  let host = Hsys.gethostname () in
  let host = Hsys.inet_of_string host in

  let deliver = Route.deliver in
  let poll =
    Mbuf.alloc_recv name mbuf handlers
      recv
      Route.deliver
  in

  let endpt mode = Addr.MpiA (rank ()) in

  let enable _ _ _ _  =
    Alarm.add_poll alarm name poll ;

    let disable () =
      Alarm.rmv_poll alarm name

    and xmit dest =
      let dests =
	match dest with
	| Domain.Pt2pt dests -> (
	    Arrayf.map (fun dest ->
	      match Addr.project dest Addr.Mpi with 	  
	      | Addr.MpiA rank -> rank
	      | _ -> failwith sanity
	    ) dests
	  )
	| Domain.Mcast _ -> failwith "multicast not supported"
	| Domain.Gossip _ -> 
	    eprintf "MPI:warning:ignoring gossip\n" ; Arrayf.empty
  (*	  failwith "gossip not supported"*)
      in
      let dests = Arrayf.to_array dests in

      (* This check is only for the gossip stuff above.
       *)
      if dests = [||] then None else (
	let xmit buf ofs len =
  (*
	  eprintf "MPI:sending to %s %d bytes\n" (string_of_int_array dests) len ;
  *)
	  send dests buf ofs len
  (*
  ;
	  eprintf "MPI:done sending\n"
  *)
	in
	let xmitv iov = 
	  let iov = Iovecl.flatten name iov in(*PERF*)
	  Iovec.read name iov xmit
	in
	let xmitvs iovl s =
	  let iov = Mbuf.alloc name mbuf s len0 (Buf.length s) in
	  let iovl = Iovecl.appendi name iovl iov in
	  xmitv iovl ;
	  Iovecl.free name iovl
	in Some(xmit,xmitv,xmitvs)
      )
    in 
    Domain.handle disable xmit
  in

  Domain.create name endpt enable

let _ = Elink.put Elink.mpi_domain domain

(**************************************************************)
