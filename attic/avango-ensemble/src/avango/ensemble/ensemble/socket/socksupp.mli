(**************************************************************)
(* SOCKSUPP.MLI *)
(* Ohad Rodeh : 10/2002                                    *)
(**************************************************************)

type buf = string
type len = int
type ofs = int
type socket = Unix.file_descr

type timeval = {
  mutable sec10 : int ;
  mutable usec : int
} 

type win = 
    Win_3_11
  | Win_95_98
  | Win_NT_3_5
  | Win_NT_4
  | Win_2000

type os_t_v = 
    OS_Unix
  | OS_Win of win

val print_line : string -> unit
val is_unix : bool 
val max_msg_size : int

