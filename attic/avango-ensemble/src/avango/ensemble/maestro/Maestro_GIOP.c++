// $Header$
// 
// Definitions of IOP + GIOP + IIOP types.
//
// Author:  Alexey Vaysburd, Sept. 1997.

#pragma implementation
#include "avango/ensemble/maestro/Maestro_GIOP.h"

#include <iomanip>
#include <iostream>
#include <sstream>

/******************************* IOP ********************************/

Maestro_IOP_TaggedProfile::Maestro_IOP_TaggedProfile() {
  tag = MAESTRO_IOP_TAG_INTERNET_IOP;
}

Maestro_IOP_TaggedProfile::Maestro_IOP_TaggedProfile(
    Maestro_IOP_ProfileId profileTag,
    Maestro_CORBA_OctetSequence &profileData) 
{
  tag = profileTag;
  profile_data = profileData;
}

Maestro_IOP_TaggedProfile::Maestro_IOP_TaggedProfile(
    Maestro_CORBA_OctetSequence profileData)
{
  tag = MAESTRO_IOP_TAG_INTERNET_IOP;
  profile_data = profileData;
}

// Write itself to a CORBA message. 
void 
Maestro_IOP_TaggedProfile::operator>> (Maestro_CORBA_Message &msg) {
  msg << tag;
  msg << profile_data;
} 

// Read itself from a CORBA message. 
void 
Maestro_IOP_TaggedProfile::operator<< (Maestro_CORBA_Message &msg) {
  msg >> tag;
  msg >> profile_data;
}

/********************** Maestro_IOP_IOR *********************/


Maestro_IOP_IOR::Maestro_IOP_IOR(
    Maestro_CORBA_String &typeId,
    Maestro_IOP_TaggedProfileList &profileList
) {
  type_id = typeId;
  profiles = profileList;
}

// Write itself to a CORBA message. 
void 
Maestro_IOP_IOR::operator>> (Maestro_CORBA_Message &msg) {
  msg << type_id;
  msg << profiles;
} 
  
// Read itself from a CORBA message. 
void 
Maestro_IOP_IOR::operator<< (Maestro_CORBA_Message &msg) {
  msg >> type_id;
  msg >> profiles;
}

// Convert IOR into a stringified octet-sequence encapsulation.
void 
Maestro_IOP_IOR::operator>> (Maestro_CORBA_String &repr) {
  Maestro_CORBA_OctetSequence encaps;
  writeTo(encaps);
  
  int size = encaps.size() * 2 + strlen(MAESTRO_IOP_IOR_PREFIX);
  char *buf = new char[size + 1];
  buf[size] = 0;
  
  std::ostringstream ost(buf);  
  ost << MAESTRO_IOP_IOR_PREFIX;
  unsigned char *p;
  int i;
  for (p = encaps.data(), i = 0; i < encaps.size(); i++, p++) {
    ost << std::setw(2) << std::setfill('0') << std::hex << (int) *p;
  }
  
  repr = buf;
}

// Retrieve IOR from its stringified octet-sequence encapsulation.
void 
Maestro_IOP_IOR::operator<< (Maestro_CORBA_String &repr) {
  Maestro_CORBA_OctetSequence encaps;
  
  int prefSize = strlen(MAESTRO_IOP_IOR_PREFIX);
  if (strncmp(repr.s, MAESTRO_IOP_IOR_PREFIX, prefSize) != 0) {
    std::cerr << "WARNING: Maestro_IOP_IOR:  bad prefix in stringified encapsulation\n";
    return;
  }
  
  int i, val, size = (repr.size - prefSize) / 2;
  encaps.size(size);
  Maestro_CORBA_Octet *data = encaps.data();
  
  for (i = 0; i < size; i++) {
    sscanf(&repr.s[prefSize + i*2], "%2x", &val);
    data[i] = val;
  }
  
  readFrom(encaps);    
}


/********************** Maestro_IOP_ServiceContext ********************/

Maestro_IOP_ServiceContext::Maestro_IOP_ServiceContext(
    Maestro_IOP_ServiceContext &ctx
){
  context_id = ctx.context_id;
  context_data = ctx.context_data;
}

Maestro_IOP_ServiceContext::Maestro_IOP_ServiceContext(
    Maestro_IOP_ServiceID contextId,
    Maestro_CORBA_OctetSequence &contextData) 
{
  context_id = contextId;
  context_data = contextData;
}

void 
Maestro_IOP_ServiceContext::operator>> (Maestro_CORBA_Message &msg) {
  msg << context_id;
  msg << context_data;
}
  
void 
Maestro_IOP_ServiceContext::operator<< (Maestro_CORBA_Message &msg) {
  msg >> context_id;
  msg >> context_data;
}


/********************** Maestro_GIOP_Version *******************/
 

Maestro_GIOP_Version::Maestro_GIOP_Version() {
  major = MAESTRO_GIOP_MAJOR;
  minor = MAESTRO_GIOP_MINOR;
}

Maestro_GIOP_Version::Maestro_GIOP_Version(
    Maestro_CORBA_Char maj, 
    Maestro_CORBA_Char min)
{
  major = maj;
  minor = min;
}

void 
Maestro_GIOP_Version::operator>> (Maestro_CORBA_Message &msg) {
  msg << major;
  msg << minor;
}

void 
Maestro_GIOP_Version::operator<< (Maestro_CORBA_Message &msg) {
  msg >> major;
  msg >> minor;
}


/********************** Maestro_GIOP_MessageHeader *********************/

Maestro_GIOP_MessageHeader::Maestro_GIOP_MessageHeader() {
  memcpy(magic, MAESTRO_GIOP_MAGIC, 4);
  byte_order = Maestro_CORBA_Message::localByteOrder();
}

Maestro_GIOP_MessageHeader::Maestro_GIOP_MessageHeader(
    Maestro_CORBA_Octet msgType,
    Maestro_CORBA_ULong msgSize) 
{
  memcpy(magic, MAESTRO_GIOP_MAGIC, 4);
  byte_order = Maestro_CORBA_Message::localByteOrder();
  message_type = msgType;
  message_size = msgSize;
}

// Write GIOP message header to a CORBA message. 
void 
Maestro_GIOP_MessageHeader::operator>> (Maestro_CORBA_Message &msg) {
  msg.align(4);
  msg.write(magic, 4);
  msg << GIOP_version;
  msg << byte_order;
  msg << message_type;
  msg << message_size;
} 
  
// Read itself from a CORBA message. 
void 
Maestro_GIOP_MessageHeader::operator<< (Maestro_CORBA_Message &msg) {
  msg.align(4);
  msg.read(magic, 4);
  msg >> GIOP_version;
  msg >> byte_order;
  
  msg.msgByteOrder(byte_order);
  msg >> message_type;
  msg >> message_size;
}
  

/********************** Maestro_GIOP_RequestHeader *********************/

Maestro_GIOP_RequestHeader::Maestro_GIOP_RequestHeader(
    Maestro_CORBA_ULong requestId,
    Maestro_CORBA_Boolean responseExpected,
    Maestro_CORBA_OctetSequence &objectKey,
    Maestro_CORBA_String &operationName)
{
  request_id = requestId;
  response_expected = responseExpected;
  object_key = objectKey;
  operation = operationName;
}

void 
Maestro_GIOP_RequestHeader::operator>> (Maestro_CORBA_Message &msg) {
  msg << service_context;
  msg << request_id;
  msg << response_expected;
  msg << object_key;
  msg << operation;
  msg << requesting_principal;
}

void 
Maestro_GIOP_RequestHeader::operator<< (Maestro_CORBA_Message &msg) {
  msg >> service_context;
  msg >> request_id;
  msg >> response_expected;
  msg >> object_key;
  msg >> operation;
  msg >> requesting_principal;
}


/********************** Maestro_GIOP_ReplyHeader ********************/

Maestro_GIOP_ReplyHeader::Maestro_GIOP_ReplyHeader(
    Maestro_CORBA_ULong requestId,
    Maestro_GIOP_ReplyStatusType replyStatus) 
{
  request_id = requestId;
  reply_status = replyStatus;
}

void 
Maestro_GIOP_ReplyHeader::operator>> (Maestro_CORBA_Message &msg) {
  msg << service_context;
  msg << request_id;
  msg << (Maestro_CORBA_ULong) reply_status;
}

void 
Maestro_GIOP_ReplyHeader::operator<< (Maestro_CORBA_Message &msg) {
  msg >> service_context;
  msg >> request_id;
  Maestro_CORBA_ULong tmp;
  msg >> tmp;
  reply_status = (Maestro_GIOP_ReplyStatusType) tmp;
}


/******************** Maestro_GIOP_CancelRequestHeader *********************/

Maestro_GIOP_CancelRequestHeader::Maestro_GIOP_CancelRequestHeader(
    Maestro_CORBA_ULong requestId /* = 0 */
) {
  request_id = requestId;
}

void 
Maestro_GIOP_CancelRequestHeader::operator>> (Maestro_CORBA_Message &msg) {
  msg << request_id;
}

void 
Maestro_GIOP_CancelRequestHeader::operator<< (Maestro_CORBA_Message &msg) {
  msg >> request_id;
}


/********************* Maestro_GIOP_LocateRequestHeader ******************/

Maestro_GIOP_LocateRequestHeader::Maestro_GIOP_LocateRequestHeader(
    Maestro_CORBA_ULong requestId,
    Maestro_CORBA_OctetSequence &objectKey)
{
  request_id = requestId;
  object_key = objectKey;
}


void 
Maestro_GIOP_LocateRequestHeader::operator>> (Maestro_CORBA_Message &msg) {
  msg << request_id;
  msg << object_key;
}

void
Maestro_GIOP_LocateRequestHeader::operator<< (Maestro_CORBA_Message &msg) {
  msg >> request_id;
  msg >> object_key;
}


/****************** Maestro_GIOP_LocateReplyHeader *****************/

Maestro_GIOP_LocateReplyHeader::Maestro_GIOP_LocateReplyHeader(
    Maestro_CORBA_ULong requestId,
    Maestro_GIOP_LocateStatusType locateStatus)
{
  request_id = requestId;
  locate_status = locateStatus;
}

void 
Maestro_GIOP_LocateReplyHeader::operator>> (Maestro_CORBA_Message &msg) {
  msg << request_id;
  msg << (Maestro_CORBA_ULong) locate_status;
}

void 
Maestro_GIOP_LocateReplyHeader::operator<< (Maestro_CORBA_Message &msg) {
  msg >> request_id;
  Maestro_CORBA_ULong tmp;
  msg >> tmp;
  locate_status = (Maestro_GIOP_LocateStatusType) tmp;
}


/********************** Maestro_IIOP_ProfileBody ************************/

Maestro_IIOP_ProfileBody::Maestro_IIOP_ProfileBody() {
  iiop_version.major = MAESTRO_IIOP_MAJOR;
  iiop_version.minor = MAESTRO_IIOP_MINOR;
}

Maestro_IIOP_ProfileBody::Maestro_IIOP_ProfileBody(
    Maestro_CORBA_String &hostName,
    Maestro_CORBA_UShort portNumber,
    Maestro_CORBA_OctetSequence &objectKey)
{
  iiop_version.major = MAESTRO_IIOP_MAJOR;
  iiop_version.minor = MAESTRO_IIOP_MINOR;
  host = hostName;
  port = portNumber;
  object_key = objectKey;
}

void 
Maestro_IIOP_ProfileBody::operator>> (Maestro_CORBA_Message &msg) {
  msg << iiop_version;
  msg << host;
  msg << port;
  msg << object_key;
}

void 
Maestro_IIOP_ProfileBody::operator<< (Maestro_CORBA_Message &msg) {
  msg >> iiop_version;
  msg >> host;
  msg >> port;
  msg >> object_key;
}

Maestro_IIOP_TaggedProfile::Maestro_IIOP_TaggedProfile(
    Maestro_IIOP_ProfileBody &profileBody
){
  tag = MAESTRO_IOP_TAG_INTERNET_IOP;
  profile_data << profileBody;
}

Maestro_IIOP_TaggedProfile::Maestro_IIOP_TaggedProfile(
    Maestro_CORBA_String &hostName,
    Maestro_CORBA_UShort portNumber,
    Maestro_CORBA_OctetSequence &objectKey
){
  tag = MAESTRO_IOP_TAG_INTERNET_IOP;
  Maestro_IIOP_ProfileBody pbody(hostName, portNumber, objectKey);
  profile_data << pbody;
}

Maestro_IIOP_TaggedProfile::Maestro_IIOP_TaggedProfile(
    Maestro_CORBA_String &hostName,
    Maestro_CORBA_UShort portNumber,
    Maestro_CORBA_String &objectKey
){
  Maestro_CORBA_OctetSequence key((unsigned char*) objectKey.s,
				  objectKey.size);
  Maestro_IIOP_ProfileBody pbody(hostName, portNumber, key);
  tag = MAESTRO_IOP_TAG_INTERNET_IOP;
  profile_data << pbody;
}

Maestro_IIOP_IOR::Maestro_IIOP_IOR(
    Maestro_CORBA_String &object_key,
    Maestro_CORBA_String &host,
    Maestro_CORBA_UShort port,
    Maestro_CORBA_String &interface_name
){
  Maestro_IIOP_TaggedProfile profile(host, port, object_key);
  Maestro_IOP_TaggedProfileList tmp(profile);
  type_id = interface_name;
  profiles = tmp;
}

Maestro_IIOP_IOR::Maestro_IIOP_IOR(
    Maestro_CORBA_OctetSequence &object_key,
    Maestro_CORBA_String &host,
    Maestro_CORBA_UShort port,
    Maestro_CORBA_String &interface_name
){
  Maestro_IIOP_TaggedProfile profile(host, port, object_key);
  Maestro_IOP_TaggedProfileList tmp(profile);
  type_id = interface_name;
  profiles = tmp;
}
