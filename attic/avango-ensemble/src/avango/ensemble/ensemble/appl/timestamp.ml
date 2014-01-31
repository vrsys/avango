(**************************************************************)
(* TIMESTAMP.ML *)
(* Author: Mark Hayden, 4/97 *)
(**************************************************************)
open Util
(**************************************************************)
let name = Trace.file "TIMESTAMP"
(**************************************************************)

type t = int

let stamps = ref []
let counter = counter ()
let register debug =
  try List.assoc debug !stamps with Not_found ->
    let i = counter () in
    stamps := (debug,i) :: !stamps ;
    i

let start = Hsys.gettimeofday ()

let asize = 5000

type item = {
  mutable name : t ;
  mutable time : Time.m ;
  mutable words : int
} 
    

type state = {
  mutable i : int ;
  mutable curr : item array ;
  mutable stashed : item array list
} 

let init () = {
  i = 0 ;
  curr = [||] ;
  stashed = []
} 

let g = init ()

let item _ = {
  name = 0 ;
  time = Time.mut () ;
  words = 0
} 

let rec(*avoid inline*) refresh () =
  g.stashed <- g.curr::g.stashed ;
  g.curr <- Array.init asize item ;
  g.i <- 0
 
let add msg = 
  if g.i >= Array.length g.curr then
    refresh () ;
  let i = g.i in
  let item = g.curr.(i) in
  g.i <- succ i ;
  Time.mut_gettimeofday item.time ;
  item.name <- msg ;
  item.words <- Hsys.minor_words ()

let linker_add msg = 
  let msg = register msg in
  fun () ->
    if g.i >= Array.length g.curr then
      refresh () ;
    let i = g.i in
    let item = g.curr.(i) in
    g.i <- succ i ;
    Time.mut_gettimeofday item.time ;
    item.name <- msg ;
    item.words <- Hsys.minor_words ()

(*	 
let addtime msg time =
  pending := (msg,time) :: !pending
*)

let print () =
(*
  List.iter (fun s ->
    eprintf "TIMESTAMP:GC:%s\n" s ;
  ) (string_list_of_gc_stat (Gc.stat ())) ;
*)

  let all =
    let stashed = g.stashed in
    let stashed = List.rev g.stashed in
    let minor = Array.sub g.curr 0 g.i in
    g.i <- 0 ;
    let stashed = stashed @ [minor] in
    Array.concat stashed
  in
  let names = Array.map (fun i -> i.name) all in
  let times = Array.map (fun i -> Time.mut_copy i.time) all in
  let words = Array.map (fun i -> i.words) all in
  let nsamples = Array.length all in
  
  let count = ref 0 in
  let tot = ref 0.0 in

(*
  eprintf "TIMESTAMP:words=%s\n" (string_of_int_array words) ;
*)
  let wordd = Queuee.create () in
  let last = ref (-1.0) in
  let targ = Queuee.create () in
  let last_msg = ref (-1) in
  let last_word = ref (-1) in
  if nsamples > 0 then (
    eprintf "TIMESTAMP:print\n" ;
    let udp_xmit = register "UDP:xmit" in
    let udp_recv = register "UDP:recv" in
    for i = 0 to pred nsamples do
      let msg = names.(i) in
      let time = Time.to_float times.(i) in
      let word = words.(i) in
(*
      eprintf "  %10.6f (%10.6f) %s\n" (time -. start) (time -. !last) msg ;
*)
      let diff = time -. !last in
      if !last_msg = udp_recv
      && msg = udp_xmit
      && diff < 0.001
      then (
	incr count ;
	tot := !tot +. diff ;
	Queuee.add diff targ ;
      ) ;

      if !last_msg = udp_recv
      && msg = udp_xmit
      && !last_word <> (-1) 
      && word <> (-1) then (
	Queuee.add (word - !last_word) wordd ;
      ) ;

      last := time ;
      last_msg := msg ;
      last_word := word ;
    done ;
    let targ = Queuee.to_array targ in
    Array.sort compare targ;
    let median = 
      try 
	targ.(Array.length targ / 2)
      with _ -> (-1.0)
    in
    let minimum = 
      try 
	targ.(0)
      with _ -> (-1.0)
    in
    
    begin
      let tenths = sample 10 targ in
      let tenths = Array.map (fun f -> f *. 1000000.0) tenths in
      let tenths = string_of_array (sprintf "%0.1f") tenths in
      eprintf "TIMESTAMP:count=%d\n" !count ;
      eprintf "TIMESTAMP:average=%10.6f\n" (!tot /. (float !count)) ;
      eprintf "TIMESTAMP:tenths=%s us\n" tenths ;
    end ;

    let words = Queuee.to_list wordd in
(*
    eprintf "TIMESTAMP:words=%s\n" (string_of_int_list words) ;
*)
    let words = Sort.list (>) words in
    let words = Array.of_list words in
    let words = sample 10 words in
    eprintf "TIMESTAMP:tenths=%s words\n" (string_of_int_array words) ;
  )


