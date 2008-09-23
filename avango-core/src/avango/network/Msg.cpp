// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include "avango/Msg.h"

#include <avango/Assert.h>
#include <avango/Distributed.h>
#include <avango/Field.h>
#include <avango/NetID.h>
#include <avango/NetNode.h>

#include <avango/Config.h>

namespace
{
  av::logging::Logger& logger(av::logging::Logger::getLogger("av::Msg"));
}

av::Msg::Msg()
  : mMsgType(relative),
    mMsgBuffer(new MessageBuffer())
{
}

av::Msg::Msg(MsgType newMsgType, boost::shared_ptr<MessageBuffer> newBuffer)
  : mMsgType(newMsgType),
    mMsgBuffer(newBuffer)
{
}

av::Msg::~Msg()
{
}

size_t
av::Msg::getSize()
{
  return mMsgBuffer->size();
}

void
av::Msg::resize(size_t newSize)
{
  mMsgBuffer->resize(newSize);
}



unsigned char*
av::Msg::getBuffer()
{
  return &(mMsgBuffer->operator[](0));
}

av::NetNode*
av::Msg::getNetNode() const
{
  return mNetNode;
}

void
av::Msg::setNetNode(av::NetNode* netNode)
{
  mNetNode = netNode;
}


av::Msg
av::Msg::clone() const
{
  boost::shared_ptr<MessageBuffer> copied_buffer(new MessageBuffer(*mMsgBuffer));
  return Msg(mMsgType, copied_buffer);
}

// void*
void
av::Msg::push(const void* buffer, size_t bytes)
{
  const unsigned char* bufferMem = static_cast<const unsigned char*>(buffer);
  mMsgBuffer->insert(mMsgBuffer->end(), bufferMem, bufferMem+bytes);

#ifdef AVANGO_DEBUG
  logger.trace() << "push: (void*) [" << bytes << " byte" << (1 == bytes ? "" : "s") << "]";
#endif
}

void
av::Msg::pop(void* buffer, size_t bytes)
{
  unsigned char* bufferMem = static_cast<unsigned char*>(buffer);

  AV_ASSERT(mMsgBuffer->size() >= bytes);
  std::copy(mMsgBuffer->end() - bytes, mMsgBuffer->end(), bufferMem);

  mMsgBuffer->resize(mMsgBuffer->size() - bytes);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (void*) [" << bytes << " byte" << (1 == bytes ? "" : "s") << "]";
#endif
}

// bool
void
av::av_pushMsg(av::Msg& msg, const bool& buf)
{
  int32_t b = (buf ? 1 : 0);
  av_pushMsg(msg, b);

#ifdef AVANGO_DEBUG
  logger.trace() << "push: (bool) [" << (buf ? "true" : "false") << "]";
#endif
}

void
av::av_popMsg(av::Msg& msg, bool& buf)
{
  int32_t b;
  av_popMsg(msg, b);
  buf = (b == 0 ? false : true);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (bool) [" << (buf ? "true" : "false") << "]";
#endif
}

// int32_t
void
av::av_pushMsg(av::Msg& msg, const int32_t& buf)
{
  int32_t b;
  XDRHandle xdr(&b, sizeof(b), XDR_ENCODE);

  xdr_int(xdr.getXDR(), (int32_t*) &buf);

  msg.push(&b, sizeof(b));

#ifdef AVANGO_DEBUG
  logger.trace() << "push: (int32_t) [" << buf << "]";
#endif
}

void
av::av_popMsg(av::Msg& msg, int32_t& buf)
{
  int32_t b;
  XDRHandle xdr(&b, sizeof(b), XDR_DECODE);

  msg.pop((void*) &b, sizeof(b));

  xdr_int(xdr.getXDR(), (int32_t*) &buf);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (int32_t) [" << buf << "]";
#endif
}

// std::vector<int32_t>
void
av::av_pushMsg(av::Msg& msg, const std::vector<int32_t>& buf)
{
  std::vector<int32_t> tmpBuf(buf.size());
  XDRHandle xdr(&tmpBuf[0], sizeof(int32_t)*buf.size(), XDR_ENCODE) ;

  int32_t* arrayStart = const_cast<int32_t*>(&buf[0]); // ugly, but necessary

  xdr_vector(xdr.getXDR(), reinterpret_cast<char*>(arrayStart), buf.size(), sizeof(int32_t),
             (xdrproc_t)xdr_int);

  msg.push(&tmpBuf[0], sizeof(int32_t)*buf.size());
  av_pushMsg(msg, buf.size());
}

void
av::av_popMsg(av::Msg& msg, std::vector<int32_t>& buf)
{
  unsigned int arraySize;
  av_popMsg(msg, arraySize);

  buf.resize(arraySize);

  std::vector<int32_t> tmpVec(arraySize);
  XDRHandle xdr(&tmpVec[0],  sizeof(int32_t) * arraySize, XDR_DECODE);

  msg.pop(&tmpVec[0], sizeof(int32_t)*arraySize);

  xdr_vector(xdr.getXDR(), reinterpret_cast<char*>(&buf[0]), buf.size(), sizeof(int32_t),
             (xdrproc_t)xdr_int);
}


// uint32_t
void
av::av_pushMsg(av::Msg& msg, const uint32_t& buf)
{
  uint32_t b;
  XDRHandle    xdr(&b, sizeof(b), XDR_ENCODE);

  xdr_u_int(xdr.getXDR(), (uint32_t*) &buf);

  msg.push(&b, sizeof(b));

#ifdef AVANGO_DEBUG
  logger.trace() << "push: (uint32_t) [" << buf << "]";
#endif
}

void
av::av_popMsg(av::Msg& msg, uint32_t& buf)
{
  uint32_t b;
  XDRHandle    xdr(&b, sizeof(b), XDR_DECODE);

  msg.pop(&b, sizeof(b));

  xdr_u_int(xdr.getXDR(), &buf);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (uint32_t) [" << buf << "]";
#endif
}

// std::vector<uint32_t>
void
av::av_pushMsg(av::Msg& msg, const std::vector<uint32_t>& buf)
{
  std::vector<uint32_t> tmpVec(buf.size());
  XDRHandle   xdr(&tmpVec[0], sizeof(uint32_t)*buf.size(), XDR_ENCODE);

  uint32_t* arrayStart = const_cast<uint32_t*>(&buf[0]); // ugly, but necessary

  xdr_vector(xdr.getXDR(), reinterpret_cast<char*>(arrayStart), buf.size(), sizeof(uint32_t),
             (xdrproc_t)xdr_u_int);

  msg.push(&tmpVec[0], sizeof(uint32_t)*buf.size());
  av_pushMsg(msg, buf.size());
}

void
av::av_popMsg(av::Msg& msg, std::vector<uint32_t>& buf)
{
  unsigned int arraySize;
  av_popMsg(msg, arraySize);

  buf.resize(arraySize);

  std::vector<uint32_t> tmpVec(arraySize);

  XDRHandle xdr(&tmpVec[0], sizeof(uint32_t)*arraySize, XDR_DECODE);
  msg.pop(&tmpVec[0], sizeof(uint32_t)*arraySize);

  xdr_vector(xdr.getXDR(), reinterpret_cast<char*>(&buf[0]), buf.size(), sizeof(uint32_t),
             (xdrproc_t)xdr_u_int);
}


// int64_t
void
av::av_pushMsg(av::Msg& msg, const int64_t& buf)
{
  int64_t b;
  XDRHandle   xdr(&b, sizeof(b), XDR_ENCODE);

  xdr_longlong_t(xdr.getXDR(), (int64_t*) &buf);

  msg.push(&b, sizeof(b));

#ifdef AVANGO_DEBUG
  logger.trace() << "push: (int64_t) [" << buf << "]";
#endif
}

void
av::av_popMsg(av::Msg& msg, int64_t& buf)
{
  int64_t b;
  XDRHandle   xdr(&b, sizeof(b), XDR_DECODE);

  msg.pop(&b, sizeof(b));

  xdr_longlong_t(xdr.getXDR(), (int64_t*) &buf);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (int64_t) [" << buf << "]";
#endif
}

// std::vector<int64_t>
void
av::av_pushMsg(av::Msg& msg, const std::vector<int64_t>& buf)
{
  std::vector<int64_t> tmpVec(buf.size());
  XDRHandle xdr(&tmpVec[0], sizeof(int64_t)*buf.size(), XDR_ENCODE);

  int64_t* arrayStart = const_cast<int64_t*>(&buf[0]); // ugly, but necessary

  xdr_vector(xdr.getXDR(), reinterpret_cast<char*>(arrayStart), buf.size(), sizeof(int64_t),
             (xdrproc_t)xdr_longlong_t);

  msg.push(&tmpVec[0], sizeof(int64_t)*buf.size());
  av_pushMsg(msg, buf.size());
}

void
av::av_popMsg(av::Msg& msg, std::vector<int64_t>& buf)
{
  unsigned int arraySize;
  av_popMsg(msg, arraySize);

  buf.resize(arraySize);

  std::vector<int64_t> tmpVec(arraySize);
  XDRHandle xdr(&tmpVec[0], sizeof(int64_t)*arraySize, XDR_DECODE);

  msg.pop(&tmpVec[0], sizeof(int64_t)*arraySize);

  xdr_vector(xdr.getXDR(), reinterpret_cast<char*>(&buf[0]), buf.size(), sizeof(int64_t),
             (xdrproc_t)xdr_longlong_t);
}


// uint64_t
void
av::av_pushMsg(av::Msg& msg, const uint64_t& buf)
{
  uint64_t b;
  XDRHandle    xdr(&b, sizeof(b), XDR_ENCODE);

  xdr_u_longlong_t(xdr.getXDR(), (uint64_t*) &buf);

  msg.push(&b, sizeof(b));

#ifdef AVANGO_DEBUG
  logger.trace() << "push: (uint64_t) [" << buf << "]";
#endif
}

void
av::av_popMsg(av::Msg& msg, uint64_t& buf)
{
  uint64_t b;
  XDRHandle    xdr(&b, sizeof(b), XDR_DECODE);

  msg.pop(&b, sizeof(b));

  xdr_u_longlong_t(xdr.getXDR(), (uint64_t*) &buf);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (uint64_t) [" << buf << "]";
#endif
}

// std::vector<uint64_t>
void
av::av_pushMsg(av::Msg& msg, const std::vector<uint64_t>& buf)
{
  std::vector<uint64_t> tmpVec(buf.size());

  XDRHandle xdr(&tmpVec[0], sizeof(uint64_t)*buf.size(), XDR_ENCODE);

  uint64_t* arrayStart = const_cast<uint64_t*>(&buf[0]); // ugly, but necessary

  xdr_vector(xdr.getXDR(), reinterpret_cast<char*>(arrayStart), buf.size(), sizeof(uint64_t),
             (xdrproc_t)xdr_u_longlong_t);

  msg.push(&tmpVec[0], sizeof(uint64_t)*buf.size());
  av_pushMsg(msg, buf.size());
}

void
av::av_popMsg(av::Msg& msg, std::vector<uint64_t>& buf)
{
  unsigned int arraySize;
  av_popMsg(msg, arraySize);

  buf.resize(arraySize);

  std::vector<uint64_t> tmpVec(arraySize);

  XDRHandle xdr(&tmpVec[0], sizeof(uint64_t)*arraySize, XDR_DECODE);
  msg.pop(&tmpVec[0], sizeof(uint64_t)*arraySize);

  xdr_vector(xdr.getXDR(), reinterpret_cast<char*>(&buf[0]), buf.size(), sizeof(uint64_t),
             (xdrproc_t)xdr_u_longlong_t);
}


// float
void
av::av_pushMsg(av::Msg& msg, const float& buf)
{
  float b;
  XDRHandle   xdr(&b, sizeof(b),  XDR_ENCODE);

  xdr_float(xdr.getXDR(), (float*) &buf);

  msg.push(&b, sizeof(b));

#ifdef AVANGO_DEBUG
  logger.trace() << "push: (float) [" << buf << "]";
#endif
}

void
av::av_popMsg(av::Msg& msg, float& buf)
{
  float b;
  XDRHandle xdr(&b, sizeof(b), XDR_DECODE);

  msg.pop(&b, sizeof(b));

  xdr_float(xdr.getXDR(), &buf);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (float) [" << buf << "]";
#endif
}

// std::vector<float>
void
av::av_pushMsg(av::Msg& msg, const std::vector<float>& buf)
{
  std::vector<float> tmpVec(buf.size());

  XDRHandle xdr(&tmpVec[0], sizeof(float)*buf.size(), XDR_ENCODE);

  float* arrayStart = const_cast<float*>(&buf[0]); // ugly, but necessary

  xdr_vector(xdr.getXDR(), (char*)arrayStart, buf.size(), sizeof(float), (xdrproc_t)xdr_float);

  msg.push(&tmpVec[0], sizeof(float)*buf.size());
  av_pushMsg(msg, buf.size());

#ifdef AVANGO_DEBUG
  logger.trace() << "push: (vector<float>) [" << buf.size() << "]";
#endif
}

void
av::av_popMsg(av::Msg& msg, std::vector<float>& buf)
{
  unsigned int arraySize;
  av_popMsg(msg, arraySize);

  buf.resize(arraySize);

  std::vector<float> tmpVec(arraySize);

  XDRHandle xdr(&tmpVec[0], sizeof(float)*arraySize, XDR_DECODE);

  msg.pop(&tmpVec[0], sizeof(float)*arraySize);

  xdr_vector(xdr.getXDR(), (char*)&buf[0], buf.size(), sizeof(float), (xdrproc_t)xdr_float);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (vector<float>) [" << buf.size() << "]";
#endif
}


// double
void
av::av_pushMsg(av::Msg& msg, const double& buf)
{
  double b;
  XDRHandle    xdr(&b, sizeof(b), XDR_ENCODE);

  xdr_double(xdr.getXDR(), (double*)&buf);

  msg.push(&b, sizeof(b));

#ifdef AVANGO_DEBUG
  logger.trace() << "push: (double) [" << buf << "]";
#endif
}

void
av::av_popMsg(av::Msg& msg, double& buf)
{
  double b;
  XDRHandle xdr(&b, sizeof(b), XDR_DECODE);

  msg.pop(&b, sizeof(b));

  xdr_double(xdr.getXDR(), &buf);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (double) [" << buf << "]";
#endif
}

// std::vector<double>
void
av::av_pushMsg(av::Msg& msg, const std::vector<double>& buf)
{
  std::vector<double> tmpVec(buf.size());

  XDRHandle xdr(&tmpVec[0], sizeof(double)*buf.size(), XDR_ENCODE);

  double* arrayStart = const_cast<double*>(&buf[0]); // ugly, but necessary

  xdr_vector(xdr.getXDR(), reinterpret_cast<char*>(arrayStart), buf.size(), sizeof(double),
             (xdrproc_t)xdr_double);

  msg.push(&tmpVec[0], sizeof(double)*buf.size());
  av_pushMsg(msg, buf.size());

#ifdef AVANGO_DEBUG
  logger.trace() << "push: (vector<double>) [" << buf.size() << "]";
#endif
}

void
av::av_popMsg(av::Msg& msg, std::vector<double>& buf)
{
  unsigned int arraySize;
  av_popMsg(msg, arraySize);

  buf.resize(arraySize);

  std::vector<double> tmpVec(arraySize);

  XDRHandle xdr(&tmpVec[0], sizeof(double)*arraySize, XDR_DECODE);

  msg.pop(&tmpVec[0], sizeof(double)*arraySize);

  xdr_vector(xdr.getXDR(), reinterpret_cast<char*>(&buf[0]), buf.size(), sizeof(double),
             (xdrproc_t)xdr_double);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (vector<double>) [" << buf.size() << "]";
#endif
}

// UUID
void
av::av_pushMsg(av::Msg& msg, const UUID& buf)
{
#ifdef AVANGO_DEBUG
  logger.trace() << "push: (UUID) [" << (std::string) buf << "]";
#endif

  av_pushMsg(msg, static_cast<std::string>(buf));
}

void
av::av_popMsg(av::Msg& msg, UUID& buf)
{
  std::string tmp;

  av_popMsg(msg, tmp);

  buf = UUID(tmp);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop (UUID) [" << (std::string) buf << "]";
#endif
}

// AnyLink
void
av::av_pushMsg(av::Msg& msg, const AnyLink& buf)
{
#ifdef AVANGO_DEBUG
  logger.trace() << "push: (AnyLink) [<blank>]" ;
#endif

  if (!buf.isValid())
  {
    NetID id(NetID::nullID());
    av_pushMsg(msg, id);
    return;
  }

  Distributed* obj = dynamic_cast<Distributed*>(buf.getBasePtr());
  AV_ASSERT(obj);

  if (!obj->isDistributed())
  {
    NetID id(NetID::nullID());
    av_pushMsg(msg, id);
    return;
  }

  NetID id(obj->netID());
  av_pushMsg(msg, id);
}

void
av::av_popMsg(av::Msg& msg, AnyLink& buf)
{
#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (AnyLink) [<blank>]";
#endif

  NetID id;
  av_popMsg(msg, id);
  if (id.isNull())
  {
    buf.clear();
    return;
  }

  AV_ASSERT(msg.getNetNode());
  Link<Distributed> obj;

  if (!msg.getNetNode()->lookup(id, obj))
  {
    buf.clear();
    return;
  }

  if (!obj.isValid() || !buf.setBasePtr(obj.getBasePtr()))
  {
    // this object is either not there or of wrong type.
    logger.warn("pop: object is either not there or of wrong type.");
    buf.clear();
  }
}

// Type
void
av::av_pushMsg(av::Msg& msg, const Type& buf)
{
#ifdef AVANGO_DEBUG
  logger.trace() << "push: (Type) [" << buf.getName().c_str() << "]";
#endif

  av_pushMsg(msg, buf.getName());
}

void
av::av_popMsg(av::Msg& msg, Type& buf)
{
  std::string type_name;
  av_popMsg(msg, type_name);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (Type) [" << type_name << "]" ;
#endif

  buf = Type::getByName(type_name);

  if (!buf.isBad())
  {
    // type is known, everything is fine
    return;
  } else
  {
  // we don't know this type
    logger.error() << "pop: unable to initialize type '"
                   << type_name
                   << "'; what else can i do?";
    return;
  }
}

// Field
void
av::av_pushMsg(av::Msg& msg, av::Field* field)
{
#ifdef AVANGO_DEBUG
  logger.trace() << "push: (Field*) ["
                 << field << ","
                 << field->getTypeId().getName() << ","
                 << field->getName()  << ","
                 << field->getIndex() << "]";
#endif

  AV_ASSERT(field);

  field->push(msg);
}

void
av::av_popMsg(av::Msg& msg, av::Field* field)
{
#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (Field*) ["
                 << field << ","
                 << field->getTypeId().getName() << ","
                 << field->getName()  << ","
                 << field->getIndex() << "]";
#endif

  AV_ASSERT(field);
  field->pop(msg);

}

// Distributed
void
av::av_pushMsg(av::Msg& msg, av::Distributed* distrObj)
{
#ifdef AVANGO_DEBUG
  logger.trace() << "push: (Distributed*) ["
                 << distrObj << ","
                 << distrObj->getTypeId().getName()
                 << "]";
#endif

  AV_ASSERT(distrObj);
  distrObj->push(msg);
}

void
av::av_popMsg(av::Msg& msg, av::Distributed* distrObj)
{
#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (Distributed*) ["
                 << distrObj << ","
                 << distrObj->getTypeId().getName()
                 << "]";
#endif

  AV_ASSERT(distrObj);
  distrObj->pop(msg);
}


// std::string
void
av::av_pushMsg(av::Msg& msg, const std::string& str)
{
#ifdef AVANGO_DEBUG
  logger.trace() << "push: (std::string) [" << str << "]";
#endif

  int length = str.size();
  if (length)
  {
    msg.push(str.c_str(), length);
  }
  av_pushMsg(msg, length);
}

void
av::av_popMsg(av::Msg& msg, std::string& str)
{
  int length = 0;
  av_popMsg(msg, length);
  if (length)
  {
    char* buf = new char[length];
    boost::scoped_array<char> bufHandler(buf);
    msg.pop(buf, length);
    str.assign(buf, length);
  }
  else
  {
    str.assign("");
  }

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (std::string) [" << str << "]";
#endif
}

// NetID
void
av::av_pushMsg(av::Msg& msg, const av::NetID& id)
{
#ifdef AVANGO_DEBUG
  logger.trace() << "push: (NetID) [" << id << "]" ;
#endif

  av_pushMsg(msg, id.getEID());
  av_pushMsg(msg, id.getNum());
}

void
av::av_popMsg(av::Msg& msg, av::NetID& id)
{
  int  number = 0;
  std::string eid;
  bool well_known = false;

  av_popMsg(msg, number);
  av_popMsg(msg, eid);

  id.set(eid, number);

#ifdef AVANGO_DEBUG
  logger.trace() << "pop: (NetID) [" << id << "]";
#endif

}
