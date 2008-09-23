// $Header$
// 
// Author:  Alexey Vaysburd, October 1997.
// 
// Contents:  "Replicated Updates" ORB Execution Style.
//
// 
// TODO:  With the current implementation, there is a single mutex for
//        all objects within an ORB.  This prevents concurrent invocation
//        of methods on different objects.  To fix, use reference counters.
//        An object may be unbound from an ORB only after all references 
//        have been released (make sure new references are not allowed once
//        started to unbind).

#include "avango/ensemble/maestro/Maestro_CSX.h"
#include "avango/ensemble/maestro/Maestro_Prim.h"
#include "avango/ensemble/maestro/Maestro_ORB.h"
#include "avango/ensemble/maestro/Maestro_ETC.h"
#include "avango/ensemble/maestro/Maestro_Adaptor.h"
#include "avango/ensemble/maestro/Maestro_ES_ReplicatedUpdates.h"
#include "avango/ensemble/maestro/Maestro_Perf.h"


/*************** Maestro_ES_ReplicatedUpdates_Options **************/ 

Maestro_ReplicatedUpdates_Options::Maestro_ReplicatedUpdates_Options() {
  // By default, allow progress in multiple partitions.
  // Maintain active replication within each partition
  // (total ordering, state transfer).
  progressInPrimaryOnly = 0;
  nReplicas = 0;
  stateTransfer = 1;
  requestsCommute = 0;
  reinstallIOR = 1;
  ORBName = "Maestro_ES_ReplicatedData";
  etc = &Maestro_DefaultEtc;
  hashSize = MAESTRO_ORB_HASH_SIZE;
}


/********************** Maestro_ES_ObjectState ********************/ 

int 
Maestro_ES_ObjectState::operator== (Maestro_ES_ObjectState &state) { 
  return (objKey == state.objKey); 
}

void 
Maestro_ES_ObjectState::operator>> (Maestro_Message &msg) { 
  msg << stateMsg << objKey;
}

void 
Maestro_ES_ObjectState::operator<< (Maestro_Message &msg) { 
  msg >> objKey >> stateMsg;
}


/********************** Maestro_ES_RUObjectEntry ********************/ 

Maestro_ES_RUObjectEntry::Maestro_ES_RUObjectEntry(
    Maestro_ES_RUObjectEntry &objEntry) {
  obj = objEntry.obj;
  key = objEntry.key;
}

Maestro_ES_RUObjectEntry::Maestro_ES_RUObjectEntry(
    Maestro_ORB_ObjectKey &_key,
    Maestro_RUObjectAdaptor *_obj) {
  obj = _obj;
  key = _key;
}

int 
Maestro_ES_RUObjectEntry::operator== (Maestro_ES_RUObjectEntry &objEntry) { 
  return (key == objEntry.key);
}


/********************** Maestro_ES_IIOPRequest ********************/ 

Maestro_ES_IIOPRequest::Maestro_ES_IIOPRequest(
      Maestro_ORB_RequestId &_reqId,
      Maestro_ORB_ObjectKey &_objKey,
      int _response_expected,
      Maestro_CORBA_String &_operation,
      Maestro_CORBA_Message &_reqBody) {
  reqId = _reqId;
  objKey = _objKey;
  response_expected = _response_expected;
  operation = _operation;
  reqBody = _reqBody;
}

void 
Maestro_ES_IIOPRequest::operator>> (Maestro_Message &msg) {
  perf_start(71, "reqId:");
  msg << reqId;
  perf_end(71);

  perf_start(72, "objKey:");
  msg << objKey;
  perf_end(72);

  perf_start(73, "response_expected:");
  msg << response_expected;
  perf_end(73);

  perf_start(74, "operation:");
  msg << operation;
  perf_end(74);

  perf_start(75, "reqBody:");
  msg << reqBody;
  perf_end(75);
}

void 
Maestro_ES_IIOPRequest::operator<< (Maestro_Message &msg) {
  perf_start(85, "reqBody:");
  msg >> reqBody;
  perf_end(85);

  perf_start(84, "operation:");
  msg >> operation;
  perf_end(84);

  perf_start(83, "response_expected:");
  msg >> response_expected;
  perf_end(83);

  perf_start(82, "objKey:");
  msg >> objKey;
  perf_end(82);

  perf_start(81, "reqId:");
  msg >> reqId;
  perf_end(81);
}


/********************** Maestro_ES_ReplicatedUpdates ********************/ 

// Design notes:
//
// - Local bindings of the ORB determine which objects receive replicated
//   updates coming via Ensemble.
// - Bindings with the ORB dispatcher determine which objects can receive
//   IIOP requests.
// * - All object replicas have the same object key.
// * - object key are published in *.key files, ior's in *.ior.
// * - pass a (ptr to Maestro_ETC) to Maestro_ES_ReplicatedData constructor.
// * - use protected xfer:  in stateTransfer_Callback, before calling getState,
//   send a "safe" p2p message to the coordinator -- include our host name +
//   ORB dispatcher's port # in use.
// * - in the view message, the coordinator includes all (host, port) pairs of
//   ORB dispatchers in the group.
// * - in AcceptedView_Callback's, the coordinator installs compound IOR's for all
//   enabled objects (using Maestro_ETC * pointer, if not NULL).
// * NB:  wizard:  ask if want to install ior's and whether to install them
// compound.  If not compound, don't need to send (host, port) of others to coord.
// If want to install ior's, ask for the ETC directory (ask whether the default
//  one is OK).

Maestro_ES_ReplicatedUpdates::Maestro_ES_ReplicatedUpdates(
    Maestro_ReplicatedUpdates_Options &_ops) {
  ops = _ops;
  bindings = new Maestro_ES_RUObjectEntryList[ops.hashSize];
  dispatcher = ops.dispatcher;
  active = 0;		       
  if (ops.reinstallIOR) {
    IIOPPort = dispatcher->port();
    char buf[256];
    memset(buf, 0, 256);
    gethostname(buf, 256);
    IIOPHost = buf;
    myProfile.port = IIOPPort;
    myProfile.host = IIOPHost;
  }
  
  Maestro_CSX_Options mops;    
  mops.mbrshipType = MAESTRO_SERVER;
  mops.xferType = (ops.stateTransfer || ops.reinstallIOR) ? 
    MAESTRO_ATOMIC_XFER : MAESTRO_NO_XFER;
  
  mops.groupName = ops.ORBName;
  
  mops.properties = "Gmp:Sync:Heal:Switch:Frag:Suspect";    
  mops.params = "";
  
  // Use Ensemble's primary layer if progress is allowed in 
  // the primary view only.
  if (ops.progressInPrimaryOnly) {
    mops.properties += ":Primary";
    char tmp[128];
    memset(tmp, 0, 128);
    if (mops.params.size > 1) {
      sprintf(tmp, ";");
      mops.params += tmp;
      memset(tmp, 0, 128);
    }
    sprintf(tmp, "primary_quorum=%d:int", ops.nReplicas);
    mops.params += tmp;
  }
  
  // If requests don't commute, use total ordering.
  if (!ops.requestsCommute) {
    mops.properties += ":Total";
  }

  //cerr << "PROPERTIES: " <<  mops.properties << endl;
  blocked = 1;

  // Initialize the group-member adaptor.
  mbr = new Maestro_AdaptorCSX(this, mops);
}

Maestro_ES_ReplicatedUpdates::~Maestro_ES_ReplicatedUpdates() { 
  deactivate(); 
  Maestro_Status res = unbindAll();
  assert(res == MAESTRO_OK);
  mutex.lock();
  delete mbr;
  delete [] bindings;
  mutex.unlock();
}

void 
Maestro_ES_ReplicatedUpdates::activate() {
  // No new objects will be allowed to bind while the ORB is active.
  mutex.lock();
  active = 1;
  mutex.unlock();
  
  // Join the group.
  mbr->join();
  
  // Register all currently bound objects with the ORB dispatcher.
  mutex.lock();
  int i, j;
  for (i = 0; i < ops.hashSize; i++) {
    for (j = 0; j < bindings[i].size(); j++) {
      Maestro_ES_RUObjectEntry *entry = &bindings[i][j];
      dispatcher->bind(entry->key, this);
    }
  }
  mutex.unlock();
}

void 
Maestro_ES_ReplicatedUpdates::deactivate() {
  mutex.lock();
  // No more requests will be processed (exceptions will be returned).
  active = 0;
  
  // Unregister all currently bound objects from the ORB Dispatcher.
  int i, j;
  for (i = 0; i < ops.hashSize; i++) {
    for (j = 0; j < bindings[i].size(); j++) {
      Maestro_ES_RUObjectEntry *entry = &bindings[i][j];
      dispatcher->unbind(entry->key);
    }
  }
  mutex.unlock();
  
  // Leave the group.
  mbr->leave();
}

Maestro_Status 
Maestro_ES_ReplicatedUpdates::bind(
    Maestro_ORB_ObjectKey &key,
    Maestro_RUObjectAdaptor *obj) {
  mutex.lock();
  // Can only be invoked while the ORB is inactive.
  if (active) {
    mutex.unlock();
    return Maestro_Error::create("bind: cannot bind when ORB is active");
  }
  
  // Add the object to the local list.  All objects will be disconnected
  // when the ORB is destructed.
  if (!key.isValid()) {
    mutex.unlock();
    return Maestro_Error::create("bind: invalid object key"); 
  } 
  if (obj == NULL) {
    mutex.unlock();
    return Maestro_Error::create("bind: invalid object adaptor (NULL)");
  }
  
  Maestro_ES_RUObjectEntry entry(key, obj);
  Maestro_CORBA_Long hash = key.hash() % ops.hashSize;

  Maestro_CORBA_OctetSequence seq;
  seq << key;
  // cout << "BIND: adding object with key of size " << seq.size() << endl;
  
  // Remove the old value if the key was already bound.
  bindings[hash] -= entry; 
  bindings[hash] += entry;
  mutex.unlock();
  
  return MAESTRO_OK;
}

Maestro_Status 
Maestro_ES_ReplicatedUpdates::unbind(Maestro_ORB_ObjectKey &key) {
  mutex.lock();
  // Can only be invoked while the ORB is inactive.
  if (active) {
    mutex.unlock();
    return Maestro_Error::create("bind: cannot bind when ORB is active");
  }
  
  // Remove the object from the local list.
  if (!key.isValid()) {
    mutex.unlock();
    return Maestro_Error::create("unbind: invalid object key"); 
  } 
  
  Maestro_ES_RUObjectEntry entry(key, NULL);
  Maestro_CORBA_Long hash = key.hash() % ops.hashSize;
  bindings[hash] -= entry; 
  mutex.unlock();
  
  return MAESTRO_OK;
}

Maestro_Status 
Maestro_ES_ReplicatedUpdates::unbindAll() {
  mutex.lock();
  // Can only be invoked while the ORB is inactive.
  if (active) {
    mutex.unlock();
    return Maestro_Error::create("bind: cannot bind when ORB is active");
  }
  
  // Remove all currently bound objects from the local list.
  int i;
  for (i = 0; i < ops.hashSize; i++) {
    bindings[i].clear();
  }
  mutex.unlock();
  return MAESTRO_OK;
}

/******************** Maestro_Communicator Callbacks ******************/

// Push the list of (host, port) pairs for all view members
// in the view msg.
void 
Maestro_ES_ReplicatedUpdates::viewMsg_Callback(
    Maestro_CSX_ViewData &viewData, 
    /*OUT*/ Maestro_Message &viewMsg) {

  //cerr << "CALLBACK: viewMsg_Callback (viewType = " 
  //     << viewData.viewType << ")\n";

  if ((viewData.viewType == MAESTRO_NO_XFER) &&
      ops.reinstallIOR) {
    int i;
    Maestro_ES_IIOPProfile prof;
    mutex.lock();
    for (i = 0; i < viewData.departedMembers.size(); i++) {
      prof.endp = viewData.departedMembers[i];

      //cerr << "*** removing profile of " << prof.endp << endl;
      int k;
      //cerr << "old list of profiles: " << endl;
      for (k = 0; k < IIOPProfiles.size(); k++) {
	//cerr << IIOPProfiles[k].endp << endl;
      }

      IIOPProfiles -= prof;

      //cerr << "updated list of profiles: " << endl;
      for (k = 0; k < IIOPProfiles.size(); k++) {
	//cerr << IIOPProfiles[k].endp << endl;
      }
    }
    
    // cerr << "Pushing IIOPProfiles on the view message..." << endl;
    viewMsg << IIOPProfiles;
    mutex.unlock();
  }
}
  
// * Update local group state.
// * Buffer all requests during state-transfer views.
// * Relay all buffered requests at this time if the view is normal(no xfer).
// * The coordinator creates a compound IOR for each registered object
//   whose application name is specified (in the object key) and stores 
//   it in the ETC directory (if ops.etc is not NULL).
// * Reinstall IOR only if the view is not an xfer view.
void 
Maestro_ES_ReplicatedUpdates::acceptedView_Callback(
    Maestro_CSX_ViewData& viewData,
    Maestro_Message &msg) {

  //cerr << "CALLBACK: acceptedView_Callback (" << viewData.nmembers << ")\n";
  //cerr << "(viewType = " << viewData.viewType << ")\n";


  view = viewData;

  mutex.lock();
  blocked = 0;
  mbr->myEndpID(myEndpID);

  if (view.viewType == MAESTRO_NO_XFER) {
    // 
    // Relay buffered IIOP requests to all other replicas.
    //      
    if (bufferedReqs.size()) {
      Maestro_Message relay;
      relay << bufferedReqs;
      mbr->scast(relay);

      // If total ordering is not used, there may be no local delivery,
      // so invoke local object's method directly.
      if (ops.requestsCommute) {
	int i;
	for (i = 0; i < bufferedReqs.size(); i++) {
	  Maestro_ES_IIOPRequest &req = bufferedReqs[i];
	  Maestro_RUObjectAdaptor *obj = lookup(req.objKey);
	  Maestro_CORBA_Message replyBody;
	  if (obj == NULL) {
	    // If object is not registered, return an exception.
	    Maestro_CORBA_String exc_name("SystemException");
	    Maestro_CORBA_Exception exc(
		       exc_name, 
		       MAESTRO_CORBA_EXCEPTION_CODE_INV_OBJREF,
		       MAESTRO_CORBA_COMPLETION_STATUS_NO);
	    replyBody << exc;
	    dispatcher->requestReply(
		       req.reqId, 
		       MAESTRO_GIOP_REPLY_STATUS_SYSTEM_EXCEPTION,
		       replyBody);
	  }
	  else {
	    Maestro_GIOP_ReplyStatusType status = 
	      obj->update(req.operation, req.reqBody, replyBody);
	    
	    // Compose an IIOP reply + invoke ORB dispatcher.
	    if (req.response_expected) {
	      dispatcher->requestReply(req.reqId, status, replyBody);
	    }
	  }
	}
      }
      bufferedReqs.clear();
    }
    
    if (ops.reinstallIOR && 
	(view.viewType == MAESTRO_NO_XFER) &&
	(ops.etc != NULL)) {
      
      // cerr << "Reading IIOPProfiles from the view message..." << endl;

      msg >> IIOPProfiles;
      myProfile.endp = view.myEndpID;
      IIOPProfiles += myProfile;

      int k;
      //cerr << "Current list of profiles: " << endl;
      for (k = 0; k < IIOPProfiles.size(); k++) {
	//cerr << IIOPProfiles[k].endp << endl;
      }

      if (view.coordinator == myEndpID) {
	// 
	// Make compound IOR's for all registered objects 
	// and reinstall in etc.
	//
	int i, j;
	for (i = 0; i < ops.hashSize; i++) {
	  for (j = 0; j < bindings[i].size(); j++) {
	    Maestro_ES_RUObjectEntry &entry = bindings[i][j];
	    char *appName = entry.key.appName();
	    if (appName == NULL) {
	      continue;
	    }
	    Maestro_IOP_TaggedProfileList profList;
	    profList.size(IIOPProfiles.size());
	    int k;
	    // 
	    // Make a compound IIOP profile for the object...
	    //
	    for (k = 0; k < IIOPProfiles.size(); k++) {
	      Maestro_ES_IIOPProfile &prof = IIOPProfiles[k];

	      //cerr << "Making compound IOR: profile " << k
	      //   << " = " << prof.endp << endl;

	      Maestro_CORBA_OctetSequence keySeq;
	      keySeq << entry.key;

	      Maestro_IIOP_ProfileBody pbody(prof.host, prof.port, keySeq);
	      Maestro_CORBA_OctetSequence pbody_encaps;
	      pbody.writeTo(pbody_encaps);
	      Maestro_IOP_TaggedProfile profile(
                        MAESTRO_IOP_TAG_INTERNET_IOP, 
			pbody_encaps);
	      profList[k] = profile;
	    }
	    
	    // 
	    // Store the profile in etc as an IOR...
	    //
	    int bufSize = strlen(appName) + 10;
	    char *buf = new char[bufSize];
	    memset(buf, 0, bufSize);
	    //
	    // CORBA standard for app. interface names:
	    // "IDL:<intf-name>:<version>"
	    //
	    sprintf(buf, "IDL:%s:1.0", appName);
	    Maestro_CORBA_String typeId(buf);
	    Maestro_IOP_IOR ior(typeId, profList);	    
	    Maestro_CORBA_String repres;
	    ior >> repres;
	    memset(buf, 0, bufSize);
	    //
	    // Convention: IOR's are stored in files <appname>.ior  
	    //
	    sprintf(buf, "%s.ior", appName);
	    Maestro_String fname(buf);
	    ops.etc->install(fname, repres);
	    delete [] buf;
	  }
	}
      }
    }
  }
  mutex.unlock();
}
  
// Dispatch request to the destination object.
// Send an IIOP reply only if scast origin = this member.
void 
Maestro_ES_ReplicatedUpdates::receiveScast_Callback(
    Maestro_EndpID &origin, 
    Maestro_Message &msg) {

  perf_end(14);

  //cerr << "CALLBACK: receiveScast_Callback" << endl;

  // If there is local delivery and total ordering not used,
  // ignore the local-delivery msg to avoid double invocation.    
  if (myEndpID == origin && ops.requestsCommute) {
    return;
  }
  
  perf_start(15, "extracting requests to be pocessed:");

  Maestro_ES_IIOPRequestList reqList;

  perf_start(65, "unmarshaling reqList:");
  msg >> reqList;
  perf_end(65);
  
  int i;
  mutex.lock();
  assert(view.viewType == MAESTRO_NO_XFER);

  perf_end(15);

  //cerr << "++++ receiveScast_Callback:  got " 
  //     << reqList.size() << " requests\n";
  
  for (i = 0; i < reqList.size(); i++) {

    perf_start(16, "processing request:");

    Maestro_ES_IIOPRequest &req = reqList[i];
    Maestro_RUObjectAdaptor *obj = lookup(req.objKey);
    if (obj == NULL) {
      //cerr << "receiveScast_Callback: object not found!!\n";
      continue;
    }
    Maestro_CORBA_Message replyBody;
    
    perf_start(17, "cost of obj->update:");

    // Apply IIOP request to the local object.
    // cerr << "Calling the object..." << endl;
    Maestro_GIOP_ReplyStatusType status = 
      obj->update(req.operation, req.reqBody, replyBody);
    // cerr << "...Returned from the call." << endl;

    perf_end(17);

    // Send an IIOP reply only if original IIOP request
    // was received by this member.
    if (origin == myEndpID && req.response_expected) {
      // cerr << "sending IIOP requestReply..." << endl;

      perf_start(18, "cost of sending reply:");

      dispatcher->requestReply(req.reqId, status, replyBody);

      perf_end(18);

      // cerr << "...done sending IIOP requestReply." << endl;
    }
    // else cerr << "*NOT* sending IIOP requestReply!!!" << endl;

    perf_end(16);
  }
  mutex.unlock();
}
  
// The coordinator receives messages from all members doing state
// transfer, with (host, port) data.  
// This info is used when view_msg callback is invoked:
// the coordinator sends a list of (host, port) pairs with the view msg.
void 
Maestro_ES_ReplicatedUpdates::receiveSend_Callback(
    Maestro_EndpID &origin, 
    Maestro_Message &msg) {

  //cerr << "CALLBACK: receiveSend_Callback" << endl;

  Maestro_ES_IIOPProfile prof;
  msg >> prof;
  mutex.lock();
  IIOPProfiles += prof;
  mutex.unlock();
}
  
// Update local group state 
// (buffer requests while the group is blocked)
void 
Maestro_ES_ReplicatedUpdates::block_Callback() {

  //cerr << "CALLBACK: block_Callback" << endl;

  mutex.lock();
  blocked = 1;
  mutex.unlock();
}
    
// Go over all objects currently bound with this
// ORB (NB:  use mutex to make sure disable() etc. are not invoked
//      at this point).  For each object, get its state.  Push all states
// into a Maestro_Message.  Send the message with a sendState method of mbr.  
// To send state:  get state from all currently bound objects,
// and push into msg as (key, msg) pairs.
void 
Maestro_ES_ReplicatedUpdates::askState_Callback(
    Maestro_EndpID &origin, 
    Maestro_XferID &xferID,
    Maestro_Message &requestMsg) {

  //cerr << "CALLBACK: askState_Callback" << endl;

  Maestro_ES_ObjectStateList objectStates;
  
  // Go over all registered objects, invoking their pushState methods
  // and collecting their states on the list.
  mutex.lock();
  int i, j;
  for (i = 0; i < ops.hashSize; i++) {
    for (j = 0; j < bindings[i].size(); j++) {
      Maestro_ES_RUObjectEntry &entry = bindings[i][j];
      assert(entry.obj != NULL);
      Maestro_ES_ObjectState state; 
      state.objKey = entry.key;
      entry.obj->pushState(state.stateMsg);
      objectStates += state;
    }
  }
  mutex.unlock();
  
  // Marshal the list of states and send to the request origin.
  Maestro_Message stateMsg;
  stateMsg << objectStates;
  mbr->sendState(origin, xferID, stateMsg);
}

// Use async state transfer.  
void 
Maestro_ES_ReplicatedUpdates::stateTransfer_Callback(Maestro_XferID &xferID) {

  //cerr << "CALLBACK: stateTransfer_Callback" << endl;

  // Before calling async getState, send a "safe" p2p message 
  // to the coordinator -- include our host name + ORB dispatcher's port.
  if (ops.reinstallIOR) {
    Maestro_Message profMsg;
    profMsg << myProfile;
    Maestro_ClSv_MsgOptions mops;
    mops.msgXferSafety = MAESTRO_MSG_XFER;
    Maestro_MsgSendView sendView;
    mbr->send(view.coordinator, profMsg, sendView, mops);
  }

  // Mske async state transfer request. 
  if (ops.stateTransfer) {
    Maestro_Message xferRequest;
    Maestro_XferStatus xferStatus;
    mbr->getState(xferID, xferRequest, xferStatus);
  }
  else {
    mbr->xferDone(xferID);
  }
} 

// Go over all objects whose state is included in the
// sate msg.  Update their state (invoke getState methods).  
void 
Maestro_ES_ReplicatedUpdates::gotState_Callback(
    Maestro_XferID &xferID,
    Maestro_Message &stateMsg) {

  //cerr << "CALLBACK: gotState_Callback" << endl;

  // Retrieve all states packed in the stateMsg; 
  // invoke getState on corresp. objects (those which are registered).
  Maestro_ES_ObjectStateList objectStates;
  stateMsg >> objectStates;
  int i;
  mutex.lock();
  for (i = 0; i < objectStates.size(); i++) {
    // Lookup the object with corresp. key;
    // invoke obj->getState().
    Maestro_ES_ObjectState &objState = objectStates[i];
    Maestro_RUObjectAdaptor *obj = lookup(objState.objKey);
    if (obj != NULL) {
      obj->getState(objState.stateMsg);
    }
  }
  mutex.unlock();
  mbr->xferDone(xferID);
}

/******************** Maestro_ORB_Base Callbacks ******************/

void 
Maestro_ES_ReplicatedUpdates::request_Callback(
    Maestro_ORB_RequestId &reqId,
    Maestro_ORB_ObjectKey &objKey,
    Maestro_CORBA_Boolean response_expected,
    Maestro_CORBA_String &operation,
    Maestro_GIOP_Principal &requesting_principal,
    Maestro_CORBA_Message &reqBody) {

  //cerr << "CALLBACK: request_Callback" << endl;

  mutex.lock();
  if (!active) {
    mutex.unlock();
    // Return an exception if the ORB is not active.
    Maestro_CORBA_String exc_name("SystemException");
    Maestro_CORBA_Exception exc(
            exc_name, 
	    MAESTRO_CORBA_EXCEPTION_CODE_INV_OBJREF,
	    MAESTRO_CORBA_COMPLETION_STATUS_NO);
    Maestro_CORBA_Message reply;
    reply << exc;
    dispatcher->requestReply(
            reqId, 
	    MAESTRO_GIOP_REPLY_STATUS_SYSTEM_EXCEPTION,
	    reply);
    return;
  }

  // Return an exception if progress is allowed in the primary view 
  // only and the current view is not primary.
  if (ops.progressInPrimaryOnly && !view.primaryFlag) {
     mutex.unlock();
     Maestro_CORBA_String exc_name("SystemException");
     Maestro_CORBA_Exception exc(
            exc_name, 
	    MAESTRO_CORBA_EXCEPTION_CODE_NO_PERMISSION,
	    MAESTRO_CORBA_COMPLETION_STATUS_NO);
     Maestro_CORBA_Message reply;
     reply << exc;
     dispatcher->requestReply(
            reqId, 
	    MAESTRO_GIOP_REPLY_STATUS_SYSTEM_EXCEPTION,
	    reply);
     return;
  }

  perf_start(11, "object lookup:");
  Maestro_RUObjectAdaptor *obj = lookup(objKey);
  perf_end(11);

  if (obj == NULL) {
    mutex.unlock();
    // Return an exception if the object is not currently bound to the ORB
    Maestro_CORBA_String exc_name("SystemException");
    Maestro_CORBA_Exception exc(
            exc_name, 
	    MAESTRO_CORBA_EXCEPTION_CODE_INV_OBJREF,
	    MAESTRO_CORBA_COMPLETION_STATUS_NO);
    Maestro_CORBA_Message reply;
    reply << exc;
    dispatcher->requestReply(
            reqId, 
	    MAESTRO_GIOP_REPLY_STATUS_SYSTEM_EXCEPTION,
	    reply);
    return;
  }


  perf_start(12, "making tmp request struct:");

  // Use a temporary message since reqBody is not writable.
  // (A message must be writable if it is to be serialized).
  unsigned pos = reqBody.pos();
  unsigned size = reqBody.size() - pos;
  char *data = (char*) reqBody.body();
  int pad = 4 - ((unsigned) data + pos) % 4;
  if (pad < 4) 
    data += pad;
  Maestro_CORBA_Message tmpMsg;
  tmpMsg.write(data + pos, size);
  reqBody.pos(pos);

  Maestro_ES_IIOPRequest req(reqId, 
			     objKey, 
			     response_expected, 
			     operation, 
			     tmpMsg);
  perf_end(12);
  
  // Buffer requests while the group is blocked or is a state-transfer view.
  if (blocked || (view.viewType != MAESTRO_NO_XFER)) {
    bufferedReqs += req;
  }
  // Otherwise relay to all other replicas.
  else {

    perf_start(19, "cost of packaging relayed requests into msg:");
    Maestro_Message relay;
    Maestro_ES_IIOPRequestList reqList(req);

    perf_start(61, "marshaling reqList into relay message:");
    relay << reqList;
    perf_end(61);

    perf_end(19);

    perf_start(14, "latency of local delivery of request:");    

    perf_start(13, "cost of relaying request (may include context switch):");

    // Instead of relaying, invoke locally (receiveScast_Callback).
    // mbr->scast(relay);
    mutex.unlock();
    receiveScast_Callback(myEndpID, relay);
    mutex.lock();

    perf_end(13);

    // If total ordering is not used, there may be no local delivery,
    // so invoke local object's method directly.
    if (ops.requestsCommute) {
      Maestro_CORBA_Message replyBody;
      Maestro_GIOP_ReplyStatusType status = 
	obj->update(operation, reqBody, replyBody);

      // Compose an IIOP reply + invoke ORB dispatcher.
      if (req.response_expected) {
	dispatcher->requestReply(reqId, status, replyBody);
      }
    }
  }
  mutex.unlock();
}

void 					       
Maestro_ES_ReplicatedUpdates::locateRequest_Callback(
        Maestro_ORB_RequestId &reqId,
	Maestro_ORB_ObjectKey &objKey) {

  //cerr << "Maestro_ES_ReplicatedUpdates::locateRequest_Callback" << endl;

  // See if the object is currently bound to this orb
  // and reply correspondingly.
  mutex.lock();
  Maestro_RUObjectAdaptor *obj = lookup(objKey);
  mutex.unlock();
  Maestro_GIOP_LocateStatusType locate_status = (obj == NULL) ? 
    MAESTRO_GIOP_LOCATE_STATUS_UNKNOWN_OBJECT :
    MAESTRO_GIOP_LOCATE_STATUS_OBJECT_HERE;
  dispatcher->locateReply(reqId, locate_status);
}

// Lookup the (key, adaptor) binding in the ORB.  
// Return NULL if not found.
// The ORB must be locked at this point.
Maestro_RUObjectAdaptor* 
Maestro_ES_ReplicatedUpdates::lookup(Maestro_ORB_ObjectKey &objKey) {
  if (!objKey.isValid()) {
    return NULL;
  }

  Maestro_RUObjectAdaptor *obj = NULL;
  Maestro_ES_RUObjectEntry entry(objKey, NULL);
  int hash = objKey.hash() % ops.hashSize;
  Maestro_ES_RUObjectEntryList &entryList = bindings[hash];

  int i;
  for (i = 0; i < entryList.size(); i++) {
    if (entryList[i] == entry) {
      obj = entryList[i].obj;
      break;
    }
  }

  return obj;
}


/******************** Maestro_RUObjectAdaptor *******************/ 

Maestro_RUObjectAdaptor::Maestro_RUObjectAdaptor(
    Maestro_RUObjectAdaptor_Options &ops) { 
  assert(ops.orb != NULL);
  orb = ops.orb;
  key = ops.key;
  Maestro_Status ret = orb->bind(key, this);
  assert(ret == MAESTRO_OK);
}

Maestro_RUObjectAdaptor::~Maestro_RUObjectAdaptor() {
  Maestro_Status ret = orb->unbind(key);
  assert(ret == MAESTRO_OK);
}
