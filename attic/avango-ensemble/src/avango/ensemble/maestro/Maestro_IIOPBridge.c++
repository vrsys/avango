// $Header$
// 
// Implementation of IIOP Bridge:  Client and Server sides.
//
// Author:  Alexey Vaysburd, Sept. 1997.

#pragma implementation
#include "avango/ensemble/maestro/Maestro_IIOPBridge.h"
#include "avango/ensemble/maestro/Maestro_Perf.h"

/****************************** IIOP Bridge ********************************/

Maestro_IIOP_BridgeBase::Maestro_IIOP_BridgeBase() {
  Maestro_GIOP_MessageHeader msgHdr;
  msgHdrSize = msgHdr.marshalSize();
}


/************************** Maestro_IIOP_ConnRef ***************************/

Maestro_IIOP_ConnRef::Maestro_IIOP_ConnRef(
    Maestro_IIOP_ConnId cid_,
    Maestro_IIOP_Connection *conn_
){
  _cid = cid_;
  _conn = conn_; 
}
    		       		   
Maestro_IIOP_ConnRef::Maestro_IIOP_ConnRef(Maestro_IIOP_ConnRef &cr) {
  _cid = cr._cid;
  _conn = cr._conn;
}


/********************** Maestro_IIOP_ConnRefList ***************************/

// Close all active connections.
void 
Maestro_IIOP_ConnRefList::closeAll() {
  mutex.lock();
  int i;
  for (i = 0; i < active.size(); i++) {
    active[i].conn()->close();
  }
  active.clear();
  mutex.unlock();
}

// Add a connection to the active list.
void 
Maestro_IIOP_ConnRefList::add(Maestro_IIOP_Connection *conn) {
  Maestro_IIOP_ConnRef cref(conn->id(), conn);
  mutex.lock();
  active += cref;
  mutex.unlock();
  }

// Remove a connection from the active list.
void 
Maestro_IIOP_ConnRefList::remove(Maestro_IIOP_Connection *conn) {
  Maestro_IIOP_ConnRef cref(conn->id(), conn);
  mutex.lock();
  active -= cref;
  mutex.unlock();
}

// Get a reference to an active connection.  Increase connection's refcnt.
// The caller is responsible for unref'ing the connection when done.
Maestro_IIOP_Connection* 
Maestro_IIOP_ConnRefList::ref(Maestro_IIOP_ConnId cid) {
  int i;
  mutex.lock();
  for (i = 0; i < active.size(); i++) {
    if (active[i].id() == cid) {
      Maestro_IIOP_Connection *conn = active[i].conn();
      conn->ref();
      mutex.unlock();
      return conn;
    }
  }
  mutex.unlock();
  return NULL;
}


/*********************** Maestro_IIOP_Connection ***************************/

Maestro_IIOP_Connection::Maestro_IIOP_Connection(
    Maestro_IIOP_Server *srv_, 
    int fd_, 
    Maestro_IIOP_ConnId connId_
){
  magic = MAGIC;
  srv = srv_;
  _fd = fd_;
  connId = connId_;
  isOpen = 1;
  refcnt = 1;			// For the dispatcher thread.
  inside = 0;
    write_mutex_acquired = 0;
}

Maestro_IIOP_Connection::~Maestro_IIOP_Connection() {
  
  //cerr << "CONNECTION:  deleting" << endl;

  mutex.lock();
  if (!inside)
    error->panic("~Maestro_IIOP_Connection: external deletion not allowed");
  ::close(_fd);
  magic = 0;
  mutex.unlock();

  //cerr << "CONNECTION:  leaving destructor" << endl;
}

// Close this connection.  
// Eventually the connection object will be deleted.
void 
Maestro_IIOP_Connection::close() {
  //cerr << "ENTERING Connection::close()" << endl;

  mutex.lock();
  if (isOpen) {
    isOpen = 0;
  }
  mutex.unlock();

  //cerr << "LEAVING Connection::close()" << endl;
}

// Write mutex should be used to avoid interleaving of messages when
// writing from multiple threads.
void 
Maestro_IIOP_Connection::acquireWriteMutex() {
  writeMutex.lock();
  write_mutex_acquired = 1;
}

void 
Maestro_IIOP_Connection::releaseWriteMutex() {
  write_mutex_acquired = 0;
  writeMutex.unlock();
}
  
// Increase reference count.
void 
Maestro_IIOP_Connection::ref() {
  mutex.lock();
  ++refcnt;

  //cerr << "CONNECTION:  ref: setting refcnt to " << refcnt << endl;

  mutex.unlock();
}

// Decrease reference count.  If 0, delete the object.
void 
Maestro_IIOP_Connection::unref() {
  mutex.lock();
  if (--refcnt == 0) {
    mutex.unlock();
    inside = 1;

    //cerr << "CONNECTION:  unref: setting refcnt to " << refcnt << endl;
    //cerr << "CONNECTION:  unref: deleting this" << endl;

    delete this;
  }
  else {
    //cerr << "CONNECTION:  unref: setting refcnt to " << refcnt << endl;
    mutex.unlock();
  }
}

// Write to this connection.
Maestro_Status 
Maestro_IIOP_Connection::write(void *buf, unsigned nbytes) {
  Maestro_Status st = MAESTRO_OK;
  
  if (!write_mutex_acquired) {
    error->panic("Maestro_IIOP_Connection: write: writeMutex must be acquired first");
  }
  
  mutex.lock();
  if (!isOpen) {
    st = Maestro_Error::create("Maestro_IIOP_Connection: write: connection is closed");
  }
  else {
    unsigned res;
    res = ::write(_fd, buf, nbytes);
    if (res != nbytes) {
      close();
      st = Maestro_Error::create("Maestro_IIOP_Connection: write failed (connection closed)");
    }
  }
  mutex.unlock();
  
  return st;
}


// Dispatch incoming messages through the server's callback.
void 
Maestro_IIOP_Connection::dispatcher(void *arg) {
  int ret, width;
  Maestro_IIOP_Connection *conn = (Maestro_IIOP_Connection*) arg;
  Maestro_IIOP_Server *srv = conn->srv;
  fd_set read_fdset;
  struct timeval tv;

  //cerr << "Connection: entering dispatcher" << endl;
  
  while (conn->isOpen) { 
    tv.tv_sec = MASTRO_SELECT_TV_SEC;
    tv.tv_usec = MASTRO_SELECT_TV_USEC;
    FD_ZERO(&read_fdset);
    FD_SET(conn->fd(), &read_fdset);
    ret = select(conn->fd() + 1, &read_fdset, NULL, NULL, &tv);
    
    switch (ret) {
    case 0:
      break;
    case 1:
      assert(FD_ISSET(conn->fd(), &read_fdset));
      // Assumption:  server closes all connections when deleted.
      // => If connection is open, the reference to server must be valid.
      if (conn->isOpen) {
	assert(srv->magic == srv->MAGIC);

	perf_start(0, "Avg. request processing cost:");
	srv->dispatchIncoming(conn);
      }
      break;
    default:
      conn->error->panic("Maestro_IIOP_Connection: select returned %d", ret);
      break;
    }
  }

  perf_print();

  //cerr << "Connection: about to leave dispatcher (conn->isOpen == 0)" << endl;

  srv->connectionClosed(conn);

  //cerr << "Connection:  removed itself from the server's active list" << endl;

  conn->unref();

  //cerr << "Connection:  leaving dispatcher now" << endl;
}


/*************************** Maestro_IIOP_Reply ***************************/

Maestro_IIOP_Reply::Maestro_IIOP_Reply(
    Maestro_GIOP_MsgType msgType,
    Maestro_IIOP_Client *clt
){
  type = msgType;
  client = clt;
}


/*************************** Maestro_IIOP_Client ***************************/

Maestro_IIOP_Client::Maestro_IIOP_Client(
    Maestro_String host, 
    unsigned short port
){
  requestId = 0;
  magic = MAGIC;
  closed = 1;
  state = OFF;
  
  // Create a socket; connect to the server.
  connect(host.s, port);
  
  // Start a thread to dispatch incoming messages.
  if (state == ON) {
    refcnt = 1;			// For dispatcher thread.
    Maestro_Thread::create(dispatcher_Thread, this);
  }
  else {
    refcnt = 0;		// No references to the client:
    sema.inc();		// can be deleted immediately.
  }
}

Maestro_IIOP_Client::~Maestro_IIOP_Client() {
  state = OFF;
  sema.dec();
  
  mutex.lock();
  magic = 0;
  if (!closed) {
    ::close(serverFd);
    closed = 1;
  }
  mutex.unlock();
}


/************************* IIOP downcalls **************************/

Maestro_Status 
Maestro_IIOP_Client::request(
    Maestro_CORBA_Boolean response_expected,
    Maestro_CORBA_OctetSequence &object_key,
    Maestro_CORBA_String &operation,
    Maestro_CORBA_Message &request_body,
    /*OUT*/ Maestro_CORBA_ULong &request_id
){
  if (state != ON) {
    return Maestro_Error::create("connection closed");
  }
  
  if ((request_id = ++requestId) == 0) {
    error->panic("Maestro_IIOP_Client: requestId wrap");
  }    
  Maestro_GIOP_RequestHeader reqHdr(request_id, 
				    response_expected,
				    object_key,
				    operation);
  Maestro_GIOP_MessageHeader msgHdr(MAESTRO_GIOP_MSG_TYPE_REQUEST, 0);
  Maestro_CORBA_Message reqMsg;
  
  // Pad the headers to 8-byte alignment. 
  // (this is necessary in case the first parameter passed with
  // request_body is a double).
  reqMsg << msgHdr << reqHdr;
  int size = reqMsg.size();
  int pad = 0;
  if (size % 8) { 
    pad = 8 - size % 8;
    reqHdr.requesting_principal.size(pad); 
  }
  
  msgHdr.message_size = (size + pad - msgHdrSize) + request_body.size();
  reqMsg.reset();
  reqMsg << msgHdr << reqHdr;
  reqMsg.write(request_body.body(), request_body.size());
  
  int nbytes;
  writeMutex.lock();
  if ((nbytes = ::write(serverFd, reqMsg.body(), reqMsg.size())) 
      != reqMsg.size()) {
    state = OFF;
    writeMutex.unlock();
    return Maestro_Error::create("Maestro_IIOP_Client: write failed");
  }
  writeMutex.unlock();
  
  return MAESTRO_OK;
}


Maestro_Status 
Maestro_IIOP_Client::locateRequest(
    Maestro_CORBA_OctetSequence &object_key,
    /*OUT*/ Maestro_CORBA_ULong &request_id
){
  if (state != ON) {
    return Maestro_Error::create("connection closed");
  }
  
  if ((request_id = ++requestId) == 0) {
    error->panic("Maestro_IIOP_Client: requestId wrap");
  }    
  Maestro_GIOP_LocateRequestHeader locReqHdr(request_id, object_key);
  Maestro_GIOP_MessageHeader msgHdr(MAESTRO_GIOP_MSG_TYPE_LOCATE_REQUEST, 
				    locReqHdr.marshalSize());
  Maestro_CORBA_Message locReqMsg;  
  locReqMsg << msgHdr << locReqHdr;
  
  int nbytes;
  writeMutex.lock();
  if ((nbytes = ::write(serverFd, locReqMsg.body(), locReqMsg.size())) 
      != locReqMsg.size()) {
    state = OFF;
    writeMutex.unlock();
    return Maestro_Error::create("Maestro_IIOP_Client: write failed");
  }
  writeMutex.unlock();
  
  return MAESTRO_OK;
}


Maestro_Status 
Maestro_IIOP_Client::cancelRequest(Maestro_CORBA_ULong request_id) {
  if (state != ON) {
    return Maestro_Error::create("connection closed");
  }
  
  Maestro_GIOP_CancelRequestHeader cancReqHdr(request_id);
  Maestro_GIOP_MessageHeader msgHdr(MAESTRO_GIOP_MSG_TYPE_CANCEL_REQUEST, 
				    cancReqHdr.marshalSize());
  Maestro_CORBA_Message cancReqMsg;  
  cancReqMsg << msgHdr << cancReqHdr;
  
  int nbytes;
  writeMutex.lock();
  if ((nbytes = ::write(serverFd, cancReqMsg.body(), cancReqMsg.size())) 
      != cancReqMsg.size()) {
    state = OFF;
    writeMutex.unlock();
    return Maestro_Error::create("Maestro_IIOP_Client: write failed");
  }
  writeMutex.unlock();
  
  return MAESTRO_OK;
}

/////////////////////////////////////////////////////////////////////////////
  
// Get a reference to the client object.  
// The client is not deleted until all references are released.
void 
Maestro_IIOP_Client::ref() {
  mutex.lock();
  refcnt++;
  mutex.unlock();
}

// Release a reference to the client object.
// If all references have been released, signal the destructor.
void 
Maestro_IIOP_Client::unref() {
  mutex.lock();
  if (--refcnt == 0) {
    sema.inc();
  }
  mutex.unlock();
}

///////////////////////////////////////////////////////////////////////////

// Connect to the server.
void 
Maestro_IIOP_Client::connect(char *host, unsigned short port) {
  struct hostent *hp;
  struct sockaddr_in server;
  
  if (host == NULL) {
    error->warning("Maestro_IIOP_Client: bad host name");
    state = OFF;
    return;
  }
  
  memset(&server, 0, sizeof(server));
  
  serverFd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (serverFd < 0) {
    error->warning("Maestro_IIOP_Client: socket() failed");
    state = OFF;
    return;
  }
  closed = 0;
  
  server.sin_family = AF_INET;
  hp = gethostbyname(host);
  if (hp == 0){
    ::close(serverFd);
    closed = 1;
    error->warning("Maestro_IIOP_Client: unknown host: %s", host);
    state = OFF;
    return;
  }
  
  memcpy((char*) &server.sin_addr, (char*) hp->h_addr, hp->h_length);
  server.sin_port = htons(port);
  
  if (::connect(serverFd, (struct sockaddr*) &server, sizeof(server)) < 0) {
    ::close(serverFd);
    closed = 1;
    error->warning("Maestro_IIOP_Client: could not connect to server");
    state = OFF;
    return;      
  }
  
  state = ON;
}

////////////////////////////////////////////////////////////////////////////

// Read an incoming message and process it in a separate thread.
void 
Maestro_IIOP_Client::handleIncoming() {
  char *data = new char[msgHdrSize];
  unsigned nread = ::read(serverFd, data, msgHdrSize);
  if (nread != msgHdrSize) {
    error->warning("Maestro_IIOP_Client: read failed (closing connection)");
    state = OFF;
    delete [] data;
    return;
  }
  
  Maestro_CORBA_Message msg(data, msgHdrSize);
  Maestro_GIOP_MessageHeader hdr;
  msg >> hdr;
  if (hdr.byte_order != Maestro_CORBA_Message::localByteOrder())
    hdr.message_size = Maestro_CORBA_Message::swap(hdr.message_size);
  delete [] data;
  
  // Consistency check.
  if (memcmp(MAESTRO_GIOP_MAGIC, 
	     hdr.magic, 
	     strlen((const char*) MAESTRO_GIOP_MAGIC)) != 0) {
    error->warning("Maestro_IIOP_Client: bad magic (closing connection)");
    state = OFF;
    return;
  }
  if (hdr.GIOP_version.major != MAESTRO_GIOP_MAJOR ||
      hdr.GIOP_version.minor != MAESTRO_GIOP_MINOR) {
    error->warning("Maestro_IIOP_Client: message GIOP version %d.%d (local %d.%d)",
		   hdr.GIOP_version.major, hdr.GIOP_version.minor,
		   MAESTRO_GIOP_MAJOR, MAESTRO_GIOP_MINOR);
  }    
  
  Maestro_IIOP_Reply *rep;
  
  switch (hdr.message_type) {
  case MAESTRO_GIOP_MSG_TYPE_REPLY: 
    {    
      data = new char[hdr.message_size];
      nread = ::read(serverFd, data, hdr.message_size);
      if (nread != hdr.message_size) {
	error->warning("Maestro_IIOP_Client: read failed (closing connection)");
	state = OFF;
	delete [] data;
	return;
      }
      Maestro_GIOP_ReplyHeader repHdr;
      Maestro_CORBA_Message msg(data, hdr.message_size);
      msg.msgByteOrder(hdr.byte_order);
      msg >> repHdr;
      // NB:  'data' will be delete'd from reply_Thread.
      
      rep = new Maestro_IIOP_Reply(MAESTRO_GIOP_MSG_TYPE_REPLY, this);
      rep->request_id = repHdr.request_id;
      rep->reply_status = repHdr.reply_status;
      rep->msg = msg;
      rep->data = data;
      ref(); // Matching unref is invoked from the corresponding thread when done.
      Maestro_Thread::create(reply_Thread, rep);
      break;
    }
  case MAESTRO_GIOP_MSG_TYPE_LOCATE_REPLY: 
    {    
      data = new char[hdr.message_size];
      nread = ::read(serverFd, data, hdr.message_size);
      if (nread != hdr.message_size) {
	error->warning("Maestro_IIOP_Client: read failed (closing connection)");
	state = OFF;
	delete [] data;
	return;
      }
      Maestro_GIOP_LocateReplyHeader locHdr;
      Maestro_CORBA_Message msg(data, hdr.message_size);
      msg.msgByteOrder(hdr.byte_order);
      msg >> locHdr;
      delete [] data;
      
      rep = new Maestro_IIOP_Reply(MAESTRO_GIOP_MSG_TYPE_LOCATE_REQUEST, this);
      rep->request_id = locHdr.request_id;
      rep->locate_status = locHdr.locate_status;
      
      ref(); // Matching unref is invoked from the corresponding thread when done.
      Maestro_Thread::create(locateReply_Thread, rep);
      break;
    }    
  case MAESTRO_GIOP_MSG_TYPE_CLOSE_CONNECTION: 
    {    
      error->warning("Maestro_IIOP_Client: got CloseConnection message (closing connection)");
      state = OFF;      
      rep = new Maestro_IIOP_Reply(MAESTRO_GIOP_MSG_TYPE_CLOSE_CONNECTION, this);
      ref(); // Matching unref is invoked from the corresponding thread when done.
      Maestro_Thread::create(closeConnection_Thread, rep);
      return;
    }
  case MAESTRO_GIOP_MSG_TYPE_MESSAGE_ERROR: 
    {
      error->warning("Maestro_IIOP_Client: got MessageError message (closing connection)");
      state = OFF;      
      rep = new Maestro_IIOP_Reply(MAESTRO_GIOP_MSG_TYPE_MESSAGE_ERROR, this);
      ref(); // Matching unref is invoked from the corresponding thread when done.
      Maestro_Thread::create(messageError_Thread, rep);
      return;
    }
  default: 
    {
      error->warning("Maestro_IIOP_Client: unexpected msg type %d (closing connection)", 
		     hdr.message_type);
      state = OFF;
      return;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////

void 
Maestro_IIOP_Client::dispatcher_Thread(void *arg) {
  int ret, width;
  Maestro_IIOP_Client *client = (Maestro_IIOP_Client*) arg;
  fd_set read_fdset;
  struct timeval tv;

  //cerr << "Maestro_IIOP_Client: entering dispatcher_Thread" << endl;
  
  while (client->state == ON) { 
    tv.tv_sec = MASTRO_SELECT_TV_SEC;
    tv.tv_usec = MASTRO_SELECT_TV_USEC;
    FD_ZERO(&read_fdset);
    FD_SET(client->serverFd, &read_fdset);
    ret = select(client->serverFd + 1, &read_fdset, NULL, NULL, &tv);
    
    switch (ret) {
    case 0:
      break;
    case 1:
      assert(FD_ISSET(client->serverFd, &read_fdset));
      assert(client->magic == client->MAGIC);
      client->handleIncoming();
      break;
    default:
      client->error->warning("Maestro_IIOP_Client: select returned %d", ret);
      client->state = OFF;
      break;
    }
  }
  
  //cerr << "Maestro_IIOP_Client: state is off, about to leave dispatcher_Thread" 
  //     << endl;

  client->mutex.lock();
  if (!client->closed) {
    ::close(client->serverFd);
    client->closed = 1;
  }
  client->mutex.unlock();
  
  // Release this thread's reference to the client.
  client->unref();

  //cerr << "Maestro_IIOP_Client: now leaving dispatcher_Thread" << endl;
}

//////////////////////////////////////////////////////////////////////////

// Dispatch IIOP Reply message.
void 
Maestro_IIOP_Client::reply_Thread(void *arg) {
  Maestro_IIOP_Reply *rep = (Maestro_IIOP_Reply*) arg;
  assert(rep->type == MAESTRO_GIOP_MSG_TYPE_REPLY);
  assert(rep->client->magic == rep->client->MAGIC);
  rep->client->reply_Callback(rep->request_id, 
			      rep->reply_status,
			      rep->msg);
  delete [] rep->data;
  rep->client->unref();
  delete rep;
}

// Dispatch IIOP LocateReply message.
void 
Maestro_IIOP_Client::locateReply_Thread(void *arg) {
  Maestro_IIOP_Reply *rep = (Maestro_IIOP_Reply*) arg;
  assert(rep->type == MAESTRO_GIOP_MSG_TYPE_LOCATE_REPLY);
  assert(rep->client->magic == rep->client->MAGIC);
  rep->client->locateReply_Callback(rep->request_id, 
				    rep->locate_status);
  rep->client->unref();
}

// Dispatch IIOP MessageError message.
void 
Maestro_IIOP_Client::messageError_Thread(void *arg) {
    Maestro_IIOP_Reply *rep = (Maestro_IIOP_Reply*) arg;
    assert(rep->type == MAESTRO_GIOP_MSG_TYPE_MESSAGE_ERROR);
    assert(rep->client->magic == rep->client->MAGIC);
    rep->client->messageError_Callback();
    rep->client->unref();
  }

// Dispatch IIOP CloseConnection message.
void 
Maestro_IIOP_Client::closeConnection_Thread(void *arg) {
  Maestro_IIOP_Reply *rep = (Maestro_IIOP_Reply*) arg;
  assert(rep->type == MAESTRO_GIOP_MSG_TYPE_CLOSE_CONNECTION);
  assert(rep->client->magic == rep->client->MAGIC);
  rep->client->closeConnection_Callback();
  rep->client->unref();
}


/************************* Maestro_IIOP_Request ***************************/

Maestro_IIOP_Request::Maestro_IIOP_Request(
    Maestro_GIOP_MsgType msgType,
    Maestro_IIOP_Server *srv, 
    Maestro_IIOP_ConnId id
){
  type = msgType;
  server = srv;
  cid = id;
}


/*************************** Maestro_IIOP_Server ***************************/


Maestro_IIOP_Server::Maestro_IIOP_Server(unsigned short port) {
  refcnt = 1;			// for the connectionListener thread.
  connId = 0;
  magic = MAGIC;
  closed = 1;
  active = new Maestro_IIOP_ConnRefList;
  isClosed = 0;

  if (port == 0) {
    char *p = getenv("MAESTRO_IIOP_PORT");
    if (p == NULL) {
      error->panic("Maestro_IIOP_Server(port): a valid port must be specified,\n or MAESTRO_IIOP_PORT environment variable must be set.");
    }
    port = atoi(p);
  }
  _port = port;
  
  // Bind to the specified port and listen on it.
  acceptConnections(port);

  // Accept connection requests.
  Maestro_Thread::create(connectionListener_Thread, this);
}

Maestro_IIOP_Server::~Maestro_IIOP_Server() {

  //cerr << "CLOSING ALL CONNECTIONS..." << endl;

  // Close all active connections.
  delete active;

  //cerr << "CLOSED ALL CONNECTIONS..." << endl;

  isClosed = 1;
  
  //cerr << "~Maestro_IIOP_Server:  setting isClosed to 1" << endl;

  // Wait till connectionListener and all incoming-request 
  // threads release their references.
  sema.dec();
  
  // mutex.lock() guarantees the object will not be released before 
  // the last call to unref() returns.
  mutex.lock();		
  magic = 0;
  if (!closed) {
    ::close(listenFd);
    closed = 1;
  }
  mutex.unlock();
}

void 
Maestro_IIOP_Server::closeConnection(Maestro_IIOP_ConnId cid) {
  // Get a hold on cid;  close if it's on the active list.
  Maestro_IIOP_Connection *conn = active->ref(cid);
  if (conn != NULL) {
    conn->close();
    conn->unref();
  }
}

/********************** IIOP Downcalls ************************/

Maestro_Status 
Maestro_IIOP_Server::requestReply(
    Maestro_CORBA_ULong request_id,
    Maestro_GIOP_ReplyStatusType reply_status,
    Maestro_CORBA_Message &reply_body,
    Maestro_IIOP_ConnId cid
){
  // cout << "Maestro_IIOP_Server::requestReply: request_id = " 
  //     << request_id << ", cid = " << cid << endl;

  // Get a hold on the cid.  Return an error if it's invalid.
  Maestro_IIOP_Connection *conn = active->ref(cid);
  if (conn == NULL) {
    return Maestro_Error::create("Maestro_IIOP_Server: bad connection");
  }

  perf_start(21, "requestReply: marshaling headers and reply body:");
  
  // Marshal the headers and reply body.

  perf_start(51, "requestReply: make msg:");
  Maestro_GIOP_ReplyHeader repHdr(request_id, reply_status);
  Maestro_GIOP_MessageHeader msgHdr(MAESTRO_GIOP_MSG_TYPE_REPLY, 
				    repHdr.marshalSize() + reply_body.size());
  Maestro_CORBA_Message repMsg;
  perf_end(51);

  perf_start(52, "requestReply: marshal headers:");
  repMsg << msgHdr << repHdr;
  perf_end(52);

  perf_start(53, "requestReply: write msg body");
  repMsg.write(reply_body.body(), reply_body.size());
  perf_end(53);

  perf_end(21);

  // Send the message.  If write fails, close the connection 
  // and return an error.
  int nbytes;
  conn->acquireWriteMutex();

  perf_start(22, "sending reply:");

  if ((nbytes = ::write(conn->fd(), repMsg.body(), repMsg.size())) 
      != repMsg.size()) {
    conn->releaseWriteMutex();
    conn->close();
    conn->unref();
    return Maestro_Error::create("Maestro_IIOP_Server: write failed (closing connection)");
  }

  perf_end(22);

  conn->releaseWriteMutex();

  conn->unref();

  perf_end(0);

  return MAESTRO_OK;
}

Maestro_Status 
Maestro_IIOP_Server::locateReply(
    Maestro_CORBA_ULong request_id,
    Maestro_GIOP_LocateStatusType locate_status,
    Maestro_IIOP_ConnId cid
){
  // Get a hold on the cid.  Return an error if it's invalid.
  Maestro_IIOP_Connection *conn = active->ref(cid);
  if (conn == NULL) {
    return Maestro_Error::create("Maestro_IIOP_Server: bad connection");
  }
  
  // Marshal the headers and reply body.
  Maestro_GIOP_LocateReplyHeader locHdr(request_id, locate_status);
  Maestro_GIOP_MessageHeader msgHdr(MAESTRO_GIOP_MSG_TYPE_LOCATE_REPLY, 
				    locHdr.marshalSize());
  Maestro_CORBA_Message repMsg;
  repMsg << msgHdr << locHdr;
  
  // Send the message.  If write fails, close the connection 
  // and return an error.
  int nbytes;
  conn->acquireWriteMutex();
  if ((nbytes = ::write(conn->fd(), repMsg.body(), repMsg.size())) 
      != repMsg.size()) {
    conn->releaseWriteMutex();
    conn->close();
    conn->unref();
    return Maestro_Error::create("Maestro_IIOP_Server: write failed (closing connection)");
  }
  conn->releaseWriteMutex();

  conn->unref();
  return MAESTRO_OK;
}

/*****************************************************************/

// Bind to the specified port and listen to connection requests.
void
Maestro_IIOP_Server::acceptConnections(unsigned short port) {
  struct sockaddr_in sock;
  memset(&sock, 0, sizeof(sock));
  
  if ((listenFd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    error->panic("Maestro_IIOP_Server: socket() failed");
  }
  
  sock.sin_family      = AF_INET;
  sock.sin_addr.s_addr = INADDR_ANY;
  sock.sin_port        = port;
  
  if (::bind(listenFd, (sockaddr*) &sock, sizeof(sock)) < 0) {
    ::close(listenFd);
    error->panic("Maestro_IIOP_Server: could not bind to port %d", port);
  }
  
  if (::listen(listenFd, 5) < 0) {
    ::close(listenFd);
    error->panic("Maestro_IIOP_Server: listen() failed");
  }
  
  closed = 0;
}

// Get a reference to the server object.  
// The server is not deleted until all references are released.
void 
Maestro_IIOP_Server::ref() {
  mutex.lock();
  refcnt++;
  mutex.unlock();
}

// Release a reference to the server object.
// If all references have been released, signal the destructor.
void 
Maestro_IIOP_Server::unref() {
  mutex.lock();
  if (--refcnt == 0) {
    sema.inc();
  }
  mutex.unlock();
}

void 
Maestro_IIOP_Server::connectionClosed(Maestro_IIOP_Connection *conn) {
  // Remove connection from the active list.
  active->remove(conn);
}


void 
Maestro_IIOP_Server::dispatchIncoming(Maestro_IIOP_Connection *conn) {
  // Read and dispatch an IIOP message.  
  // Read in the same thread, dispatch in another one.
  // If read fails, close the bad connection + remove it from the active list.
  assert(conn->magic == conn->MAGIC);
  char *data = new char[msgHdrSize];

  perf_start(1, "reading GIOP message header:");
  unsigned nread = ::read(conn->fd(), data, msgHdrSize);
  perf_end(1);

  if (nread != msgHdrSize) {
    error->warning("Maestro_IIOP_Server: read failed (closing connection)");
    conn->close();
    delete [] data;
    return;
  }

  perf_start(2, "processing message header:");

  perf_start(41, "make CORBA msg:");
  Maestro_CORBA_Message msg(data, msgHdrSize);
  perf_end(41);

  perf_start(42, "stream msg into GIOP msg header:");
  Maestro_GIOP_MessageHeader hdr;
  msg >> hdr;
  perf_end(42);

  if (hdr.byte_order != Maestro_CORBA_Message::localByteOrder())
    hdr.message_size = Maestro_CORBA_Message::swap(hdr.message_size);
  delete [] data;
  
  // Consistency check.
  if (memcmp(MAESTRO_GIOP_MAGIC, 
	     hdr.magic, 
	     strlen((const char*) MAESTRO_GIOP_MAGIC)) != 0) {
    error->warning("Maestro_IIOP_Server: bad magic (closing connection)");
    conn->close();
    return;
  }
  if (hdr.GIOP_version.major != MAESTRO_GIOP_MAJOR ||
      hdr.GIOP_version.minor != MAESTRO_GIOP_MINOR) {
    error->warning("Maestro_IIOP_Server: message GIOP version %d.%d (local %d.%d)",
		   hdr.GIOP_version.major, hdr.GIOP_version.minor,
		   MAESTRO_GIOP_MAJOR, MAESTRO_GIOP_MINOR);
  }    
  
  Maestro_IIOP_Request *req;

  perf_end(2);
  
  switch (hdr.message_type) {
  case MAESTRO_GIOP_MSG_TYPE_REQUEST:
    {

      perf_start(3, "reading request header:");

      data = new char[hdr.message_size];
      nread = ::read(conn->fd(), data, hdr.message_size);

      perf_end(3);

      if (nread != hdr.message_size) {
	error->warning("Maestro_IIOP_Server: read failed (closing connection)");
	conn->close();
	delete [] data;
	return;
      }

      perf_start(4, "processing request header:");

      Maestro_GIOP_RequestHeader reqHdr;

      perf_start(43, "make CORBA msg:");
      Maestro_CORBA_Message msg(data, hdr.message_size);
      msg.msgByteOrder(hdr.byte_order);
      perf_end(43);

      perf_start(44, "stream msg into req header:");
      msg >> reqHdr;
      perf_end(44);

      // NB:  'data' will be delete'd from request_Thread.
      
      perf_start(45, "make and fill in request struct:");

      req = new Maestro_IIOP_Request(MAESTRO_GIOP_MSG_TYPE_REQUEST,
				     this, 
				     conn->id());
      req->request_id = reqHdr.request_id;
      req->response_expected = reqHdr.response_expected;
      req->object_key <<= reqHdr.object_key;
      req->operation = reqHdr.operation;
      req->requesting_principal = reqHdr.requesting_principal;
      req->msg = msg;
      req->data = data;
      ref(); // Matching unref is invoked from the corresponding thread when done.
      //Maestro_Thread::create(request_Thread, req);

      perf_end(45);

      perf_end(4);

      perf_start(5, "processing request:");
      request_Thread(req); // invoke directly for performance comparison.
      perf_end(5);

      break;
    }
  case MAESTRO_GIOP_MSG_TYPE_LOCATE_REQUEST:
    {
      data = new char[hdr.message_size];
      nread = ::read(conn->fd(), data, hdr.message_size);
      if (nread != hdr.message_size) {
	error->warning("Maestro_IIOP_Server: read failed (closing connection)");
	conn->close();
	delete [] data;
	return;
      }
      Maestro_GIOP_LocateRequestHeader locHdr;
      Maestro_CORBA_Message msg(data, hdr.message_size);
      msg.msgByteOrder(hdr.byte_order);
      msg >> locHdr;
      delete [] data;
      
      req = new Maestro_IIOP_Request(MAESTRO_GIOP_MSG_TYPE_LOCATE_REQUEST,
				     this, 
				     conn->id());
      req->request_id = locHdr.request_id;
      req->object_key <<= locHdr.object_key;
      ref(); // Matching unref is invoked from the corresponding thread when done.
      Maestro_Thread::create(locateRequest_Thread, req);
      break;
    }
  case MAESTRO_GIOP_MSG_TYPE_CANCEL_REQUEST:
    {
      data = new char[hdr.message_size];
      nread = ::read(conn->fd(), data, hdr.message_size);
      if (nread != hdr.message_size) {
	error->warning("Maestro_IIOP_Server: read failed (closing connection)");
	conn->close();
	delete [] data;
	return;
      }
      Maestro_GIOP_CancelRequestHeader reqHdr;
      Maestro_CORBA_Message msg(data, hdr.message_size);
      msg.msgByteOrder(hdr.byte_order);
      msg >> reqHdr;
      delete [] data;
      
      req = new Maestro_IIOP_Request(MAESTRO_GIOP_MSG_TYPE_CANCEL_REQUEST,
				     this, 
				     conn->id());
      req->request_id = reqHdr.request_id;
      ref(); // Matching unref is invoked from the corresponding thread when done.
      Maestro_Thread::create(cancelRequest_Thread, req);
      break;
    }
  case MAESTRO_GIOP_MSG_TYPE_MESSAGE_ERROR:
    {
      error->warning("Maestro_IIOP_Server: got MessageError message (closing connection)");
      Maestro_IIOP_ConnId cid = conn->id();
      conn->close();
      
      req = new Maestro_IIOP_Request(MAESTRO_GIOP_MSG_TYPE_MESSAGE_ERROR,
				     this, 
				     cid);
      ref(); // Matching unref is invoked from the corresponding thread when done.
      Maestro_Thread::create(messageError_Thread, req);
      return;
    }
  default:
    {
      error->warning("Maestro_IIOP_Server: unexpected msg type %d (closing connection)", 
		     hdr.message_type);
      conn->close();
      return;
    }
  }
}

// Accept new connections.
void 
Maestro_IIOP_Server::connectionListener_Thread(void *arg) {
  // In a loop, while not closed:  
  // - select; 
  // - accept connection;
  // - allocate new connection object (use unique connId);
  // - add connection to the active list.
  
  int ret, width;
  Maestro_IIOP_Server *server = (Maestro_IIOP_Server*) arg;
  fd_set read_fdset;
  struct timeval tv;
  int newFd;
  int ok = 1;

  //cerr << "entering connectionListener_Thread" << endl;

  while (ok && !server->isClosed) { 
    tv.tv_sec = MASTRO_SELECT_TV_SEC;
    tv.tv_usec = MASTRO_SELECT_TV_USEC;
    FD_ZERO(&read_fdset);
    FD_SET(server->listenFd, &read_fdset);
    ret = select(server->listenFd + 1, &read_fdset, NULL, NULL, &tv);

    switch (ret) {
    case 0:
      break;
    case 1:

      //cerr << "ACCEPTING CONNECTION" << endl;

      // assert(FD_ISSET(server->listenFd, &read_fdset));
      assert(server->magic == server->MAGIC);
      if ((newFd = ::accept(server->listenFd, NULL, NULL)) < 0) {
	server->error->warning("Maestro_IIOP_Server: accept returned %d", newFd);
	ok = 0;
      }
      else {

	//cerr << "ACCEPTED CONNECTION" << endl;

	Maestro_IIOP_ConnId connId = ++server->connId;
	if (connId == 0) {
	  server->error->warning("Maestro_IIOP_Server: connection ID wrap");
	  ok = 0;
	}
	else {
	  Maestro_IIOP_Connection *conn = 
	    new Maestro_IIOP_Connection(server, newFd, connId); 
	  server->active->add(conn);	  
	  conn->start();
	}
      }
      break;
    default:
      server->error->warning("Maestro_IIOP_Server: select returned %d", ret);
      ok = 0;
      break;
    }
  } 

  //cerr << "leaving connectionListener_Thread" << endl;
  
  // Release this thread's reference to the server object.
  server->unref();
}

//////////////////////////////////////////////////////////////////////////

// Dispatch IIOP Request message.
void 
Maestro_IIOP_Server::request_Thread(void *arg) {
  Maestro_IIOP_Request *req = (Maestro_IIOP_Request*) arg;
  assert(req->type == MAESTRO_GIOP_MSG_TYPE_REQUEST);
  assert(req->server->magic == req->server->MAGIC);

  //cout << "Maestro_IIOP_Server::request_Thread: request_id = " 
  //     << req->request_id << ", cid = " << req->cid << endl;

  req->server->request_Callback(req->request_id, 
				req->response_expected,
				req->object_key,
				req->operation,
				req->requesting_principal,
				req->msg,
				req->cid);
  delete [] req->data;
  req->server->unref();
  delete req;
}

// Dispatch IIOP CancelRequest message.
void 
Maestro_IIOP_Server::cancelRequest_Thread(void *arg) {
  Maestro_IIOP_Request *req = (Maestro_IIOP_Request*) arg;
  assert(req->type == MAESTRO_GIOP_MSG_TYPE_CANCEL_REQUEST);
  assert(req->server->magic == req->server->MAGIC);
  req->server->cancelRequest_Callback(req->request_id, 
				      req->cid);
  req->server->unref();
}

// Dispatch IIOP LocateRequest message.
void 
Maestro_IIOP_Server::locateRequest_Thread(void *arg) {
  Maestro_IIOP_Request *req = (Maestro_IIOP_Request*) arg;
  assert(req->type == MAESTRO_GIOP_MSG_TYPE_LOCATE_REQUEST);
  assert(req->server->magic == req->server->MAGIC);
  req->server->locateRequest_Callback(req->request_id, 
				      req->object_key, 
				      req->cid);
  req->server->unref();
}

// Dispatch IIOP MessageError message.
void 
Maestro_IIOP_Server::messageError_Thread(void *arg) {
  Maestro_IIOP_Request *req = (Maestro_IIOP_Request*) arg;
  assert(req->type == MAESTRO_GIOP_MSG_TYPE_MESSAGE_ERROR);
  assert(req->server->magic == req->server->MAGIC);
  req->server->messageError_Callback(req->cid);
  req->server->unref();
}
