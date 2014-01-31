// $Header$

/****************************************************************************
 *                                                                          *
 * Author:  Alexey Vaysburd  December 96                                    *
 *                                                                          *
 * Contents:  Implementation of Maestro_CSX class (higher-level state xfer) *
 *                                                                          *
 ****************************************************************************/

#pragma implementation
#include "avango/ensemble/maestro/Maestro_CSX.h"

Maestro_CSX::Maestro_CSX(Maestro_CSX_Options &ops) : 
Maestro_ClSv(ops), currentXferID(0) {
  xferData = 0; 
  xferMode = MAESTRO_MODE_MULTI_THREADED;
  getstateStatus = GETSTATE_NONE;
  state = MAESTRO_CLSV_STATE_INITIAL;
}

Maestro_CSX::Maestro_CSX(Maestro_CSX& hcsx) :
Maestro_ClSv(hcsx) {
  error->panic("Maestro_CSX: copy constructor not supported");
  assert(0);
}


Maestro_CSX::~Maestro_CSX() {}


Maestro_CSX& 
Maestro_CSX::operator= (Maestro_CSX& hcsx) {
 error->panic("Maestro_CSX: operator= not supported");
 assert(0);
 return *this;
}

// State transfer downcalls:

void 
Maestro_CSX::getState(Maestro_XferID &xferID, 
		      Maestro_Message &requestMsg, 
		      Maestro_XferStatus &xferStatus) {
  // don't ask state if this xfer's been terminated or there's no xfer in progress
  if ((xferID.id != currentXferID.id) ||
      (state != MAESTRO_CLSV_STATE_SERVER_XFER)) {
    xferStatus = MAESTRO_XFER_TERMINATED;
    return;
  }

  xferLock.lock();
  
  // don't allow re-entry
  if (xferData) {
    xferLock.unlock();
    error->panic("Maestro_CSX::getState is not reentrant");
    assert(0);
  }

  // Making a blocking call (needs to be awaken when reply arrives).
  assert(getstateStatus == GETSTATE_NONE);
  getstateStatus = GETSTATE_ASYNC;

  // allocate new xferData struct
  Maestro_XferData *xdp =  new Maestro_XferData;
  xferData = xdp;
  assert(servers.size());                    // there are servers
  xdp->server = servers[0];             // ask the oldest server for state
  xdp->request = requestMsg;
  xdp->request << xferID.id;

  // send state request msg
  Maestro_Message msg;
  msg <<= xdp->request;
  askState(xdp->server, msg);
   
  xferLock.unlock();
}

void 
Maestro_CSX::getState(Maestro_XferID &xferID, 
		      Maestro_Message &requestMsg, 
		      Maestro_Message &stateMsg,
		      Maestro_XferStatus &xferStatus) {

  // Blocking getState is not allowed in the single-threaded mode.
  if (xferMode != MAESTRO_MODE_MULTI_THREADED)
    error->panic("Maestro_CSX::getState:  blocking getState is not allowed in single-threaded mode");

  // don't ask state if this xfer's been terminated or there's no xfer in progress
  if ((xferID.id != currentXferID.id) ||
      (state != MAESTRO_CLSV_STATE_SERVER_XFER)) {
    xferStatus = MAESTRO_XFER_TERMINATED;
    return;
  }
  
  xferLock.lock();
  
  // don't allow re-entry
  if (xferData) {
    xferLock.unlock();
    error->panic("Maestro_CSX::getState is not reentrant");
    assert(0);
  }

  // Making a blocking call (needs to be awaken when reply arrives).
  assert(getstateStatus == GETSTATE_NONE);
  getstateStatus = GETSTATE_SYNC;

  // allocate new xferData struct
  Maestro_XferData *xdp =  new Maestro_XferData;
  xferData = xdp;
  assert(servers.size());                    // there are servers
  xdp->server = servers[0];             // ask the oldest server for state
  xdp->request = requestMsg;
  xdp->request << xferID.id;

  // send state request msg
  Maestro_Message msg;
  msg <<= xdp->request;
  askState(xdp->server, msg);
   
  xferLock.unlock();

  // Wait for reply or xfer termination.
  xdp->sema.dec();

  xferLock.lock();

  // Get reply message.
  xferStatus = xdp->status;
  stateMsg = xdp->reply;

  delete xdp;
  xferData = 0;
  getstateStatus = GETSTATE_NONE;

  xferLock.unlock();
}


void 
Maestro_CSX::sendState(Maestro_EndpID &dest,
		    Maestro_XferID &xferID, 
		    Maestro_Message &stateMsg) {
  stateMsg << xferID.id;
  Maestro_ClSv::sendState(dest, stateMsg);
}
		    

// (private) xfer upcalls
void 
Maestro_CSX::askState_Callback(Maestro_EndpID &origin, Maestro_Message &msg) {
  Maestro_XferID xid;
  msg >> xid.id;
  askState_Callback(origin, xid, msg);
}


void 
Maestro_CSX::rcvState_Callback(Maestro_EndpID &origin, Maestro_Message &msg) {
    //Maestro_XferID xferID;
  
  xferLock.lock();
  
  // Proceed only if we are waiting for a state msg (iff xferData != 0).
  if (xferData) {
    Maestro_XferID xid;
    msg >> xid.id;
    
    // If this state msg is for the current state xfer, use it.
    if (xid.id == currentXferID.id) {
      switch (getstateStatus) {
      case GETSTATE_SYNC:
	xferData->reply = msg;
	xferData->status = MAESTRO_XFER_OK;
	xferData->sema.inc();
	break;
      case GETSTATE_ASYNC:
	delete xferData;
	xferData = 0;
	getstateStatus = GETSTATE_NONE;
	//xferID = MAESTRO_XFER_OK;
	gotState_Callback(xid, msg);
	break;
      case GETSTATE_NONE:
	error->panic("Maestro_CSX::rcvState_Callback: bad state GETSTATE_NONE");
	break;
      default:
	error->panic("Maestro_CSX::rcvState_Callback: bad value of getstateStatus");
      }
    }
  }

  xferLock.unlock();
}


void 
Maestro_CSX::xferDispatcher(void *env) {
  Maestro_CSX_XferEnv *xfer_env = (Maestro_CSX_XferEnv*) env;
  (xfer_env->csx)->stateTransfer_Callback(xfer_env->xid);
  delete xfer_env;
}


void 
Maestro_CSX::clSv_AcceptedView_Callback(Maestro_ClSv_ViewData &viewData,
					Maestro_Message &viewMsg) {
  state = viewData.state;
  servers = viewData.servers;
  xferServers = viewData.xferServers;

  // (re)start state xfer if necessary
  if (viewData.startXfer) {
    // In single-threaded mode, invoke xfer callback directly.
    if (xferMode == MAESTRO_MODE_SINGLE_THREADED) {
      stateTransfer_Callback(currentXferID);
    }

    // In multi-threaded mode, invoke xfer callback in a separate thread.
    else {
      Maestro_CSX_XferEnv *xfer_env = 
	new Maestro_CSX_XferEnv(this, currentXferID.id);
      Maestro_Thread::create(xferDispatcher, (void*) xfer_env);
    }
  }

  // Terminate state xfer if necessary.
  //
  // NB:  Semantics of callbacks/dncalls in single-threaded mode:
  //   A call to (async) getState is eventually followed by an invocation
  //   of either a gotState_Callback or xferCanceled_Callback.
  if ((viewData.state != MAESTRO_CLSV_STATE_SERVER_XFER) && xferData) {
    xferLock.lock();
    if (getstateStatus == GETSTATE_SYNC) {
      // wake up waiting xfer thread w/ XFER_TERMINATED status
      xferData->status = MAESTRO_XFER_TERMINATED;
      xferData->sema.inc();
      xferData = 0;
      xferLock.unlock();
    }
    else {
      delete xferData;
      xferData = 0;
      getstateStatus = GETSTATE_NONE;
      xferLock.unlock();
      xferCanceled_Callback(currentXferID);
    }
  }

  // if this member is waiting for state from a server that has just crashed,
  // resubmit the request to another server
  if ((viewData.state == MAESTRO_CLSV_STATE_SERVER_XFER) &&
      xferData && (!viewData.servers.contains(xferData->server))) {
    xferLock.lock();

    assert(viewData.servers.size());
    //assert(!viewData.xferServers.contains(viewData.servers[0]));
    // the oldest server must have the state
    xferData->server = viewData.servers[0];
    Maestro_Message msg;
    msg <<= xferData->request;
    askState(xferData->server, msg);
    
    xferLock.unlock();
  }

  // invoke user upcall
  csx_AcceptedView_Callback(viewData, viewMsg);
}


void 
Maestro_CSX::becomeServer(Maestro_ClSv_ViewData& vd) {
  currentXferID.id++;  // expire old xfer ID 
  Maestro_ClSv::becomeServer(vd);
}

// private
void 
Maestro_CSX::xferDone(Maestro_ClSv_ViewData& vd) {
  currentXferID.id++;  // expire old xfer ID 
  Maestro_ClSv::xferDone(vd);
}

// public 
void
Maestro_CSX::xferDone(Maestro_XferID &xferID) {
  // don't do anything if invoked for an expired xfer
  if (xferID.id == currentXferID.id) {
    currentXferID.id++;  // expire old xfer ID 
    Maestro_ClSv::xferDone();
  }
}
