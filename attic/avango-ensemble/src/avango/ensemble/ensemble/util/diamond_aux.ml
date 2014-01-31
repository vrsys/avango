(**************************************************************)
(* DIAMOND_AUX.ML : Additional functions for the diamond structure *)
(* Author: Ohad Rodeh, 6/2000 *)
(*************************************************************************)
open Util
open Trans
open Diamond

(*************************************************************************)

let test () = 
  let prog = ref "rand" in
  let n = ref 100 in
  let num_iter = ref 100 in
  let leave_num = ref 4 in
  
  Arg.parse [
    "-prog", Arg.String(fun s -> prog:=s), "which program to run? [split;rand;spec;spec1;loop]";
    "-verbose", Arg.Set(verbose), "Verbose debugging information";
    "-n", Arg.Int(fun i -> n:=i), "Size of tree";
    "-num_iter", Arg.Int(fun i -> num_iter:=i), "Number of iterations";
    "-leave", Arg.Int(fun i -> leave_num:=i), "Number of nodes to remove"
  ] (fun _ -> exit 0) "Testing the diamond structrue";


  begin match !prog with 
    | "split" -> 
	
	let t0 = create (Some 1) (singleton 2) (singleton 3) (Some 4) false in
	let t1 = create (Some 5) (singleton 6) (singleton 7) (Some 8) false in
	let t2 = create (Some 0) t0 t1 (Some 9) false in
	let t3 = create (Some 10) (singleton 11) (singleton 12) (Some 13) false in
	let t4 = create (Some 14) t2 t3 (Some 15) false in
	
	(*
	  let t5 = Three(1,2,3) in
	  printf "t0=\n%s\n" (string_of_t t5);
	  
	  printf "fathers t5 1=%s\n" (string_of_int_list (fathers t5 1));
	  printf "fathers t5 2=%s\n" (string_of_int_list (fathers t5 2));
	  printf "fathers t5 3=%s\n" (string_of_int_list (fathers t5 3));
	  
	  let t0 = Diam (1, One 2, One 3, 4) in
	  let t1 = Diam (5, One 6, One 7, 8) in
	  let t2 = Diam (0, t0, t1, 9) in
	  printf "t0=\n%s\n" (pretty_string_of_t t0);
	  printf "t1=\n%s\n" (pretty_string_of_t t1);
	  printf "t2=\n%s\n" (pretty_string_of_t t2);
	  
	  let t3 = Diam (10,One 11, One 12, 13) in
	  let t4 = Diam (14, t2, t3, 15) in
	  printf "t4=\n%s\n" (pretty_string_of_t t4);
	  
	  printf "fathers t2 0=%s\n" (string_of_int_list (fathers t2 0));
	  printf "fathers t2 1=%s\n" (string_of_int_list (fathers t2 1));
	  printf "fathers t2 2=%s\n" (string_of_int_list (fathers t2 2));
	  printf "fathers t2 4=%s\n" (string_of_int_list (fathers t2 4));
	  printf "fathers t2 9=%s\n" (string_of_int_list (fathers t2 9));
	  printf "children t2 0=%s\n" (string_of_int_list (children t2 0));
	  printf "children t2 1=%s\n" (string_of_int_list (children t2 1));
	  printf "children t2 2=%s\n" (string_of_int_list (children t2 2));
	  printf "children t2 4=%s\n" (string_of_int_list (children t2 4));
	  printf "children t2 9=%s\n" (string_of_int_list (children t2 9));
	*)
	
	(* Large examples, Testing split.
	 *)
	printf "t4=\n%s\n" (pretty_string_of_t t4);
	
	let t1 = split t4 1 [14;15] in
	printf "fully_connected t2=%b\n" (fully_connected t1);
	printf "t2=\n%s\n" (pretty_string_of_t t1);
	
	let t1 = split t4 1 [9;13;14;15] in
	printf "fully_connected t2=%b\n" (fully_connected t1);
	printf "t2=\n%s\n" (pretty_string_of_t t1);
	
	let t1 = split t4 1 [2;6;9;13;14;15] in
	printf "fully_connected t2=%b\n" (fully_connected t1);
	printf "t2=\n%s\n" (pretty_string_of_t t1);
	
	let t1 = split t4 1 [3;2;6;9;13;14;15] in
	printf "fully_connected t2=%b\n" (fully_connected t1);
	printf "t2=\n%s\n" (pretty_string_of_t t1);
	
  (* Testing the simple cases with trees of size 4. 
   *)
	(*
	printf "t0=\n%s\n" (string_of_t t0);
	let t,_ = merge1 [t0] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t);
	
  (* Single failures
   *)
	let t1 = split t0 1 [4] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);
	
	let t1 = split t0 1 [2] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);
	
	let t1 = split t0 1 [3] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);
	
	let t1 = split t0 4 [1] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);

    (*  Multiple failures.
     *)
	let t1 = split t0 1 [2;4] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);
	
	
	let t1 = split t0 1 [3;4] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);
	
	let t1 = split t0 4 [1;2] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);
	
	
	let t1 = split t0 4 [1;2;3] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);

	let t1 = split t2 1 [] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);
	
	
	let t1 = split t2 1 [9] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);
	
	
	let t1 = split t2 1 [8] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);
	
	let t1 = split t2 1 [8;7] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);

	let t1 = split t2 1 [0;6] in
	printf "t0=\n%s\n" (string_of_t t1);
	let t1,_ = merge1 [t1] in
	printf "merge1 t0=\n%s\n" (FullTree.pretty_string_of_t t1);
	
	let t1 = Diam (Some 5, One 6, One 7, Some 8) in
	let t1,_ = merge1 [t1;One 0; One 6] in
	printf "t1=\n%s\n" (FullTree.pretty_string_of_t t1);
*)
	
    | "loop" -> 
	eprintf "Loop test\n" ;

	for i=4 to 100 do
	  let a = Array.init i (function j -> singleton j) in
	  let _,t,_ = merge (Array.to_list a) i in
	  if not (fully_connected t) then failwith "Tree is not connected";
	  
	  eprintf "i=%d:" i;
	  for k=0 to i-2 do 
	    eprintf "k=%d " k;
	    let t1 = split t (i-1) [k] in
	    if not (fully_connected t1) then failwith "Tree is not connected";
	    let t2 = singleton k in
	    let _,t3,_ = merge [t1;t2] i in
	    if not (fully_connected t3) then failwith "Tree is not connected";
	    ()
	  done;
	  eprintf "\n";
	done

    | "rand" -> 
	eprintf "Random test\n";


	(* Take a tree with an arbitrary set of members k1,k2,k3.... 
	 * Condense them to 1,2,3,..
	 *)
	let condense t = 
	  let a = Array.of_list (members_of_t t) in
	  let len = Array.length a in
	  let h = Hashtbl.create (Array.length a) in
	  Array.iteri (fun i v ->  
	    Hashtbl.add h v i
	  ) a;
	  let t' = map (fun rank -> Hashtbl.find h rank) t in

	  let l = members_of_t t' in
	  List.iter (fun i -> 
	    if i<0 || i>=len then failwith "condense:Did not manage to condense";
	  ) l;
	  t'
	in
	  
	(* Take a list of trees and spread their members, so there would be
	 * no intersections. 
	 *)
	let spray_members l= 
	  let (l,_) = List.fold_left (fun (accu,max) t -> 
	    let t = map (fun i -> i+max) t in
	    t::accu,max+(List.length (members_of_t t))
	  ) ([],0) l in 
	  l
	in
	       
	let subleader t = 
	  let l = members_of_t t in 
	  match l with 
	    | [] -> failwith "subleader: Diamond is empty"
	    | hd::_ -> hd 
	in
	
	let create_tree n = 
	  let a = Array.init n (function j -> singleton j) in
	  let _,t,_ = merge (Array.to_list a) n in
	  if not (fully_connected t) then failwith "Tree is not connected";
	  t
	in

	let choose_leave_set () = 
	  assert (!n > !leave_num) ;
	  let a = Array.init !leave_num (fun _ -> Random.int !n) in
	  Array.to_list a 
	in
	
(*	
	let t0 = create (Some 2) (singleton 3) (singleton 4) None false in
	let t1 = create (Some 5) empty empty None false in
	let t2 = create (Some 1) t0 t1 None false in
	
	let t3 = create (Some 7) (singleton 9) (singleton 10) (Some 8) false in
	let t4 = create (Some 11) (singleton 12) (singleton 13) None false in
	let t5 = create (Some 6)  t3 t4 None false in
	
	let t6 = create (Some 0) t2 t5 None false in

	printf "t=\n%s\n" (pretty_string_of_t t6);
	let t = condense t6 in
	let nmembers = List.length (members_of_t t) in
	let _,t,_ = merge [t] nmembers in
	()
*)



	let base_tree = create_tree !n in
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
		if nmembers = 1 then Some (singleton 0)
		else (
		  let t = condense t in
		  (*if !verbose then (
		    eprintf "- merging sub-component\n";
		    eprintf "pre=\n%s\n" (pretty_string_of_t t)
		  );*)
		  let _,t,_ = merge [t] nmembers in
		  if !verbose then 
		    eprintf "comp = \n%s\n" (pretty_string_of_t t);
		  Some t 
		)
	      ) else None
	    ) a in
	    let l = Array.to_list a in
	    let l = filter_nones l in
	    let l = spray_members l in
	    let _,t',_ = merge l !n in
	    if List.length (members_of_t t') <> !n then 
	      failwith (sprintf "does not contain %d members" !n);

	    loop t' (counter-1)
	  )
	in
	loop base_tree !num_iter

    | "spec" -> 
	let i=8 in
	let a = Array.init i (function j -> singleton j) in
	let _,t,_ = merge (Array.to_list a) i in
	if not (fully_connected t) then failwith "Tree is not connected";
	
	eprintf "i=%d:" i;
	let k=3 in
	eprintf "k=%d " k;
	printf "t=\n%s\n" (pretty_string_of_t t);
	let t1 = split t (i-1) [k] in
	if not (fully_connected t1) then failwith "Tree is not connected";
	printf "t1=\n%s\n" (pretty_string_of_t t1);
	let t2 = singleton k in
	let _,t3,_ = merge [t1;t2] i in
	if not (fully_connected t3) then failwith "Tree is not connected";
	()

    (*
       For this structure, two_connect fails. 

 N                                                          
H      N                                                    
     N  H                                                   
   N  H                                                     
  H 4                                                       
   3                                                        
     2                                                      
       1                                                    
 0                                                          
     
   N                                                        
 N  H                                                       
H H                                                         
 2                                                          
   1                                                        
     
*)
    | "spec1" -> 
	eprintf "Spec1\n";

	let t0 = create None empty (singleton 4) (Some 3) false in
	let t1 = create None t0 empty  (Some 2) false in
	let t2 = create None t1 empty  (Some 1) false in
	let t3 = create None empty t2 (Some 0) false in

	let t6 = t3 in
	printf "t6=\n%s\n" (pretty_string_of_t t6);
	let t7 = two_connect t6 in

	printf "final t=\n%s\n" (pretty_string_of_t t7)


    | _ -> failwith "Unknown test"
  end;
  exit 0;
  ()
(*
let _ = test ()
*)
(*************************************************************************)
    
    
    
