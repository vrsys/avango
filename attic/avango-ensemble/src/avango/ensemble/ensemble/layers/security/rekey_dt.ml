(**************************************************************)
(* REKEY_DT.ML : switching to a new key using a dynamic tree structure *)
(* Authors: Ohad Rodeh *)
(**************************************************************)
open Util
open Layer
open View
open Event
open Trans

(**************************************************************)
let name = Trace.filel "REKEY_DT"
(**************************************************************)

module Dt = Mrekey_dt
type tree = Dt.t 

type header =
  | NoHdr
  | Start
  | Component of tree
  | NewTree of tree

(**************************************************************)

type state = {
  dyn_tree_r         : tree ref ;
  mutable tree       : tree ;
  mutable comp_l     : tree list ;
  rcv_comp_vct       : bool array;
  mutable key_sug    : Security.key;
  mutable new_tree   : tree option; 
  mutable children   : rank list ;
  mutable father     : rank option;
  mutable blocking   : bool
} 
	       
(**************************************************************)
	       
let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "tree=%s\n" (Dt.string_of_t s.tree);
  sprintf "blocking=%b \n" s.blocking;
|])

(**************************************************************)

let init s (ls,vs) = 
  let tree = !(s.Layer.dyn_tree) in
  let tree = 
    if tree = Dt.empty then Dt.singleton ls.rank else tree in
  let father = Dt.father tree ls.rank in
  let rcv_comp_vct = 
    if ls.am_coord then Array.create ls.nmembers false
    else Array.create 0 false in
  { 
    dyn_tree_r= s.Layer.dyn_tree ;
    tree      = tree ;
    comp_l    = [];
    rcv_comp_vct = rcv_comp_vct ;
    new_tree  = None ;
    key_sug   = Security.NoKey;
    children  = [] ;
    father    = father ;
    blocking  = false 
  }

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name (sprintf "%d" ls.rank) in
  let log1 = Trace.log2 (name^"1") (sprintf "%d" ls.rank) in
  let logs = Trace.log2 (name^"S") (sprintf "%d" ls.rank) in

  let send_key_to_children key_buf = 
    List.iter (fun i -> 
      let ev = (create name ESecureMsg[Peer i; SecureMsg key_buf]) in
      dnlm ev NoHdr
    ) s.children
  in

  let enter_rekey () = 
    if not s.blocking then 
      match s.key_sug,s.new_tree with 
	| Security.Common _, Some new_tree -> 
	    if s.children <> [] then 
	      send_key_to_children (Security.buf_of_key s.key_sug);
	    dnnm (create name ERekeyPrcl[AgreedKey s.key_sug]) 
	| _ -> ()
  in

  (*  Only the root of the tree should create a new key and send
   * it off. 
   *)
  let got_new_tree peer new_tree = 
    s.new_tree <- Some new_tree ;
    log1 (fun () -> sprintf " <- %d NewTree(%s), old=%s" 
	peer (Dt.string_of_t new_tree) (Dt.string_of_t s.tree));
    s.children <- Dt.children new_tree ls.rank ;
    s.father <- Dt.father new_tree ls.rank ;
    if s.father = None then (
      log (fun () -> "I'm root, creating a new key");
      s.key_sug <- Shared.Prng.create_key ();
    ) ;
    enter_rekey ()
  in
  
  let got_component peer c = 
    assert (ls.am_coord);
    s.comp_l <- c :: s.comp_l;
    let l = Dt.members_of_t c in
    List.iter (fun i -> s.rcv_comp_vct.(i) <- true) l;
    log (fun () -> 
      sprintf "<- %d Component %s" peer (Dt.string_of_t c)
    );
    if not s.blocking 
      && array_for_all ident s.rcv_comp_vct then (
	let new_tree = Dt.merge s.comp_l in
	log (fun () -> sprintf "merge = %s" (Dt.string_of_t new_tree));
	logs (fun () -> sprintf "merge = \n%s" (Dt.pretty_string_of_t new_tree));
	logs (fun () -> sprintf "comp_l = %s\n" (string_of_list Dt.string_of_t s.comp_l));
	dnlm (castEv name) (NewTree new_tree);
	s.comp_l <- [];
	got_new_tree ls.rank new_tree 
    )
  in    
  
  let up_hdlr ev abv () = up ev abv
    
  and uplm_hdlr ev hdr = match getType ev,hdr with
  | ECast _, Start -> (

      if not s.blocking then 
	match s.father with 
	  | None -> 
	      dnlm (sendPeer name 0) (Component s.tree)
	  | _ -> ()
    )
      
  | ESend _, Component c -> 
      got_component (getPeer ev) c
	
  | ECast _, NewTree nt -> 
      got_new_tree (getPeer ev) nt
	
  (* Got a key from the parent.  
   *)
  | ESecureMsg,NoHdr ->
      log (fun () -> sprintf "ESecureMsg(new_key) <- %d" (getPeer ev));
      let key = getSecureMsg ev in
      s.key_sug <- Security.key_of_buf key ;
      enter_rekey ()

  | _ -> failwith unknown_local
      
  and upnm_hdlr ev = match getType ev with
      
  (* Start Rekey, from PERFREKEY.
   *)
  | ERekeyPrcl -> 
      assert (ls.am_coord);
      if not s.blocking then (
	(match s.father with 
	  | None -> 
	      got_component ls.rank s.tree 
	  | Some _ -> ()
	);
	dnlm (castEv name) Start
      )

  (* Cleanup the layer. This is an empty call for this layer. 
   * It is bounced down to PERFREKEY. 
   *)
  | ERekeyCleanup -> 
      log (fun () -> "ERekeyCleanup");
      s.tree <- Dt.singleton ls.rank ;
      s.children <- [];
      s.father <- None;
      dnnm ev

  (* Rekey completed, commit the new tree.
   *)
  | ERekeyCommit -> (
      log (fun () -> "ERekeyCommit");
      match s.new_tree with 
	| None -> failwith "Internal Error, new_tree is empty"
	| Some new_tree -> 
	    s.tree <- new_tree;
	    logs (fun () -> sprintf "ERekeyCommit( %s )" (Dt.string_of_t s.tree));
	    s.new_tree <- None
    )	    

  (* EBlock: Mark that we are now blocking.
   *)
  | EBlock ->
      s.blocking <- true;
      upnm ev
	
  | EDump -> ( dump vf s ; upnm ev )

  (* Need to set my_component
   *)
  | EView -> 
      let new_vs = getViewState ev in
      if new_vs.view <> vs.view then (
	(* First, remove all the members that are leaving. 
	 * split the tree accordingly.
	 *)
	let new_view = Lset.inject new_vs.view
	and cur_view = Lset.inject vs.view in
	let leave = Lset.subtracta cur_view new_view in
	let leave = Lset.project leave in
	let leave = Arrayf.to_array leave in
	let leave = Array.map (function e -> 
	  Arrayf.index e vs.view 
	) leave in
 	let leave = Array.to_list leave in
	log (fun () -> sprintf "split %s %s %d %s " 
	    (string_of_int_list (Dt.members_of_t s.tree))
	    (Dt.string_of_t s.tree) ls.rank (string_of_int_list leave));
	let new_tree = Dt.split s.tree  ls.rank leave in
	let new_tree = Mrekey_dt.map (function i -> 
	  let e = Arrayf.get vs.view i in
	  Arrayf.index e new_vs.view 
	) new_tree in
	s.tree <- new_tree;
      );
      s.dyn_tree_r := s.tree;
      logs (fun () -> sprintf "EView %s" (Dt.string_of_t s.tree));
      upnm ev
	
  | _ -> upnm ev
      
  and dn_hdlr ev abv = dn ev abv ()
			 
  and dnnm_hdlr = dnnm
		    
in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}
     
let l args vf = Layer.hdr init hdlrs None NoOpt args vf
		  
let _ = Layer.install name l

(**************************************************************)
    
