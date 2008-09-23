(**************************************************************)
(* APPL_LWE.ML: lwe application interface *)
(* Author: Mark Hayden, 11/97 *)
(**************************************************************)
open Trans
open Util
open View
open Appl_intf 
  open New
(**************************************************************)
let name = Trace.file "APPL_LWE"
let failwith = Trace.make_failwith name
let log = Trace.log name
(**************************************************************)

let arrayf_combine5 a b c d e =
  let len = Arrayf.length a in
  let lb = Arrayf.length b in
  let lc = Arrayf.length c in
  let ld = Arrayf.length d in
  let le = Arrayf.length e in

  if lb <> len || lc <> len || ld <> len || le <> len then (
    eprintf "APPL_LWG:arrayf_combine:a=%d b=%d c=%d d=%d e=%d\n"
      len lb lc ld le ;
    failwith sanity
  ) ;

  Arrayf.init len (fun i ->
    ((Arrayf.get a i),
     (Arrayf.get b i),
     (Arrayf.get c i),
     (Arrayf.get d i),
     (Arrayf.get e i))
  )

let arrayf_split5 abcde =
  let a = Arrayf.map (fun (x,_,_,_,_) -> x) abcde in
  let b = Arrayf.map (fun (_,x,_,_,_) -> x) abcde in
  let c = Arrayf.map (fun (_,_,x,_,_) -> x) abcde in
  let d = Arrayf.map (fun (_,_,_,x,_) -> x) abcde in
  let e = Arrayf.map (fun (_,_,_,_,x) -> x) abcde in
  (a,b,c,d,e)

(**************************************************************)

type header = appl_lwe_header (* in Appl_intf *)

let genf comb split alarm (ls,vs) il =
  if il = [] then failwith "no interfaces given" ;
  let endpts,intfs = List.split il in

  (* The heartbeat rate is the minimum of all rates.
   *)
  let heartbeat_rate =
    List.fold_left (fun hb intf -> min intf.heartbeat_rate hb) ((List.hd intfs).heartbeat_rate) intfs
  in

  let disable_asyncs =
    let async = Alarm.async alarm in
    let hwe_async = Async.find async ls.async in
    List.map (fun endpt -> 
      Async.add async (vs.group,endpt) hwe_async
    ) endpts
  in

  (* Exit causes all exits to be called and asyncs to
   * be disabled.
   *)
  let exit () = 
    List.iter (fun i -> i.exit ()) intfs ;
    List.iter (fun f -> f ()) disable_asyncs
  in

  let install (ls,vs) =
    let hwg_view = vs.view in
    let ranks = Arrayf.init ls.nmembers ident in
    let info = arrayf_combine5 vs.lwe vs.address vs.clients vs.out_of_date ranks in
    let info =
      Arrayf.map (fun (lwe,address,clients,ood,hw_rank) ->
      	Arrayf.map (fun endpt -> (endpt,address,clients,ood,hw_rank)) lwe
      ) info
    in
    let info = Arrayf.map Arrayf.to_list info in
    let info = Arrayf.to_list info in
    let info = List.flatten info in
    let info = Arrayf.of_list info in
    let view,address,clients,ood,hw_ranks = arrayf_split5 info in

    let translate rank actions = 
      Array.map (function
	| Cast msg ->
	    let msg,iov = split msg in
  (*
	    eprintf "APPL_LWG:Cast:%s\n" (Endpt.string_of_id rank) ;
  *)
	    Cast(((LCast rank),msg),iov)
	| Send(destsa,msg) ->
	    let dests = Arrayf.of_array destsa in
	    let msg,iov = split msg in
	    (* Calculate actual destinations.
	     *)
  (*
	    eprintf "APPL_LWG:Send:%s->%s\n" (Endpt.string_of_id endpt) (string_of_array Endpt.string_of_id dendpts) ;
  *)

	    (* And their ranks in the HWG.
	     *)
	    let dranks =
	      Arrayf.map (Arrayf.get hw_ranks) dests
	    in

	    (* Remove duplicate ranks (done implicitly in
	     * Lset.inject).
	     *)
	    let dranks = Lset.inject dranks in
	    let dranks = Lset.project dranks in
	    let dranks = Arrayf.to_array dranks in

	    Send(dranks,((LSend(rank,destsa),msg), iov))
	| Send1(dest,msg) ->
	    let msg,iov = split msg in

	    (* Calculate rank in the HWG.
	     *)
	    let drank = Arrayf.get hw_ranks dest in

	    Send1(drank,((LSend(rank,[|dest|]),msg), iov))
	| Control o ->
	    begin
	      match o with
	      | Migrate _
	      | Suspect _
	      | Block _
	      | Leave
	      | XferDone ->
		  failwith (sprintf "does not support %s actions" (string_of_control o))
	      |	_ -> ()
	    end ;
	    Control o
      ) actions
    in

    let vs = set vs [
      Vs_view view ;
      Vs_address address ;
      Vs_clients clients ;
      Vs_out_of_date ood 
(* ;     Vs_lwe lwe*) (* BUG *)
    ] in

    let ah =
      List.map (fun (endpt,intf) ->
    	let ls = View.local name endpt vs in
	let actions,handlers = intf.install (ls,vs) in
	let actions = translate ls.rank actions in
	(actions,(ls.rank,handlers))
      ) il
    in
    let actions,handlers = List.split ah in
    let actions = Array.concat actions in
    let handlers = Array.of_list handlers in

    (* Block local members.
     *)
    let block () =
      let actions = 
	Array.map (fun (rank,handlers) -> 
	  translate rank (handlers.block ())
        ) handlers
      in
      array_flatten actions
    in

    (* Give heartbeat to all local members.
     *)
    let heartbeat t =
      let actions =
 	Array.map (fun (rank,handlers) -> 
	  translate rank (handlers.heartbeat t)
	) handlers 
      in
      array_flatten actions
    in    

    (* This is an array of handlers for local members.
     * For doing fast delivery of Sends.
     *)
    let handlersa =
      let a = Array.create (Arrayf.length view) None in
      Array.iter (fun (rank,handlers) ->
	a.(rank) <- Some handlers
      ) handlers ;
      a
    in

    (* PERF: The closures should be cached, but that isn't
     * so easy to do.
     *)
    let receive _ bk cs =
      let handler = function
      	| ((LCast origin),msg),iov ->
	    let msg = comb msg iov in
	    let actions = 
	      Array.map (fun (rank,handlers) ->
		let actions = handlers.receive origin bk cs msg in
	      	translate rank actions
	      ) handlers
	    in
	    array_flatten actions
      	| (LSend(origin,dests),msg),iov ->
	    let msg = comb msg iov in
	    let actions =
	      Array.map (fun rank ->
		match handlersa.(rank) with
		| None -> [||]
		| Some handlers ->
	      	    let actions = handlers.receive origin bk cs msg in
	      	    translate rank actions
	      ) dests
	    in
	    array_flatten actions
      in handler
    in
    
    let disable () =
      Array.iter (fun (_,handlers) -> handlers.disable ()) handlers
    in

    let handlers = { 
			flow_block = (fun _ -> ());
      heartbeat = heartbeat ;
      receive = receive ;
      block = block ;
      disable = disable
    } in
    
    actions,handlers
  in 

  let intf =
    { heartbeat_rate = heartbeat_rate ;
      install = install ;
      exit = exit }
  in

  (* Does not affect ls.
   *)
  let vs = 
    View.set vs [
      Vs_lwe (Arrayf.singleton (Arrayf.of_list endpts))
    ] 
  in
  ((ls,vs),intf)

let pf appl info =
  let comb msg iov = (msg,iov) in
  let split (msg,iov) = (msg,iov) in
  genf comb split appl info

let f appl info =
  let comb () iov = iov in
  let split iov = ((),iov) in
  genf comb split appl info

(**************************************************************)

