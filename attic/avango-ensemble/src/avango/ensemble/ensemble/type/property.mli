(**************************************************************)
(* PROPERTY.MLI *)
(* Author: Mark Hayden, 12/96 *)
(**************************************************************)

type id =
  | Agree				(* agreed (safe) delivery *)
  | Gmp					(* group-membership properties *)
  | Sync				(* view synchronization *)
  | Total				(* totally ordered messages *)
  | Heal				(* partition healing *)
  | Switch				(* protocol switching *)
  | Auth				(* authentication *)
  | Causal				(* causally ordered broadcasts *)
  | Subcast				(* subcast pt2pt messages *)
  | Frag				(* fragmentation-reassembly *)
  | Debug				(* adds debugging layers *)
  | Scale				(* scalability *)
  | Xfer				(* state transfer *)
  | Cltsvr				(* client-server management *)
  | Suspect				(* failure detection *)
  | Flow				(* flow control *)
  | Migrate				(* process migration *)
  | Privacy				(* encryption of application data *)
  | Rekey				(* support for rekeying the group *)
  | OptRekey				(* optimized rekeying protocol *)
  | DiamRekey                           (* Diamond rekey algorithm *)
  | Primary				(* primary partition detection *)
  | Local				(* local delivery of messages *)
  | Slander				(* members share failure suspiciions *)
  | Asym			        (* overcome asymmetry *)

    (* The following are not normally used.
     *)
  | Drop				(* randomized message dropping *)
  | Pbcast				(* Hack: just use pbcast prot. *)
  | Zbcast                              (* Use Zbcast protocol. *)
  | Gcast                               (* Use gcast protocol. *)
  | Dbg                                 (* on-line modification of network topology *)
  | Dbgbatch                            (* batch mode network emulation *)
  | P_pt2ptwp                           (* Use experimental pt2pt flow-control protocol *)

(* Create protocol with desired properties.
 *)
val choose : id list -> Proto.id

(* Common property lists.
 *)
val vsync : id list			(* Fifo virtual synchrony *)
val causal : id list			(* vsync + Causal *)
val total : id list			(* vsync + Total *)
val scale : id list			(* vsync + Scale *)
val fifo : id list			(* only Fifo (no membership) *)
val local : id list                     (* vsync + Local *)

val string_of_id : id -> string
val id_of_string : string -> id
val all : unit -> id array

(**************************************************************)

(* Strip unnecessary properties for use of groupd.
 *)
val strip_groupd : id list -> id list

(**************************************************************)
