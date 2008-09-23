(**************************************************************)
(* HOT_OUTBOARD.ML *)
(* Author: Alexey Vaysburd, 11/96 *)
(* Bug Fixes, Cleanup: Mark Hayden, 4/97 *)
(* More Bug Fixes, Cleanup: Mark Hayden, 5/97 *)
(* Aded TCP outboard mode for NT: Werner Vogels 10/97 *)
(* More cleanup: Mark Hayden, 11/97 *)
(**************************************************************)
open Ensemble
open Util
open View
open Appl_intf
open Protos
open Hot_util
open Marsh
open Buf
(**************************************************************)
let name = Trace.file "HOT_OUTBOARD"
let failwith s = Trace.make_failwith name s
let log = Trace.log name
let log_iov = Trace.log (name^":IOV")
(**************************************************************)
(* Marshalling and unmarshalling.  These functions are using
 * the Ensemble Marsh module.  For marshalling, we first write
 * into a "marshaller", then convert that to a string, and
 * finally write the string out preceded by its length.
 *)

let write_init = Marsh.marsh_init

let write_endpt = write_string
let write_msg m buf ofs len = write_string m (String.sub buf ofs len)
let write_gctx = write_int
let write_cbtype = write_int
let write_endpt_array m = write_array m (write_endpt m)
let write_bool_array m = write_array m (write_bool m)

(**************************************************************)

let up_view      = 0 
and up_cast      = 1 
and up_send      = 2 
and up_heartbeat = 3 
and up_block     = 4 
and up_exit      = 5 

let marsh_upcall id msg =
  let m = write_init () in
  write_gctx m id ;
  let iovl = 
    match msg with
      | UInstall vf ->
	  let v = c_view_state vf in
  	  write_cbtype m up_view ;
  	  write_string m v.c_version ;
  	  write_string m v.c_group ;
  	  write_endpt_array m v.c_view ;
  	  write_int m v.c_rank ;
  	  write_string m v.c_protocol ;
  	  write_bool m v.c_groupd ;
  	  let ltime,coord = v.c_view_id in
  	  write_int m ltime ;
  	  write_endpt m coord ;
  	  write_string m v.c_params ;
  	  write_bool m v.c_xfer_view ;
  	  write_bool m v.c_primary ;
  	  write_bool_array m v.c_clients;
  	  write_string m v.c_key;
	  Iovecl.empty
      | UReceive(origin,cs,iovl) -> (
	  let typ = 
	    match cs with
	      | Appl_intf.New.C -> up_cast
	      | Appl_intf.New.S -> up_send
	  in
	  write_cbtype m typ ;
	  write_int m origin ;
	  iovl
	)
      | UHeartbeat time ->
	  write_cbtype m up_heartbeat ;
  	  let time = milli_of_time time in
  	  write_int m time;
	  Iovecl.empty
      | UBlock ->
  	  write_cbtype m up_block;
	  Iovecl.empty
      | UExit ->
  	  write_cbtype m up_exit;
	  Iovecl.empty
  in
  let buf = Marsh.marsh_done m in
  buf,iovl

(**************************************************************)

(* Process input from stdin.  Read a downcall + add to the
 * array of pending downcalls.
 *)
let dncall_unmarsh buf iovl =

  let m = Marsh.unmarsh_init buf len0 in
  let read_int () = Marsh.read_int m in
  let read_string () = Marsh.read_string m in
  let read_bool () = 
    let b = read_int() in
    (b != 0)
  in
  let read_endpt = read_string in
  let id = read_int () in
  let dntype = read_int () in
  log (fun () -> sprintf "id=%d dntype=%d" id  dntype);
  let ret =
    match dntype with
    | 0 ->
	let rate = read_int () in
	let transports = read_string () in
	let protocol = read_string () in
	let group_name = read_string () in
	let properties = read_string () in
	let use_properties = read_bool () in
	let groupd = read_bool () in
	let params = read_string () in
	let client = read_bool () in
	let debug = read_bool () in

	(* This is currently unsupported for outboard mode.
	 *)
	let endpt = "" in
	let princ = read_string () in
	let key   = read_string () in
	let secure = read_bool () in

	let jops = {
	  jops_hrtbt_rate = rate;
	  jops_transports = transports;
	  jops_protocol = protocol;
	  jops_group_name = group_name;
	  jops_properties = properties;
	  jops_use_properties = use_properties;
	  jops_groupd = groupd;
	  jops_params = params;
	  jops_client = client;
	  jops_debug = debug;
	  jops_endpt = endpt;
	  jops_princ  = princ;
	  jops_key    = key;
	  jops_secure = secure
	} in
	let _,vs = init_view_state jops in
	let heartbeat_rate = Time.of_float (float jops.jops_hrtbt_rate /. 1000.0) in
	let modes = Arrayf.to_list (Addr.ids_of_set (Arrayf.get vs.address 0)) in
	Create(id,vs,heartbeat_rate,modes)
    | 1 ->				(* Cast *)
	Dncall(id,Cast(iovl))
    | 2 ->				(* Send*)
	let dest = read_endpt () in
	SendEndpt(id,dest,iovl)
    | 3 ->				(* Suspect *)
	let size = read_int () in
	let susp = ref [] in
	for i = 1 to size do
	  let endpt = read_endpt () in
	  susp := endpt :: !susp
	done;
	let suspects = List.rev !susp in
	SuspectEndpts(id,suspects)
    | 4 ->                              (* XferDone *)
	Dncall(id,Control XferDone)
    | 5 ->				(* Protocol *)
	let proto = read_string () in
	Dncall(id,Control(Protocol(Proto.id_of_string proto)))
    | 6 ->				(* Property *)
	let props = read_string () in
	let protocol =
	  let props = 
	    let props_l = Util.string_split ":" props in
	    let props_l = List.map Property.id_of_string props_l in
	    props_l
	  in
	  Property.choose props
	in
	Dncall(id,Control(Protocol protocol))
    | 7 ->				(* Leave *)
	Dncall(id,Control Leave)
    | 8 ->				(* Prompt *)
	Dncall(id,Control Prompt)
    | 9 ->				(* Rekey *)
	Dncall(id,Control (Rekey false))
    | 10 ->				(* Block *)
	Dncall(id,Control(Block true))
    | 11 ->
	let dest = read_int () in
	Dncall(id,Send1(dest,iovl))
    | _ -> failwith sanity
  in
  Marsh.unmarsh_check_done_all m ;
  ret

(**************************************************************)

let run () =
  (* Set the default pollcount value to 0.  This makes the
   * application somewhat more efficient.  
   *)
  Arge.set Arge.pollcount 0 ;
  let set_ident name v = v in
  let tcp_port     = Arge.int set_ident 5002 "tcp_port" "set port outboard TCP channel" in

  Arge.parse [
    (*
     * Extra arguments can go here.
     *)
  ] (Arge.badarg name) "outboard:Ensemble HOT tools outboard daemon";

  let alarm = Appl.alarm name in

  (* Force linking of the Udp mode.  This causes
   * a port number to be installed in the 
   * Unique generator.
   *)
  let _ = Domain.of_mode alarm Addr.Udp in

  Appl.start_monitor ();

  (* Get the server.
   *)
  let protos_server = Protos.server alarm Appl.addr Appl.config_new in

  (* Wrap the client handler with the HOT outboard marshaller.
   *)
  let client info send =
    let send = function
      |	Upcall(id,msg) ->
	  let buf,iovl = marsh_upcall id msg in
	  if Iovecl.len iovl >|| len0 then
	    log_iov (fun () -> 
	      sprintf "iovecs=%s" (Iovecl.sum_refs iovl));
	  send buf len0 (Buf.length buf) iovl
      |	_ -> failwith sanity
    in

    let recv,disable,() = protos_server info send in

    let recv buf iovl =
      let msg = dncall_unmarsh buf iovl in
      recv msg
    in

    (recv,disable,())
  in

  (* as of version 1.3, we only use TCP.
   *)
  let chan =
    let port = Arge.get tcp_port in
    Hsyssupp.server name alarm port
  in
  chan client ;

  eprintf "HOT_OUTBOARD:starting Ensemble in outboard mode\n";
  Appl.main_loop ()

(* Run the application, with exception handlers to catch any
 * problems that might occur.
 *)
let _ = Appl.exec ["outboard";"outboard-crypto"] run

(**************************************************************)
