// $Header$
//
// Author:  Alexey Vaysburd, October 1997.
//
// Contents: Simple ORB Execution Style.

#ifndef __MAESTRO_ES_SIMPLEORB_H__
#define __MAESTRO_ES_SIMPLEORB_H__

#include <avango/ensemble/maestro/Maestro_ORB.h>
#include <avango/ensemble/maestro/Maestro_ETC.h>

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1681
#pragma set woff 1682
#pragma set woff 3624
#endif

/******************** Maestro_SimpleORBObjectAdaptor_Options *******************/

class Maestro_ES_SimpleORB;

struct Maestro_SimpleORBObjectAdaptor_Options
{
    Maestro_ORB_ObjectKey key;
    Maestro_ES_SimpleORB* orb;
};

/*************** Maestro_ES_SimpleORB_Options **************/

struct Maestro_SimpleORB_Options
{
    Maestro_SimpleORB_Options();
    Maestro_ORB_IIOPDispatcher* dispatcher;

    // Set if IOR of the bound object should be instaleld
    // in the ETC directory.
    int installIOR;

    // Hash size for the map of object bindings.
    unsigned hashSize;

    Maestro_String ORBName;
    Maestro_Etc* etc;
};

class Maestro_SimpleORBObjectAdaptor;

/********************** Maestro_ES_SimpleObjectEntry ********************/

struct Maestro_ES_SimpleObjectEntry : Maestro_Base
{
    Maestro_ES_SimpleObjectEntry() {}
    Maestro_ES_SimpleObjectEntry(Maestro_ES_SimpleObjectEntry& objEntry);

    Maestro_ES_SimpleObjectEntry(Maestro_ORB_ObjectKey& _key, Maestro_SimpleORBObjectAdaptor* _obj);

    int operator==(Maestro_ES_SimpleObjectEntry& objEntry);

    Maestro_SimpleORBObjectAdaptor* obj;
    Maestro_ORB_ObjectKey key;
};

typedef Maestro_OrderedSet(Maestro_ES_SimpleObjectEntry) Maestro_ES_SimpleObjectEntryList;

/******************** Maestro_SimpleORBObjectAdaptor *******************/

// Object Adaptor for Simple ORB execution style.
// In an application, the abstract methods will be overloaded as needed.
class Maestro_SimpleORBObjectAdaptor
{
  public:
    Maestro_SimpleORBObjectAdaptor(Maestro_SimpleORBObjectAdaptor_Options& ops);
    virtual ~Maestro_SimpleORBObjectAdaptor();

    virtual Maestro_GIOP_ReplyStatusType update(Maestro_CORBA_String& operation, Maestro_CORBA_Message& request, Maestro_CORBA_Message& reply) = 0;

  protected:
    Maestro_ES_SimpleORB* orb;
    Maestro_ORB_ObjectKey key;
};

/********************** Maestro_ES_SimpleORB ********************/

//
// Simple ORB Execution Style.
//
// * Requests are processed by the local object (no reliability/replicaion).
//
class Maestro_ES_SimpleORB : virtual public Maestro_ORB_Base, virtual public Maestro_Base
{
  public:
    Maestro_ES_SimpleORB(Maestro_SimpleORB_Options& _ops);
    ~Maestro_ES_SimpleORB();

    // Activate the ORB:  Join the replicated-ORB group and
    // start accepting IIOP requests to bound objects.
    // No new objects will be allowed to bind while the ORB is active.
    virtual void activate();

    // Deactivate the ORB:  Stop accepting IIOP requests; leave the
    // replicated-ORB group.
    virtual void deactivate();

    // Bind an object to the ORB.  Objects can only be bound
    // while the ORB is not active.
    virtual Maestro_Status bind(Maestro_ORB_ObjectKey& key, Maestro_SimpleORBObjectAdaptor* obj);

    // Unbind an object from the ORB.  Objects can only be unbound
    // while the ORB is not active.
    virtual Maestro_Status unbind(Maestro_ORB_ObjectKey& key);

    // Unbind all object from the ORB.  Objects can only be unbound
    // while the ORB is not active.
    virtual Maestro_Status unbindAll();

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
    virtual Maestro_SimpleORBObjectAdaptor* lookup(Maestro_ORB_ObjectKey& objKey);
    void installIOR(Maestro_ORB_ObjectKey& key);

    Maestro_ORB_IIOPDispatcher* dispatcher;
    Maestro_Lock mutex;
    Maestro_SimpleORB_Options ops;
    Maestro_ES_SimpleObjectEntryList* bindings;
    Maestro_CORBA_String IIOPhost;
    int IIOPport;
    int active; // set when the ORB is active.
};

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1681
#pragma reset woff 1682
#pragma reset woff 3624
#endif

#endif //  __MAESTRO_ES_SIMPLEORB_H__
