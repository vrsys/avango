// $Header$

/********************************************************************
 *                                                                  *
 * Author:  Alexey Vaysburd  December 96                            *
 *                                                                  *
 * Contents:  Definition of MAESTRO_CLSV method                     *
 *                                                                  *
 ********************************************************************/

#pragma implementation
#include "avango/ensemble/maestro/Maestro_ClSv.h"

/****************************************************************************/
/**********************                       *******************************/
/**********************      Maestro_ClSv      *******************************/
/**********************                       *******************************/
/****************************************************************************/


Maestro_ClSv::Maestro_ClSv(Maestro_ClSv_Options &ops)
  : Maestro_GroupMember(ops) {
    jops.client = (ops.mbrshipType == MAESTRO_CLIENT);
    myView.state = MAESTRO_CLSV_STATE_INITIAL;
    myView.myServerRank = myView.myOldServerRank = -1;
    myView.myMbrshipType = ops.mbrshipType;
    myView.myXferType = ops.xferType;
    myView.stateCounter = 0;
    viewType_nextView = MAESTRO_NO_XFER;
    validStateGuro = 0;
}

Maestro_ClSv::Maestro_ClSv(Maestro_ClSv& hclsv) : Maestro_GroupMember(hclsv) {
  error->panic("Maestro_ClSv: copy constructor not supported");
}

Maestro_ClSv& Maestro_ClSv::operator= (Maestro_ClSv& hclsv) {
  error->panic("Maestro_ClSv: operator= not supported");
  return *this;
}

void 
Maestro_ClSv::join() {
  if (myView.state == MAESTRO_CLSV_STATE_INITIAL) {
    myView.state = MAESTRO_CLSV_STATE_JOINING;
    Maestro_GroupMember::join();
  }
  else
    error->panic("Maestro_ClSv::join: Cannot join in this state");
}

/**************************** cast ****************************************/

void 
Maestro_ClSv::cast(Maestro_Message &msg) {
  Maestro_MsgSendView sendView;
  Maestro_ClSv::cast(msg, sendView);
}

void 
Maestro_ClSv::cast(Maestro_Message &msg, Maestro_MsgSendView &sendView) {
  Maestro_ClSv_MsgOptions msgOps;
  Maestro_ClSv::cast(msg, sendView, msgOps);
}

void 
Maestro_ClSv::cast(Maestro_Message &msg, 
		  Maestro_MsgSendView &sendView, 
		  Maestro_ClSv_MsgOptions &msgOps) {
  msg << (int) MAESTRO_CLSV_CAST;
  if (msgOps.msgXferSafety >= myView.viewType) {
    Maestro_GroupMember::cast(msg, sendView); 
  } 
  else {
    delayMsg(msg);
  }
}

/**************************** send ****************************************/

void 
Maestro_ClSv::send(Maestro_EndpID &dest, Maestro_Message &msg) {
  Maestro_MsgSendView sendView;
  Maestro_ClSv::send(dest, msg, sendView);
}

void 
Maestro_ClSv::send(Maestro_EndpID &dest, 
		  Maestro_Message &msg,
		  Maestro_MsgSendView &sendView) {
  Maestro_ClSv_MsgOptions msgOps;
  Maestro_ClSv::send(dest, msg, sendView, msgOps);
}

void 
Maestro_ClSv::send(Maestro_EndpID &dest, 
		  Maestro_Message &msg,
		  Maestro_MsgSendView &sendView, 
		  Maestro_ClSv_MsgOptions &msgOps) {
  msg << (int) MAESTRO_CLSV_SEND;
  if (msgOps.msgXferSafety >= myView.viewType) {
    Maestro_GroupMember::send(dest, msg, sendView);
  }
  else {
    delayMsg(dest, msg);
  }
}

/******************************* scast ************************************/

// Send a multicast message to servers only.
void 
Maestro_ClSv::scast(Maestro_Message &msg) {
  Maestro_MsgSendView sendView;
  Maestro_ClSv::scast(msg, sendView);
}

void 
Maestro_ClSv::scast(Maestro_Message &msg, Maestro_MsgSendView &sendView) {
  Maestro_ClSv_MsgOptions msgOps;
  Maestro_ClSv::scast(msg, sendView, msgOps);
}

void 
Maestro_ClSv::scast(Maestro_Message &msg, 
		   Maestro_MsgSendView &sendView,
		   Maestro_ClSv_MsgOptions &msgOps) {
  msg << msgOps.destList << (int) MAESTRO_CLSV_SCAST;
  if (msgOps.msgXferSafety >= myView.viewType) {
    Maestro_GroupMember::cast(msg); 
  }
  else {
    delayMsg(msg);
  }
}

/****************************** lsend *************************************/

void 
Maestro_ClSv::lsend(Maestro_Message &msg, 
		   Maestro_MsgSendView &sendView,
		   Maestro_ClSv_MsgOptions &msgOps) {
  msg << msgOps.destList << (int) MAESTRO_CLSV_LSEND;
  if (msgOps.msgXferSafety >= myView.viewType) {
    Maestro_GroupMember::cast(msg, sendView);
  }
  else {
    delayMsg(msg);
  }
}


/****************************************************************************/
/******************** Overloaded Maestro_GroupMember upcalls *****************/
/****************************************************************************/

void gdb_stop (Maestro_GrpMemb_ViewData &vd1, 
	       Maestro_GrpMemb_ViewData &vd2) {
  printf("gdb stop\n");
  vd1.dump("vd");
  vd2.dump("myView");
}

/******************* grpMemb_AcceptedView_Callback ***************************/
void 
Maestro_ClSv::grpMemb_AcceptedView_Callback(Maestro_GrpMemb_ViewData &gmView,
					   Maestro_Message &msg) {
  Maestro_ClSv_ViewData vd(gmView);

  // Extract membership info from the view message.
  if (xfer_from_oldest) {
    // Roys new code
    msg >> (int &) vd.stateCounter >> (int&) vd.viewType >> vd.xferServers >> vd.servers;
  }
  else
    msg >> (int&) vd.viewType >> vd.xferServers >> vd.servers;

  vd.servers &= vd.members;
  vd.xferServers &= vd.members;  //servers;

  if (!vd.xferServers.size())
    vd.viewType = MAESTRO_NO_XFER;
  
  // Set fields in the view struct:
  
  vd.oldState = vd.state = myView.state;

  vd.myXferType = myView.myXferType;
  vd.myMbrshipType = myView.myMbrshipType;
 
  vd.clients = vd.members;
  vd.clients -= vd.servers;
  vd.clients -= vd.xferServers;

  vd.newServers = vd.servers;
  vd.newServers -= myView.servers;

  vd.departedServers = myView.servers;
  vd.departedServers -= vd.servers;

  vd.newXferServers = vd.xferServers;
  vd.newXferServers -= myView.xferServers;

  vd.departedXferServers = myView.xferServers;
  vd.departedXferServers -= vd.xferServers;

  vd.newClients = vd.clients;
  vd.newClients -= myView.clients;

  vd.departedClients = myView.clients;
  vd.departedClients -= vd.clients;

  vd.oldServerCoordinator = myView.coordinator;
  if (vd.servers.size()) {
    vd.serverCoordinator = vd.servers[0];
  }

  vd.myOldServerRank = myView.myServerRank;
  if (vd.servers.contains(vd.myEndpID)) {
    vd.myServerRank = vd.servers[vd.myEndpID];
  }
  else {
    vd.myServerRank = -1;
  }

  // Update state values for the next view.
  servers_nextView = vd.servers;
  xferServers_nextView = vd.xferServers;
  viewType_nextView = vd.viewType;

  if (vd.members.size() == 0)
    gdb_stop(vd, myView);

  switch (vd.state) {
    
  case MAESTRO_CLSV_STATE_JOINING:

    if (!vd.clients.contains(vd.myEndpID))
      gdb_stop(vd, myView);

    assert(vd.clients.contains(vd.myEndpID)); 
    switch (vd.myMbrshipType) {
    case MAESTRO_CLIENT:
      vd.state = MAESTRO_CLSV_STATE_CLIENT_NORMAL;
      break;
    case MAESTRO_SERVER:
      becomeServer(vd);
      assert((vd.state == MAESTRO_CLSV_STATE_BECOMING_SERVER) ||
	     (vd.state == MAESTRO_CLSV_STATE_SERVER_NORMAL));
      break;
    default:
      error->panic("Maestro_ClSv::grpMemb_AcceptedView_Callback:  bad mbrship type");
    }
    break;

  case MAESTRO_CLSV_STATE_BECOMING_SERVER:

    if (vd.servers.contains(vd.myEndpID)) {
      assert(vd.myXferType == MAESTRO_NO_XFER);
      vd.state = MAESTRO_CLSV_STATE_SERVER_NORMAL;
    }
    else if (vd.xferServers.contains(vd.myEndpID)) {
      assert(vd.myXferType != MAESTRO_NO_XFER);
      if (vd.servers.size() == 0) {
	xferDone(vd);
	assert((vd.state == MAESTRO_CLSV_STATE_SERVER_XFER_DONE) ||
	       (vd.state == MAESTRO_CLSV_STATE_SERVER_NORMAL));
      }
      else
	  vd.state = MAESTRO_CLSV_STATE_SERVER_XFER;
    }
    else { 

      if (!vd.clients.contains(vd.myEndpID))
	gdb_stop(vd, myView);

      assert(vd.clients.contains(vd.myEndpID));
      if (vd.oldCoordinator != vd.coordinator) {
	becomeServer(vd);
	assert((vd.state == MAESTRO_CLSV_STATE_BECOMING_SERVER) ||
	       (vd.state == MAESTRO_CLSV_STATE_SERVER_NORMAL));
      }
    }
    break;

  case MAESTRO_CLSV_STATE_SERVER_XFER:

    if (vd.xferServers.contains(vd.myEndpID)) {
      if (vd.servers.size() == 0) {
	xferDone(vd);
	assert((vd.state == MAESTRO_CLSV_STATE_SERVER_XFER_DONE) ||
	       (vd.state == MAESTRO_CLSV_STATE_SERVER_NORMAL));
      }
    }
    else {

      if (!vd.clients.contains(vd.myEndpID))
	gdb_stop(vd, myView);

      assert(vd.clients.contains(vd.myEndpID));
      assert(vd.coordinator != vd.oldCoordinator);
      becomeServer(vd);
      assert((vd.state == MAESTRO_CLSV_STATE_BECOMING_SERVER) ||
	     (vd.state == MAESTRO_CLSV_STATE_SERVER_NORMAL));
    }
    break;

  case MAESTRO_CLSV_STATE_SERVER_XFER_DONE:

    if (vd.servers.contains(vd.myEndpID)) 
      vd.state = MAESTRO_CLSV_STATE_SERVER_NORMAL;
    else if (vd.xferServers.contains(vd.myEndpID)) {
      if (vd.coordinator != vd.oldCoordinator) {
	xferDone(vd);
	assert((vd.state == MAESTRO_CLSV_STATE_SERVER_XFER_DONE) ||
	       (vd.state == MAESTRO_CLSV_STATE_SERVER_NORMAL));
      }
    }
    else {

      if (!vd.clients.contains(vd.myEndpID))
	gdb_stop(vd, myView);

      assert(vd.clients.contains(vd.myEndpID));
      assert(vd.coordinator != vd.oldCoordinator);
      becomeServer(vd);
      assert((vd.state == MAESTRO_CLSV_STATE_BECOMING_SERVER) ||
	     (vd.state == MAESTRO_CLSV_STATE_SERVER_NORMAL));
    }
    break;
    
  case MAESTRO_CLSV_STATE_SERVER_NORMAL:

    if (!vd.servers.contains(vd.myEndpID)) {

      if (!vd.clients.contains(vd.myEndpID))
	gdb_stop(vd, myView);
      
      assert(vd.clients.contains(vd.myEndpID));
      assert(vd.coordinator != vd.oldCoordinator);
      becomeServer(vd);
      assert((vd.state == MAESTRO_CLSV_STATE_BECOMING_SERVER) ||
	     (vd.state == MAESTRO_CLSV_STATE_SERVER_NORMAL));
    }
    break;

  case MAESTRO_CLSV_STATE_CLIENT_NORMAL:
  case MAESTRO_CLSV_STATE_LEAVING:
    
    break;

  default:
    error->panic("Maestro_ClSv::grpMemb_AcceptedView_Callback:  bad cl/sv state"); 
  }

  // Set start-state-xfer flag iff we have to (re)start xfer now.
  vd.startXfer = ((vd.state == MAESTRO_CLSV_STATE_SERVER_XFER) &&
		  (vd.oldState != MAESTRO_CLSV_STATE_SERVER_XFER));
  if (vd.startXfer) {
    assert(vd.newXferServers.contains(vd.myEndpID));
  }

  // Update private view data.
  myView = vd;

  // Send out xfer-delayed messages.
  sendDelayedMsgs(); 

  // Invoke user upcall.
  clSv_AcceptedView_Callback(vd, msg);
}

void
Maestro_ClSv::grpMemb_ViewMsg_Callback(Maestro_GrpMemb_ViewData& viewData,
				       Maestro_Message &view_msg) {
    // Invoke user callback first, in case layers above want to push headers
    // in the view message.
    Maestro_ClSv_ViewData vd(viewData);
    clSv_ViewMsg_Callback(vd, view_msg);
    
    // Push this layer's headers.
    if (xfer_from_oldest) {
      // Roys new state xfer code
      view_msg << servers_nextView << xferServers_nextView 
	     << (int) viewType_nextView << myView.stateCounter;

      if (vd.myRank == 0) {
        validStateGuro = 0;
	maxStateCounter = -1;
	numStates = 0;
      }
    }
    else
      // Original state xfer code
      view_msg << servers_nextView << xferServers_nextView 
	     << (int) viewType_nextView;

}

void
Maestro_ClSv::grpMemb_ViewStateInfo_Callback(Maestro_EndpID& origin, Maestro_GrpMemb_ViewData& viewData, 
				       Maestro_Message &view_msg, Maestro_Message &next_view_msg, int &final) {
	int upStateCounter;

	final = 0;
	if (viewData.myRank == 0) {
		numStates++;
		view_msg >> upStateCounter;
		if (upStateCounter > maxStateCounter) {
			validStateGuro = 1;
			maxStateCounter = upStateCounter;
			stateMsg = view_msg;
			stateGuro = origin;
		}
		if (numStates == viewData.nmembers) {
			next_view_msg <<= stateMsg;
			next_view_msg << maxStateCounter;
			final = 1;
		}
	}
}

#ifdef notdef /* remove ProposeView & ChangingView */
/********************** grpMemb_ProposeView_Callback *************************/

// Push clients and servers into the view message.
void 
Maestro_ClSv::grpMemb_ProposeView_Callback(Maestro_GrpMemb_ViewData& viewData,
					  Maestro_MessageList &stateMsgs,
					  Maestro_Message &view_msg) {
  // Invoke user callback first, in case layers above want to push headers
  // in the view message.
  Maestro_ClSv_ViewData vd(viewData);
  clSv_ProposeView_Callback(vd, stateMsgs, view_msg);
  
  // Push this layer's headers.
  view_msg << servers_nextView << xferServers_nextView 
	    << (int) viewType_nextView;
}

/********************** grpMemb_ChangingView_Callback ************************/

void
Maestro_ClSv::grpMemb_ChangingView_Callback(Maestro_GrpMemb_ViewData &viewData,
					    /*OUT*/ Maestro_Message &stateMsg)
{
  Maestro_ClSv_ViewData vd(viewData);
  clSv_ChangingView_Callback(vd, stateMsg);
}
#endif /* remove ProposeView & ChangingView */

/********************** grpMemb_ReceiveCast_Callback *************************/
void 
Maestro_ClSv::grpMemb_ReceiveCast_Callback(Maestro_EndpID &origin, 
					  Maestro_Message &msg) {
  Maestro_ClSvMsgType mtype;
  msg >> (int&) mtype;

  Maestro_EndpList dests;

  switch(mtype) {

  case MAESTRO_CLSV_CAST:
    if (xfer_from_oldest)
      myView.stateCounter++;
    clSv_ReceiveCast_Callback(origin, msg);
    break;

  case MAESTRO_CLSV_SCAST:

    msg >> dests;
    if (myView.servers.contains(myView.myEndpID) ||
	myView.xferServers.contains(myView.myEndpID) ||
	dests.contains(myView.myEndpID)) {
      if (xfer_from_oldest)
	myView.stateCounter++;
      clSv_ReceiveScast_Callback(origin, msg);
    }
    break;
    
  case MAESTRO_CLSV_LSEND:
    
    msg >> dests;
    if (dests.contains(myView.myEndpID)) {
      if (xfer_from_oldest)
	myView.stateCounter++;
      clSv_ReceiveLsend_Callback(origin, msg);
    }
    break;
    
  default:
    error->panic("Maestro_ClSv::grpMemb_ReceiveCast_Callback:  bad message type");
  }
}


/*********************** grpMemb_ReceiveSend_Callback ************************/
void 
Maestro_ClSv::grpMemb_ReceiveSend_Callback(Maestro_EndpID &origin, 
					  Maestro_Message &msg) {

  Maestro_ClSvMsgType mtype;
  msg >> (int&) mtype;

  switch (mtype) {

  case MAESTRO_CLSV_SEND:

    clSv_ReceiveSend_Callback(origin, msg);
    break;

  case MAESTRO_CLSV_BECOME_SERVER:

    Maestro_ViewType xvtype;
    msg >> (int&) xvtype;
    BECOME_SERVER(myView, origin, xvtype);
    break;
    
  case MAESTRO_CLSV_XFER_DONE:

    XFER_DONE(myView, origin);
    break;

  case MAESTRO_CLSV_ASK_STATE:

    assert(myView.xferServers.contains(origin));
    askState_Callback(origin, msg);
    break;

  case MAESTRO_CLSV_SEND_STATE:

    rcvState_Callback(origin, msg);
    break;

  default:
    error->panic("Maestro_ClSv::grpMemb_ReceiveSend_Callback:  Bad message class");
  }
}

      
/**************************** state xfer downcalls **************************/

void
Maestro_ClSv::askState(Maestro_EndpID &server, Maestro_Message& msg) {
  if (myView.servers.contains(server)) {
    msg << MAESTRO_CLSV_ASK_STATE;
    Maestro_GroupMember::send(server, msg);
  }
  else
    error->panic("Maestro_ClSv::askState:  destination is not a server");
}

void 
Maestro_ClSv::sendState(Maestro_EndpID &joiningServer, Maestro_Message &msg) {
  if (myView.xferServers.contains(joiningServer)) {
     msg << MAESTRO_CLSV_SEND_STATE;
     Maestro_GroupMember::send(joiningServer, msg);
  }
  else
    error->panic("Maestro_ClSv::sendState:  destination isn't an xfer server");
}

void 
Maestro_ClSv::xferDone(Maestro_ClSv_ViewData& vd) {

  if (!vd.xferServers.contains(vd.myEndpID))
    return;

  if (vd.myEndpID == vd.coordinator) {
    if (vd.members.size() == 1) {
      vd.xferServers.clear();
      xferServers_nextView.clear();
      vd.servers.clear();
      vd.servers += vd.myEndpID;
      servers_nextView = vd.servers;
      vd.state = MAESTRO_CLSV_STATE_SERVER_NORMAL;
      viewType_nextView = vd.viewType = MAESTRO_NO_XFER;
    }

    else {
      XFER_DONE(vd, vd.myEndpID);
      vd.state = MAESTRO_CLSV_STATE_SERVER_XFER_DONE;
    }
  }

  else {
    Maestro_Message msg;
    msg << (int) MAESTRO_CLSV_XFER_DONE;
    vd.state = MAESTRO_CLSV_STATE_SERVER_XFER_DONE;
    Maestro_GroupMember::send(vd.coordinator, msg);
  }
}

void
Maestro_ClSv::becomeServer(Maestro_ClSv_ViewData& vd) {
  if (!vd.clients.contains(vd.myEndpID))
    return;

  if (vd.myEndpID == vd.coordinator) {
    if (vd.members.size() == 1) {
      vd.servers.clear();
      vd.servers += vd.myEndpID;
      vd.myServerRank = 0;
      servers_nextView = vd.servers;
      vd.clients.clear();
      vd.xferServers.clear();
      xferServers_nextView.clear();
      vd.state = MAESTRO_CLSV_STATE_SERVER_NORMAL;
      viewType_nextView = vd.viewType = MAESTRO_NO_XFER;
    }

    else {
      BECOME_SERVER(vd, vd.myEndpID, vd.myXferType);
      vd.state = MAESTRO_CLSV_STATE_BECOMING_SERVER;
    }
  }

  else {
    Maestro_Message msg;
    // send become-server msg to the coordinator (specify requested xfer type)
    msg << (int) vd.myXferType << (int) MAESTRO_CLSV_BECOME_SERVER;

    vd.state = MAESTRO_CLSV_STATE_BECOMING_SERVER;
    Maestro_GroupMember::send(vd.coordinator, msg);
  }
}
  

void 
Maestro_ClSv::XFER_DONE(Maestro_ClSv_ViewData& vd,
		     Maestro_EndpID &newServ) {

  if (!vd.xferServers.contains(newServ)) 
    return;
  
  assert(vd.myEndpID == vd.coordinator);
  xferServers_nextView -= newServ;
  servers_nextView += newServ;
  if (xferServers_nextView.size() == 0)
    viewType_nextView = MAESTRO_NO_XFER;

  requestNewView();
}


void 
Maestro_ClSv::BECOME_SERVER(Maestro_ClSv_ViewData& vd,
			 Maestro_EndpID &newServ, 
			 Maestro_ViewType xferOpt) {
  if (!vd.clients.contains(newServ))
    return;

  assert(vd.myEndpID == vd.coordinator);
  if (xferOpt != MAESTRO_NO_XFER) {
    xferServers_nextView += newServ;
    // view type for the next view is the strictest of the two:
    viewType_nextView = (xferOpt > viewType_nextView) 
      ? xferOpt : viewType_nextView;      
  }
  else
    servers_nextView += newServ;
  requestNewView();
}


void
Maestro_ClSv::delayMsg(Maestro_Message& msg) {
  Maestro_ClSv_DelayedMessage tmp(msg);
  xferDelayedMessages += tmp;
}


void
Maestro_ClSv::delayMsg(Maestro_EndpID& dest, Maestro_Message& msg) {
  Maestro_ClSv_DelayedMessage tmp(dest, msg);
  xferDelayedMessages += tmp;
}
  
// TODO:  Some messages may be delivered in the next view, even if they
//        are not safe for that view.  Do something about that.
void 
Maestro_ClSv::sendDelayedMsgs() {

  // If we are leaving, don't send any messages -- release all of them.
  if (myView.state == MAESTRO_CLSV_STATE_LEAVING) {
    xferDelayedMessages.clear();
    return;
  }

  if (myView.viewType <= MAESTRO_FREE_XFER) {
    Maestro_ClSv_DelayedMessageList dml;
    dml <<= xferDelayedMessages;
    assert(xferDelayedMessages.size() == 0);
    for (int i = 0; i < dml.size(); i++) {
      if (dml[i].p2p) {
	Maestro_GroupMember::send(dml[i].dest, dml[i].msg);
      }
      else {
	Maestro_GroupMember::cast(dml[i].msg);
      }
    }
  }
}


/***************************** ViewData ***********************************/

Maestro_ClSv_ViewData::Maestro_ClSv_ViewData(Maestro_GrpMemb_ViewData &gmView):
    Maestro_GrpMemb_ViewData(gmView) {
#if 0
	version = gmView.version;
	groupName = gmView.groupName;
	myEndpID = gmView.myEndpID;
	
	myRank = gmView.myRank;
	myOldRank = gmView.myOldRank;
	coordinator = gmView.coordinator;
	oldCoordinator = gmView.oldCoordinator;
	
	members <<= gmView.members;
	nmembers = gmView.nmembers;
	newMembers <<= gmView.newMembers;
	departedMembers <<= gmView.departedMembers;
	clientFlags <<= gmView.clientFlags;
	
	protocol = gmView.protocol;
	groupdFlag = gmView.groupdFlag;
	viewID = gmView.viewID;
	params = gmView.params;
	primaryFlag = gmView.primaryFlag;
#endif	
	myServerRank = myOldServerRank = -1;
	startXfer = 0;
	viewType = MAESTRO_NO_XFER;
	stateCounter = 0;
}

Maestro_ClSv_ViewData::Maestro_ClSv_ViewData() {
  myServerRank = myOldServerRank = -1;
  startXfer = 0;
  viewType = MAESTRO_NO_XFER;
  stateCounter = 0;
}




