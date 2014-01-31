// $Header$

#ifndef __MAESTRO_GROUP_MEMBER_H__
#define __MAESTRO_GROUP_MEMBER_H__

/********************************************************************
 *                                                                  *
 * Author:  Alexey Vaysburd  December 96                            *
 *                                                                  *
 * Contents:  Definition of the Maestro_GroupMember class           *
 *                                                                  *
 ********************************************************************/

/* jsd: added cond */
#if defined(__GNUC__)
#pragma interface
#endif

#include <avango/ensemble/maestro/Maestro_Types.h>
// keep order
#include <avango/ensemble/hot/hot_ens.h>

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1234
#pragma set woff 1401
#pragma set woff 1681
#pragma set woff 1682
#pragma set woff 3624
#endif

#define MAESTRO_MESSAGE_MAGIC_VID 929834

typedef enum {
    MAESTRO_MODE_SINGLE_THREADED,
    MAESTRO_MODE_MULTI_THREADED
} Maestro_ThreadMode;

class Maestro_ViewID : virtual public Maestro_Base {
    friend class Maestro_GroupMember;

public:
    Maestro_ViewID();
    Maestro_ViewID(hot_view_id &view_id);
    Maestro_ViewID& operator= (Maestro_ViewID &vid2);
    Maestro_ViewID& operator= (hot_view_id &view_id);
    int operator== (Maestro_ViewID &vid2);
    int operator!= (Maestro_ViewID &vid2);
    int operator< (Maestro_ViewID &vid2);
    int operator> (Maestro_ViewID &vid2);
    void operator<< (Maestro_Message& msg);
    void operator>> (Maestro_Message& msg);
    void operator>> (std::ostream& out) const;
    int messageMagic() const { return MAESTRO_MESSAGE_MAGIC_VID; }
protected:
    hot_view_id vid;
};

struct Maestro_GrpMemb_ViewData {
    Maestro_GrpMemb_ViewData();
  
    Maestro_String version;
    Maestro_String groupName;
    Maestro_EndpID myEndpID;
    Maestro_EndpList members, newMembers, departedMembers;
    int nmembers, myRank, myOldRank;
    Maestro_EndpID coordinator, oldCoordinator;
    Maestro_String protocol;
    int groupdFlag;  
    Maestro_ViewID viewID;
    Maestro_String params;
    int primaryFlag;
    Maestro_IntList clientFlags;
    int nGrpClients, nGrpMembers;
    void dump(char *title);
};

class Maestro_GrpMemb_Options : Maestro_Options {
public:
    
    Maestro_GrpMemb_Options() { reset(); }
    unsigned heartbeatRate;
    Maestro_String groupName;      
    Maestro_String protocol;
    Maestro_String properties;
    Maestro_String params;
    Maestro_String transports;
    Maestro_String outboard;
    Maestro_ErrorHandler *errorHandler;
    int groupdFlag;
    int viewMsgFlag;
    char **argv;
    void dump(char *title);
    int client;
    int debug;

private:
    
    void reset() {
	heartbeatRate = MAESTRO_DEFAULT_HEARTBEAT_RATE; 

	Maestro_String props_str((char*) MAESTRO_DEFAULT_PROTOCOL_PROPERTIES);
	properties = props_str;

	Maestro_String grpnam_str("Maestro-default-group");
	groupName = grpnam_str;
	
	Maestro_String transp_str((char*) MAESTRO_DEFAULT_TRANSPORTS);
	transports = transp_str;
	
	errorHandler = 0;
	groupdFlag = 0;
	viewMsgFlag = 1;
	argv = 0;
	client = 0;
	debug = 0;
	outboard = MAESTRO_DEFAULT_OUTBOARD_MODULE;
    }
};

enum Maestro_GrpMemb_MsgType {
    MAESTRO_GRPMBR_MSG_TYPE_DATA,
    MAESTRO_GRPMBR_MSG_TYPE_VIEW,
	MAESTRO_GRPMBR_MSG_TYPE_STATE
};

struct Maestro_GrpMemb_MsgOptions : Maestro_Options {
    // no options at present
};

// The view in which the message will be delivered.
// This type mirrors the corresp. type in hot_ens.h.
enum Maestro_MsgSendView {
    MAESTRO_MSG_SEND_UNSPECIFIED_VIEW,
    MAESTRO_MSG_SEND_CURRENT_VIEW,
    MAESTRO_MSG_SEND_NEXT_VIEW
};

class Maestro_GroupMember : virtual public Maestro_Base {
public:
    
    // Initialize the group-member object.
    Maestro_GroupMember(Maestro_GrpMemb_Options &membOps);
    
    Maestro_GroupMember(Maestro_GroupMember& hgm);
    virtual ~Maestro_GroupMember();
    virtual Maestro_GroupMember& operator= (Maestro_GroupMember& hgm);
    
    // Join the group.
    virtual void join();
    
    // Send point-to-point message.
    MAESTRO_INLINE virtual void send(Maestro_EndpID &dest, Maestro_Message &msg);
    
    MAESTRO_INLINE virtual void send(Maestro_EndpID &dest, Maestro_Message &msg,
			     Maestro_MsgSendView &sendView);
    
    MAESTRO_INLINE virtual void send(Maestro_EndpID &dest, Maestro_Message &msg, 
			     Maestro_MsgSendView &sendView,
			     Maestro_GrpMemb_MsgOptions &msgOps);
    
    // Send multicast message.
    MAESTRO_INLINE virtual void cast(Maestro_Message &msg);
    
    MAESTRO_INLINE virtual void cast(Maestro_Message &msg, 
			     Maestro_MsgSendView &sendView);
    
    MAESTRO_INLINE virtual void cast(Maestro_Message &msg, 
			     Maestro_MsgSendView &sendView, 
			     Maestro_GrpMemb_MsgOptions &msgOps);
    
    // Report group members as failure-suspected.
    virtual void suspect(Maestro_EndpList &suspects);
    
    // Request a change of group protocol.
    virtual void changeProtocol(Maestro_String &protocol);

    // Request a change of group protocol (specify properties).
    virtual void changeProperties(Maestro_String &properties);
    
    // Request a new view to be installed.
    virtual void requestNewView();
    
    // Get endpoint ID of this group member.
    virtual void myEndpID(Maestro_EndpID &eid);
    
    // Leave the group.
    virtual void leave();

    void dump(char *title);

protected:
    
    /*********************** Interface Callbacks *****************************/
    
    // Message callbacks:

    virtual void 
    grpMemb_ReceiveCast_Callback(Maestro_EndpID &,
				 Maestro_Message &) {}
    
    virtual void 
    grpMemb_ReceiveSend_Callback(Maestro_EndpID &,
				 Maestro_Message &) {}
    
    // View callbacks:
    
    virtual void
    grpMemb_ViewMsg_Callback(Maestro_GrpMemb_ViewData &,
			     Maestro_Message &) {}

    virtual void
    grpMemb_MergeState_Callback(Maestro_MessageList /*in*/ &,
				 Maestro_Message /*out*/ &) {}

	virtual void
    grpMemb_ViewStateInfo_Callback(Maestro_EndpID /*in*/ &, Maestro_GrpMemb_ViewData /*in*/&,
			     Maestro_Message /*in*/ &, Maestro_Message /*out*/ &,
				 int /*out*/ &) {}


    virtual void 
    grpMemb_AcceptedView_Callback(Maestro_GrpMemb_ViewData &,
				  Maestro_Message &) {}

    // The group is blocked for a view change:
    
    virtual void
    grpMemb_Block_Callback() {}
    
    // The member has left the group:
    
    virtual void 
    grpMemb_Exit_Callback() {}
    
    // Heartbeat:
    
    virtual void
    grpMemb_Heartbeat_Callback(unsigned) {}
    
    
    /**********************************************************************/
    
    Maestro_GrpMemb_Options jops;
    int xfer_from_oldest;
private:
    
    /*********************************************************************/
    
    enum { GRPMEMB_BOGUS, 
	   GRPMEMB_JOINING, 
	   GRPMEMB_NORMAL, 
	   GRPMEMB_LEAVING 
    } mbrState;  

    int expectingViewMsg;
    int expectingViewState;
    Maestro_ViewID lastViewID;
    Maestro_EndpID lastViewMsgOrigin;

    Maestro_GrpMemb_ViewData myView;
    hot_gctx_t gctx;

    Maestro_Semaphore jl_sema;	// sema for joins + leaves
    enum { 
	JL_NORMAL,
	JL_JOINING,
	JL_LEAVING
    } jl_status;		// status of jl_sema

    // Setup a view state.
    void setupViewState(hot_view_state_t *view_state, 
			       /*OUT*/ Maestro_GrpMemb_ViewData &viewData);
    
    /********************* Callback dispatchers ****************************/
    
    // Received a multicast message.
    MAESTRO_INLINE static void receiveCast_Dispatcher(hot_gctx_t gctx, void *env, 
					      hot_endpt_t *origin, hot_msg_t msg);
    
    // Received a point-to-point message.
    MAESTRO_INLINE static void receiveSend_Dispatcher(hot_gctx_t gctx, void *env, 
					      hot_endpt_t *origin, hot_msg_t msg);
    
    // Accepted a new view.
    MAESTRO_INLINE static void acceptedView_Dispatcher(hot_gctx_t gctx, void *env, 
					       hot_view_state_t *view_state);
    
    // Got a heartbeat event (invoked periodically).
    // Current time (in milliseconds) is specified.
    static void heartbeat_Dispatcher(hot_gctx_t gctx, void *env, 
					    unsigned rate);
    
    // The group is blocked for a view change.
    MAESTRO_INLINE static void 
    block_Dispatcher(hot_gctx_t gctx, void *env);
    
    // The member has left the group.
    static void 
    exit_Dispatcher(hot_gctx_t gctx, void *env);
};

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1234
#pragma reset woff 1401
#pragma reset woff 1681
#pragma reset woff 1682
#pragma reset woff 3624
#endif

#endif // __MAESTRO_GROUP_MEMBER_H__
