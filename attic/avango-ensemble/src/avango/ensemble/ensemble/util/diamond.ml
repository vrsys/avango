(**************************************************************)
(* DIAMOND.ML : A diamond structure *)
(* Author: Ohad Rodeh, 4/2000 *)
(*************************************************************************)
open Trans
open Util 
(*************************************************************************)
let name = Trace.filel "DIAMOND"
let log = Trace.log name 
let log1 = Trace.log (name^"1")
let log2 = Trace.log (name^"2")
(*************************************************************************)

(* A diamond structure 
 *)
  
(* The last *bool* denotes whether the top and bottom nodes are connected. 
*)
type 'a diam =
  | Hol
  | One of rank
  | Diam of rank option * 'a diam * 'a diam * rank option * bool * 'a

(* This is the regular type used through this module. 
 * There is an option to attach other "material" through the alpha
 * type.
*)
type t = unit diam

let empty = Hol
let singleton rank = One (rank)
let create x l r y cot = Diam(x, l, r, y, cot,())
  
(*let create x l r y= Diam (x,l,r,y)*)
  
let string_of_mem_opt u = match u with 
  | None -> "N"
  | Some x -> string_of_int x
      
(*  a
 * B C  -> Diam (a,B,C,d)
 *  d
 *)

let rec string_of_t = function
  | Hol -> "H"
  | One x -> "{" ^ string_of_int x ^ "}"
  | Diam (x,l,r,y,_,_) -> 
      "(" ^ string_of_mem_opt x ^ "," ^
      string_of_t l  ^ "," ^
      string_of_t r  ^ "," ^
      string_of_mem_opt y ^ ")"
      
(* Instructions for members. 
*)
type simple_cmd = 
  | Ack
  | Key 

(* The first array is for sources, the second is for
 * destinations. 
*)
type instr = (int * simple_cmd) list array * (int * simple_cmd) list array

let string_of_simple_cmd = function
  | Ack -> "Ack"
  | Key -> "Key"

let hd = function
  | Hol -> None
  | One x -> Some x
  | Diam (x,_,_,_,_,_) -> x
      
let last = function
  | Hol -> None
  | One x -> Some x
  | Diam (_,_,_,x,_,_) -> x
      
let connect = function
  | Hol 
  | One _ -> failwith "cot cannot be applied to Hol|One"
  | Diam (_,_,_,_,cot,_) -> cot

(* An approximation of a diamond size.
 *)
let rec diameter = function
  | Hol -> 0
  | One _ -> 1
  | Diam (x,l,r,y,_,_) -> 
      2 + max (diameter l) (diameter r)


let mem elm t = 
  let rec loop = function 
    | Hol -> false
    | One x -> x = elm
    | Diam (x,l,r,y,_,_) -> 
	if x= Some elm || y= Some elm then true
	else loop l || loop r
  in loop t
       

let rec members_of_t = function
  | Hol -> []
  | One x -> [x]
  | Diam (x,l,r,y,_,_) -> 
      filter_nones [x;y] @ members_of_t l @ members_of_t r
	
let rec map f = function 
  | Hol -> Hol
  | One x -> One (f x)
  | Diam (x,l,r,y,conns,a) -> 
      Diam (option_map f x, map f l, map f r, option_map f y, conns,a)


(*************************************************************************)
	(*  a
	 * B C -> put a and d at (length B)
	 *  d
	 *)
module Pretty_Print = struct
  let rec depth = function
    | Hol -> 0
    | One _ -> 1
    | Diam (x,l,r,y,_,_) -> 
	let len = match l,r with 
	  | Hol,Hol -> 1
	  | _,Hol -> depth l
	  | Hol, _ -> depth r
	  | _,_ -> max (depth l) (depth r) in
	2 + len
	  
	  
  let to_string t s_a depth = 
    let put depth index x = 
      (*printf "put %d,%d <- %d\n" depth index x;*)
      let s = s_a.(depth) in
      let len = String.length x in
      assert (index + len <= 60);
      String.blit x 0 s index len;
      index+len
    in      
    let rec loop t total_len d_top d_bot = match t with 
      | Hol  ->
	  put d_top total_len "H"
      | One x -> 
	  put d_top total_len (string_of_int x)
      | Diam (x,l,r,y,_,_) -> 
	  let len_l = loop l total_len (d_top+1) (d_bot-1) in
	  let len_x = put d_top len_l (string_of_mem_opt x) in
	  let len_y = put d_bot len_l (string_of_mem_opt y) in
	  let len_r = loop r (max len_x len_y) (d_top+1) (d_bot-1) in
	  len_r
    in 
    let _ = loop t 0 0 (depth-1) in
    ()
    
  let string_of_t t = 
    let depth = depth t in 
    let s_a = Array.init (depth+1) (fun _ -> String.make 60 ' ') in
    to_string t s_a depth;
    String.concat "\n" (Array.to_list s_a)
      
end
(*************************************************************************)

(* Export the pretty printing function itself.
*)
let pretty_string_of_t = Pretty_Print.string_of_t  
  

(* Split a diamond into a set of reachability classes
 *)
      
(*  This is a data structure that helps pass up reachablity information. 
 *  The idea is that a diamond can be split into the list that is connected 
 *  to the top member, a list connected to the bottom member, and the 
 *  rest of the components. 
 *)
type v = 
  | Com of rank list
  | Man of rank list * rank list list
  | Spl of rank list * rank list list * rank list
      

let string_of_v = function
  | Com l -> "Com(" ^ string_of_int_list l  ^ ")"
  | Man (t,m) -> "Man(" ^ string_of_int_list t  ^ "," ^ 
      string_of_list string_of_int_list m ^ ")"
  | Spl(t,m,b) -> "Spl(" ^ string_of_int_list t ^ "," ^
      string_of_list string_of_int_list m ^ "," ^
      string_of_int_list b ^ ")"

let merge_all = function
  | Com l -> [l]
  | Man (l,m) -> l::m
  | Spl (t,m,b) -> t::b::m 

(* [reach strict t]
 * Compute the reachability classes of diamond [t]. The strict flag
 * is used to require that in the case:
 *                 a
 *                H H   
 *                 d
 * There is a connection between a and d. 
*)      
let reach t = 
  let rec reach1 t = 
    let top = function
      | Com l -> l
      | Man (l,_) -> l
      | Spl (l,_,_) -> l
    and top_exl = function
      | Com _ -> []
      | Man _ -> []
      | Spl (l,_,_) -> l
    and bot = function
      | Com l -> l
      | Man (l,_) -> l
      | Spl (_,_,l) -> l 
    and bot_exl = function
      | Com _ -> []
      | Man _ -> []
      | Spl (_,_,l) -> l
    and mid = function
      | Com l -> []
      | Man (_,l) -> l
      | Spl (_,l,_) -> l in
    match t with 
      | Hol -> Com []
      | One x -> Com [x]
      | Diam (x,lT,rT,y,cot,_) -> 
	  let l = reach1 lT in
	  let r = reach1 rT in 
	  match x,y with 
	    | None,None -> 
		Spl ([], merge_all l @  merge_all r, [])
	    | Some x', None ->
		Spl (x' :: top l @ top r, bot_exl l :: bot_exl r:: mid l @ mid r, []) 
	    | None,Some y' ->
		Spl ([], top_exl l :: top_exl r:: mid l @ mid r, y'::bot l @ bot r)
	    | Some x', Some y' -> 
		let handle_one_side s = 
		  match s with 
		    | Com l -> Com (x'::y'::l)
		    | Man (l,m) -> Man(x'::y'::l,m)
		    | Spl (t,m,b) -> Spl(x'::t,m,y'::b) 
		in
		match lT,rT with 
		  | Hol,Hol -> 
		      if cot then Com([x';y'])
		      else Spl([x'],[],[y'])
		  | Hol,_ -> handle_one_side r
		  | _,Hol -> handle_one_side l
		  | _,_ -> 
		      match l,r with 
			| Com l, Com r -> Com(x'::y'::l@r)
			| Com l, Man (rt,rm) -> Man(x'::y'::rt@l,rm)
			| Com l, Spl (rt,rm,rb) -> Man(x'::y'::l@rt@rb,rm)
			| Man (lt,lm), Com r -> Man(x'::y'::lt@r,lm)
			| Man (lt,lm), Man(rt,rm) -> Man(x'::y'::lt@rt, lm@rm)
			| Man (lt,lm), Spl(rt,rm,rb) -> Man(x'::y'::lt@rt@rb,lm@rm)
			| Spl(lt,lm,lb),Com r -> Man(x'::y'::lt@lb@r, lm)
			| Spl(lt,lm,lb),Man (rt,rm) -> Man(x'::y'::lt@lb@rt, lm@rm)
			| Spl(lt,lm,lb),Spl (rt,rm,rb) -> Spl(x'::lt@rt,lm@rm,y'::lb@rb)
  in
  let clean_mid m = List.find_all (function l -> l<>[]) m in
  match reach1 t with
    | Com l -> Com l 
    | Man (t,m) -> Man(t,clean_mid m)
    | Spl(t,m,b) -> Spl(t,clean_mid m,b)


(* Remove all dead members from the tree. 
 * for example: 
 * 
 *    x           x
 *  a   N       a
 * b c H H  -> b c  H
 *  d   N       d
 *    y           y
 * 
 * We must be carefull to keep the middle connection (if it exists) accurate.
 *)

let strip1 strict t = 
  let rec loop = function
  | Hol -> Hol,false
  | One x -> One x,true
  | Diam (x,l,r,y,cot,()) -> 
      match x, y with 
	| None,None -> (match loop l, loop r with 
	    | (l',true),(r',true) -> 
		failwith (sprintf 
		  "Impossible -- components are not connected %s" 
		  (string_of_t t))
	    | (l',true),(_,false) -> l',true  (* We are done here in fact *)
	    | (_,false),(r',true) -> r',true  (* We are done here in fact *)
	    | (_,false),(_,false) -> Hol,false
	  )
	| None, Some y' -> (match loop l, loop r with 
	    | (l',true),(r',true) -> Diam(None,l',r',y,false,()),true
	    | (l',true),(_,false) -> Diam(None,l',Hol,y,false,()),true
	    | (_,false),(r',true) -> Diam(None,Hol,r',y,false,()),true
	    | (_,false),(_,false) -> 
		if strict then (One y'),true
		else Diam(None,Hol,Hol,Some y',false,()),true
	  )
	| Some x', None -> (match loop l, loop r with 
	    | (l',true),(r',true) -> Diam(x,l',r',None,false,()),true
	    | (l',true),(_,false) -> Diam(x,l',Hol,None,false,()),true
	    | (_,false),(r',true) -> Diam(x,Hol,r',None,false,()),true
	    | (_,false),(_,false) -> 
		if strict then (One x'),true
		else Diam(Some x',Hol,Hol,None,false,()),true
	  )
	| Some _, Some _ -> (match loop l, loop r with 
	    | (l',true),(r',true) -> Diam(x,l',r',y,cot,()),true
	    | (l',true),(_,false) -> Diam(x,l',Hol,y,cot,()),true
	    | (_,false),(r',true) -> Diam(x,Hol,r',y,cot,()),true
	    | (_,false),(_,false) -> Diam(x,Hol,Hol,y,cot,()),true
	  ) in 
  let t',_ = loop t in
  let t' = match t' with 
    | Hol -> Hol
    | One x -> t'
    | Diam(x,l,r,y,cot,()) -> 
	match x,y,l,r with 
	  | None, Some y',Hol,Hol -> One y'
	  | Some x', None,Hol,Hol -> One x'
	  | _ -> t' in
  log (fun () -> sprintf "strip %s->%s\n" (string_of_t t) (string_of_t t'));
  t'

let strip = strip1 true
let strip_not_strict = strip1 false

(* Check if a diamond is fully conected. 
 * 
 * This is a strict test. The idea is to check 
 * whether the diamond remained one-connected after 
 * members were removed. The test here is a bit stricter
 * because we reqire that the case: 
 *              a
 *             H H
 *              b
 * where there is no connection between [a] and [b] to 
 * count as not connected. 
*)
let fully_connected t =
  match reach t with
    | Com _ -> true
    | Man (t,m) -> m=[] 
    | Spl(t,m,b) -> m=[] && (t=[] || b=[])

(* [split t elm rmv_l] Set all the members in [rmv_l] to None in the 
 * tree. Find the component that contains [elm].
 *)
let split2 t elm rmv_l = 
  let f = function 
    | None -> None 
    | Some x -> 
	if List.mem x rmv_l then None 
	else Some x in
  let rec loop = function 
    | Hol -> Hol
    | One x -> 
	if List.mem x rmv_l then Hol
	else One x
    | Diam (x,l,r,y,cot,()) -> Diam (f x, loop l, loop r, f y, cot,()) in
  let t = loop t in
  let v = reach t in
  let l = merge_all v in
  let res = List.fold_left (fun accu t -> match accu with
    | None -> if List.mem elm t then Some t else None
    | Some _ -> accu
  ) None l in
  match res with 
    | None -> 
	failwith (sprintf "split failed, did not find %d in the subtree." elm)
    | Some t -> t 

(* Mark all members outside my component as dead, and remove dead 
 * pieces from the tree. 
*)
let split t elm rmv_l = 
  let check_sanity res = 
    let l = members_of_t res in
    let rep = 
      if not (List.mem elm l) then false
      else 
	List.fold_left (fun accu e -> 
	  if not accu then false
	  else if List.mem e l then false
	  else true
	) true rmv_l in
    if not rep then 
      eprintf "split:check_sanity: failed %s %d %s\n" (string_of_t t) elm 
	(string_of_int_list rmv_l);
    rep
  in
  let my_component = split2 t elm rmv_l in
  let f = function 
    | None -> None 
    | Some x -> 
	if not (List.mem x my_component) then None 
	else Some x in
  let rec loop = function 
    | Hol -> Hol
    | One x -> 
	if not (List.mem x my_component) then Hol
	else One x
    | Diam (x,l,r,y,cot, a) -> Diam (f x, loop l, loop r, f y, cot, a) in
  let t = loop t in
  let t = strip_not_strict t in
  assert (check_sanity t);
  t
    
    
(*************************************************************************)
(* This module attaches a size count to each sub-diamond. 
 * This allows adding members in a balanced manner to an
 * exisiting structure.
*)
module TreeSize = struct
  type st = int diam
    
  (* Add a single member to the tree. 
   * First, try to fill up holes. 
   * If there aren't any holes, then open a new diamond.
   * 
   * Note: We must be carefull not add connections to the tree. 
   *)
  let size = function
    | Hol -> 0
    | One x -> 1
    | Diam(_,_,_,_,_,s) -> s

  let rec st_of_t = function
    | Hol -> Hol 
    | One x -> One x
    | Diam(x,l,r,y,cot,()) -> 
	let l' = st_of_t l
	and r' = st_of_t r in
	let num = List.length (filter_nones [x;y]) in
	Diam(x,l',r',y,cot,size l' + size r' + num) 
    
  let rec t_of_st = function
    | Hol -> Hol 
    | One x -> One x
    | Diam(x,l,r,y,cot,_) -> Diam(x, t_of_st l, t_of_st r, y, cot,())

  let rec add_one (t : st) elm = 
    (* [fill_node t x] fill an empty space in tree [t] with member [x].
     * FDiam(None,...) -> FDiam(Some x,...)
     *)
    let rec fill_node = function 
      | Hol -> One elm,true
      | One _ as t -> t,false
      | Diam (x,l,r,y,cot,sz) ->
	  match x,y with 
	    | None,_ -> Diam (Some elm,l,r,y,false,sz+1),true
	    | _,None -> Diam (x,l,r,Some elm,false,sz+1),true
	    | _ -> 
		let l',flag_l = fill_node l in
		if flag_l then Diam (x,l',r,y,cot,sz+1),true
		else (
		  let r',flag_r = fill_node r in
		  if flag_r then Diam (x,l,r',y,cot,sz+1),true
		  else t,false
		)
    in
    (* [fill_hole t x] fill an empty subtree in [t] with member [x].
     * FHol -> FOne x.
     *)
    let rec fill_hole = function 
      | Hol -> One elm,true
      | One _ as t -> t,false
      | Diam (x,l,r,y,cot,sz) ->
	  match l,r with 
	    | Hol,_ ->  Diam (x,One elm,r,y,cot,sz+1), true
	    | _,Hol ->  Diam (x,l,One elm,y,cot,sz+1), true
	    | _ -> 
		let l',flag_l = fill_hole l in
		if flag_l then Diam (x,l',r,y,cot,sz+1),true
		else (
		  let r',flag_r = fill_hole r in
		  if flag_r then Diam (x,l,r',y,cot,sz+1),true
		  else t,false
		)
    in
    (* [add_new t x] Add a member to the smaller subtree. 
     * Make *sure* that the cot structure is correct. To do
     * so we need to pass the big tree down. For example:
     *    a         a
     *   B c ->    B  c
     *    d           x
     *              d
     * We must make sure that d and x are not connected.
     *)
    let rec add_new = function
      | Hol -> 
	  failwith "Problem in add_one, empty tree"
      | One x -> 
	  Diam (Some x,Hol,Hol,Some elm,false, 2)
      | Diam (x,l,r,y,cot,sz) -> 
	  if size l < size r then 
	    Diam (x, add_new l , r, y, cot, sz+1)
	  else 
	    Diam (x, l, add_new r , y, cot, sz+1)
    in
    let t,flag = fill_node t in
    if flag then t
    else 
      let t,flag = fill_hole t in
      if flag then t
      else add_new t 
	
		
  (* [add_one t x] Add member [x] to tree [t]. We already know that
   * t has no empty slots. 
   *)
  let rec add_members ft = function
    | [] -> ft
    | hd::tl -> add_members (add_one ft hd) tl 
end
(*************************************************************************)

(* Export the [add_members] function
*)
let add_members	t l = TreeSize.t_of_st (TreeSize.add_members (TreeSize.st_of_t t) l)


(*************************************************************************)
(*  In this sub-module we add some statistics to the 
 * information carried in each tree node.
 *)
module FullTree = struct
  
  (* The diamond: 
   * 1. Not connected
   * 2. only to the top
   * 3. only the bottom, 
   * 4. to both
   *)
  type conn = 
    | No
    | Top
    | Bot
    | Full

  (* The type of a command. There are two kinds: 
   * 1) Send an ack to a member    2) Send a key to a member.
   *)
  type cmd = 
    | Nop
    | UpAck
    | DnAck
    | UpKey
    | DnKey
	
  (* Type full_t. 
   *)
  type ft = (conn * (cmd * cmd * cmd * cmd * cmd)) diam
	
  let conn = function
    | Hol -> No
    | One _ -> Full
    | Diam (_,_,_,_,_,(c,_)) -> c
	
  let string_of_conn = function
    | No -> "No"
    | Top -> "Top"
    | Bot -> "Bot"
    | Full -> "Full"

  module Pretty_print = struct
    let string_of_cmd = function
      | Nop   -> "_"
      | UpAck -> "A"
      | DnAck -> "a"
      | UpKey -> "K"
      | DnKey -> "k"
	  
  let rec depth = function
    | Hol -> 0
    | One _ -> 1
    | Diam (x,l,r,y,_,_) -> 
	let len = match l,r with 
	  | Hol,Hol -> 1
	  | _,Hol -> depth l
	  | Hol, _ -> depth r
	  | _,_ -> max (depth l) (depth r) in
	4 + len
	  
    let to_string t s_a depth = 
      let put depth index x = 
	(*printf "put %d,%d <- %d\n" depth index x;*)
	let s = s_a.(depth) in
	let len = String.length x in
	assert (index + len <= 60);
	String.blit x 0 s index len;
	index+len
      in      
      let rec loop t total_len d_top d_bot = match t with 
	| Hol  ->
	    put d_top total_len "H"
	| One x -> 
	    put d_top total_len (string_of_int x)
	| Diam (x,l,r,y,_,(_,(c1,c2,cm,c3,c4))) -> 
	    let len_l = loop l total_len (d_top+2) (d_bot-2) in
	    let len_c1 = put (d_top+1) len_l (string_of_cmd c1) in
	    let len_c3 = put (d_bot-1) len_l (string_of_cmd c3) in
	    let len = max len_c1 len_c3 in
	    let len_x = put d_top len (string_of_mem_opt x) in
	    let len_y = put d_bot len (string_of_mem_opt y) in
	    let len_cm = 
	      if cm<>Nop then 
		put ((d_bot+d_top)/2) len (string_of_cmd cm) 
	      else len in
	    let len = max (max len_x len_y) len_cm in
	    let len_c2 = put (d_top+1) len (string_of_cmd c2) in
	    let len_c4 = put (d_bot-1) len (string_of_cmd c4) in
	    let len_r = loop r (max len_c2 len_c4) (d_top+2) (d_bot-2) in
	    len_r
      in 
      let _ = loop t 0 0 (depth-1) in
      ()
      

    let string_of_t t = 
      let depth = depth t in 
      let s_a = Array.init (depth+1) (fun _ -> String.make 60 ' ') in
      to_string t s_a depth;
      String.concat "\n" (Array.to_list s_a)
  end
    
  let pretty_string_of_t = Pretty_print.string_of_t

  (* Transform tree type to a full_tree. Do not compute the 
   * connectivity yet. This will be done later. 
   *)
  let rec ft_of_t = function
    | Hol -> Hol
    | One x -> One x
    | Diam (x,l,r,y,cot,()) ->  
	let l' = ft_of_t l 
	and r' = ft_of_t r in 
	Diam (x, l', r', y, cot, (No,(Nop,Nop,Nop,Nop,Nop)))


  let rec t_of_ft = function 
    | Hol -> Hol
    | One x -> One x
    | Diam (x,l,r,y,cot,_) -> Diam (x, t_of_ft l, t_of_ft r,y,cot,()) 
	
	
    (* Flip an [ft] over:
     *  a          d 
     * B C  -> F(B) F(C)
     *  d          a
     *)
  let flip ft = 
    let flip_cmd = function
      | Nop  -> Nop 
      | UpAck -> DnAck
      | DnAck -> UpAck
      | UpKey -> DnKey
      | DnKey -> UpKey in
    let flip_cmds (c1,c2,cm,c3,c4) = 
      (flip_cmd c3,flip_cmd c4,flip_cmd cm,flip_cmd c1,flip_cmd c2) in
    let flip_conn = function
      | No -> No 
      | Top -> Bot
      | Bot -> Top
      | Full -> Full in
    let rec loop = function
      | Hol -> Hol
      | (One x) as ft -> ft
      | Diam (x,l,r,y,cot,(c,cmds)) -> 
	  Diam (y, loop l, loop r, x, cot, (flip_conn c, flip_cmds cmds))
    in loop ft

  (*  a       a
   * B C  -> C B 
   *  d       d
   *)
  let flip_side = function
      | Hol -> Hol
      | One _ as t -> t
      | Diam(x,l,r,y,cot, (c,(c1,c2,cm,c3,c4))) -> 
	  Diam(x,r,l,y,cot, (c,(c2,c1,cm,c4,c3)))

	
  (* [comp_con t] compute the connectivity of diamond t. This
   * is done recursively for all of its sub-diamonds. 
   * 
   * The strict variant.
   * This version computes the recursive connectivity assuming that
   * the diamond is one connected already. This means that we 
   * can be more "choosy" in our route choices. We can disseminate
   * messages without creating new channels AT ALL.
   *)
  let rec comp_con strict = function
    | Hol -> Hol
    | (One x) as ft -> ft
    | Diam (x,l,r,y,cot,(_,cmds)) ->  
	let l' = comp_con strict l 
	and r' = comp_con strict r in 
	let conn = 
	  (* 1. One of the sides of the diamond may not be connected.
	   * 2. One of the leaders can be dead, in which case, both
	   *    sides should be connected. 
	   * 3. It is possible that one of the sides is cut in the middle:
	   *   
	   *       a
	   *    d   
	   *   N N   C
	   *    e 
	   *       f
	   * 
	   *    If C is full, we can pass through it. 
	   *)
	  match x, y, conn l', conn r' with 
	    | Some _,Some _,Full,_
	    | Some _,Some _,_,Full -> Full
	    | Some _,Some _,No,No -> 
		if strict then
		  if r=Hol && l=Hol && cot then Full else No
		else
		  if r=Hol && l=Hol then Full else No
	    | Some _,None,(Top|Full),(Top|Full) -> Top 
	    | None,Some _,(Bot|Full),(Bot|Full) -> Bot
	    | Some _,None,No,No -> 
		if r=Hol && l=Hol then Top else No
	    | Some _,None,(Top|Full),No -> 
		if r=Hol then Top else No
	    | Some _,None,No,(Top|Full) -> 
		if l=Hol then Top else No
	    | None,Some _,No,No -> 
		if r=Hol && l=Hol then Bot else No
	    | None,Some _,(Bot|Full),No -> 
		if r=Hol then Bot else No
	    | None,Some _,No,(Bot|Full) -> 
		if l=Hol then Bot else No
	    | x,y,z,w -> 
		log2 (fun () -> sprintf "%s,%s,%s,%s\n" 
		    (string_of_mem_opt x) (string_of_mem_opt y) 
		    (string_of_conn z) (string_of_conn w));
		No
	in
	Diam (x, l', r', y, cot, (conn, cmds))
	  
	  
    (* [compute_instr ft]
     * Compute the set of instructions required to build the first
     * phase of the tree.
     * 
     * We use a couple of rules here:  
     * 
     * k> = send a key     a> send an ack    *> The component isn't full
     * 
     *    e          e         e
     *   k k        k k       k  k
     *  B   C*     B   C            y
     *   k k        k a     B      k k
     *    d          d            U   V* 
     *                       k     a k
     *                              z
     *                           k
     *                         d
     *    A    A                   A
     *    k    k                   k
     *    |    |                   |
     *    e    e*                  e*
     * b* a       send the ack     |  send the ack to A
     *    d    d  to A             k  bypass e totally.
     *    |    |
     *    k    k
     *)
	
    (* From which direction do we pass the key. 
     * It is possible to arrive from the bottom, from the top, or
     * from both directions
     *)
  type direction = 
    | FromBot
    | FromTop
    | Both
	
    (*  Create a hashtable that will keep all the commands. 
     * This hashtable will then need to be optimized. This
     * algorithm may create a flow where a key arrives from two
     * sources to the same destination. 
     *)
  let compute_instr strict t =
    let ft = comp_con strict t in
    let rec loop direction ft = 
      let from_top = function
	| Hol 
	| One _ -> failwith "Sanity, FHol|FOne in from_top"
	| Diam (x,tL,tR,y,cot,(c,_)) as t -> 
	    let from_top_left_is_full l r = match conn r with 
	      | Full -> 
		  Diam (x, loop FromTop l, loop FromTop r, y, cot ,
		                (c,(DnKey,DnKey,Nop,DnKey,DnAck)))
	      | Top ->
		  Diam (x, loop FromTop l, loop FromTop r, y, cot,
		                (c,(DnKey,DnKey,Nop,DnKey,Nop)))
	      | Bot ->
		  Diam (x, loop FromTop l, loop FromBot r, y, cot,
           		        (c,(DnKey,Nop,Nop,DnKey,UpKey)))
	      | No -> (
		  match r with 
		    | Hol -> 
			Diam (x, loop FromTop l, Hol, y, cot,
 			           (c, (DnKey,Nop,Nop,DnKey,Nop)))
		    | One _ -> 
			Diam (x, loop FromTop l, r, y, cot,
			           (c, (DnKey,DnKey,Nop,DnKey,DnAck)))
		    | Diam _ -> 
			Diam (x, loop FromTop l, loop Both r, y, cot,
			           (c, (DnKey,DnKey,Nop,DnKey,UpKey)))
		)
	    in
	    log2 (fun () -> sprintf "%s,%s,%s\n" (string_of_conn c) 
		(string_of_conn (conn tL)) (string_of_conn (conn tR)));
	    match c, conn tL,conn tR with 
	      | Full,Full,_ -> 
		  from_top_left_is_full tL tR
	      | Full,_,Full -> 
		  flip_side (from_top_left_is_full tR tL)
	      | Top,(Top|Full),(Top|Full) -> 
		  Diam (x, loop FromTop tL, loop FromTop tR, y, cot, 
  		                          (c,(DnKey,DnKey,Nop,Nop,Nop)))
	      | Top,No,(Top|Full) -> 
		  Diam (x, loop FromTop tL, loop FromTop tR, y, cot,
		                          (c,(Nop,DnKey,Nop,Nop,Nop)))
	      | Top,(Top|Full),No -> 
		  Diam (x, loop FromTop tL, loop FromTop tR, y, cot,
		                          (c,(DnKey,Nop,Nop,Nop,Nop)))
	      | Full,No,No -> 
		  if tL=Hol && tR=Hol then 
		    Diam (x, Hol, Hol, y, cot,  
		                          (c,(Nop,Nop,DnKey,Nop,Nop)))
		  else failwith "compute_instr: Full,No,No, Cannot reach from top"
	      | Top,No,No -> 
		  if tL=Hol && tR=Hol then 
		    Diam (x, Hol, Hol, None, cot,  
		                          (c,(Nop,Nop,Nop,Nop,Nop)))
		  else failwith "compute_instr: Top,No,No, Cannot reach from top"
	      | _ -> 
		  failwith "compute_instr: ?,?,? Cannot reach tree from top"
      in
      match direction,ft with
	| _, Hol  
	| _, One _ -> ft
	| FromTop, Diam _ -> from_top ft
	| FromBot, Diam _ -> flip (from_top (flip ft))
	| Both, Diam(x,l,r,y,cot,(c,_)) ->
	    match x,y with 
	      | None,None -> failwith "compute_instr:Both,Diam, disconnected"
	      | None,Some _ -> 
		  Diam (x,loop FromBot l, loop FromBot r, y, cot,
	                              (c, (Nop, Nop, Nop, UpKey, UpKey)))
	      | Some _, None -> 
		  Diam (x,loop FromTop l, loop FromTop r, y, cot,
	                              (c, (DnKey, DnKey, Nop, Nop, Nop)))
	      | Some _, Some _ -> 
		  Diam (x,loop Both l, loop Both r, y, cot,
	                              (c, (DnKey, DnKey, Nop, UpKey, UpKey)))
	      
    in
    log2 (fun () -> sprintf "conn ft=%s\n" (string_of_conn (conn ft)));
    match conn ft with 
      | No -> 
	  eprintf "Tree is not connected \n%s\n" (pretty_string_of_t ft);
	  failwith "compute_instr:Tree is not connected"
      | Full
      | Top -> loop FromTop ft
      | Bot -> loop FromBot ft


  let instructions_of_ft ft n = 
    let a_s = Array.create n [] 
    and a_d = Array.create n [] in
    let add_cmd x y cmd = 
      log1 (fun () -> sprintf "add_cmd %d %d %s" x y (string_of_simple_cmd cmd));
      a_s.(x) <- (y,cmd)::a_s.(x) ;
      a_d.(y) <- (x,cmd)::a_d.(y)
    in
    let add x y cmd = match x,y with 
      | Some x, Some y -> (
	  match cmd with 
	    | DnAck -> add_cmd x y Ack 
	    | UpAck -> add_cmd y x Ack 
	    | DnKey -> add_cmd x y Key
	    | UpKey -> add_cmd y x Key
	    | Nop   -> ()
	)
      | _ -> () 
    in
    let rec loop = function
      | Hol 
      | One _ -> () 
      | Diam(x,l,r,y,_,(_,(c1,c2,cm,c3,c4))) -> 
	  add x (hd l) c1;
	  add x (hd r) c2;
	  add x  y     cm;
	  add (last l) y c3;
	  add (last r) y c4;
	  log1 (fun () -> "Done one loop");
	  loop l;
	  loop r
    in loop ft;

    (* patch for the case that a member has no one to send 
     * an ack to. 
     * 
     * First, find the list of members that are at the end
     * of the graph. If there is only one, then there is nothing
     * to do. If there are several, choose the first as the SINK,
     * and have the rest send Acks to the SINK. 
     * 
     *)
    let res = ref [] in
    Array.iteri (fun i v -> 
      if v=[] then res := i::!res
    ) a_s;
    (match !res with 
      | [] -> failwith "instructions_of_ft: failed, no SINK"
      | [hd] -> log (fun () -> "instructions_of_ft: Just one SINK")
      | hd::tl -> 
	  List.iter (function i -> add_cmd i hd Ack) tl
    );  
    a_s,a_d


  let final_instr ft nmembers = 
    let rec loop = function
      | Hol 
      | One _ as ft' -> ft'
      | Diam (x,l,r,y,cot,(c,_)) -> 
	  let cmds = 
	    match l,r with 
	      | Hol,Hol -> (Nop,Nop,DnKey,Nop,Nop) 
	      | Hol, _ -> (Nop,DnKey,Nop,Nop,DnKey)
	      | _,Hol -> (DnKey,Nop,Nop,DnKey,Nop)
	      | _,_ -> (DnKey,DnKey,Nop,DnKey,DnKey) in
	  Diam (x,loop l,loop r,y,cot,(c,cmds)) 
    in
    
    let ft = loop ft in
    log (fun () -> sprintf "diamond key=\n%s\n" (pretty_string_of_t ft));
    instructions_of_ft ft nmembers

  let string_of_instructions (a_s,a_d) = 
    let f = string_of_array (function l -> 
      string_of_list (function rank,cmd -> 
	string_of_int rank ^ ":" ^ string_of_simple_cmd cmd
      ) l
    ) in
    "src=" ^ f a_s ^ " dst=" ^ f a_d
    
end
(*************************************************************************)

(* Export the fulltree type, for debugging purposes. 
*)
type ft = FullTree.ft  

(* [merge_one et_l]
 * Create a new one-connected component. 
 * Take the largest element [max], break up the rest, and try filling
 * [max].
 * max must be one-connected. It is possible that no such component exists, 
 * and we shall have to break up all of them. 
 *)
let merge_one et_l = 
  let max_comp,rest = List.fold_left (fun (max_c,rest) t -> 
    assert (fully_connected t);
    let d = diameter t in
    match max_c with 
      | None -> Some (t,d),rest
      | Some (max_c,d') -> 
	  if d > d' then Some(t,d), (members_of_t max_c) @ rest
	  else Some(max_c,d'), (members_of_t t) @ rest
  ) (None,[]) et_l in
  let t,l = match max_comp with 
    | None -> One (List.hd rest), List.tl rest
    | Some (max_c,_) -> max_c,rest
  in 
  log2 (fun () -> sprintf "t=%s l=%s\n" (string_of_t t) (string_of_int_list l));
  add_members t l 
    
    
(*************************************************************************)
(* [two_connect t] takes a one_connected tree t, and makes it two-connected.
 * 
 * The [steal_one t] function "steals" one node from subtree [t]. It tries
 * to take the node that is inner-most in the tree. This minimizes the disruption
 * to the tree. 
 * 
 * The basic idea is, to recursively,  make sure that each diamond has a first
 * and a last node. If not, then it has only one member. In this case it can be 
 * rewritten as a singelton. 
 * 
 * the "cov" values are not-important here. We set all relevant connections
 * to true later. 
 * 
 * The correct_right_left function balances between the left and right 
 * sides. 
*)
module TwoConnect = struct
  open TreeSize
    
  (* [real_balance x l r y sz] 
   * Take a diamond with non empty First, Last, Left and Right. It
   * is possible that the left and right sides are not balanced. This
   * function balances them, at low cost. 
   *)
  let rec real_balance t = 
    (* [merge b x s y] assumes [b] is larger by two than [s], and merges [s] 
     * into [b]. [x] and [y] are the first and last members. 
     *)
    let merge b x s y = 
      match b with 
	| Hol 
	| One _ -> failwith "real_balance:merge sanity"
	| Diam(bx,bl,br,by,_,sz) -> 
	    if size bl > size br then 
	      Diam(bx, bl, real_balance (Diam (x,br,s,y,false, size s + size br + 2)), by, false, sz)
	    else 
	      Diam(bx, real_balance (Diam (x,s,bl,y,false, size s + size bl + 2)), br, by, false, sz)
    in	  
    match t with 
      | Hol 
      | One _ -> t
      | Diam(x,l,r,y,_,sz) -> 
	  if size l > 3 * size r + 2 then (
	    match l with 
	      | Hol
	      | One _ -> failwith "real_balance: Hol,One _ are too small"
	      | Diam _ -> merge l x r y 
	  ) else if size r > 3 * size l + 2 then (
	    match r with 
	      | Hol
	      | One _ -> failwith "real_balance: Hol,One _ are too small"
	      | Diam _ -> merge r x l y
	  ) else t
	    
	    
  let rec two_connect1 diam= 
    let rec steal_one = function
      | Hol -> failwith "steal_one: cannot steal from Hol"
      | One x -> Hol,x
      | Diam(x,l,r,y,cov,sz) -> 
	  match l,r with 
	    | Hol,Hol -> (
		match x,y with 
		  | None, None -> failwith "steal_one: cannot steal from None,Hol,Hol,None"
		  | Some x',None -> Hol,x'
		  | None,Some y' -> Hol,y'
		  | Some x',Some y' -> (One x'), y'
	      )
	    | One z, Hol -> Diam(x,Hol,Hol,y,false,sz-1),z
	    | Hol,One z ->  Diam(x,Hol,Hol,y,false,sz-1),z
	    | One z,One w -> Diam(x,One z,Hol,y,false,sz-1),w
	    | Diam _, _ -> 
		let l',res = steal_one l in
		Diam(x,l',r,y,false,sz -1),res
	    | _, Diam _ -> 
		let r',res = steal_one r in
		Diam(x,l,r',y,false,sz -1),res  in
    let balance x l r y sz = 
      let res = 
	match l,r with 
	  | Hol,Hol 
	  | Hol,One _
	  | One _,Hol -> Diam(Some x,l,r,Some y, false,sz)
	  | Diam _, Hol -> 
	      let l',res = steal_one l in
	      Diam(Some x, two_connect1 l',One res, Some y, false,sz)
	  | Hol, Diam _ -> 
	      let r',res = steal_one r in
	      let r' = two_connect1 r' in
	      Diam(Some x,One res, r', Some y, false,sz)
	  | _ -> 
	      Diam(Some x, two_connect1 l, two_connect1 r, Some y, false, sz) in
      real_balance res

    in
    (*eprintf "two_connect1 \n%s\n" (pretty_string_of_t diam);*)
    match diam with 
      | Hol -> Hol
      | One x as t -> t
      | Diam(x,l,r,y,_,sz) as t -> 
	  let steal_from_min l r = 
	    if size l > size r then 
	      let l',res = steal_one l in 
	      (l',r,res)
	    else 
	      let r',res = steal_one r in
	      (l,r',res)
	  in
	  match x,l,r,y with 
	    | None,Hol,Hol,None -> Hol
	    | Some x',Hol,Hol,None -> One x'
	    | None,Hol,Hol,Some y' -> One y'
	    | _,Hol,Hol,_ -> t
	    | None,_,_,None -> 
		failwith "two_connect:None,_,_,None, the graph is not one-connected"
	    | Some x',_,_, None -> 
		let l',r',res = steal_from_min l r in
		balance x' l' r' res sz
	    | None,_,_, Some y' -> 
		let l',r',res = steal_from_min l r in
		let res = balance res l' r' y' sz in
		res
	    | Some x',_,_,Some y' -> 
		balance x' l r y' sz
		  
  (* This function checks if a tree is two-connected or not.
   * This is for sanity checks and debugging only. 
   *)
  let rec check_two_connected = function
    | Hol -> false
    | One x -> true
    | Diam(x,l,r,y,_,_) -> 
	match x,y with 
	  | Some _,Some _ -> (
	      match l,r with 
		| Hol,Hol 
		| Hol,One _ 
		| One _,Hol -> true
		| Hol, Diam _ 
		| Diam _, Hol -> false
		| _ -> check_two_connected l && check_two_connected r
	    )
	  | _ -> false

  let two_connect t = 
(*    let t' = 
    try 
      two_connect1 t 
    with e -> 
      eprintf "two_connect failed, \n%s\n" (pretty_string_of_t t);
      raise e
    in*)
    let t' = two_connect1 t in
    assert (check_two_connected t');
    (*if not (check_two_connected t') then (
      eprintf "TwoConnect.check_two_connected failed\n";
      eprintf "t=\n%s\n" (pretty_string_of_t (TreeSize.t_of_st t'));
      failwith ""
    );*)
    t'
end

let two_connect t = TreeSize.t_of_st (TwoConnect.two_connect (TreeSize.st_of_t t))

(*************************************************************************)
(* [check_sched t sched] checks if the schedule [sched] makes sense. 
*)
module CheckSched = struct
  (* Check that a command [cmd'] is really supposed to come from 
   * [peer].
   *)
  let check_sanity peer cmd' l =
    let res = List.fold_left (fun accu (rank,cmd) -> 
      if rank=peer then 
	if cmd<>cmd' then Some false
	else Some true
      else accu
    ) None l in
    match res with 
      | None -> failwith "No such source"
      | Some false -> failwith "bad command"
      | Some true -> () (*true*)
	  
  let rec bfs node_l (s_a,d_a) a_r comp = 
    (*    eprintf "bfs %s\n" (string_of_int_list node_l);
    let string_of_l l = string_of_list (function (rank,cmd) -> 
      sprintf "%d,%s" rank (string_of_simple_cmd cmd)) l in
    List.iter (function rank -> 
      eprintf "%s,%s\n" (string_of_l s_a.(rank)) (string_of_l d_a.(rank))
    ) node_l ;*)
    let comp_r = ref comp in
    let next = ref [] in
    List.iter (function src -> 
      List.iter (function (rank,cmd) -> 
	check_sanity src cmd d_a.(rank);
	a_r.(rank) <- (src,cmd) :: a_r.(rank);
	if List.length (a_r.(rank))  = List.length (d_a.(rank)) then (
	  comp_r := !comp_r + 1 ;
	  next := rank :: !next
	)
      ) s_a.(src)
    ) node_l;
    if !next = [] then (
      if !comp_r = Array.length s_a then true
      else false
    ) else bfs !next (s_a,d_a) a_r !comp_r
      
  let f (s_a,d_a) ft = 
    let a_r = Array.map (fun _ -> []) s_a in
    let res = ref None in
    Array.iteri (fun i v -> 
      if v=[] then match !res with 
	| None -> res := Some i
	| Some _ -> 
	    eprintf "ft=\n%s\n" (FullTree.pretty_string_of_t ft);
	    failwith "Two tree roots were found "
    ) d_a;
    match !res with 
      | None -> failwith "No root the the distribution tree"
      | Some root -> 
	  (*eprintf "root=%d\n" root;*)
	  bfs [root] (s_a,d_a) a_r 1
      
end

let check_sched sched ft = CheckSched.f sched ft

(*************************************************************************)

(* In the case of 
 *  a
 * H H 
 *  b 
 * constructions, add the middle edge. 
*)
let rec add_middle_edges = function
  | Hol -> Hol
  | One x as t -> t
  | Diam (x,l,r,y,cot,()) -> 
      if l=Hol && r=Hol then Diam(x,l,r,y,true,()) 
      else Diam (x, add_middle_edges l, add_middle_edges r, y, cot,())

let check_sanity t_l = 
  let res = ref true in
  let l = List.map (function t -> members_of_t t) t_l in
  let _ = List.fold_left (fun accu l -> 
    List.iter (function elm -> 
      if List.mem elm accu then (
	eprintf "Components are not disjoint %s\n" 
	                (string_of_list string_of_t t_l);
	res := false
      )
    ) l;
    l@accu
  ) [] l in
  !res


  (* [merge t_l n] Merge a list of trees [t_l] assuming the list of members is
   * frmo 0 to n-1. 
   * 
   * There are two cases: 
   * 1. There is only one component, and it is one-connected. In this
   *    case, no secure channels should be used. 
   * 2. There are many components. Since secure channels must be used
   *    in this case anyway, we do not make a special effort to use
   *    a minimal number. 
   * 
   * This currently works well only 
   * for a single case. One large tree, and other singleton trees. 
   * The return value is 
   * 1. A list of instruction describing how to pass a new key 
   *    as fast as possible through the existing set of links. 
   * 2. A new diamond. Currently, one must pass secure messages
   *    through all its links, to rebuild it. This could be optimized
   *    to create only the missing links. 
   *)
let merge t_l nmembers  = 
  let merge_one_connected strict t1 = 
    let ft1 = FullTree.compute_instr false (FullTree.ft_of_t t1) in
    (*log (fun () -> sprintf "merge_one ft=%s \n%s\n" 
      (string_of_list string_of_t t_l)
      (FullTree.pretty_string_of_t ft1));*)
    (*eprintf "merge_one ft=\n%s\n" (FullTree.pretty_string_of_t ft1);*)
    let instr1 = FullTree.instructions_of_ft ft1 nmembers in
    assert (check_sched instr1 ft1);
    let t = strip (FullTree.t_of_ft ft1) in
    let t = two_connect t in
    let t = add_middle_edges t in
    let final_instr = FullTree.final_instr (FullTree.ft_of_t t) nmembers in
    (*log1 (fun () -> FullTree.string_of_instructions instr1);
      log1 (fun () -> sprintf "final_instr=%s" 
      (FullTree.string_of_instructions final_instr));*)
    instr1,t,final_instr
  in
  match t_l with 
  | [] -> failwith "Cannot merge an empty list"
  | [t] -> merge_one_connected true t 
  | t_l -> 
      assert (check_sanity t_l);
      let t_l = List.map (strip) t_l in
      let t1 = merge_one t_l in
      merge_one_connected false t1


(*************************************************************************)

(* Used to help the application keep its state.
*)
type appl_state  = 
  | T of t
  | Tinstr of t * t * instr 

(*************************************************************************)
