(**************************************************************)
(* ARRAYOP.MLI *)
(* Author: Mark Hayden, 1/97 *)
(**************************************************************)
(* More array operations.  All are in-place (the first arg
 * is updated).
 *)
val maxify : int array -> int array -> unit
val minify : int array -> int array -> unit
val mergify : bool array -> bool array -> unit
val setify : 'a array -> 'a array -> unit
val fillify : 'a array -> 'a -> unit

val fsetify : 'a array -> 'a Arrayf.t -> unit
val fmaxify : int array -> int Arrayf.t -> unit
val fminify : int array -> int Arrayf.t -> unit
val fmergify : bool array -> bool Arrayf.t -> unit

(* These are purely functional versions.
 *)
val ffmaxify : int Arrayf.t -> int Arrayf.t -> int Arrayf.t
val ffminify : int Arrayf.t -> int Arrayf.t -> int Arrayf.t
val ffmergify : bool Arrayf.t -> bool Arrayf.t -> bool Arrayf.t

(**************************************************************)
