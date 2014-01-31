(**************************************************************)
(* UTIL.ML *)
(* Author: Mark Hayden, 4/95 *)
(**************************************************************)
let failwith m = failwith ("UTIL:"^m)
(**************************************************************)

external (=|) : int -> int -> bool = "%eq"
external (<>|) : int -> int -> bool = "%noteq"
external (>=|) : int -> int -> bool = "%geint"
external (<=|) : int -> int -> bool = "%leint"
external (>|) : int -> int -> bool = "%gtint"
external (<|) : int -> int -> bool = "%ltint"
let int_max a b = if a >=| b then a else b
let int_min a b = if a <=| b then a else b

(**************************************************************)

let verbose = ref false
let quiet = ref false

(* The identity function.
 *)
let ident a = a

(* It had been defined as follows (both in util.ml and .mli):

     external ident : 'a -> 'a = "%identity"

 * but there seemed to be a problem because it would
 * often require allocation of a closure.
 *)

(*
let unit _ = ()
let unit2 _ _ = ()
*)
let ignore2 _ _ = ()

let info a b = a^":"^b

let sanity = "sanity"

let sanityn i = sanity^":"^(string_of_int i)

(**************************************************************)
(* Some weird stuff.
 *)
type ('a,'b,'c) fun2arg = 'a -> 'b -> 'c
type ('a,'b,'c,'d) fun3arg = 'a -> 'b -> 'c -> 'd

(*
external arity2 : ('a,'b,'c) fun2arg -> ('a,'b,'c) fun2arg = "%arity2"
external arity3 : ('a,'b,'c,'d) fun3arg -> ('a,'b,'c,'d) fun3arg = "%arity3"
*)
let arity2 = ident
let arity3 = ident
(**************************************************************)

let override = ref None

let fprintf_override f =
  override := Some f

let fprintf ch fmt =
  let f s = 
    match !override with
    | None ->
        output_string ch s ;
        flush ch
    | Some f -> f ch s
  in
  Printe.f f fmt

let sprintf fmt = Printe.f ident fmt
let printf  fmt = fprintf stdout fmt
let eprintf fmt = fprintf stderr fmt

(**************************************************************)

let if_some o f = 
  match o with
  | None -> ()
  | Some a -> f a

let some_of debug = function
  | None -> failwith (sprintf "some_of:%s" debug)
  | Some a -> a

let is_none = function
  | None -> true
  | Some _ -> false

let string_of_option sov = function
  | None -> "None"
  | Some v -> sprintf "Some(%s)" (sov v)

let option_map f = function
  | None -> None
  | Some o -> Some(f o)

let if_none o f =
  match o with
  | None -> f ()
  | Some _ -> o
      
let filter_nones l =
  List.fold_right (fun a b -> match a with
    | None -> b
    | Some a -> a :: b
  ) l []			    

let once debug l =
  match filter_nones l with
  | [a] -> a
  | [] -> failwith (sprintf "once:never:%s" debug)
  | _ -> failwith (sprintf "once:multiple Some's:%s" debug)

(**************************************************************)

(* Different from String.uppercase in that it
 * does not copy the string if it doesn't need
 * to.
 *)
let string_uppercase s =
  let upper = ref false in
  for i = 0 to pred (String.length s) do
    if s.[i] <> Char.uppercase s.[i] then
      upper := true
  done ;
  if !upper then 
    String.uppercase s
  else 
    s

(**************************************************************)

let int_of_hex c =
  let c = Char.lowercase c in
  match c with
  | '0'..'9' -> Char.code c - Char.code '0'
  | 'a'..'f' -> Char.code c - Char.code 'a' + 10
  | _ -> failwith "not in hex format"

let hex_to_string h = 
  let len = String.length h in
  assert ((len mod 2) = 0) ;
  let len = len / 2 in
  let n = String.create len in
  for i = 0 to (pred len) do
    let i1 = int_of_hex h.[2*i] in
    let i2 = int_of_hex h.[2*i+1] in
    let i3 = i1*16+i2 in
    n.[i] <- Char.chr i3
  done ;
  n

let hex_of_string s =
  let n = String.length s in
  let h = String.create (2 * n) in
  for i = 0 to pred n do
    let c = s.[i] in
    let c = Char.code c in
    let c = sprintf "%02X" c in
    String.blit c 0 h (2 * i) 2
  done ;
  h

(**************************************************************)

let counter () =
  let count = ref (-1) in
  fun () ->
    incr count ;
    !count

(**************************************************************)
(* SEQUENCE: [sequence n] evaluates to the array [0;1;...;n-1].
 *)

let sequence n = Array.init n ident

(**************************************************************)
(* TransisE addition
 *)

let array_exists f v = 
  let r = ref false in 
  for i = 0 to pred (Array.length v) do
    if f i (v.(i)) then
      r := true
  done ;
  !r
  
(**************************************************************)

let array_flatten a =
  let n = Array.length a in
  let len = ref 0 in
  let init = ref (-1) in
  for i = 0 to pred n do
    let l = Array.length a.(i) in
    if l > 0 then (
      len := !len + l ;
      init := i
    )
  done ;
  let len = !len in
  if len = 0 then [||] else (
    assert (!init >= 0) ;
    let b = Array.create len (a.(!init).(0)) in
    let j = ref 0 in
    for i = 0 to pred n do
      let l = Array.length a.(i) in
      Array.blit a.(i) 0 b !j l ;
      j := !j + l
    done ;
    assert (!j == len) ;
    b
  )

(**************************************************************)

let array_filter f a =
  let a = Array.copy a in
  let j = ref 0 in
  for i = 0 to pred (Array.length a) do
    let it = a.(i) in
    if f it then (
      if !j <> i then
	a.(!j) <- it ;
      incr j
    )
  done ;
  Array.sub a 0 !j

(**************************************************************)

let array_filter_nones a =
  let a = array_filter (function None -> false | _ -> true) a in
  Array.map (function Some i -> i | None -> failwith sanity) a

(**************************************************************)

let array_is_empty a =
  Array.length a = 0

(**************************************************************)

let array_incr v i = v.(i) <- succ v.(i)
let array_decr v i = v.(i) <- pred v.(i)
let array_add v i j = v.(i) <- v.(i) + j
let array_sub v i j = v.(i) <- v.(i) - j
let matrix_incr v i j = v.(i).(j) <- succ v.(i).(j)

(**************************************************************)

let array_index x a =
  let idx = ref (-1) in
  for i = 0 to pred (Array.length a) do
    if a.(i) = x then idx := i
  done ;
  if !idx < 0 then 
    raise Not_found ;
  !idx

(**************************************************************)

let hashtbl_size t =
  let n = ref 0 in
  Hashtbl.iter (fun _ _ -> incr n) t ;
  !n

(**************************************************************)

let hashtbl_to_list h =
  let l = ref [] in
  Hashtbl.iter (fun k d ->
    l := (k,d) :: !l
  ) h ;
  !l

(**************************************************************)

let do_once f =
  let cache = ref None in
  fun () ->
    match !cache with
    | None ->
      	let v = f () in
      	cache := Some v ;
	v
    | Some v -> v

(**************************************************************)

let string_map f s =
  let len = String.length s in
  let t = String.create len in
  for i = 0 to pred (String.length s) do
    t.[i] <- f s.[i]
  done;
  t

(**************************************************************)

let strchr c s =
  let n = String.length s in
  let rec loop i =
    if i >= n then 
      raise Not_found
    else if s.[i] = c then
      i
    else loop (succ i)
  in loop 0

(**************************************************************)

let array_mem it v =
  let ret = ref false in
  for i = 0 to pred (Array.length v) do
    if v.(i) = it then
      ret := true 
  done ;
  !ret

let array_for_all f a =
  let ret = ref true in
  for i = 0 to pred (Array.length a) do
    if not (f a.(i)) then
      ret := false
  done ;
  !ret

(**************************************************************)

let strstr c s =
  let n = String.length s in
  let rec loop i =
    if i >= n then 
      raise Not_found
    else if List.mem s.[i] c then
      i
    else loop (succ i)
  in loop 0

let strstrk c s k =
  let n = String.length s in
  let rec loop i =
    if i >= n then 
      raise Not_found
    else if array_mem s.[i] c then
      i
    else loop (succ i)
  in loop k

(* Return first char not in 'c'.
 *)
let strnstr c s =
  let n = String.length s in
  let rec loop i =
    if i >= n then 
      raise Not_found
    else if array_mem s.[i] c then
      loop (succ i)
    else i
  in loop 0

let chars_of_string s =
  let len = String.length s in
  if len = 0 then [||] else (
    let a = Array.create len s.[0] in
    for i = 0 to pred (String.length s) do
      a.(i) <- s.[i]
    done ;
    a
  )

(**************************************************************)

let string_split c s =
  let c = chars_of_string c in
  try 
    let lo = strnstr c s in
    let rec loop lo acc =
      try
	let hi = strstrk c s lo in
	let hd = String.sub s lo (hi-lo) in
	loop (succ hi) (hd::acc)
      with Not_found ->
	let l = 
	  if s = "" then 
	    acc
	  else
	    (String.sub s lo (String.length s - lo))::acc
	in List.rev l
    in loop lo []
  with Not_found -> []

(**************************************************************)

let bool_of_string s =
  let s = String.uppercase s in
  match s with
  | "T" | "TRUE" -> true
  | "F" | "FALSE" -> false
  | _ -> failwith "bool_of_string:bad argument"

(**************************************************************)

let string_of_unit ()      = "()"
let string_of_pair fa fb (a,b) = sprintf "(%s,%s)" (fa a) (fb b)
let string_of_list     f l = sprintf "[%s]" (String.concat "|" (List.map f l))
let string_of_array    f a = string_of_list f (Array.to_list a)
let string_of_int_array  a = string_of_array string_of_int a
let string_of_int_list   l = string_of_list string_of_int l
let string_of_bool         = function true -> "T" | false -> "f"
let string_of_bool_list  l = string_of_list string_of_bool l
let string_of_bool_array l = string_of_array string_of_bool l

(**************************************************************)
(*
let string_split c s =
  let l = string_split c s in
  eprintf "string_split:(%s,%s) -> %s\n"
    c s (string_of_list ident l) ;
  l
*)
(**************************************************************)

(* Use the marshalling facility to do a deep copy
 * of an object with no sharing of sub-structures.
 *)
let deepcopy o =
  let o = Marshal.to_string o [Marshal.No_sharing] in
  let o = Marshal.from_string o 0 in
  o

(**************************************************************)

let except el =
  let rec loop = function
    | [] -> []
    | hd::tl -> if el = hd then tl else hd::loop tl
  in loop
  
let index a =
  let rec loop i = function
    | []  -> raise Not_found
    | b::l -> if a = b then i else loop (succ i) l
  in loop 0
  
(**************************************************************)

let make_magic () = 
  ((Obj.repr,Obj.magic) : ('a -> Obj.t) * (Obj.t -> 'a))

(**************************************************************)
(* Support for keeping track of message sizes *)
(*
let average =
  let cntr = ref 0
  and sum  = ref 0
  and min_len  = ref 10000 in
  fun i ->
    incr cntr ;
    sum := !sum + i ;
    if i < !min_len then min_len := i ;
    if (!cntr mod 1000) = 0 then (
      eprintf "UTIL:accounting:#msgs=%d, total bytes=%d, min=%d avg=%.2f\n"
      	!cntr !sum !min_len (float !sum /. float !cntr)
    )
*)
(**************************************************************)

let disable_sigpipe =
  do_once (fun () ->
    try
      if !verbose then
      	eprintf "UTIL:warning:disabling SIGPIPE signals\n" ;

      (* SIGPIPE is not available on windows and I can't
       * catch the invalid_argument in native code.  
       *)
      if Sys.os_type <> "Win32" then (
      	ignore (Sys.signal Sys.sigpipe Sys.Signal_ignore)
      )
    with _ -> ()
  )

(**************************************************************)

let chars_of_string s =
  let l = ref [] in
  for i = 0 to pred (String.length s) do
    l := s.[i] :: !l
  done ;
  !l

let string_of_char_list =
  string_of_list Char.escaped

let str_verb = ref false

let strip_prefix s c =
  if !str_verb then
    eprintf "strip_prefix:%s:%s\n" 
      s (string_of_char_list c) ;
  let res =
    let rec loop i =
      if i >= String.length s then ""
      else if List.mem s.[i] c then loop (succ i)
      else String.sub s i ((String.length s) - i)
    in loop 0
  in
  if !str_verb then
    eprintf "strip_prefix:%s\n" res ;
  res

let strtok s c =
  let c = chars_of_string c in
  if !str_verb then
    eprintf "strtok:%s:%s\n" 
      s (string_of_char_list c) ;
  let s = strip_prefix s c in
  let res =
    let rec loop i =
      if String.length s = 0 then 
      	raise Not_found
      else if i >= String.length s then
      	(s,"")
      else if List.mem s.[i] c then
	let tok = String.sub s 0 i in
	let s = String.sub s i ((String.length s) - i) in
	let s = strip_prefix s c in
	(tok,s)
      else loop (succ i)
    in loop 0
  in
  if !str_verb then
    eprintf "strtok:out:%s:%s\n" (fst res) (snd res) ;
  res

(**************************************************************)

let string_of_id debug mapping key =
  let mapping = Array.to_list mapping in
  let mapping = List.map (fun (a,b) -> (b,a)) mapping in
  try
    let data = List.assoc key mapping in
    String.capitalize (String.lowercase data)
  with Not_found ->
    failwith (sprintf "%s:string_of_id:bad id" debug)

let id_of_string debug mapping key =
  let mapping = Array.to_list mapping in
  let tmp = string_uppercase key in
  try
    List.assoc tmp mapping
  with Not_found ->
    failwith (sprintf "%s:id_of_string:no such id:'%s'" debug key)
	
(**************************************************************)

let failmsg s t = s^t

let addinfo s t = 
  (*s^":"^*)t

(**************************************************************)

let string_list_of_gc_stat s =
  [sprintf "live_words=%dK"  (s.Gc.live_words/1024)  ; 
   sprintf "total_words=%dK" (s.Gc.heap_words/1024); 
   sprintf "heap_chunks=%d"  s.Gc.heap_chunks;
   sprintf "collections: minor=%d, major=%d, compact=%d"
      s.Gc.minor_collections s.Gc.major_collections s.Gc.compactions]

(*
  let alloc_promote_pct = (s.Gc.promoted_words * 100) / s.Gc.minor_words in
  let alloc_major_direct = s.Gc.major_words - s.Gc.promoted_words in
  let blocks_total = s.Gc.live_blocks + s.Gc.free_blocks in
  let blocks_live_pct = (s.Gc.live_blocks * 100) / blocks_total in
  let words_live_pct = (s.Gc.live_words * 100) / s.Gc.heap_words in
  [ sprintf "allocation: minor=%.1fM (%d%% promoted) (direct major=%dK)" 
    ((float s.Gc.minor_words) /. 1000000.0) alloc_promote_pct (alloc_major_direct/1000) ;
    sprintf "collections: minor=%d, major=%d, compact=%d" s.Gc.minor_collections s.Gc.major_collections s.Gc.compactions ;
    sprintf "words: %d (%d%% live) (%d chunks)" s.Gc.heap_words words_live_pct s.Gc.heap_chunks ;
    sprintf "blocks: %d (%d%% live) (largest_free=%d)" blocks_total blocks_live_pct s.Gc.largest_free
  ]	
*)
(**************************************************************)

let tag o =
  let o = Obj.repr o in
  if Obj.is_block o then 
    Obj.tag o
  else (-1)

(**************************************************************)

let string_check debug buf ofs len =
  if ofs < 0 || len < 0 || ofs + len > String.length buf then (
    eprintf "UTIL:string_check:%s: ofs=%d len=%d act_len=%d\n" debug ofs len (String.length buf) ;
    failwith ("string_check:"^debug)
  )

(**************************************************************)

let sample n a =
  let n = min n (Array.length a) in
  if n = 0 then [||] else (
    let len = pred (Array.length a) in
    Array.init (succ n) (fun i ->
      let j = (len * i) / n in
      a.(j)
    )
  )

(**************************************************************)
(* Stuff moved from Hsys. 
*)

(* Handler defaults to nothing.
 *)
let error_log = ref (fun _ -> ())

let log f = !error_log f


let full_error = ref (fun _ -> "Unknown Exception")

let install_error f = full_error := f

let error e =
  match e with
  | Out_of_memory  -> "Out_of_memory";
  | Stack_overflow -> "Stack_overflow";
  | Not_found      -> "Not_found"
  | Failure s      -> sprintf "Failure(%s)" s
  | Sys_error s    -> sprintf "Sys_error(%s)" s
  | Unix.Unix_error(Unix.EUNKNOWNERR i, s1, s2) -> 
      sprintf "Unix(%d,%s,%s)" i s1 s2 
  | Unix.Unix_error(err,s1,s2) ->
      let msg = 
	try Unix.error_message err 
      	with _ -> "Unix(unknown error)"
      in
      let s1 = if s1 = "" then "" else ","^s1 in
      let s2 = if s2 = "" then "" else ","^s2 in
      sprintf "Unix(%s%s%s)" msg s1 s2
  | Invalid_argument s -> 
      raise e
  | Assert_failure _ ->
      raise e
  | _ ->
      raise e
      (*!full_error e*)

let catch f a =
  try f a with e ->
    eprintf "gorp\n\n" ;
    eprintf "Uncaught exception: %s\n" (error e) ;
    exit 2

let set_error_log f     = error_log := f ; ()
(**************************************************************)

