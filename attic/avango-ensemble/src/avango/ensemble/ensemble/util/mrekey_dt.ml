(**************************************************************)
(* MREKEY_DT.ML : Dynamic Tree rekey support module *)
(* Author: Ohad Rodeh, 4/2000 *)
(*************************************************************************)
open Trans
open Util 
(*************************************************************************)
let name = Trace.filel "MREKEY_DT"
let log = Trace.log name 
(*************************************************************************)

type 'a tree = 
    Nil
  | Br of 'a tree * 'a  * 'a tree


type t = rank tree

let empty = Nil 
      
let rec string_of_t = function 
  | Nil -> "N"
  | Br (l,x,r) -> 
      if l=Nil && r=Nil then 
	string_of_int x 
      else  
	("(" ^ string_of_t l ^ " " ^ string_of_int x ^ " " 
	^ string_of_t r ^ ")")
      
let create r x l = Br (r,x,l)
  
let singleton elm = create Nil elm Nil
  
  (* [find t elm] Find the subtree of [t] where elm is the root. 
   *)
let rec find t elm = match t with
  | Nil -> None
  | Br (l,x,r) -> 
      if x=elm then Some t
      else match find l elm with
	| None -> find r elm
	| Some subtree -> Some subtree
	    
let get_val = function 
  | Nil -> None
  | Br (_,x,_) -> Some x
      
  (* Compute the list of childern a member has. 
   *)
let children rank tree = 
  let tree = match find rank tree with 
    | None -> failwith "Could not find the node, children failed"
    | Some node -> node
  in
  match tree with 
    | Nil -> failwith "Cannot be Nil"
    | Br (l,x,r) -> 
	let res = [get_val l; get_val r] in
	filter_nones res
	  
(* Return the father of the node. 
 *)
let rec father tree rank = match tree with 
  | Nil -> None
  | Br (l,x,r) -> 
      if get_val l = Some rank || get_val r = Some rank then Some x
      else match father l rank with
	| None -> father r rank
	| Some x -> Some x
	    
	    
let rec map f = function
  | Nil -> Nil
  | Br (l,x,r) -> create (map f l) (f x) (map f r)
      
      
let rec members_of_t = function
  | Nil -> []
  | Br (l,x,r) -> (x :: members_of_t l) @ (members_of_t r)
      

  (* Search the tree in BFS order. return the first member that has a 
   * Nil pointer. We can attach a new subtree to it. 
   *)
  exception FoundIt of rank

  let bfs_find_low_fanout t = 
(*    assert (t <> UNil);*)
    if not (t <> Nil) then failwith "t=UNil";
    try 
      let rec loop wave = 
	log (fun () -> sprintf "wave=%s\n" (string_of_list string_of_t wave));
	(*assert (wave <> []);*)
	if (wave = []) then failwith "wave=[]";
	let wave = List.map (function 
	  | Nil -> failwith "Found a Nil node inside the wave"
	  | Br (l,x,r) -> 
	      if l = Nil || r = Nil then raise (FoundIt x)
	      else l,r
	) wave in 
	let wave = List.fold_left (fun accu (l,r) -> 
	  l::r::accu
	) [] wave
	in loop wave
      in loop [t]
    with FoundIt s -> s
	    

	 

(*************************************************************************)
(*************************************************************************)
(*************************************************************************)

module Pretty_printer = struct
  let rec depth = function
    | Nil -> 0
    | Br (t1,_,t2) -> 1 + max (depth t1) (depth t2)

  let to_string t s_a = 
    let rec loop t total_len depth = 
      match t with 
      | Nil -> 
	  let s = s_a.(depth) in
	  s.[total_len] <-'N';
	  total_len+1
      | Br (t1,rank,t2) -> 
	  let len1 = loop t1 total_len (depth+1) in
	  let rank_s = string_of_int rank in
	  let rank_len = String.length rank_s in
	  let s = s_a.(depth) in
	  assert (len1 + rank_len <= 60);
	  String.blit rank_s 0 s len1 rank_len;
	  let len2 = loop t2 (len1+rank_len) (depth+1) in
	  len2
    in 
    let _ = loop t 0 0 in
    ()
	 
  let string_of_t t = 
    let depth = depth t in 
    let s_a = Array.init (depth+1) (fun _ -> String.make 60 ' ') in
    to_string t s_a;
    String.concat "\n" (Array.to_list s_a)
    
end
  
let pretty_string_of_t = Pretty_printer.string_of_t

(*************************************************************************)
(*************************************************************************)
(*************************************************************************)

(* This module works on trees with holes. This is required
 * to handle node failures, and tree spliting.
 * The module provides a single function: split. 
*)
module Holes = struct
  type t = rank option tree

  let rec of_t = function
    | Nil -> Nil
    | Br (l,x,r) -> Br ((of_t l), Some x ,(of_t r))
      
  let rec t_of = function
    | Nil -> Nil
    | Br(l,x,r) -> 
	match x with 
	  | None -> failwith "of_holes: Cannot map a tree with holes to a full one"
	  | Some x -> Br((t_of l), x, (t_of r))
	      
	      
  let string_of_int_option = function
    | None -> "N"
    | Some x -> string_of_int x 

  let rec string_of_t = function 
    | Nil -> "N"
    | Br (l,x,r) -> 
	"(" ^ string_of_t l ^ " " ^ string_of_int_option x ^ " " ^ string_of_t r ^ ")"
	

	
  module Pretty_printer = struct
    let rec depth = function
      | Nil -> 0
      | Br (t1,_,t2) -> 1 + max (depth t1) (depth t2)
	  
    let to_string t s_a = 
      let rec loop t total_len depth = 
	match t with 
	  | Nil -> 
	      let s = s_a.(depth) in
	      s.[total_len] <-'N';
	      total_len+1
	  | Br (t1,elm_opt,t2) -> 
	      let len1 = loop t1 total_len (depth+1) in
	      let rank_s = string_of_int_option elm_opt in
	      let rank_len = String.length rank_s in
	      let s = s_a.(depth) in
	      assert (len1 + rank_len <= 60);
	      String.blit rank_s 0 s len1 rank_len;
	      let len2 = loop t2 (len1+rank_len) (depth+1) in
	      len2
      in 
      let _ = loop t 0 0 in
      ()
      
    let string_of_t t = 
      let depth = depth t in 
      let s_a = Array.init (depth+1) (fun _ -> String.make 60 ' ') in
      to_string t s_a;
      String.concat "\n" (Array.to_list s_a)
	
  end
    
  let pretty_string_of_t = Pretty_printer.string_of_t
    
    
  (* [map f t] map a tree using function f. 
   *)
  let map f = function
    | Nil -> Nil 
    | Br(x,l,r) -> Br (map f x, f l, map f r)


  (* [mem elm t] 
   * Is [elm] a member of tree [t]?
   *)
  let rec mem elm t = match t with 
    | Nil -> false
    | Br(l,x,r) -> (
 	match x with 
	  | None -> mem elm l || mem elm r
	  | Some x -> x = elm || mem elm l || mem elm r
      )

  (* [comp_con rank tree] return the tree partition in which member 
   * [rank] is. 
   *)

  (*  This is a data structure that helps pass up reachablity information. 
   *  The idea is that a tree the list that is connected 
   *  to the top member, and the rest.
   *)
  type v = 
    | Com of t
    | Spl of t * t list 

  let string_of_v = function
    | Com t -> "Com(" ^ string_of_t t ^ ")"
    | Spl (t,tl) -> "Spl(" ^ string_of_t t ^ "," ^  string_of_list (string_of_t) tl ^ ")"

  let reach t = 
    let rec reach1 t = match t with 
    | Nil -> Com Nil
    | Br (l,x,r) -> 
	match x with 
	  | None -> (
	      match reach1 l, reach1 r with 
		| Com l, Com r -> Spl (Nil, [l;r])
		| Com l, Spl (rt,rm) -> Spl (Nil, l :: rt :: rm)
		| Spl (lt,lm), Com r  -> Spl (Nil, r::lt ::lm)
		| Spl (lt,lm), Spl(rt, rm) -> Spl (Nil, lt :: rt :: lm @ rm)
	    )
	  | Some x -> (
	      match reach1 l, reach1 r with 
		| Com l, Com r -> Com (Br (l,Some x,r))
		| Com l, Spl (rt,rm) -> Spl (Br(l,Some x,rt) , rm)
		| Spl (lt,lm), Com r  -> Spl (Br(lt,Some x,r), lm)
		| Spl (lt,lm), Spl(rt, rm) -> Spl (Br(lt,Some x,rt), lm @ rm )
	    )
    in 
    let clean_mid m = List.find_all (function l -> l<>Nil) m in
    let res = match reach1 t with
      | Com l -> Com l 
      | Spl(top,m) -> Spl(top,clean_mid m) in
    (*eprintf "v=%s\n" (string_of_v res);*)
    res


  let rmv_members tree rmv_l = 
    map (function 
      | None -> None
      | Some x -> if List.mem x rmv_l then None else Some x
    ) tree 
      
  let find_correct_comp elm tree = 
    let v = reach tree in
    try 
      match v with 
	| Com l -> l
	| Spl (t,m) -> List.find (fun t -> mem elm t) (t :: m)
    with e -> 
      eprintf "\nfailure in find_correct_comp\n";
      eprintf "elm=%d tree=\n%s\n v=%s\n" elm (pretty_string_of_t tree) (string_of_v v);
      raise e
	
	
  (* Remove a few members from a tree and return the partition in which 
   * member [rank] is. 
   *)
  let split elm tree rmv_l = 
      let tree = rmv_members tree rmv_l in
      find_correct_comp elm tree
end

let split t  elm rmv_l = 
  let res = Holes.split elm (Holes.of_t t) rmv_l in
  Holes.t_of res

(*************************************************************************)
(* A sanity check. Check that a tree has no duplicate members.
 *)
let no_dup tree = 
  let mem_l = members_of_t tree in
  let mem_l = List.sort (-) mem_l in
  let rec dup accu l = match accu,l with 
    | _,[] -> true
    | None, hd::tl -> dup (Some hd) tl
    | Some elm,hd::tl -> if hd=elm then false else dup (Some hd) tl
  in dup None mem_l
(*************************************************************************)
       
  (* Merge a couple of tree together. In order to do so efficiently, 
   * compute their diameter and their center. Try to link them together
   * as close to their centers as possible.  
   *)
       
  (* [dia tree] Compute the diameter of [tree]. For trees this is simply
   * the maximum distance between the furthest node from the right hand
   * to the furthest node on the left hand. 
   *)
let dia tree = 
  let rec depth = function 
    | Nil -> 0
    | Br (l,x,r) -> 1 + max (depth l) (depth r)
    in
    match tree with 
      | Nil -> 0
      | Br (l,x,r) -> depth l + depth r
	    
(* Merge two trees. This function assume that dia t1 < dia t2. 
 * 1. Find the first node in t1 that has a fanout of 1 or 0 . 
 * 2. merge t2 there. 
 *)
  let merge2 t1 t2 = 
    let node = bfs_find_low_fanout t2 in
    (*eprintf "bfs_find_low_fanout=%d\n" node;*)
    let rec loop main_tree = match main_tree with 
      | Nil -> Nil 
      | Br (l,x,r) -> 
	  if x = node then 
	    match l,r with 
	      | Nil,_ -> Br(t1,x,r) 
	      | _,Nil -> Br(l,x,t1) 
	      | _,_ -> failwith "Did not find a member with one empty child"
	  else Br(loop l, x, loop r) in
    let res = loop t2 in
    (*eprintf "merge t1=%s t2=%s res=%s\n" (string_of_t t1) (string_of_t t2)
      (string_of_t res);*)
    res

      
  let merge t_l_base  = 
    let rec loop t_l = 
      log (fun () -> sprintf "merge |t_l|=%d\n" (List.length t_l));
      let t_l = List.sort (fun t1 t2 -> dia t1 - dia t2 ) t_l in
      match t_l with 
	| [] -> failwith "merge called with an empty list of trees"
	| [hd] -> 
	    if not (no_dup hd) then (
	      eprintf "Duplicates in hd\n";
	      eprintf "hd=\n%s\n" (pretty_string_of_t hd);
	      eprintf "t_l=\n%s\n" (string_of_list (function t -> string_of_t t) t_l_base);
	      failwith "duplicates in hd";
	    );
	    hd
	| hd1::hd2::tl -> 
	    let hd = merge2 hd1 hd2 in
	    loop (hd::tl)
    in loop t_l_base
  
(*************************************************************************)
(*************************************************************************)

(* Some tests for the code
 *)
open Pretty_printer
let run () = 
  let prog = ref "split" in
  let n = ref 10 in
  let iter_num = ref 100 in
  let leave_num = ref 3 in
  let verbose = ref false in

  Arg.parse [
    "-prog", Arg.String (fun s -> prog:=s), "Which test to run? [split,merge, simple]";
    "-n", Arg.Int(fun i -> n:=i), "Tree size";
    "-verbose", Arg.Set(verbose), "Verbose mode"; 
    "-leave_num", Arg.Int(fun i-> leave_num:=i), "Size of leave set";
    "-iter_num", Arg.Int(fun i-> iter_num:=i), "Number of iterations"
  ] (fun _ -> exit 0) "Automatic test for the dynamic tree module";
  
  let create_tree m n = 
    let a = Array.init n (function i -> singleton (i+m)) in
    let l = Array.to_list a in
    merge l 
  in
  
  begin match !prog with 
    | "simple" -> 
	let tree3 = create_tree 0 3 in
	eprintf "tree3: \n%s\n"  (pretty_string_of_t tree3);
	()
	
    | "merge" -> 
	let tree3 = create_tree 0 3 
	and tree10 = create_tree 3 10 in
	let tree13 = merge [tree3;tree10] in
	eprintf "tree3: \n%s\n"  (pretty_string_of_t tree3);
	eprintf "tree10: \n%s\n"(pretty_string_of_t tree10);
	eprintf "tree13: \n%s\n" (pretty_string_of_t tree13);
	
	let tree2 = create_tree 13 2 in
	let tree15 = merge [tree2;tree13] in
	eprintf "tree2: \n%s\n"  (pretty_string_of_t tree2);
	eprintf "tree15: \n%s\n"(pretty_string_of_t tree15);  
	
	let tree2 = create_tree 15 2 in
	let tree17 = merge [tree2;tree15] in
	eprintf "tree2: \n%s\n"  (pretty_string_of_t tree2);
	eprintf "tree15: \n%s\n"(pretty_string_of_t tree17);  
	
	
    (*
      let t1 = create_tree 0 10 in
      let t2 = create_tree 10 3 in
      let t3 = merge [t1;t2] in
      eprintf "t1: %s\n" (string_of_t t1);
      eprintf "t2: %s\n" (string_of_t t2);
  eprintf "t3: %s\n" (string_of_t t3);
*)

(*
  let t0 = singleton 0 in
  let t1 = singleton 1 in
  let t0_2 = create t0 2 t1 in
  
  let t3 = singleton 3 in
  let t0_4 = create t3 4 t0_2 in
  
  let t5 = singleton 5 in
  let t6 = singleton 6 in
  let t5_7 = create t5 7 t6 in
  
  printf "t0_4=%s t_5_7=%s\n" (string_of_t t0_4) (string_of_t t5_7);
  let t0_7 = merge [t0_4;t5_7] in
  printf "t0_7=%s\n" (string_of_t t0_7) ;
  let t0_7 = merge [t5_7;t0_4] in
  printf "t0_7=%s\n" (string_of_t t0_7) ;
  
  printf "t0_4=\n%s\n" (pretty_string_of_t t0_4);
  printf "t5_7=\n%s\n" (pretty_string_of_t t5_7);
  printf "t0_7=\n%s\n" (pretty_string_of_t t0_7);
	(*let t2 = merge [t0;t1] in
  eprintf "t2=%s\n" (string_of_t t2);*)
  
*)
	
    | "split" -> 
	let t0_4 = create_tree 0 5 in
	eprintf "t0_4:\n%s\n" (string_of_t t0_4);
	let t_0 = split t0_4 0 [4] 
	and t_1 = split t0_4 1 [4] 
	and t_2 = split t0_4 2 [4] 
	and t_3 = split t0_4 3 [4] in
	printf "t_0=\n%s\n" (string_of_t t_0);
	printf "t_1=\n%s\n" (string_of_t t_1);
	printf "t_2=\n%s\n" (string_of_t t_2);
	printf "t_3=\n%s\n" (string_of_t t_3);
	
	
	let t_0 = split t0_4 0 [2] 
	and t_1 = split t0_4 1 [2] 
	and t_3 = split t0_4 3 [2] 
	and t_4 = split t0_4 4 [2] in
	printf "t_0=\n%s\n" (string_of_t t_0);
	printf "t_1=\n%s\n" (string_of_t t_1);
	printf "t_3=\n%s\n" (string_of_t t_3);
	printf "t_4=\n%s\n" (string_of_t t_4);


    | "rand" -> 
	eprintf "Random test\n";
	assert (!n > !leave_num) ;

	let subleader t = 
	  let l = members_of_t t in 
	  match l with 
	    | [] -> failwith "subleader: Tree is empty"
	    | hd::_ -> hd 
	in
	
	let choose_leave_set () = 
	  let rec loop l = 
	    if List.length l = !leave_num then l 
	    else (
	      let rand = Random.int !n  in
	      if List.mem rand l then loop l 
	      else loop (rand::l)
	    )
	  in loop []
	in
	
	
	let base_tree = create_tree 0 !n in
	let mem_a = Array.init !n ident in
	
	let rec loop t counter = 
	  if counter=0 then () 
	  else (
	    eprintf ".";
	    
	    let leave = choose_leave_set () in
	    let a = Array.map (fun i -> 
	      if not (List.mem i leave) then 
		split t i leave 
	      else 
		singleton i 
	    ) mem_a in
	    let a = Array.mapi (fun i t -> 
	      if subleader t = i then (
		let nmembers = List.length (members_of_t t) in 
		if nmembers = 1 then Some (singleton i)
		else (
		  if !verbose then 
		    eprintf "comp = \n%s\n" (pretty_string_of_t t);
		  Some t 
		)
	      ) else None
	    ) a in
	    let l = Array.to_list a in
	    let l = filter_nones l in
	    let t' = merge l in
	    if List.length (members_of_t t') <> !n then 
	      failwith (sprintf "does not contain %d members" !n);

	    if !verbose then 
	      eprintf "t' = \n%s\n" (pretty_string_of_t t);
	    loop t' (counter-1)
	  )
	in
	loop base_tree !iter_num

	
    | _ -> failwith "Unknown test"
  end;

  exit 0;
  ()
(*************************************************************************)
