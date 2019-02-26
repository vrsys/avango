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

#ifndef __MAESTRO_GROUP_H__
#define __MAESTRO_GROUP_H__

#include <avango/ensemble/maestro/Maestro_GroupMember.h>

#include <iostream>

struct Maestro_ViewData
{
    Maestro_String version;
    Maestro_String groupName;
    Maestro_ViewID viewID;
    Maestro_String params;
    Maestro_String protocol;
    int primaryFlag;

    Maestro_EndpID coordinator;
    Maestro_EndpID myEndpID;
    int myRank;

    int nmembers;
    Maestro_EndpList members;

    Maestro_EndpList servers;
    Maestro_EndpList clients;
};

struct Maestro_GroupOptions
{
    Maestro_GroupOptions();

    Maestro_String groupName;
    Maestro_String protocol;
    Maestro_String properties;
    Maestro_String params;
    Maestro_String transports;
    Maestro_String outboard;
    unsigned heartbeatRate;

    int groupdFlag;
    int serverFlag;
    char** argv;
    int debug;
};

class Maestro_GroupListener
{
  public:
    virtual ~Maestro_GroupListener() {}

    virtual void receivedSend(Maestro_EndpID& sender, Maestro_Message& msg) {}
    virtual void receivedCast(Maestro_EndpID& sender, Maestro_Message& msg) {}
    virtual void receivedLsend(Maestro_EndpID& sender, Maestro_Message& msg) {}
    virtual void receivedScast(Maestro_EndpID& sender, Maestro_Message& msg) {}

    virtual void acceptedView(Maestro_ViewData& view) {}
    virtual void blocked() {}

    virtual void getState(/*OUT*/ Maestro_Message& stateMsg) const {}
    virtual void setState(/*IN*/ Maestro_Message& stateMsg) {}
};

class Maestro_Group : virtual public Maestro_Base
{
  public:
    Maestro_Group(Maestro_GroupListener& groupListener, Maestro_GroupOptions& options);

    virtual ~Maestro_Group();

    virtual void cast(Maestro_Message& msg);
    virtual void send(Maestro_EndpID& dest, Maestro_Message& msg);
    virtual void scast(Maestro_Message& msg);
    virtual void lsend(Maestro_EndpList& destinations, Maestro_Message& msg);

  private:
    enum ProtocolState
    {
        INITIAL,
        NORMAL,
        BLOCKED,
        LEAVING
    } _state;

    enum MsgType
    {
        CAST,
        LSEND,
        SCAST,
        STATE
    };

    struct StateVersionNumber : public Maestro_Base
    {
        StateVersionNumber()
        {
            nmsgs = 0;
            nviews = 0;
        }

        virtual int operator<(const StateVersionNumber& vn) const { return ((nmsgs < vn.nmsgs) || ((nmsgs == vn.nmsgs) && (nviews < vn.nviews))); }

        virtual int operator==(const StateVersionNumber& vn) const { return ((nmsgs == vn.nmsgs) && (nviews == vn.nviews)); }

        void operator<<(Maestro_Message& msg) { msg >> nmsgs >> nviews; }
        void operator>>(Maestro_Message& msg) { msg << nviews << nmsgs; }
        int messageMagic() const { return 847373; }

        void pversion() { std::cout << "VERSION: nmsgs=" << nmsgs << " nviews=" << nviews << std::endl; }

        int nmsgs;
        int nviews;
    };

    Maestro_GroupOptions _options;
    Maestro_GroupListener* _groupListener;
    Maestro_ViewData _view;
    int _gotView;

    StateVersionNumber _myStateVersionNumber;
    StateVersionNumber _highestVersionNumber;
    Maestro_Message _mostAdvancedState;
    int _nReceivedStateMsgs;

    Maestro_Semaphore _sema;
    hot_gctx_t _gctx;

    static void receiveCast_Dispatcher(hot_gctx_t gctx, void* env, hot_endpt_t* origin, hot_msg_t msg);

    // Received a point-to-point message.
    static void receiveSend_Dispatcher(hot_gctx_t gctx, void* env, hot_endpt_t* origin, hot_msg_t msg);

    // Accepted a new view.
    static void acceptedView_Dispatcher(hot_gctx_t gctx, void* env, hot_view_state_t* view_state);

    static void heartbeat_Dispatcher(hot_gctx_t gctx, void* env, unsigned rate) {}

    // The group is blocked for a view change.
    static void block_Dispatcher(hot_gctx_t gctx, void* env);

    // The member has left the group.
    static void exit_Dispatcher(hot_gctx_t gctx, void* env);
};

#endif
