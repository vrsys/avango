(**************************************************************)
(* OPTREKEY.ML : D_WGL algorithm *)
(* Author: Ohad Rodeh 2/99  *)
(**************************************************************)
(* This layer acts in concert with PERFREKEY and REALKEYS to 
 * implement the DWGL rekey algorithm. Its function is to gather 
 * information about the seperate group components each possesing
 * its own complete key-tree. 
 * 
 * 1. Each subgroup leader sends the leader its key-tree structure. 
 *    This is the key-tree stripped of the REAL key values. Only
 *    symbolic key names are attached. 
 * 2. The leader composes the subtrees together and computes a new
 *    keytree. It multicasts the new keytree to the group. 
 * 3. Member get the new keytree and start the REALKEYS layer. 
 * 
 * TODO: 
 *   Write a version that works for an empty view change. The leader
 *   replaces the top (symbolic) key with a new key and multicasts. 
 *   Receivers do a ERekeyPrcl[Tree(old_tree, new_tree, full)]. We
 *   can remove the option here (!).
*)
(**************************************************************)
open Util
open Layer
open View
open Event
open Trans
open Shared
open Tdefs
(**************************************************************)
let name = Trace.filel "OPTREKEY"
(**************************************************************)
type xfer_msg = 
  | Start
  | MyTree of Tree.z * rank list
  | FinalTree of bool * Tree.z * (Tree.actions_h * (Tree.key * Tree.key) list)

(**************************************************************)

type tree_state = {
  tree      : Tree.z ;
  chief     : bool ;
  subgroup  : rank list
}
    
type state = {
  tree_ref           : Tree.z ref ;
  blocked            : Once.t ;
  mutable new_state  : (Tree.z * Tree.sent) option;
  mutable ts         : tree_state ;
  mutable xfer_vct   : Tree.z list ;
  sent_final         : Once.t ;
  xfer_orig          : bool array ;

  (* For PERF measurements *)
  mutable init_time  : float
}
	       
(**************************************************************)
	       
let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "chief=%b" s.ts.chief ;
  sprintf "subgroup=%s" (string_of_int_list s.ts.subgroup);
  sprintf "endpts=%s" (string_of_int_list s.ts.subgroup);
  sprintf "tree=%s" (Tree.string_of_z s.ts.tree)
|])

let string_of_msg = function
  | Start       -> "Start"
  | MyTree _    -> "MyTree"
  | FinalTree _ -> "FinalTree"
(**************************************************************)

let string_of_endpt e = Endpt.string_of_id_very_short e 
let string_of_fes fea = string_of_list string_of_endpt (Arrayf.to_list fea)
let string_of_es ea = string_of_list string_of_endpt ea

(* Compute the subtree to which the member belongs.
*)
let init s (ls,vs) = 
  try 
  if ls.nmembers > 255 then failwith "The optimized rekey protocols do not
work for more than 255 members due to technical difficulties. Please use
the regular rekeying protocol";
  let z = 
    let z = !(s.Layer.tree) in
    if z=Tree.zempty then Tree.zleaf (Member.of_int ls.rank) else z
  in
  let members = List.map Member.int_of (Tree.members_of_z z) in
  assert (List.mem ls.rank members);
  {
    tree_ref       = s.Layer.tree ;
    blocked        = Once.create "blocked" ;
    new_state      = None ;
    ts = {
      tree = z;
      chief = Tree.zam_leader z (Member.of_int ls.rank) ; 
      subgroup = members
    };
    xfer_vct       = [] ;
    sent_final     = Once.create "sent_final" ;
    xfer_orig      = Array.create ls.nmembers false ;

    init_time      = 0.0
  }
  with e -> eprintf "OPTREKEY exception in init\n"; exit 0

(**************************************************************)
let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  try 

  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name (string_of_int ls.rank) in
  let log1 = Trace.log2 (name^"1") "" in
  let log2 = Trace.log2 (name^"2") "" in
  let logm = Trace.log2 (name^"M") "" in

  (* [translate view view_new old_tree] 
   * Translate a tree from the current view to a new 
   * view. 
   *)
  let translate vs' tree =
    let diff = ref [] in
    Arrayf.iteri (fun i e -> 
      if not (Arrayf.mem e vs'.view) then diff:=i::!diff
    ) vs.view;
    let diff_m = List.map Member.of_int !diff in
    try 
      let subtree = Tree.zremove_members_spec tree (Member.of_int ls.rank) diff_m in
      let new_tree = Tree.zmap (function rank -> 
	let rank = Member.int_of rank in
	let e = Arrayf.get vs.view rank in
	Member.of_int (Arrayf.index e vs'.view)
      ) ident subtree in
      log2 (fun () -> sprintf "T=%s | NT=%s | diff=%s" 
	  (Tree.string_of_z tree)
	  (Tree.string_of_z new_tree)
	  (string_of_int_list !diff));
      subtree,new_tree
    with _ -> 
      eprintf "Failed in translate\n";
      eprintf "view=%s view'=%s diff=%s\n"
	(string_of_fes vs.view) (string_of_fes vs'.view) (string_of_int_list !diff);
      exit 0
  in
  
  let recv_done () =
    match s.new_state with 
      | None -> log1 (fun () -> "No stored state"); ()
      | Some (new_tree, new_full) -> 
	  s.new_state <- None ;
	  let members = List.map Member.int_of (Tree.members_of_z new_tree) in
	  s.ts <- {
	    tree = new_tree;
	    chief =  Tree.zam_leader new_tree (Member.of_int ls.rank) ;
	    subgroup = members
	  };
	  log1 (fun () -> sprintf "n=%d %s chief=%b FinalTree=%s" ls.nmembers 
	      (string_of_endpt ls.endpt) s.ts.chief 
	      (Tree.string_of_z s.ts.tree));
  in
  
  let recv_final_tree view_type new_tree inst =
    log1 (fun () -> "recv_final_tree");
    s.new_state <- Some (new_tree, inst);
    upnm (create name ERekeyPrcl[Tree (view_type,new_tree); TreeAct inst])
  in
  
  let send_final_tree view_type new_tree inst = 
    log (fun () -> sprintf "%d FinalTree=%s" ls.rank (Tree.string_of_z new_tree));
    logm (fun () -> sprintf " %d FinalTree= %d" ls.nmembers 
      (String.length (Marshal.to_string (FinalTree (view_type,new_tree,inst)) [])));
    recv_final_tree view_type new_tree inst ;
    dnlm (castEv name) (FinalTree (view_type, new_tree, inst))
  in
  

  (* Check if all subgroup leaders sent their information.
   *)
  let check_recv_all origin tree rl = 
    try 
      assert (ls.rank = 0);
      s.xfer_vct <- tree :: s.xfer_vct;
      List.iter (function i -> s.xfer_orig.(i) <- true) rl; 
      if array_for_all Util.ident s.xfer_orig then (

	(*
	let now = Unix.gettimeofday () in
	logm (fun () -> sprintf "%d Waited for %d components :%1.5f." 
	    ls.nmembers (List.length s.xfer_vct) (now -. s.init_time ));
	s.init_time <- now;
*)

	log (fun () -> "Recv all");
	Once.set s.sent_final ;
	let new_tree,al = Tree.merge (List.map Tree.unzip s.xfer_vct) in
	let ((actions_h,_,_,bad_set) as full) = 
	  try Tree.actions_of_merge new_tree al 
	  with e -> eprintf "Error in actions_of_merge\n"; 
	    eprintf "Actions=%s" (Tree.string_of_alist al);
	    eprintf "Trees=%s\n" (string_of_list Tree.string_of_z s.xfer_vct);
	    eprintf "Tree=%s\n" (Tree.string_of_t new_tree);
	    raise e
	in

	assert (
	  let _,result,_ = Tree.self_check false new_tree full in 
	  result
	);
	
	send_final_tree true (Tree.zip new_tree) (actions_h,bad_set)
      )
    with e -> eprintf "Error in Recvall"; raise e
  in      

  (* A subleader (chief) sends his (symbolic) key-tree to the 
   * view leader. 
   *)
  let send_component_to_leader () = 
    if s.ts.chief then (
      log (fun () -> sprintf "n=%d %d->%d MyTree=%s rl=%s" 
	  ls.nmembers ls.rank 0
	  (Tree.string_of_z s.ts.tree)
	  (string_of_int_list s.ts.subgroup));
      
      if ls.rank=0 then check_recv_all 0 s.ts.tree s.ts.subgroup 
      else dnlm (sendPeer name 0) (MyTree(s.ts.tree, s.ts.subgroup))
    )
  in
  
  (* Only at the leader. 
   * Hack --- do nothing in an empty view change.
   *)
  let init_rekey () = 
    if (List.length s.ts.subgroup = ls.nmembers) then (
      let new_tree, al = Tree.new_top (Tree.unzip s.ts.tree) in
      let ((actions_h,_,_,bad_set) as full) = 
	Tree.actions_of_merge new_tree al in
      send_final_tree false (Tree.zip new_tree) (actions_h,bad_set)
    ) else (
      log (fun () -> "Start"); 
      (*s.init_time <- Unix.gettimeofday ();*)
      dnlm (castEv name) Start ;
      send_component_to_leader ()
    )
  in

  let up_hdlr ev abv () = up ev abv

  and uplm_hdlr ev hdr =
    if not (Once.isset s.blocked) then 
      let origin = getPeer ev in 
      match getType ev,hdr with
	  
  | ECast _, Start -> send_component_to_leader ()

  | ESend _, MyTree (tree, rl) when ls.rank=0 -> 
      check_recv_all origin tree rl 
	
  | ECast _, FinalTree (view_type,tree,inst) when origin=0 ->  
      recv_final_tree view_type tree inst

  | typ, msg -> 
      eprintf "Unknown message type received. origin=%d type=%s msg=%s\n" 
	origin (Event.string_of_type typ) (string_of_msg msg);
      failwith ""

	
  and upnm_hdlr ev = match getType ev with

    (* EBlock: Mark that we are now blocking.
     *)
  | EBlock ->
      Once.set s.blocked ;
      upnm ev
	
  (* Translate the old key-tree into the new view. 
   * Normalize the key set, and set up the tree. REALKEYS
   * should also set it's key set strait. 
   *)
  | EView ->
      let new_vs = getViewState ev in
      let subtree,new_tree = translate new_vs s.ts.tree in
      s.tree_ref := new_tree;
      upnm (set name ev[Tree (false, subtree)]) 

  (* Received an initialize Rekey command. 
   *)
  | ERekeyPrcl -> 
      assert (ls.rank=0);
      init_rekey ()
	
  (* Rekey completed, commit the new tree.
   *)
  | ERekeyCommit -> ()

  (* Cleanup the layer. pass up to REALKEYS.
   *)
  | ERekeyCleanup -> 
      s.new_state <- None ;
      s.ts        <- {
	tree  = Tree.zleaf (Member.of_int ls.rank);
	chief = true ;
	subgroup = [ls.rank]
      };
      s.xfer_vct <- [];
      upnm ev

  | EDump -> ( dump vf s ; upnm ev )
	
  | _ -> upnm ev
	
  and dn_hdlr ev abv = dn ev abv ()
    
  and dnnm_hdlr ev = match getType ev with 
      (* REALKEYS completed processing the instructions from OPTREKEY. 
       * New keys have been chosen and distributed. Set the new key-tree
       * structure, and pass the event down to PREFREKEY. 
       *)
    | ERekeyPrcl -> 
	recv_done ();
	dnnm ev
	  
    | _ -> dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}
  with e -> (eprintf "failure in OPTREKEY\n"; exit 0)
     
let l args vf = Layer.hdr init hdlrs None NoOpt args vf
		  
let _ = Layer.install name l

(**************************************************************)
    
