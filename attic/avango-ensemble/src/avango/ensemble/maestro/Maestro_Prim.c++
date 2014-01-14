// $Header$

/************************************************************************
 *                                                                      *
 * Author:  Alexey Vaysburd  August 97                                  *
 *                                                                      *
 * Contents:  Implementation of Maestro_Prim class                      *
 *            (primary-partition mode)                                  *
 *                                                                      *
 * Maestro_Prim delivers only primary views where state transfer has    *
 * completed.                                                           *
 ************************************************************************/

#pragma implementation
#include "avango/ensemble/maestro/Maestro_Prim.h"

#include <cassert>
#include <iostream>

void trace(char *s) {
  std::cerr << "*** Maestro_Prim: " << s << std::endl;
}


Maestro_Prim::Maestro_Prim(Maestro_Prim_Options &ops) : Maestro_CSX(ops) {
  char *s, *quorum, *param ;

  assert(ops.params.s) ;
  param = new char[strlen(ops.params.s) + 1];

  // Make sure that primary views are requested.
  if (strstr(ops.properties.s, "Primary") == NULL)
    error->panic("Maestro_Prim::Maestro_Prim: 'Primary' property must be requested");
  strcpy(param, ops.params.s);
  if ((quorum = strstr(param, "primary_quorum")) == NULL)
    error->panic("Maestro_Prim::Maestro_Prim: primary_quorum parameter must be set");
  if ((quorum = strchr(quorum, '=')) == NULL)
    error->panic("Maestro_Prim::Maestro_Prim: Bad parameter string");
  quorum++;
  if ((s = strchr(quorum, ':')) == NULL)
    error->panic("Maestro_Prim::Maestro_Prim: Bad parameter string");
  *s = 0;
  quorumSize = atoi(quorum);
  state = PRIM_BOGUS;
}

Maestro_Prim::Maestro_Prim(Maestro_Prim &mprim) : Maestro_CSX(mprim) {
  error->panic("Maestro_Prim: copy constructor not supported");
  assert(0);
}

Maestro_Prim::~Maestro_Prim() {}

Maestro_Prim& 
Maestro_Prim::operator= (Maestro_Prim &mprim) {
  error->panic("Maestro_Prim: operator= not supported");
  assert(0);
  return *this;
}

void 
Maestro_Prim::cast(Maestro_Message &msg) {
  Maestro_MsgSendView sv;
  cast(msg, sv);
}

void 
Maestro_Prim::cast(Maestro_Message &msg, 
                   Maestro_MsgSendView &sendView) {
  Maestro_ClSv_MsgOptions mops;
  cast(msg, sendView, mops);
}

void 
Maestro_Prim::cast(Maestro_Message &msg, 
                   Maestro_MsgSendView &sendView,
                   Maestro_Prim_MsgOptions &msgOps) {
  int xfer = (msgOps.msgXferSafety == MAESTRO_MSG_XFER);
  switch (state) {
  case PRIM_PRIMARY:
    Maestro_CSX::cast(msg, sendView, msgOps);
    break;
  case PRIM_BLOCKED: {
    sendView = MAESTRO_MSG_SEND_NEXT_VIEW;
    Maestro_Prim_DelayedMessage tmp(MAESTRO_PRIM_MSG_CAST, msg, msgOps);
    primLock.lock();
    delayedMsgs += tmp;
    primLock.unlock();
    break;
  }
  case PRIM_JOINING: {
    if (xfer) {
	    Maestro_CSX::cast(msg, sendView, msgOps);
    }
    else {
	    error->panic("Maestro_Prim::cast: can't cast when joining");
    }
    break;
  }
  default:
    error->panic("Maestro_Prim::cast: can't cast in this state");
    break;
  }	
}    
    
void 
Maestro_Prim::send(Maestro_EndpID &dest, 
                   Maestro_Message &msg) {
  Maestro_MsgSendView sendView;
  send(dest, msg, sendView);
}

void 
Maestro_Prim::send(Maestro_EndpID &dest, 
                   Maestro_Message &msg, 
                   Maestro_MsgSendView &sendView) {
  Maestro_ClSv_MsgOptions mops;
  send(dest, msg, sendView, mops);
}

void 
Maestro_Prim::send(Maestro_EndpID &dest, 
                   Maestro_Message &msg,
                   Maestro_MsgSendView &sendView,
                   Maestro_ClSv_MsgOptions &msgOps) {
  int xfer = (msgOps.msgXferSafety == MAESTRO_MSG_XFER);
  switch (state) {
  case PRIM_PRIMARY:
    Maestro_CSX::send(dest, msg, sendView, msgOps);
    break;
  case PRIM_BLOCKED: {
    sendView = MAESTRO_MSG_SEND_NEXT_VIEW;
    Maestro_Prim_DelayedMessage 
	    tmp(MAESTRO_PRIM_MSG_SEND, dest, msg, msgOps);
    primLock.lock();
    delayedMsgs += tmp;
    primLock.unlock();
    break;
  }
  case PRIM_JOINING: {
    if (xfer) {
	    Maestro_CSX::send(dest, msg, sendView, msgOps);
    }
    else {
	    error->panic("Maestro_Prim::send: can't send when joining");
    }
    break;
  }
  default:
    error->panic("Maestro_Prim::send: can't send in this state");
    break;
  }	
}
    
void 
Maestro_Prim::scast(Maestro_Message &msg) {
  Maestro_MsgSendView sendView;
  scast(msg, sendView);
}

void 
Maestro_Prim::scast(Maestro_Message &msg, 
                    Maestro_MsgSendView &sendView) {
  Maestro_ClSv_MsgOptions msgOps;
  scast(msg, sendView, msgOps);
}

void 
Maestro_Prim::scast(Maestro_Message &msg, 
                    Maestro_MsgSendView &sendView,
                    Maestro_ClSv_MsgOptions &msgOps) {
  int xfer = (msgOps.msgXferSafety == MAESTRO_MSG_XFER);
  switch (state) {
  case PRIM_PRIMARY:
    Maestro_CSX::scast(msg, sendView, msgOps);
    break;
  case PRIM_BLOCKED: {
    sendView = MAESTRO_MSG_SEND_NEXT_VIEW;
    Maestro_Prim_DelayedMessage 
	    tmp(MAESTRO_PRIM_MSG_SCAST, msg, msgOps);
    primLock.lock();
    delayedMsgs += tmp;
    primLock.unlock();
    break;
  }
  case PRIM_JOINING: {
    if (xfer) {
	    Maestro_CSX::scast(msg, sendView, msgOps);
    }
    else {
	    error->panic("Maestro_Prim::scast: can't scast when joining");
    }
    break;
  }
  default:
    error->panic("Maestro_Prim::scast: can't scast in this state");
    break;
  }	
}

void 
Maestro_Prim::lsend(Maestro_Message &msg, 
                    Maestro_MsgSendView &sendView,
                    Maestro_ClSv_MsgOptions &msgOps) {
  int xfer = (msgOps.msgXferSafety == MAESTRO_MSG_XFER);
  switch (state) {
  case PRIM_PRIMARY:
    Maestro_CSX::lsend(msg, sendView, msgOps);
    break;
  case PRIM_BLOCKED: {
    sendView = MAESTRO_MSG_SEND_NEXT_VIEW;
    Maestro_Prim_DelayedMessage 
	    tmp(MAESTRO_PRIM_MSG_LSEND, msg, msgOps);
    primLock.lock();
    delayedMsgs += tmp;
    primLock.unlock();
    break;
  }
  case PRIM_JOINING: {
    if (xfer) {
	    Maestro_CSX::lsend(msg, sendView, msgOps);
    }
    else {
	    error->panic("Maestro_Prim::lsend: can't lsend when joining");
    }
    break;
  }
  default:
    error->panic("Maestro_Prim::lsend: can't lsend in this state");
    break;
  }	
} 

void 
Maestro_Prim::join() {
  primLock.lock();
  if (state != PRIM_BOGUS &&
      state != PRIM_LEAVING) {
    primLock.unlock();
    error->panic("Maestro_Prim::join(): bad state (%d)", state);
  }
  state = PRIM_JOINING;
  primLock.unlock();
  Maestro_CSX::join();

  // Block until the first primary view is installed.
  joinSema.dec();		
}

void 
Maestro_Prim::leave() {
  // Cleanup:  drop delayed messages.
  primLock.lock();
  if (state != PRIM_JOINING && 
      state != PRIM_PRIMARY && 
      state != PRIM_BLOCKED) {
    primLock.unlock();
    error->panic("Maestro_Prim::leave(): bad state (%d)", state);
  }
  state = PRIM_LEAVING;
  delayedMsgs.clear();
  primLock.unlock();
  Maestro_CSX::leave();
}
    
void 
Maestro_Prim::csx_ViewMsg_Callback(Maestro_Prim_ViewData &viewData, 
                                   /*OUT*/ Maestro_Message &viewMsg) {

  trace("ViewMsg_Callback");

  // If the view to be installed is primary, pass the call up.
  // Otherwise ignore it.

  /*
    cerr << "ViewMsg: viewData.primaryFlag = " << viewData.primaryFlag
    << ", nmembers = " << viewData.members.size()
    << ", nservers = " << viewData.servers.size()
    << endl;
  */

  if (viewData.primaryFlag)
    prim_ViewMsg_Callback(viewData, viewMsg);
}

void 
Maestro_Prim::csx_AcceptedView_Callback(
                                        Maestro_Prim_ViewData& viewData,
                                        Maestro_Message &viewMsg) {
  int first_view = 0;

  trace("AcceptedView_Callback");
  assert(viewData.clientFlags.size() == viewData.nmembers);
    
  // See if majority has been lost or an unexpected state transfer
  // has been started.
  primLock.lock();

  /*
    cout << "******************* AcceptedView ******************" << endl;
    cout << "state = " << state << endl;
    cout << "quorumSize = " << quorumSize << endl;
    cout << "nmembers = " << viewData.nmembers << endl;
    cout << "primaryFlag = " << viewData.primaryFlag << endl;
  */

  if (state == PRIM_PRIMARY || state == PRIM_BLOCKED) {
    if (viewData.nGrpMembers < quorumSize) {
	    primLock.unlock();
	    assert(!viewData.primaryFlag);
	    prim_PrimarityLost_Calllback(viewData);
	    primLock.lock();
    }
    if (viewData.startXfer) {
	    primLock.unlock();
	    error->panic("Maestro_Prim::csx_AcceptedView_Callback: state transfer requested after a primary view has been installed");
	    assert(0);
    }
  }
  primLock.unlock();
	
  // If the new view is primary and no state transfers are in progress, 
  // pass the call up.  Otherwise ignore the view.
  if (viewData.primaryFlag && 
      viewData.nGrpMembers == viewData.servers.size()) {
	  primLock.lock();
	  if (state == PRIM_JOINING) {
      first_view = 1;
	  }
	  state = PRIM_PRIMARY;
	  int i;
	  Maestro_MsgSendView sv;

	  for (i = 0; i < delayedMsgs.size(); i++) {
      Maestro_Prim_DelayedMessage *dm = &delayedMsgs[i];
      switch (dm->msgType) {
      case MAESTRO_PRIM_MSG_SEND:
        Maestro_CSX::send(dm->dest, dm->msg, sv, dm->msgOps);
        break;
      case MAESTRO_PRIM_MSG_CAST:
        Maestro_CSX::cast(dm->msg, sv, dm->msgOps);
        break;
      case MAESTRO_PRIM_MSG_SCAST:
        Maestro_CSX::scast(dm->msg, sv, dm->msgOps);
        break;
      case MAESTRO_PRIM_MSG_LSEND:
        Maestro_CSX::lsend(dm->msg, sv, dm->msgOps);
        break;
      default:
        error->panic("Maestro_Prim::csx_AcceptedView_Callback: bad type of a delayed msg");
        break;
      }
	  }

	  delayedMsgs.clear();
	  primLock.unlock();
	  prim_AcceptedView_Callback(viewData, viewMsg);
	  if (first_view) {
      joinSema.inc();
	  }		
  }
}

void
Maestro_Prim::csx_Block_Callback() {
  primLock.lock();
  if (state == PRIM_PRIMARY)
    state = PRIM_BLOCKED;
  primLock.unlock();
  prim_Block_Callback();
}
