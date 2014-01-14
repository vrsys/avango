(**************************************************************)
(* REKEY.ML : switching to a new key using tree structure *)
(* Authors: Zhen Xiao, Mark Hayden, 6/97 *)
(* Optimization: Ohad Rodeh 12/97  *)
(**************************************************************)
open Util
open Layer
open View
open Event
open Trans
open Shared
(**************************************************************)
let name = Trace.filel "REKEY"
(**************************************************************)

type header =
  | NoHdr
  | Rekey of bool 

(**************************************************************)

type state = {
  mutable father     : rank option;
  mutable children   : rank list ;
  mutable key_sug    : Security.key ;
  rekeying           : Once.t ;
  blocking           : Once.t ;
  mutable ack        : int ;
} 
	       
(**************************************************************)
	       
let dump = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "ack=%d\n" s.ack ;
  sprintf "blocking=%s, rekeying=%s\n"
    (Once.to_string s.blocking) (Once.to_string s.rekeying)
|])

(**************************************************************)

let init s (ls,vs) = 
  let degree = Param.int vs.params "rekey_degree" in
  let first_child n = degree * n + 1 in
  let father n = 
    if n<=0 then None
    else Some ((n-1)/degree)
  in
  let children =
    let rec loop rank degree =
      if (degree = 0) || (rank >= ls.nmembers) then []
      else rank :: (loop (succ rank) (degree - 1))
    in loop (first_child ls.rank) degree
  in 
  { 
  father    = father ls.rank;
  children  = children;
  key_sug   = Security.NoKey ;
  rekeying  = Once.create name ;
  blocking  = Once.create "blocking" ;
  ack       = 0 ;
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name "" in

  (* Encrypt and then send the key to all its children (except the father).
   *)
  let dis_key () =
    log (fun () -> sprintf "Distributing new key to my children: %s" 
	(string_of_int_list s.children)) ;    
    let key_sug = Security.buf_of_key s.key_sug in
    List.iter (fun rank ->
      let ev = (create name ESecureMsg[Peer rank; SecureMsg key_sug]) in
      dnlm ev NoHdr
    ) s.children
  in
  
  let enter_rekey () = 
    if not (Once.isset s.rekeying)
      && not (Once.isset s.blocking) then (
	Once.set s.rekeying;
	dis_key ();
	let key = s.key_sug in 
	dnnm (create name ERekeyPrcl[AgreedKey key]);
      )
  in

  let init_rekey () =
    let key = Prng.create_key () in
    s.key_sug <- key;
    log (fun () -> sprintf "init_rekey, generating a new key %s" (Security.string_of_key_short key));
    enter_rekey ()
  in
  
  let up_hdlr ev abv () = up ev abv
			    
  and uplm_hdlr ev hdr = match getType ev,hdr with
    
    (* Got a ticket from the parent.  
     *)
  | ESecureMsg,NoHdr ->
      let key = getSecureMsg ev in
      s.key_sug <- Security.key_of_buf key ;
      enter_rekey () ;
      free name ev
	
  | _ -> failwith unknown_local
	
  and upnm_hdlr ev = match getType ev with

  (* Start Rekey, from PERFREKEY.
   *)
  | ERekeyPrcl -> 
      assert (ls.rank = 0);
      init_rekey ()

  (* Nothing to commit in this layer. 
   *)
  | ERekeyCommit -> ()

  (* Cleanup the layer. This is an empty call for this layer. 
   * It is bounced down to PERFREKEY. 
   *)
  | ERekeyCleanup -> 
      dnnm ev

  (* EBlock: Mark that we are now blocking.
   *)
  | EBlock ->
      Once.set s.blocking ;
      upnm ev
	
  | EDump -> ( dump vf s ; upnm ev )
  | _ -> upnm ev
	
  and dn_hdlr ev abv = dn ev abv ()
			 
  and dnnm_hdlr = dnnm
		    
in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}
     
let l args vf = Layer.hdr init hdlrs None NoOpt args vf
		  
let _ = 
  Param.default "rekey_degree" (Param.Int 2) ;
  Layer.install name l

(**************************************************************)
    
