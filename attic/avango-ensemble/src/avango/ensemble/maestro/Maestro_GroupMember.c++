// $Header$

/********************************************************************
 *                                                                  *
 * Author:  Alexey Vaysburd  December 96                            *
 *                                                                  *
 * Contents:  Definition of Maestro_GroupMember class methods       *
 *                                                                  *
 ********************************************************************/
#if !defined(__hpux)
#pragma implementation
#endif
#include "avango/ensemble/maestro/Maestro_GroupMember.h"
#include "avango/ensemble/maestro/Maestro_Perf.h"

#include <iostream>

/**************************** Maestro_GroupMember ***************************/


Maestro_GroupMember::Maestro_GroupMember(Maestro_GrpMemb_Options &membOps) :
  Maestro_Base(membOps.errorHandler),
  myView() {
  // Initialize local state.
  mbrState = GRPMEMB_BOGUS;
  jops = membOps;

  /* Roy Friedman's new state xfer code relies upon the Total
   * ordering property being set.  Here I check for it in the
   * jops.properties, and if its there, I set a flag which will
   * use Roy's new code.  Otherwise the original code is used.
   */
  if (strstr(jops.properties.s, "Total") != (char*)0)
    xfer_from_oldest = 1;
  else
    xfer_from_oldest = 0;
}

Maestro_GroupMember::Maestro_GroupMember(Maestro_GroupMember &gm) :
  Maestro_Base(gm.error) {
  error->panic("Maestro_GroupMember: copy constructor not supported");
  assert(0);
}

Maestro_GroupMember& 
Maestro_GroupMember::operator= (Maestro_GroupMember& hgm) {
  error->panic("Maestro_GroupMember: operator= not supported");
  assert(0);
  return *this;
}

/**************************** ~Maestro_GroupMember ***************************/

Maestro_GroupMember::~Maestro_GroupMember() {
  // Leave the group.
  Maestro_GroupMember::leave();
}

/****************************** Public Downcalls *****************************/

void
Maestro_GroupMember::join() {
  if (mbrState != GRPMEMB_BOGUS) 
    error->panic("Maestro_GroupMember::join: Bad state for joining");
  mbrState = GRPMEMB_JOINING;

  // Setup join options.
  hot_ens_JoinOps_t join_ops;
  hot_ens_InitJoinOps(&join_ops);
  join_ops.heartbeat_rate = jops.heartbeatRate;

  if (jops.transports.s)
    strcpy(join_ops.transports, jops.transports.s);

  if (jops.properties.s) {
    strcpy(join_ops.properties, jops.properties.s);
    join_ops.use_properties = 1;
  }
  else {
    join_ops.use_properties = 0;
  }
   
  if (jops.protocol.s)
	  strcpy(join_ops.protocol, jops.protocol.s);
  if (jops.params.s)
	  strcpy(join_ops.params, jops.params.s);
  if (jops.outboard.s)
	  strcpy(join_ops.outboard, jops.outboard.s);

  if (jops.groupName.s)
    strcpy(join_ops.group_name, jops.groupName.s);

  join_ops.argv = jops.argv;
  join_ops.groupd = jops.groupdFlag;
  join_ops.debug = jops.debug;
  join_ops.client = jops.client;

  join_ops.conf.receive_cast = receiveCast_Dispatcher;
  join_ops.conf.receive_send = receiveSend_Dispatcher;
  join_ops.conf.accepted_view = acceptedView_Dispatcher;
  join_ops.conf.heartbeat = heartbeat_Dispatcher;
  join_ops.conf.block = block_Dispatcher;
  join_ops.conf.exit = exit_Dispatcher;

  join_ops.env = (void*) this;
  expectingViewMsg = 0;

  // Join the group.
  jl_status = JL_JOINING;
  err = hot_ens_Join(&join_ops, &gctx);
  check_err("Maestro_GroupMember::join: hot_ens_Join");

  // Don't return till the first view is delivered.
  jl_sema.dec();
  
  //dump("Maestro_GroupMember::join: leave");
}

void 
Maestro_GroupMember::send(Maestro_EndpID &dest, Maestro_Message &msg) {
  Maestro_MsgSendView sendView;
  Maestro_GroupMember::send(dest, msg, sendView);
}

void 
Maestro_GroupMember::send(Maestro_EndpID &dest, Maestro_Message &msg,
                          Maestro_MsgSendView &sendView) {
  Maestro_GrpMemb_MsgOptions ops;
  Maestro_GroupMember::send(dest, msg, sendView, ops);
}

void 
Maestro_GroupMember::send(Maestro_EndpID &dest, Maestro_Message &msg, 
                          Maestro_MsgSendView &sendView,
                          Maestro_GrpMemb_MsgOptions &msgOps) {
  // Drop the message if we are leaving.
  if (mbrState == GRPMEMB_LEAVING)
    return;

  if (mbrState != GRPMEMB_NORMAL)
    error->panic("Maestro_GroupMember::send: Bad state for sending");

  if (xfer_from_oldest && jops.viewMsgFlag) {
    // Roys new state xfer code.

    Maestro_GrpMemb_MsgType msg_type = MAESTRO_GRPMBR_MSG_TYPE_DATA;
    msg << (int&) msg_type;
  }

  hot_endpt_t dest_endp = dest.getHotEndpt();
  err = hot_ens_Send(gctx, &dest_endp, msg.msg, 
                     (hot_ens_MsgSendView*) &sendView);
  check_err("Maestro_GroupMember::send: hot_ens_Send");
}

void 
Maestro_GroupMember::cast(Maestro_Message &msg) {
  Maestro_MsgSendView sendView;
  Maestro_GroupMember::cast(msg, sendView);
}

void 
Maestro_GroupMember::cast(Maestro_Message &msg, 
                          Maestro_MsgSendView &sendView) {
  Maestro_GrpMemb_MsgOptions ops;
  Maestro_GroupMember::cast(msg, sendView, ops);
}

void 
Maestro_GroupMember::cast(Maestro_Message &msg, 
                          Maestro_MsgSendView &sendView, 
                          Maestro_GrpMemb_MsgOptions &msgOps) {
  // Drop the message if we are leaving.
  if (mbrState == GRPMEMB_LEAVING)
    return;
    
  if (mbrState != GRPMEMB_NORMAL)
    error->panic("Maestro_GroupMember::cast: Bad member state");
    
  if (jops.viewMsgFlag) {
    Maestro_GrpMemb_MsgType msg_type = MAESTRO_GRPMBR_MSG_TYPE_DATA;
    msg << (int&) msg_type;
  }

  perf_start(41, "GroupMember: latency of local delivery:");
    
  err = hot_ens_Cast(gctx, msg.msg, (hot_ens_MsgSendView*) &sendView);
  check_err("Maestro_GroupMember::cast: hot_ens_Cast");
}

void 
Maestro_GroupMember::suspect(Maestro_EndpList &suspects) {
  // Ignore if we are leaving.
  if (mbrState == GRPMEMB_LEAVING)
    return;

  if (mbrState != GRPMEMB_NORMAL)
    error->panic("Maestro_GroupMember::suspect: Bad member state");

  hot_endpt_t *susp = new hot_endpt_t[suspects.size()];
  int i;

  for (i = 0; i < suspects.size(); i++) {
    susp[i] = suspects[i].getHotEndpt();
  }

  err = hot_ens_Suspect(gctx, susp, suspects.size());
  check_err("Maestro_GroupMember::suspect: hot_ens_Suspect");

  delete [] susp;
}

void 
Maestro_GroupMember::changeProtocol(Maestro_String &protocol) {
  std::cerr << "*** GrpMbr: changeProtocol" << std::endl;

  // Ignore if we are leaving.
  if (mbrState == GRPMEMB_LEAVING)
    return;

  if (mbrState != GRPMEMB_NORMAL)
    error->panic("Maestro_GroupMember::changeProtocol: Bad member state");

  err = hot_ens_ChangeProtocol(gctx, protocol.s);
  check_err("Maestro_GroupMember::changeProtocol: hot_ens_ChangeProtocol");
}

void 
Maestro_GroupMember::changeProperties(Maestro_String &properties) {
  std::cerr << "*** GrpMbr: changeProperties" << std::endl;

  // Ignore if we are leaving.
  if (mbrState == GRPMEMB_LEAVING)
    return;

  if (mbrState != GRPMEMB_NORMAL)
    error->panic("Maestro_GroupMember::changeProperties: Bad member state");

  err = hot_ens_ChangeProperties(gctx, properties.s);
  check_err("Maestro_GroupMember::changeProperties: hot_ens_ChangeProperties");
}

void 
Maestro_GroupMember::requestNewView() {
  std::cerr << "*** GrpMbr: requestNewView" << std::endl;

  // Ignore if we are leaving.
  if (mbrState == GRPMEMB_LEAVING)
    return;

  if (mbrState != GRPMEMB_NORMAL)
    error->panic("Maestro_GroupMember::requestNewView: Bad member state");

  err = hot_ens_RequestNewView(gctx);
  check_err("Maestro_GroupMember::requestNewView");
}

void 
Maestro_GroupMember::myEndpID(Maestro_EndpID &eid) {
  if (mbrState != GRPMEMB_NORMAL)
    error->panic("Maestro_GroupMember::myEndpID: Bad member state");
      
  eid = myView.myEndpID;
}

void
Maestro_GroupMember::leave() {
  if (mbrState != GRPMEMB_NORMAL)
    error->panic("Maestro_GroupMember::leave: Bad member state");
  mbrState = GRPMEMB_LEAVING;

  jl_status = JL_LEAVING;
  err = hot_ens_Leave(gctx);
  check_err("Maestro_GroupMember::leave: hot_ens_Leave");

  // Don't return till the exit callback is invoked.
  jl_sema.dec();
}
  
void Maestro_GrpMemb_Options::dump(char *title) {
  std::cout << "================ JOIN OPTIONS DUMP ====================" << std::endl;
  std::cout << "Title: " << title << std::endl;
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "heartbeatRate: " << heartbeatRate << std::endl;
  std::cout << "groupName: " << groupName << std::endl;
  std::cout << "protocol: " << protocol << std::endl;
  std::cout << "properties: " << properties << std::endl;
  std::cout << "params: " << params << std::endl;
  std::cout << "transports: " << transports << std::endl;
  std::cout << "groupdFlag: " << groupdFlag << std::endl;
  std::cout << "viewMsgFlag: " << viewMsgFlag << std::endl;
  std::cout << "============= END OF JOIN OPTIONS DUMP ================" << std::endl;
}

void Maestro_GroupMember::dump(char *title) {
  std::cout << "================ GROUP MEMBER DUMP ====================" << std::endl;
  std::cout << "Title: " << title << std::endl;
  std::cout << "----------------------------------------------------" << std::endl;
  myView.dump("group member dump: view data");

  std::cout << "mbrState: ";
  switch (mbrState) {
  case GRPMEMB_BOGUS:
    std::cout << "GRPMEMB_BOGUS";
    break;
  case GRPMEMB_JOINING:
    std::cout << "GRPMEMB_JOINING";
    break;
  case GRPMEMB_NORMAL:
    std::cout << "GRPMEMB_NORMAL";
    break;
  case GRPMEMB_LEAVING:
    std::cout << "GRPMEMB_LEAVING";
    break;
  default:
    error->panic("Maestro_GroupMember::dump: bad mbrState");
    break;
  }

  jops.dump("group member dump: join options");
  std::cout << "gctx = " << gctx << std::endl;

  std::cout << "expectingViewMsg: " << expectingViewMsg << std::endl;
  std::cout << "lastViewID: " << lastViewID << std::endl;
  std::cout << "lastViewMsgOrigin: " << lastViewMsgOrigin << std::endl;
  
  std::cout << "============== END OF GROUP MEMBER DUMP ===============" << std::endl;
}

/************************* Callback Dispatchers ******************************/

// Received a multicast message.
void 
Maestro_GroupMember::receiveCast_Dispatcher(hot_gctx_t gctx, 
                                            void *env, 
                                            hot_endpt_t *origin, 
                                            hot_msg_t msg) {
  perf_end(41);

  Maestro_GroupMember *mbr = (Maestro_GroupMember*) env;
  mbr->gctx = gctx;
  Maestro_Message cast_msg(msg);
  Maestro_EndpID originID(*origin);

  if (mbr->jops.viewMsgFlag) {
    Maestro_GrpMemb_MsgType msg_type;
    cast_msg >> (int&) msg_type;
    switch (msg_type) {
    case MAESTRO_GRPMBR_MSG_TYPE_DATA:
      mbr->grpMemb_ReceiveCast_Callback(originID, cast_msg);
      break;
    case MAESTRO_GRPMBR_MSG_TYPE_VIEW:

      if (!(mbr->myView.myEndpID == originID)) {
        Maestro_EndpID eid;
        Maestro_ViewID vid;
        cast_msg >> eid;
        cast_msg >> vid;

        if (vid.vid.ltime < mbr->myView.viewID.vid.ltime) {
          // It is possible that an old view message will be received
          // in the current view.  Don't panic, just ignore it.
          // This may happen b/c of the missing-BLOCK_OK-action bug in
          // the outboard mode of Ensemble.
          break;
        } else
          if (!mbr->expectingViewMsg || 
              !(vid == mbr->myView.viewID) ||
              !(originID == mbr->myView.coordinator)) {
	      
            std::cerr << "************** PANIC *******************" << std::endl;

            std::cerr << "got view msg from " << originID << std::endl;
            std::cerr << "message says its from " << eid << std::endl ;
            std::cerr << "my coordinator is " << mbr->myView.coordinator << std::endl;
            std::cerr << "view msg was " << (mbr->expectingViewMsg?"":"NOT ")
                      << "expected" << std::endl;
            std::cerr << "got view msg with ViewID = " << vid << std::endl;
            std::cerr << "my current ViewID = " << mbr->myView.viewID << std::endl;
            mbr->dump("bad view message");

            mbr->error->panic("Maestro_GroupMember::grpMemb_AcceptedView_Callback: bad view message");
          }
        mbr->lastViewMsgOrigin = originID;   
        mbr->expectingViewMsg = 0;
        mbr->grpMemb_AcceptedView_Callback(mbr->myView, cast_msg);

        // Wake up the join() downcall.
        if (mbr->jl_status == JL_JOINING) {
          mbr->jl_status = JL_NORMAL;
          mbr->jl_sema.inc();
        }
      }
      break;
    default:
      mbr->error->panic("Maestro_GroupMember::receiveCast_Dispatcher: bad msg type");
      break;
    }
  }
  else {
    mbr->grpMemb_ReceiveCast_Callback(originID, cast_msg);
  }
}

// Received a point-to-point message.
void 
Maestro_GroupMember::receiveSend_Dispatcher(hot_gctx_t gctx, 
                                            void *env, 
                                            hot_endpt_t *origin, 
                                            hot_msg_t msg) {

  Maestro_GroupMember *mbr = (Maestro_GroupMember*) env;
  mbr->gctx = gctx;
  Maestro_EndpID originID(*origin);
  Maestro_Message send_msg(msg);


  if (mbr->xfer_from_oldest && mbr->jops.viewMsgFlag) {
    /* Roy Friedmans new state xfer code */

    Maestro_GrpMemb_MsgType msg_type;
    send_msg >> (int&) msg_type;

    switch (msg_type) {
    case MAESTRO_GRPMBR_MSG_TYPE_DATA:

      mbr->grpMemb_ReceiveSend_Callback(originID, send_msg);
      break;
    case MAESTRO_GRPMBR_MSG_TYPE_STATE:

      Maestro_EndpID eid;
      Maestro_ViewID vid;
      send_msg >> eid;
      send_msg >> vid;

      if (vid.vid.ltime < mbr->myView.viewID.vid.ltime) {
        // It is possible that an old view message will be received
        // in the current view.  Don't panic, just ignore it.
        break;
      } else if ((mbr->myView.myRank != 0) || !mbr->expectingViewState || 
                 !(vid == mbr->myView.viewID)) {
	      
        std::cerr << "************** PANIC *******************" << std::endl;
        std::cerr << "got view state msg from " << originID << std::endl;
        std::cerr << "message says its from " << eid << std::endl ;
        std::cerr << "my rank is " << mbr->myView.myRank << std::endl;
        std::cerr << "view state msg was " << (mbr->expectingViewState?"":"NOT ")
                  << "expected" << std::endl;
        std::cerr << "got view msg with ViewID = " << vid << std::endl;
        std::cerr << "my current ViewID = " << mbr->myView.viewID << std::endl;
        mbr->dump("bad view message");

        mbr->error->panic("Maestro_GroupMember::grpMemb_AcceptedSend_Callback: bad view message");
      }
      Maestro_Message next_view_msg;
      int final;

      mbr->grpMemb_ViewStateInfo_Callback(mbr->myView.myEndpID, mbr->myView, send_msg, 
                                          next_view_msg, final);
      if (final) {
        mbr->expectingViewState = 0;

        Maestro_Message tmp_msg;
        tmp_msg <<= next_view_msg;
        tmp_msg << mbr->myView.viewID;
        tmp_msg << mbr->myView.myEndpID;
        tmp_msg << (int) MAESTRO_GRPMBR_MSG_TYPE_VIEW;

        // Multicast the view message only if we are not leaving.
        if (mbr->mbrState == GRPMEMB_NORMAL) {
          hot_err_t err;
          err = hot_ens_Cast(mbr->gctx, tmp_msg.msg, NULL);
          if (err != HOT_OK)
            mbr->error->panic(err, "Maestro_GroupMember::grpMemb_AcceptedView_Callback: hot_ens_Cast");
        }
        mbr->grpMemb_AcceptedView_Callback(mbr->myView, next_view_msg);

        // Wake up the join() downcall.
        if (mbr->jl_status == JL_JOINING) {
          mbr->jl_status = JL_NORMAL;
          mbr->jl_sema.inc();
        }
      }
    }
  } else {
    /* original state xfer code */

    mbr->grpMemb_ReceiveSend_Callback(originID, send_msg);
  }
}

// Accepted a new view.
void 
Maestro_GroupMember::acceptedView_Dispatcher(hot_gctx_t gctx, 
                                             void *env, 
                                             hot_view_state_t *view_state) { 
  Maestro_GroupMember *mbr = (Maestro_GroupMember*) env;
  mbr->gctx = gctx;
  mbr->setupViewState(view_state, mbr->myView);

  switch (mbr->mbrState) {
  case GRPMEMB_BOGUS:
    mbr->error->panic("Maestro_GroupMember::acceptedView_Dispatcher: accepted view in BOGUS state");
    break;
  case GRPMEMB_JOINING:
    mbr->mbrState = GRPMEMB_NORMAL;
    break;
  case GRPMEMB_NORMAL:  // FALL THROUGH
  case GRPMEMB_LEAVING:
    break;
  default:
    mbr->error->panic("Maestro_GroupMember::acceptedView_Dispatcher: bad state");
    break;
  }

  Maestro_Message view_msg;

  if (mbr->xfer_from_oldest && mbr->jops.viewMsgFlag) {
    /* Roy Friedmans new state xfer code */

    mbr->lastViewID = mbr->myView.viewID;

    Maestro_GrpMemb_ViewData vd(mbr->myView);
    mbr->grpMemb_ViewMsg_Callback(vd, view_msg);
    int final;
    Maestro_Message tmp_msg;
    tmp_msg <<= view_msg;
    tmp_msg << mbr->myView.viewID;
    tmp_msg << mbr->myView.myEndpID;
    tmp_msg << (int) MAESTRO_GRPMBR_MSG_TYPE_STATE;
    // If I am coordinator (rank == 0) pass my info to the ViewStateInfo callback
    if (mbr->myView.myRank == 0) {
      Maestro_Message next_view_msg;
      mbr->grpMemb_ViewStateInfo_Callback(mbr->myView.myEndpID, vd, view_msg, next_view_msg, final);
      if (final) {
        mbr->expectingViewState = 0;
        mbr->expectingViewMsg = 0;
        mbr->grpMemb_AcceptedView_Callback(mbr->myView, next_view_msg);
        // Wake up the join() downcall.
        if (mbr->jl_status == JL_JOINING) {
          mbr->jl_status = JL_NORMAL;
          mbr->jl_sema.inc();
        }
      }
      else {
        mbr->expectingViewState = 1;
        mbr->expectingViewMsg = 0;
      }
      // otherwise, send my info to the coordinator
    } else {
      hot_err_t err;
      mbr->expectingViewState = 0;
      mbr->expectingViewMsg = 1;
      
      hot_endpt_t endp_id = mbr->myView.members[0].getHotEndpt();
      
      err = hot_ens_Send(mbr->gctx, &endp_id, tmp_msg.msg, NULL);
      if (err != HOT_OK)
        mbr->error->panic(err, "Maestro_GroupMember::grpMemb_AcceptedView_Callback: hot_ens_Send");
    }
  }
  else if (mbr->jops.viewMsgFlag) {
    /* Original state xfer code */

    mbr->lastViewID = mbr->myView.viewID;

    if (mbr->myView.myRank == 0) {
      mbr->expectingViewMsg = 0;
      Maestro_Message tmp_msg;
      Maestro_GrpMemb_ViewData vd(mbr->myView);
      mbr->grpMemb_ViewMsg_Callback(vd, view_msg);
      tmp_msg <<= view_msg;
      tmp_msg << mbr->myView.viewID;
      tmp_msg << mbr->myView.myEndpID;
      tmp_msg << (int) MAESTRO_GRPMBR_MSG_TYPE_VIEW;

      // Multicast the view message only if we are not leaving.
      if (mbr->mbrState == GRPMEMB_NORMAL) {
        hot_err_t err;

        perf_start(41, "GroupMember: latency of local delivery:");

        err = hot_ens_Cast(mbr->gctx, tmp_msg.msg, NULL);
        if (err != HOT_OK) mbr->error->panic(err, "Maestro_GroupMember::grpMemb_AcceptedView_Callback: hot_ens_Cast"); 	  	 
      }

      mbr->grpMemb_AcceptedView_Callback(mbr->myView, view_msg);

      // Wake up the join() downcall.
      if (mbr->jl_status == JL_JOINING) {
        mbr->jl_status = JL_NORMAL;
        mbr->jl_sema.inc();
      }
    }
    else {
      mbr->expectingViewMsg = 1;
    }
  }
  else {
    mbr->grpMemb_AcceptedView_Callback(mbr->myView, view_msg);

    // Wake up the join() downcall.
    if (mbr->jl_status == JL_JOINING) {
      mbr->jl_status = JL_NORMAL;
      mbr->jl_sema.inc();
    }
  }
}

// Got a heartbeat event (invoked periodically).
// Current time (in milliseconds) is specified.
void 
Maestro_GroupMember::heartbeat_Dispatcher(hot_gctx_t gctx, 
                                          void *env, 
                                          unsigned rate) {
  Maestro_GroupMember *mbr = (Maestro_GroupMember*) env;
  mbr->gctx = gctx;
  mbr->grpMemb_Heartbeat_Callback(rate);
}

// The group is blocked for a view change.
void 
Maestro_GroupMember::block_Dispatcher(hot_gctx_t gctx, 
                                      void *env) {
  Maestro_GroupMember *mbr = (Maestro_GroupMember*) env;
  mbr->gctx = gctx;
  mbr->grpMemb_Block_Callback();
}

// The member has left the group.
void 
Maestro_GroupMember::exit_Dispatcher(hot_gctx_t gctx, 
                                     void *env) {
  Maestro_GroupMember *mbr = (Maestro_GroupMember*) env;
  mbr->mbrState = GRPMEMB_BOGUS;
  mbr->gctx = gctx;
  mbr->grpMemb_Exit_Callback();

  // Wake up the leave() downcall.
  assert(mbr->jl_status == JL_LEAVING);
  mbr->jl_status = JL_NORMAL;
  mbr->jl_sema.inc();
}

/*************************** Private ***********************************/

void 
Maestro_GroupMember::setupViewState(hot_view_state_t *view_state, 
                                    Maestro_GrpMemb_ViewData &viewData) {
  if (myView.members.size())
    viewData.oldCoordinator = myView.members[0];
  viewData.departedMembers = myView.members;

  Maestro_EndpList tmp(view_state->members, view_state->nmembers);
  viewData.members <<= tmp;

  viewData.newMembers = viewData.members;
  viewData.newMembers -= viewData.departedMembers;

  viewData.departedMembers -= viewData.members;

  viewData.nmembers = view_state->nmembers;  
  viewData.coordinator = viewData.members[0];
  viewData.myOldRank = myView.myRank;
  viewData.myRank = view_state->rank;

  Maestro_IntList tmp_cl(view_state->clients, view_state->nmembers);
  viewData.clientFlags <<= tmp_cl;

  int i;
  for (i = viewData.nGrpMembers = 0; i < viewData.nmembers; i++) {
    if (viewData.clientFlags[i] == 0) {
      viewData.nGrpMembers++;
    }
  }
  viewData.nGrpClients = viewData.nmembers - viewData.nGrpMembers;

  Maestro_String proto_str(view_state->protocol);
  viewData.protocol = proto_str;

  viewData.myEndpID = viewData.members[viewData.myRank];  
  
  Maestro_String version_str(view_state->version);
  viewData.version = version_str;

  Maestro_String group_name(view_state->group_name);
  viewData.groupName = group_name;

  Maestro_String params_str(view_state->params);
  viewData.params = params_str;

  viewData.groupdFlag = view_state->groupd;
  viewData.primaryFlag = view_state->primary;
  viewData.viewID = view_state->view_id;
}
  
/******************** Maestro_GrpMemb_ViewData ***********************/

Maestro_GrpMemb_ViewData::Maestro_GrpMemb_ViewData()
  : version(""),
    groupName(""),
    coordinator(),
    oldCoordinator(),
    myEndpID(),
    members(),
    newMembers(),
    departedMembers(),
    nmembers(0),
    myRank(-1),
    myOldRank(-1),
    viewID(),
    primaryFlag(0),
    protocol(""),
    groupdFlag(0),
    params(""),
    clientFlags(),
    nGrpClients(0),
    nGrpMembers(0)
{}

void Maestro_GrpMemb_ViewData::dump(char *title) {
  std::cout << "================ VIEW DATA DUMP ====================" << std::endl;
  std::cout << "Title: " << title << std::endl;
  std::cout << "----------------------------------------------------" << std::endl;
  std::cout << "version: " << version << std::endl;
  std::cout << "groupName: " << groupName << std::endl;
  std::cout << "coordinator: " << coordinator << std::endl;
  std::cout << "oldCoordinator: " << oldCoordinator << std::endl;
  std::cout << "myEndpID: " << myEndpID << std::endl;
  std::cout << "members: " << members << std::endl;
  std::cout << "nmembers: " << nmembers << std::endl;
  std::cout << "myRank: " << myRank << std::endl;
  std::cout << "myOldRank: " << myOldRank << std::endl;
  std::cout << "viewID: " << viewID << std::endl;
  std::cout << "primaryFlag: " << primaryFlag << std::endl;
  std::cout << "protocol: " << protocol << std::endl;
  std::cout << "groupdFlag: " << groupdFlag << std::endl;
  std::cout << "params: " << params << std::endl;
  std::cout << "============= END OF VIEW DATA DUMP ================" << std::endl;
}

/******************** Maestro_GrpMemb_ViewID ***********************/
			
Maestro_ViewID::Maestro_ViewID() {
  vid.ltime = -1;
  strcpy(vid.coord.name, "{?}");
}

Maestro_ViewID::Maestro_ViewID(hot_view_id &view_id) {
  vid = view_id;
}

Maestro_ViewID& Maestro_ViewID::operator= (Maestro_ViewID &vid2) {
  vid = vid2.vid;
  return *this;
}

Maestro_ViewID& Maestro_ViewID::operator= (hot_view_id &view_id) {
  vid = view_id;
  return *this;
}

int Maestro_ViewID::operator== (Maestro_ViewID &vid2) {
  return (vid.ltime == vid2.vid.ltime &&
          !strcmp(vid.coord.name, vid2.vid.coord.name));
}

int Maestro_ViewID::operator!= (Maestro_ViewID &vid2) {
  return (vid.ltime != vid2.vid.ltime ||
          strcmp(vid.coord.name, vid2.vid.coord.name));
}

int Maestro_ViewID::operator< (Maestro_ViewID &vid2) {
  return (vid.ltime < vid2.vid.ltime ||
          (vid.ltime == vid2.vid.ltime && 
           strcmp(vid.coord.name, vid2.vid.coord.name) < 0));
}

int Maestro_ViewID::operator> (Maestro_ViewID &vid2) {
  return (vid.ltime > vid2.vid.ltime ||
          (vid.ltime == vid2.vid.ltime && 
           strcmp(vid.coord.name, vid2.vid.coord.name) > 0));
}

void Maestro_ViewID::operator>> (std::ostream& out) const {
  out << "[" << vid.ltime << "," << vid.coord.name << "]";
}

void Maestro_ViewID::operator<< (Maestro_Message& msg) {
  Maestro_String coord;
  msg >> coord >> vid.ltime;
  strcpy(vid.coord.name, coord.s);
}

void Maestro_ViewID::operator>> (Maestro_Message& msg) {
  Maestro_String coord(vid.coord.name);
  msg << vid.ltime << coord;
}
