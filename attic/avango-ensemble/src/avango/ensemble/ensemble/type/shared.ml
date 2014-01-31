(**************************************************************)
(* SHARED *)
(* Author: Mark Hayden, 6/95 *)
(* Modifed : Ohad Rodeh , 2/99 *)
(**************************************************************)
open Buf
open Util
open Security
(**************************************************************)
let name = Trace.file "SHARED"
let failwith s = Trace.make_failwith name s
let log = Trace.log name    
let log1 = Trace.log (name^"1")
(**************************************************************)
  
  
module Prng = struct
  type context 
      
  type t = {
    name : string ;
    init : Buf.t -> context ;
    rand : context -> Buf.t
  }
      
  type full_context = t * context

  let namea t = t.name
  let init t = t.init
  let rand t = t.rand
    
  let table = ref []
    
  let install name init rand = 
    let t = {
      name = name ;
      init = init ;
      rand = rand
    } in
    table := (name,t) :: !table
      
  let lookup name =
    let rec loop = function
      | [] -> raise (Failure "no appropriate Pseudo Random Generator available. This probably means that you have not installed a cryptographic library.")
      | (prng_name,t)::tl ->
	  if name = prng_name then t else loop tl
    in loop !table  
	 

  let full_ctx = ref None 
    
  (* This function creates random strings of arbitrary size
   * It initializes the random number generator, if
   * this has not been done so far.  It then uses a sequence
   * of calls to the PRNG.  Each
   * call produces 4 random bytes of data. They 
   * are concatenated together to form a string.
   *)
  let create_random_buf (t,ctx) len = 
    let rec loop s togo = 
      if Buf.length s >=|| len then s
      else 
	let r = rand t ctx in
	loop (Buf.append s r) (togo -|| Buf.length r) 
    in
    let s = loop Buf.empty len in
    Buf.sub s len0 len 
      

  (* Get a context to the PRNG. The PRNG is initialized if it 
   * this is the first call. 
   *)
  let get_full_ctx () = 
    match !full_ctx with 
      | None -> 
	  let t = lookup "ISAAC" in
	  let init = init t in

	  let context = 
	    let frac_of_time () = 
	      let t = Time.to_float (Time.gettimeofday ()) in
	      let sec = float (Pervasives.truncate t) in
	      Pervasives.truncate (1000000.0 *. (t -. sec))
	    in
	    let frc = frac_of_time () in 
	    let pad = Buf.of_string (string_of_int frc) in
	    log (fun () -> sprintf "PRNG, init %s" (Buf.string_of pad));
	    init pad
	  in
	  full_ctx := Some(t,context);
	  (t,context)

      | Some full_ctx -> full_ctx
	  
  (* Create a fresh security key/mac/cipher/buf. 
   *)
  let create_key () = 
    let full_ctx = get_full_ctx () in
    let key = create_random_buf full_ctx (Security.key_len) in
    Security.key_of_buf key

  let create_mac () = 
    let full_ctx = get_full_ctx () in
    let mac = create_random_buf full_ctx (Security.mac_len) in
    Security.mac_of_buf mac

  let create_cipher () = 
    let full_ctx = get_full_ctx () in
    let cipher = create_random_buf full_ctx (Security.cipher_len) in
    Security.cipher_of_buf cipher

  let create_buf len = 
    let full_ctx = get_full_ctx () in
    create_random_buf full_ctx len
end    
  
(**************************************************************)
  
module Cipher = struct
  type src = Buf.t
  type dst = Buf.t
      
  type ctx = string
  type iv  = string
      
  type context = 
    | Encrypt of ctx * iv
    | Decrypt of ctx * iv
	
  type t = {
    name   : string ;
    key_ok : Security.cipher -> bool;
    init   : Security.cipher -> bool -> context ;
    update : context -> Buf.t -> ofs -> Buf.t -> ofs -> len ->unit ;
    update_iov : context -> Iovec.t ->unit ;
  } 
      
  let namea t = t.name
  let init t = t.init

  let table = ref []
    
  let install name key_ok init update update_iov = 
    let t = {
      name = name ;
      key_ok = key_ok ;
      init = init ;
      update = update ;
      update_iov = update_iov 
    } in
    table := (name,t) :: !table
      
  let lookup name =
    let rec loop = function
      | [] -> raise (Failure ("Could not find encryption algorithm <" ^ name ^ ">. This means that either you have not installed a cryptographic library, or, this algorithm is not supported by this library."))

      | (name_enc,t)::tl ->
	  if name=name_enc then t else loop tl
    in loop !table  


  let encrypt t ctx buf ofs len = 
    let res = Buf.create len in
    t.update ctx buf ofs res len0 len;
    res

  let encrypt_inplace t ctx iovec = 
    t.update_iov ctx iovec;
    ()
    
  let single_use t key enc src =
    let ctx = init t key enc in
    (*
      let dst = Buf.create (Buf.length src) in
      encrypt t ctx src len0 dst len0 (Buf.length src) ;
      dst
    *)
    encrypt t ctx src len0 (Buf.length src)
      
  (* Encrypt and decrypt a Security.key
   *)
  let encrypt_key t cipher = function
    | NoKey -> failwith "Cannot encrypt an empty key"
    | Common key -> 
	let ctx = init t cipher true in
	let encrypt = encrypt t ctx in
	Common ({
	  mac = mac_of_buf (encrypt (buf_of_mac key.mac) len0 mac_len) ;
	  cipher = cipher_of_buf (encrypt (buf_of_cipher key.cipher) len0 cipher_len)
	}) 
  
  let decrypt_key t cipher = function
    | NoKey -> failwith "Cannot decrypt an empty key"
    | Common enc_key -> 
	let ctx = init t cipher false in
	let decrypt = encrypt t ctx in
	Security.Common ({
	  mac = mac_of_buf (decrypt (buf_of_mac enc_key.mac) len0 mac_len) ;
	  cipher = cipher_of_buf (decrypt (buf_of_cipher enc_key.cipher) len0 cipher_len) 
	}) 
  

  (* Run an encryption algorithm through a sanity check.
   *)
  let sanity_check alg snt_loops perf_loops = 
    let t = lookup alg in
    let init = init t
    and encrypt = encrypt t
    and encrypt_inplace = encrypt_inplace t in
    
    eprintf "Sanity\n";
    let key = Security.key_of_buf (Buf.of_string "01234567012345670123456701234567") in
    let buf = Buf.of_string "11111111222222223333333344444444" in
    let ctx_fl_enc = init (get_cipher key) true in
    let buf1 = encrypt ctx_fl_enc buf (Buf.len_of_int 0) (Buf.len_of_int 32) in
    let ctx_fl_dec = init (get_cipher key) false in
    let buf2 = encrypt ctx_fl_dec buf1 (Buf.len_of_int 0) (Buf.len_of_int 32) in
    printf "%s\n" (Buf.string_of buf2);
    if buf2 <> buf then failwith "Check_failed";
    eprintf "OK.\n";

    eprintf "Concatenation\n";
    let key = Security.key_of_buf (Buf.of_string "01234567012345670123456701234567") in
    let buf = Buf.of_string "11111111222222223333333344444444" in
    let buf_next = Buf.of_string "55555555666666667777777788888888" in
    let ctx_enc = init (get_cipher key) true in
    let ctx_dec = init (get_cipher key) false in
    let buf1 = encrypt ctx_enc buf (Buf.len_of_int 0) (Buf.len_of_int 32) in
    let buf1_next = 
      encrypt ctx_enc buf_next (Buf.len_of_int 0) (Buf.len_of_int 32) in
    let buf2 = encrypt ctx_dec buf1 (Buf.len_of_int 0) (Buf.len_of_int 32) in
    let buf2_next = encrypt ctx_dec buf1_next (Buf.len_of_int 0) (Buf.len_of_int 32) in
    printf "%s:%s\n" (Buf.string_of buf2) (Buf.string_of buf2_next);
    if buf2 <> buf || buf2_next <> buf_next  then failwith "Check_failed";
    eprintf "OK.\n";
    
    printf "Memory check\n";
    let control = Gc.get () in
    control.Gc.verbose <- 1;
    Gc.set control;

    (* Check sanity of the encryption algorithm. Create a 
     * random string of size [mlen], encrypt and decrypt it. 
     *)
    let mlen = len_of_int 40 in
    for i=0 to snt_loops do 
      let key = Security.key_of_buf (Buf.of_string "01234567012345670123456701234567") in
      let buf = Prng.create_buf mlen in
      let ctx_enc = init (get_cipher key) true in
      let ctx_dec = init (get_cipher key) false in
      let buf1 = encrypt ctx_enc buf len0 mlen in
      let buf2 = encrypt ctx_dec buf1 len0 mlen in
      if buf <> buf2 then failwith "bad encryption"
    done;
    control.Gc.verbose <- 0;
    Gc.set control;
    eprintf "\n";
    eprintf "OK.\n";
    
    
    eprintf "Encrypt_inplace.\n";
    let mlen = len_of_int 40 in
    for i=1 to 10 do
      let key = Security.key_of_buf (Buf.of_string "01234567012345670123456701234567") in
      let buf = Prng.create_buf mlen in
      let iov = Iovec.of_buf buf len0 (Buf.length buf) in
      let ctx_enc = init (get_cipher key) true in
      let ctx_dec = init (get_cipher key) false in
      encrypt_inplace ctx_enc iov;
      encrypt_inplace ctx_dec iov;
      let buf' = Iovec.buf_of iov in
      if  buf' <> buf then
	failwith "Bad encryption"
    done;
    eprintf "OK.\n";
    
    eprintf "Performance.\n";
    let time0 = Time.gettimeofday () in
    let len = 1000  in
    let buf = Buf.of_string (String.create len ) in 
    let iov = Iovec.of_buf buf len0 (Buf.length buf) in
    let len = len_of_int len in
    for i=1 to perf_loops do
      let context = init (get_cipher key) true in
      encrypt_inplace context iov;
    done;
    let time1 = Time.gettimeofday () in
    let diff = Time.sub time1 time0 in
    let diff = Time.to_string diff in
    printf "time for encryption (%d * %d) bytes = %s\n" perf_loops (Buf.int_of_len len)
      diff;

end
  
(**************************************************************)
  
  
module Sign  = struct
  type src = Buf.t
  type dst = Buf.t
      
  type context = Buf.t
  type digest  = Buf.t
      
  type t = {
    name : string ; 
    init : Buf.t -> context ;
    init_hack : context -> Buf.t -> unit ;
    update : context -> Buf.t -> ofs -> len -> unit ;
    final : context -> digest ;
    final_hack : context -> Buf.t -> ofs -> unit
  }
      
  let namea t = t.name
  let init t = t.init
  let init_hack t = t.init_hack 
  let update t = t.update 
  let final t = t.final
  let final_hack  t = t.final_hack
    
  let table = ref []
    
  let install name init init_hack update final final_hack = 
    let t = {
      name = name ;
      init = init ;
      init_hack = init_hack ; 
      update = update ;  
      final = final ; 
      final_hack = final_hack
    } in
    table := (name,t) :: !table
      
  let lookup name =
    let rec loop = function
      | [] -> raise (Failure ("Could not find signature algorithm <" ^ name ^ ">. This means that either you have not installed a cryptographic library, or, this algorithm is not supported by this library."))
      | (sign_name,t)::tl ->
	  if name = sign_name then t else loop tl
    in loop !table  
	 
	 
  let sign alg mac buf ofs len =
    let ctx = init alg (Security.buf_of_mac mac) in 
    update alg ctx buf ofs len ;
    let sign = final alg ctx in
    sign
      
  let sign_hack alg mac buf ofs len dst ofs = 
    let ctx = init alg (Security.buf_of_mac mac) in 
    update alg ctx buf ofs len ;
    final_hack alg ctx dst ofs
end
  
  
(**************************************************************)
  
module DH = struct
  (* The type of a long integer (arbitrary length). It is implemented
   * efficiently * by the OpenSSL library.
   *)
  type bignum
    
  (* A key contains four bignums: p,g,x,y. Where p is the modulo, g is
   * a generator for the group of integers modulo p, x is the private key, 
   * and y is the public key. y = p^x mod p.
   *)
  type key
    
    
  type t = {
    name                : string ;
    init                : unit -> unit ;
    generate_parameters : int -> key ;
    copy_params         : key -> key ;
    generate_key        : key -> unit ;
    get_p               : key -> bignum ;
    get_g               : key -> bignum ;
    get_pub             : key -> bignum ;
    get_prv             : key -> bignum ;
    key_of_string       : string -> key ;
    string_of_key       : key -> string ;
    string_of_bignum    : bignum -> string ;
    bignum_of_string    : string -> bignum ;
    compute_key         : key -> bignum -> string
  }
      
  let namea t = t.name

  let init t = t.init 
      
  let generate_parameters t = t.generate_parameters
      
  let copy_params t = t.copy_params
      
  let generate_key t = t.generate_key
      
  let get_p t = t.get_p 
      
  let get_g t = t.get_g 
      
  let get_pub t = t.get_pub
      
  let get_prv t = t.get_prv
      
  let key_of_string t = t.key_of_string
      
  let string_of_key t = t.string_of_key
      
  let string_of_bignum t = t.string_of_bignum
      
  let bignum_of_string t = t.bignum_of_string
      
  let compute_key t = t.compute_key


  let table = ref []
    
  let install name init generate_parameters copy_params generate_key 
    get_p get_g get_pub get_prv key_of_string string_of_key    
    string_of_bignum bignum_of_string compute_key        
    = 
    let t = {
      name                = name; 
      init                = init;
      generate_parameters = generate_parameters ;
      copy_params         = copy_params ;
      generate_key        = generate_key ;
      get_p               = get_p ;
      get_g               = get_g ;
      get_pub             = get_pub ;
      get_prv             = get_prv ;
      key_of_string       = key_of_string ;
      string_of_key       = string_of_key ;
      string_of_bignum    = string_of_bignum ;
      bignum_of_string    = bignum_of_string ;
      compute_key         = compute_key 
    } in
    table := (name,t) :: !table
      
  let lookup name =
    let rec loop = function
      | [] -> raise (Failure "Could not find a Diffie-Hellman implementation. This probably means that you have not installed a cryptographic library.")
      | (sign_name,t)::tl ->
	  if name = sign_name then t else loop tl
    in loop !table  
	 
  let show_params dh key = 
    let p = get_p dh key
    and g = get_g dh key in
    let p = string_of_bignum dh p 
    and g = string_of_bignum dh g in
    eprintf "p=%s\n" (hex_of_string p);
    eprintf "g=%s\n" (hex_of_string g);
    let priv = get_prv dh key in
    let pub = get_pub dh key in
    let priv = string_of_bignum dh priv in
    let pub = string_of_bignum dh pub in
    eprintf "priv=%s\n" (hex_of_string priv);
    eprintf "pub=%s\n" (hex_of_string pub)
      
      
  let fromFile dh keysize = 
    log (fun () -> sprintf "fromFile, keysize=%d" keysize);
    let ensroot = 
      try Unix.getenv "ENSROOT" 
      with Not_found -> 
	eprintf "Must have the ENSROOT enviroment variable defined, so 
I can find the Diffie-Hellman keys.\n";
	failwith "ENSROOT undefined"
    in
    let fName = sprintf "%s/crypto/OpenSSL/key%d" ensroot keysize in
    let fd = open_in fName in
    let len = in_channel_length fd in
    let s = String.create len in
    let len' = input fd s 0 len in
    if (len' <> len) then 
      failwith "Couldn't read the whole file";
    key_of_string dh s


  (* Run Diffie-Hellman implementation through a sanity check.
   *)
  let sanity_check alg num key_len = 
    let t = lookup alg in
    let init = init t
    and generate_parameters = generate_parameters t
    and copy_params = copy_params t
    and generate_key = generate_key t
    and get_p = get_p t
    and get_g = get_g t
    and get_pub  = get_pub t
    and get_prv  = get_prv t
    and key_of_string = key_of_string t
    and string_of_key = string_of_key t
    and string_of_bignum = string_of_bignum t
    and bignum_of_string = bignum_of_string t
    and compute_key      = compute_key t
    in
    
    init ();

    let create_key len = 
      let key = fromFile t len in
      log (fun () -> "generate_key");
      generate_key key;
      show_params t key;
      flush stdout;
      key
    in
    let key = create_key key_len in
    

    eprintf "Coping keys\n"; 
    let control = Gc.get () in
(*    control.Gc.verbose <- 1;
    Gc.set control;*)
    for i=0 to num do 
      let key_s = string_of_key key in
      let key' = key_of_string key_s in
      ()
    done;
    
    eprintf "Checking BIGNUMs\n";
    for i=0 to num do 
      let p = get_p key
      and g = get_p key
      and prv = get_prv key
      and pub = get_pub key in
      ()
    done;
    
    eprintf "Checking string<->BIGNUM \n";
    let pub = get_pub key in
    for i=0 to num do 
      let s = string_of_bignum pub in
      let key = bignum_of_string s in
      ()
    done;
(*    control.Gc.verbose <- 0;*)

    eprintf "Checking compute_key\n";
    let key_a = key 
    and key_b = create_key key_len in

    for i=0 to 50 do 
      eprintf ".";
      let pub_a = get_pub key_a
      and pub_b = get_pub key_b in
      let shared  = compute_key key_a pub_b
      and shared' = compute_key key_b pub_a in
      if shared <> shared' then 
	failwith "bad compute_key";
      let shared = bignum_of_string shared in
      let shared = string_of_bignum shared in
      if shared <> shared' then 
	failwith "bad string_of_bignum & compute_key";
      ()
    done;

    let pub_a = get_pub key_a
    and pub_b = get_pub key_b in
    let shared  = compute_key key_a pub_b
    and shared' = compute_key key_b pub_a in
    eprintf "\nshared=%s\n" (hex_of_string shared);


    eprintf "Performance.\n";
    let time0 = Time.gettimeofday () in
    for i=1 to 50 do
      eprintf ".";
      let pub_a = get_pub key_a
      and pub_b = get_pub key_b in
      let shared  = compute_key key_a pub_b
      and shared' = compute_key key_b pub_a in
      ()
    done;
    let time1 = Time.gettimeofday () in
    let diff = Time.to_float (Time.sub time1 time0) in
    printf "\ntime for %d diffie-hellman exponentiations (key_len=%d)= %f(sec)\n" 
      100 key_len (diff/.(2.0 *. 50.0)) ;

    ()

  let generate_new_parameters dh len = 
    let key = generate_parameters dh len in
    let p = get_p dh key
    and g = get_g dh key in
    let p = string_of_bignum dh p 
    and g = string_of_bignum dh g in
    eprintf "p=%s\n" (hex_of_string p);
    eprintf "g=%s\n" (hex_of_string g);
    let key = string_of_key dh key in
    log1 (fun () -> sprintf "toFile, keysize=%d, key=%s" len (hex_of_string key));
    let ensroot = Unix.getenv "ENSROOT" in
    let fName = sprintf "%s/crypto/OpenSSL/key%d" ensroot len in
    log1 (fun () -> sprintf "ensroot=%s" ensroot);
    log1 (fun () -> sprintf "fName=%s" fName);
    let fd = open_out fName in
    output_string fd key;
    flush fd;
    ()
    
end
(**************************************************************)
  
  
  
