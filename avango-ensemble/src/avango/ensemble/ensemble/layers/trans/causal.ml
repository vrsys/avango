(**************************************************************)
(* CAUSAL.ML : Causal ordering layer. *)
(* Author: Ohad Rodeh, 7/97 *)
(**************************************************************)
(* This layer implements causal ordering. It uses supporting
 * functions from the Mcausal module. It can thus use either
 * complete causal vectors or compressed vectors. The compression
 * is done Transis style.
 * 
 * If A,B,C are in view V, and A receives messages B1,B2,C1,C2, 
 * then sends A1, then a complete vector would be:
 *      A B C
 *    [|1;2;2|]
 * The compressed version would be:
 *    [B2;C2]
 * If B2 already acknowledges C2 then the compressed version would 
 * be just:
 *    [B2]
 *)
(**************************************************************)
open Trans
open Layer
open Event
open Util
open View
(*************************************************************)
let name = Trace.filel "CAUSAL"
(**************************************************************)

type header = NoHdr | Causal of Mcausal.s

type 'abv state = {
  delay    : (Event.t * 'abv * Mcausal.s) Queuee.t array; (* Buffers of delayed msgs *)
  mutable ndelayed : int;                        (* Total number delayed messages *)
  dag       : Mcausal.t                       (* Causally received seqno per member *)
}

(**************************************************************)

let dump vf s = Layer.layer_dump name (fun (ls,vs) s -> [|
  sprintf "ndelayed=%d last=%s\n" 
    s.ndelayed (string_of_int_array (Mcausal.last_of s.dag))
|]) vf s

(**************************************************************)

let init _ (ls,vs) = {
  delay    = Array.init ls.nmembers (fun _ -> Queuee.create ()) ;
  ndelayed = 0;
  dag      = Mcausal.create_state ls.nmembers
}

(**************************************************************)

let hdlrs s ((ls,vs) as vf) {up_out=up;upnm_out=upnm;dn_out=dn;dnlm_out=dnlm;dnnm_out=dnnm} =
  let failwith = layer_fail dump vf s name in
  let log = Trace.log2 name ls.name in
  let logb = Trace.log3 Layer.buffer name ls.name in

  (* Check if messages in the queues became deliverable. 
   * perform a recursive check on all the messages.
   *)
  let try_deliver () = 
    let advance = ref true in
    while s.ndelayed > 0 && !advance do 
      advance := false;
      Array.iteri (fun i q -> 
	if not (Queuee.empty q) then 
	  begin
	    let (ev, abv, ack_vct) = Queuee.peek q in
	    if Mcausal.can_deliver s.dag ack_vct i then
	      begin
    	      	Mcausal.update s.dag ack_vct i;
	      	s.ndelayed <- pred s.ndelayed;
	      	advance := true;
		up ev abv;
	      	ignore (Queuee.take q)
	      end
	  end
      ) s.delay ;
    done
  in

  let up_hdlr ev abv hdr = match getType ev, hdr with
  (* UpCast:Data: Got a data message.
   *)
  | ECast _ , (Causal ack_strct) -> 
      let origin = getPeer ev in
      if (Queuee.empty s.delay.(origin)) 
      && Mcausal.can_deliver s.dag ack_strct origin 
      then
      (* Message is causally deliverable. Try to deliver messages
       * from the queues.
       *)
	begin
	  up ev abv;
    	  Mcausal.update s.dag ack_strct origin;
      	  try_deliver ();
	end
      else
      (* Message is not causally deliverable; queue it.
       *)
      	begin
	  Queuee.add (ev, abv, ack_strct) s.delay.(origin);
	  s.ndelayed <- succ s.ndelayed;
      	end

  | _  -> up ev abv 

  and uplm_hdlr ev hdr = match getType ev,hdr with
  | _ -> failwith unknown_local

  and upnm_hdlr ev = match getType ev with
  | EAccount ->
      (* Dump information about status of buffers.
       *)
      let qa = Array.map (fun q -> Queuee.length q) s.delay in
      logb (fun () -> sprintf "Queued=%d msgs=%s" s.ndelayed (string_of_int_array qa));
      logb (fun () -> sprintf "causally received=%s" 
	(string_of_int_array (Mcausal.last_of s.dag))); 
      upnm ev  
  | EDump -> ( dump vf s ; upnm ev ) 
  | _ -> upnm ev 

  and dn_hdlr ev abv = match getType ev with
  | ECast _ when not (getNoTotal ev) ->
      (* Update my state and sent and acknowledgment list/vector.
       *)
      let last = Mcausal.incr s.dag ls.rank in
      dn ev abv (Causal last)
  |  _ -> dn ev abv NoHdr

  and dnnm_hdlr = dnnm

in {up_in=up_hdlr;uplm_in=uplm_hdlr;upnm_in=upnm_hdlr;dn_in=dn_hdlr;dnnm_in=dnnm_hdlr}

let l args vs = Layer.hdr init hdlrs None NoOpt args vs

let _ = Layer.install name l

(**************************************************************)
