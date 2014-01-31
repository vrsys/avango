(**************************************************************)
(* TREE.ML : D-WGL algorithm *)
(* Author: Ohad Rodeh, 2/99 *)
(**************************************************************)
(* A (functor style) library for DWGL key trees. It is implemeted
 * stand-alone from the rest of the distribution so it could be 
 * compiled and checked without using Util. 
*)
(**************************************************************)

module type OrderedKey = sig
  type t

  val compare : t -> t -> int
  val rand    : unit -> t 
  val string_of_t : t -> string
end

module type OrderedMember = sig
  type t 

  val of_int  : int -> t
  val int_of  : t -> int
  val compare : t -> t -> int
  val string_of_t : t -> string
end

module type S = sig
  type t 
  type member
  type key 

    (* A list of actions describing the merging of several trees.
     *)
  type alist  

  (* The set of actions to be performed by a member. 
   *)
  type memrec = {
    mutable send : (member * key) list ;
    mutable cast1: (key list * key) list ;
    mutable recv : key option ;
    mutable cast2: (key * key) option
  }

  val string_of_memrec : memrec -> string

  (* A mapping between member and its original tree.
   *)
  type memmap 

  type keytable = (key , t) Hashtbl.t
  type actions_h = (member, memrec) Hashtbl.t

  (* The full information about a merge of several trees
   *)
  type full = actions_h * memmap * keytable * (key * key) list
  type sent = actions_h * (key * key) list

  type debug_map 
    
  (* merge a list of trees.
   *)
  val merge  : t list -> t * alist  

  (* Replace the top key with a new one.
   *)
  val new_top : t -> t * alist

  val string_of_t : t -> string
  val height_string_of_t : t -> string
  val string_of_alist : alist -> string
  val actions_of_merge : t -> alist -> full
  val string_of_full : full -> string
  val string_of_keytable : keytable  -> string
  val string_of_debug_map : debug_map -> string 

  (* [decrypt keyl mcl] Compute the set of keys decryptable from the list [mcl] 
   * assuming [keyl] are known 
  *)
  val decrypt : key list -> (key list * key) list -> key list
    
  (* Compute the total set of multicasts performed 
   *)
  val mcast_sweep : actions_h -> (key list * key) list 

  (* [self_check verbose tree full] checks whether the (tree,full) 
   * are correct. All members may encrypt only with keys they know, and 
   * all members must receive all the keys they need. 
   *)
  val self_check : bool -> t -> full -> debug_map * bool * string 
      
  (* Zipping and unzipping trees. Trees have to be passed through
   * the network, in order to reduce message size their size should
   * be reduced. 
   *)
  type z 
  val string_of_z : z -> string
  val zip : t -> z 
  val unzip : z -> t 
  val zempty : z
  val zleaf  : member -> z
  val zam_leader : z -> member -> bool 
  val members_of_z : z -> member list

  (* [keylist full member] find the list of keys from [member] to the root on 
   * tree [t] 
   *)
  val zkeylist : z -> member -> key list

  (* [remove_members_spec tree member rmv_l] returns the largest subtree 
   * of [tree] that contains [member] but does not contain any member from
   * [rmv_l].
   *)
  val zremove_members_spec : z -> member -> member list -> z

  (* [zmap f g tree] Map the tree. [f] works on the members, [g]
   * works on the keys. 
   *)
  val zmap   : (member -> member) -> (key -> key) -> z -> z
end

(*********************************************************************)
module Make(Member: OrderedMember) (Key : OrderedKey) : (S with type member = Member.t and type key=Key.t) = struct

  type key = Key.t
  type member = Member.t
  type height = int  (* Tree height *)

  type t = 
    | Nil
    | Leaf of member
    | Br of member (* The subtree leader *) * t * key * t * height 
	
  type memrec = {
    mutable send : (member * key) list ;
    mutable cast1: (key list * key) list ;
    mutable recv : key option ;
    mutable cast2: (key * key) option
  }

  (* A mapping between member and its original tree.
   *)
  module MemMap    = Map.Make(Member)
  type memmap      = t MemMap.t

  type keytable = (key, t) Hashtbl.t
  type actions_h = (member, memrec) Hashtbl.t

  type full = actions_h * memmap * keytable * (key * key) list
  type sent = actions_h * (key * key) list
      
  type alist = (t * t * t) list * t MemMap.t * (Key.t * Key.t) list
		 
  open Printf

  (*******************************************************************)
  (* This version of sort removes equal items. *)
  let rec sort_merge order l1 l2 =
    match l1 with
	[] -> l2
      | h1 :: t1 ->
	  match l2 with
              [] -> l1
	    | h2 :: t2 ->
		if h1=h2 then h1 :: (sort_merge order t1 t2)
		else 
		  if order h1 h2 then 
		    h1 :: sort_merge order t1 l2
		  else h2 :: sort_merge order l1 t2
		    
		    
  let remove_dup l = 
    let rec loop pre h post = match post with 
      | [] -> List.rev (h::pre)
      | hd::tl -> if h=hd then loop pre h tl else loop (h::pre) hd tl
    in
    match l with 
      | [] -> []
      | hd::tl -> loop [] hd tl
	    
  let intersect l1 l2 = List.filter (function x -> List.mem x l1) l2
  let list_sub  l1 l2 = List.filter (function x -> not (List.mem x l2)) l1 
  (*******************************************************************)
			  
  let empty = Nil
		
  let leaf member = Leaf member
		 
  let is_empty = function 
    | Nil -> true
    | _ -> false
	  
  let h = function
    | Nil -> 0
    | Leaf _ -> 1
    | Br (_,_,_,_,height) -> height
	  
  let ldr = function
    | Nil -> failwith "No leader to an empty tree"
    | Leaf i -> i
    | Br(i,_,_,_,_) -> i
	  
  let key = function
    | Nil | Leaf _ -> failwith "No key to a (Nil | Leaf _) tree"
    | Br(_,_,key,_,_) -> key
	  
  let left = function 
    | Nil | Leaf _ -> failwith "No left to a (Nil | Leaf _) tree"
    | Br(_,l,_,_,_) -> l
	  
  let right = function 
    | Nil | Leaf _ -> failwith "No right to a (Nil | Leaf _) tree"
    | Br(_,_,_,r,_) -> r
	  
  let rec string_of_t = function
    | Nil -> ""
    | Leaf i -> Member.string_of_t i
    | Br (_,l,x,r,_) -> sprintf "(%s %s %s)" (string_of_t l) (Key.string_of_t x) (string_of_t r)
	  
  let rec size = function 
    | Nil -> 0
    | Leaf _ -> 1
    | Br(_,l,_,r,_) -> size(l) + size(r)

  let rec height_string_of_t = function
    | Nil -> ""
    | Leaf i -> Member.string_of_t i
    | Br (_,l,x,r,h) -> sprintf "(%s %s %s)[%d]" 
	  (height_string_of_t l) (Key.string_of_t x) (height_string_of_t r) h 
	  
  let rec check_avl = function 
    | Nil -> true
    | Leaf i -> true
    | Br(_,l,_,r,_) -> abs ((h l) - (h r)) < 2 && (check_avl l) && (check_avl l)

  let string_of_list = function l -> String.concat " " l 
  let stringf_of_list = fun f l -> String.concat " " (List.map f l)
  let stringfc_of_list = fun f l c -> String.concat c (List.map f l)
  let ident = function x -> x    
      
  (*********************************************************************)
      
  let create l x r = Br (ldr l, l, x,r, 1 + (max (h l) (h r))) 
	    
  (*********************************************************************)

  (* [map f g tree] Map the tree. [f] works on the members, [g]
   * works on the keys. 
   *)
  let rec map f g tree = match tree with 
    | Nil -> Nil
    | Leaf i -> Leaf (f i)
    | Br (ldr,l,k,r,h) -> create (map f g l) (g k) (map f g r) 

  let merge2 unq a b = 
    let rec mrg l r = 
      (*printf "mrg %s %s\n" (string_of_t l) (string_of_t r); flush stdout;*)
      if h l <= h r then let t = (create l (unq ()) r) in t,(l,r,t)
      else match l with 
	| Nil -> failwith (sprintf "Sanity, merging a Nil tree")
	| Leaf _ -> let t = (create l (unq ()) r) in t,(l,r,t)
	| Br (_,ll,k,lr,_) -> 
	    if h ll > h lr then 
	      let r,m = (mrg lr r) in (create ll k r),m
	    else if h ll < h lr then 
	      let l,m = (mrg ll r) in (create l k lr),m
	    else (*h l = h r >= h s*)
	      let r,m = (mrg lr r) in (create ll k r),m
    in
    match a,b with
      | _,Nil -> a,(a,Nil,a)
      | Nil,_ -> b,(Nil,b,b)
      | _ when h a > h b -> mrg a b 
      | _ -> mrg b a
	  
  (* This function creates a map. the mapping is between members and their
   * original trees.
   *)
  let orig_map treel = 
    List.fold_left (fun map tree -> 
      let rec loop map t = match t with 
	| Nil -> map
	| Leaf member -> MemMap.add member tree map
	| Br(_,l,_,r,_) -> 
	    let map = loop map l in
	    loop map r
      in
      loop map tree 
    ) MemMap.empty treel 
      

  module KeyList = Set.Make(Key)

  (* Choose a key that is NOT already in [set].
   *)
  let rec choose set = 
    let rnd = Key.rand () in
    if not (KeyList.mem rnd set) then (
      rnd
    ) else choose set 
      
  (* [dis_keys l] Disjoin the key names of a list of 
   * trees. When several trees are merged, their may be 
   * overlap of key names. Such keys must be remapped
   * prior to the merge operation. The function returns
   * a list of keys that have been moved. 
   *)
  let dis_keys l = 
    let set_r = ref KeyList.empty in
    let bad_set = ref [] in

    let l = List.map (fun tree -> 
      map Util.ident (fun k -> 
	if KeyList.mem k !set_r then (
	  let rnd = choose !set_r in
	  set_r := KeyList.add rnd !set_r;
	  bad_set := (k,rnd) :: !bad_set;
	  rnd
	) else (
	  set_r := KeyList.add k !set_r;
	  k  
	)
      ) tree
    ) l in
    l,!set_r,!bad_set
	
  (* Merge a list of trees. Adda the smallest two, and add to the list. 
   * Since a small number of trees is expected, it is enough to use a 
   * sorted list. A more efficient solution would be to use a heap, 
   * where trees are sorted by height. 
   *)
  let merge l = 
    let l,set,bad_set = dis_keys l in
    let orig_map = orig_map l in
    let less = fun x y -> h x < h y in
    let set_r = ref set in
    let rec mrg al acc l = match l with 
      | [] -> acc, (al, orig_map, bad_set)
      | hd::tl -> 
	  let acc,step = merge2 (fun () -> 
	    let rnd = choose !set_r in
	    set_r := KeyList.add rnd !set_r;
	    rnd
	  ) acc hd in
	  (*printf "acc=%s\n" (string_of_t acc); flush stdout;*)
	  match Sort.merge less [acc] tl with (*PERF*)
	    | [] -> failwith "Sanity, Empty list after sort"
	    | hd::tl -> mrg (step::al) hd tl
    in
    match l with 
      | []  -> failwith "Merging an Empty list"
      | _ -> 
	  match Sort.list less l with
	    | [] -> failwith "Sanity, empty list after sort"
	    | hd::tl -> mrg [] hd tl

  (* Replace the top key with a new one.
   *)
  let new_top t = 
    match t with 
      | Nil | Leaf _ -> failwith "can't new_top a tree with no keys"
      | Br(_,l,_,r,_) -> merge [l;r]


  let string_of_alist (alist,_,_) = 
    stringf_of_list (function (t1,t2,t) -> 
      "("^ string_of_t t1 ^","^ string_of_t t2 ^")\n"
    ) alist
      
  (* [t_of_key t key] Find the subtree of [t] whose top key is [key] 
   *)
  let t_of_key tree key = 
    let rec loop t = match t with 
      | Nil | Leaf _ -> None
      | Br(_,l,k,r,_) -> 
	  if k=key then Some t else match loop l with 
	    | None -> loop r 
	    | Some t' -> Some t'
    in
    match loop tree with 
      | None -> failwith (sprintf "Sanity, key %s not found in %s" (Key.string_of_t key) 
	  (string_of_t tree))
      | Some t -> t
	    
	    
  (*********************************************************************)
   (* [create_key_map tree] Create a hash table that associates the key of a 
    * subtree t of [tree] 
    * with t. Used for fast lookup and comparisons between keys. 
    * PERF: we use 100 here.
   *)
  let create_key_tbl tree = 
    let h = Hashtbl.create 100 in
    let rec loop t = match t with 
      | Nil | Leaf _ -> h
      | Br (_,l,key,r,_) -> 
	  Hashtbl.add h key t;
	  loop l;
	  loop r
    in
    loop tree

  let le keymap k1 k2 = 
    (*try *)
      h (Hashtbl.find keymap k1) <= h (Hashtbl.find keymap k2)
    (*with Not_found -> failwith "Not_found in keymap"*)
			
  (*********************************************************************)
  (*********************************************************************)
	    
  let string_of_keys (k1,k2) = 
    sprintf "{%s}%s" (Key.string_of_t k1) (Key.string_of_t k2)
  let string_of_keyl (kl,k) = 
    sprintf "{%s}%s" (stringf_of_list Key.string_of_t kl) (Key.string_of_t k)
      
  let string_of_l f = function 
    | [] -> ""
    | l -> f l
	  
  let string_of_memrec memrec = 
    string_of_l (function send -> 
      "send="^ stringf_of_list (function (ldr,key) -> 
	(Member.string_of_t ldr) ^" "^(Key.string_of_t key)) send
    ) memrec.send ^ 
    string_of_l (function cast1 -> 
      " cast1="^ stringf_of_list (string_of_keyl) cast1
    ) memrec.cast1 ^ 
    (match memrec.recv with
      | None -> ""
      | Some recv -> " recv=" ^ (Key.string_of_t recv)
    ) ^ 
    (match memrec.cast2 with 
      | None -> ""
      | Some (cast,enc) -> " cast2=" ^ (Key.string_of_t cast) ^" "^
	  (Key.string_of_t enc)
    )
      
  let string_of_full (mm_tbl,_,_,_) = 
    let l = ref [] in
    Hashtbl.iter (fun m memrec -> 
      l := ((Member.string_of_t m) ^":"^ (string_of_memrec memrec)) :: !l;
      ()
    ) mm_tbl ;
    stringfc_of_list ident !l "\n"

  (* What do we need this for? 
   *)
  let string_of_keytable kt = 
    let l = ref [] in
    Hashtbl.iter (fun key t -> 
      l := ((Key.string_of_t key) ^":"^ (string_of_int (h t))) :: !l;
      ()
      ) kt;
    stringfc_of_list ident !l "\n"
    
  (* [keylist t member] find the list of keys from [member] to the root 
   * on tree [t].
   *)
  let keylist t member = 
    let rec loop keyl = function
      | Nil -> None
      | Leaf m -> if m=member then Some keyl else None
      | Br(_,l,key,r,_) -> 
	  match loop (key::keyl) l with 
	    | None -> begin match loop (key::keyl) r with 
		| None -> None 
		| Some l -> Some l
	      end
	    | Some l -> Some l 
    in
    match loop [] t with 
      | None -> raise (Failure(sprintf "XX keylist %s %s failed" (string_of_t t)
	    (Member.string_of_t member)))
      | Some l -> List.rev l
	    
  (*********************************************************************)
  (* Group together similar encryptions
   *)
      
  (* Merge  {K1 K2 K3}K4  with  {K1 K9}K4  into  {K1 K2 K3 K9}K4
   *)
  let merge_same_key l = 
    let rec loop pre ((kl,key) as sep) post = match post with 
      | [] -> sep::pre
      | (kl',key')::tl when key'=key -> loop pre ((sort_merge (<) kl kl'), key) tl 
      | hd::tl -> loop (sep::pre) hd tl
    in
    match l with 
      | [] -> []
      | hd::tl -> loop [] hd tl
	    
	    
  (* {K15}K14 {K14 K15}K12 {K12 K14 K15}K8 -> {K15}K14 {K14}K12 {K12}K8
   *)
  let remove_from_top l = 
    let rec loop pre ((kl,k) as hd) post = match post with 
      | [] -> hd::pre
      | _ -> 
	  let post = List.map (function l,k' -> list_sub l kl,k') post in
	  match post with 
	    | [] -> failwith "Sanity, remove_from_top, post is empty"
	    | hd2::tl2 -> loop (hd::pre) hd2 tl2
    in 
    match l with 
      | [] -> l
      | hd::tl -> loop [] hd tl
	    
  (* Optimize the set of multicasts performed in cast1.
   *)
  let mcast1_opt h key_table orig_map = 
    Hashtbl.iter (fun member memrec -> 
      let cast1 = Sort.list (fun (kl1,key1) (kl2,key2) -> le key_table key1 key2)
		    memrec.cast1 in
      let cast1 = merge_same_key cast1 in
      let cast1 = remove_from_top cast1 in
      memrec.cast1 <- cast1
    ) h 
      
      
  (* A -> B : key1
   * B -> All : {key1} K_T_B
   *)
  let mcast2 t h key_table orig_map = 
    Hashtbl.iter (fun member memrec -> 
      match memrec.recv with 
	| None -> ()
	| Some recv_key -> 
	    try 
	      let t = Hashtbl.find key_table recv_key in
	      let key = key (right t) in
	      memrec.cast2 <- Some(recv_key, key)
	    with _ -> ()
    ) h 
      
  (* This function computes cast1 out of send1.
   * 
   * There are a number of tricks involved here. 
   * Assume A sends to B key1 through a secure channel (in send1). 
   * Then, it will also multicast key1 encrypted with K_T_A. 
   * All the members of T_A will then be able to decrypt key1, the key 
   * of T_{AB}. Hence, the first step is to encrypt key1 with K_T_A.
   * The second issue is to multicast the set of keys above K_T_A so that
   * T_B will have knowledge of them. This set of keys includes 
   * the set of keys from A to the root minus the keys from key1 down, 
   * it is also intersected with the set of keys A originally possesses. 
   * 
   * A->B: key1
   * A->All: {key1} K_T_A,
   *         {Keys above (T_A, key1, T_B)}key1
   *)
  let mcast1 tree h key_table orig_map = 
    try 
      Hashtbl.iter (fun member memrec -> 
	memrec.cast1 <- 
	List.fold_left (fun send_l (_,send_key) -> 
	  let t = Hashtbl.find key_table send_key in
	  let send_l = 
	    try ([send_key],key (left t)):: send_l
	    with _ -> send_l
	  in 
	  let total_key_path = keylist tree member in
	  let key_path_in_t = keylist t member in
	  let orignal_key_path = keylist (MemMap.find member orig_map) member in
	  let kl = list_sub total_key_path key_path_in_t in
	  let kl = intersect orignal_key_path kl in
	  if kl<>[] then (kl, send_key):: send_l
	  else send_l
	) [] memrec.send 
      ) h
    with e -> 
      eprintf "Error in mcast1\n";
      raise e
	
  let add_recv_key h m key = 
    try 
      let memrec = Hashtbl.find h m in
      match memrec.recv with 
	| Some _ -> failwith "Sanity, recv can only be filled once."
	| None -> memrec.recv <- Some key
    with Not_found -> 
      Hashtbl.add h m {
	send  = [] ;
	cast1 = [] ;
	recv  = Some key ;
	cast2 = None 
      }
	
  let actions_of_merge t (ml,orig_map,bad_set) = 
    let key_table = create_key_tbl t in
    (* Process the merges from small to large 
     *)
    let mhtbl = Hashtbl.create 100 in
    List.iter (function (t1,t2,t) -> 
      let ldr1 = ldr t1 
      and ldr2 =ldr t2 
      and key= key t in 
      let memrec = 
	try 
	  Hashtbl.find mhtbl ldr1
	with Not_found -> 
	  let memrec = {send = [] ; cast1 = [] ; recv  = None ; cast2 = None } in
	  Hashtbl.add mhtbl ldr1 memrec;
	  memrec
      in
      memrec.send  <- (ldr2,key)::memrec.send ;
      add_recv_key mhtbl ldr2 key
    ) ml ;
    mcast1 t mhtbl key_table orig_map ;
    mcast2 t mhtbl key_table orig_map ;
    mcast1_opt mhtbl key_table orig_map ;
    (mhtbl,orig_map,key_table,bad_set)
    
  (*********************************************************************)

  (* Compute the total set of multicasts performed.
   *)
  let mcast_sweep full = 
    let l = ref [] in
    Hashtbl.iter (fun member memrec -> 
      match memrec.cast2 with 
	| None ->  l := memrec.cast1 :: !l
	| Some (k_mcast, k_enc) -> l:= memrec.cast1 :: [[k_mcast],k_enc] :: !l
    ) full; 
    List.flatten !l

      
  (*********************************************************************)
  (*********************************************************************)
  (*********************************************************************)
    
  type check = (member, key list) Hashtbl.t
		 
  let rec members_of_t = function 
    | Nil -> []
    | Leaf m -> [m]
    | Br (_,l,_,r,_) -> (members_of_t l) @ (members_of_t r)

  (* [key_paths t] Compute the set of keys from members to the root, and 
   * insert into the hash table.
  *)
  let rec key_paths t = 
    let kp = (Hashtbl.create 7 : check) in
    let rec loop keyl t = match t with 
      | Nil -> ()
      | Leaf member -> Hashtbl.add kp member (Sort.list (<) keyl)
      | Br (_,t1,key,t2,_) -> loop (key::keyl) t1; loop (key::keyl) t2
    in  
    loop [] t;
    kp 
      
      
  (* [decrypt keyl mcl] Compute the set of keys decryptable from 
   * the list [mcl] assuming [keyl] are known 
   *)
  let decrypt keyl mcl = 
    let f (k1,k2) = "{"^ Key.string_of_t k1 ^"}" ^ Key.string_of_t k2 in
    let rec loop know pre post = match post with 
      | [] -> know
      | ((kl,k2) as hd)::tl -> 
	  if List.mem k2 know then 
	    loop (sort_merge (<) kl know) [] (pre @ tl) 
	  else loop know (hd::pre) tl
    in
    let result = loop keyl [] mcl in
    (* printf "decrypt %s with %s = %s\n" (stringf_of_list f " " mcl) 
       (stringf_of_list Key.string_of_t " " keyl)
       (stringf_of_list Key.string_of_t " " result)
       ; *)
    result
      
  type debug_memrec = {
    memrec : memrec ;
    need_to_know : key list ;
    know : key list
  }
			
  type debug_map = (member, debug_memrec) Hashtbl.t
		     
  let string_of_debug_map debug_map= 
    let l = ref [] in
    Hashtbl.iter (fun m dm -> 
      l := ((Member.string_of_t m) ^":"^ (string_of_memrec dm.memrec) 
      ^" ntn="^ (stringf_of_list Key.string_of_t dm.need_to_know) 
      ^ " know="^ (stringf_of_list Key.string_of_t dm.know))
      :: !l;
    ) debug_map;
    stringfc_of_list ident !l "\n"
      
  (* Check that a member knows all the keys it uses to encrypt.
   * cast1 should contain only keys that the member create (in send) 
   * and keys known beforehand from its original tree (original group). 
   * The check for cast2 and recv checks that the key received is actually
   * the one sent. 
   *)
  let check_sanity verbose member memrec orig_map=
    try 
      if verbose then printf "check_sanity %s\n" (Member.string_of_t member);
      let know = List.map (function m,k -> k) memrec.send in
      let know = know @ (keylist (MemMap.find member orig_map) member) in
      let cast1 = List.map (function (kl,key) -> key::kl) memrec.cast1 in
      let cast1 = List.flatten cast1 in
      if not (List.for_all (function key -> List.mem key know) cast1) then begin
	printf "--------------------------------------\n";
	printf "check_sanity failed.  %s does not posses all the keys it encrypts with in cast1. know=%s\n" (Member.string_of_t member)
	  (stringf_of_list Key.string_of_t know);
	printf "--------------------------------------\n";
	false
      end else
	match memrec.recv,memrec.cast2 with 
	  | None,None 
	  | Some _,None -> true
	  | None,Some _ -> 
	      printf "--------------------------------------\n";
	      printf "check_sanity failed. In %s recv and cast2 are None,Some _.\n"  (Member.string_of_t member);
	      printf "--------------------------------------\n";
	      false
	  | Some(key1), Some (enc,key2) -> (*key1=enc && List.mem key2 know *)
	      if key1 <> enc then begin
      		printf "--------------------------------------\n";
		printf "check_sanity failed. In %s recv %s but sends %s in cast2.\n" 
		  (Member.string_of_t member) (Key.string_of_t key1) 
		  (Key.string_of_t enc);
		printf "--------------------------------------\n";
		false
	      end else 
		if not (List.mem key2 know) then begin
      		  printf "--------------------------------------\n";
		  printf "check_sanity failed. %s does not have %s with which it encrtypts cast2.\n" (Member.string_of_t member) (Key.string_of_t key2) ;
		  printf "--------------------------------------\n";
		  false
		end else 
		  true
    with Not_found -> failwith "Not_found in check_sanity"

      
  (* Check that all members can decrypt and find all the keys from 
   * them to the root using the total multicast messages.
   *)
  let self_check verbose t (mm_tbl,orig_map,key_table,_) = 
    try 
      let mcl = mcast_sweep mm_tbl in
      let key_paths = key_paths t in
      let debug_map = Hashtbl.create 100 in
      let result = ref true in
      
    (* Check that all members (the senders) know all the keys they encrypt with. 
     *)
      Hashtbl.iter (fun member memrec -> 
	let know = List.map (function m,k -> k) memrec.send in
	let know = match memrec.recv with 
	  | None -> know 
	  | Some key -> key ::know
	in
	let know = know @ (keylist (MemMap.find member orig_map) member) in
	if not (check_sanity verbose member memrec orig_map) then 
	  result:=false;
	let know = decrypt know mcl in     
	let know = remove_dup (Sort.list (<) know) in
	let need_to_know = Hashtbl.find key_paths member in
	Hashtbl.add debug_map member {
	  memrec = memrec ;
	  know = know ;
	  need_to_know = need_to_know
	};
	if know <>  need_to_know then result:=false 
      ) mm_tbl;
      
      (* Check whether members get all the keys they are supposed to get. 
       * It completes the above check for non-sender members.
       *)
      let members = members_of_t t in
      let members = List.find_all (function m -> not (Hashtbl.mem mm_tbl m)) members in
      List.iter (function m -> 
	let know = keylist (MemMap.find m orig_map) m in
	let know = decrypt know mcl in     
	let know = remove_dup (Sort.list (<) know) in
	let need_to_know = Hashtbl.find key_paths m in
	Hashtbl.add debug_map m {
	  memrec =  { send = []; cast1 = []; recv = None; cast2 = None } ;
	  know = know ; 
	  need_to_know = need_to_know 
	} ;
	if know <> need_to_know then result:=false 
      ) members; 
      
      debug_map, !result, (stringf_of_list string_of_keyl) mcl

    with Not_found -> failwith "Not_found in self_check"
	
  (*********************************************************************)
  (*********************************************************************)
  (*********************************************************************)
	
  (* Zipping and unzipping trees. Trees have to be passed through
   * the network, in order to reduce message size their size should
   * be reduced. 
   *)
  type z = 
    | ZNil
    | ZLeaf of member
    | ZBr of z * key * z

  let rec string_of_z = function
    | ZNil -> ""
    | ZLeaf i -> Member.string_of_t i
    | ZBr (l,x,r) -> sprintf "(%s %s %s)" (string_of_z l) (Key.string_of_t x) (string_of_z r)

  let rec zip = function 
    | Nil -> ZNil
    | Leaf m -> ZLeaf m
    | Br(_,r,x,l,_) -> ZBr(zip r, x, zip l)

  let rec unzip = function 
    | ZNil -> Nil
    | ZLeaf m -> Leaf m
    | ZBr(r,x,l) -> create (unzip r) x (unzip l)
      
  let zempty = ZNil 

  let zleaf m = ZLeaf m 

  let rec zam_leader z m = match z with 
    | ZNil -> false
    | ZLeaf m' -> m=m'
    | ZBr(r,_,_) -> zam_leader r m

  let rec members_of_z = function 
    | ZNil -> []
    | ZLeaf m -> [m]
    | ZBr (l,_,r) -> (members_of_z l) @ (members_of_z r)

  (* [zkeylist t member] find the list of keys from [member] to the root 
   * on tree [t].
   *)
  let zkeylist z member = 
    let rec loop keyl = function
      | ZNil -> None
      | ZLeaf m -> if m=member then Some keyl else None
      | ZBr(l,key,r) -> 
	  match loop (key::keyl) l with 
	    | None -> begin match loop (key::keyl) r with 
		| None -> None 
		| Some l -> Some l
	      end
	    | Some l -> Some l 
    in
    match loop [] z with 
      | None -> failwith (sprintf "zkeylist %s %s failed" (string_of_z z) 
	    (Member.string_of_t member))
      | Some l -> List.rev l


  (* The following three functions are used for [remove_members_spec].
   * 
   * [remove_members_spec tree member rmv_l] returns the largest subtree 
   * of [tree] that contains [member] but does not contain any member from
   * [rmv_l].
   *)
  let rec zremove_members t rmv_list = match t with 
    | ZNil -> [ZNil]
    | ZLeaf member -> if List.mem member rmv_list then [] else [t]
    | ZBr (t1,_,t2) -> 
	let t1_new = zremove_members t1 rmv_list 
	and t2_new = zremove_members t2 rmv_list in
	if t1_new = [t1] && t2_new = [t2] then [t]
	else t1_new @ t2_new 
	  
  let rec mem t member = match t with 
    | ZNil -> false
    | ZLeaf m -> member=m 
    | ZBr (t1,_,t2) -> if mem t1 member then true else mem t2 member

  let zremove_members_spec t member rmv_list =
    let l = zremove_members t rmv_list in
    let rec loop = function 
      | [] -> raise Not_found
      | hd::tl -> if mem hd member then hd else loop tl 
    in loop l 

  (* [map f g tree] Map the tree. [f] works on the members, [g]
   * works on the keys. 
   *)
  let rec zmap f g tree = match tree with 
    | ZNil -> ZNil
    | ZLeaf i -> ZLeaf (f i)
    | ZBr (l,k,r) -> ZBr ( zmap f g l, g k, zmap f g r)

end


(************************************************************************)

									
									
									
    
