(**************************************************************)
(* TREE.MLI : D-WGL algorithm *)
(* Author: Ohad Rodeh, 2/99 *)
(**************************************************************)

module type OrderedKey = sig
  type t

  val compare : t -> t -> int
  val rand    : unit -> t 
  val string_of_t : t -> string
end

module type OrderedMember = sig
  type t 

  val of_int  : int -> t
  val int_of  : t -> int
  val compare : t -> t -> int
  val string_of_t : t -> string
end

(*********************************************************************)
module type S = sig
  type t 
  type member
  type key 

    (* A list of actions describing the merging of several trees.
     *)
  type alist  

  (* The set of actions to be performed by a member. 
   *)
  type memrec = {
    mutable send : (member * key) list ;
    mutable cast1: (key list * key) list ;
    mutable recv : key option ;
    mutable cast2: (key * key) option
  }

  val string_of_memrec : memrec -> string

  (* A mapping between member and its original tree.
   *)
  type memmap 

  type keytable = (key, t) Hashtbl.t
  type actions_h = (member, memrec) Hashtbl.t

  (* The full information about a merge of several trees
   *)
  type full = actions_h * memmap * keytable * (key * key) list
  type sent = actions_h * (key * key) list

  type debug_map 
    
  (* merge a list of trees.
   *)
  val merge  : t list -> t * alist  

  (* Replace the top key with a new one.
   *)
  val new_top : t -> t * alist

  val string_of_t : t -> string
  val height_string_of_t : t -> string
  val string_of_alist : alist -> string
  val actions_of_merge : t -> alist -> full
  val string_of_full : full -> string
  val string_of_keytable : keytable  -> string
  val string_of_debug_map : debug_map -> string 

  (* [decrypt keyl mcl] Compute the set of keys decryptable from the list [mcl] 
   * assuming [keyl] are known 
  *)
  val decrypt : key list -> (key list * key) list -> key list
    
  (* Compute the total set of multicasts performed 
   *)
  val mcast_sweep : actions_h -> (key list * key) list 

  (* [self_check verbose tree full] checks whether the (tree,full) 
   * are correct. All members may encrypt only with keys they know, and 
   * all members must receive all the keys they need. 
   *)
  val self_check : bool -> t -> full -> debug_map * bool * string 
      
  (* Zipping and unzipping trees. Trees have to be passed through
   * the network, in order to reduce message size their size should
   * be reduced. 
   *)
  type z 
  val string_of_z : z -> string
  val zip : t -> z 
  val unzip : z -> t 
  val zempty : z
  val zleaf  : member -> z
  val zam_leader : z -> member -> bool 
  val members_of_z : z -> member list

  (* [keylist full member] find the list of keys from [member] to the root on 
   * tree [t] 
   *)
  val zkeylist : z -> member -> key list

  (* [remove_members_spec tree member rmv_l] returns the largest subtree 
   * of [tree] that contains [member] but does not contain any member from
   * [rmv_l].
   *)
  val zremove_members_spec : z -> member -> member list -> z

  (* [zmap f g tree] Map the tree. [f] works on the members, [g]
   * works on the keys. 
   *)
  val zmap   : (member -> member) -> (key -> key) -> z -> z
end

module Make(Member: OrderedMember) (Key: OrderedKey) : (S with type member = Member.t and type key=Key.t)
(*********************************************************************)


