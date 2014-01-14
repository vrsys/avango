(**************************************************************)
(* ARRAYOP.ML *)
(* Author: Mark Hayden, 1/97 *)
(**************************************************************)
open Util
(**************************************************************)

let maxify a1 a2 =
  if Array.length a1 <> Array.length a2 then
    failwith "maxify:mismatched arrays" ;
  for i = 0 to pred (Array.length a1) do
    a1.(i) <- int_max a1.(i) a2.(i)
  done	      

let minify a1 a2 =
  if Array.length a1 <> Array.length a2 then
    failwith "minify:mismatched arrays" ;
  for i = 0 to pred (Array.length a1) do
    a1.(i) <- int_min a1.(i) a2.(i)
  done	      

let mergify a1 a2 =
  if Array.length a1 <> Array.length a2 then
    failwith "mergify:mismatched arrays" ;
  for i = 0 to pred (Array.length a1) do
    a1.(i) <- a1.(i) || a2.(i)
  done	      

let fillify a v =
  Array.fill a 0 (Array.length a) v

let setify a1 a2 =
  if Array.length a1 <> Array.length a2 then
    failwith "setify:mismatched arrays" ;
  Array.blit a2 0 a1 0 (Array.length a1)

let fmaxify a1 a2 =
  if Array.length a1 <> Arrayf.length a2 then
    failwith "fmaxify:mismatched arrays" ;
  for i = 0 to pred (Array.length a1) do
    a1.(i) <- max a1.(i) (Arrayf.get a2 i)
  done	      

let fminify a1 a2 =
  if Array.length a1 <> Arrayf.length a2 then
    failwith "fminify:mismatched arrays" ;
  for i = 0 to pred (Array.length a1) do
    a1.(i) <- min a1.(i) (Arrayf.get a2 i)
  done	      

let fmergify a1 a2 =
  if Array.length a1 <> Arrayf.length a2 then
    failwith "fmergify:mismatched arrays" ;
  for i = 0 to pred (Array.length a1) do
    a1.(i) <- a1.(i) || (Arrayf.get a2 i)
  done	      

let fsetify a1 a2 =
  setify a1 (Arrayf.to_array a2)

let memoize a b c =
  if a = c then a else if b = c then b else c

let ffminify a b = memoize a b (Arrayf.map2 int_min a b)
let ffmaxify a b = memoize a b (Arrayf.map2 int_max a b)
let ffmergify a b = memoize a b (Arrayf.map2 (or) a b)
