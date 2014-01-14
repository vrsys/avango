// $Header$
// 
// Author:  Alexey Vaysburd, October 1997.
// 
// Contents:  Simple ORB Execution Style.

#include "avango/ensemble/maestro/Maestro_ORB.h"
#include "avango/ensemble/maestro/Maestro_ETC.h"
#include "avango/ensemble/maestro/Maestro_ES_Simple.h"


//#define PERFTEST
#ifdef PERFTEST
extern unsigned long total_latency3;
extern unsigned long ncalls3;
extern struct timeval before3, after3;

extern unsigned long total_latency4;
extern unsigned long ncalls4;
extern struct timeval before4, after4;

extern unsigned long total_latency5;
extern unsigned long ncalls5;
extern struct timeval before5, after5;

extern unsigned long total_latency6;
extern unsigned long ncalls6;
extern struct timeval before6, after6;

extern unsigned long total_latency7;
extern unsigned long ncalls7;
extern struct timeval before7, after7;
#endif

/*************** Maestro_ES_SimpleORB_Options **************/ 

Maestro_SimpleORB_Options::Maestro_SimpleORB_Options() {
  installIOR = 1;
  ORBName = "Maestro_ES_SimpleORB";
  etc = &Maestro_DefaultEtc;
  hashSize = MAESTRO_ORB_HASH_SIZE;
}


/********************** Maestro_ES_SimpleObjectEntry ********************/ 

Maestro_ES_SimpleObjectEntry::Maestro_ES_SimpleObjectEntry(
    Maestro_ES_SimpleObjectEntry &objEntry) {
  obj = objEntry.obj;
  key = objEntry.key;
}

Maestro_ES_SimpleObjectEntry::Maestro_ES_SimpleObjectEntry(
    Maestro_ORB_ObjectKey &_key,
    Maestro_SimpleORBObjectAdaptor *_obj) {
  obj = _obj;
  key = _key;
}

int 
Maestro_ES_SimpleObjectEntry::operator== (Maestro_ES_SimpleObjectEntry &objEntry) { 
  return (key == objEntry.key);
}


/********************** Maestro_ES_SimpleORB ********************/ 

Maestro_ES_SimpleORB::Maestro_ES_SimpleORB(
    Maestro_SimpleORB_Options &_ops) {
  ops = _ops;
  bindings = new Maestro_ES_SimpleObjectEntryList[ops.hashSize];
  dispatcher = ops.dispatcher;
  active = 0;		

  IIOPport = dispatcher->port();
  char buf[256];
  memset(buf, 0, 256);
  gethostname(buf, 256);
  IIOPhost = buf;
}

Maestro_ES_SimpleORB::~Maestro_ES_SimpleORB() { 
  deactivate(); 
  Maestro_Status res = unbindAll();
  assert(res == MAESTRO_OK);
  mutex.lock();
  delete [] bindings;
  mutex.unlock();
}

void 
Maestro_ES_SimpleORB::activate() {
  // No new objects will be allowed to bind while the ORB is active.
  mutex.lock();
  active = 1;
  mutex.unlock();
  
  // Register all currently bound objects with the ORB dispatcher.
  mutex.lock();
  int i, j;
  for (i = 0; i < ops.hashSize; i++) {
    for (j = 0; j < bindings[i].size(); j++) {
      Maestro_ES_SimpleObjectEntry *entry = &bindings[i][j];
      dispatcher->bind(entry->key, this);
    }
  }
  mutex.unlock();
}

void 
Maestro_ES_SimpleORB::deactivate() {
  mutex.lock();
  // No more requests will be processed (exceptions will be returned).
  active = 0;
  
  // Unregister all currently bound objects from the ORB Dispatcher.
  int i, j;
  for (i = 0; i < ops.hashSize; i++) {
    for (j = 0; j < bindings[i].size(); j++) {
      Maestro_ES_SimpleObjectEntry *entry = &bindings[i][j];
      dispatcher->unbind(entry->key);
    }
  }
  mutex.unlock();
}

Maestro_Status 
Maestro_ES_SimpleORB::bind(
    Maestro_ORB_ObjectKey &key,
    Maestro_SimpleORBObjectAdaptor *obj) {
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
  
  Maestro_ES_SimpleObjectEntry entry(key, obj);
  Maestro_CORBA_Long hash = key.hash() % ops.hashSize;

  Maestro_CORBA_OctetSequence seq;
  seq << key;
  // cout << "BIND: adding object with key of size " << seq.size() << endl;
  
  // Remove the old value if the key was already bound.
  bindings[hash] -= entry; 
  bindings[hash] += entry;

  // Install the object's IOR in the ETC directory.
  installIOR(key);

  mutex.unlock();
  
  return MAESTRO_OK;
}

Maestro_Status 
Maestro_ES_SimpleORB::unbind(Maestro_ORB_ObjectKey &key) {
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
  
  Maestro_ES_SimpleObjectEntry entry(key, NULL);
  Maestro_CORBA_Long hash = key.hash() % ops.hashSize;
  bindings[hash] -= entry; 
  mutex.unlock();
  
  return MAESTRO_OK;
}

Maestro_Status 
Maestro_ES_SimpleORB::unbindAll() {
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


/******************** Maestro_ORB_Base Callbacks ******************/

void 
Maestro_ES_SimpleORB::request_Callback(
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


#ifdef PERFTEST
  gettimeofday(&before5, NULL);
#endif
  
  Maestro_SimpleORBObjectAdaptor *obj = lookup(objKey);

  
#ifdef PERFTEST
  gettimeofday(&after5, NULL);
  ncalls5++;
  total_latency5 += (after5.tv_sec - before5.tv_sec) * 1000000 +
    (after5.tv_usec - before5.tv_usec);
#endif

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


#ifdef PERFTEST
  gettimeofday(&before6, NULL);
#endif


#ifdef PERFTEST
  gettimeofday(&after6, NULL);
  ncalls6++;
  total_latency6 += (after6.tv_sec - before6.tv_sec) * 1000000 +
    (after6.tv_usec - before6.tv_usec);
#endif
  
  // Invoke local object's method directly.
  Maestro_CORBA_Message replyBody;

#ifdef PERFTEST
  gettimeofday(&before4, NULL);
#endif

  Maestro_GIOP_ReplyStatusType status = 
    obj->update(operation, reqBody, replyBody);
      
#ifdef PERFTEST
  gettimeofday(&after4, NULL);
  ncalls4++;
  total_latency4 += (after4.tv_sec - before4.tv_sec) * 1000000 +
    (after4.tv_usec - before4.tv_usec);
#endif

  // Compose an IIOP reply + invoke ORB dispatcher.
  if (response_expected) {
    
#ifdef PERFTEST
    gettimeofday(&before3, NULL);
#endif
    
    dispatcher->requestReply(reqId, status, replyBody);
    
#ifdef PERFTEST
    gettimeofday(&after3, NULL);
    ncalls3++;
    total_latency3 += (after3.tv_sec - before3.tv_sec) * 1000000 +
      (after3.tv_usec - before3.tv_usec);
#endif
    
  }
  mutex.unlock();
}

void 					       
Maestro_ES_SimpleORB::locateRequest_Callback(
        Maestro_ORB_RequestId &reqId,
	Maestro_ORB_ObjectKey &objKey) {

  //cerr << "Maestro_ES_SimpleORB::locateRequest_Callback" << endl;

  // See if the object is currently bound to this orb
  // and reply correspondingly.
  mutex.lock();
  Maestro_SimpleORBObjectAdaptor *obj = lookup(objKey);
  mutex.unlock();
  Maestro_GIOP_LocateStatusType locate_status = (obj == NULL) ? 
    MAESTRO_GIOP_LOCATE_STATUS_UNKNOWN_OBJECT :
    MAESTRO_GIOP_LOCATE_STATUS_OBJECT_HERE;
  dispatcher->locateReply(reqId, locate_status);
}

// Lookup the (key, adaptor) binding in the ORB.  
// Return NULL if not found.
// The ORB must be locked at this point.
Maestro_SimpleORBObjectAdaptor* 
Maestro_ES_SimpleORB::lookup(Maestro_ORB_ObjectKey &objKey) {
  if (!objKey.isValid()) {
    return NULL;
  }

  Maestro_SimpleORBObjectAdaptor *obj = NULL;

#ifdef PERFTEST
  gettimeofday(&before7, NULL);
#endif

  Maestro_ES_SimpleObjectEntry entry(objKey, NULL);

#ifdef PERFTEST
  gettimeofday(&after7, NULL);
  ncalls7++;
  total_latency7 += (after7.tv_sec - before7.tv_sec) * 1000000 +
    (after7.tv_usec - before7.tv_usec);
#endif

  int hash = objKey.hash() % ops.hashSize;
  Maestro_ES_SimpleObjectEntryList &entryList = bindings[hash];

  int i;
  for (i = 0; i < entryList.size(); i++) {
    if (entryList[i] == entry) {
      obj = entryList[i].obj;
      break;
    }
  }

  return obj;
}


void
Maestro_ES_SimpleORB::installIOR(Maestro_ORB_ObjectKey &key) {
  if (!ops.installIOR || ops.etc == NULL) {
    return;
  }

  char *appName = key.appName();
  if (appName == NULL) {
    return;
  }
   
  Maestro_IOP_TaggedProfileList profList;
  profList.size(1);
  
  // 
  // Make an IIOP profile for the object...
  //
  Maestro_CORBA_OctetSequence keySeq;
  keySeq << key;
      
  Maestro_IIOP_ProfileBody pbody(IIOPhost, IIOPport, keySeq);
  Maestro_CORBA_OctetSequence pbody_encaps;
  pbody.writeTo(pbody_encaps);
  Maestro_IOP_TaggedProfile profile(
				    MAESTRO_IOP_TAG_INTERNET_IOP, 
				    pbody_encaps);
  profList[0] = profile;
  
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


/******************** Maestro_SimpleORBObjectAdaptor *******************/ 

Maestro_SimpleORBObjectAdaptor::Maestro_SimpleORBObjectAdaptor(
    Maestro_SimpleORBObjectAdaptor_Options &ops) { 
  assert(ops.orb != NULL);
  orb = ops.orb;
  key = ops.key;
  Maestro_Status ret = orb->bind(key, this);
  assert(ret == MAESTRO_OK);
}

Maestro_SimpleORBObjectAdaptor::~Maestro_SimpleORBObjectAdaptor() {
  Maestro_Status ret = orb->unbind(key);
  assert(ret == MAESTRO_OK);
}
