// $Header$
// 
// Author:  Alexey Vaysburd, March 1999.
// 
// Contents:  New implementation of Maestro clients/servers/state transfer protocol.
// 
// Maestro_Group defines a collection of downcalls for sending messages to a group.
// Maestro_GroupListener is an interface defining group callbacks.
// The application will define a class that implements Maestro_GroupListener and 
// will pass a Maestro_GroupListener object to an instance of Maestro_Group.

#include "avango/ensemble/maestro/Maestro_Group.h"

Maestro_GroupOptions::Maestro_GroupOptions()
{
  heartbeatRate = MAESTRO_DEFAULT_HEARTBEAT_RATE; 
  
  Maestro_String props_str((char*) MAESTRO_DEFAULT_PROTOCOL_PROPERTIES);
  properties = props_str;
  
  Maestro_String grpnam_str("Maestro-default-group");
  groupName = grpnam_str;
  
  Maestro_String transp_str((char*) MAESTRO_DEFAULT_TRANSPORTS);
  transports = transp_str;

  outboard = MAESTRO_DEFAULT_OUTBOARD_MODULE;
  
  argv = 0;
  serverFlag = 1;
  groupdFlag = 0;
  debug = 0;
}


Maestro_Group::Maestro_Group(Maestro_GroupListener &groupListener, 
			     Maestro_GroupOptions &options)
{
  _groupListener = &groupListener;
  _options = options;
  _state = INITIAL;
  _gotView = 0;

  /*
   * Setup join options for HOT.
   */
  hot_ens_JoinOps_t join_ops;
  hot_ens_InitJoinOps(&join_ops);
  join_ops.heartbeat_rate = options.heartbeatRate;

  if (options.transports.s)
    strcpy(join_ops.transports, options.transports.s);
  
  if (options.properties.s) {
      strcpy(join_ops.properties, options.properties.s);
      join_ops.use_properties = 1;
  }
  else {
      join_ops.use_properties = 0;
  }
   
  if (options.protocol.s)
	  strcpy(join_ops.protocol, options.protocol.s);
  if (options.params.s)
	  strcpy(join_ops.params, options.params.s);
  if (options.outboard.s)
	  strcpy(join_ops.outboard, options.outboard.s);

  if (options.groupName.s)
    strcpy(join_ops.group_name, options.groupName.s);

  join_ops.argv = options.argv;
  join_ops.groupd = options.groupdFlag;
  join_ops.debug = options.debug;
  join_ops.client = 0;

  join_ops.conf.receive_cast = receiveCast_Dispatcher;
  join_ops.conf.receive_send = receiveSend_Dispatcher;
  join_ops.conf.accepted_view = acceptedView_Dispatcher;
  join_ops.conf.heartbeat = heartbeat_Dispatcher;
  join_ops.conf.block = block_Dispatcher;
  join_ops.conf.exit = exit_Dispatcher;

  join_ops.env = (void*) this;

  /*
   * Join the group.
   */
  err = hot_ens_Join(&join_ops, &_gctx);
  check_err("Maestro_Group::Maestro_Group: hot_ens_Join");  

  /*
   * Wait for the first view.
   */
  _sema.dec();
}


Maestro_Group::~Maestro_Group()
{
  _state = LEAVING;

  err = hot_ens_Leave(_gctx);
  check_err("Maestro_Group::leave: hot_ens_Leave");

  _sema.dec();
}


void 
Maestro_Group::cast(Maestro_Message &msg)
{
  if (_state != NORMAL) {
    error->panic("Maestro_Group::cast: Can't send messages in this state");
  }

  int msgType = CAST;  
  msg << msgType;

  hot_ens_MsgSendView sendView;
  err = hot_ens_Cast(_gctx, msg.getHotMsg(), &sendView);
  check_err("Maestro_Group::cast: hot_ens_Cast");
}


void 
Maestro_Group::send(Maestro_EndpID &dest, Maestro_Message &msg)
{
  if (_state != NORMAL) {
    error->panic("Maestro_Group::send: Can't send messages in this state");
  }

  hot_ens_MsgSendView sendView;
  hot_endpt_t dest_endp = dest.getHotEndpt();
  err = hot_ens_Send(_gctx, &dest_endp, msg.getHotMsg(), &sendView);
  check_err("Maestro_Group::send: hot_ens_Send");
}


void 
Maestro_Group::scast(Maestro_Message &msg)
{
  if (_state != NORMAL) {
    error->panic("Maestro_Group::scast: Can't send messages in this state");
  }

  int msgType = SCAST;  
  msg << msgType;

  hot_ens_MsgSendView sendView;
  err = hot_ens_Cast(_gctx, msg.getHotMsg(), &sendView);
  check_err("Maestro_Group::cast: hot_ens_Cast");
}
 

void 
Maestro_Group::lsend(Maestro_EndpList &destinations, Maestro_Message &msg)
{
  if (_state != NORMAL) {
    error->panic("Maestro_Group::lsend: Can't send messages in this state");
  }

  msg << destinations;

  int msgType = LSEND;  
  msg << msgType;

  hot_ens_MsgSendView sendView;
  err = hot_ens_Cast(_gctx, msg.getHotMsg(), &sendView);
  check_err("Maestro_Group::lsend: hot_ens_Cast");
}


/*
 * Received a multicast message.
 */
void 
Maestro_Group::receiveCast_Dispatcher(hot_gctx_t gctx, void *env, 
				      hot_endpt_t *origin, hot_msg_t msg)
{
  Maestro_Group *mbr = (Maestro_Group*) env;
  mbr->_gctx = gctx;

  Maestro_EndpID sender(*origin);
  Maestro_Message receivedMsg(msg);

  int msgType;
  receivedMsg >> msgType;

  switch (msgType) {
  case CAST: 
    {
      mbr->_groupListener->receivedCast(sender, receivedMsg);
      mbr->_myStateVersionNumber.nmsgs++;
      break;
    }
  
  case LSEND: 
    {
      Maestro_EndpList destinations;
      receivedMsg >> destinations;
      if (destinations.contains(mbr->_view.myEndpID)) {
	mbr->_groupListener->receivedLsend(sender, receivedMsg);
	mbr->_myStateVersionNumber.nmsgs++;
      }
      break;
    }

  case SCAST: 
    {
      if (mbr->_options.serverFlag) {
	mbr->_groupListener->receivedScast(sender, receivedMsg);
	mbr->_myStateVersionNumber.nmsgs++;
      }
      break;
    }

  case STATE: 
    {
      Maestro_ViewID viewID;
      receivedMsg >> viewID;
      if (mbr->_view.viewID != viewID) {
	/*
	 * State message for a wrong view -- drop it.
	 */
	break;
      }
      
      int server;
      receivedMsg >> server;
      
      /*
       * Got a state message from a server.
       */
      if (server) { 
	mbr->_view.servers += sender;
	
	/*
	 * If I am a server, check the state versions.
	 */
	if (mbr->_options.serverFlag && (sender != mbr->_view.myEndpID)) {
	  StateVersionNumber version;
	  receivedMsg >> version;
	  
	  if (mbr->_highestVersionNumber < version) {
	    mbr->_highestVersionNumber = version;
	    receivedMsg >> mbr->_mostAdvancedState;
	  }
	}      
      }
      /*
       * Got a state message from a client.
       */  
      else {
	mbr->_view.clients += sender;
      }

      /*
       * If got all state messages, can deliver the view.
       */
      if (sender != mbr->_view.myEndpID) {
	++mbr->_nReceivedStateMsgs;
      }

      if (mbr->_nReceivedStateMsgs == mbr->_view.nmembers - 1) {
	/*
	 * Servers with outdated state update their state first.
	 */
	if (mbr->_options.serverFlag && 
	    mbr->_myStateVersionNumber < mbr->_highestVersionNumber)
	  {
	    mbr->_groupListener->setState(mbr->_mostAdvancedState);
	    mbr->_myStateVersionNumber = mbr->_highestVersionNumber;
	  }
	/*
	 * Deliver the view.
	 */
	mbr->_myStateVersionNumber.nviews++;
	mbr->_state = NORMAL;
	mbr->_groupListener->acceptedView(mbr->_view);

	/* 
	 * Let the constructor return.
	 */
	if (!mbr->_gotView) {
	  mbr->_gotView = 1;
	  mbr->_sema.inc();
	}
      }
      break;
    }
  
  default:
    /*
     * Bad message -- drop it.
     */     
    break;
  }  
}

    
/*
 * Received a point-to-point message.
 */
void 
Maestro_Group::receiveSend_Dispatcher(hot_gctx_t gctx, void *env, 
				      hot_endpt_t *origin, hot_msg_t msg)
{
  Maestro_Group *mbr = (Maestro_Group*) env;
  mbr->_gctx = gctx;

  Maestro_EndpID sender(*origin);
  Maestro_Message receivedMsg(msg);

  mbr->_groupListener->receivedSend(sender, receivedMsg);
  mbr->_myStateVersionNumber.nmsgs++;
}

    
/*
 * Accepted a new view.
 */
void 
Maestro_Group::acceptedView_Dispatcher(hot_gctx_t gctx, void *env, 
				       hot_view_state_t *view_state)
{
  Maestro_Group *mbr = (Maestro_Group*) env;
  mbr->_gctx = gctx;

  mbr->_nReceivedStateMsgs = 0;
  
  Maestro_EndpList tmp(view_state->members, view_state->nmembers);
  mbr->_view.members <<= tmp;
  mbr->_view.coordinator = mbr->_view.members[0];

  mbr->_view.nmembers = view_state->nmembers;    
  mbr->_view.myRank = view_state->rank;
  mbr->_view.myEndpID = mbr->_view.members[mbr->_view.myRank];  

  mbr->_view.clients.clear();
  mbr->_view.servers.clear();

  if (mbr->_options.serverFlag) {
    mbr->_view.servers += mbr->_view.myEndpID;
  }
  else {
    mbr->_view.clients += mbr->_view.myEndpID;
  }    
 
  Maestro_String proto_str(view_state->protocol);
  mbr->_view.protocol = proto_str;
  
  Maestro_String version_str(view_state->version);
  mbr->_view.version = version_str;

  Maestro_String group_name(view_state->group_name);
  mbr->_view.groupName = group_name;

  Maestro_String params_str(view_state->params);
  mbr->_view.params = params_str;

  mbr->_view.primaryFlag = view_state->primary;
  mbr->_view.viewID = view_state->view_id;

  /* 
   * If this is a singleton view, deliver right away.
   */
  if (mbr->_view.nmembers == 1) {
    /*
     * Deliver the view.
     */
    mbr->_myStateVersionNumber.nviews++;
    mbr->_state = NORMAL;
    mbr->_groupListener->acceptedView(mbr->_view);
    /* 
     * Let the constructor return.
     */
    if (!mbr->_gotView) {
      mbr->_gotView = 1;
      mbr->_sema.inc();
    }
  }
  /* 
   * Multicast our state.
   */
  else {
    Maestro_Message msg;
    
    if (mbr->_options.serverFlag) {
      Maestro_Message stateMsg;
      mbr->_groupListener->getState(stateMsg);
      msg << stateMsg;
      msg << mbr->_myStateVersionNumber;
    }
    
    int msgType = STATE;
    msg << mbr->_options.serverFlag << mbr->_view.viewID << msgType;
    
    hot_ens_MsgSendView sendView;
    mbr->err = hot_ens_Cast(mbr->_gctx, msg.getHotMsg(), &sendView);
    if (mbr->err != HOT_OK) {
      mbr->error->panic(mbr->err,  
			"Maestro_Group::acceptedView_Dispatcher: hot_ens_Cast");
    }
  }
}

  
/*
 * The group is blocked for a view change.
 */
void 
Maestro_Group::block_Dispatcher(hot_gctx_t gctx, void *env)
{
  Maestro_Group *mbr = (Maestro_Group*) env;
  mbr->_gctx = gctx;
  mbr->_groupListener->blocked();
  mbr->_state = BLOCKED;
}


/*
 * The member has left the group.
 */
void 
Maestro_Group::exit_Dispatcher(hot_gctx_t gctx, void *env)
{
  Maestro_Group *mbr = (Maestro_Group*) env;;
  mbr->_gctx = gctx;

  assert(mbr->_state == LEAVING);
  mbr->_sema.inc();
}
