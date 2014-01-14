(**************************************************************)
(* TDEFS.MLI: definitions for the tree modules *)
(* Author: Ohad Rodeh 2/99  *)
(**************************************************************)
open Printf
open Util 

(**************************************************************)

let safe_chr i = 
  if i<|0 || i>|255 then 
    raise (Invalid_argument (sprintf "Error, chr out of range, i=%d\n" i));
  Char.chr i

module Key = struct
  type t = char

  let compare x y = (Char.code x) - (Char.code y)
  let rand () = safe_chr (Random.int 256)
  let string_of_t i = "K" ^ string_of_int (Char.code i)
end

module Member = struct 
  type t = char

  let of_int i = safe_chr i 
  let int_of c = Char.code c
  let compare t1 t2 = (Char.code t1) - (Char.code t2)
  let string_of_t i = "M" ^ string_of_int (Char.code i)
end

(**************************************************************)

module Tree = Tree.Make(Member)(Key)

(**************************************************************)

module type RealKeyType = sig
  type t 
  type set 

  val empty : set 
  val create : Key.t -> Security.cipher option -> t
  val tag : t -> Key.t
  val real : t -> Security.cipher option
  val string_of_t : t -> string
  val add : set -> Key.t -> Security.cipher option -> unit
  val decrypt : Shared.Cipher.t -> set -> ((Key.t * Buf.t) list * Key.t) list -> unit
  val assoc : Key.t -> set -> Security.cipher option
  val translate : set -> (Key.t * Key.t) list -> set
  val purge  : Tree.z -> set -> int -> set 
  val inflate : Tree.z -> set -> int -> set
  val check_all : set -> bool
  val pairs_of : set -> (Key.t * Security.cipher option) list
  val from_pair : Key.t * Security.cipher option -> t
  val from_list : t list -> set
  val hd        : set -> Key.t * Security.cipher option
  val tl        : set -> set
end


module RealKey = struct
  type t = Key.t * Security.cipher option ref 
  type set = t  list

  let empty = []
  let create k s = (k,ref s)

  let tag (k,_) = k
    
  let real (_,s_r) = !s_r

  let string_of_t (k,s_r) = match !s_r with 
    | None -> (Key.string_of_t k) ^ " None"
    | Some cipher -> 
	(Key.string_of_t k) ^ " " ^ (Buf.string_of (Security.buf_of_cipher cipher))

  let add  r_k_l tag key = 
    let s_r = List.assoc tag r_k_l in
    s_r := key

  (* [decrypt set mcl] Decrypt keys from [mcl] and insert into [set].
   * First, create a list of type: (key,tag,decryptable list option).
   * Then, decrypt it from bottom-up.
   *)
  let decrypt shared set mcl = 
    let dcr cipher = Shared.Cipher.single_use shared cipher false in
    let mcl' = List.map (fun (x,y) -> (y,x)) mcl in
    let strange = List.map (fun (tag,s_r) -> 
      try 
	let l = List.assoc tag mcl' in
	(tag,s_r,Some l)
      with Not_found -> (tag,s_r,None)
    ) (List.rev set) in
    List.iter (function 
      | _,_,None -> ()
      | tag,s_r,Some l -> 
	  match !s_r with
	    | None -> failwith "Error in decrypt"
	    | Some cipher -> 
		List.iter (fun (tag',key') -> 
		  let key' = dcr cipher key' in
		  add set tag' (Some (Security.cipher_of_buf key'))
		) l 
    ) strange
	  
	  
  (* [assoc tag r_k_l] Find a security key associated with a key-tag 
   * in [r_k_l].
   *)
  let assoc tag r_k_l = 
    let s_r = List.assoc tag r_k_l in
    !s_r

  (* Translate the keys whose names have changed. 
   *)
  let translate r_k_l bad_set = 
    List.map (fun r_k -> 
      let tag,(s_r : Security.cipher option ref)  = r_k in
      try 
	let tag' = List.assoc tag bad_set in
	create tag' !s_r
      with Not_found -> r_k
    ) r_k_l
  

  (* [purge_list tree r_k_l] purge a list of RealKeys [r_k_l]. They 
   * must all be on the way to the root of [tree]. 
   *)
  let purge tree r_k_l rank = 
    let keys = Tree.zkeylist tree (Member.of_int rank) in
    List.find_all (function k -> 
      List.mem (tag k) keys 
    )  r_k_l


  (* [inflate tree r_k_l] Add new key tags into the list, with
   * empty keys. 
   *)
  let inflate tree r_k_l rank = 
    let keys = Tree.zkeylist tree (Member.of_int rank) in
    List.map (function k -> 
      try 
	let s_r = List.assoc k r_k_l in
	create k !s_r
      with Not_found -> create k None
    ) keys

  (* Check if all keys are different to Security.NoKey
   *)
  let check_all r_k_l = 
    List.for_all (fun (tag,s_r) -> !s_r <> None) r_k_l

  let pairs_of r_k_l = 
    List.map (fun (tag,s_r) -> tag,!s_r) r_k_l

  let from_pair (tag,s) = (tag, ref s)
  let from_list l = l

  let hd l = 
    let k,s_r = List.hd l in 
    k,!s_r

  let tl l = List.tl l 
end


(**************************************************************)

(************************************************************************)
(* Self check part, A test of random trees.  
 * open the original Tree definition, create a couple of random trees, 
 * and check that they merge correctly.
*)

let test () = 
  let size = ref 10 in
  let num_subtrees = ref 10 in
  let rounds = ref 10 in
  let prog = ref "leaves" in
  let verbose = ref false in
  let n = ref 10 in
  let break_point = ref None in
  Arg.parse [
    "-s", Arg.Int(fun i -> size:=i), "The size of each individual subtree" ;
    "-num_subtrees", Arg.Int(fun i -> num_subtrees:=i), "The number of subtrees to merge" ;
    "-verbose", Arg.Set(verbose), "Print all the tree information" ;
    "-rounds", Arg.Int(fun i -> rounds:=i), "Number of rounds to run" ;
    "-prog", Arg.String(fun s -> prog := s), "Which test to run? [subtrees, leaves, random, top_key, marshal, join_leave]";
    "-n", Arg.Int(fun i -> n:=i), "Size of tree";
    "-break", Arg.Int(fun i -> break_point := Some i), "Round to break and print info"
  ] (fun _ -> exit 0) "Random test of DWGL trees";

  printf "Starting random test\n";

  let base_name = ref (-1) in
  let fresh_name () = 
    if !base_name = 255 then base_name := -1;
    incr base_name;
    (Member.of_int !base_name)
  in

  let create_tree n = 
    let a = Array.init n (fun i -> Tree.zleaf (fresh_name ())) in
    let l = Array.to_list a in
    Tree.merge (List.map Tree.unzip l)
  in

  let check_tree t ml full = 
    if !verbose then (
      printf "final tree=%s\n" (Tree.string_of_t t);
      printf "merge list=%s\n" (Tree.string_of_alist ml);
      printf "full info = \n%s\n" (Tree.string_of_full full)
    );
    let debug_map,result,s = Tree.self_check false t full in
    if !verbose then (
      printf "debug info=\n%s\n" (Tree.string_of_debug_map debug_map);
      flush stdout
    );
    if not result then 
      failwith (sprintf "failed on tree %s" (Tree.string_of_t t));
  in

  let create_and_check_tree n = 
    let t,ml = create_tree n in
    let full = Tree.actions_of_merge t ml in
    check_tree t ml full
  in


  begin match !prog with 
    | "leaves" -> 
	(* First test. Create trees of size 0 to !size, and check their sanity.
	 * The trees are constructed of a list of leaves.
	 *)
	for i=1 to !size do 
	  printf "Checking  size %d\n" i;
	  create_and_check_tree i
	done;

    | "subtrees" -> 
	(* Second test. Do a !rounds number of rounds. In each round create a 
	 * set of subtrees of size !size. Merge the subtrees and check the
	 * sanity of the created tree. 
	 *)
	printf "starting subtree test, num_subtrees=%d\n" !num_subtrees;
	let a = Array.init !num_subtrees (fun _ -> create_tree !size) in
	let l = Array.to_list a in
	let l = List.map (function (t,_) -> t) l in
	let t,ml = Tree.merge l in
	let full = Tree.actions_of_merge t ml in
	
	check_tree t ml full
  
    | "random" -> 
	for i=1 to !rounds do 
	  let size_a = Array.init !num_subtrees (fun _ -> 1 + (Random.int !size)) in
	  printf "round %d size=%s\n" i (string_of_int_array size_a);
	  let a = Array.map (fun size -> create_tree size) size_a in
	  let l = Array.to_list a in
	  let l = List.map (function (t,_) -> t) l in
	  if !verbose then 
	    List.iter (fun t -> printf "%s\n" (Tree.string_of_t t)) l;
	  let new_tree,al = Tree.merge l in
	  (* Check the merge
	   *)
	  let ((actions_h,memmap,keytable,bad_set) as full) = 
	    try Tree.actions_of_merge new_tree al 
	    with e -> eprintf "Error in actions_of_merge\n"; 
	      raise e
	  in
	  let _,result,_ = Tree.self_check false new_tree full in 
	  if not result then (
	    eprintf "\nSelf_check failed for %s\n" (Tree.string_of_t new_tree);
	    eprintf "keytable=%s\n" (Tree.string_of_keytable keytable);
	    exit 0;
	  );
	done;
	
    | "top_key" -> 
	printf "In top_key\n"; flush stdout;
	let t,_ = create_tree !size in
	printf "tree= %s\n" (Tree.string_of_t t); flush stdout ;
	let t,_ = Tree.new_top t in
	printf "tree= %s\n" (Tree.string_of_t t);
	flush stdout
	  

    | "marshal" -> 
	(* Size of messages
	 *)
	for i=1 to !size do 
	  let t,ml = create_tree i in
	  let full = Tree.actions_of_merge t ml in
	  let s = Marshal.to_string full [] in
	  printf "i=%d size=%d\n" i (String.length s);
	done;

    | "join_leave" -> 
	let rec loop t count = 
	  let z = Tree.zip t in
	  
	(* Removing a random member
	 *)
	  let dead = Member.of_int (Random.int !n) in 
	  let l = Array.to_list (Array.init !n (fun i -> i)) in
	  let z_l = List.fold_left (fun l i -> 
	    try 
	      let me = Member.of_int i in
	      let z' = Tree.zremove_members_spec z me [dead] in
	      if Tree.zam_leader z' me then z'::l else l
	    with Not_found -> l
	  ) [] l  in
	  
	  let z_a = Array.of_list z_l in
	  (*printf "orignal=%s\n" (Tree.string_of_z z);
	  Array.iteri (fun i z -> 
	    printf "t(%d) = %s\n" i (Tree.string_of_z z)
	  ) z_a;*)
	  let t_l = List.map Tree.unzip z_l in
	  let new_tree,al = Tree.merge (Tree.unzip (Tree.zleaf dead)::t_l) in
	  
	  (* Check the merge
	   *)
	  let ((actions_h,_,_,bad_set) as full) = 
	    try Tree.actions_of_merge new_tree al 
	    with e -> eprintf "Error in actions_of_merge\n"; 
	      raise e
	  in
	  let _,result,_ = Tree.self_check false new_tree full in 
	  printf "re-merged=%s\n" (Tree.string_of_t t);
	  if not result then 
	    failwith "re-merged failed for %s" (Tree.string_of_t new_tree);
	  if count > 0  then loop new_tree (pred count)
	in
	let t,_ = create_tree !n in
	loop t !rounds
	  
    | _ -> failwith "No such test exists"
  end;	
  exit 0

(*let _ = test () *)

(**************************************************************)

