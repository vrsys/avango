(**************************************************************)
(* IOVECL.ML: operations on arrays of Iovec's. *)
(* Author: Mark Hayden, 5/96 *)
(* Rewritten by Ohad Rodeh 9/2001 *)
(**************************************************************)
open Trans
open Buf
open Util
(**************************************************************)
let name = "IOVECL"
let failwith s = failwith ("name:" ^ s)
let log = Trace.log name
(**************************************************************)
type t = Iovec.t Arrayf.t

let to_arrayf = ident
let of_iovec_arrayf = ident
let of_iovec_array il = Arrayf.of_array_break il
let to_iovec_array il = Arrayf.to_array_break il
let to_iovec_raw_array il = 
  Arrayf.to_array_map Iovec.raw_of il
let of_iovec_raw_array il = 
  Arrayf.of_array_map Iovec.of_raw il
(**************************************************************)

let free il =
  for i = pred (Arrayf.length il) downto 0 do 
    Iovec.free  (Arrayf.get il i) 
  done

let copy il = 
  for i = pred (Arrayf.length il) downto 0 do 
    ignore (Iovec.copy  (Arrayf.get il i))
  done; 
  il 
  
(**************************************************************)
(* Although the ref used below this causes allocation with
 * bytecode, I have verified that the native code compiler
 * eliminates the reference allocation.  
 *)

let len il = 
  let l = Pervasives.ref len0 in
  for i = 0 to pred (Arrayf.length il) do
    l := !l +|| Iovec.len (Arrayf.get il i)
  done ;
  !l

(**************************************************************)

let flatten il =  Iovec.flatten (Arrayf.to_array_break il)
let flatten_w_copy il = Iovec.flatten_w_copy (Arrayf.to_array_break il)

let concata iovll = Arrayf.flatten iovll

(**************************************************************)

let empty = Arrayf.empty

let is_empty il =
  Arrayf.is_empty il || len il = len0

let of_iovec iov = Arrayf.singleton iov

let append iovl1 iovl2 = Arrayf.append iovl1 iovl2
let prependi iov iovl = Arrayf.prependi iov iovl
let appendi iovl iov = Arrayf.append iovl (Arrayf.singleton iov)

(**************************************************************)

(* [loc] The location inside an iovecl. The [int] represents
 * the index number of an iovec. [ofs] is the accumulated 
 * offset from the beginning of the iovecl.
*)
type loc = int * ofs
let loc0 = (0,len0)

let rec sub_help iovl ((i,ofs) as loc) pos doing_hi =
  assert (pos >=|| ofs) ;
  assert (i < Arrayf.length iovl) ;
  let leni = Iovec.len (Arrayf.get iovl i) in
  let endi = ofs +|| leni in
  if pos < endi || (doing_hi && pos = endi) then 
    (loc,(pos -|| ofs))
  else 
    sub_help iovl ((succ i),(ofs+||leni)) pos doing_hi

let sub_scan loc iovl ofs len' =
  if len' =|| len0 then (
    empty,loc
  ) else (
    let total_len = len iovl in
    if ofs <|| len0 || len' <|| len0 || len' +|| ofs >|| total_len then (
      eprintf "IOVECL:sub:ofs=%d len=%d total_len=%d\n" 
	(int_of_len ofs) (int_of_len len') (int_of_len total_len);
      failwith "sub:ofs,len out of bounds" ;
    ) ;

    let ((lo_iov,_) as loc),lo_ofs = sub_help iovl loc ofs false in
    let ((hi_iov,_) as loc),hi_ofs = sub_help iovl loc (ofs +|| len') true in
(*
    eprintf "IOVECL:sub:ofs=%d len=%d lo_iov=%d lo_ofs=%d hi_iov=%d hi_ofs=%d pos=%d\n" 
      (int_of_len ofs) (int_of_len len') lo_iov (int_of_len lo_ofs) hi_iov (int_of_len hi_ofs) 
      (int_of_len pos) ;
*)
    let ret = 
      if lo_iov = hi_iov then (
	let iov = Iovec.sub (Arrayf.get iovl lo_iov) lo_ofs (hi_ofs -|| lo_ofs) in
	of_iovec iov
      ) else (
	assert (lo_iov < hi_iov) ;
	let last = hi_iov - lo_iov in
	let iovl = 
	  Arrayf.init (hi_iov - lo_iov + 1) (fun i ->
	    let iov = Arrayf.get iovl (lo_iov + i) in
	    if i = 0 then (
	      let leni = Iovec.len iov in
	      Iovec.sub iov lo_ofs (leni -|| lo_ofs)
	    ) else if i = last then (
	      Iovec.sub iov len0 hi_ofs
	    ) else (
	      Iovec.copy iov
	    )
	  )
	in
	iovl (*of_iovec_array iovl*)
      )
    in ret,loc
  )


let sub iovl ofs len = fst (sub_scan loc0 iovl ofs len)
(**************************************************************)

let fragment_iovec frag_len iov = 
  let len = Iovec.len iov in
  let fleni = int_of_len frag_len in
  let n = (int_of_len len + (pred fleni)) / fleni in
  let a = Array.create n Iovec.empty in
  for i = 0 to pred n do 
    let ofs = frag_len *|| i in
        let len =
	  if i = pred n then
	    len -|| ofs
	  else
	    frag_len
	in
	let iov_frag = Iovec.sub iov ofs len in
	a.(i) <- iov_frag
  done;
  Arrayf.of_array a

(* Fragment an iovecl.
 * This is a preliminary, inefficient version. 
 * A more efficient version will come later. 
 * 
 * It is inefficient in two ways:
 * 1. To many intermediate allocations. 
 * 2. It does not merge small iovecs into those of size [frag_len]. 
 *)
(*let fragment frag_len iovl =
  let iovl = Arrayf.to_list iovl in
  let iovl = List.map (fun iov -> 
    fragment_iovec frag_len iov 
  ) iovl in
  Arrayf.of_list iovl 
*)

(* If the iovecl is too short to fragment, an exception is raised.
 * 
 * This assumes that the array is initialized from 0 to (n-1).
 *)
let fragment frag_len iovl = 
  let ifrag_len = int_of_len frag_len in
  let total_len = len iovl in
  if (total_len <=|| frag_len) then 
    raise (Invalid_argument "The iovecl is too short to fragment")
  else (
    let nfrags = ((int_of_len total_len) + (pred ifrag_len)) / ifrag_len in
    let loc = ref loc0 in
    Arrayf.init nfrags (fun i ->
      let ofs = len_of_int (i * ifrag_len) in
      if i = pred nfrags then
	let t,_ = sub_scan !loc iovl ofs (total_len -|| ofs) in
	t
      else (
	let (t,loc') = sub_scan !loc iovl ofs frag_len in
	loc := loc';
	t
      )
    )
  ) 
    
(**************************************************************)

let sum_refs iovl = 
  let rep = Arrayf.map (fun iov -> Iovec.num_refs iov) iovl in
  let rep = Arrayf.to_array rep in
  Util.string_of_int_array rep

(**************************************************************)
let singleton iov = Arrayf.singleton iov

let is_singleton il =
  Arrayf.length il = 1

let get_singleton il =
  assert (Arrayf.length il = 1) ;
  Arrayf.get il 0

(**************************************************************)

exception Bad_message of string

(* The unmarshalling function frees the iovec according to the flag 
 * [free_iovl].
 * We can marshal directly into the iovec.
 * Thanks to Xavier (ocaml 3.04/3.05).
 * 
 *)
let make_marsh free_iovl = 
  let marsh obj = 
    let iov = Iovec.marshal obj [Marshal.Closures] in
    of_iovec iov

  and unmarsh iovl = 
    let iov = Iovec.flatten (to_iovec_array iovl) in
    let obj = Iovec.unmarshal iov in
    Iovec.free iov ;
    if free_iovl then free iovl;
    obj
  in
  (marsh,unmarsh) 
  
(**************************************************************)
  
let count_nonempty il =
  let nonempty = Pervasives.ref 0 in
  for i = 0 to pred (Arrayf.length il) do
    if Iovec.len (Arrayf.get il i) <>|| len0 then
      incr nonempty
  done ;
  !nonempty

(**************************************************************)

let _ = Trace.test_declare name (fun () ->
  let ma,um = make_marsh true in
  for i=0 to 10 do
    let obj = Array.init 20000 (fun _ -> (Random.int 100, "xyz", Random.int 100)) in
    let obj' = um (ma obj) in
    if obj <> obj' then
      failwith "failed IOVECL marshal test";
    eprintf "."
  done;
  eprintf "\nPassed Iovecl marshal test\n";
    
  (* Testing marsh,unmarsh from/to iovecls.
   *)
    
  printf "Running iovecl sanity test\n";
  let marsh,unmarsh = make_marsh true in
  
  let iovl = marsh (1,2,("A","B")) in
  let str = unmarsh iovl in
  begin match str with 
      x,y,(z,w) -> 
	printf "x=%d, y=%d z=%s w=%s\n" x y z w
  end;
  ()
)
  
(**************************************************************)
