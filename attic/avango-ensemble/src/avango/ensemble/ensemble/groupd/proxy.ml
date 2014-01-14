(**************************************************************)
(* PROXY.ML *)
(* Author: Mark Hayden, 11/96 *)
(* Designed with Roy Friedman *)
(**************************************************************)
open Trans
open Util
open Mutil
open Buf
(**************************************************************)
let name = Trace.file "PROXY"
let failwith s = failwith (name^":"^s)
let log = Trace.log name
let logc = Trace.log (name^"C")
(**************************************************************)

type message = 
  | Version of string * string
  | Error of string
  | Coord of group * endpt * coord_msg
  | Member of group * endpt * member_msg
      
let string_of_msg = function
  | Coord (_,_,msg) -> sprintf "Coord(%s)" (string_of_coord_msg msg)
  | Member(_,_,msg) -> sprintf "Member(%s)" (string_of_member_msg msg)
  | Error(exp) -> sprintf "Error(%s)" exp
  | Version(s,i) -> sprintf "Version(%s,%s)" s i

(**************************************************************)
(* Marshallers for reading/writing to strings.
 *)

let write_endpt = Marsh.write_buf
let write_group = Marsh.write_buf
let read_endpt = Marsh.read_buf
let read_group = Marsh.read_buf

let make_marsh () =
  let coord_view = 0
  and coord_sync = 1
  and coord_failed = 2
  and member_join = 3
  and member_sync = 4
  and member_fail = 5
  and client_version = 6
  and server_error = 7
  in

  let marsh msg =
    let m = Marsh.marsh_init () in
    let common group endpt =
      write_group m group ;
      write_endpt m endpt
    in
    begin
      match msg with
      | Coord(g,e,View(ltime,primary,view)) ->
	  Marsh.write_int m coord_view ;
	  common g e ;
	  Marsh.write_int m ltime ;
	  Marsh.write_bool m primary ;
	  Marsh.write_list m (fun endpt ->
	    write_endpt m endpt
	  ) view
      | Coord(g,e,Sync) ->
	  Marsh.write_int m coord_sync ;
	  common g e
      | Coord(g,e,Failed(failed)) ->
	  Marsh.write_int m coord_failed ;
	  common g e ;
	  Marsh.write_list m (fun endpt ->
	    write_endpt m endpt
	  ) failed ;
      | Member(g,e,Join(ltime)) ->
	  Marsh.write_int m member_join ;
	  common g e ;
	  Marsh.write_int m ltime
      | Member(g,e,Synced) ->
	  Marsh.write_int m member_sync ;
	  common g e
      | Member(g,e,Fail(failed)) ->
	  Marsh.write_int m member_fail ;
	  common g e ;
	  Marsh.write_list m (fun endpt ->
	    write_endpt m endpt
	  ) failed
      |	Version(service,info) -> 
	  Marsh.write_int m client_version ;
          Marsh.write_string m service ;
          Marsh.write_string m info
      |	Error(explanation) -> 
	  Marsh.write_int m server_error ;
          Marsh.write_string m explanation
    end ;
    Marsh.marsh_done m
  in

  let common m =
    let group = read_group m in
    let endpt = read_endpt m in
    (group,endpt)
  in

  let map = [
    coord_view,(fun m ->
      let g,e = common m in
      let ltime = Marsh.read_int m in
      let primary = Marsh.read_bool m in
      let view = Marsh.read_list m (fun () -> read_endpt m) in
      Coord(g,e,View(ltime,primary,view))
    ) ;
    coord_sync,(fun m -> 
      let g,e = common m in
      Coord(g,e,Sync)) ;
    coord_failed,(fun m -> 
      let g,e = common m in
      let failed = Marsh.read_list m (fun () -> read_endpt m) in
      Coord(g,e,Failed(failed))
    ) ;
    member_join,(fun m -> 
      let g,e = common m in
      let ltime = Marsh.read_int m in
      Member(g,e,Join(ltime))
    ) ;
    member_sync,(fun m -> 
      let g,e = common m in
      Member(g,e,Synced)) ;
    member_fail,(fun m -> 
      let g,e = common m in
      let failed = Marsh.read_list m (fun () -> read_endpt m) in
      Member(g,e,Fail(failed))
    ) ;
    client_version,(fun m ->
      let service = Marsh.read_string m in
      let info = Marsh.read_string m in
      Version(service,info)
    ) ;
    server_error,(fun m ->
      let explanation = Marsh.read_string m in
      Error(explanation)
    )
  ] in
  
  let unmarsh buf =
    try
      let m = Marsh.unmarsh_init buf Buf.len0 in
      let tag = Marsh.read_int m in
      begin try
      	(List.assoc tag map) m
      with Not_found ->
	raise (Marsh.Error "bad message tag")
      end
    with Marsh.Error s ->
      eprintf "PROXY:unmarshalling error:bad format (%s), exiting\n" s ;
      exit 0
  in
  
  (marsh,unmarsh)

(**************************************************************)

type t = {
  send : group -> endpt -> member_msg -> unit ;
  members : (group * endpt, coord_msg -> unit) Hashtbl.t
} 
    
(**************************************************************)

let create alarm sock =
  let marsh,unmarsh = make_marsh () in
  let verbose = ref true in
  
  Hsyssupp.client name alarm sock (fun info send ->
    let send msg =
      let buf = marsh msg in
      send buf len0 (Buf.length buf) Iovecl.empty
    in

    send (Version("ENSEMBLE:groupd",(Version.string_of_id Version.id))) ;

    let sendmsg group endpt msg =
      log (fun () -> sprintf "send:%s" (string_of_member_msg msg)) ;
      let msg = Member(group,endpt,msg) in
      send msg
    in

    let s = {
      send = sendmsg ;
      members = Hashtbl.create 100
    } in 

    let recv msg _ = 
      let msg = unmarsh msg in
      log (fun () -> sprintf "recv:%s (%d)" (string_of_msg msg) (hashtbl_size s.members)) ;
      match msg with
      | Coord(group,endpt,msg) ->
	  let to_client =      
	    try Hashtbl.find s.members (group,endpt)
	    with Not_found ->
	      if !verbose then
		eprintf "PROXY:recv'd message, but no member:%s\n" (string_of_endpt endpt) ;
  (*
    Hashtbl.iter (fun (g,e) _ ->
    eprintf "  %s\n" (string_of_endpt e)
    )  s.members ;
    *)
	      fun _ -> ()
	  in
	  to_client msg
      | Error(exp) ->
	  eprintf "PROXY:groupd server error:%s\n" exp
      | _ -> failwith sanity
    in

    let disable () =
      eprintf "PROXY:lost connection to groupd server, exiting\n" ;
      exit 1
    in
    (recv,disable,s)
  )
  
(**************************************************************)

let join s group endpt ltime to_client =
  Hashtbl.add s.members (group,endpt) to_client ;
  let to_server msg =
    s.send group endpt msg
  in
  to_server
  
(**************************************************************)

(* Number of open connections.
 *)
let conns = ref 0

(**************************************************************)

let server alarm port join =
  let marsh,unmarsh = make_marsh () in
 
  let client_init info send =
    let clients = Hashtbl.create 10 in
    incr conns ;
    logc (fun () -> sprintf "#connections=%d" !conns) ;

    let disable () =
      decr conns ;
      logc (fun () -> sprintf "disable, #connections=%d" !conns) ;
      Hashtbl.iter (fun (_,endpt) to_server ->
	to_server (Fail([endpt]))	(*PERF*)
      ) clients
    in

    let send msg =
      let buf = marsh msg in
      send buf Buf.len0 (Buf.length buf) Iovecl.empty
    in

    let sendmsg group endpt msg =
      log (fun () -> string_of_coord_msg msg) ;
      begin match msg with
      | Failed(failed) when List.mem endpt failed -> (
	  try
	    Hashtbl.remove clients (group,endpt)
	  with Not_found -> ()
	)
      | _ -> ()
      end ;
      send (Coord(group,endpt,msg))
    in

    let recv msg iovl =
      assert (Iovecl.len iovl =|| len0);
      let msg = unmarsh msg in
      log (fun () -> string_of_msg msg) ;
      match msg with 
      |	Member(group,endpt,msg) ->
	  (* For joins, we first have to add a connection record
	   * to our table.
	   *)
	  begin match msg with
	  | Join(ltime) ->
	      let to_client = sendmsg group endpt in
	      let to_server = join group endpt ltime to_client in
	      Hashtbl.add clients (group,endpt) to_server
	  | _ -> ()
	  end ;
	  
	  (* If record of the client is not here, then we assume
	   * the connection was previously broken and do nothing.
	   *)
	  begin 
	    try 
	      let to_server = Hashtbl.find clients (group,endpt)  in
	      to_server msg
	    with Not_found -> ()
	  end
      |	Version(service,version) ->
	  let my_version = Version.string_of_id Version.id in
	  if service <> "ENSEMBLE:groupd"
	  || version <> my_version
	  then (
	    let msg = sprintf "bad version (my version is: 'ENSEMBLE:groupd', '%s')" my_version in
	    send (Error msg) ;
	    disable ()
	      )
      | _ -> failwith sanity
    in
    recv,disable,()
  in
  
  Hsyssupp.server name alarm port client_init

(**************************************************************)
