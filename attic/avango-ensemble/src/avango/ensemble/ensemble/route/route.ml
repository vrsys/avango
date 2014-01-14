(**************************************************************)
(* ROUTE.ML *)
(* Author: Mark Hayden, 12/95 *)
(**************************************************************)
open Trans
open Buf
open Util
(**************************************************************)
let name = Trace.file "ROUTE"
let failwith = Trace.make_failwith name
(**************************************************************)

let drop = Trace.log "ROUTED"
let info = Trace.log "ROUTEI"

(**************************************************************)

type pre_processor =
  | Unsigned of (rank -> Obj.t option -> seqno -> Iovecl.t -> unit)
  | Signed of (bool -> rank -> Obj.t option -> seqno -> Iovecl.t -> unit)

(**************************************************************)

type id =
  | UnsignedId
  | SignedId

(**************************************************************)
(* [merge] is the type of a combined receive function that is
 * called once on behalf of a set of stacks that all need to
 * receive a packet. 
 * 
 * [data] is the actual array of stack receive functions.
 * 
 * [key] contains an extra integer to improve the hash function.
*)
type key = Conn.id * Conn.key * int
type merge = Buf.t -> Buf.ofs -> Buf.len -> Iovecl.t -> unit
type data = Conn.id * Buf.t * Security.key * pre_processor *
     ((Conn.id * Buf.t * Security.key * pre_processor) Arrayf.t ->
       merge Arrayf.t)

type handlers = (key,data,merge) Handler.t

(* The type of a packet sending function. It so happens that
 * the type of sending and receiving functions is nearly same. 
*)
type xmitf = Buf.t -> Buf.ofs -> Buf.len -> Iovecl.t -> unit

(* Type of routers.
 *)
type 'xf t = {
  debug		: debug ;
  secure	: bool ;
  proc_hdlr     : ((bool -> 'xf) -> pre_processor) ;
  pack_of_conn  : (Conn.id -> Buf.t) ;
  merge         : ((Conn.id * digest * Security.key * pre_processor) Arrayf.t -> 
                   merge Arrayf.t) ;
  blast         : (xmitf -> Security.key -> digest -> Conn.id -> 'xf)
} 

(**************************************************************)

let id_of_pre_processor = function 
  | Unsigned _  -> UnsignedId
  | Signed _ -> SignedId

(**************************************************************)

let create debug secure proc_hdlr pack_of_conn merge blast = {
  debug = debug ;
  secure = secure ;
  proc_hdlr = proc_hdlr ;
  pack_of_conn = pack_of_conn ;
  merge = merge ;
  blast = blast
}

let debug r = r.debug
let secure r = r.secure

let hash_help p =
  Hashtbl.hash_param 100 1000 p

let install r handlers ck all_recv key handler =
  List.iter (fun (conn,kind) ->
    let pack = r.pack_of_conn conn in
    let pack0 = Buf.int16_of_substring pack (Buf.length pack -|| len4) in
    info (fun () -> sprintf "Install: pack0=%d conn=%s" 
      pack0 (Conn.string_of_id conn)) ;

    (* We put in an extra integer to make the hashing less
     * likely to result in collisions.  Note that this
     * integer has to go last in order to be effective
     * (see ocaml/byterun/hash.c).  Yuck.  
     *)
    let hash_help = hash_help (conn,ck) in
    let hdlr = r.proc_hdlr (handler kind) in
    Handler.add handlers pack0 (conn,ck,hash_help) (conn,pack,key,hdlr,r.merge)
  ) all_recv
(*; eprintf "ltimes=%s\n" (string_of_int_list (Sort.list (>=) ((List.map (fun ((c,_,_),_) -> Conn.ltime c) (Handler.to_list handlers)))))*)

let remove r handlers ck all_recv =
  List.iter (fun (conn,_) ->
    let pack = r.pack_of_conn conn in
    let pack0 = Buf.int16_of_substring pack (Buf.length pack -|| len4) in
    let hash_help = hash_help (conn,ck) in
    Handler.remove handlers pack0 (conn,ck,hash_help)
  ) all_recv

let blast r xmits key conn =
  let pack = r.pack_of_conn conn in
(*  info (fun () -> sprintf "blast pack=%s conn=%s" 
    (Util.hex_of_string (Buf.string_of pack))
    (Conn.string_of_id conn)) ;*)
  r.blast xmits key pack conn

(**************************************************************)
(* Security stuff.
 *)

let secure_ref = ref false
let insecure_warned = ref false

let set_secure () =
  eprintf "ROUTE:security enabled\n" ;
  secure_ref := true

let security_check router =
  (* Check for security problems.
   *)
  if !secure_ref && not router.secure then (
    eprintf "ROUTE:enabling transport with insecure router (%s)\n" router.debug ;
    eprintf "  -secure flag is set, exiting\n" ;
    exit 1
  )
(* ;
  if router.secure & (not !secure) & (not !insecure_warned) then (
    insecure_warned := true ;
    eprintf "ROUTE:warning:enabling secure transport but -secure flag not set\n" ;
    eprintf "  (an insecure transport may be enabled)\n"
  )
*)

(**************************************************************)
(* This function takes an array of pairs and returns an
 * array of pairs, where the fst's are unique, and snd'd are
 * grouped as the snd's of the return value.  [group
 * [|(1,2);(3,4);(1,5)|]] evaluates to
 * [|(1,[|2;5|]);(3,[|4|])|], where (1,2) and (1,5) are
 * merged into a single entry in the return value.  
 *)

let group_cmp i j = compare (fst i) (fst j)

let group a =
  let len = Arrayf.length a in
  match len with
    (* Handle simple cases quickly.
     *)
  | 0 -> Arrayf.empty
  | 1 -> 
      let a = Arrayf.get a 0 in
      Arrayf.singleton ((fst a),(Arrayf.singleton (snd a)))

  | _ -> 
      (* General case.
       *)

      (* Sort by the fst of the pairs in the array.
       *)
      let a = Arrayf.sort group_cmp a in

      (* Initialize result state.
       *)
      let key = ref (fst (Arrayf.get a 0)) in
      let b = Arraye.create len (!key,Arrayf.empty) in
      let lasta = ref 0 in
      let lastb = ref 0 in

      (* Collect items with equivalent fsts into
       * lists.
       *)
      for i = 1 to len do
	if i = len || fst (Arrayf.get a i) <> !key then (
	  let sub = Arrayf.sub a !lasta (i - !lasta) in
	  let sub = Arrayf.map snd sub in
	  Arraye.set b !lastb (!key,sub) ;
	  incr lastb ;
	  lasta := i ;
	  if i < len then (
	    key := fst (Arrayf.get a i)
	  )
	) ;
      done ;

      (* Clean up the result state.
       *)
      let b = Arraye.sub b 0 !lastb in
      let b = Arrayf.of_arraye b in
      b

(**************************************************************)

(* Test code for 'group'
*)
let _ = Trace.test_declare "group" (fun () ->
  let group_simple a =
    let len = Arrayf.length a in
    if len = 0 then Arrayf.empty else (
      (* Sort by the fst of the pairs in the array.
       *)
      let a = Arrayf.sort group_cmp a in

      (* Initialize result state.
       *)
      let k0,d0 = Arrayf.get a 0 in
      let k = Arraye.create len k0 in
      let d = Arraye.create len [] in
      let j = ref 0 in
      Arraye.set d 0 [d0] ;

      (* Collect items with equivalent fsts into
       * lists.
       *)
      for i = 1 to pred len do
	if fst (Arrayf.get a i) <> Arraye.get k !j then (
	  incr j ;
	  assert (!j < len) ;
	  Arraye.set k !j (fst (Arrayf.get a i))
	) ;
	Arraye.set d !j (snd (Arrayf.get a i) :: (Arraye.get d !j))
      done ;

      (* Clean up the result state.
       *)
      incr j ;
      let d = Arraye.sub d 0 !j in
      let k = Arraye.sub k 0 !j in
      let d = Arraye.map Arrayf.of_list d in
      let d = Arrayf.of_arraye d in
      let k = Arrayf.of_arraye k in
      Arrayf.combine k d
    )
  in
  
  let expand a =
    let a = Arrayf.map (fun (i,b) ->
      Arrayf.map (fun j -> (i,j)) b
    ) a in
    Arrayf.flatten a
  in

  for c = 1 to 100000 do
    if (c mod 1000) = 0 then
      eprintf "test:%d\n" c ;
    let a = Arrayf.init (Random.int 50) (fun _ ->
      (Random.int 5, Random.int 15))
    in
    let b = group_simple a in
    let c = group a in

    let a = Arrayf.sort compare a in
    let b = Arrayf.map (fun (i,a) -> (i,(Arrayf.sort compare a))) b in
    let c = Arrayf.map (fun (i,a) -> (i,(Arrayf.sort compare a))) c in
    let d = Arrayf.sort compare (expand c) in

    if b <> c || a <> d then (
      eprintf "inp=%s\n" ((Arrayf.to_string (string_of_pair string_of_int string_of_int)) a) ;
      eprintf "old=%s\n" ((Arrayf.to_string (string_of_pair string_of_int Arrayf.int_to_string)) b) ;
      eprintf "new=%s\n" ((Arrayf.to_string (string_of_pair string_of_int Arrayf.int_to_string)) c) ;
      assert (b = c) ;
    ) ;
    
  done
)

(**************************************************************)
let no_handler () = "no upcalls for message"
let empty1 _ _ = drop no_handler
let empty2 _ _ = drop no_handler
let empty3 _ _ _ = drop no_handler
let empty4 _ _ _ _ = drop no_handler

(**************************************************************)
(* The new support stuff.
 *
 * The point is to increment the ref-count for each xmit call 
 * on an iovec.
 *)

let merge1 a = ident (fun a1 -> Arrayf.iter (fun f -> f a1) a)

let merge2 a =
  match (Arrayf.to_array a) with
  | [||] -> empty2
  | [|f1|] -> f1
  | [|f1;f2|] ->
      fun a1 a2 ->
	f1 a1 a2 ; f2 a1 a2
  | _ ->
      let n = pred (Arrayf.length a) in
      fun a1 a2 ->
	for i = 0 to n do
	  (Arrayf.get a i) a1 a2
	done

let merge3 a = 
  match (Arrayf.to_array a) with
  | [||] -> empty3
  | [|f1|] -> f1
  | [|f1;f2|] ->
      fun a1 a2 a3 ->
	f1 a1 a2 a3 ; f2 a1 a2 a3
  | _ ->
      let n = pred (Arrayf.length a) in
      fun a1 a2 a3 ->
	for i = 0 to n do
	  (Arrayf.get a i) a1 a2 a3
	done

let merge4 a = ident (fun a1 a2 a3 a4 -> Arrayf.iter (fun f -> f a1 a2 a3 a4) a)

(**************************************************************)

let empty3rc r o l = drop no_handler ; Iovecl.free r
let merge3rc a = 
  match Arrayf.to_array a with
  | [||] -> empty3rc
  | [|f1|] -> f1
  | [|f1;f2|] ->
      fun r o l -> 
	f1 (Iovecl.copy r) o l ; f2 r o l
  | _ ->
      let n = pred (Arrayf.length a) in
      fun r o l ->
	for i = 1 to n do
	  (Arrayf.get a i) (Iovecl.copy r) o l
	done ;
	(Arrayf.get a 0) r o l

(**************************************************************)
(* Handle the reference counting.  The idea here is that in
 * the code we allocate an iovec array with one reference.
 * Here we want to increment that reference count if there
 * is more than 1 destination and decrement it if there are
 * none.  Note that we do the operations that increment the
 * reference counts first so that the count does not get
 * zeroed on us.
 *)

let empty1iov argv = drop no_handler ; Iovecl.free argv
let merge1iov a = 
  match Arrayf.to_array a with
  | [||] -> empty1iov
  | [|f1|] -> f1
  | [|f1;f2|] ->
      fun argv -> 
	f1 (Iovecl.copy argv) ; f2 argv
  | _ ->
      let pred_len = pred (Arrayf.length a) in
      fun argv ->
	for i = 1 to pred_len do
	  (Arrayf.get a i) (Iovecl.copy argv)
	done ;
	(Arrayf.get a 0) argv

let empty2iov arg1 argv = drop no_handler ; Iovecl.free argv
let merge2iov a = 
  match Arrayf.to_array a with
  | [||] -> empty2iov
  | [|f1|] -> f1
  | [|f1;f2|] ->
      fun arg1 argv -> 
	f1 arg1 (Iovecl.copy argv) ; f2 arg1 argv
  | _ ->
      let pred_len = pred (Arrayf.length a) in
      fun arg1 argv ->
	for i = 1 to pred_len do
	  (Arrayf.get a i) arg1 (Iovecl.copy argv)
	done ;
	(Arrayf.get a 0) arg1 argv

let empty2iovr argv arg2 = drop no_handler ; Iovecl.free argv
let merge2iovr a = 
  match Arrayf.to_array a with
  | [||] -> empty2iovr
  | [|f1|] -> f1
  | [|f1;f2|] ->
      fun argv arg2 -> 
	f1 (Iovecl.copy argv) arg2 ; f2 argv arg2
  | _ ->
      let pred_len = pred (Arrayf.length a) in
      fun argv arg2 ->
	for i = 1 to pred_len do
	  (Arrayf.get a i) (Iovecl.copy argv) arg2
	done ;
	(Arrayf.get a 0) argv arg2

let empty3iov arg1 arg2 argv = drop no_handler ; Iovecl.free argv
let merge3iov a = 
  match Arrayf.to_array a with
  | [||] -> empty3iov
  | [|f1|] -> f1
  | [|f1;f2|] ->
      fun arg1 arg2 argv ->
	f1 arg1 arg2 (Iovecl.copy argv) ; f2 arg1 arg2 argv
  | _ ->
      let pred_len = pred (Arrayf.length a) in
      fun arg1 arg2 argv ->
	for i = 1 to pred_len do
	  (Arrayf.get a i) arg1 arg2 (Iovecl.copy argv)
	done ;
	(Arrayf.get a 0) arg1 arg2 argv

let empty4iov arg1 arg2 arg3 argv = drop no_handler ; Iovecl.free argv
let merge4iov a = 
  match Arrayf.to_array a with
  | [||] -> empty4iov
  | [|f1|] -> f1
  | [|f1;f2|] -> fun arg1 arg2 arg3 argv ->
      f1 arg1 arg2 arg3 (Iovecl.copy argv) ; f2 arg1 arg2 arg3 argv
  | _ ->
      let pred_len = pred (Arrayf.length a) in
      fun arg1 arg2 arg3 argv ->
	for i = 1 to pred_len do
	  (Arrayf.get a i) arg1 arg2 arg3 (Iovecl.copy argv)
	done ;
	(Arrayf.get a 0) arg1 arg2 arg3 argv

let pack_of_conn = Conn.hash_of_id

(**************************************************************)
(*type merge = (Conn.id * Security.key * Conn.kind * rank * message) array 
  -> (Iovec.t -> unit)*)

let merge info =
  let info = Arrayf.map (fun (c,p,k,h,m) -> 
    ((id_of_pre_processor h),(m,(c,p,k,h)))) info in
  let info = group info in
  let info = 
    Arrayf.map (fun (_,i) -> 
      assert (Arrayf.length i > 0) ; 
      (fst (Arrayf.get i 0)) (Arrayf.map snd i)
    ) info 
  in
  let info = Arrayf.flatten info in
  merge4iov info

let delay f =
  let delayed a b c d =
    (f ()) a b c d
  in
  delayed


let handlers () =
  let table = Handler.create merge delay in
(*
  Trace.install_root (fun () -> [
    sprintf "ROUTE:#enabled=%d" (Handler.size table) ;
    sprintf "ROUTE:table:%s" (Handler.info table)
  ]) ;
*)
  table

let deliver handlers buf ofs len iovl = 
  (* Buf.int16_of_substring pack (Buf.length pack -|| len4) in*)
  (*raise (Invalid_argument "deliver");*)

  let pack0 = int16_of_substring buf (ofs +|| md5len -|| len4) in
  if Iovecl.len iovl >|| len0 then 
    info (fun () -> sprintf "deliver pack0=%d pack=%s (mo=%d iovl=%d)"
      pack0
      (Util.hex_of_string (Buf.string_of (Buf.sub buf ofs len16)))
      (int_of_len len) (int_of_len (Iovecl.len iovl))
    );

  let handler = Handler.find handlers pack0 in
  handler buf ofs len iovl

(**************************************************************)
