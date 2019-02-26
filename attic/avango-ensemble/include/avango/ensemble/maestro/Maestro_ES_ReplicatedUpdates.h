// $Header$
//
// Author:  Alexey Vaysburd, October 1997.
//
// Contents:  "Replicated Updates" ORB Execution Style.

#ifndef __MAESTRO_ES_REPLICATEDUPDATES_H__
#define __MAESTRO_ES_REPLICATEDUPDATES_H__

#include <avango/ensemble/maestro/Maestro_CSX.h>
#include <avango/ensemble/maestro/Maestro_Prim.h>
#include <avango/ensemble/maestro/Maestro_ORB.h>
#include <avango/ensemble/maestro/Maestro_ETC.h>
#include <avango/ensemble/maestro/Maestro_Adaptor.h>

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1681
#pragma set woff 1682
#pragma set woff 3624
#endif

/******************** Maestro_RUObjectAdaptor_Options *******************/

class Maestro_ES_ReplicatedUpdates;

struct Maestro_RUObjectAdaptor_Options
{
    Maestro_ORB_ObjectKey key;
    Maestro_ES_ReplicatedUpdates* orb;
};

/*************** Maestro_ES_ReplicatedUpdates_Options **************/

struct Maestro_ReplicatedUpdates_Options
{
    Maestro_ReplicatedUpdates_Options();
    Maestro_ORB_IIOPDispatcher* dispatcher;

    // Set if updates are allowed only in the primary (majority) component.
    int progressInPrimaryOnly;

    int nReplicas; // # replicas (determines quorum size)

    int stateTransfer;   // set if state transfer is needed
    int requestsCommute; // set if total ordering is not needed

    // Set if compound IOR's of all bound objects should be reinstaleld
    // in the ETC directory after membership changes.
    int reinstallIOR;

    unsigned hashSize; // hash size for the map of object bindings

    Maestro_String ORBName;
    Maestro_Etc* etc;
};

class Maestro_RUObjectAdaptor;

/********************** Maestro_ES_IIOPProfile ********************/

struct Maestro_ES_IIOPProfile : Maestro_Base
{
    int operator==(Maestro_ES_IIOPProfile& prof) { return (endp == prof.endp); }
    void operator>>(Maestro_Message& msg) { msg << port << host << endp; }
    void operator<<(Maestro_Message& msg) { msg >> endp >> host >> port; }
    int port;
    Maestro_CORBA_String host;
    Maestro_EndpID endp;
};

typedef Maestro_OrderedSet(Maestro_ES_IIOPProfile) Maestro_ES_IIOPProfileList;

/********************** Maestro_ES_ObjectState ********************/

struct Maestro_ES_ObjectState : Maestro_Base
{
    int operator==(Maestro_ES_ObjectState& state);
    void operator>>(Maestro_Message& msg);
    void operator<<(Maestro_Message& msg);
    Maestro_CORBA_Message stateMsg;
    Maestro_ORB_ObjectKey objKey;
};

typedef Maestro_OrderedSet(Maestro_ES_ObjectState) Maestro_ES_ObjectStateList;

/********************** Maestro_ES_RUObjectEntry ********************/

struct Maestro_ES_RUObjectEntry : Maestro_Base
{
    Maestro_ES_RUObjectEntry() {}
    Maestro_ES_RUObjectEntry(Maestro_ES_RUObjectEntry& objEntry);

    Maestro_ES_RUObjectEntry(Maestro_ORB_ObjectKey& _key, Maestro_RUObjectAdaptor* _obj);

    int operator==(Maestro_ES_RUObjectEntry& objEntry);

    Maestro_RUObjectAdaptor* obj;
    Maestro_ORB_ObjectKey key;
};

typedef Maestro_OrderedSet(Maestro_ES_RUObjectEntry) Maestro_ES_RUObjectEntryList;

/********************** Maestro_ES_IIOPRequest ********************/

struct Maestro_ES_IIOPRequest : Maestro_Base
{
    Maestro_ES_IIOPRequest() {}

    Maestro_ES_IIOPRequest(Maestro_ORB_RequestId& _reqId, Maestro_ORB_ObjectKey& _objKey, int _response_expected, Maestro_CORBA_String& _operation, Maestro_CORBA_Message& _reqBody);

    void operator<<(Maestro_Message& msg);
    void operator>>(Maestro_Message& msg);
    int operator==(Maestro_ES_IIOPRequest&) { return 0; }

    Maestro_ORB_RequestId reqId;
    Maestro_ORB_ObjectKey objKey;
    int response_expected;
    Maestro_CORBA_String operation;
    Maestro_CORBA_Message reqBody;
};

typedef Maestro_OrderedSet(Maestro_ES_IIOPRequest) Maestro_ES_IIOPRequestList;

/******************** Maestro_RUObjectAdaptor *******************/

// Object Adaptor for Replicated Updates ORB execution style.
// In an application, the abstract methods will be overloaded as needed.
class Maestro_RUObjectAdaptor
{
  public:
    Maestro_RUObjectAdaptor(Maestro_RUObjectAdaptor_Options& ops);
    virtual ~Maestro_RUObjectAdaptor();

    virtual Maestro_GIOP_ReplyStatusType update(Maestro_CORBA_String& operation, Maestro_CORBA_Message& request, Maestro_CORBA_Message& reply) = 0;

    virtual void pushState(Maestro_CORBA_Message&) {}
    virtual void getState(Maestro_CORBA_Message&) {}

  protected:
    Maestro_ES_ReplicatedUpdates* orb;
    Maestro_ORB_ObjectKey key;
};

/********************** Maestro_ES_ReplicatedUpdates ********************/

//
// Replicated Updates ORB Execution Style.
//
// * Requests are actively replicated:
//   All object replicas receive the same set of requests.
// * State transfer is done if needed.
// * Supports primary-partition or partitionable mode.
// * All replicas receive requests in the same order if needed.
//
class Maestro_ES_ReplicatedUpdates : virtual public Maestro_Communicator, virtual public Maestro_ORB_Base, virtual public Maestro_Base
{
  public:
    Maestro_ES_ReplicatedUpdates(Maestro_ReplicatedUpdates_Options& _ops);
    ~Maestro_ES_ReplicatedUpdates();

    // Activate the ORB:  Join the replicated-ORB group and
    // start accepting IIOP requests to bound objects.
    // No new objects will be allowed to bind while the ORB is active.
    virtual void activate();

    // Deactivate the ORB:  Stop accepting IIOP requests; leave the
    // replicated-ORB group.
    virtual void deactivate();

    // Bind an object to the ORB.  Objects can only be bound
    // while the ORB is not active.
    virtual Maestro_Status bind(Maestro_ORB_ObjectKey& key, Maestro_RUObjectAdaptor* obj);

    // Unbind an object from the ORB.  Objects can only be unbound
    // while the ORB is not active.
    virtual Maestro_Status unbind(Maestro_ORB_ObjectKey& key);

    // Unbind all object from the ORB.  Objects can only be unbound
    // while the ORB is not active.
    virtual Maestro_Status unbindAll();

    /******************** Maestro_Communicator Callbacks ******************/

    // A new view is about to be installed.
    // The coordinator can fill in the view message.
    void viewMsg_Callback(Maestro_CSX_ViewData& viewData,
                          /*OUT*/ Maestro_Message& viewMsg);

    // Received a new view.
    void acceptedView_Callback(Maestro_CSX_ViewData& viewData, Maestro_Message& msg);

    // Received a server-cast.
    void receiveScast_Callback(Maestro_EndpID& origin, Maestro_Message& msg);

    // Received a point-to-point message.
    void receiveSend_Callback(Maestro_EndpID& origin, Maestro_Message& msg);

    // The group is blocked.  A new view will be installed.
    void block_Callback();

    // Received a state-transfer request.
    void askState_Callback(Maestro_EndpID& origin, Maestro_XferID& xferID, Maestro_Message& requestMsg);

    // State transfer needs to be started by this member.
    void stateTransfer_Callback(Maestro_XferID& xferID);

    // Received a state-transfer message.
    void gotState_Callback(Maestro_XferID& xferID, Maestro_Message& stateMsg);

    /******************** Maestro_ORB_Base Callbacks ******************/

    // Received an IIOP request.
    void request_Callback(Maestro_ORB_RequestId& reqId,
                          Maestro_ORB_ObjectKey& objKey,
                          Maestro_CORBA_Boolean response_expected,
                          Maestro_CORBA_String& operation,
                          Maestro_GIOP_Principal& requesting_principal,
                          Maestro_CORBA_Message& reqBody);

    // Received an IIOP locate-request.
    void locateRequest_Callback(Maestro_ORB_RequestId& reqId, Maestro_ORB_ObjectKey& objKey);

  protected:
    // Lookup the (key, adaptor) binding in the ORB.
    // Return NULL if not found.
    // The ORB must be locked at this point.
    virtual Maestro_RUObjectAdaptor* lookup(Maestro_ORB_ObjectKey& objKey);

    // Depending on initialization options,
    // this may be initialized as an CSX or Prim Adaptor.
    Maestro_AdaptorBase* mbr;

    Maestro_ORB_IIOPDispatcher* dispatcher;
    Maestro_Lock mutex;
    Maestro_ReplicatedUpdates_Options ops;
    int IIOPPort;
    Maestro_CORBA_String IIOPHost;
    Maestro_ES_IIOPProfile myProfile;
    Maestro_ES_IIOPProfileList IIOPProfiles;
    Maestro_EndpID myEndpID;
    Maestro_CSX_ViewData view;
    Maestro_ES_RUObjectEntryList* bindings;
    int active;                              // set when the ORB is active.
    int blocked;                             // set when the group is blocked.
    Maestro_ES_IIOPRequestList bufferedReqs; // buffered requests
};

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1681
#pragma reset woff 1682
#pragma reset woff 3624
#endif

#endif //  __MAESTRO_ES_REPLICATEDUPDATES_H__
