(**************************************************************)
(* DOMAIN.ML *)
(* Author: Mark Hayden, 4/96 *)
(**************************************************************)
open Util
open Trans
open Buf
(**************************************************************)
let name = Trace.file "DOMAIN"
let failwith = Trace.make_failwith name
(**************************************************************)

type loopback = bool

type dest =
| Pt2pt of Addr.set Arrayf.t
| Mcast of Group.id * loopback
| Gossip of Group.id

type handle = {
  xmit		: dest -> Route.xmitf option ;
  disable 	: unit -> unit
}
    
type t = {
  name		: string ;
  addr      	: Addr.id -> Addr.t ;
  enable    	: Addr.id -> Group.id -> Addr.set -> View.t -> handle ;
}

(**************************************************************)

let string_of_dest = function
| Pt2pt d    -> sprintf "{Pt2pt:%s}"    (Arrayf.to_string Addr.string_of_set d)
| Mcast(d,l) -> sprintf "{Mcast:%s:%b}" (Group.string_of_id d) l
| Gossip(d)  -> sprintf "{Gossip:%s}"   (Group.string_of_id d)

let name    d = d.name
let enable  d = d.enable
let disable d = d.disable ()
let xmit    d = d.xmit
let addr    d = d.addr

let create name addr enable = {
  name 	  	= name ;
  addr 		= addr ;
  enable  	= enable
}

let handle disable xmit = {
  xmit          = xmit ;
  disable       = disable
} 

(**************************************************************)

(* Domain management.
 *)

let dormant = ref []
let activated = ref []

let install id d =
  dormant := (id,d) :: !dormant

let of_mode alarm mode =
  (* Use the Udp domain for Deering and Sp2.
   *)
  let mode = match mode with
  | Addr.Deering -> Addr.Udp
  | _ -> mode
  in

  (* If in activated list, use that.
   *)
  try 
    List.assoc mode !activated 
  with Not_found -> (
    try
      (* Look up in dormant list.
       *)
      let d = List.assoc mode !dormant in

      (* Remove it from dormant list.
       *)
      dormant := 
        List.fold_left (fun o ((i,_) as it) ->
	  if mode = i then o else it :: o
        ) [] !dormant ;

      (* Enable the domain and add to activated list.
       *)
      let d = d alarm in
      activated := (mode,d) :: !activated ;
      d
    with Not_found ->
      eprintf "DOMAIN:domain '%s' not installed, exiting\n"
      (Addr.string_of_id mode) ;
      exit 1
  )

(**************************************************************)
