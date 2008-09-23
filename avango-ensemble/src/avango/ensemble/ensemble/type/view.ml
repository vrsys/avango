(**************************************************************)
(* VIEW.ML *)
(* Author: Mark Hayden, 3/96 *)
(**************************************************************)
open Trans
open Util
(**************************************************************)
let name = Trace.file "VIEW"
let failwith s = Trace.make_failwith2 name s
(**************************************************************)

type t = Endpt.id Arrayf.t

let to_string = Arrayf.to_string Endpt.string_of_id

(**************************************************************)

type id = ltime * Endpt.id

(**************************************************************)

type state = {
  (* Group information.
   *)
  version       : Version.id ;		(* version of Ensemble *)
  group		: Group.id ;		(* name of group *)
  proto_id	: Proto.id ;		(* id of protocol in use *)
  coord         : rank ;		(* initial coordinator *)
  ltime         : ltime ;		(* logical time of this view *)
  primary       : primary ;		(* primary partition? (only w/some protocols) *)
  groupd        : bool ;		(* using groupd server? *)
  xfer_view	: bool ;		(* is this an XFER view? *)
  key		: Security.key ;	(* keys in use *)
  prev_ids      : id list ;             (* identifiers for prev. views *)
  params        : Param.tl ;		(* parameters of protocols *)
  uptime        : Time.t ;		(* time this group started *)

  (* Per-member arrays.
   *)
  view 		: t ;			(* members in the view *)
  clients	: bool Arrayf.t ;	(* who are the clients in the group? *)
  address       : Addr.set Arrayf.t ;	(* addresses of members *)
  out_of_date   : ltime Arrayf.t ;	(* who is out of date *)
  lwe           : Endpt.id Arrayf.t Arrayf.t ; (* for light-weight endpoints *)
  protos        : bool Arrayf.t 	(* who is using protos server? *)
}

(**************************************************************)

type local = {
  endpt	        : Endpt.id ;		(* endpoint id *)
  addr	        : Addr.set ;		(* my address *)
  rank 	        : rank ;		(* rank in the view *)
  name		: string ;		(* my string name *)
  nmembers 	: nmembers ;		(* # members in view *)
  view_id 	: id ;			(* unique id of this view *)
  am_coord      : bool ;  		(* rank = vs.coord? *)
  falses        : bool Arrayf.t ;       (* all false: used to save space *)
  zeroes        : int Arrayf.t ;        (* all zero: used to save space *)
  loop          : rank Arrayf.t ;      	(* ranks in a loop, skipping me *)
  async         : (Group.id * Endpt.id)	(* info for finding async *)
}  

(**************************************************************)

type full = local * state

(**************************************************************)

let debug_name rank endpt ltime =
  sprintf "(%d,%s,%d)" rank (Endpt.string_of_id endpt) ltime

let check dbg (ls,vs) =
  let failwith s =
    failwith (sprintf "check:%s:%s" dbg s)
  in
  if ls.name <> debug_name ls.rank ls.endpt vs.ltime then
    failwith "bad name" ;
  if not (Arrayf.mem ls.endpt vs.view) then
    failwith "endpt not in view" ;
  if ls.rank <> Arrayf.index ls.endpt vs.view then
    failwith "bad rank" ;
  if ls.nmembers <> Arrayf.length vs.view then
    failwith "bad nmembers<>length view" ;
  if ls.nmembers <> Arrayf.length vs.address then
    failwith "bad nmembers<>length address"

(**************************************************************)

let id_of_state vs = 
  if Arrayf.is_empty vs.view then
    failwith "empty view" ;
  (vs.ltime,(Arrayf.get vs.view 0))

let string_of_version_id = Version.string_of_id
  
(**************************************************************)

let make_loop nmembers rank =
  Arrayf.init nmembers (fun i ->
    let next = (succ i) mod nmembers in
    if next = rank then
      (succ next) mod nmembers
    else
      next
  )
     
let local debug endpt vs =
  if not (Arrayf.mem endpt vs.view) then
    failwith "local:endpt not in view" ;
  let rank = Arrayf.index endpt vs.view in
  let name = debug_name rank endpt vs.ltime in
  let nmembers = Arrayf.length vs.view in
  let addr = Arrayf.get vs.address rank in
  let falses = Arrayf.create nmembers false in
  let zeroes = Arrayf.create nmembers 0 in
  { name = name ;
    endpt = endpt ;
    addr = addr ;
    nmembers = nmembers ;
    rank = rank ;
    view_id = id_of_state vs ;
    am_coord = rank = vs.coord ;
    falses = falses ;
    zeroes = zeroes ;
    loop = make_loop nmembers rank ;
    async = (vs.group,endpt)
  } 

let redirect_async debug async l = { l with async = async }

(**************************************************************)

let singleton key proto_id group endpt addr uptime =
  let vs = {
    coord       = 0 ;
    group	= group ;
    version     = Version.id ;
    view   	= Arrayf.singleton endpt ;
    address     = Arrayf.singleton addr ;
    clients	= Arrayf.singleton false ;
    out_of_date = Arrayf.singleton 0 ;
    ltime       = 0 ;
    params      = [] ;
    prev_ids    = [] ;
    proto_id 	= proto_id ;
    key		= key ;
    xfer_view 	= false ;
    protos      = Arrayf.singleton false ;
    primary     = false ;
    groupd      = false ;
    lwe         = Arrayf.singleton Arrayf.empty ;
    uptime      = uptime
  } in

  let ls = local name endpt vs in
  (ls,vs)

(**************************************************************)

type fields =
  | Vs_coord        of rank
  | Vs_group	    of Group.id
  | Vs_view 	    of t
  | Vs_ltime        of ltime
  | Vs_params	    of Param.tl
  | Vs_prev_ids     of id list
  | Vs_proto_id	    of Proto.id
  | Vs_xfer_view    of bool
  | Vs_key	    of Security.key
  | Vs_clients	    of bool Arrayf.t
  | Vs_groupd       of bool
  | Vs_protos       of bool Arrayf.t
  | Vs_primary      of primary
  | Vs_address      of Addr.set Arrayf.t
  | Vs_out_of_date  of ltime Arrayf.t
  | Vs_lwe          of Endpt.id Arrayf.t Arrayf.t
  | Vs_uptime       of Time.t

(**************************************************************)

let string_of_id view_id = 
  string_of_pair string_of_int Endpt.string_of_id view_id

let string_of_full (ls,vs) =
  sprintf "View.full{endpt=%s;group=%s;coord=%d;view=%s;address=%s;rank=%d;nmembers=%d;view_id=%s;prev_ids=%s;proto_id=%s;xfer_view=%b;key=%s;clients=%s;primary=%b;lwe=%s;uptime=%s}"
    (Endpt.string_of_id ls.endpt)
    (Group.string_of_id vs.group)
    vs.coord
    (to_string vs.view)
    (Arrayf.to_string Addr.string_of_set vs.address)
    ls.rank
    ls.nmembers
    (string_of_id ls.view_id)
    (string_of_list string_of_id vs.prev_ids)
    (Proto.string_of_id vs.proto_id)
    vs.xfer_view
    (Security.string_of_key vs.key)
    (Arrayf.to_string string_of_bool vs.clients)
    vs.primary
    (Arrayf.to_string to_string vs.lwe)
    (Time.to_string vs.uptime)

let string_of_state vs =
  sprintf "View.state{group=%s;coord=%d;view=%s;address=%s;ltime=%d;prev_ids=%s;proto_id=%s;xfer_view=%b;key=%s;clients=%s;primary=%b;lwe=%s;uptime=%s}"
    (Group.string_of_id vs.group)
    vs.coord
    (to_string vs.view)
    (Arrayf.to_string Addr.string_of_set vs.address)
    vs.ltime
    (string_of_list string_of_id vs.prev_ids)
    (Proto.string_of_id vs.proto_id)
    vs.xfer_view
    (Security.string_of_key vs.key)
    (Arrayf.to_string string_of_bool vs.clients)
    vs.primary
    (Arrayf.to_string to_string vs.lwe)
    (Time.to_string vs.uptime)

(**************************************************************)

(* The functional version of the set function.  This has 2
 * problems.  One is that Ocaml does not compile it as a
 * tail recursive function.  The second is that the native
 * code compiler generates really nasty code for it.  The
 * mutable version of the function reduces the assembly code
 * by 4k.  
 *)
(*
let rec set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd vsl = match vsl with
  | [] -> {version=ver;group=gro;view=vie;ltime=lti;proto_id=pro;xfer_view=xfe;key=key;clients=cli;prev_ids=pre;params=par;primary=pri;address=add;coord=coo;out_of_date=ood;lwe=lwe;secchan=sec;uptime=upt;protos=pts;rekey_cleanup=rku;groupd=gpd}
  | hd :: tl -> match hd with
    | Vs_coord	     coo -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_group	     gro -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_view 	     vie -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_address     add -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_ltime       lti -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_params      par -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_prev_ids    pre -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_proto_id    pro -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_xfer_view   xfe -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_key	     key -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_clients     cli -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_protos      pts -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_primary     pri -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_groupd      gpd -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_out_of_date ood -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_lwe         lwe -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_uptime      upt -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_secchan     sec -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl
    | Vs_rekey_cleanup rku -> set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd tl

let set {version=ver;group=gro;view=vie;ltime=lti;proto_id=pro;xfer_view=xfe;key=key;clients=cli;prev_ids=pre;params=par;primary=pri;address=add;coord=coo;out_of_date=ood;lwe=lwe;secchan=sec;uptime=upt;protos=pts;rekey_cleanup=rku;groupd=gpd} vsl = set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe sec upt pts rku gpd vsl
*)

(* This is the version we are using now that Ocaml fixed the
 * problem with "with".  
 *)
let rec set s = function
  | [] -> s
  | hd :: tl -> 
      match hd with
      | Vs_coord	 v -> set { s with coord = v } tl
      | Vs_group	 v -> set { s with group = v } tl
      | Vs_view 	 v -> set { s with view = v } tl
      | Vs_address       v -> set { s with address = v } tl
      | Vs_ltime         v -> set { s with ltime = v } tl
      | Vs_params        v -> set { s with params = v } tl
      | Vs_prev_ids      v -> set { s with prev_ids = v } tl
      | Vs_proto_id      v -> set { s with proto_id = v } tl
      | Vs_xfer_view     v -> set { s with xfer_view = v } tl
      | Vs_key	         v -> set { s with key = v } tl
      | Vs_clients       v -> set { s with clients = v } tl
      | Vs_protos        v -> set { s with protos = v } tl
      | Vs_primary       v -> set { s with primary = v } tl
      | Vs_groupd        v -> set { s with groupd = v } tl
      | Vs_out_of_date   v -> set { s with out_of_date = v } tl
      | Vs_lwe           v -> set { s with lwe = v } tl
      | Vs_uptime        v -> set { s with uptime = v } tl

(**************************************************************)
(* The mutable version of the set function.  See notes above.
 *)
(*
(* This datatype is not exported from this file.
 *)
type mut_state = {
  mutable mut_version       : Version.id ;
  mutable mut_group	    : Group.id ;
  mutable mut_proto_id	    : Proto.id ;
  mutable mut_coord         : rank ;
  mutable mut_ltime         : ltime ;
  mutable mut_primary       : primary ;
  mutable mut_groupd        : bool ;	
  mutable mut_xfer_view	    : bool ;
  mutable mut_key	    : Security.key ;
  mutable mut_prev_ids      : id list ;
  mutable mut_params        : Param.tl ;
  mutable mut_uptime        : Time.t ;
  mutable mut_view 	    : t ;
  mutable mut_clients	    : bool Arrayf.t ;
  mutable mut_address       : Addr.set Arrayf.t ;
  mutable mut_out_of_date   : ltime Arrayf.t ;
  mutable mut_lwe           : Endpt.id Arrayf.t Arrayf.t ;
  mutable mut_protos        : bool Arrayf.t
}

let rec set_loop s = function
  | [] -> ()
  | hd :: tl -> 
      begin
	match hd with
	| Vs_coord	 v -> s.mut_coord <- v
	| Vs_group	 v -> s.mut_group <- v
	| Vs_view 	 v -> s.mut_view <- v
	| Vs_address     v -> s.mut_address <- v
	| Vs_ltime       v -> s.mut_ltime <- v
	| Vs_params      v -> s.mut_params <- v
	| Vs_prev_ids    v -> s.mut_prev_ids <- v
	| Vs_proto_id    v -> s.mut_proto_id <- v
	| Vs_xfer_view   v -> s.mut_xfer_view <- v
	| Vs_key	 v -> s.mut_key <- v
	| Vs_clients     v -> s.mut_clients <- v
	| Vs_protos      v -> s.mut_protos <- v
	| Vs_primary     v -> s.mut_primary <- v
	| Vs_groupd      v -> s.mut_groupd <- v
	| Vs_out_of_date v -> s.mut_out_of_date <- v
	| Vs_lwe         v -> s.mut_lwe <- v
	| Vs_uptime      v -> s.mut_uptime <- v
      end ;
      set_loop s tl

let set vsf l =
  let vsm = {
    mut_version       = vsf.version ;
    mut_group	      = vsf.group ;
    mut_proto_id      = vsf.proto_id ;
    mut_coord         = vsf.coord ;
    mut_ltime         = vsf.ltime ;
    mut_primary       = vsf.primary ;
    mut_groupd        = vsf.groupd ;
    mut_xfer_view     = vsf.xfer_view ;
    mut_key	      = vsf.key ;
    mut_prev_ids      = vsf.prev_ids ;
    mut_params        = vsf.params ;
    mut_uptime        = vsf.uptime ;
    mut_view 	      = vsf.view ;
    mut_clients	      = vsf.clients ;
    mut_address       = vsf.address ;
    mut_out_of_date   = vsf.out_of_date ;
    mut_lwe           = vsf.lwe ;
    mut_protos        = vsf.protos ;
  } in
  set_loop vsm l ;
  let vsf = {
    version           = vsm.mut_version ;
    group	      = vsm.mut_group ;
    proto_id          = vsm.mut_proto_id ;
    coord             = vsm.mut_coord ;
    ltime             = vsm.mut_ltime ;
    primary           = vsm.mut_primary ;
    groupd            = vsm.mut_groupd ;
    xfer_view         = vsm.mut_xfer_view ;
    key	              = vsm.mut_key ;
    prev_ids          = vsm.mut_prev_ids ;
    params            = vsm.mut_params ;
    uptime            = vsm.mut_uptime ;
    view 	      = vsm.mut_view ;
    clients	      = vsm.mut_clients ;
    address           = vsm.mut_address ;
    out_of_date       = vsm.mut_out_of_date ;
    lwe               = vsm.mut_lwe ;
    protos            = vsm.mut_protos
  } in
  vsf
*)
(*
let set {version=ver;group=gro;view=vie;ltime=lti;proto_id=pro;xfer_view=xfe;key=key;clients=cli;prev_ids=pre;params=par;primary=pri;address=add;coord=coo;out_of_date=ood;lwe=lwe;uptime=upt;protos=pts;groupd=gpd} vsl = set_loop ver gro vie lti pro xfe nke key cli pre par pri add coo ood lwe upt pts gpd vsl
*)
(**************************************************************)

let endpt_full (ls,vs) = (ls.endpt, ls.addr)

let coord_full vs =
  let endpt = Arrayf.get vs.view vs.coord in
  let addr  = Arrayf.get vs.address vs.coord in
  (endpt,addr)

(**************************************************************)

let arrayf_combine6 a b c d e f =
  let len = Arrayf.length a in
  if Arrayf.length b <> len
  || Arrayf.length c <> len
  || Arrayf.length d <> len
  || Arrayf.length e <> len
  || Arrayf.length f <> len
  then failwith sanity ;

  Arrayf.init len (fun i ->
    ((Arrayf.get a i),
     (Arrayf.get b i),
     (Arrayf.get c i),
     (Arrayf.get d i),
     (Arrayf.get e i),
     (Arrayf.get f i))
  )

let arrayf_split6 abcde =
  let a = Arrayf.map (fun (x,_,_,_,_,_) -> x) abcde in
  let b = Arrayf.map (fun (_,x,_,_,_,_) -> x) abcde in
  let c = Arrayf.map (fun (_,_,x,_,_,_) -> x) abcde in
  let d = Arrayf.map (fun (_,_,_,x,_,_) -> x) abcde in
  let e = Arrayf.map (fun (_,_,_,_,x,_) -> x) abcde in
  let f = Arrayf.map (fun (_,_,_,_,_,x) -> x) abcde in
  (a,b,c,d,e,f)

let fail vs failed =
  if Arrayf.length vs.view <> Arrayf.length failed then
    failwith "fail: failed array different size from view" ;
  let members = arrayf_combine6 vs.view vs.address vs.clients vs.out_of_date vs.lwe vs.protos in
  let members = Arrayf.combine failed members in
  let members = Arrayf.filter (fun (f,_) -> not f) members in
  let members = Arrayf.map snd members in
  let view,address,clients,out_of_date,lwe,protos = arrayf_split6 members in
  
  set vs [
    Vs_view view ;
    Vs_address address ;
    Vs_clients clients ;
    Vs_out_of_date out_of_date ;
    Vs_lwe lwe ;
    Vs_protos protos
  ]

(* NOTE: the PRIMARY layer depends on the
 * coordinator's view id being the first element in
 * this list.  
 *)
let merge mergers =
  match mergers with
  | [] -> failwith "merge:zero view states"
  | primary::_ ->
      let prev_ids = List.map id_of_state mergers in
      
      (* Take the maximum of the ltimes, plus one.
       *)
      let ltime = List.map (fun vs -> vs.ltime) mergers in
      let ltime = succ (List.fold_left max 0 ltime) in

      (* Take the minimum of the uptimes.
       *)
      let uptime = List.map (fun vs -> vs.uptime) mergers in
      let uptime = List.fold_left min (List.hd uptime) uptime in

      let mergers = 
 	List.map (fun vs -> 
	  arrayf_combine6 vs.view vs.address vs.clients vs.out_of_date vs.lwe vs.protos
	) mergers 
      in
      let mergers = Arrayf.concat mergers in
      let view,address,clients,out_of_date,lwe,protos = arrayf_split6 mergers in

      set primary [
        Vs_view view ;
        Vs_address address ;
        Vs_clients clients ;
        Vs_out_of_date out_of_date ;
        Vs_prev_ids prev_ids ;
        Vs_ltime ltime ;
        Vs_lwe lwe ;
        Vs_uptime uptime ;
        Vs_protos protos
      ]

(**************************************************************)
