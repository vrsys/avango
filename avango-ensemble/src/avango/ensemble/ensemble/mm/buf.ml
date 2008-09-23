(**************************************************************)
(* BUF.MLI *)
(* Author: Mark Hayden, 8/98 *)
(**************************************************************)
open Trans
open Util
(**************************************************************)
let name = "BUF"
let log = Trace.log name
(**************************************************************)

type len = int
type ofs = len

external int_of_len : len -> int = "%identity"
external len_of_int : int -> len = "%identity"
external (=||) : len -> len -> bool = "%eq"
external (<>||) : len -> len -> bool = "%noteq"
external (>=||) : len -> len -> bool = "%geint"
external (<=||) : len -> len -> bool = "%leint"
external (>||) : len -> len -> bool = "%gtint"
external (<||) : len -> len -> bool = "%ltint"
external (+||) : len -> len -> len = "%addint"
external (-||) : len -> len -> len = "%subint"
external ( *||) : len -> int -> len = "%mulint"

let len0 = 0
let len4 = 4
let len8 = 8
let len12 = 12
let len16 = 16

(* We use a defined value instead of calculating
 * it each time.  This lets the optimizer sink its
 * teeth into the value.
 *)
let md5len = 16
let _ = assert (md5len = String.length (Digest.string ""))
let md5len_plus_4 = md5len +|| len4
let md5len_plus_8 = md5len +|| len8
let md5len_plus_12 = md5len +|| len12
let md5len_plus_16 = md5len +|| len16
(**************************************************************)

type t = string
type digest = string (* A 16-byte string *)

let of_string = ident
let string_of = ident

let create = String.create
let digest_sub = Digest.substring
let digest_substring = Digest.substring
let copy = String.copy
let empty = ""
let concat bl = String.concat "" bl
let fragment len buf =
  let buf_len = String.length buf in
  let nfrags = (buf_len + pred len)/ len in
  Array.init nfrags (fun i -> 
    if i< pred nfrags then
      String.sub buf (i*len) len
    else
      String.sub buf (i*len) (buf_len mod len)
  ) 

let append b0 b1 = b0 ^ b1
let to_hex = Util.hex_of_string

let length s = len_of_int (String.length s)

let check buf ofs len =
  assert(
    if ofs < 0 || len < 0 || ofs + len > length buf then (
      eprintf "BUF:check:ofs=%d len=%d act_len=%d\n" ofs len (length buf) ;
      invalid_arg ("Buf.check:")
    );
    true
  )

(* Unsafe blit can be used because we make the appropriate
 * checks first.
 *)
let blit sbuf sofs dbuf dofs len =
  check sbuf sofs len ;
  check dbuf dofs len ;
  String.unsafe_blit sbuf sofs dbuf dofs len

let blit_str = blit

let sub buf ofs len = 
  check buf ofs len ;
  String.sub buf ofs len

(**************************************************************)

let int16_of_substring s ofs =
  (Char.code s.[ofs    ] lsl  8) +
  (Char.code s.[ofs + 1]       )

let subeq8 buf ofs buf8 =
     buf.[ofs    ] = buf8.[0] 
  && buf.[ofs + 1] = buf8.[1]
  && buf.[ofs + 2] = buf8.[2]
  && buf.[ofs + 3] = buf8.[3]
  && buf.[ofs + 4] = buf8.[4]
  && buf.[ofs + 5] = buf8.[5]
  && buf.[ofs + 6] = buf8.[6]
  && buf.[ofs + 7] = buf8.[7]

let subeq16 buf ofs buf16 =
     subeq8 buf ofs buf16
  && buf.[ofs +  8] = buf16.[8] 
  && buf.[ofs +  9] = buf16.[9]
  && buf.[ofs + 10] = buf16.[10]
  && buf.[ofs + 11] = buf16.[11]
  && buf.[ofs + 12] = buf16.[12]
  && buf.[ofs + 13] = buf16.[13]
  && buf.[ofs + 14] = buf16.[14]
  && buf.[ofs + 15] = buf16.[15]

let write_int32 s ofs i =
  (* Xavier says it is unnecessary to "land 255" the values
   * prior to setting the string values.
   *)
  s.[ofs    ] <- Char.unsafe_chr (i asr 24) ;
  s.[ofs + 1] <- Char.unsafe_chr (i asr 16) ;
  s.[ofs + 2] <- Char.unsafe_chr (i asr  8) ;
  s.[ofs + 3] <- Char.unsafe_chr (i       )
    
let read_int32 s ofs =
  (Char.code s.[ofs    ] lsl 24) +
  (Char.code s.[ofs + 1] lsl 16) +
  (Char.code s.[ofs + 2] lsl  8) +
  (Char.code s.[ofs + 3]       ) 
  
(**************************************************************)
  
let make_marsh debug = 
  let unmarsh obj ofs = 
    Marshal.from_string obj ofs in
  let marsh obj = Marshal.to_string obj [] in
  (marsh,unmarsh)

(**************************************************************)
let max_msg_len = 8192 (*Socket.max_msg_len ()*)

type prealloc = {
  chunk_size : len ;     (* The chucks size to allocate *)
  mutable buf : t ;      (* A preallocated (long) buffer *)
  mutable ofs : len ;    (* The offset in [str] *)
}

let p = 
  let chunk_size = 2 *|| max_msg_len in
  {
    chunk_size = chunk_size ;
    buf = create chunk_size ; 
    ofs = 0 
  } 

(* 
*)
let advance len = 
  p.buf <- String.create p.chunk_size ;
  p.ofs <- 0
    
let prealloc len f = 
  if p.ofs + len > String.length p.buf then (
    p.buf <- String.create p.chunk_size ;
    p.ofs <- len ;
    f p.buf 0 len;
  ) else (
    f p.buf p.ofs len;
    p.ofs <- p.ofs + len;
  )


(* Mashal an ML object into a substring and send it. 
 *)  
let prealloc_marsh len obj f = 
  (* The case where the cached buffer is too short, and we need to
   * start from scratch.
   *)
  let fresh_start () = 
    p.buf <- String.create p.chunk_size ;
    let molen = 
      try
	Marshal.to_buffer p.buf len (p.chunk_size -|| len) obj []
      with Failure _ -> 
	printf "Fatal error, ML object too large";
	exit 0
    in
    f p.buf 0 (len+molen);
    p.ofs <- len + molen
  in

  if p.ofs + len >|| p.chunk_size then (
    fresh_start ()
  ) else (
      try
	let molen = 
	  Marshal.to_buffer p.buf (p.ofs+len) (p.chunk_size -|| p.ofs -|| len)
	    obj []
	in    
	log (fun () -> sprintf "writing into ofs=%d, max_len=%d, molen=%d"
	  (p.ofs+len) (p.chunk_size - p.ofs - len) molen);
	f p.buf p.ofs (len +|| molen);
	p.ofs <- p.ofs +|| len +|| molen;
      with Failure _ -> 
	fresh_start ()
  )
  
  
