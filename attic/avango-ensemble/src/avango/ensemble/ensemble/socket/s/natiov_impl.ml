(**************************************************************)
(* NATIOV.ML : Memory management functions. *)
(* Author: Ohad Rodeh 10/2002 *)
(**************************************************************)
open Printf
open Socksupp
(**************************************************************)
external (=|) : int -> int -> bool = "%eq"
external (<>|) : int -> int -> bool = "%noteq"
external (>=|) : int -> int -> bool = "%geint"
external (<=|) : int -> int -> bool = "%leint"
external (>|) : int -> int -> bool = "%gtint"
external (<|) : int -> int -> bool = "%ltint"
(**************************************************************)

let log f =  ()
(*  let s = f () in
  print_string "NATIOV:"; print_string s; print_string "\n"; flush stdout
*)

(* It is not clear whether this is really neccessary 
 * with the newer versions of OCAML.
 * 
 * PERF
 *)
external (=||) : int -> int -> bool = "%eq"
external (<>||) : int -> int -> bool = "%noteq"
external (>=||) : int -> int -> bool = "%geint"
external (<=||) : int -> int -> bool = "%leint"
external (>||) : int -> int -> bool = "%gtint"
external (<||) : int -> int -> bool = "%ltint"
external (+||) : int -> int -> int = "%addint"
external (-||) : int -> int -> int = "%subint"
external ( *||) : int -> int -> int = "%mulint"
  
(* The type of a C memory-buffer. It is opaque.
 *)
type cbuf  
type buf = string
type ofs = int
type len = int
    
type raw = {
  len : int ;
  cbuf : cbuf ;
}
    
type base = {
  mutable count : int; (* A reference count *)
  base_iov : raw   (* Free this cbuf when the refcount reaches zero *)
}
    
type t = { 
  base : base ;  
  iov : raw
}
    
(* Instead of using the raw malloc function, we take
 * memory chunks of size CHUNK from the user. We then
 * try to allocated from them any message that can fit. 
 * 1. If the request length fits into what remains of the current 
 *    chunk, it is allocated there.
 * 2. If the message is larger than CHUNK, then we allocate 
 *    directly using mm_alloc.
 * 3. Otherwise, allocate a new chunk and allocated from it.
 * 
 * Note that we waste the very last portion of the iovec. 
 * however, we assume that allocation is performed in slabs
 * of no more than 8K (max_msg_size), and so waste is less
 * than 10%.
 *)
    
type alloc_state = {
  chunk_size : int ;
  mutable chunk : t ; (* The current chunk  *)
  mutable pos : int ;
}
    
module Opt = struct
  external copy_raw_into_string : raw -> string -> ofs -> unit
    = "mm_copy_raw_into_string" "noalloc"
  
  external copy_raw_into_raw : raw -> raw -> unit
    = "mm_copy_raw_into_raw" "noalloc"
  
  external copy_string_into_raw : string -> ofs -> len -> raw -> unit
    = "mm_copy_string_into_raw" "noalloc"
  
  external free_cbuf : cbuf -> unit 
    = "mm_cbuf_free" "noalloc"
  
  external mm_cbuf_sub : raw -> ofs -> cbuf
    = "mm_cbuf_sub" 
  
  let num_refs t = t.base.count

  let mm_sub iov ofs len = 
    assert (iov.len >=|| ofs +|| len);
    let cbuf = mm_cbuf_sub iov ofs in
    {len=len; cbuf=cbuf}
    
  (* Flatten an array of iovectors. The destination iovec is
   * preallocated.
   *)
  external mm_flatten : raw array -> raw -> unit
    = "mm_flatten" "noalloc"
      
  (* Raw (C) allocation function. Can be user defined.
   *)
  external mm_alloc : int -> raw
    = "mm_alloc"
      
  (* Create an empty iovec. We do this on the C side, so
   * that it will be usable by C functions that may return
   * empty Iovec's.
   *)
  external mm_empty : unit -> raw
    = "mm_empty"
      
  let len t = t.iov.len
    
  (* These conversions are needed for CE, or in general, for
   * outside C interfaces. 
   *)
  let t_of_raw iovec = {
    base = {count=1; base_iov=iovec};
    iov = iovec 
  } 
  let raw_of_t t = 
    assert (num_refs t >| 0);
    t.iov
    
  let t_of_iovec base iovec = {base = base; iov = iovec} 
    
  (* Decrement the refount, and free the cbuf if the count
   * reaches zero. 
   *)
  let free t =
    if t.base.base_iov.len >|| 0 then (
      if t.base.count <=|| 0 then (
	printf "Error, refcount already zero. len=%d\n" t.base.base_iov.len;
	flush stdout;
	raise (Invalid_argument "bad refcounting");
      );
      (*log (fun () -> sprintf "free: len=%d" t.iov.len);*)
      t.base.count <- pred t.base.count;
      if t.base.count =|| 0 then (
	log (fun () -> sprintf "freeing a buffer (len=%d)" t.base.base_iov.len);
	free_cbuf t.base.base_iov.cbuf;
      )
    )
      
  (* Current size is 256K. Must be larger than max_msg_size.
   *)
  let size = 
    let len = 32 * max_msg_size in
    assert(len >| max_msg_size);
    len
    
  let s = 
    let raw = mm_alloc size in
    {
      chunk_size = size;
      chunk = {
	base=  {count=1; base_iov = raw};
	iov = raw
      };
      pos = 0 ;
    }
    
  let free_old_chunk () = 
    s.chunk.base.count <- pred s.chunk.base.count ;
    (*log (fun () -> sprintf "free_old_chunk, num_refs=%d" s.chunk.base.count);*)
    if s.chunk.base.count =|| 0 then (
      log (fun () -> "actually free old-chunk");
      free_cbuf s.chunk.base.base_iov.cbuf;
    )
      
  let alloc len = 
    if s.chunk_size -|| s.pos >=|| len then (
      (*log (fun () -> sprintf "simple_alloc %d" len);*)
      let iov = mm_sub s.chunk.base.base_iov s.pos len in
      s.chunk.base.count <- succ s.chunk.base.count;
      s.pos <- s.pos +|| len;
      t_of_iovec s.chunk.base iov
    ) else 
      (* requested size too large, allocating a fresh (C) region.
       *)
      if len >=|| s.chunk_size then (
	log (fun () -> sprintf "large_alloc %d\n" len);
	let large_iov = mm_alloc len in
	t_of_iovec {count=1;base_iov=large_iov} large_iov
      ) else (
	(* Here we waste the end of the cached iovec.
	 * 
	 * We do 
	 * 1) Keep a refcount of 1 for the iovec we give out,
	 * 2) Another 1 that we free only after we've used
	 *    up the whole chunk size.
	 * 3) Decrement the refcount of the old chunk.
	 *)
	free_old_chunk ();
	let new_chunk = mm_alloc s.chunk_size in
	let iov = mm_sub new_chunk 0 len in
	s.chunk <- {base={count=2;base_iov=new_chunk}; iov=new_chunk};
	s.pos <- len;
	t_of_iovec s.chunk.base iov
      )
	
  (* check that there is enough space in the cached iovec before
   * a receive operation. 
   * 
   * This has two effects:
   * 1) we waste the end of the current 2.
   * iovecg) we save an exception being thrown from inside the
   *    the receive, because there will always be enough space.
   * 
   * Return a cbuf from which to start allocating.
   *)
  let check_pre_alloc () = 
    if s.chunk_size -|| s.pos <|| max_msg_size then (
      (* Here we waste the end of the cached iovec.
       *)
      free_old_chunk ();
      
      let raw = mm_alloc s.chunk_size in
      s.chunk <- {
	base = {count=1; base_iov=raw};
	iov = raw;
      };
      s.pos <- 0;
      mm_cbuf_sub s.chunk.base.base_iov 0
    ) else
      mm_cbuf_sub s.chunk.base.base_iov s.pos
	
  (* We have allocated [len] space from the cached iovec.
   * Update our postion in it.
   *)
  let advance len = 
    s.chunk.base.count <- succ s.chunk.base.count;
    s.pos <- s.pos +|| len;
    assert (s.pos <=|| s.chunk_size)
      
  let empty = 
    let iov = mm_empty () in
    t_of_iovec {count=1; base_iov=iov} iov
      
  let t_of_string s ofs len = 
    let t = alloc len in
    copy_string_into_raw s ofs len t.iov;
    t
      
  let string_of_t t = 
    assert (num_refs t >| 0);
    log (fun () -> sprintf "string_of_t: num_refs=%d len=%d" (num_refs t) (len t));
    let s = String.create (len t) in
    copy_raw_into_string t.iov s 0;
    s
      
  let string_of_t_full s ofs t = 
    assert (num_refs t >| 0);
    copy_raw_into_string t.iov s ofs
      
      
  (* Create an iovec that points to a sub-string in [iov].
   *)
  let sub t ofs len = 
    assert (num_refs t >| 0);
    if ofs +|| len >|| t.iov.len then 
      raise (Invalid_argument 
	(Printf.sprintf "out-of-bounds, in Iovec.sub ofs=%d len=%d act_len=%d"
	  ofs len t.iov.len)
      );
    t.base.count <- succ t.base.count;
    { 
      base = t.base;
      iov= mm_sub t.iov ofs len 
    } 
    
    
  (* Increment the refount, do not really copy. 
   *)
  let copy t = 
    assert (num_refs t >| 0);
    (*log (fun () -> "copy");*)
    t.base.count <- succ t.base.count ;
    t
      
  let really_copy t = 
    assert (num_refs t >| 0);
    if t.iov.len >|| 0 then (
      let new_iov = alloc t.iov.len in
      copy_raw_into_raw t.iov new_iov.iov;
      new_iov
    ) else
      empty
	
	
  (* Compute the total length of an iovec array.
   *)
  let iovl_len iovl = Array.fold_left (fun pos iov -> 
    assert (num_refs iov >| 0);
    pos +|| len iov
  ) 0 iovl
    
  (* Flatten an iovec array into a single iovec.  Copying only
   * occurs if the array has more than 1 non-empty iovec.
   * 
   * If there was not enough memory for the flatten operation,
   * then an empty buffer is returned.
   *)
  let flatten iovl = 
    match iovl with 
	[||] -> raise (Invalid_argument "An empty iovecl to flatten")
      | [|i|] -> copy i
      | _ -> 
	  let total_len = iovl_len iovl in
	  let iovl = Array.map (fun x -> 
	    assert (num_refs x >| 0);
	    x.iov
	  ) iovl in
	  let dst = alloc total_len in
	  mm_flatten iovl dst.iov;
	  dst
	    
  let flatten_w_copy iovl = 
    match iovl with 
	[||] -> raise (Invalid_argument "An empty iovecl to flatten")
      | [|i|] -> really_copy i
      | _ -> 
	  let total_len = iovl_len iovl in
	  let iovl = Array.map (fun x -> 
	    assert (num_refs x >| 0);
	    x.iov
	  ) iovl in
	  let dst = alloc total_len in
	  mm_flatten iovl dst.iov;
	  dst
	    
  external mm_marshal : 'a -> raw -> Marshal.extern_flags list -> int
    = "mm_output_val"
    
  external mm_unmarshal : raw -> 'a
    = "mm_input_val"
    
  let mm_marshal obj t flags = 
    mm_marshal obj t.iov flags
    
  let marshal obj flags = 
    let space_left = s.chunk_size -|| s.pos in
    let tail = sub s.chunk s.pos space_left in
    try 
      let len = mm_marshal obj tail flags in
      s.pos <- s.pos +|| len;
      assert (s.pos <=|| s.chunk_size);
      let res = sub tail 0 len in
      free tail;
      res
    with Failure _ -> 
      (* Not enough space.
       * We waste the end of the cached iovec, and 
       * allocate a new chunk.
       *)
      free tail ;
      free_old_chunk ();
      let raw = mm_alloc s.chunk_size in
      s.chunk <- {
	base = {count=1; base_iov=raw};
	iov = raw
      };
      s.pos <- 0;
      try 
	let len = mm_marshal obj s.chunk flags in
	s.pos <- s.pos +|| len;
	assert (s.pos <=|| s.chunk_size);
	sub s.chunk 0 len
      with Failure _ -> 
	(* The messgae is larger than a chunk size, hence too long.
	 *)
	failwith (sprintf "message is too longer (> %d)" s.chunk_size)
	
  let unmarshal t =  
    assert (num_refs t >| 0);
    mm_unmarshal t.iov 
    
(*
  let _ = 
  printf "Running Socket iovec marshal/unmarshal sanity test\n"; flush stdout;
  let iov = alloc 100 in
  ignore (marshal (1,2,("A","B")) iov []);
  let obj = unmarshal iov in
  printf "xxx\n"; Pervasives.flush Pervasives.stdout;
  begin match obj with 
  x,y,(z,w) -> 
  printf "x=%d, y=%d z=%s w=%s\n" x y z w
  end;
  printf "Passed)\n"; flush stdout;
  ()
*)
end

(**************************************************************)

module Debug = struct
end

(**************************************************************)

module Export = Opt

let check_pre_alloc = Export.check_pre_alloc
let advance = Export.advance
let t_of_iovec = Export.t_of_iovec
let s = Export.s

let raw_of_t = Export.raw_of_t
let t_of_raw = Export.t_of_raw
let string_of_t = Export.string_of_t
let string_of_t_full = Export.string_of_t_full
let t_of_string = Export.t_of_string
let len = Export.len
let empty = Export.empty
let alloc = Export.alloc
let sub = Export.sub
let free = Export.free
let copy = Export.copy
let really_copy = Export.really_copy
let flatten = Export.flatten
let flatten_w_copy = Export.flatten_w_copy
let num_refs = Export.num_refs
let marshal = Export.marshal
let unmarshal = Export.unmarshal
  
