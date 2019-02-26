// $Header$

#ifndef __MAESTRO_CLT_SVR_H__
#define __MAESTRO_CLT_SVR_H__

/********************************************************************
 *                                                                  *
 * Author:  Alexey Vaysburd  December 96                            *
 *                                                                  *
 * Contents:  Definition of Maestro_ClSv class (client/server group)*
 *                                                                  *
 ********************************************************************/

/* jsd: added cond */
#if defined(__GNUC__)
#pragma interface
#endif

#include <avango/ensemble/maestro/Maestro_GroupMember.h>

/****************************************************************************/
/**********************                       *******************************/
/**********************      Maestro_ClSv      *******************************/
/**********************                       *******************************/
/****************************************************************************/

/****************************************************************************/
//
//
//         State Transition Diagram for Clients/Servers with Xfer
//         ------------------------------------------------------
//
//
//
// [NB:  when VIEW arrives, servers, clients, xferServers_nextView,
//      servers_nextView should be updated: in particular,
//      servers_nextView = servers, xferServers_nextView = xferServers]
//
//    join(mbrshipOption) =
//         (state is INITIAL) -->
//              <send joinRequest msg to the group>
//              state = JOINING
//
//    becomeServer() =
//         (myEndpID is in Clients) -->
//              (myEnpID is Coordinator) -->
//                   (nmembers is 1) -->
//                        <add myEndpID to Servers>
//                        <assert:  myEndpID is not in xferServers>
//                        state = SERVER_NORMAL
//                   (else) -->
//                        <BECOME_SERVER(myEndpID)>
//                        state = BECOMING_SERVER
//              (else) -->
//                   <send becomeServer msg to Coordinator>
//                   state = BECOMING_SERVER
//
//    xferDone() =
//         (myEndpID is in xferServers) -->
//              (myEndpID is Coordinator) -->
//                   (nmembers is 1) -->
//                        <remove myEndpID from xferServers>
//                        <add myEndpID to Servers>
//                        state = SERVER_NORMAL
//                   (else) -->
//                        <XFER_DONE(myEndpID)>
//                        state = SERVER_XFER_DONE
//              (else) -->
//                   <send xferDone msg to Coordinator>
//                   state = SERVER_XFER_DONE
//
//    /* incoming becomeServer msg results in BECOME_SERVER invoked */
//    BECOME_SERVER(S) =
//         (S is in Clients) -->
//              <assert:  myEndpID == Coordinator>
//              (xferOption of S is NO_XFER) -->
//                   <add S to Servers_nextView>
//              (else) -->
//                   <add S to xferServers_nextView>
//              <request a new view to be installed>
//
//    /* incoming xferDone msg results in XFER_DONE invoked */
//    XFER_DONE(S) =
//         (S is in xferServers) -->
//              <assert:  myEndpID == Coordinator>
//              <remove S from xferServers_nextView>
//              <add S to Servers_nextView>
//              <request a new view to be installed>
//
//
// JOINING:
// --------
//         AcceptedView:
//              <assert:  myEndpID is in Clients>
//              (mbrshipOption is CLIENT) -->
//                   state = CLIENT_NORMAL
//              (mbrshipOption is SERVER) -->
//                   <becomeServer()>
//                   <assert:  state = BECOMING_SERVER or SERVER_NORMAL>
//
// BECOMING_SERVER:
// ----------------
//         AcceptedView:
//              (myEndpID is in Servers) -->
//                   <assert:  myEndpID is in newServers>
//                   <assert:  xferOption is NO_XFER>
//                   state = SERVER_NORMAL
//              (myEndpID is in xferServers) -->
//                   <assert:  myEndpID is in newXferServers>
//                   <assert:  xferOption is not NO_XFER>
//                   (Servers list is empty) -->
//                        <xferDone()>
//                        <assert:  state = SERVER_NORMAL or SERVER_XFER_DONE>
//                   (else) -->
//                        state = SERVER_XFER
//              (else) -->
//                   <assert:  myEndpID is in Clients>
//                   (Coordinator has changed) -->
//                        <becomeServer()>
//                        <assert:  state = BECOMING_SERVER or SERVER_NORMAL>
//
// SERVER_XFER:
// ------------
//         AcceptedView:
//              (myEndpID is in xferServers) -->
//                   (Servers list is empty) -->
//                        <xferDone()>
//                        <assert:  state = SERVER_NORMAL or SERVER_XFER_DONE>
//              (else) -->
//                   /* this may happen when merging with another group */
//                   <assert:  myEndpID is in Clients>
//                   <assert:  Coordinator has changed>
//                   <becomeServer()>
//                   <assert:  state = BECOMING_SERVER or SERVER_NORMAL>
//
// SERVER_XFER_DONE:
// -----------------
//         AcceptedView:
//              (myEndpID is in Servers) -->
//                   state = SERVER_NORMAL
//              (myEndpID is in xferServers) -->
//                   (Coordinator has changed) -->
//                        <xferDone()>
//                        <assert:  state = SERVER_NORMAL or SERVER_XFER_DONE>
//              (else) -->
//                   /* this may happen when merging with another group */
//                   <assert:  myEndpID is in Clients>
//                   <assert:  Coordinator has changed>
//                   <becomeServer()>
//                   <assert:  state = BECOMING_SERVER or SERVER_NORMAL>
//
// SERVER_NORMAL:
// --------------
//         AcceptedView:
//              /* this may happen when merging with another group */
//              (myEndpID is not in Servers) -->
//                   <assert:  myEndpID is in Clients>
//                   <assert:  Coordinator has changed>
//                   <becomeServer()>
//                   <assert:  state = BECOMING_SERVER or SERVER_NORMAL>
//
/*****************************************************************************/

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1681
#pragma set woff 1682
#pragma set woff 3624
#endif

// NOTE:  It is possible that the group coordinator will not be a server

// Client/server states.
typedef enum
{
    MAESTRO_CLSV_STATE_INITIAL,
    MAESTRO_CLSV_STATE_JOINING,
    MAESTRO_CLSV_STATE_CLIENT_NORMAL,
    MAESTRO_CLSV_STATE_BECOMING_SERVER,
    MAESTRO_CLSV_STATE_SERVER_XFER,
    MAESTRO_CLSV_STATE_SERVER_XFER_DONE,
    MAESTRO_CLSV_STATE_SERVER_NORMAL,
    MAESTRO_CLSV_STATE_LEAVING
} Maestro_ClSvState;

// Membership options.
typedef enum
{
    MAESTRO_CLIENT,
    MAESTRO_SERVER
} Maestro_MbrshipOption;

// View types.
typedef enum
{
    MAESTRO_NO_XFER,        // normal view: all messages are allowed
    MAESTRO_FREE_XFER,      // all messages are allowed
    MAESTRO_PROTECTED_XFER, // only xfer & safe msgs allowed
    MAESTRO_ATOMIC_XFER,    // only xfer messages are allowed
} Maestro_ViewType;

typedef Maestro_ViewType Maestro_XferType;

// MH: Redefined this in order to eliminate type warnings
// Msg xfer safety types.
// typedef enum {
//  MAESTRO_MSG_GENERIC = MAESTRO_FREE_XFER,   // OK for free/no xfer
//  MAESTRO_MSG_SAFE = MAESTRO_PROTECTED_XFER, // OK for free/protected/no xfer
//  MAESTRO_MSG_XFER = MAESTRO_ATOMIC_XFER     // OK for xfers of all types
//} Maestro_MsgXferSafety;
typedef Maestro_ViewType Maestro_MsgXferSafety;
#define MAESTRO_MSG_GENERIC MAESTRO_FREE_XFER   // OK for free/no xfer
#define MAESTRO_MSG_SAFE MAESTRO_PROTECTED_XFER // OK for free/protected/no xfer
#define MAESTRO_MSG_XFER MAESTRO_ATOMIC_XFER    // OK for xfers of all types

// msg types
typedef enum
{
    // user data messages:
    MAESTRO_CLSV_CAST,  // msg for all group members
    MAESTRO_CLSV_SCAST, // msg for all servers and other specified members
    MAESTRO_CLSV_LSEND, // msg for specified members
    MAESTRO_CLSV_SEND,  // pt-2-pt message

    // special messages:
    MAESTRO_CLSV_BECOME_SERVER, // becomeServer msg
    MAESTRO_CLSV_XFER_DONE,     // xferDone msg
    MAESTRO_CLSV_ASK_STATE,     // askState msg
    MAESTRO_CLSV_SEND_STATE     // sendState msg
} Maestro_ClSvMsgType;

struct Maestro_ClSv_ViewData : Maestro_GrpMemb_ViewData
{
    Maestro_ClSv_ViewData();
    Maestro_ClSv_ViewData(Maestro_GrpMemb_ViewData& gmView);
    Maestro_EndpList servers, newServers, departedServers;
    Maestro_EndpList xferServers, newXferServers, departedXferServers;
    Maestro_EndpList clients, newClients, departedClients;
    Maestro_EndpID serverCoordinator, oldServerCoordinator;
    Maestro_ViewType myXferType, viewType;
    Maestro_ClSvState state, oldState;
    int myServerRank, myOldServerRank;
    Maestro_MbrshipOption myMbrshipType; // client or server

    int stateCounter; // counts the number of cast and send upcalls
                      // added by Roy Friedman

    // startXfer specifies whether xfer should be (re)started.  By definition,
    // startXfer = (state == MAESTRO_CLSV_STATE_SERVER_XFER) &&
    //             (oldState != MAESTRO_CLSV_STATE_SERVER_XFER)
    int startXfer;
};

struct Maestro_ClSv_MsgOptions
{
    Maestro_ClSv_MsgOptions() { msgXferSafety = MAESTRO_MSG_GENERIC; }
    Maestro_MsgXferSafety msgXferSafety;
    Maestro_EndpList destList; // used by scast, lsend
};

class Maestro_ClSv_Options : public Maestro_GrpMemb_Options
{
  public:
    Maestro_ClSv_Options() { reset(); }
    Maestro_MbrshipOption mbrshipType; // MAESTRO_CLIENT or MAESTRO_SERVER
    Maestro_XferType xferType;         // protection level for state xfer

  private:
    int viewMsgFlag;
    int client;
    void reset()
    {
        mbrshipType = MAESTRO_SERVER;
        xferType = MAESTRO_NO_XFER;
        viewMsgFlag = 1;
        client = 0;
    }
};

struct Maestro_ClSv_DelayedMessage : public Maestro_Base
{
    Maestro_ClSv_DelayedMessage() {}

    Maestro_ClSv_DelayedMessage(Maestro_Message& delayed_msg)
    {
        msg <<= delayed_msg;
        p2p = 0;
    }

    Maestro_ClSv_DelayedMessage(Maestro_EndpID& delayed_dest, Maestro_Message& delayed_msg)
    {
        msg <<= delayed_msg;
        dest = delayed_dest;
        p2p = 1;
    }

    int operator==(Maestro_ClSv_DelayedMessage&) { return 0; }

    Maestro_Message msg;
    Maestro_EndpID dest;
    int p2p;
};

typedef Maestro_OrderedSet(Maestro_ClSv_DelayedMessage) Maestro_ClSv_DelayedMessageList;

class Maestro_ClSv : public Maestro_GroupMember
{
  public:
    Maestro_ClSv(Maestro_ClSv_Options& ops);

    Maestro_ClSv(Maestro_ClSv& hclsv);
    virtual ~Maestro_ClSv() {}
    virtual Maestro_ClSv& operator=(Maestro_ClSv& hclsv);

    virtual int isServer() { return (myView.myMbrshipType == MAESTRO_SERVER); }
    virtual int isClient() { return (myView.myMbrshipType == MAESTRO_CLIENT); }

    // Join the group.
    virtual void join();

    // State transfer downcalls.
    MAESTRO_INLINE virtual void askState(Maestro_EndpID& server, Maestro_Message& msg);
    MAESTRO_INLINE virtual void sendState(Maestro_EndpID& joiningServer, Maestro_Message& msg);
    virtual void xferDone() { xferDone(myView); }

    // Sending messages.
    MAESTRO_INLINE virtual void cast(Maestro_Message& msg);
    MAESTRO_INLINE virtual void cast(Maestro_Message& msg, Maestro_MsgSendView& sendView);

    MAESTRO_INLINE virtual void cast(Maestro_Message& msg, Maestro_MsgSendView& sendView, Maestro_ClSv_MsgOptions& msgOps);

    MAESTRO_INLINE virtual void send(Maestro_EndpID& dest, Maestro_Message& msg);
    MAESTRO_INLINE virtual void send(Maestro_EndpID& dest, Maestro_Message& msg, Maestro_MsgSendView& sendView);

    MAESTRO_INLINE virtual void send(Maestro_EndpID& dest, Maestro_Message& msg, Maestro_MsgSendView& sendView, Maestro_ClSv_MsgOptions& msgOps);

    MAESTRO_INLINE virtual void scast(Maestro_Message& msg);
    MAESTRO_INLINE virtual void scast(Maestro_Message& msg, Maestro_MsgSendView& sendView);

    MAESTRO_INLINE virtual void scast(Maestro_Message& msg, Maestro_MsgSendView& sendView, Maestro_ClSv_MsgOptions& msgOps);

    MAESTRO_INLINE virtual void lsend(Maestro_Message& msg, Maestro_MsgSendView& sendView, Maestro_ClSv_MsgOptions& msgOps);

    void leave()
    {
        myView.state = MAESTRO_CLSV_STATE_LEAVING;
        Maestro_GroupMember::leave();
    }

  protected:
    /**************** Client-Server interface upcalls ***********************/

    // View callbacks:

    virtual void clSv_ViewMsg_Callback(Maestro_ClSv_ViewData&,
                                       /*OUT*/ Maestro_Message&)
    {
    }

    virtual void clSv_AcceptedView_Callback(Maestro_ClSv_ViewData&, Maestro_Message&) {}
    // Message callbacks:

    virtual void clSv_ReceiveCast_Callback(Maestro_EndpID&, Maestro_Message&) {}

    virtual void clSv_ReceiveScast_Callback(Maestro_EndpID&, Maestro_Message&) {}

    virtual void clSv_ReceiveSend_Callback(Maestro_EndpID&, Maestro_Message&) {}

    virtual void clSv_ReceiveLsend_Callback(Maestro_EndpID&, Maestro_Message&) {}

    // The group is blocked for a view change:

    virtual void clSv_Block_Callback() {}

    // The member has left the group:

    virtual void clSv_Exit_Callback() {}

    // Heartbeat callback:

    virtual void clSv_Heartbeat_Callback(unsigned) {}

    // State transfer callbacks:

    virtual void askState_Callback(Maestro_EndpID&, Maestro_Message&) {}

    virtual void rcvState_Callback(Maestro_EndpID&, Maestro_Message&) {}

    /************************************************************************/

    virtual void becomeServer(Maestro_ClSv_ViewData& vd);
    virtual void xferDone(Maestro_ClSv_ViewData& vd);

  private:
    Maestro_ClSv_ViewData myView;
    Maestro_EndpList servers_nextView, xferServers_nextView;
    Maestro_ViewType viewType_nextView;
    Maestro_ClSv_DelayedMessageList xferDelayedMessages;

    // Added by Roy Friedman for new state xfer
    int validStateGuro;
    Maestro_Message stateMsg;
    int maxStateCounter;
    Maestro_EndpID stateGuro;
    int numStates;

    /**************************** Callbacks ************************************/

    // View callbacks:

    MAESTRO_INLINE void grpMemb_ViewMsg_Callback(Maestro_GrpMemb_ViewData& viewData,
                                                 /*OUT*/ Maestro_Message& viewMsg);

    MAESTRO_INLINE void
    grpMemb_ViewStateInfo_Callback(Maestro_EndpID /*in*/& origin, Maestro_GrpMemb_ViewData /*in*/& viewData, Maestro_Message /*in*/& msg, Maestro_Message /*out*/& next_view_msg, int /*out*/& final);

    MAESTRO_INLINE void grpMemb_AcceptedView_Callback(Maestro_GrpMemb_ViewData& viewData, Maestro_Message& msg);

    // Message Callbacks:

    MAESTRO_INLINE void grpMemb_ReceiveCast_Callback(Maestro_EndpID& origin, Maestro_Message& msg);

    MAESTRO_INLINE void grpMemb_ReceiveSend_Callback(Maestro_EndpID& origin, Maestro_Message& msg);

    // The group is blocked for a view change:

    void grpMemb_Block_Callback() { clSv_Block_Callback(); }

    // The member has left the group:
    virtual void grpMemb_Exit_Callback()
    {
        // cout << "grpMemb_Exit_Callback:  setting state to INITIAL" << endl;
        myView.state = MAESTRO_CLSV_STATE_INITIAL;
        clSv_Exit_Callback();
    }

    // Heartbeat Callback:

    void grpMemb_Heartbeat_Callback(unsigned time) { clSv_Heartbeat_Callback(time); }

    /*************************************************************************/

    // Special events:

    virtual void BECOME_SERVER(Maestro_ClSv_ViewData& vd, Maestro_EndpID& newServ, Maestro_ViewType xferOpt);

    virtual void XFER_DONE(Maestro_ClSv_ViewData& vd, Maestro_EndpID& newServ);

    void init();

    // Delay messages during state xfer.
    virtual void delayMsg(Maestro_Message& msg);
    virtual void delayMsg(Maestro_EndpID& dest, Maestro_Message& msg);
    virtual void sendDelayedMsgs();
};

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1681
#pragma reset woff 1682
#pragma reset woff 3624
#endif

#endif // __MAESTRO_CL_SV_H__
