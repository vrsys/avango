(**************************************************************)
open Appl_intf open New
open Util
open View
(**************************************************************)

let f i =
  eprintf "APPL_INTF:aggregate:preprocessing not done" ;
  let ma,um = Iovecl.make_marsh true in

  let install (ls,vs) =
    let actions,handlers = i.install (ls,vs) in

    let pending = Queuee.create () in
    let next_sweep = ref Time.zero in

    let collect actions =
      Queuee.add actions pending
    in    

    let casts = ref [] in
    let sends = Array.create ls.nmembers [] in
    let emit () =
      let other = Queuee.create () in

      Queuee.iter (fun al -> 
	Array.iter (fun a -> match a with
	| Cast(m) -> 
	    casts := m :: !casts
	| Send(dl,m) -> 
	    Array.iter (fun d -> sends.(d) <- m :: sends.(d)) dl
	| Send1(d,m) -> 
	    sends.(d) <- m :: sends.(d)
	| Control o ->
	    Queuee.add (Control o) other
	) al
      ) pending ;
      Queuee.clear pending ;

      let emit = other in

      for i = 0 to pred ls.nmembers do
	let pending = sends.(i) in
	if pending <> [] then (
	  let pending = Array.of_list pending in
	  let pending = ma pending in
	  Queuee.add (Send([|i|], pending)) emit
        )
      done ;
      if !casts <> [] then (
	let pending = Array.of_list !casts in
	let pending = ma pending in
	Queuee.add (Cast(pending)) emit
      ) ;
      let emit = Queuee.to_list emit in
      Array.of_list emit
    in

    let receive o b cs m =
      Array.iter (fun m -> collect (handlers.receive o b cs m)) (um m) ; 
      [||] 
    in  

    let heartbeat time =
      collect (handlers.heartbeat time) ;
      if time > !next_sweep then (
	next_sweep := Time.add time i.heartbeat_rate ;
	emit ()
      ) else [||]
    in

    let block () =
      collect (handlers.block ()) ;
      emit ()
    in

    let handlers = { 
      flow_block = handlers.flow_block ;
      heartbeat = heartbeat ;
      receive = receive ;
      block = block ;
      disable = handlers.disable
    } in
    
    collect actions ;
    [||],handlers
  in

  { heartbeat_rate = i.heartbeat_rate ;
    install = install ;
    exit = i.exit }

(**************************************************************)
