// $Header$
// 
// Definitions of CORBA 2.0 types + message marshaling.
//
// Author:  Alexey Vaysburd, Sept. 1997.

#ifndef __MAESTRO_CORBA_H__
#define __MAESTRO_CORBA_H__

/* jsd: added cond */
#if defined(__GNUC__)
#pragma interface
#endif

#include <avango/ensemble/maestro/Maestro_Types.h>

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1681
#pragma set woff 1682
#pragma set woff 3624
#endif

class Maestro_CORBA_Message;

/********************** Maestro_CORBA_Base ************************/

// Base class for all CORBA classes that will be read/written from/to
// CORBA messages.
class Maestro_CORBA_Base {
public:
  virtual ~Maestro_CORBA_Base() {}
  
  // Write itself to a CORBA message.  Must be overloaded if used.
  virtual void operator>> (Maestro_CORBA_Message &) { assert(0); }

  // Read itself from a CORBA message.  Must be overloaded if used.
  virtual void operator<< (Maestro_CORBA_Message &) { assert(0); }

  // Write itself to std::ostream.
  virtual void operator>> (std::ostream &) { assert(0); }

  // Return object's size when marshaled into a message.
  MAESTRO_INLINE virtual unsigned marshalSize(unsigned alignment = 0);
};

std::ostream& operator<< (std::ostream &out, Maestro_CORBA_Base &base);


/********************** Maestro_CORBA_Sequence ************************/

// Unbounded CORBA sequences.  The parameter type objects must 
// know how to read/write themselves from/to messages 
// (<< and >> operators).
#define Maestro_CORBA_Sequence(T)                                \
                                                                 \
class Maestro_CORBA_Sequence ## T : virtual public Maestro_CORBA_Base {\
                                                                 \
public:                                                          \
                                                                 \
  Maestro_CORBA_Sequence ## T () {                               \
    list = 0;                                                    \
    listSize = 0;                                                \
  }                                                              \
                                                                 \
  Maestro_CORBA_Sequence ## T (T &t)                             \
  {                                                              \
    list = new T[1];                                             \
    list[0] = t;                                                 \
    listSize = 1;                                                \
  }                                                              \
                                                                 \
  Maestro_CORBA_Sequence ## T (Maestro_CORBA_Sequence ## T &l)   \
  : Maestro_CORBA_Base(l)                                        \
  {                                                              \
    listSize = l.size();                                         \
    if (listSize) {                                              \
      list = new T[listSize];                                    \
      for (unsigned i = 0; i < listSize; i++)                    \
        list[i] = l.list[i];                                     \
    }                                                            \
    else                                                         \
      list = 0;                                                  \
  }                                                              \
                                                                 \
  Maestro_CORBA_Sequence ## T (T *tp, unsigned nelem) {          \
    listSize = nelem;                                            \
    if (listSize) {                                              \
      list = new T[listSize];                                    \
      for (unsigned i = 0; i < listSize; i++)                    \
        list[i] = tp[i];                                         \
    }                                                            \
    else                                                         \
      list = 0;                                                  \
  }                                                              \
                                                                 \
  virtual ~Maestro_CORBA_Sequence ## T () {                      \
     clear();                                                    \
                                                                 \
  }                                                              \
                                                                 \
  virtual Maestro_CORBA_Sequence ## T& operator=                 \
    (Maestro_CORBA_Sequence ## T &l)                             \
  {                                                              \
    if (this != &l) {                                            \
      clear();                                                   \
      listSize = l.size();                                       \
      if (listSize) {                                            \
        list = new T[listSize];                                  \
        for (unsigned i = 0; i < listSize; i++)                  \
	  list[i] = l.list[i];                                   \
      }                                                          \
    }                                                            \
    return *this;                                                \
  }                                                              \
                                                                 \
  virtual void operator <<= (Maestro_CORBA_Sequence ## T &l) {   \
    if (this != &l) {                                            \
      clear();                                                   \
      listSize = l.listSize;                                     \
      l.listSize = 0;                                            \
      list = l.list;                                             \
      l.list = 0;                                                \
    }                                                            \
  }                                                              \
                                                                 \
  virtual T& operator [] (unsigned i) const {                    \
    assert(i < listSize);                                        \
    return list[i];                                              \
  }                                                              \
                                                                 \
  virtual void clear() {                                         \
    delete [] list;                                              \
    list = 0;                                                    \
    listSize = 0;                                                \
  }                                                              \
                                                                 \
  virtual unsigned size() const { return listSize; }             \
                                                                 \
  virtual T* data() { return list; }                             \
                                                                 \
  virtual void size(unsigned size) {                             \
    clear();                                                     \
    assert(listSize == 0 && list == 0);                          \
    listSize = size;						 \
    if (listSize)                                                \
      list = new T[listSize];                                    \
  }                                                              \
                                                                 \
  void operator >> (Maestro_CORBA_Message& msg) {                \
    unsigned int i;                                                       \
    msg << listSize;                                             \
    for (i = 0; i < listSize; i++)                               \
      msg << list[i];                                            \
  }                                                              \
                                                                 \
  void operator << (Maestro_CORBA_Message& msg) {                \
    clear();                                                     \
    assert(listSize == 0 && list == 0);                          \
                                                                 \
    msg >> listSize;                                             \
    if (listSize) {                                              \
      list = new T[listSize];                                    \
      unsigned int i;                                                     \
      for (i = 0; i < listSize; i++) {                           \
	msg >> list[i];                                          \
      }                                                          \
    }                                                            \
  }                                                              \
                                                                 \
  void operator >> (std::ostream& out) const {                        \
    unsigned int i;                                                       \
    for (i = 0; i < listSize; i++)                               \
      out << list[i];                                            \
  }                                                              \
                                                                 \
protected:                                                       \
                                                                 \
  T *list;                                                       \
  Maestro_CORBA_ULong listSize;                                  \
}

typedef hot_int16     Maestro_CORBA_Short;
typedef hot_uint16    Maestro_CORBA_UShort;
typedef hot_int32     Maestro_CORBA_Long;
typedef hot_uint32    Maestro_CORBA_ULong;
typedef unsigned char Maestro_CORBA_Char;
typedef unsigned char Maestro_CORBA_Boolean;
typedef unsigned char Maestro_CORBA_Octet;


/********************** Maestro_CORBA_Message ***********************/

//
// Reads messages from left to right.  
// Byte ordering and alignment are done according to CORBA 2.0.
//
// Scalar types (short, long, ushort, ulong) are automatically
// byte swapped (when read) if the msg's byte order (msgByteOrder()) is
// different from the byte order at the local host (localByteOrder()).
//
class Maestro_CORBA_Message: public Maestro_Base {
public:

  MAESTRO_INLINE Maestro_CORBA_Message(); 

  MAESTRO_INLINE Maestro_CORBA_Message(
          void *data, 
	  unsigned size,
	  void (*onRelease)(void*) = 0, 
	  void *env = 0);

  MAESTRO_INLINE virtual void rewind();
  MAESTRO_INLINE virtual void* body();
  virtual void reset() { msg.reset(); }
  virtual void align(unsigned n) { msg.align(n); }
  virtual unsigned size() { return msg.size(); }
  virtual unsigned pos() { return msg.getPos(); }
  virtual void pos(unsigned n) { msg.setPos(n); }

  //////////////   Byte Ordering and Swapping   /////////////////

  // Byte ordering of the local host: 
  // 0 = BIG endian, 1 = LITTLE endian.
  MAESTRO_INLINE static Maestro_CORBA_Boolean localByteOrder();
  MAESTRO_INLINE virtual void msgByteOrder(Maestro_CORBA_Boolean byteOrder);
  virtual Maestro_CORBA_Boolean msgByteOrder() { return _byteOrder; }

  MAESTRO_INLINE static Maestro_CORBA_UShort swap(Maestro_CORBA_UShort v); 
  MAESTRO_INLINE static Maestro_CORBA_Short swap(Maestro_CORBA_Short v); 
  MAESTRO_INLINE static Maestro_CORBA_ULong swap(Maestro_CORBA_ULong v);
  MAESTRO_INLINE static Maestro_CORBA_Long swap(Maestro_CORBA_Long v); 


  //////////////////////    Message Input    ////////////////////

  MAESTRO_INLINE virtual Maestro_CORBA_Message& 
  operator>> (Maestro_CORBA_Short &v);
  
  MAESTRO_INLINE virtual Maestro_CORBA_Message& 
  operator>> (Maestro_CORBA_UShort &v);

  MAESTRO_INLINE virtual Maestro_CORBA_Message& 
  operator>> (Maestro_CORBA_Long &v);

  MAESTRO_INLINE virtual Maestro_CORBA_Message& 
  operator>> (Maestro_CORBA_ULong &v);

  // This also takes care of Maestro_CORBA_Char and Maestro_CORBA_Boolean
  MAESTRO_INLINE virtual Maestro_CORBA_Message& 
  operator>> (Maestro_CORBA_Octet &v);

  virtual void write(void *buf, unsigned size) { msg.write(buf, size); }


  //////////////////////    Message Output    ////////////////////

  MAESTRO_INLINE virtual Maestro_CORBA_Message& 
  operator<< (Maestro_CORBA_Short v);

  MAESTRO_INLINE virtual Maestro_CORBA_Message& 
  operator<< (Maestro_CORBA_UShort v);

  MAESTRO_INLINE virtual Maestro_CORBA_Message& 
  operator<< (Maestro_CORBA_Long v);

  MAESTRO_INLINE virtual Maestro_CORBA_Message& 
  operator<< (Maestro_CORBA_ULong v);

  MAESTRO_INLINE virtual Maestro_CORBA_Message& 
  operator<< (Maestro_CORBA_Octet v);

  virtual void read(void *buf, unsigned size) { msg.read(buf, size); }

  // Generic message I/O operations.  

  MAESTRO_INLINE virtual Maestro_CORBA_Message& 
  operator>> (Maestro_CORBA_Base &v); 
  
  MAESTRO_INLINE Maestro_CORBA_Message& 
  operator<< (Maestro_CORBA_Base &v); 

  // Pushing CORBA messages to/reading from Maestro messages
  MAESTRO_INLINE void operator>> (Maestro_Message &maeMsg);
  MAESTRO_INLINE void operator<< (Maestro_Message &maeMsg);
  int operator== (Maestro_CORBA_Message& ) { return 0; }

private:
    Maestro_Message msg;
    Maestro_CORBA_Boolean _byteOrder;
};


typedef Maestro_OrderedSet(Maestro_CORBA_Message)
  Maestro_CORBA_MessageList;

typedef Maestro_CORBA_Sequence(Maestro_CORBA_Octet)  
  Maestro_CORBA_OctetSequence_Base;


class Maestro_CORBA_OctetSequence: 
  virtual public Maestro_CORBA_OctetSequence_Base 
{
public:
  Maestro_CORBA_OctetSequence():
    Maestro_CORBA_OctetSequence_Base() {}

  Maestro_CORBA_OctetSequence(Maestro_CORBA_Octet o):
    Maestro_CORBA_OctetSequence_Base(o) {}

  Maestro_CORBA_OctetSequence(Maestro_CORBA_OctetSequence &seq):
    Maestro_CORBA_Base(seq), Maestro_CORBA_OctetSequence_Base(seq) {}

  Maestro_CORBA_OctetSequence(Maestro_CORBA_Octet *data, 
			      unsigned nelem):
    Maestro_CORBA_OctetSequence_Base(data, nelem) {}
  
  void operator>> (Maestro_CORBA_Message& msg);
  void operator<< (Maestro_CORBA_Message& msg);
};


/********************** Maestro_CORBA_Serializable ***********************/

// Base class for CORBA classes that need to support octet-sequence
// encapsulation in CORBA 2.0 format.
class Maestro_CORBA_Serializable: virtual public Maestro_CORBA_Base {
public:
  // Write itself to an encapsulated octet sequence.
  virtual void operator>> (Maestro_CORBA_OctetSequence &seq);
  virtual void writeTo(Maestro_CORBA_OctetSequence &seq);

  // Read itself from an encapsulated octet sequence.
  virtual void operator<< (Maestro_CORBA_OctetSequence &seq); 
  virtual void readFrom(Maestro_CORBA_OctetSequence &seq); 

    // Read/write to/from Maestro Message.
    //virtual void operator>> (Maestro_Message &maeMsg);
    //virtual void operator<< (Maestro_Message &maeMsg);
};

void operator>> (Maestro_CORBA_OctetSequence &seq,
		 Maestro_CORBA_Serializable &ser);

void operator<< (Maestro_CORBA_OctetSequence &seq,
		 Maestro_CORBA_Serializable &ser);

#if 0
Maestro_Message& 
operator>> (Maestro_Message &msg,
	    Maestro_CORBA_Serializable &ser);

Maestro_Message&
operator<< (Maestro_Message &msg,
	    Maestro_CORBA_Serializable &ser);
#endif

/********************** Maestro_CORBA_String *********************/

struct Maestro_CORBA_String: virtual public Maestro_CORBA_Serializable {
  Maestro_CORBA_String(char *str = 0);
  Maestro_CORBA_String(Maestro_CORBA_String& str);
  virtual ~Maestro_CORBA_String();
  virtual Maestro_CORBA_String& operator= (Maestro_CORBA_String&);
  virtual Maestro_CORBA_String& operator= (char*);
  int operator== (Maestro_CORBA_String& str);
  
  MAESTRO_INLINE void operator<< (Maestro_CORBA_Message &msg);
  MAESTRO_INLINE void operator>> (Maestro_CORBA_Message &msg);
  void operator>> (std::ostream &out);

  char *s;
  Maestro_CORBA_ULong size;
  static char NULL_STRING[1];
};


Maestro_Message& 
operator>> (Maestro_Message &msg,
	    Maestro_CORBA_String &str);

Maestro_Message&
operator<< (Maestro_Message &msg,
	    Maestro_CORBA_String &str);


/************************** Maestro_CORBA_Exception *********************/

// System exceptions defined by CORBA.
enum Maestro_CORBA_ExceptionCode {
  MAESTRO_CORBA_EXCEPTION_CODE_UNKNOWN             = 10000,
  MAESTRO_CORBA_EXCEPTION_CODE_BAD_PARAM           = 10020,
  MAESTRO_CORBA_EXCEPTION_CODE_NO_MEMORY           = 10040,
  MAESTRO_CORBA_EXCEPTION_CODE_IMP_LIMIT           = 10060,
  MAESTRO_CORBA_EXCEPTION_CODE_COMM_FAILURE        = 10080,
  MAESTRO_CORBA_EXCEPTION_CODE_INV_OBJREF          = 10100,
  MAESTRO_CORBA_EXCEPTION_CODE_NO_PERMISSION       = 10120,
  MAESTRO_CORBA_EXCEPTION_CODE_INTERNAL            = 10140,
  MAESTRO_CORBA_EXCEPTION_CODE_MARSHAL             = 10160,
  MAESTRO_CORBA_EXCEPTION_CODE_INITIALIZE          = 10180,
  MAESTRO_CORBA_EXCEPTION_CODE_NO_IMPLEMENT        = 10200,
  MAESTRO_CORBA_EXCEPTION_CODE_BAD_TYPECODE        = 10220,
  MAESTRO_CORBA_EXCEPTION_CODE_BAD_OPERATION       = 10240,
  MAESTRO_CORBA_EXCEPTION_CODE_NO_RESOURCES        = 10260,
  MAESTRO_CORBA_EXCEPTION_CODE_NO_RESPONSE         = 10280,
  MAESTRO_CORBA_EXCEPTION_CODE_PERSIST_STORE       = 10300,
  MAESTRO_CORBA_EXCEPTION_CODE_BAD_INV_ORDER       = 10320,
  MAESTRO_CORBA_EXCEPTION_CODE_TRANSIENT           = 10340,
  MAESTRO_CORBA_EXCEPTION_CODE_FREE_MEM            = 10360,
  MAESTRO_CORBA_EXCEPTION_CODE_INV_IDENT           = 10380,
  MAESTRO_CORBA_EXCEPTION_CODE_INV_FLAG            = 10400,
  MAESTRO_CORBA_EXCEPTION_CODE_INTF_REPOS          = 10420,
  MAESTRO_CORBA_EXCEPTION_CODE_BAD_CONTEXT         = 10440,
  MAESTRO_CORBA_EXCEPTION_CODE_OBJ_ADAPTOR         = 10460,
  MAESTRO_CORBA_EXCEPTION_CODE_DATA_CONVERSION     = 10480
};

enum Maestro_CORBA_CompletionStatus { 
  MAESTRO_CORBA_COMPLETION_STATUS_YES, 
  MAESTRO_CORBA_COMPLETION_STATUS_NO, 
  MAESTRO_CORBA_COMPLETION_STATUS_MAYBE
}; 

struct Maestro_CORBA_Exception: virtual public Maestro_CORBA_Serializable {
  Maestro_CORBA_Exception(Maestro_CORBA_String name,
			  Maestro_CORBA_ExceptionCode minor_code,
			  Maestro_CORBA_CompletionStatus status);

  void operator<< (Maestro_CORBA_Message &msg);
  void operator>> (Maestro_CORBA_Message &msg);
  void operator>> (std::ostream &out);

  Maestro_CORBA_String _name;
  Maestro_CORBA_ExceptionCode _minor_code;
  Maestro_CORBA_CompletionStatus _status;
};

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1681
#pragma reset woff 1682
#pragma reset woff 3624
#endif

#endif // __MAESTRO_CORBA_H__
