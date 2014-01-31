// $Header$
// 
// Definitions of IOP + GIOP + IIOP types.
//
// Author:  Alexey Vaysburd, Sept. 1997.

#ifndef __MAESTRO_GIOP_H__
#define __MAESTRO_GIOP_H__

/* jsd: added cond */
#if defined(__GNUC__)
#pragma interface
#endif

#include <avango/ensemble/maestro/Maestro_CORBA.h>

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1681
#pragma set woff 1682
#pragma set woff 3624
#endif

/******************************* IOP ********************************/

typedef Maestro_CORBA_ULong Maestro_IOP_ProfileId;

const Maestro_IOP_ProfileId MAESTRO_IOP_TAG_INTERNET_IOP = 0;


/********************** Maestro_IOP_TaggedProfile *********************/

struct Maestro_IOP_TaggedProfile:
  virtual public Maestro_CORBA_Serializable 
{
  Maestro_IOP_TaggedProfile();

  Maestro_IOP_TaggedProfile(Maestro_IOP_ProfileId profileTag,
			    Maestro_CORBA_OctetSequence &profileData);

  Maestro_IOP_TaggedProfile(Maestro_CORBA_OctetSequence profileData);      

  // Write itself to a CORBA message. 
  virtual void operator>> (Maestro_CORBA_Message &msg);
  
  // Read itself from a CORBA message. 
  virtual void operator<< (Maestro_CORBA_Message &msg);

  Maestro_IOP_ProfileId tag;
  Maestro_CORBA_OctetSequence profile_data;
};


typedef Maestro_CORBA_Sequence(Maestro_IOP_TaggedProfile)
  Maestro_IOP_TaggedProfileList;

const char* const MAESTRO_IOP_IOR_PREFIX = "IOR:";


/********************** Maestro_IOP_IOR *********************/

struct Maestro_IOP_IOR:
  virtual public Maestro_CORBA_Serializable
{
  Maestro_IOP_IOR() {}
  Maestro_IOP_IOR(Maestro_CORBA_String &typeId,
		  Maestro_IOP_TaggedProfileList &profileList);

  // Write itself to a CORBA message. 
  virtual void operator>> (Maestro_CORBA_Message &msg);
  
  // Read itself from a CORBA message. 
  virtual void operator<< (Maestro_CORBA_Message &msg);

  // Convert IOR into a stringified octet-sequence encapsulation.
  virtual void operator>> (Maestro_CORBA_String &repr);

  // Retrieve IOR from its stringified octet-sequence encapsulation.
  virtual void operator<< (Maestro_CORBA_String &repr); 

  Maestro_CORBA_String type_id;
  Maestro_IOP_TaggedProfileList profiles;
};

typedef Maestro_CORBA_ULong Maestro_IOP_ServiceID;


/********************** Maestro_IOP_ServiceContext ********************/

struct Maestro_IOP_ServiceContext: 
  virtual public Maestro_CORBA_Serializable 
{
  Maestro_IOP_ServiceContext() {}
  Maestro_IOP_ServiceContext(Maestro_IOP_ServiceContext &ctx);
  Maestro_IOP_ServiceContext(Maestro_IOP_ServiceID contextId,
			     Maestro_CORBA_OctetSequence &contextData);

  virtual void operator>> (Maestro_CORBA_Message &msg);
  virtual void operator<< (Maestro_CORBA_Message &msg);

  Maestro_IOP_ServiceID context_id;
  Maestro_CORBA_OctetSequence context_data;
};

typedef Maestro_CORBA_Sequence(Maestro_IOP_ServiceContext)
  Maestro_IOP_ServiceContextList;


/******************************* GIOP ********************************/

typedef enum {
  MAESTRO_GIOP_MSG_TYPE_REQUEST,
  MAESTRO_GIOP_MSG_TYPE_REPLY,
  MAESTRO_GIOP_MSG_TYPE_CANCEL_REQUEST,
  MAESTRO_GIOP_MSG_TYPE_LOCATE_REQUEST,
  MAESTRO_GIOP_MSG_TYPE_LOCATE_REPLY,
  MAESTRO_GIOP_MSG_TYPE_CLOSE_CONNECTION,
  MAESTRO_GIOP_MSG_TYPE_MESSAGE_ERROR
} Maestro_GIOP_MsgType;

const Maestro_CORBA_Char* const MAESTRO_GIOP_MAGIC = (unsigned char*) "GIOP";

const Maestro_CORBA_Char MAESTRO_GIOP_MAJOR = 1;
const Maestro_CORBA_Char MAESTRO_GIOP_MINOR = 0;


/********************** Maestro_GIOP_Version *******************/
 
struct Maestro_GIOP_Version:
  virtual public Maestro_CORBA_Serializable 
{
  Maestro_GIOP_Version();
  Maestro_GIOP_Version(Maestro_CORBA_Char maj, Maestro_CORBA_Char min);

  virtual void operator>> (Maestro_CORBA_Message &msg);
  virtual void operator<< (Maestro_CORBA_Message &msg); 

  Maestro_CORBA_Char major;
  Maestro_CORBA_Char minor;
};


/********************** Maestro_GIOP_MessageHeader *********************/

struct Maestro_GIOP_MessageHeader: 
  virtual public Maestro_CORBA_Serializable 
{
  Maestro_GIOP_MessageHeader();
  
  Maestro_GIOP_MessageHeader(Maestro_CORBA_Octet msgType,
			     Maestro_CORBA_ULong msgSize);

  // Write GIOP message header to a CORBA message. 
  virtual void operator>> (Maestro_CORBA_Message &msg);
  
  // Read itself from a CORBA message. 
  virtual void operator<< (Maestro_CORBA_Message &msg);
  
  Maestro_CORBA_Char magic[4];
  Maestro_GIOP_Version GIOP_version;
  Maestro_CORBA_Boolean byte_order;
  Maestro_CORBA_Octet message_type;
  Maestro_CORBA_ULong message_size;
};

typedef Maestro_CORBA_OctetSequence Maestro_GIOP_Principal;


/********************** Maestro_GIOP_RequestHeader *********************/

struct Maestro_GIOP_RequestHeader: 
  virtual public Maestro_CORBA_Serializable 
{
  Maestro_GIOP_RequestHeader() {}

  Maestro_GIOP_RequestHeader(Maestro_CORBA_ULong requestId,
			     Maestro_CORBA_Boolean responseExpected,
			     Maestro_CORBA_OctetSequence &objectKey,
			     Maestro_CORBA_String &operationName);

  virtual void operator>> (Maestro_CORBA_Message &msg); 
  virtual void operator<< (Maestro_CORBA_Message &msg);

  Maestro_IOP_ServiceContextList service_context;
  Maestro_CORBA_ULong            request_id;
  Maestro_CORBA_Boolean          response_expected;
  Maestro_CORBA_OctetSequence    object_key;
  Maestro_CORBA_String           operation;
  Maestro_GIOP_Principal         requesting_principal;
};

typedef enum {
  MAESTRO_GIOP_REPLY_STATUS_NO_EXCEPTION,
  MAESTRO_GIOP_REPLY_STATUS_USER_EXCEPTION,
  MAESTRO_GIOP_REPLY_STATUS_SYSTEM_EXCEPTION,
  MAESTRO_GIOP_REPLY_STATUS_LOCATION_FORWARD
} Maestro_GIOP_ReplyStatusType;


/********************** Maestro_GIOP_ReplyHeader ********************/

struct Maestro_GIOP_ReplyHeader: 
  virtual public Maestro_CORBA_Serializable 
{
  Maestro_GIOP_ReplyHeader() {}

  Maestro_GIOP_ReplyHeader(Maestro_CORBA_ULong requestId,
			   Maestro_GIOP_ReplyStatusType replyStatus);

  virtual void operator>> (Maestro_CORBA_Message &msg);
  virtual void operator<< (Maestro_CORBA_Message &msg); 

  Maestro_IOP_ServiceContextList service_context;
  Maestro_CORBA_ULong            request_id;
  Maestro_GIOP_ReplyStatusType   reply_status;
};


/********************** Maestro_GIOP_CancelRequestHeader *********************/

struct Maestro_GIOP_CancelRequestHeader:
  virtual public Maestro_CORBA_Serializable 
{
  Maestro_GIOP_CancelRequestHeader(Maestro_CORBA_ULong requestId = 0); 

  virtual void operator>> (Maestro_CORBA_Message &msg); 
  virtual void operator<< (Maestro_CORBA_Message &msg); 

  Maestro_CORBA_ULong            request_id;
};


/********************** Maestro_GIOP_LocateRequestHeader ******************/

struct Maestro_GIOP_LocateRequestHeader:
  virtual public Maestro_CORBA_Serializable 
{
  Maestro_GIOP_LocateRequestHeader() {}
  Maestro_GIOP_LocateRequestHeader(Maestro_CORBA_ULong requestId,
				   Maestro_CORBA_OctetSequence &objectKey);

  virtual void operator>> (Maestro_CORBA_Message &msg); 
  virtual void operator<< (Maestro_CORBA_Message &msg); 

  Maestro_CORBA_ULong            request_id;
  Maestro_CORBA_OctetSequence    object_key;
};

typedef enum {
  MAESTRO_GIOP_LOCATE_STATUS_UNKNOWN_OBJECT,
  MAESTRO_GIOP_LOCATE_STATUS_OBJECT_HERE,
  MAESTRO_GIOP_LOCATE_STATUS_OBJECT_FORWARD
} Maestro_GIOP_LocateStatusType;


/****************** Maestro_GIOP_LocateReplyHeader *****************/

struct Maestro_GIOP_LocateReplyHeader:
  virtual public Maestro_CORBA_Serializable 
{
  Maestro_GIOP_LocateReplyHeader() {}
  Maestro_GIOP_LocateReplyHeader(Maestro_CORBA_ULong requestId,
				 Maestro_GIOP_LocateStatusType locateStatus);

  virtual void operator>> (Maestro_CORBA_Message &msg); 
  virtual void operator<< (Maestro_CORBA_Message &msg); 

  Maestro_CORBA_ULong            request_id;
  Maestro_GIOP_LocateStatusType  locate_status;
};


/******************************* IIOP ********************************/

const Maestro_CORBA_Char MAESTRO_IIOP_MAJOR = 1;
const Maestro_CORBA_Char MAESTRO_IIOP_MINOR = 0;


/********************** Maestro_IIOP_ProfileBody ************************/

struct Maestro_IIOP_ProfileBody:
   virtual public Maestro_CORBA_Serializable 
{
  Maestro_IIOP_ProfileBody(); 
  Maestro_IIOP_ProfileBody(Maestro_CORBA_String &hostName,
			   Maestro_CORBA_UShort portNumber,
			   Maestro_CORBA_OctetSequence &objectKey);

  virtual void operator>> (Maestro_CORBA_Message &msg); 
  virtual void operator<< (Maestro_CORBA_Message &msg); 

  Maestro_GIOP_Version iiop_version;
  Maestro_CORBA_String host;
  Maestro_CORBA_UShort port;
  Maestro_CORBA_OctetSequence object_key;
};

struct Maestro_IIOP_TaggedProfile:
  Maestro_IOP_TaggedProfile
{
  
  Maestro_IIOP_TaggedProfile() { tag = MAESTRO_IOP_TAG_INTERNET_IOP; }

  Maestro_IIOP_TaggedProfile(Maestro_IIOP_ProfileBody &profileBody);

  Maestro_IIOP_TaggedProfile(Maestro_CORBA_String &hostName,
			     Maestro_CORBA_UShort portNumber,
			     Maestro_CORBA_OctetSequence &objectKey);

  Maestro_IIOP_TaggedProfile(Maestro_CORBA_String &hostName,
			     Maestro_CORBA_UShort portNumber,
			     Maestro_CORBA_String &objectKey);
};
  

struct Maestro_IIOP_IOR: Maestro_IOP_IOR {
  Maestro_IIOP_IOR() {}

  Maestro_IIOP_IOR(Maestro_CORBA_String &object_key,
		   Maestro_CORBA_String &host,
		   Maestro_CORBA_UShort port,
		   Maestro_CORBA_String &interface_name);


  Maestro_IIOP_IOR(Maestro_CORBA_OctetSequence &object_key,
		   Maestro_CORBA_String &host,
		   Maestro_CORBA_UShort port,
		   Maestro_CORBA_String &interface_name);
};

/* jsd: added */
#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1681
#pragma reset woff 1682
#pragma reset woff 3624
#endif

#endif // __MAESTRO_GIOP_H__
