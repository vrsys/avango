(**************************************************************)
(* ARGE.ML *)
(* Author: Mark Hayden, 4/96 *)
(**************************************************************)
open Util
open Trans
(**************************************************************)
let name = Trace.file "ARGE"
let failwith s = Trace.make_failwith name s
let logu = Trace.log (name^"U")
(**************************************************************)

let filter = ref ident

let arg_filter f = filter := f

(**************************************************************)

(*type 'a data = Ok of 'a | Error of exc*)

type 'a t = 
  { name : name ;
    convert : string -> 'a ;
    mutable default : 'a } 

let create name conv default = {
  name = name ; 
  convert = conv ;
  default = default
}

let get a =
  a.default

let set a data =
  a.default <- data

let set_string a data =
  a.default <- a.convert data

(**************************************************************)

let check debug v =
  match get v with
  | Some v -> v
  | None ->
      eprintf "%s:error:%s has not been set\n" debug v.name ;
      eprintf "  (set with [-%s val] command-line argument or\n" v.name ;
      eprintf "   the ENS_%s environment variable)\n" (string_uppercase v.name) ;
      exit 1

(**************************************************************)

let param_args = ref (Some [])

(* General case argument manager.  All the others use this.
 *)
let general unit_arg val_of_string conv default name info =
  let default =
    let environ_prefix = "ENS_" in
    let key = string_uppercase name in
    let key = environ_prefix^key in
    match Hsys.getenv key with
    | None -> 
	default
    | Some s ->
	conv name (val_of_string name s)
  in

  let v = create name (fun s -> conv name (val_of_string name s)) default in

  (* Then add the command-line argument.
   *)
  begin
    let com_set s =
      v.default <- conv name (val_of_string name s)
    in
    let com_set =
      if unit_arg then
      	Arg.Unit(fun () -> com_set "true")
      else 
	Arg.String(com_set)
    in
    let com_name = "-"^name in
    let com_info = ": "^info in
    match !param_args with
    | None -> failwith sanity
    | Some a ->
    	param_args := Some((com_name,com_set,com_info) :: a) ;
  end ;

  v

(**************************************************************)

(* Handle string-based args.
 *)
let string_conv _ = ident
let rec(*no-inline*) string conv =
  general false string_conv conv

(* Handle int-based args.
 *)
let rec(*no-inline*) int_conv name s = 
  try int_of_string s with Failure _ ->
    eprintf "ARGE:bad value for %s:'%s' not integer, exiting\n" name s ;
    exit 1
let rec(*no-inline*) int conv = general false int_conv conv

(* Handle bool-based args.
 *)
let rec(*no-inline*) bool_conv name s =
  try bool_of_string s with Failure _ ->
    eprintf "ARGE:bad value for %s:'%s' not boolean, exiting\n" name s ;
    exit 1
let rec(*no-inline*) bool conv = general true bool_conv conv

(**************************************************************)

let colon_split s = string_split ":" s

let set_ident name v = v

let set_f f name v = f v

let set_option name v = Some v

let set_pos_int name i =
  if i > 0 then i else (
    eprintf "ARGE:non-positive value for %s, exiting\n" name ;
    exit 1 
  )

let set_properties name prop =
  let prop = colon_split prop in
  let prop = List.map Property.id_of_string prop in
  prop

let set_modes name modes =
  let modes = colon_split modes in
  let modes = 
    try List.map Addr.id_of_string modes with e ->
      eprintf "ARGE:bad value for %s, exiting\n" name ;
      eprintf "  (modes={%s}, error=%s\n" 
        (String.concat ", " modes) (Util.error e) ;
      exit 1
  in	
  modes

let set_string_list name s = colon_split s
let set_string_list_option name s = Some(colon_split s)

let inet_of_string_list a hosts =
  let hosts = 
    List.map (fun host ->
      try Some (Hsys.inet_of_string host) with
      |	Not_found ->
	  eprintf "ARGE:warning:%s:unknown host '%s' (stripping from list)\n" name host ;
	  None
      |	e ->
	  eprintf "ARGE:bad value for %s, exiting\n" a.name ;
	  eprintf "  (host=%s, all hosts={%s}, error=%s)\n" 
	    host (String.concat ", " hosts) (Util.error e) ;
	  exit 1
    ) hosts
  in
  let fhosts = filter_nones hosts in
  if hosts <> [] && fhosts = [] then (
    eprintf "ARGE:no known hosts in %s, exiting\n" a.name ;
    exit 1
  ) ;
  fhosts

let inet_of_string a host =
  try Hsys.inet_of_string host with e -> 
    eprintf "ARGE:bad value for %s, exiting\n" a.name ;
    eprintf "  (host={%s}, error=%s\n" host (Util.error e) ;
    exit 1

let set_glue name s =
  try Glue.of_string s
  with Failure _ ->
    eprintf "ARGE:unknown glue '%s'\n" s ;
    exit 1

let set_ranking name s =
  match s with
  | "DEFAULT" -> Addr.default_ranking
  | "TCP" -> (function Addr.Tcp -> 1 | _ -> 0)
  | _ ->
    eprintf "ARGE:unknown ranking '%s'\n" s ;
    exit 1

let set_time name = Time.of_string

(**************************************************************)

(* This buffer size was suggested by Werner Vogels.
 *)
let sock_buf_default = 64*1024*1024/(2*128+1024) (* = 52428 *)

(**************************************************************)

(* Set the verbose flag for the Ocaml garbage collector.
 *)
let gc_verb () =
  let r = Gc.get () in		
  r.Gc.verbose <- 3 (*127*) ;		
  Gc.set r			

(* Set the compaction rate for the Ocaml garbage collector.
 *)
let gc_compact pct =
  let r = Gc.get () in		
  r.Gc.max_overhead <- pct;
  Gc.set r			

let socket_verb () = Socket.set_verbose true

(**************************************************************)

let aggregate    = bool set_ident false "aggregate" "aggregate messages"
let alarm        = string set_ident "REAL" "alarm" "set alarm package to use"
let force_modes  = bool set_ident false "force_modes" "disable transport modes checking"
let glue         = string set_glue Glue.Imperative "glue" "set layer glue to use"
let gossip_hosts = string set_string_list_option None "gossip_hosts" "set hosts for gossip servers"
let gossip_port  = int set_option None "gossip_port" "set port for gossip server"
let port  = int set_option (Some(0)) "port" "set default ensemble port"
let group_name	 = string set_ident "ensemble" "group_name" "set default group name"
let groupd_balance = bool set_ident false "groupd_balance" "load balance groupd servers"
let groupd_hosts = string set_string_list_option None "groupd_hosts" "set hosts for groupd servers"
let groupd_port  = int set_option None "groupd_port" "set port for groupd server"
let groupd_repeat = bool set_ident false "groupd_repeat" "try multiple times to connect to groupd"
let protos_port  = int set_option None "protos_port" "set port for protos server"
let protos_test  = bool set_ident false "protos_test" "used for testing protos"
let id           = string set_ident "" "id" "set 'user id' of process"
let key          = string set_option None "key" "set security key"
let log          = bool set_ident false "log" "use remote log server"
let log_port     = int set_option None "log_port" "set port for log server"
let modes        = string set_modes [Addr.Udp] "modes" "set default transport modes"
let deering_port = int set_option None "deering_port" "set port for Deering IPMC"
let properties   = string set_properties Property.vsync "properties" "set default stack properties"
let groupd       = bool set_ident false "groupd" "use groupd server"
let protos       = bool set_ident false "protos" "use protos server"
let roots        = bool set_ident false "roots" "print information about Ensemble roots"
let pgp          = string set_option None "pgp" "set my id for using pgp"
let pgp_pass     = string set_option None "pgp_pass" "set my passphrase for using pgp"
let pollcount    = int set_ident 10 "pollcount" "number of polling operations before blocking"
let refcount     = bool set_ident false "refcount" "use reference counting"
let multiread    = bool set_ident false "multiread" "read all data from sockets before delivering"
let ranking      = string set_ranking Addr.default_ranking "ranking" "ranking of communication modes"
let sched_step   = int set_pos_int 200 "sched_step" "number of events to schedule before reading from network"
let udp_host     = string set_option None "udp_host" "override host for UDP communication"
let udp_port     = int set_option (Some 0) "udp_port" "override port for UDP communication"
let traces       = string set_ident "" "traces" "set modules to trace"
let netsim_socks = bool set_ident false "netsim_socks" "allow socks with Netsim"
let debug_real   = bool set_ident false "debug_real" "use real time for debugging logs"
let short_names  = bool set_ident false "short_names" "use short names for endpoints"
let sock_buf     = int set_ident sock_buf_default "sock_buf" "size of kernel socket buffers"

(**************************************************************)

let set_properties prop =
  set properties prop

let add_properties p =
  let p = colon_split p in
  let p = List.map Property.id_of_string p in
  set_properties (Lset.union (get properties) p)

let add_scale () =
  add_properties "Scale"

let add_total () =
  add_properties "Total"

let set_fifo () =
  set_properties Property.fifo

(**************************************************************)

let remove_properties p =
  let p = colon_split p in
  let p = List.map Property.id_of_string p in
  set_properties (Lset.subtract (get properties) p)

(**************************************************************)

let trace name =
  let name = string_uppercase name in
  let f info dbg =
    if info = "" then 
      eprintf "%s\n" (String.concat ":" [name;dbg])
    else 
      eprintf "%s\n" (String.concat ":" [name;info;dbg])
  in
  Trace.log_add name f

(**************************************************************)
(*
let log_server appl =
  unit (Alarm.choose (get alarm) appl.Layer.sched appl.Layer.handlers) ;
  let port = check name log_port in
  (Elink.get name Elink.debug_server) appl port
*)
(**************************************************************)
(*
let log_attach alarm host =
  let port = check name log_port in
  let host = 
    try Hsys.inet_of_string host
    with Not_found ->
      eprintf "ARGE:-log_attach:unknown host:%s, exiting\n" host ;
      exit 1
  in
  (Elink.get name Elink.debug_client) alarm host port ;
  while true do 
    Alarm.block alarm
  done
*)
(**************************************************************)

let timestamps = ref []

let timestamp_use n = timestamps := n :: !timestamps

let timestamp_check n = List.mem n !timestamps

(**************************************************************)
(* Print version and contact information.
 *)

let version () =
  eprintf "This application is using Ensemble version %s.\n"
    (Version.string_of_id Version.id) ;
  eprintf "  more information can be found at our web page:\n" ;
  eprintf "    http://www.cs.cornell.edu/Info/Projects/Ensemble/index.html\n" 
  (* exit 0 *)

(**************************************************************)

(* Helper function for parameter management.
 *)
let param_help debug cb s =
  let s = string_split "=" s in
  match s with
  | [name;value] ->
    cb name value
  | _ -> 
      eprintf "ARGE:bad format [-%s name=value], exiting\n" debug ;
      exit 1

let param_int =
  param_help "pint" (fun name value ->
    let value = int_of_string value in
    Param.default name (Param.Int value)
  )

let param_string =
  param_help "pstring" (fun name value ->
    Param.default name (Param.String value)
  )

let param_time =
  param_help "ptime" (fun name value ->
    let value = Time.of_string value in
    Param.default name (Param.Time value)
  )

let param_float =
  param_help "pfloat" (fun name value ->
    let value = float_of_string value in
    Param.default name (Param.Float value)
  )

let param_bool =
  param_help "pbool" (fun name value ->
    let value = 
      try bool_of_string value with Failure _ ->
      	eprintf "ARGE:bad value for parameter %s:'%s' not boolean, exiting\n" name value ;
	exit 1
    in
    Param.default name (Param.Bool value)
  )

(**************************************************************)

let seedval s = 
  let s = Digest.string s in
  let len = String.length s in
  let seed = Array.create len 0 in
  for i = 0 to pred len do
    seed.(i) <- Char.code s.[i]
  done ;
  Random.full_init seed

(**************************************************************)

let args () = 
  (* We "zero" the args after the first use to eliminate
   * clutter in the major heap.
   *)
  let param_args = match !param_args with
  | None -> failwith "args used more than once"
  | Some a ->
      param_args := None ;
      a
  in

  let addtl_args = [|
    "-add_prop",        Arg.String(add_properties), ": add protocol properties" ;
    "-fifo",            Arg.Unit set_fifo, ": set fifo stack" ;
(*
    "-log_attach",      Arg.String(fun s -> log_attach root_sched s), ": attach to remote ensemble process" ;
    "-log",             Arg.Unit(fun () -> log_server root_sched), ": use remote log server" ;
*)
    "-pbool",           Arg.String(param_bool), ": set boolean parameter" ;
    "-pfloat",          Arg.String(param_float), ": set float parameter" ;
    "-pint",            Arg.String(param_int), ": set integer parameter" ;
    "-print_config",	Arg.Unit(fun () -> Trace.config_print () (* ; exit 0 *) ), ": print configuration information" ;
    "-print_defaults",	Arg.Unit(fun () -> Param.print_defaults () (* ; exit 0 *) ), ": print default parameter settings" ;
    "-pstr",            Arg.String(param_string), ": set string parameter" ;
    "-ptime",           Arg.String(param_time), ": set time parameter" ;
    "-remove_prop",     Arg.String(remove_properties), ": remove protocol properties" ;
    "-scale",           Arg.Unit add_scale, ": add scalable property" ;
    "-secure",		Arg.Unit Route.set_secure, ": prevent insecure communication" ;
    "-seed",		Arg.Unit(Random.self_init), ": seed the random number generator" ;
    "-seedval",		Arg.String(seedval), ": seed the random number generator" ;
    "-socket_verb",     Arg.Unit socket_verb, "enable socket library msgs";
    "-test",		Arg.String(Trace.test_exec), ": run named test (-print_config lists available tests)" ;
    "-time",            Arg.String(timestamp_use), ": enable timers for performance testing" ;
    "-total",           Arg.Unit add_total, ": add total ordering property" ;
    "-trace",		Arg.String(trace), ": enable named trace messages" ;
    "-v",               Arg.Unit(version), ": print version" ;
    "-verbose",		Arg.Set Util.verbose, ": enable verbose messages" ;
    "-quiet",		Arg.Set Util.quiet, ": enable quiet operation" ;
    "-gc_verb",         Arg.Unit gc_verb, ": enable Ocaml gc messages" ;
    "-gc_compact",      Arg.Int gc_compact, ": set the compaction rate for Ocaml"
  |] in
  let args = (Array.to_list addtl_args) @ param_args in
  let args = List.sort (fun (a,_,_) (b,_,_) -> compare a b) args in
  args

(**************************************************************)
(* Adapted from Ocaml stdlib/arg.ml *)
open Arg

exception Bad of string

type error =
  | Unknown of string
  | Wrong of string * string * string  (* option, actual, expected *)
  | Missing of string
  | Message of string

let rec assoc3 x l =
  match l with
  | [] -> raise Not_found
  | (y1, y2, y3)::t when y1 = x -> y2
  | _::t -> assoc3 x t
;;

let usage speclist errmsg =
  eprintf "%s\n" errmsg;
  List.iter (function (key, _, doc) -> eprintf "  %s %s\n" key doc) speclist
;;

let parse speclist anonfun errmsg argv =
  let stop error =
    let progname =
      if Array.length Sys.argv > 0 then Sys.argv.(0) else "(?)" in
    begin match error with
      | Unknown s when s = "-help" -> ()
      | Unknown s ->
          eprintf "%s: unknown option `%s'.\n" progname s
      | Missing s ->
          eprintf "%s: option `%s' needs an argument.\n" progname s
      | Wrong (opt, arg, expected) ->
          eprintf "%s: wrong argument `%s'; option `%s' expects %s.\n"
            progname arg opt expected
      | Message s ->
          eprintf "%s: %s.\n" progname s
    end;
    usage speclist errmsg;
    exit 2
  in
  let rec p = function
    | [] -> ()
    | s :: t ->
        if String.length s >= 1 & String.get s 0 = '-'
        then do_key s t
        else begin try (anonfun s); p t with Bad m -> stop (Message m) end
  and do_key s l =
    let action =
      try assoc3 s speclist
      with Not_found -> stop (Unknown s) in
    try
      match (action, l) with
      | (Unit f, l) -> f (); p l
      | (Set r, l) -> r := true; p l
      | (Clear r, l) -> r := false; p l
      | (String f, arg::t) -> f arg; p t
      | (Int f, arg::t) ->
          begin try f (int_of_string arg)
          with Failure "int_of_string" -> stop (Wrong (s, arg, "an integer"))
          end;
          p t
      | (Float f, arg::t) -> f (float_of_string arg); p t
      | (_, []) -> stop (Missing s)
      |	(Rest _,_) -> failwith "Rest not supported"
    with Bad m -> stop (Message m)
  in
    match Array.to_list argv with
    | [] -> ()
    | a::l -> p l

(**************************************************************)

let badarg name s =
  eprintf "%s: unknown argument '%s'\n" name s ;
  eprintf "%s: argv=%s\n" name (string_of_array ident Sys.argv) ;
  exit 1

let parse app_args badarg doc =
  try
    let no_command_line_args = "ENS_NO_COMMAND_LINE_ARGS" in
    ignore (Sys.getenv no_command_line_args) ;
    eprintf "ARGE:parse:%s set\n" no_command_line_args
  with Not_found ->
    let args = app_args @ (args ()) in
    let args = List.sort (fun (a,_,_) (b,_,_) -> compare a b) args in
    parse (app_args @ args) badarg doc (!filter Sys.argv) ;

    begin
      let traces = get traces in
      let traces = string_split " \t" traces in
      List.iter trace traces
    end

 (**************************************************************)
(* The following was added by Roy Friedman to allow dynamic   *)
(* changes to the gossip hosts and cluster servers            *)

let gossip_changed = ref false

let new_gossip_hosts = ref [||]

let set_new_gossip hosts =
  new_gossip_hosts := hosts;
  gossip_changed := true

let get_new_gossip () =
  gossip_changed := false;
  !new_gossip_hosts

let servers = ref []

let set_servers sl =
  servers := sl

let get_servers () =
  !servers

(**************************************************************)
