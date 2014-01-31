// $Header$
// 
// Definitions of CORBA 2.0 types + message marshaling.
//
// Author:  Alexey Vaysburd, Sept. 1997.

#pragma implementation
#include "avango/ensemble/maestro/Maestro_CORBA.h"


/********************** Maestro_CORBA_Base ************************/

unsigned 
Maestro_CORBA_Base::marshalSize(unsigned alignment) {
  Maestro_CORBA_Message msg;
  *this >> msg;
  if (alignment) 
    msg.align(alignment);
  return msg.size();
}

std::ostream& operator<< (std::ostream &out, Maestro_CORBA_Base &base) {
  base >> out;
  return out;
}


/********************** Maestro_CORBA_Message ***********************/

Maestro_CORBA_Message::Maestro_CORBA_Message() { 
  msg.readDir(MAESTRO_MSG_READ_RIGHT);  // Read from left to right.
  msgByteOrder(localByteOrder());	
}

Maestro_CORBA_Message::Maestro_CORBA_Message(
    void *data, 
    unsigned size,
    void (*onRelease)(void*), 
    void *env): msg(data, size, onRelease, env) 
{
  rewind();
  msgByteOrder(localByteOrder());	
}

void 
Maestro_CORBA_Message::rewind() {
  msg.readDir(MAESTRO_MSG_READ_RIGHT); // Read from left to right.
  msg.setPos(0);		         // Rewind to the left.
}

void* 
Maestro_CORBA_Message::body() {
  rewind();
  void *data;
  msg.look(data, msg.size());
  return data;
}

  //////////////   Byte Ordering and Swapping   /////////////////


  // Byte ordering of the local host: 
  // 0 = BIG endian, 1 = LITTLE endian.
Maestro_CORBA_Boolean 
Maestro_CORBA_Message::localByteOrder() {
  union {
    hot_uint32 u;
    char b[4];
  } d;
  d.u = 1;
  return d.b[0];
  }

void 
Maestro_CORBA_Message::msgByteOrder(Maestro_CORBA_Boolean byteOrder) { 
  _byteOrder = byteOrder; 
}


Maestro_CORBA_UShort 
Maestro_CORBA_Message::swap(Maestro_CORBA_UShort v) {
  union {
    hot_uint16 u;
    char b[2];
  } d1, d2;
  d1.u = v;
  d2.b[0] = d1.b[1];
  d2.b[1] = d1.b[0];
  return d2.u;
}

Maestro_CORBA_Short 
Maestro_CORBA_Message::swap(Maestro_CORBA_Short v) {
  return (Maestro_CORBA_Short) swap((Maestro_CORBA_UShort) v);
}

Maestro_CORBA_ULong 
Maestro_CORBA_Message::swap(Maestro_CORBA_ULong v) {
  union {
    hot_uint32 u;
    char b[4];
  } d1, d2;
  d1.u = v;
  d2.b[0] = d1.b[3];
  d2.b[1] = d1.b[2];
  d2.b[2] = d1.b[1];
  d2.b[3] = d1.b[0];
  return d2.u;
}

Maestro_CORBA_Long 
Maestro_CORBA_Message::swap(Maestro_CORBA_Long v) {
  return (Maestro_CORBA_Long) swap((Maestro_CORBA_ULong) v);
}

  //////////////////////    Message Input    ////////////////////

Maestro_CORBA_Message& 
Maestro_CORBA_Message::operator>> (Maestro_CORBA_Short &v) { 
  msg.align(sizeof(v));
  msg.read(&v, sizeof(v)); 
  if (msgByteOrder() != localByteOrder())
    v = swap(v);
}

Maestro_CORBA_Message& 
Maestro_CORBA_Message::operator>> (Maestro_CORBA_UShort &v) {
  msg.align(sizeof(v));
  msg.read(&v, sizeof(v));
  if (msgByteOrder() != localByteOrder())
    v = swap(v);
}

Maestro_CORBA_Message& 
Maestro_CORBA_Message::operator>> (Maestro_CORBA_Long &v) {
  msg.align(sizeof(v));
  msg.read(&v, sizeof(v));
  if (msgByteOrder() != localByteOrder())
    v = swap(v);
}

Maestro_CORBA_Message& 
Maestro_CORBA_Message::operator>> (Maestro_CORBA_ULong &v) {
  msg.align(sizeof(v));
  msg.read(&v, sizeof(v));
  if (msgByteOrder() != localByteOrder())
    v = swap(v);
}

// This also takes care of Maestro_CORBA_Char and Maestro_CORBA_Boolean
Maestro_CORBA_Message& 
Maestro_CORBA_Message::operator>> (Maestro_CORBA_Octet &v) {
  msg.read(&v, sizeof(v));
}


//////////////////////    Message Output    ////////////////////

Maestro_CORBA_Message& 
Maestro_CORBA_Message::operator<< (Maestro_CORBA_Short v) {
  Maestro_CORBA_Short _v = v;
  msg.align(sizeof(_v));
  msg.write(&_v, sizeof(_v));
}

Maestro_CORBA_Message& 
Maestro_CORBA_Message::operator<< (Maestro_CORBA_UShort v) {
  Maestro_CORBA_UShort _v = v;
  msg.align(sizeof(_v));
  msg.write(&_v, sizeof(_v));
}

Maestro_CORBA_Message& 
Maestro_CORBA_Message::operator<< (Maestro_CORBA_Long v) {
  Maestro_CORBA_Long _v = v;
  msg.align(sizeof(_v));
  msg.write(&_v, sizeof(_v));
  }

Maestro_CORBA_Message& 
Maestro_CORBA_Message::operator<< (Maestro_CORBA_ULong v) {
  Maestro_CORBA_ULong _v = v;
  msg.align(sizeof(_v));
  msg.write(&_v, sizeof(_v));
}

Maestro_CORBA_Message& 
Maestro_CORBA_Message::operator<< (Maestro_CORBA_Octet v) {
  Maestro_CORBA_Octet _v = v;
  msg.write(&_v, sizeof(_v));
}

// Generic message I/O operations.  

Maestro_CORBA_Message& 
Maestro_CORBA_Message::operator>> (Maestro_CORBA_Base &v) { 
  v << *this; 
  return *this;
}
  
Maestro_CORBA_Message& 
Maestro_CORBA_Message::operator<< (Maestro_CORBA_Base &v) { 
  v >> *this; 
  return *this;
}

// Pushing a CORBA message into a Maestro message. 
void
Maestro_CORBA_Message::operator>> (Maestro_Message &maeMsg) {
  if (!msg.isWritable()) {
    error->panic("Maestro_CORBA_Message::operator>>: message not writable");
  }
  Maestro_Message tmp = msg;
  assert(tmp.size() == tmp.getPos());
  tmp.readDir(MAESTRO_MSG_READ_LEFT);
  void *data;
  tmp.look(data, tmp.size());

  // Pad to 4-byte boundary.
  char pad[4];
  char npad = 4 - (maeMsg.getPos() % 4);
  pad[npad - 1] = npad;
  maeMsg.write(pad, npad);

  maeMsg.write(data, tmp.size());
  maeMsg.write(tmp.size());
}

// Reading a CORBA message from a Maestro message.
void
Maestro_CORBA_Message::operator<< (Maestro_Message &maeMsg) {
  int size;
  maeMsg.read(size);
  
  void *data;
  maeMsg.look(data, size);
  Maestro_Message tmp(data, size, NULL, NULL);
  msg <<= tmp;
  rewind();
  msgByteOrder(localByteOrder());	

  // Read the padding.
  char pad[4];
  char npad;
  maeMsg.read(&npad, 1);
  if (npad > 1) {
    maeMsg.read(pad, npad - 1);
  }
}


/********************** Maestro_CORBA_OctetSequence ***********************/

void
Maestro_CORBA_OctetSequence::operator>> (Maestro_CORBA_Message& msg) {
  int i;           
  msg << listSize; 
  msg.write(list, listSize);
}

void 
Maestro_CORBA_OctetSequence::operator<< (Maestro_CORBA_Message& msg) {
  clear();
  assert(listSize == 0 && list == 0);  
  msg >> listSize;                                             
  if (listSize) {                                              
    list = new Maestro_CORBA_Octet[listSize];       
    msg.read(list, listSize);
  } 
}

/********************** Maestro_CORBA_Serializable ***********************/

// Write itself to an encapsulated octet sequence.
void 
Maestro_CORBA_Serializable::operator>> (Maestro_CORBA_OctetSequence &seq) {
  writeTo(seq);
}

void 
Maestro_CORBA_Serializable::writeTo(Maestro_CORBA_OctetSequence &seq) {
  Maestro_CORBA_Message msg;
  msg << Maestro_CORBA_Message::localByteOrder();
  msg << *this;
  msg.rewind();
  seq.size(msg.size());
  msg.read(seq.data(), msg.size());
}

// Read itself from an encapsulated octet sequence.
void 
Maestro_CORBA_Serializable::operator<< (Maestro_CORBA_OctetSequence &seq) {
  readFrom(seq);
}

void 
Maestro_CORBA_Serializable::readFrom(Maestro_CORBA_OctetSequence &seq) {
  Maestro_CORBA_Message msg;
  msg.write(seq.data(), seq.size());
  msg.rewind();
  Maestro_CORBA_Octet byte_ordering;
  msg >> byte_ordering;
  msg.msgByteOrder(byte_ordering);
  msg >> *this;
}


void operator>> (Maestro_CORBA_OctetSequence &seq,
		 Maestro_CORBA_Serializable &ser
){
  ser.readFrom(seq);
}

void operator<< (Maestro_CORBA_OctetSequence &seq,
		 Maestro_CORBA_Serializable &ser
){
  ser.writeTo(seq);
}

#if 0
void 
Maestro_CORBA_Serializable::operator>> (Maestro_Message &maeMsg) {
  Maestro_CORBA_OctetSequence seq;
  writeTo(seq);
  maeMsg.write(seq.data(), seq.size());
  maeMsg.write(seq.size());
}

void 
Maestro_CORBA_Serializable::operator<< (Maestro_Message &maeMsg) {
  Maestro_CORBA_OctetSequence seq;
  int size;
  maeMsg.read(size);
  seq.size(size);
  maeMsg.read(seq.data(), size);
  readFrom(seq);
}

Maestro_Message& 
operator>> (Maestro_Message &msg,
	    Maestro_CORBA_Serializable &ser) { ser << msg; return msg; }

Maestro_Message&
operator<< (Maestro_Message &msg,
	    Maestro_CORBA_Serializable &ser) { ser >> msg; return msg; }
#endif

/********************** Maestro_CORBA_String *********************/

Maestro_CORBA_String::Maestro_CORBA_String(char *str) {
  assert(NULL_STRING[0] == 0);  

  if (str != NULL) { 
    size = strlen(str) + 1;
    s = new char[size];
    strcpy(s, str);
  }
  else {
    s = NULL_STRING;
    size = 1;
  }

  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);    
  assert(size == strlen(s) + 1);
}


Maestro_CORBA_String::~Maestro_CORBA_String() { 
  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);  
  assert(size == strlen(s) + 1);

  if (s != NULL_STRING) {
    delete [] s;
  }
}


Maestro_CORBA_String::Maestro_CORBA_String(Maestro_CORBA_String& str) {
  assert(str.s != NULL && str.size);
  assert(str.size == strlen(str.s) + 1);
  assert(NULL_STRING[0] == 0);  

  if (str.s == NULL_STRING) {
    assert(str.size == 1);
    s = NULL_STRING;
    size = 1;
  }
  else {
    size = str.size;
    s = new char[size];
    strcpy(s, str.s);
  }

  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);  
  assert(size == strlen(s) + 1);
}


Maestro_CORBA_String& 
Maestro_CORBA_String::operator= (Maestro_CORBA_String &str) {
  assert(str.size && str.s != NULL);  
  assert(str.size == strlen(str.s) + 1);
  assert(size == strlen(s) + 1);
  assert(size && s != NULL);  
  assert(NULL_STRING[0] == 0);  

  if (this == &str) {
    return *this;
  }
  
  if (s != NULL_STRING) {
    delete [] s;
  }
  
  size = str.size;

  if (str.s != NULL_STRING) {
    s = new char[size];
    strcpy(s, str.s);
  }
  else {
    assert(str.size == 1);
    s = NULL_STRING;
  }

  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);  
  assert(size == strlen(s) + 1);

  return *this;
}


Maestro_CORBA_String& 
Maestro_CORBA_String::operator= (char *p) {
  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);  
  assert(size == strlen(s) + 1);

  if (s != NULL_STRING) {
    delete [] s;
  }

  if (p != NULL) {
    size = strlen(p) + 1;
    s = new char[size];
    strcpy(s, p);
  }
  else {
    s = NULL_STRING;
    size = 1;
  }

  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);  
  assert(size == strlen(s) + 1);

  return *this;
}


int 
Maestro_CORBA_String::operator== (Maestro_CORBA_String& str) {
  assert(size && s != NULL);
  assert(str.size == strlen(str.s) + 1);
  assert(size == strlen(s) + 1);
  assert(str.size && str.s != NULL);
  assert(NULL_STRING[0] == 0);  

  return (strcmp(s, str.s) == 0);
}

void
Maestro_CORBA_String::operator>> (std::ostream &out) {
  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);  
  assert(size == strlen(s) + 1);

  out << s;
}

void
Maestro_CORBA_String::operator>> (Maestro_CORBA_Message& msg) {
  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);  
  assert(size == strlen(s) + 1);

  msg << size;
  msg.write(s, size);
}

void
Maestro_CORBA_String::operator<< (Maestro_CORBA_Message& msg) {
  assert(size && s != NULL);
  assert(size == strlen(s) + 1);
  assert(NULL_STRING[0] == 0);  

  if (s != NULL_STRING) {
    delete [] s;
  }

  msg >> size;		       
  assert(size > 0);
  s = new char[size];
  msg.read(s, size);

  assert(size && s != NULL);
  assert(size == strlen(s) + 1);
  assert(NULL_STRING[0] == 0);  
}
 
char Maestro_CORBA_String::NULL_STRING[1] = "";

Maestro_Message& 
operator>> (Maestro_Message &msg,
	    Maestro_CORBA_String &str) {
    if (str.s != Maestro_CORBA_String::NULL_STRING) {
	delete [] str.s;
    }

    int size;
    msg.read(size);
    str.size = size;
    assert(str.size > 0);
    str.s = new char[str.size];
    msg.read(str.s, str.size);
}

Maestro_Message&
operator<< (Maestro_Message &msg,
	    Maestro_CORBA_String &str) {
    msg.write(str.s, str.size);
    msg.write(str.size);
}

/************************** Maestro_CORBA_Exception *********************/

Maestro_CORBA_Exception::Maestro_CORBA_Exception(
    Maestro_CORBA_String name,
    Maestro_CORBA_ExceptionCode minor_code,
    Maestro_CORBA_CompletionStatus status
){
  _name = name;
  _minor_code = minor_code;
  _status = status;
}

void 
Maestro_CORBA_Exception::operator<< (Maestro_CORBA_Message &msg) {
  msg >> _name;
  Maestro_CORBA_ULong tmp;
  msg >> tmp;
  _minor_code = (Maestro_CORBA_ExceptionCode) tmp;
  msg >> tmp;
  _status = (Maestro_CORBA_CompletionStatus) tmp;
}

void 
Maestro_CORBA_Exception::operator>> (Maestro_CORBA_Message &msg) {
  msg << _name << (Maestro_CORBA_ULong) _minor_code << 
    (Maestro_CORBA_ULong) _status;
}

void 
Maestro_CORBA_Exception::operator>> (std::ostream &out) {
  out << "Exception " << _name.s << " (minor code: " << _minor_code << 
    ", status: ";
  switch (_status) {
  case MAESTRO_CORBA_COMPLETION_STATUS_YES:
    out << "COMPLETED_YES";
    break;
  case MAESTRO_CORBA_COMPLETION_STATUS_NO:
    out << "COMPLETED_NO";
    break;
  case MAESTRO_CORBA_COMPLETION_STATUS_MAYBE:
    out << "COMPLETED_MAYBE";
    break;
  default:
    assert(0);
    break;
  }
  out << ")" << std::endl;
}
