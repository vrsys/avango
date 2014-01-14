// $Header$
// 
// Support for ORB Core and arbitrary ORB's.
//
// Author:  Alexey Vaysburd, Sept. 1997.

#pragma implementation
#include "avango/ensemble/maestro/Maestro_ORB.h"

#include <iomanip>
#include <sstream>

/******************** Maestro_ORB_ObjectKey ********************/

const char *Maestro_ORB_ObjectKey::MAGIC = "MAE:";

// Maestro_ORB_ObjectKey:  Globally unique object keys.

void
Maestro_ORB_ObjectKey::init(char *appName) {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  srandom(tp.tv_usec);
  long r = random();

  char buf[128];
  gethostname(buf, 128);

  long h = 0xffffffff;
  int i;
  for (i = 0; i < strlen(buf); i++) {
    int k = strlen(buf) - i;
    h ^= ~((buf[i] << (k*k*k*k % 31)) + 
	   (buf[i] << (k*k*k % 31)) +
	   (buf[i] << (k*k % 31)) +
	   (buf[i] << (k % 31)));
  }
  
  memset(buf, 0, 128);
  std::ostringstream os(buf);
  os << MAGIC;
  os << std::setw(8) << std::setfill('0') << std::hex << r
     << std::setw(8) << std::setfill('0') << std::hex << h
     << std::setw(8) << std::setfill('0') << std::hex << tp.tv_sec
     << std::setw(8) << std::setfill('0') << std::hex << tp.tv_usec;

  int size = (sizeof(r) + sizeof(h) + sizeof(tp.tv_sec) + 
	      sizeof(tp.tv_usec)) * 2;
  unsigned char cs;
  int magicSize = strlen(MAGIC);
  for (i = cs = 0; i < size; i++) {
    cs += (unsigned char) buf[i + magicSize];
  }
  os << std::setw(2) << std::setfill('0') << std::hex << (long) cs;

  if (appName != NULL) {
    os << ":" << appName;
  }

  key = buf;
}

Maestro_ORB_ObjectKey::Maestro_ORB_ObjectKey(
    Maestro_CORBA_OctetSequence &obj_key
) {
  key.readFrom(obj_key);
}

Maestro_ORB_ObjectKey& 
Maestro_ORB_ObjectKey::operator= (Maestro_ORB_ObjectKey &objKey) { 
  key = objKey.key; 
  return *this;
}

int 
Maestro_ORB_ObjectKey::isValid() { 
  if (strncmp(key.s, MAGIC, strlen(MAGIC)))
    return 0;

  char *p = key.s + strlen(MAGIC);
  char *end = strchr(p, ':');
  int size;
  size = (end == NULL) ? (strlen(p) - 2) : (end - p - 2);
  if (size <= 0) {
    return 0;
  }
  unsigned char cs = 0;
  int i;
  for (i = 0; i < size; i++) {
    cs += (unsigned char) p[i];
  }
  char buf[16];
  std::ostringstream os(buf);
  os << std::setw(2) << std::setfill('0') << std::hex << (long) cs;
  return (strncmp(buf, p + size, 2) == 0);
}

Maestro_CORBA_Long
Maestro_ORB_ObjectKey::hash() {
  Maestro_CORBA_Long l;
  char buf[9];

  if (!isValid()) {
    return -1;
  }

  memcpy(buf, &key.s[strlen(MAGIC)], 8);
  buf[9] = 0;
  sscanf(buf, "%x", &l);

  return l;
}

char* 
Maestro_ORB_ObjectKey::appName() {
  if (!isValid()) {
    return NULL;
  }
  char *p = strchr(key.s, ':');
  assert(p);
  p = strchr(p+1, ':');
  return (p) ? (p+1) : (char*)NULL;
}

/******************** Maestro_ORB_RequestId ********************/

Maestro_ORB_RequestId::Maestro_ORB_RequestId(
    Maestro_CORBA_ULong iiopReqId, 
    Maestro_IIOP_ConnId connId
){
  request_id = iiopReqId;
  cid = connId;
}

Maestro_ORB_RequestId::Maestro_ORB_RequestId(Maestro_ORB_RequestId &reqId) {
  request_id = reqId.request_id; 
  cid = reqId.cid;
}

Maestro_ORB_RequestId& 
Maestro_ORB_RequestId::operator= (Maestro_ORB_RequestId &reqId) {
  request_id = reqId.request_id; 
  cid = reqId.cid;
  return *this;
}

int 
Maestro_ORB_RequestId::operator== (Maestro_ORB_RequestId &reqId) {
  return (request_id == reqId.request_id && cid == reqId.cid);
}


/******************** Maestro_ORB_ObjectBinding ********************/

Maestro_ORB_ObjectBinding::Maestro_ORB_ObjectBinding(
        Maestro_ORB_ObjectBinding &bnd
) {
  obj = bnd.obj;
  orb = bnd.orb;
}

/******************** Maestro_ORB_DispatcherBase ********************/

Maestro_ORB_DispatcherBase::Maestro_ORB_DispatcherBase(unsigned hashSize_) {
  hashSize = hashSize_;
  bindings = new Maestro_ORB_ObjectBindingList[hashSize];
}

Maestro_ORB_DispatcherBase::~Maestro_ORB_DispatcherBase() { 
  mutex.lock(); 
  delete [] bindings; 
  mutex.unlock();
}

// Bind an (object key, ORB) pair with the ORB base.
Maestro_Status
Maestro_ORB_DispatcherBase::bind(
    Maestro_ORB_ObjectKey &objKey, 
    Maestro_ORB_Base *orb
){
  if (!objKey.isValid()) {
    return Maestro_Error::create("bind: invalid object key"); 
  } 
  if (orb == NULL) {
    return Maestro_Error::create("bind: invalid ORB (NULL)");
  }

  Maestro_ORB_ObjectBinding bnd(objKey, orb);
  Maestro_CORBA_Long hash = objKey.hash() % hashSize;
  mutex.lock();
  bindings[hash] -= bnd; // remove the old value if the key was already bound.
  bindings[hash] += bnd;
  mutex.unlock();

  return MAESTRO_OK;
}

Maestro_Status
Maestro_ORB_DispatcherBase::unbind(Maestro_ORB_ObjectKey &objKey) {
  if (!objKey.isValid()) {
    return Maestro_Error::create("unbind: invalid object key"); 
  } 

  Maestro_ORB_ObjectBinding bnd(objKey, NULL);
  Maestro_CORBA_Long hash = objKey.hash() % hashSize;
  mutex.lock();
  bindings[hash] -= bnd; 
  mutex.unlock();

  return MAESTRO_OK;
}

Maestro_ORB_Base*
Maestro_ORB_DispatcherBase::lookup(Maestro_ORB_ObjectKey &objKey) {
  if (!objKey.isValid()) {
    return NULL;
  }

  int i;
  Maestro_ORB_Base *orb = NULL;
  Maestro_ORB_ObjectBinding bnd(objKey, NULL);
  Maestro_CORBA_Long hash = objKey.hash() % hashSize;
  Maestro_ORB_ObjectBindingList *bndList = &bindings[hash];
  mutex.lock();
  for (i = 0; i < bndList->size(); i++) {
    if (bndList->operator[](i) == bnd) {
      orb = bndList->operator[](i).orb;
      break;
    }
  }
  mutex.unlock();
  return orb;
}


/******************** Maestro_ORB_IIOPDispatcher ********************/

Maestro_Status 
Maestro_ORB_IIOPDispatcher::requestReply(
    Maestro_ORB_RequestId &reqId,
    Maestro_GIOP_ReplyStatusType reply_status,
    Maestro_CORBA_Message &reply_body
){
  return Maestro_IIOP_Server::requestReply(reqId.reqId(), 
					   reply_status, 
					   reply_body, 
					   reqId.connId());
}

Maestro_Status 
Maestro_ORB_IIOPDispatcher::locateReply(
    Maestro_ORB_RequestId &reqId,
    Maestro_GIOP_LocateStatusType locate_status
){
  return Maestro_IIOP_Server::locateReply(reqId.reqId(), 
					  locate_status, 
					  reqId.connId());
}

void 
Maestro_ORB_IIOPDispatcher::request_Callback(
    Maestro_CORBA_ULong request_id, 
    Maestro_CORBA_Boolean response_expected,
    Maestro_CORBA_OctetSequence &object_key,
    Maestro_CORBA_String &operation,
    Maestro_GIOP_Principal &requesting_principal,
    Maestro_CORBA_Message &msg,
    Maestro_IIOP_ConnId cid
){
  Maestro_ORB_ObjectKey objKey(object_key);
  Maestro_ORB_Base *orb = lookup(objKey);
  if (orb != NULL) {
    Maestro_ORB_RequestId reqId(request_id, cid);
    orb->request_Callback(reqId, 
			  objKey, 
			  response_expected, 
			  operation, 
			  requesting_principal, 
			  msg);
  }
}

void 
Maestro_ORB_IIOPDispatcher::locateRequest_Callback(
    Maestro_CORBA_ULong request_id, 
    Maestro_CORBA_OctetSequence &object_key,
    Maestro_IIOP_ConnId cid
){
  Maestro_ORB_ObjectKey objKey(object_key);
  Maestro_ORB_Base *orb = lookup(objKey);
  if (orb != NULL) {
    Maestro_ORB_RequestId reqId(request_id, cid);
    orb->locateRequest_Callback(reqId, objKey);
  }
}

//************* Generic configuration parameters *********************
 
Maestro_Conf_Item::Maestro_Conf_Item(
    Maestro_String &_key, 
    Maestro_String &_val
) { 
  key=_key; val=_val;
}

std::ostream& 
operator << (std::ostream &out, Maestro_Conf_Item &item) {
  out << "\t\t" << item.key << " = " << item.val << std::endl;
  return out;
}

std::ostream& 
operator << (std::ostream &out, Maestro_Conf_Argument &arg) {
  out << "\t" << arg.type << " " << arg.name << " {" << std::endl;
  int i;
  for (i = 0; i < arg.items.size(); i++) { out << arg.items[i]; }
  out << "\t}" << std::endl;
  return out;
}

Maestro_Conf_Status 
operator >> (std::istream &sdl, Maestro_Conf_Argument &arg) {
  return (arg << sdl);
}

void
Maestro_Conf_Argument::reset(Maestro_String &_type, Maestro_String &_name) { 
  valid = 0; 
  type = _type;
  name = _name;
  items.clear();
}

void
Maestro_Conf_Argument::add(Maestro_String &key, Maestro_String &val) {
  Maestro_Conf_Item item(key, val);
  items += item;
}


// Read "method-name {" line followed by a list of key/value pairs,
// followed by "}".
Maestro_Conf_Status 
Maestro_Conf_Argument::operator << (std::istream &sdl) {
  char buf[256], bracket[256];

  int ret = EOF;
  if (!sdl.eof()) {
    sdl >> buf;
    name = buf;
  }
  if (!sdl.eof()) {
    ret = 1;
    sdl >> bracket;
  }
  if (ret != EOF) {

    if (strcmp(bracket, "{") == 0) {
      //cout << "\t" << type << " " << name << " {" << std::endl;

      // Read the list of "key = value;" pairs until "}" is encountered.
      char key[256], equal[256], val[256];
      do {
	Maestro_Conf_Item item;

	// If "}" encountered, the argument is complete.
	if (!sdl.eof()) {
	  sdl >> key;

	  if (strcmp(key, "}") == 0) {
	    //cout << "\t}" << std::endl;
	    valid = 1;
	    return MAESTRO_CONF_ARG_ST_OK;
	  }
	} else {
	  //cerr << "unexpected end of file" << std::endl;
	  return MAESTRO_CONF_ARG_ST_SYS_ERR;
	}

	// Otherwise try to complete reading the next item. 
	
	ret = EOF;
	if (!sdl.eof()) {
	  sdl >> equal;
	}
	if (!sdl.eof()) {
	  ret = 1;
	  sdl >> val;
	}
	if (ret != EOF) {

	  if (strcmp(equal, "=") == 0) {

	    // Add this item to the list.
	    item.key = key;
	    item.val = val;
	    items += item;

	    //cout << "\t\t" << key << " = " << val << std::endl;
	  }
	  else { 
	    //cerr << "syntax error" << std::endl;
	    return MAESTRO_CONF_ARG_ST_SYNTAX_ERR;
	  }
	} else {
	  //cerr << "unexpected end of file" << std::endl;
	  return MAESTRO_CONF_ARG_ST_SYS_ERR;
	}
      } while (1);
    } else {
      //cerr << "syntax error" << std::endl;
      return MAESTRO_CONF_ARG_ST_SYNTAX_ERR;
    }
  } else {
    //cerr << "unexpected end of file" << std::endl;
    return MAESTRO_CONF_ARG_ST_SYS_ERR;
  }

  return MAESTRO_CONF_ARG_ST_OK;
}

void
Maestro_Conf::operator << (std::istream &sdl) {
  char buf[256];
  char intf[256], bracket[256];
  Maestro_Conf_Status st;
  
  // Read intf. name line.
  int ret = EOF;
  if (!sdl.eof()) sdl >> intf;
  if (!sdl.eof()) sdl >> buf;
  name = buf;
  if (!sdl.eof()) {
    sdl >> bracket;
    ret = 1;
  }
  if (ret != EOF) {
    if (strcmp(intf, "interface") == 0 &&
	strcmp(bracket, "{") == 0) {
      //cout << "interface " << name << " {" << std::endl;
    } else {
      //cerr << "Syntax error reading head line" << std::endl;
      return;
    }
  }
  else {
    //cerr << "Syntax error reading head line" << std::endl;
    return;
  }
 
  do {
    // Read next token.  It should be one of "method", "orb", or "}".
    if (!sdl.eof()) {
      sdl >> buf;      

      if (strcmp(buf, "method") == 0 ||
	  strcmp(buf, "orb") == 0) {
	// Read the next interface entry (argument).
	// st = read_intf_entry(sdl, buf);
	Maestro_Conf_Argument arg;
	arg.type = buf;
	st = (sdl >> arg);
	if (arg.valid) {
	  arguments += arg;
	}
      }
      else if (strcmp(buf, "}") == 0) {
	//cout << "}" << std::endl;
	valid = 1;
	st = MAESTRO_CONF_ARG_ST_EOF;
      }
      else {
	st = MAESTRO_CONF_ARG_ST_SYNTAX_ERR;
      }
    }
    else {
      st = MAESTRO_CONF_ARG_ST_SYS_ERR;
    }
  } while (st == MAESTRO_CONF_ARG_ST_OK);

  if (st != MAESTRO_CONF_ARG_ST_EOF) {
    //cerr << "Error reading file" << std::endl;
  }    
}

// Generate initialization code corresponding to this configuration.
// Generated code is written to out.
// prefix is written in the beginning of each line.
// confVar is the name of the config. variable, including dereferencing
// symbol (e.g. "conf." or "conf->"). 
void 
Maestro_Conf::generateCode(std::ostream &out, char *prefix, char *confVar) {
  int i, k;
  // out << prefix << "Maestro_Conf conf(\"" << name << "\");" << std::endl;
  out << prefix << "Maestro_Conf_Argument arg;" << std::endl;
  for (i = 0; i < arguments.size(); i++) {
    out << prefix << "arg.reset(\"" << arguments[i].type << "\", \"" 
	 << arguments[i].name << "\");" << std::endl;
    for (k = 0; k < arguments[i].items.size(); k++) {
      out << prefix << "arg.add(\""
	   << arguments[i].items[k].key << "\", \"" 
	   << arguments[i].items[k].val << "\");" << std::endl;
    }
    out << prefix << confVar << "add(arg);" << std::endl;
  }
}

std::ostream& 
operator << (std::ostream &out, Maestro_Conf &intf) {
  out << "interface " << intf.name << " {" << std::endl;
  int i;
  for (i = 0; i < intf.arguments.size(); i++) { out << intf.arguments[i]; }
  out << "}" << std::endl;
  return out;
}

void 
operator >> (std::istream &sdl, Maestro_Conf &intf) { intf << sdl; }
