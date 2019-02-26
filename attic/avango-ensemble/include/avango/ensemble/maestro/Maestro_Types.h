// $Header$

#ifndef __MAESTRO_TYPES_H__
#define __MAESTRO_TYPES_H__

/********************************************************************
 *                                                                  *
 * Author:  Alexey Vaysburd  December 96                            *
 *                                                                  *
 * Contents:  Definition of Maestro Wrapper Classes                 *
 *                                                                  *
 ********************************************************************/

/* jsd: added cond */
#if defined(__GNUC__)
#pragma interface
#endif

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ostream>
#include <malloc.h>
#include <sys/time.h>

// keep order
#include <avango/ensemble/hot/hot_sys.h>
#include <avango/ensemble/hot/hot_error.h>
#include <avango/ensemble/hot/hot_thread.h>
#include <avango/ensemble/hot/hot_msg.h>
#include <avango/ensemble/hot/hot_ens.h>

#include <avango/ensemble/maestro/Maestro_Config.h>

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1681
#pragma set woff 1682
#pragma set woff 3624
#endif

extern "C"
{
    void srandom(unsigned);
#if defined(LINUX) || defined(__linux)
    long int random(void);
    int gethostname(char*, size_t);
    int gettimeofday(struct timeval* tv, struct timezone* tz);
#else
    long random();
    /* jsd: added conditional for __sgi */
#ifndef __sgi
#ifndef HPUX
    int gethostname(char*, int);
#endif
    int gettimeofday(struct timeval* tp, void*);
#endif
#endif
}

class Maestro_ErrorHandler;
class Maestro_Base;

class Maestro_Thread;
class Maestro_Semaphore;
class Maestro_Lock;

class Maestro_Message;
class Maestro_EndpID;
class Maestro_EndpList;
class Maestro_String;

class Maestro_GroupMember;

#include <avango/ensemble/maestro/Maestro_OrderedSet.h>

typedef unsigned long Maestro_ULong;

#define check_err(s)                                                                                                                                                                                   \
    if(err != HOT_OK)                                                                                                                                                                                  \
    error->panic(err, s)

// printing debugging messages
#ifdef MAESTRO_DEBUG
#define putd(s) (cout << (s) << endl)
#else
#define putd(s)
#endif

// magic numbers for message typechecking for various datatypes
const int MAESTRO_MESSAGE_MAGIC_BASE = 498752;
const int MAESTRO_MESSAGE_MAGIC_BUFFER = 269385;
const int MAESTRO_MESSAGE_MAGIC_INT = 153274;
const int MAESTRO_MESSAGE_MAGIC_ENDP_ID = 807649;
const int MAESTRO_MESSAGE_MAGIC_ENDP_LIST = 340127;
const int MAESTRO_MESSAGE_MAGIC_INT_LIST = 524938;
const int MAESTRO_MESSAGE_MAGIC_STRING = 935810;
const int MAESTRO_MESSAGE_MAGIC_MESSAGE = 693847;

/***************************** Maestro_Error ***************************/

typedef hot_err_t Maestro_Status;

// Return status of method invocations.
class Maestro_Error
{
  public:
    static Maestro_Status create(char* errStr) { return hot_err_Create(0, errStr); }

    static void release(Maestro_Status st) { hot_err_Release(st); }

    static char* errString(Maestro_Status st) { return hot_err_ErrString(st); }
};

const Maestro_Status MAESTRO_OK = HOT_OK;

/***************************** Maestro_ErrorHandler *******************/

// This class defines panic() methods that are invoked when an error occurs.
class Maestro_ErrorHandler
{
  public:
    Maestro_ErrorHandler() {}
    Maestro_ErrorHandler(Maestro_ErrorHandler& heh);
    virtual ~Maestro_ErrorHandler() {}
    virtual Maestro_ErrorHandler& operator=(Maestro_ErrorHandler& heh);
    virtual void panic(char* context = "unknown problem", ...);
    virtual void warning(char* context = "unknown problem", ...);
    virtual void panic(hot_err_t err, char* context = "unknown problem", ...);
};

extern Maestro_ErrorHandler Maestro_DefaultErrorHandler;

/******************************* Maestro_Base **************************/

// this is the base class for all following Maestro classes.
class Maestro_Base
{
  public:
    MAESTRO_INLINE Maestro_Base(Maestro_ErrorHandler* error_handler = 0);
    Maestro_Base(Maestro_Base&) {}
    virtual ~Maestro_Base() {}
    virtual Maestro_Base& operator=(Maestro_Base&) { return *this; }
    virtual void operator<<(Maestro_Message& msg);
    virtual void operator>>(Maestro_Message& msg);
    virtual void operator>>(std::ostream& out) const;

    // magic number for message I/O (to be overloaded for finer typechecking)
    virtual int messageMagic() const { return MAESTRO_MESSAGE_MAGIC_BASE; }

    // Marshal/unmarshal the object into/from a string.
    virtual void operator<<(Maestro_String& msg);
    virtual void operator>>(Maestro_String& msg);

  protected:
    Maestro_ErrorHandler* error;
    hot_err_t err;
};

Maestro_Message& operator<<(Maestro_Message& msg, Maestro_Base& hb);
Maestro_Message& operator>>(Maestro_Message& msg, Maestro_Base& hb);
std::ostream& operator<<(std::ostream& out, Maestro_Base& hb);

// all options classes inherit from Maestro_Options
struct Maestro_Options : private Maestro_Base
{
    virtual void reset() {}
};

/**************************** Maestro_Thread *****************************/

struct Maestro_ThreadOps
{
    int stackSize;
};

class Maestro_Thread : virtual public Maestro_Base
{
  public:
    static void create(void (*proc)(void*), void* arg, Maestro_ThreadOps* ops = 0, Maestro_ErrorHandler* error_handler = 0);
    static void usleep(int usecs);
};

/***************************** Maestro_Semaphore **************************/

class Maestro_Semaphore : virtual public Maestro_Base
{
  public:
    Maestro_Semaphore(int initial_value = 0, Maestro_ErrorHandler* error_handler = 0);
    Maestro_Semaphore(Maestro_Semaphore& hs);
    virtual ~Maestro_Semaphore();
    virtual Maestro_Semaphore& operator=(Maestro_Semaphore& hs);
    virtual void inc();
    virtual void dec();

  protected:
    hot_sema_t sema;
};

/********************************* Maestro_Lock ****************************/

class Maestro_Lock : virtual public Maestro_Base
{
  public:
    Maestro_Lock(Maestro_ErrorHandler* error_handler = 0);
    Maestro_Lock(Maestro_Lock& hl);
    virtual ~Maestro_Lock();
    virtual Maestro_Lock& operator=(Maestro_Lock& hl);
    virtual void lock();
    virtual void unlock();

  protected:
    hot_lock_t lck;
};

/********************************* Maestro_Barrier ***********************/

typedef enum
{
    MAESTRO_BARRIER_OPEN,
    MAESTRO_BARRIER_CLOSED
} Maestro_BarrierState;

// if Barrier's state is CLOSED, pass() will block until open() is called
class Maestro_Barrier : virtual public Maestro_Base
{
  public:
    Maestro_Barrier(Maestro_BarrierState theState = MAESTRO_BARRIER_OPEN, Maestro_ErrorHandler* error_handler = 0);
    Maestro_Barrier(Maestro_Barrier& hc);
    ~Maestro_Barrier();
    Maestro_Barrier& operator=(Maestro_Barrier& hc);
    void close();
    void open();
    void pass();
    int isOpen() { return (state == MAESTRO_BARRIER_OPEN); }
    int isClosed() { return (state == MAESTRO_BARRIER_CLOSED); }

  protected:
    Maestro_Semaphore* sema;
    Maestro_Lock* mutex;
    int nwaiting;
    Maestro_BarrierState state;
};

/**************************** Maestro_String ********************************/

struct Maestro_String : virtual public Maestro_Base
{
    Maestro_String(const char* str = 0);
    Maestro_String(Maestro_String& str);
    virtual ~Maestro_String();
    virtual Maestro_String& operator=(Maestro_String&);
    virtual Maestro_String& operator=(const char*);
    int operator==(Maestro_String& str);

    MAESTRO_INLINE void operator<<(Maestro_Message& msg);
    MAESTRO_INLINE void operator>>(Maestro_Message& msg);
    int messageMagic() const { return MAESTRO_MESSAGE_MAGIC_STRING; }
    void operator>>(std::ostream& out) const;

    virtual void operator+=(char*);
    virtual void operator+=(Maestro_String&);

    char* s;
    int size;

  private:
    static char NULL_STRING[1];
};

/**************************** Maestro_Message ******************************/

// This mirrors the corresp. definition in hot_msg.h
typedef enum
{
    MAESTRO_MSG_SEEK_FROM_HEAD,
    MAESTRO_MSG_SEEK_FROM_TAIL,
    MAESTRO_MSG_SEEK_FROM_CURRENT
} Maestro_MsgSeek;

typedef enum
{
    MAESTRO_MSG_READ_LEFT,
    MAESTRO_MSG_READ_RIGHT
} Maestro_MsgReadDir;

class Maestro_Message : virtual public Maestro_Base
{
    friend class Maestro_GroupMember;

  public:
    MAESTRO_INLINE Maestro_Message(Maestro_ErrorHandler* error_handler = 0);

    MAESTRO_INLINE Maestro_Message(Maestro_Message& msg, Maestro_ErrorHandler* error_handler = 0);

    MAESTRO_INLINE Maestro_Message(hot_msg_t hmsg, Maestro_ErrorHandler* error_handler = 0);

    MAESTRO_INLINE Maestro_Message(void* data, unsigned nbytes, void (*on_release)(void*) = 0, void* env = 0, Maestro_ErrorHandler* error_handler = 0);

    MAESTRO_INLINE virtual ~Maestro_Message();
    MAESTRO_INLINE virtual Maestro_Message& operator=(Maestro_Message& msg);
    MAESTRO_INLINE virtual Maestro_Message& operator<<=(Maestro_Message& msg);

    MAESTRO_INLINE virtual void read(int& i);

    virtual Maestro_Message& operator>>(int& i)
    {
        read(i);
        return *this;
    }

    virtual Maestro_Message& operator>>(Maestro_ULong& u)
    {
        int i = u;
        *this >> i;
        u = i;
        return *this;
    }

    MAESTRO_INLINE virtual void read(void* buf, unsigned size);
    MAESTRO_INLINE virtual void look(void*& data, unsigned size);

    MAESTRO_INLINE virtual void write(int i);

    virtual Maestro_Message& operator<<(int i)
    {
        write(i);
        return *this;
    }

    MAESTRO_INLINE virtual void write(void* buf, int size);

    MAESTRO_INLINE virtual unsigned size();
    MAESTRO_INLINE virtual unsigned getPos();
    MAESTRO_INLINE virtual void setPos(unsigned offset);
    MAESTRO_INLINE virtual void seek(int offset, Maestro_MsgSeek whence);
    MAESTRO_INLINE virtual void readDir(Maestro_MsgReadDir dir);
    MAESTRO_INLINE virtual void align(unsigned nbytes);

    MAESTRO_INLINE virtual void pack(Maestro_Message* msgs, unsigned nmsgs);
    MAESTRO_INLINE virtual void unpack(Maestro_Message*& msgs, unsigned& nmsgs);

    MAESTRO_INLINE virtual void reset();
    virtual int operator==(Maestro_Message&) { return 0; }
    MAESTRO_INLINE virtual int isWritable();

    MAESTRO_INLINE void operator<<(Maestro_Message& msg);
    MAESTRO_INLINE void operator>>(Maestro_Message& msg);
    int messageMagic() const { return MAESTRO_MESSAGE_MAGIC_MESSAGE; }

    void setName(char* s) { name = s; }

    hot_msg_t& getHotMsg() { return msg; }

  protected:
    hot_msg_t msg;
    Maestro_String name;
};

/**************************** Maestro_Buffer *******************************/

struct Maestro_Buffer : virtual public Maestro_Base
{
    Maestro_Buffer(void* theBuf, int theSize)
    {
        buf = theBuf;
        size = theSize;
    }

    void operator<<(Maestro_Message& msg) { msg.read(buf, size); }
    void operator>>(Maestro_Message& msg) { msg.write(buf, size); }

    void* buf;
    int size;
};

/**************************** Maestro_EndpID *************************/

class Maestro_EndpID : virtual public Maestro_Base
{
  public:
    Maestro_EndpID(Maestro_ErrorHandler* error_handler = 0);
    Maestro_EndpID(hot_endpt_t ep, Maestro_ErrorHandler* error_handler = 0);

    virtual int operator==(Maestro_EndpID& eid);
    virtual int operator!=(Maestro_EndpID& eid);
    virtual Maestro_EndpID& operator=(Maestro_EndpID& eid);
    virtual Maestro_EndpID& operator=(hot_endpt_t& ep);

    // Message I/O methods.
    MAESTRO_INLINE void operator<<(Maestro_Message& msg);
    MAESTRO_INLINE void operator>>(Maestro_Message& msg);
    int messageMagic() const { return MAESTRO_MESSAGE_MAGIC_ENDP_ID; }

    void operator>>(std::ostream& out) const { out << endpID.name << std::endl; }

    // jsd: added constness (on both fun-signature and return value)
    const hot_endpt_t& getHotEndpt() const { return endpID; }

  protected:
    hot_endpt_t endpID; // defined as struct {char name[HOT_ENDP_MAX_NAME_SIZE];}
};

/*************************** Maestro_EndpList *****************************/

typedef Maestro_OrderedSet(Maestro_EndpID) Maestro_EndpList_Base;

class Maestro_EndpList : virtual public Maestro_EndpList_Base
{
  public:
    Maestro_EndpList(Maestro_ErrorHandler* error_handler = 0);

    Maestro_EndpList(Maestro_EndpID& eid, Maestro_ErrorHandler* error_handler = 0);

    Maestro_EndpList(Maestro_EndpList& ep_list, Maestro_ErrorHandler* error_handler = 0);

    Maestro_EndpList(hot_endpt_t endps[], unsigned nendp, Maestro_ErrorHandler* error_handler = 0);

    int messageMagic() const { return MAESTRO_MESSAGE_MAGIC_ENDP_LIST; }
};

/*************************** Maestro_IntList ***************************/

typedef Maestro_OrderedSet(int) Maestro_IntList_Base;
typedef Maestro_OrderedSet(Maestro_ULong) Maestro_ULongList_Base;

class Maestro_IntList : virtual public Maestro_IntList_Base
{
  public:
    Maestro_IntList(Maestro_ErrorHandler* error_handler = 0);

    Maestro_IntList(int& i, Maestro_ErrorHandler* error_handler = 0);

    Maestro_IntList(Maestro_IntList& ilist, Maestro_ErrorHandler* error_handler = 0);

    Maestro_IntList(int* elem, unsigned nelem, Maestro_ErrorHandler* error_handler = 0);

    int messageMagic() const { return MAESTRO_MESSAGE_MAGIC_INT_LIST; }
    int& operator[](int i) { return Maestro_IntList_Base::operator[]((int)i); }
};

class Maestro_ULongList : virtual public Maestro_ULongList_Base
{
  public:
    Maestro_ULongList(Maestro_ErrorHandler* error_handler = 0);

    Maestro_ULongList(Maestro_ULong& i, Maestro_ErrorHandler* error_handler = 0);

    Maestro_ULongList(Maestro_ULongList& ilist, Maestro_ErrorHandler* error_handler = 0);

    Maestro_ULongList(Maestro_ULong* elem, unsigned nelem, Maestro_ErrorHandler* error_handler = 0);

    int messageMagic() const { return MAESTRO_MESSAGE_MAGIC_INT_LIST; }
    Maestro_ULong& operator[](Maestro_ULong i) { return Maestro_ULongList_Base::operator[]((int)i); }
};

typedef Maestro_OrderedSet(Maestro_Message) Maestro_MessageList;

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1681
#pragma reset woff 1682
#pragma reset woff 3624
#endif

#endif // __MAESTRO_TYPES_H__
