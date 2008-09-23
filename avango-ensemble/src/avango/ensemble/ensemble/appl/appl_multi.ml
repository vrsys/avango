(**************************************************************)
(* APPL_MULTI.ML: Multiplexion of interfaces *)
(* Author:  Ohad Rodeh  11/97 *)
(**************************************************************)
open Trans
open Hsys
open Util
open View
open Appl_intf
  open New 
open Arge
(**************************************************************)
let name = Trace.file "APPL_MULTI"
let failwith s = Trace.make_failwith name s
(**************************************************************)

type header = Appl_intf.appl_multi_header

let f iv =
  let log = Trace.log name
  and logs = Trace.log (name^"S") in

  if Array.length iv = 0 then failwith "Zero interfaces";
  let heartbeats = Array.map (fun x -> Time.to_float x.heartbeat_rate) iv in
  let heartbeat_rate = Array.fold_left min heartbeats.(0) heartbeats in 
  let heartbeat_rate = Time.of_float heartbeat_rate in
  let width = Array.length iv in
  let xfer_vct = Array.create width false in

  let filter i a = 
    let rec loop = function 
      |	hd :: tl -> (match hd with
    	| Cast msg -> 
      	    Cast (i,msg) :: loop tl
    	| Send (dst,msg) -> 
      	    Send (dst, (i,msg)) :: loop tl
    	| Send1 (dst,msg) -> 
      	    Send1 (dst, (i,msg)) :: loop tl
    	| Control a -> (match a with
	  | Leave 
	  | Prompt 
	  | Suspect _ 
	  | Rekey _
	  | Protocol _ 
	  | Migrate _ 
	  | Timeout _
	  | Dump  -> (Control a) :: loop tl

	  | XferDone -> 
	    logs (fun () -> "XferDone");
	    xfer_vct.(i) <- true;
	    if array_for_all ident xfer_vct then (
	      (Control XferDone) :: loop tl
	    ) else
	      loop tl

	  | Block _ -> failwith "Action Not supported under appl_multi"
	  | No_op -> loop tl
	  )
        )
      |	[] -> []
    in

    let a = Array.to_list a in 
    let a = loop a in
    Array.of_list a 
  in
  let install (ls,vs) = 

    (* View change.
     *)
    log (fun () -> sprintf "New View, nmembers=%d" ls.nmembers);
    Array.fill xfer_vct 0 width false;

    let ha = Array.mapi (fun i apli -> 
      let a,h = apli.install (ls,vs) in
      let a = filter i a in
      (h,a)) iv in
    let hndl_vct = Array.map (fun (h,a) -> h) ha in
    let a  = Array.map (fun (h,a) -> a) ha in
    let actions = (array_flatten a) in
    
    let receive origin bk cs =
      let handlers = Array.map (fun handlers -> handlers.receive origin bk cs) hndl_vct in
      fun (i,msg) ->
	if i < 0 || i >= Array.length handlers then
	  failwith "receive:inconsistent number of multiplexed interfaces" ;
	filter i (handlers.(i) msg)
	  
    and block () =
      let a = Array.mapi (fun i h -> (filter i (h.block ()))) hndl_vct in
      array_flatten a

    and heartbeat t = 
      let a = Array.mapi (fun i h -> (filter i (h.heartbeat t))) hndl_vct in
      array_flatten a

    and disable () =
      Array.iter (fun h -> h.disable ()) hndl_vct
    in

    let handlers = { 
			flow_block = (fun _ -> ());
      receive = receive ;
      block = block ;
      heartbeat = heartbeat ;
      disable = disable
    } 
    in
    actions,handlers

  and exit () = 
    Array.iter (fun apli -> apli.exit ()) iv;
    log (fun () -> "Got exit")
      
  in { 
    heartbeat_rate      = heartbeat_rate; 
    install             = install ;
    exit                = exit
  }

(**************************************************************)

