// $Header$

/********************************************************************
 *                                                                  *
 * Author:  Alexey Vaysburd  December 96                            *
 *                                                                  *
 * Contents:  Definition of Maestro Wrapper classes' methods        *
 *                                                                  *
 ********************************************************************/

#pragma implementation

#include "avango/ensemble/maestro/Maestro_Types.h"

/************************* Maestro_ErrorHandler ******************************/

Maestro_ErrorHandler::Maestro_ErrorHandler(Maestro_ErrorHandler& heh) {
  panic("Maestro_ErrorHandler: copy constructor is not supported");
  assert(0);
}

Maestro_ErrorHandler&
Maestro_ErrorHandler::operator= (Maestro_ErrorHandler& heh) {
  panic("Maestro_ErrorHandler: operator= is not supported");
  assert(0);
  return *this;
}

void
Maestro_ErrorHandler::panic(char *context, ...) {
  va_list args;
  va_start(args, context);
  hot_sys_vPanic(context, args);
  va_end(args);
}

void
Maestro_ErrorHandler::warning(char *context, ...) {
  va_list args;
  va_start(args, context);
  hot_sys_vWarning(context, args);
  va_end(args);
}

void
Maestro_ErrorHandler::panic(hot_err_t err, char *context, ...) {
  char buf[128];
  va_list args;
  va_start(args, context);

  if (strlen(hot_err_ErrString(err)) + strlen(context) < 128) {
    sprintf(buf, "%s:%s", context, hot_err_ErrString(err));
    hot_sys_vPanic(buf, args);
  }
  else {
    hot_sys_vPanic(context, args);
  }

  va_end(args);
}

Maestro_ErrorHandler Maestro_DefaultErrorHandler;

/***************************** Maestro_Base **********************************/

// all subclusses of Maestro_Base support message I/O.
// in order to be readable/writeable from/to a message, a subclass of
// Maestro_Base has to overload the following two operators:
// virtual void operator<< (Maestro_Message& msg); (reading itself from msg)
// virtual void operator>> (Maestro_Message& msg); (pushing itself onto msg)

Maestro_Base::Maestro_Base(Maestro_ErrorHandler *error_handler) {
  error = (error_handler) ? error_handler : &Maestro_DefaultErrorHandler;
}

void
Maestro_Base::operator<< (Maestro_Message& msg) {
  error->panic("Maestro_Base::operator<< (Maestro_Message&) not overloaded");
}

void
Maestro_Base::operator>> (Maestro_Message& msg) {
  error->panic("Maestro_Base::operator>> (Maestro_Message&) not overloaded");
}

void
Maestro_Base::operator>> (std::ostream& out) const {
  error->panic("Maestro_Base::operator>> (ostream&) not overloaded");
}

void
Maestro_Base::operator<< (Maestro_String& msg) {
  error->panic("Maestro_Base::operator<< (Maestro_String&) not overloaded");
}

void
Maestro_Base::operator>> (Maestro_String& msg) {
  error->panic("Maestro_Base::operator>> (Maestro_String&) not overloaded");
}

Maestro_Message&
operator<< (Maestro_Message& msg, Maestro_Base& hb) {
  hb >> msg;

#ifdef MAESTRO_MESSAGE_TYPECHECKING
  msg << hb.messageMagic();
#endif

  return msg;
}

Maestro_Message&
operator>> (Maestro_Message& msg, Maestro_Base& hb) {

#ifdef MAESTRO_MESSAGE_TYPECHECKING
  int magic;
  msg >> magic;
  Maestro_ErrorHandler eh;
  if (magic != hb.messageMagic())
    eh.panic("Maestro_Message >> :  WRONG MAGIC NUMBER");
#endif

  hb << msg;
  return msg;
}

std::ostream&
operator << (std::ostream& out, Maestro_Base& hb) {
  hb >> out;
  return out;
}

/**************************** Maestro_Thread *********************************/

void
Maestro_Thread::create(void (*proc)(void*), void *arg,
                       Maestro_ThreadOps *ops,
                       Maestro_ErrorHandler *error_handler) {
  hot_err_t err;
  hot_thread_attr_t attr;

  if (ops)
    attr.stacksize = ops->stackSize;

  Maestro_ErrorHandler *error = (error_handler) ?
    error_handler : &Maestro_DefaultErrorHandler;

  err = hot_thread_Create(proc, arg, ops ? &attr : 0);
  if (err != HOT_OK)
    error->panic(err, "Maestro_Thread::create: hot_thread_Create");
}

void
Maestro_Thread::usleep(int usecs) {
  hot_err_t err ;
  err = hot_thread_Usleep(usecs);
  if (err != HOT_OK)
    Maestro_DefaultErrorHandler.panic(err, "Maestro_Thread::usleep: hot_thread_Usleep");
}

/********************** Maestro_Semaphore ***********************************/


Maestro_Semaphore::Maestro_Semaphore(int initial_value,
                                     Maestro_ErrorHandler *error_handler) :
  Maestro_Base(error_handler) {
  err = hot_sema_Create(initial_value, &sema);
  check_err("Maestro_Semaphore:hot_sema_Create");
}


Maestro_Semaphore::Maestro_Semaphore(Maestro_Semaphore& hs) {
  error->panic("Maestro_Semaphore: copy constructor not supported");
  assert(0);
}


Maestro_Semaphore::~Maestro_Semaphore() {
  err = hot_sema_Destroy(sema);
  check_err("Maestro_Semaphore:hot_sema_Destroy");
}

Maestro_Semaphore&
Maestro_Semaphore::operator = (Maestro_Semaphore& hs) {
  error->panic("Maestro_Semaphore: operator= not supported");
  assert(0);
  return *this;
}

void
Maestro_Semaphore::inc() {
  err = hot_sema_Inc(sema);
  check_err("Maestro_Semaphore: hot_sema_Inc");
}

void
Maestro_Semaphore::dec() {
  err = hot_sema_Dec(sema);
  check_err("Maestro_Semaphore: hot_sema_Dec");
}

/************************ Maestro_Lock *************************************/

Maestro_Lock::Maestro_Lock(Maestro_ErrorHandler *error_handler) :
  Maestro_Base(error_handler) {
  err = hot_lck_Create(&lck);
  check_err("Maestro_Lock::Maestro_Lock:hot_lck_Create");
}

Maestro_Lock::Maestro_Lock(Maestro_Lock& hl) {
  error->panic("Maestro_Lock::Maestro_Lock: copy constructor not supported");
  assert(0);
}


Maestro_Lock::~Maestro_Lock() {
  err = hot_lck_Destroy(lck);
  check_err("Maestro_Lock::~Maestro_Lock:hot_lck_Destroy");
}

Maestro_Lock&
Maestro_Lock::operator= (Maestro_Lock& hl) {
  error->panic("Maestro_Lock::operator= not supported");
  assert(0);
  return *this;
}

void
Maestro_Lock::lock() {
  err = hot_lck_Lock(lck);
  check_err("Maestro_Lock::Maestro_Lock:hot_lck_Lock");
}

void
Maestro_Lock::unlock() {
  err = hot_lck_Unlock(lck);
  check_err("Maestro_Lock::Maestro_Unlock:hot_lck_Unlock");
}


/********************* Maestro_Barrier **************************************/

Maestro_Barrier::Maestro_Barrier(Maestro_BarrierState theState,
                                 Maestro_ErrorHandler *error_handler) :
  Maestro_Base(error_handler) {
  sema = new Maestro_Semaphore(0, error_handler);
  mutex = new Maestro_Lock(error_handler);
  nwaiting = 0;
  state = theState;
}

Maestro_Barrier::Maestro_Barrier(Maestro_Barrier& hc) {
  error->panic("Maestro_Barrier: copy constructor not supported");
  assert(0);
}

Maestro_Barrier::~Maestro_Barrier() {
  mutex->lock();
  open();
  delete sema;
  mutex->unlock();
  delete mutex;
}

Maestro_Barrier&
Maestro_Barrier::operator = (Maestro_Barrier& hc) {
  error->panic("Maestro_Barrier: operator= not supported");
  assert(0);
  return *this;
}

void
Maestro_Barrier::close() {
  mutex->lock();
  state = MAESTRO_BARRIER_CLOSED;
  mutex->unlock();
}

void
Maestro_Barrier::open() {
  mutex->lock();
  if (state == MAESTRO_BARRIER_CLOSED) {
    for (; nwaiting; nwaiting--) {
      sema->inc();
    }
    state = MAESTRO_BARRIER_OPEN;
  }
  mutex->unlock();
}

void
Maestro_Barrier::pass() {
  mutex->lock();
  if (state == MAESTRO_BARRIER_CLOSED) {
    nwaiting++;
    mutex->unlock();
    sema->dec();
  }
  else {
    mutex->unlock();
  }
}

/***************************** Maestro_Entity ********************************/

Maestro_EndpID::Maestro_EndpID(Maestro_ErrorHandler *error_handler) :
  Maestro_Base(error_handler) {
}

Maestro_EndpID::Maestro_EndpID(hot_endpt_t ep,
                               Maestro_ErrorHandler *error_handler) :
  Maestro_Base(error_handler) {
  endpID = ep;
}

int
Maestro_EndpID::operator== (Maestro_EndpID &eid) {
  return (strncmp(endpID.name,eid.endpID.name,HOT_ENDP_MAX_NAME_SIZE) == 0);
}

int
Maestro_EndpID::operator!= (Maestro_EndpID &eid) {
  return !(*this == eid);
}

Maestro_EndpID&
Maestro_EndpID::operator= (Maestro_EndpID &eid) {
  endpID = eid.endpID;

  return *this;
}

Maestro_EndpID&
Maestro_EndpID::operator= (hot_endpt_t &ep) {
  endpID = ep;

  return *this;
}

void
Maestro_EndpID::operator << (Maestro_Message &msg) {
  msg.read(&endpID, sizeof(endpID));
}

void
Maestro_EndpID::operator >> (Maestro_Message &msg) {
  msg.write(&endpID, sizeof(endpID));
}


/***************************** Maestro_EndpList ***************************/

Maestro_EndpList::Maestro_EndpList(Maestro_ErrorHandler *error_handler) :
  Maestro_EndpList_Base(error_handler) {}

Maestro_EndpList::Maestro_EndpList(Maestro_EndpID& eid,
                                   Maestro_ErrorHandler *error_handler) :
  Maestro_EndpList_Base(eid, error_handler) {}

Maestro_EndpList::Maestro_EndpList(Maestro_EndpList& ep_list,
                                   Maestro_ErrorHandler *error_handler) :
  Maestro_EndpList_Base(ep_list, error_handler) {}

Maestro_EndpList::Maestro_EndpList(hot_endpt_t endps[], unsigned nendp,
                                   Maestro_ErrorHandler *error_handler) :
  Maestro_Base(error_handler) {
  listSize = nendp;
  if (listSize) {
    list = new Maestro_EndpID*[listSize];
    for (unsigned i = 0; i < listSize; i++)
      list[i] = new Maestro_EndpID(endps[i]);
  }
  else
    list = 0;
}


/***************************** Maestro_IntList ***************************/

Maestro_IntList::Maestro_IntList(Maestro_ErrorHandler *error_handler /* = 0 */) :
  Maestro_IntList_Base(error_handler) {}

Maestro_IntList::Maestro_IntList(int &i, Maestro_ErrorHandler *error_handler /* = 0 */) :
  Maestro_IntList_Base(i, error_handler) {}

Maestro_IntList::Maestro_IntList(Maestro_IntList& ilist,
                                 Maestro_ErrorHandler *error_handler /* = 0 */) :
  Maestro_IntList_Base(ilist, error_handler) {}

Maestro_IntList::Maestro_IntList(int *elem, unsigned nelem,
                                 Maestro_ErrorHandler *error_handler /* = 0 */) :
  Maestro_IntList_Base(elem, nelem, error_handler) {}


Maestro_ULongList::Maestro_ULongList(Maestro_ErrorHandler *error_handler /* = 0 */) :
  Maestro_ULongList_Base(error_handler) {}

Maestro_ULongList::Maestro_ULongList(Maestro_ULong &i,
                                     Maestro_ErrorHandler *error_handler /* = 0 */) :
  Maestro_ULongList_Base(i, error_handler) {}

Maestro_ULongList::Maestro_ULongList(Maestro_ULongList& ilist,
                                     Maestro_ErrorHandler *error_handler /* = 0 */) :
  Maestro_ULongList_Base(ilist, error_handler) {}

Maestro_ULongList::Maestro_ULongList(Maestro_ULong *elem, unsigned nelem,
                                     Maestro_ErrorHandler *error_handler /* = 0 */) :
  Maestro_ULongList_Base(elem, nelem, error_handler) {}

/************************ Maestro_Message ***********************************/

Maestro_Message::Maestro_Message(Maestro_ErrorHandler *error_handler) :
  Maestro_Base(error_handler) {
  msg = hot_msg_Create();
}

Maestro_Message::Maestro_Message(Maestro_Message& src_msg,
                                 Maestro_ErrorHandler *error_handler) :
  Maestro_Base(error_handler) {
  err = hot_msg_Alias(src_msg.msg, &msg);
  check_err("Maestro_Message::Maestro_Message: hot_msg_Alias");
}

Maestro_Message::Maestro_Message(hot_msg_t hmsg,
                                 Maestro_ErrorHandler *error_handler) :
  Maestro_Base(error_handler) {
  err = hot_msg_Alias(hmsg, &msg);
  check_err("Maestro_Message::Maestro_Message: hot_msg_Alias");
}

Maestro_Message::Maestro_Message(void *data,
                                 unsigned nbytes,
                                 void (*on_release)(void*),
                                 void *env,
                                 Maestro_ErrorHandler *error_handler) :
  Maestro_Base(error_handler) {
  msg = hot_msg_CreateDontCopy(data, nbytes, on_release, env);
}

Maestro_Message::~Maestro_Message() {
  err = hot_msg_Release(&msg);
  check_err("Maestro_Message::~Maestro_Message: hot_msg_Release");
}

Maestro_Message&
Maestro_Message::operator= (Maestro_Message &src_msg) {
  err = hot_msg_Release(&msg);
  check_err("Maestro_Message::operator=: hot_msg_Release");

  err = hot_msg_Alias(src_msg.msg, &msg);
  check_err("Maestro_Message::operator=: hot_msg_Alias");

  return *this;
}

// Make a writable copy.
Maestro_Message&
Maestro_Message::operator<<= (Maestro_Message &src_msg) {
  err = hot_msg_Release(&msg);
  check_err("Maestro_Message::operator=: hot_msg_Release");

  err = hot_msg_Copy(src_msg.msg, &msg);
  check_err("Maestro_Message::operator<<=: hot_msg_Copy");

  return *this;
}

// Make msg empty + writable.
void
Maestro_Message::reset() {
  err = hot_msg_Release(&msg);
  check_err("Maestro_Message::reset: hot_msg_Release");

  msg = hot_msg_Create();
}

unsigned
Maestro_Message::size() {
  unsigned s;
  err = hot_msg_Size(msg, &s);
  check_err("Maestro_Message::size: hot_msg_Size");
  return s;
}

unsigned
Maestro_Message::getPos() {
  unsigned pos;
  err = hot_msg_GetPos(msg, &pos);
  check_err("Maestro_Message::getPos: hot_msg_GetPos");
  return pos;
}

void
Maestro_Message::setPos(unsigned offset) {
  err = hot_msg_SetPos(msg, offset);
  check_err("Maestro_Message::setPos: hot_msg_SetPos");
}

void
Maestro_Message::seek(int offset, Maestro_MsgSeek whence) {
  err = hot_msg_Seek(msg, (hot_msg_seek_t) whence, offset);
  check_err("Maestro_Message::seek: hot_msg_Seek");
}

void
Maestro_Message::readDir(Maestro_MsgReadDir dir) {
  err = hot_msg_ReadDirection(msg, (hot_msg_readDirection_t) dir);
  check_err("Maestro_Message::readDir: hot_msg_ReadDirection");
}

void
Maestro_Message::align(unsigned nbytes) {
  err = hot_msg_Align(msg, nbytes);
  check_err("Maestro_Message::align: hot_msg_Align");
}

void
Maestro_Message::read(int &i) {
#ifdef MAESTRO_MESSAGE_TYPECHECKING
  hot_uint32 magic;
  err = hot_msg_ReadUint32(msg, &magic);
  check_err("Maestro_Message::read: hot_msg_ReadUint32");
  if (magic != MAESTRO_MESSAGE_MAGIC_INT)
    error->panic("Maestro_Message::read(int):  WRONG MAGIC NUMBER");
#endif

  hot_int32 i32;
  err = hot_msg_ReadUint32(msg, (hot_uint32*) &i32);
  check_err("Maestro_Message::read(int): hot_msg_ReadUint32");
  i = i32;
}

void
Maestro_Message::read(void *buf, unsigned size) {
#ifdef MAESTRO_MESSAGE_TYPECHECKING
  hot_uint32 magic;
  err = hot_msg_ReadUint32(msg, &magic);
  check_err("Maestro_Message::read: hot_msg_ReadUint32");
  if (magic != MAESTRO_MESSAGE_MAGIC_BUFFER)
    error->panic("Maestro_Message::read(buffer):  WRONG MAGIC NUMBER");
#endif

  err = hot_msg_Read(msg, buf, size);
  check_err("Maestro_Message::read(buffer): hot_msg_Read");
}

void
Maestro_Message::look(void *&data, unsigned size) {
  err = hot_msg_Look(msg, size, &data);
  check_err("Maestro_Message::look: hot_msg_Look");
}

void
Maestro_Message::write(int i) {
  hot_int32 data = i;
  err = hot_msg_WriteUint32(msg, (hot_uint32*) &data);
  check_err("Maestro_Message::write(int): hot_msg_WriteUint32");

#ifdef MAESTRO_MESSAGE_TYPECHECKING
  hot_uint32 magic = MAESTRO_MESSAGE_MAGIC_INT;
  err = hot_msg_WriteUint32(msg, &magic);
  check_err("Maestro_Message::write(int): hot_msg_WriteUint32");
#endif
}

void
Maestro_Message::write(void *buf, int size) {
  err = hot_msg_Write(msg, buf, size);
  check_err("Maestro_Message::write(buffer): hot_msg_Write");

#ifdef MAESTRO_MESSAGE_TYPECHECKING
  hot_uint32 magic = MAESTRO_MESSAGE_MAGIC_BUFFER;
  err = hot_msg_WriteUint32(msg, &magic);
  check_err("Maestro_Message::write(buffer): hot_msg_WriteUint32");
#endif
}

void
Maestro_Message::pack(Maestro_Message *msgs, unsigned nmsgs) {
  hot_msg_t *src_msgs = new hot_msg_t[nmsgs];
  for (int i = 0; i < nmsgs; i++)
    src_msgs[i] = msgs[i].msg;
  err = hot_msg_Pack(msg, src_msgs, nmsgs);
  check_err("Maestro_Message::pack: hot_msg_Pack");
  delete [] src_msgs;
}

void
Maestro_Message::unpack(Maestro_Message *&msgs, unsigned &nmsgs) {
  hot_msg_t *dest_msgs;
  err = hot_msg_Unpack(msg, &dest_msgs, &nmsgs);
  check_err("Maestro_Message::unpack: hot_msg_Unpack");

  msgs = new Maestro_Message[nmsgs];
  for (int i = 0; i < nmsgs; i++) {
    Maestro_Message tmp(dest_msgs[i]);
    msgs[i] = tmp;
  }
  free(dest_msgs);
}

int
Maestro_Message::isWritable() {
  return hot_msg_IsWritable(msg);
}

// Pack data_msg into this msg.
void
Maestro_Message::operator<< (Maestro_Message &data_msg) {
  err = hot_msg_Pack(msg, &data_msg.msg, 1);
  check_err("Maestro_Message::operator<<: hot_msg_Pack");
}

// Unpack data_msg from this msg.
void
Maestro_Message::operator>> (Maestro_Message &data_msg) {
  unsigned nmsgs;
  hot_msg_t *mp;

  err = hot_msg_Release(&data_msg.msg);
  check_err("Maestro_Message::operator>>: hot_msg_Release");

  err = hot_msg_Unpack(msg, &mp, &nmsgs);
  check_err("Maestro_Message::operator>>: hot_msg_Unpack");
  if (nmsgs != 1)
    error->panic("Maestro_Message::operator>>: nmsgs != 1 ???");

  data_msg.msg = *mp;
}


/*********************** Maestro_String ***********************/

Maestro_String::Maestro_String(const char *str) {
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


Maestro_String::~Maestro_String() {
  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);
  assert(size == strlen(s) + 1);

  if (s != NULL_STRING) {
    delete [] s;
  }
}


Maestro_String::Maestro_String(Maestro_String& str) {
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


Maestro_String&
Maestro_String::operator = (Maestro_String &str) {
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


Maestro_String&
Maestro_String::operator= (const char *p) {
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
Maestro_String::operator== (Maestro_String& str) {
  assert(size && s != NULL);
  assert(str.size && str.s != NULL);
  assert(str.size == strlen(str.s) + 1);
  assert(NULL_STRING[0] == 0);
  assert(size == strlen(s) + 1);

  return (strcmp(s, str.s) == 0);
}

void
Maestro_String::operator >> (std::ostream &out) const {
  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);
  assert(size == strlen(s) + 1);

  out << s;
}

void
Maestro_String::operator >> (Maestro_Message& msg) {
  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);
  assert(size == strlen(s) + 1);

  msg.write(s, size);
  msg.write(size);
}

void
Maestro_String::operator << (Maestro_Message& msg) {
  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);
  assert(size == strlen(s) + 1);

  if (s != NULL_STRING) {
    delete [] s;
  }

  msg.read(size);
  assert(size > 0);
  s = new char[size];
  msg.read(s, size);

  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);
  assert(size == strlen(s) + 1);
}


void
Maestro_String::operator += (char *src) {
  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);
  assert(size == strlen(s) + 1);

  if (src == NULL)
    return;

  char *tmp = new char [size + strlen(src)];
  memset(tmp, 0, size + strlen(src));
  strcat(tmp, s);
  strcat(tmp, src);

  assert(strlen(tmp) == strlen(s) + strlen(src));

  if (s != NULL_STRING) {
    delete [] s;
  }

  s = tmp;
  size = strlen(tmp) + 1;

  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);
  assert(size == strlen(s) + 1);
}

void
Maestro_String::operator += (Maestro_String &src) {
  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);
  assert(size == strlen(s) + 1);
  assert(src.size && src.s != NULL);
  assert(src.size == strlen(src.s) + 1);

  if (src.s == NULL_STRING)
    return;

  char *tmp = new char [size + src.size - 1];
  memset(tmp, 0, size + src.size - 1);
  strcat(tmp, s);
  strcat(tmp, src.s);

  assert(strlen(tmp) == strlen(s) + strlen(src.s));

  if (s != NULL_STRING) {
    delete [] s;
  }

  s = tmp;
  size = strlen(tmp) + 1;

  assert(size && s != NULL);
  assert(NULL_STRING[0] == 0);
  assert(size == strlen(s) + 1);
}


char Maestro_String::NULL_STRING[1] = "";
