(**************************************************************)
(* SHARED *)
(* Author: Mark Hayden, 6/95 *)
(**************************************************************)
open Buf
(**************************************************************)

module Cipher : sig 
  type src = Buf.t
  type dst = Buf.t

  type ctx = string
  type iv  = string

  type context = 
    | Encrypt of ctx * iv
    | Decrypt of ctx * iv

  (* The type of a shared key manager.
   *)
  type t

  (* Lookup an encryption mechanism.
   *)
  val lookup : string (* NAME *) -> t

  (* What is the name of this mechanism
   *)
  val namea : t -> string 

  (* Create a new context from the key.
   *)
  val init : t -> Security.cipher -> bool -> context

  (* Encrypt/decrypt 
   *)
  val encrypt : t -> context -> src -> Buf.ofs -> Buf.len -> dst

  (* Encrypt/decrypt inplace. This works directly on an iovec, 
   * i.e., this is a front-end for a C function.
   *)
  val encrypt_inplace : t -> context -> Iovec.t -> unit

  val single_use : t -> Security.cipher -> bool -> src -> dst


  (* Encrypt and decrypt a Security.key
   *)
  val encrypt_key : t -> Security.cipher -> Security.key -> Security.key
  val decrypt_key : t -> Security.cipher -> Security.key -> Security.key


  (* Install a new encryption mechanism.
   *)
  val install : 
    string ->  (* NAME *)
    (Security.cipher -> bool) -> (* key_ok *) 
    (Security.cipher -> bool -> context) -> (* init *)
    (context -> Buf.t -> ofs -> Buf.t -> ofs -> len -> unit) -> (* update *) 
    (context -> Iovec.t -> unit)  (* encrypt an iovec inplace *)
	-> unit

  (* [check_sanity name snt_loops perf_loops] Check that an algorithm 
   * actually works. Also measure its performance. 
   *)
  val sanity_check : string -> int -> int -> unit
end 

(**************************************************************)

module Sign : sig
  (* The type of a shared signature manager.
   *)
  type t

  type context = Buf.t
  type digest  = Buf.t

  (* Find the signature algorithm. 
   *)
  val lookup : string (* NAME *) -> t

  (* The name of the specified algorithm
   *)
  val namea : t -> string 

  (* Create a context from a key, the hack version takes a preallocated string.
   *)
  val init : t -> Buf.t -> context
  val init_hack : t -> context -> Buf.t -> unit

  (* Update the context with a substring
   *)
  val update : t -> context -> Buf.t -> ofs -> len -> unit

  (* Create a digest from a context, the hack version takes a preallocated 
   * context. 
   *)
  val final : t -> context -> digest
  val final_hack : t -> context -> Buf.t -> ofs -> unit

  (* Install a new signture mechanism.
   *)
  val install : 
    string -> 
    (Buf.t -> context) ->  (* init *)
    (context -> Buf.t -> unit) -> (* init_hack *)
    (context -> Buf.t -> ofs -> len -> unit) -> (* update *)
    (context -> digest) ->  (* final *)
    (context -> Buf.t -> ofs -> unit) -> (* final_hack *)
      unit

  (* Sign a portion of a buffer with a key. The hack version takes
   * a preallocated string.
   *)
  val sign : t -> Security.mac -> Buf.t -> Buf.ofs -> Buf.len -> Buf.t
  val sign_hack : t -> Security.mac -> Buf.t -> Buf.ofs -> Buf.len -> Buf.t -> 
    Buf.ofs -> unit
end	    
	    
(**************************************************************)

module Prng : sig
  type t
    
  type context
  type full_context
    
  val namea : t -> string 
  val init : t -> Buf.t -> context
  val rand : t -> context -> Buf.t
  val lookup : string (* NAME *)-> t
      
  val install : 
    string -> (* NAME *)
    (Buf.t -> context) -> (* init *)
    (context -> Buf.t) -> (* rand *)
      unit
	    
  (* Create a fresh security key/mac/cipher. 
   *)
  val create_key : unit -> Security.key 
  val create_mac : unit -> Security.mac
  val create_cipher : unit -> Security.cipher
  val create_buf : Buf.len -> Buf.t

end    
  
(**************************************************************)
module DH : sig
  type t 

  (* The type of a long integer (arbitrary length). It is implemented
   * efficiently * by the OpenSSL library.
   *)
  type bignum
    
  (* A key contains four bignums: p,g,x,y. Where p is the modulo, g is
   * a generator for the group of integers modulo p, x is the private key, 
   * and y is the public key. y = p^x mod p.
   *)
  type key
    
  (* Find the signature algorithm. 
   *)
  val lookup : string (* NAME *) -> t

  (* print the parameters of a key.
   *)
  val show_params : t -> key -> unit
      
  (* get a key of a specific length from the standard file list.
   *)
  val fromFile : t -> int -> key 

  val namea : t -> string 

  val init : t -> unit -> unit 
      
  (* generate p and g.
   *)
  val generate_parameters : t -> int -> key
      
  (* Create a new key with the same p and g.
   *)
  val copy_params : t -> key -> key
      
  (* create new x and y, where the p and g already exist.
   *)
  val generate_key : t -> key -> unit
      
  val get_p : t -> key -> bignum
      
  val get_g : t -> key -> bignum 
      
  val get_pub : t -> key -> bignum 
      
  val get_prv : t -> key -> bignum 
      
  (* Convert a string into a DH key.
   *)
  val key_of_string : t -> string -> key
      
      
  (* Convert a DH key into a key.
   *)
  val string_of_key : t -> key -> string
      
      
  val string_of_bignum : t -> bignum -> string
      
  val bignum_of_string : t -> string -> bignum
      
  (* [compute_key Key(x) g^z] returns g^(zx) mod p 
   *)
  val compute_key : t -> key -> bignum -> string


  (* Install a DH mechanism
   *)
  val install : 
    string -> 
    ( unit -> unit ) ->       (* init *)
    ( int -> key ) ->       (* generate_parameters *)
    ( key -> key ) ->       (* copy_params *)
    ( key -> unit ) ->      (* generate_key *)
    ( key -> bignum ) ->    (* get_p *)
    ( key -> bignum ) ->    (* get_g *)
    ( key -> bignum ) ->    (* get_pub *)
    ( key -> bignum ) ->    (* get_prv *)
    ( string -> key ) ->    (* key_of_string *)
    ( key -> string ) ->    (* string_of_key *)
    ( bignum -> string ) -> (* string_of_bignum *)
    ( string -> bignum ) -> (* bignum_of_string *)
    ( key -> bignum -> string ) -> (* compute_key *)
      unit

  (* [check_sanity name snt_loops perf_loops] Check that an algorithm 
   * actually works. Also measure its performance. 
   *)
  val sanity_check : string -> int -> int -> unit

  (* [generate_key t len] Generate new key parameters of size [len]
   *)
  val generate_new_parameters : t -> int -> unit
end
  
(**************************************************************)
  
  
