// $Header$
//
// Support for ORB Core and arbitrary ORB's.
//
// Author:  Alexey Vaysburd, Sept. 1997.

#ifndef __MAESTRO_ORB_H__
#define __MAESTRO_ORB_H__

/* jsd: added cond */
#if defined(__GNUC__)
#pragma interface
#endif

#include <avango/ensemble/maestro/Maestro_CORBA.h>
#include <avango/ensemble/maestro/Maestro_GIOP.h>
#include <avango/ensemble/maestro/Maestro_IIOPBridge.h>

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1681
#pragma set woff 1682
#pragma set woff 3624
#endif

/******************** Maestro_ORB_ObjectKey ********************/

// Maestro_ORB_ObjectKey:  Globally unique object keys.
class Maestro_ORB_ObjectKey : virtual public Maestro_Base, virtual public Maestro_CORBA_Serializable
{
  public:
    Maestro_ORB_ObjectKey() {}
    Maestro_ORB_ObjectKey(Maestro_ORB_ObjectKey& objKey) : Maestro_Base(objKey), Maestro_CORBA_Base(objKey), Maestro_CORBA_Serializable(objKey) { key = objKey.key; }
    Maestro_ORB_ObjectKey(Maestro_CORBA_String& obj_key) { key = obj_key; }
    Maestro_ORB_ObjectKey(Maestro_CORBA_OctetSequence& obj_key);

    Maestro_ORB_ObjectKey& operator=(Maestro_ORB_ObjectKey& objKey);
    int operator==(Maestro_ORB_ObjectKey& objKey) { return (key == objKey.key); }

    void init(char* appName = NULL);
    int isValid();
    virtual Maestro_CORBA_Long hash();
    virtual void operator>>(Maestro_CORBA_String& str) { str = key; }
    void operator>>(Maestro_CORBA_Message& msg) { msg << key; }
    void operator<<(Maestro_CORBA_Message& msg) { msg >> key; }
    void operator>>(Maestro_Message& msg) { msg << key; }
    void operator<<(Maestro_Message& msg) { msg >> key; }
    virtual char* appName();

#if 0 
  void operator>> (Maestro_Message &msg) 
  { 
    msg.write(key.s, key.size); 
    msg.write(key.size);
  }

  void operator<< (Maestro_Message &msg) 
  { 
    int size;
    msg.read(size);
    void *data;
    msg.look(data, size);
    key = (char*) data;
  }
#endif

  protected:
    static const char* MAGIC;
    Maestro_CORBA_String key;
};

/******************** Maestro_ORB_RequestId ********************/

class Maestro_ORB_RequestId : virtual public Maestro_CORBA_Serializable, virtual public Maestro_Base
{
  public:
    Maestro_ORB_RequestId() {}
    Maestro_ORB_RequestId(Maestro_CORBA_ULong iiopReqId, Maestro_IIOP_ConnId connId);
    Maestro_ORB_RequestId(Maestro_ORB_RequestId& reqId);

    Maestro_ORB_RequestId& operator=(Maestro_ORB_RequestId& reqId);
    int operator==(Maestro_ORB_RequestId& reqId);

    Maestro_CORBA_ULong reqId() { return request_id; }
    Maestro_IIOP_ConnId connId() { return cid; }

    void operator<<(Maestro_CORBA_Message& msg) { msg >> cid >> request_id; }
    void operator>>(Maestro_CORBA_Message& msg) { msg << cid << request_id; }
    void operator<<(Maestro_Message& msg) { msg >> cid >> request_id; }
    void operator>>(Maestro_Message& msg) { msg << request_id << cid; }

  protected:
    Maestro_CORBA_ULong request_id;
    Maestro_IIOP_ConnId cid;
};

/******************** Maestro_ORB_Base ********************/

// Maestro_ORB_Base:  Base class for ORB execution styles.
class Maestro_ORB_Base
{
  public:
    virtual ~Maestro_ORB_Base() {}

    // To be overloaded in subclasses of Maestro_ORB_Base:

    virtual void request_Callback(Maestro_ORB_RequestId&, Maestro_ORB_ObjectKey&, Maestro_CORBA_Boolean, Maestro_CORBA_String&, Maestro_GIOP_Principal&, Maestro_CORBA_Message&) {}

    virtual void locateRequest_Callback(Maestro_ORB_RequestId&, Maestro_ORB_ObjectKey&) {}

    virtual void cancelRequest_Callback(Maestro_ORB_RequestId&) {}
};

/******************** Maestro_ORB_Binding ********************/

// Binding between an object and an ORB.
// Bindings are registed with the ORB dispatcher and are used
// to route incoming requests to appropriate ORB's.
struct Maestro_ORB_ObjectBinding : virtual public Maestro_Base
{
    Maestro_ORB_ObjectBinding() {}
    Maestro_ORB_ObjectBinding(Maestro_ORB_ObjectBinding& bnd);

    Maestro_ORB_ObjectBinding(Maestro_ORB_ObjectKey& obj_, Maestro_ORB_Base* orb_)
    {
        obj = obj_;
        orb = orb_;
    }

    int operator==(Maestro_ORB_ObjectBinding& bnd) { return (obj == bnd.obj); }

    Maestro_ORB_ObjectKey obj;
    Maestro_ORB_Base* orb;
};

typedef Maestro_OrderedSet(Maestro_ORB_ObjectBinding) Maestro_ORB_ObjectBindingList;

/******************** Maestro_ORB_DispatcherBase ********************/

const int MAESTRO_ORB_HASH_SIZE = 256;

// Maestro_ORB_DispatcherBase:  Generic ORB dispatcher functionality.
class Maestro_ORB_DispatcherBase
{
  public:
    Maestro_ORB_DispatcherBase(unsigned hashSize_ = MAESTRO_ORB_HASH_SIZE);
    virtual ~Maestro_ORB_DispatcherBase();

    // Bind an (object key, ORB policy) pair with the ORB base.
    virtual Maestro_Status bind(Maestro_ORB_ObjectKey& objKey, Maestro_ORB_Base* orb);
    virtual Maestro_Status unbind(Maestro_ORB_ObjectKey& objKey);

    virtual Maestro_Status requestReply(Maestro_ORB_RequestId& reqId, Maestro_GIOP_ReplyStatusType reply_status, Maestro_CORBA_Message& reply_body) = 0;

    virtual Maestro_Status locateReply(Maestro_ORB_RequestId& reqId, Maestro_GIOP_LocateStatusType locate_status) = 0;

  protected:
    // Returns the ORB associated with the given object.  NULL if not found.
    Maestro_ORB_Base* lookup(Maestro_ORB_ObjectKey& objKey);
    Maestro_Lock mutex;
    Maestro_ORB_ObjectBindingList* bindings;
    unsigned hashSize;
};

/******************** Maestro_ORB_IIOPDispatcher ********************/

// Maestro_ORB_IIOPDispatcher:  ORB dispatcher over IIOP transport.
class Maestro_ORB_IIOPDispatcher : virtual public Maestro_ORB_DispatcherBase, virtual public Maestro_IIOP_Server
{
  public:
    Maestro_ORB_IIOPDispatcher(unsigned short port = 0, unsigned hashSize = MAESTRO_ORB_HASH_SIZE) : Maestro_ORB_DispatcherBase(hashSize), Maestro_IIOP_Server(port) {}

    virtual Maestro_Status requestReply(Maestro_ORB_RequestId& reqId, Maestro_GIOP_ReplyStatusType reply_status, Maestro_CORBA_Message& reply_body);

    virtual Maestro_Status locateReply(Maestro_ORB_RequestId& reqId, Maestro_GIOP_LocateStatusType locate_status);

  protected:
    void request_Callback(Maestro_CORBA_ULong request_id,
                          Maestro_CORBA_Boolean response_expected,
                          Maestro_CORBA_OctetSequence& object_key,
                          Maestro_CORBA_String& operation,
                          Maestro_GIOP_Principal& requesting_principal,
                          Maestro_CORBA_Message& msg,
                          Maestro_IIOP_ConnId cid);

    void locateRequest_Callback(Maestro_CORBA_ULong request_id, Maestro_CORBA_OctetSequence& object_key, Maestro_IIOP_ConnId cid);
};

//************** Generic configuration parameters. **************

enum Maestro_Conf_Status
{
    MAESTRO_CONF_ARG_ST_OK,
    MAESTRO_CONF_ARG_ST_EOF,
    MAESTRO_CONF_ARG_ST_SYS_ERR,
    MAESTRO_CONF_ARG_ST_SYNTAX_ERR
};

struct Maestro_Conf_Item : Maestro_Base
{
    Maestro_String key;
    Maestro_String val;

    Maestro_Conf_Item() {}
    Maestro_Conf_Item(Maestro_String& _key, Maestro_String& _val);
    int operator==(Maestro_Conf_Item&) { return 0; }
};

std::ostream& operator<<(std::ostream& out, Maestro_Conf_Item& item);

typedef Maestro_OrderedSet(Maestro_Conf_Item) Maestro_Conf_ItemList;

struct Maestro_Conf_Argument : Maestro_Base
{
    Maestro_String type;
    Maestro_String name;
    Maestro_Conf_ItemList items;

    Maestro_Conf_Argument() { valid = 0; }
    void reset(Maestro_String& _type, Maestro_String& _name);
    void add(Maestro_String& key, Maestro_String& val);

    int operator==(Maestro_Conf_Argument&) { return 0; }
    Maestro_Conf_Status operator<<(std::istream& sdl);
    int valid;
};

std::ostream& operator<<(std::ostream& out, Maestro_Conf_Argument& arg);

Maestro_Conf_Status operator>>(std::istream& sdl, Maestro_Conf_Argument& arg);

typedef Maestro_OrderedSet(Maestro_Conf_Argument) Maestro_Conf_ArgumentList;

struct Maestro_Conf
{
    Maestro_String name;
    Maestro_Conf_ArgumentList arguments;

    Maestro_Conf() { valid = 0; }
    Maestro_Conf(Maestro_String& _name)
    {
        valid = 0;
        name = _name;
    }
    void add(Maestro_Conf_Argument& arg) { arguments += arg; }
    void operator<<(std::istream& sdl);
    void generateCode(std::ostream& out, char* prefix, char* confVar);

    int valid;
};

std::ostream& operator<<(std::ostream& out, Maestro_Conf& intf);

void operator>>(std::istream& sdl, Maestro_Conf& intf);

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1681
#pragma reset woff 1682
#pragma reset woff 3624
#endif

#endif // __MAESTRO_ORB_H__
