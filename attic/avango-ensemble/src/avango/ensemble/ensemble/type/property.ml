(**************************************************************)
(* PROPERTY.ML *)
(* Author: Mark Hayden, 12/96 *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "PROPERTY"
let failwith s = Trace.make_failwith name s
(**************************************************************)

type id =
  | Agree				(* agreed (safe) delivery *)
  | Gmp					(* group-membership properties *)
  | Sync				(* view synchronization *)
  | Total				(* totally ordered messages *)
  | Heal				(* partition healing *)
  | Switch              		(* protocol switching *)
  | Auth				(* authentication *)
  | Causal		        	(* causally ordered broadcasts *)
  | Subcast			        (* subcast pt2pt messages *)
  | Frag				(* fragmentation-reassembly *)
  | Debug				(* debugging layers *)
  | Scale				(* scalability *)
  | Xfer				(* state transfer *)
  | Cltsvr           			(* client-server management *)
  | Suspect		        	(* failure detection *)
  | Flow                	        (* flow control *)
  | Migrate         			(* process migration *)
  | Privacy		        	(* encryption of application data *)
  | Rekey				(* support for rekeying the group *)
  | OptRekey				(* optimized rekeying protocol *)
  | DiamRekey                           (* Diamond rekey algorithm *)
  | Primary			        (* primary partition detection *)
  | Local				(* local delivery of messages *)
  | Slander			        (* members share failure suspiciions *)
  | Asym			        (* overcome asymmetry *)

  | Drop				(* randomized message dropping *)
  | Pbcast				(* Hack: just use pbcast prot. *)
  | Zbcast                              (* Use Zbcast protocol. *)
  | Gcast                               (* Use gcast protocol. *)
  | Dbg                                 (* user controlled network partition *)
  | Dbgbatch                            (* batch mode network emulation *)
  | P_pt2ptwp                           (* Use experimental pt2pt flow-control protocol *)

(**************************************************************)

let mapping = [|
  "GMP", Gmp ;
  "SYNC", Sync ;
  "TOTAL", Total ;
  "HEAL", Heal ;
  "SWITCH", Switch ;
  "AUTH", Auth ;
  "CAUSAL", Causal ;
  "SUBCAST", Subcast ;
  "FRAG", Frag ;
  "DEBUG", Debug ;
  "SCALE", Scale ;
  "XFER", Xfer ;
  "CLTSVR", Cltsvr ;
  "SUSPECT", Suspect ;
  "DROP", Drop ;
  "PBCAST", Pbcast ;
  "ZBCAST", Zbcast ;
  "GCAST", Gcast ;
  "DBG", Dbg ;
  "DBGBATCH", Dbgbatch ;
  "FLOW", Flow ;
  "MIGRATE", Migrate ;
  "PRIVACY", Privacy ;
  "REKEY", Rekey ;
  "OPTREKEY", OptRekey ;
  "DIAMREKEY", DiamRekey ;
  "PRIMARY", Primary ;
  "LOCAL", Local ;
  "SLANDER", Slander ;
  "ASYM", Asym ;
  "P_PT2PTWP", P_pt2ptwp
|]

let all () = Array.map snd mapping
let string_of_id id = Util.string_of_id name mapping id
let id_of_string s = Util.id_of_string name mapping s

(**************************************************************)

type r = {
    mutable agree : bool ;
    mutable gmp : bool ;
    mutable sync : bool ;
    mutable total : bool ;
    mutable heal : bool ;
    mutable switch : bool ;
    mutable auth : bool ;
    mutable causal : bool ;
    mutable subcast : bool ;
    mutable frag : bool ;
    mutable debug : bool ;
    mutable scale : bool ;
    mutable xfer : bool ;
    mutable cltsvr : bool ;
    mutable suspect : bool ;
    mutable pbcast : bool ;
    mutable zbcast : bool ;
    mutable gcast : bool ;
    mutable drop : bool ;
    mutable dbg : bool ;
    mutable dbgbatch : bool ;
    mutable flow : bool ;
    mutable migrate : bool ;
    mutable privacy : bool ;
    mutable rekey : bool ;
    mutable optrekey : bool ;
    mutable diamrekey : bool ;
    mutable primary : bool ;
    mutable local : bool ;
    mutable slander : bool ;
    mutable asym : bool ;
    mutable p_pt2ptwp : bool 
} 

(**************************************************************)

let flatten props =
  let r = {
    agree = false ;
    gmp = false ;
    sync = false ;
    total = false ;
    heal = false ;
    switch = false ;
    auth = false ;
    causal = false ;
    subcast = false ;
    frag = false ;
    debug = false ;
    scale = false ;
    xfer = false ;
    cltsvr = false ;
    suspect = false ;
    pbcast = false ;
    zbcast = false ;
    gcast = false ;
    drop = false ;
    dbg = false ;
    dbgbatch = false ;
    flow = false ;
    migrate = false ;
    privacy = false ;
    rekey = false ;
    optrekey = false ;
    diamrekey = false ;
    primary = false ;
    local = false ;
    slander = false ;
    asym = false ;
    p_pt2ptwp = false 
  } in
  List.iter (function
  | Agree	-> r.agree <- true
  | Gmp		-> r.gmp <- true
  | Sync	-> r.sync <- true
  | Total	-> r.total <- true
  | Heal	-> r.heal <- true
  | Switch	-> r.switch <- true
  | Auth	-> r.auth <- true
  | Causal	-> r.causal <- true
  | Subcast	-> r.subcast <- true
  | Frag	-> r.frag <- true
  | Debug	-> r.debug <- true
  | Scale	-> r.scale <- true
  | Xfer	-> r.xfer <- true
  | Cltsvr	-> r.cltsvr <- true
  | Suspect	-> r.suspect <- true
  | Pbcast	-> r.pbcast <- true
  | Zbcast      -> r.zbcast <- true
  | Gcast       -> r.gcast <- true
  | Drop        -> r.drop <- true
  | Dbg         -> r.dbg <- true
  | Dbgbatch    -> r.dbgbatch <- true
  | Flow        -> r.flow <- true
  | Migrate     -> r.migrate <- true
  | Privacy     -> r.privacy <- true
  | Rekey       -> r.rekey <- true
  | OptRekey    -> r.optrekey <- true
  | DiamRekey   -> r.diamrekey <- true
  | Primary     -> r.primary <- true
  | Local       -> r.local <- true
  | Slander     -> r.slander <- true
  | Asym        -> r.asym <- true
  | P_pt2ptwp   -> r.p_pt2ptwp <- true
  ) props ;
  r

(**************************************************************)

let choose props =
  let log = Trace.log2 name "choose" in
  let p = flatten props in

  let stack =
    if p.zbcast then (
      let stack = 
	["Top"]::
	["Top_appl"]::
	(if p.debug then ["Chk_fifo"] else []) ::
	["Pt2pt:Zbcast"] ::
	(if p.gcast then ["Gcast"] else []) ::
	(if p.drop then ["Drop"] else []) ::
	(if p.dbg then ["Dbg"] else []) ::
	(if p.dbgbatch then ["Dbgbatch"] else []) ::
	["Bottom"] :: [[]] 
      in
      let stack = List.flatten stack in
      let stack = String.concat ":" stack in
      stack
    ) else if p.pbcast then (
      let stack = 
	["Top"]::
	["Top_appl"]::
	(if p.debug then ["Chk_fifo"] else []) ::
	["Pt2pt:Pbcast"] ::
        (if p.gcast then ["Gcast"] else []) ::
	(if p.drop then ["Drop"] else []) ::
	(if p.dbg then ["Dbg"] else []) ::
	(if p.dbgbatch then ["Dbgbatch"] else []) ::
	["Bottom"] :: [[]] 
      in
      let stack = List.flatten stack in
      let stack = String.concat ":" stack in
      stack
    ) else (
      (* Todo: check for other incompatibilities.
       *)
      if p.agree && p.total then
	failwith "cannot have both Agree and Total properties" ;

      let stack = 
	["Top"] ::
	(if p.heal then ["Heal"] else []) ::
	(if p.switch then ["Switch"] else []) ::
	(if p.migrate then ["Migrate"] else []) ::
	(if p.xfer then ["Xfer"] else []) ::
	(if p.primary then ["Primary:Present"] else []) ::

	(* All rekeying code must be above Exchange.
	 * Exchange must be above GMP 
	 *)
	(if p.rekey && p.debug then ["Chk_secchan:Chk_rekey"] else []) :: 
	(if p.rekey then ["Rekey_dt:PerfRekey:Secchan"] else []) :: 
	(if p.optrekey then ["RealKeys:OptRekey:PerfRekey:Secchan"] else []) ::
	(if p.diamrekey then ["Rekey_diam:PerfRekey:Secchan"] else []) ::
	(if p.auth then ["Exchange"] else []) :: 

	(if p.gmp then ["Leave:Inter:Intra:Elect:Merge"] else []) ::
	(if p.slander then ["Slander"] else []) ::
	(if not (p.sync) then [] else ["Sync"]) ::
	(if not (p.suspect) then [] else 
	  if p.scale then ["Pr_suspect"] else ["Suspect"]) ::
	(if p.scale then ["Pr_stable"] else ["Stable"]) ::
	["Vsync"] ::
	["Frag_Abv"] ::

	(if p.total then ["Partial_appl"] else ["Top_appl"]) ::

	(if p.debug && p.sync then ["Chk_sync"] else []) ::
	(if p.cltsvr then ["Cltsvr"] else []) ::
	(if p.total && p.debug then ["Chk_total"] else []) ::

	(* Total ordering implies local delivery.
	 *)
	(if p.total then ["Seqbb:Collect"] else 
	  if p.agree then ["Tops:Collect"] else 
	    if p.local then ["Local"] else []) ::

	(if p.debug && p.agree then ["Chk_total:Chk_causal"] else []) ::
	(if p.debug && p.causal then ["Chk_causal"] else []) :: 
	(if p.causal then ["Causal"] else []) ::
	
	(if p.privacy then ["Encrypt"] else []) ::
	(if p.frag then ["Frag"] else []) ::

	(* Handle debug, flow control, and subcast.  This is
         * a bit confusing to get right.  Basically, people
	 * should not use subcast.   The problem with Debug
	 * is that it must go below the Mflow protocol because
	 * non-application messages are can be reordered and
	 * it must go above the Pt2pt protocol.
	 *)
	(* Chk_fifo must also be below total ordering
	 * protocols because of the NoTotal flag on
	 * membership messages.  
	 *)
	let pt2ptw = 
	  if p.p_pt2ptwp then "Pt2ptwp" else "Pt2ptw" in
	[match p.debug, p.flow, p.subcast with
	| false,false,false -> "Pt2pt"
	| false,false, true -> "Subcast"
	| false, true,false -> pt2ptw ^":Mflow:Pt2pt"
	| false, true, true -> "Subcast:Window"
	| true ,false,false -> "Chk_fifo:Pt2pt"
	| true ,false, true -> "Chk_fifo:Subcast"
	| true , true,false -> pt2ptw ^":Mflow:Chk_fifo:Pt2pt"
	| true , true, true -> "Chk_fifo:Subcast:Window"] :: (* Is this last line ok? *)

	["Mnak"] ::

	(if p.gcast then ["Gcast"] else []) ::
        (if p.asym then ["Asym"] else []) ::
	(if p.drop then ["Drop"] else []) ::
	(if p.dbg then ["Dbg"] else []) ::
	(if p.dbgbatch then ["Dbgbatch"] else []) ::
	["Bottom"] :: 
	[[]]
      in
      let stack = List.flatten stack in

      let stack =
	match List.rev stack with
	| "Bottom"::"Mnak"::"Pt2pt"::(*"Frag"::*)"Top_appl"::stack ->
	    List.rev ("FPMB"::stack)
	| _ -> stack
      in

      let stack = String.concat ":" stack in
      stack
    ) 
  in
  let stack = Proto.id_of_string stack in
  log (fun () -> sprintf "properties=%s" (string_of_list string_of_id props)) ;
  log (fun () -> sprintf "stack=%s" (Proto.string_of_id stack)) ;
  stack

(**************************************************************)

let vsync = [Gmp;Sync;Heal;Frag;Suspect;Flow;Slander]
(*let vsync = [Gmp;Sync;Heal;Switch;Frag;Suspect;Subcast;Slander]*)

let total = vsync @ [Total]
let scale = vsync @ [Scale]
let fifo = [Frag;Flow]
let causal = vsync @ [Causal]
let local =  vsync @ [Local]

(**************************************************************)

let strip_groupd properties =
  Lset.subtract properties [Gmp;Sync;Heal;Switch;Suspect;Primary]

(**************************************************************)
