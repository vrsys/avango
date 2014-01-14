(**************************************************************)
(* HANDLE.ML: application interface *)
(* Author: Mark Hayden, 10/97 *)
(**************************************************************)
open Trans
open Util
open View
(**************************************************************)
let name = Trace.file "HANDLE"
let failwith = Trace.make_failwith name
let log = Trace.log name
(**************************************************************)

module New = struct
  open Appl_handle open New

let f i =
  let install ((ls,vs) as vf) =
    let view = vs.view in
    let nmembers = ls.nmembers in
    let handles = Arrayf.map (handle_hack name) view in

    let handle_of_rank rank =
      if rank < 0 then
	failwith "negative rank" ;
      if rank >= ls.nmembers then
	failwith "rank too large" ;
      Arrayf.get handles rank
    in

    let rank_of_handle handle =
      let rank = handle.rank in
      if rank >= 0 
      && rank < nmembers 
      && Arrayf.get view rank == handle.endpt 
      then (
	rank
      ) else (
	let endpt = handle.endpt in
	try
	  let rank = Arrayf.index endpt view in
	  handle.endpt <- Arrayf.get view rank ;
	  handle.rank <- rank ;
	  rank
	with Not_found ->
	  eprintf "HANDLE:got invalid endpt\n" ;
	  (-1)
      )
    in

    let map a =
      Array.map (function
	| Cast m -> Appl_intf.Cast m
	| Send(handles,m) -> 
	    let dests = Array.map rank_of_handle handles in
	    if array_mem (-1) dests then
	      Appl_intf.Send([|-1|],m)
	    else
	      Appl_intf.Send(dests,m)
	| Control o -> Appl_intf.Control o
      ) a
    in
    let actions,handlers = i.install vf handles in
    let actions = map actions in

    let receive o b cs =
      let o = handle_of_rank o in
      let handler = handlers.receive o b cs in
      let handler m =
      	map (handler m)
      in handler
    in
    
    let block () =
      map (handlers.block ())
    in

    let heartbeat t =
      map (handlers.heartbeat t)
    in

    let handlers = { 
      Appl_intf.New.flow_block = (fun _ -> ());
      Appl_intf.New.heartbeat = heartbeat ;
      Appl_intf.New.receive = receive ;
      Appl_intf.New.block = block ;
      Appl_intf.New.disable = handlers.disable
    } in
    
    actions,handlers
  in 
  
  (),{ Appl_intf.New.heartbeat_rate = i.heartbeat_rate ;
    Appl_intf.New.install = install ;
    Appl_intf.New.exit = i.exit }

end

let new_f = New.f

