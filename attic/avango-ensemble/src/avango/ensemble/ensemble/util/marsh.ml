(**************************************************************)
(* MARSH.ML *)
(* Author: Mark Hayden, 12/96 *)
(**************************************************************)
open Trans
open Util
open Buf
(**************************************************************)
let name = Trace.file "MARSH"
(**************************************************************)

exception Error of string

let error s = raise (Error s)

(**************************************************************)
type item =
  | Int of int
  | String of Buf.t
  | Buf of Buf.t

type marsh = {
  mutable buf : item list
} 

let marsh_init () = {
  buf = []
} 

let add m v =
  m.buf <- v :: m.buf

let write_int m i =
  add m (Int i)

let write_len m i =
  write_int m (int_of_len i)

let write_bool m b =
  write_int m (if b then 1 else 0)

let write_buf m b =
  add m (Buf b)

let write_string m s =
  let buf = Buf.of_string s in
  add m (String buf)

let write_list m f l =
  write_int m (List.length l) ;
  List.iter (fun it -> f it) l

let write_array m f a =
  write_int m (Array.length a) ;
  Array.iter (fun it -> f it) a

let write_option m f o =
  match o with
  | None -> write_bool m false
  | Some o -> 
      write_bool m true ;
      f o

let length = function
  | Int _ -> len4
  | String buf 
  | Buf buf -> Buf.length buf +|| len4

(* The total length accumulated in a list of objects. 
*)
let len l = List.fold_left (fun accu item -> 
  accu +|| length item
) len0 l
  
(* Marshal a list into a set 
 *)
let marsh_done msh =
  let l = List.rev msh.buf in
  let len = len l in
  let buf = Buf.create len in
  let _ = List.fold_left (fun pos it ->
    let len =
      match it with
	| Int i ->
	    Buf.write_int32 buf pos i ;
	    len4
	| String str
	| Buf str -> 
	    let len = Buf.length str in
	    Buf.write_int32 buf pos (int_of_len len) ;
	    Buf.blit str len0 buf (pos+||len4) len;
	    len +|| len4
    in
    pos +|| len
  ) len0 l 
  in
  buf
    
(**************************************************************)
      

type unmarsh = {
  unm : Buf.t;
  mutable pos : ofs ;
  len : len ;
} 

let unmarsh_init buf ofs = {
  unm = buf ;
  pos = ofs ;
  len = Buf.length buf -|| ofs
} 

let unmarsh_check_done_all m =
  assert (m.pos == m.len) ;
  ()

let bounds_check m len =
  if m.pos +|| len >|| m.len then
    error "bounds_check"

let advance m len =
  m.pos <- m.pos +|| len

let get_string m len =
  let buf = Buf.string_of (Buf.sub m.unm m.pos len) in
  advance m len;
  buf

let read_int m =
  let ret = Buf.read_int32 m.unm m.pos in
  advance m len4 ;
  ret
      
let read_len m =
  let len = read_int m in
  len_of_int len

let read_bool m =
  match read_int m with
  | 0 -> false
  | 1 -> true
  | _ -> error "read_bool:not a bool"

let read_string m =
  let len = read_len m in
  get_string m len

let read_buf m =
  Buf.of_string (read_string m)

let read_list m f =
  let len = read_int m in
  let rec loop len =
    if len = 0 then [] 
    else (f ()) :: (loop (pred len))
  in loop len

let read_option m f =
  if read_bool m then 
    Some (f ())
  else None


(**************************************************************)
