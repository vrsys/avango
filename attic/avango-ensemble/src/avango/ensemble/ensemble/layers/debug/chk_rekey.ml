(**************************************************************)
(* CHK_REKEY.ML *)
(* Author: Ohad Rodeh, 3/99 *)
(* Sanity test for the rekeying *)
(**************************************************************)
open Layer
open Trans
open Util
open View
open Event
(**************************************************************)
let name = Trace.filel "CHK_REKEY"
(**************************************************************)

type header = NoHdr | Key of Security.key 

type state = {
  degree             : int ;
  father             : rank option ;
  children           : rank list ;
  mutable rmt_keys   : Security.key list ;
  mutable my_new_key : Security.key option
} 

let init s (ls,vs) =   
  let degree = Param.int vs.params "chk_rekey_degree" in
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
  in { 
      degree     = degree ;
      father     = father ls.rank ;
      children   = children ;
      rmt_keys   = [] ;
      my_new_key = None
    }
       
(**************************************************************)
  
let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let log = Trace.log2 name "" in
  let log1 = Trace.log2 (name^"1") "" in
  let failwith m = failwith ("("^name^":"^m^")") in
  let num_children = List.length s.children in
  
  let check_recv_all () = 
    match s.my_new_key, s.children with
      | None,_ -> ()
      | Some my_key, [] -> (
	  match s.father with 
	    | None -> ()
	    | Some father -> dnlm (sendPeer name father) (Key my_key)
	)
      | Some my_key, _ -> 
	  if List.length s.rmt_keys = num_children then (
	    if List.for_all (function k -> k=my_key) s.rmt_keys then 
	      match s.father with 
		| None -> log (fun ()-> "All members have the same key")
		| Some father -> dnlm (sendPeer name father) (Key my_key)
	    else (
	      (* Something went wrong, print out a detailed
	       * report
	       *) 
	      printf "CHK_REKEY: Not all members have the same key\n";
	      let my_key = match s.my_new_key with 
		| None -> ""
		| Some key -> Security.string_of_key_short key
	      in
	      printf "rank=%d my_key=%s keys=%s\n" ls.rank my_key
		(string_of_list Security.string_of_key_short s.rmt_keys);
	      flush Pervasives.stdout;
	      exit 0;
	    )
	  )
  in
      
  let up_hdlr ev abv hdr = up ev abv
	  
  and uplm_hdlr ev hdr = match getType ev,hdr with
    | ESend _, Key rmt_key -> 
	log1 (fun () -> sprintf "recv key <- %d" (getPeer ev));
	s.rmt_keys <- rmt_key :: s.rmt_keys;
	check_recv_all () 

    | _ -> failwith unknown_local
		
  and upnm_hdlr ev = match getType ev with
    | EView -> 
	let vs_new = getViewState ev in
	s.my_new_key <- Some vs_new.key;
	check_recv_all ();
	upnm ev
	  
    | _ -> upnm ev
	  
  and dn_hdlr ev abv = dn ev abv NoHdr
	  
  and dnnm_hdlr ev = dnnm ev

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vf = Layer.hdr init hdlrs None NoOpt args vf

let _ = 
  Param.default "chk_rekey_degree" (Param.Int 4);
  Layer.install name l

(**************************************************************)




