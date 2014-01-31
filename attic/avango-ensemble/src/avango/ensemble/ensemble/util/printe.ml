(* Modified version of printf.ml to automatically flush after every print.
 *)

(***********************************************************************)
(*                                                                     *)
(*                           Objective Caml                            *)
(*                                                                     *)
(*            Xavier Leroy, projet Cristal, INRIA Rocquencourt         *)
(*                                                                     *)
(*  Copyright 1996 Institut National de Recherche en Informatique et   *)
(*  Automatique.  Distributed only by permission.                      *)
(*                                                                     *)
(***********************************************************************)

external format_int: string -> int -> string = "caml_format_int"
external format_float: string -> float -> string = "caml_format_float"
(*
let format_int f i = Printf.sprintf f i
let format_float f i = Printf.sprintf f i
*)

let f callback format =
  let format = (Obj.magic format : string) in
  let rec doprn start i accu =
    if i >= String.length format then begin
      let res = 
        if i > start    
        then String.sub format start (i-start) :: accu
        else accu in
      Obj.magic(callback (String.concat "" (List.rev res)))
    end else
      if String.unsafe_get format i <> '%' then
        doprn start (i+1) accu
      else begin
        let accu1 =
          if i > start then
          String.sub format start (i-start) :: accu
          else accu in
        let j = skip_args (succ i) in
        match String.unsafe_get format j with
          '%' ->
            doprn j (succ j) accu1
        | 's' ->
            Obj.magic(fun s ->
              let accu2 =
                if j <= i+1 then
                  s :: accu1
                else begin
                  let p =
                    try
                      int_of_string (String.sub format (i+1) (j-i-1))
                    with _ ->
                      invalid_arg "fprintf: bad %s format" in
                  if p > 0 & String.length s < p then
                    s :: String.make (p - String.length s) ' ' :: accu1
                  else if p < 0 & String.length s < -p then
                    String.make (-p - String.length s) ' ' :: s :: accu1
                  else
                    s :: accu1
                end in
              doprn (succ j) (succ j) accu2)
        | 'c' ->
            Obj.magic(fun c ->
              doprn (succ j) (succ j) (String.make 1 c :: accu1))
        | 'd' | 'o' | 'x' | 'X' | 'u' ->
            Obj.magic(fun n ->
              doprn (succ j) (succ j)
                    (format_int (String.sub format i (j-i+1)) n :: accu1))
        | 'f' | 'e' | 'E' | 'g' | 'G' ->
            Obj.magic(fun f ->
              doprn (succ j) (succ j)
                    (format_float (String.sub format i (j-i+1)) f :: accu1))
        | 'b' ->
            Obj.magic(fun b ->
              doprn (succ j) (succ j) (string_of_bool b :: accu1))
        | 'a' ->
            Obj.magic(fun printer arg ->
              doprn (succ j) (succ j) (printer () arg :: accu1))
        | 't' ->
            Obj.magic(fun printer ->
              doprn (succ j) (succ j) (printer () :: accu1))
        | c ->
            invalid_arg ("sprintf: unknown format")
      end

  and skip_args j =
    match String.unsafe_get format j with
      '0' .. '9' | ' ' | '.' | '-' -> skip_args (succ j)
    | c -> j

  in doprn 0 0 []
