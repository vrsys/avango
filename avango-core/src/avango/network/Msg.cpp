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

#include <boost/format.hpp>
#include <boost/scoped_array.hpp>

namespace
{
av::logging::Logger& logger(av::logging::Logger::getLogger("av::Msg"));
}

av::Msg::Msg() : mMsgType(relative), mMsgBuffer(new MessageBuffer()) {}

av::Msg::Msg(MsgType newMsgType, boost::shared_ptr<MessageBuffer> newBuffer) : mMsgType(newMsgType), mMsgBuffer(newBuffer) {}

av::Msg::~Msg() {}

size_t av::Msg::getSize() { return mMsgBuffer->size(); }

void av::Msg::resize(size_t newSize) { mMsgBuffer->resize(newSize); }

unsigned char* av::Msg::getBuffer() { return &(mMsgBuffer->operator[](0)); }

av::NetNode* av::Msg::getNetNode() const { return mNetNode; }

void av::Msg::setNetNode(av::NetNode* netNode) { mNetNode = netNode; }

av::Msg av::Msg::clone() const
{
    boost::shared_ptr<MessageBuffer> copied_buffer(new MessageBuffer(*mMsgBuffer));
    return Msg(mMsgType, copied_buffer);
}

// void*
void av::Msg::push(const void* buffer, size_t bytes)
{
    const unsigned char* bufferMem = static_cast<const unsigned char*>(buffer);
    mMsgBuffer->insert(mMsgBuffer->end(), bufferMem, bufferMem + bytes);

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (void*) [%1% byte%2%]") % bytes % (1 == bytes ? "" : "s")))
}

void av::Msg::pop(void* buffer, size_t bytes)
{
    unsigned char* bufferMem = static_cast<unsigned char*>(buffer);

    AV_ASSERT(mMsgBuffer->size() >= bytes);
    std::copy(mMsgBuffer->end() - bytes, mMsgBuffer->end(), bufferMem);

    mMsgBuffer->resize(mMsgBuffer->size() - bytes);

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (void*) [%1% byte%2%]") % bytes % (1 == bytes ? "" : "s")))
}

// bool
void av::av_pushMsg(av::Msg& msg, const bool& buf)
{
    int32_t b = (buf ? 1 : 0);
    av_pushMsg(msg, b);

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (bool) [%s]") % (buf ? "true" : "false")))
}

void av::av_popMsg(av::Msg& msg, bool& buf)
{
    int32_t b;
    av_popMsg(msg, b);
    buf = (b == 0 ? false : true);

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (bool) [%s]") % (buf ? "true" : "false")))
}

// int32_t
void av::av_pushMsg(av::Msg& msg, const int32_t& buf)
{
    msg.push(&buf, sizeof(buf));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (int32_t) [%1%]") % buf));
}

void av::av_popMsg(av::Msg& msg, int32_t& buf)
{
    msg.pop((void*)&buf, sizeof(buf));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (int32_t) [%1%]") % buf));
}

// std::vector<int32_t>
void av::av_pushMsg(av::Msg& msg, const std::vector<int32_t>& buf)
{
    msg.push(buf.data(), sizeof(int32_t) * buf.size());
    av_pushMsg(msg, static_cast<uint32_t>(buf.size()));
}

void av::av_popMsg(av::Msg& msg, std::vector<int32_t>& buf)
{
    uint32_t arraySize;
    av_popMsg(msg, arraySize);

    buf.resize(arraySize);

    msg.pop(buf.data(), sizeof(int32_t) * arraySize);
}

// uint32_t
void av::av_pushMsg(av::Msg& msg, const uint32_t& buf)
{
    msg.push(&buf, sizeof(buf));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (uint32_t) [%1%]") % buf));
}

void av::av_popMsg(av::Msg& msg, uint32_t& buf)
{
    msg.pop(&buf, sizeof(buf));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (uint32_t) [%1%]") % buf));
}

// std::vector<uint32_t>
void av::av_pushMsg(av::Msg& msg, const std::vector<uint32_t>& buf)
{
    msg.push(buf.data(), sizeof(uint32_t) * buf.size());
    av_pushMsg(msg, static_cast<uint32_t>(buf.size()));
}

void av::av_popMsg(av::Msg& msg, std::vector<uint32_t>& buf)
{
    uint32_t arraySize;
    av_popMsg(msg, arraySize);

    buf.resize(arraySize);

    msg.pop(buf.data(), sizeof(uint32_t) * arraySize);
}

// int64_t
void av::av_pushMsg(av::Msg& msg, const int64_t& buf)
{
    msg.push(&buf, sizeof(buf));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (int64_t) [%1%]") % buf));
}

void av::av_popMsg(av::Msg& msg, int64_t& buf)
{
    msg.pop(&buf, sizeof(buf));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (int64_t) [%1%]") % buf));
}

// std::vector<int64_t>
void av::av_pushMsg(av::Msg& msg, const std::vector<int64_t>& buf)
{
    msg.push(buf.data(), sizeof(int64_t) * buf.size());
    av_pushMsg(msg, static_cast<uint32_t>(buf.size()));
}

void av::av_popMsg(av::Msg& msg, std::vector<int64_t>& buf)
{
    uint32_t arraySize;
    av_popMsg(msg, arraySize);

    buf.resize(arraySize);

    msg.pop(buf.data(), sizeof(int64_t) * arraySize);
}

// uint64_t
void av::av_pushMsg(av::Msg& msg, const uint64_t& buf)
{
    msg.push(&buf, sizeof(buf));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (uint64_t) [%1%]") % buf));
}

void av::av_popMsg(av::Msg& msg, uint64_t& buf)
{
    msg.pop(&buf, sizeof(buf));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (uint64_t) [%1%]") % buf));
}

// std::vector<uint64_t>
void av::av_pushMsg(av::Msg& msg, const std::vector<uint64_t>& buf)
{
    msg.push(buf.data(), sizeof(uint64_t) * buf.size());
    av_pushMsg(msg, static_cast<uint32_t>(buf.size()));
}

void av::av_popMsg(av::Msg& msg, std::vector<uint64_t>& buf)
{
    uint32_t arraySize;
    av_popMsg(msg, arraySize);

    buf.resize(arraySize);

    msg.pop(buf.data(), sizeof(uint64_t) * arraySize);
}

// float
void av::av_pushMsg(av::Msg& msg, const float& buf)
{
    msg.push(&buf, sizeof(buf));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (float) [%1%]") % buf));
}

void av::av_popMsg(av::Msg& msg, float& buf)
{
    msg.pop(&buf, sizeof(buf));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (float) [%1%]") % buf));
}

// std::vector<float>
void av::av_pushMsg(av::Msg& msg, const std::vector<float>& buf)
{
    msg.push(buf.data(), sizeof(float) * buf.size());
    av_pushMsg(msg, static_cast<uint32_t>(buf.size()));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (vector<float>) [%1%]") % buf.size()))
}

void av::av_popMsg(av::Msg& msg, std::vector<float>& buf)
{
    uint32_t arraySize;
    av_popMsg(msg, arraySize);

    buf.resize(arraySize);

    msg.pop(buf.data(), sizeof(float) * arraySize);

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (vector<float>) [%1%]") % buf.size()))
}

// double
void av::av_pushMsg(av::Msg& msg, const double& buf)
{
    msg.push(&buf, sizeof(buf));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (double) [%1%]") % buf))
}

void av::av_popMsg(av::Msg& msg, double& buf)
{
    msg.pop(&buf, sizeof(buf));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (double) [%1%]") % buf))
}

// std::vector<double>
void av::av_pushMsg(av::Msg& msg, const std::vector<double>& buf)
{
    msg.push(buf.data(), sizeof(double) * buf.size());
    av_pushMsg(msg, static_cast<uint32_t>(buf.size()));

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (vector<double>) [%1%]") % buf.size()))
}

void av::av_popMsg(av::Msg& msg, std::vector<double>& buf)
{
    uint32_t arraySize;
    av_popMsg(msg, arraySize);

    buf.resize(arraySize);

    msg.pop(buf.data(), sizeof(double) * arraySize);

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (vector<double>) [%1%]") % buf.size()))
}

// AnyLink
void av::av_pushMsg(av::Msg& msg, const AnyLink& buf)
{
    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (AnyLink) [<blank>]")));

    if(!buf.isValid())
    {
        NetID id(NetID::nullID());
        av_pushMsg(msg, id);
        return;
    }

    Distributed* obj = dynamic_cast<Distributed*>(buf.getBasePtr());
    AV_ASSERT(obj);

    if(!obj->isDistributed())
    {
        NetID id(NetID::nullID());
        av_pushMsg(msg, id);
        return;
    }

    NetID id(obj->netID());
    av_pushMsg(msg, id);
}

void av::av_popMsg(av::Msg& msg, AnyLink& buf)
{
    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (AnyLink) [<blank>]")));

    NetID id;
    av_popMsg(msg, id);
    if(id.isNull())
    {
        buf.clear();
        return;
    }

    AV_ASSERT(msg.getNetNode());
    Link<Distributed> obj;

    if(!msg.getNetNode()->lookup(id, obj))
    {
        buf.clear();
        return;
    }

    if(!obj.isValid() || !buf.setBasePtr(obj.getBasePtr()))
    {
        // this object is either not there or of wrong type.
        AVANGO_LOG(logger, logging::WARN, boost::str(boost::format("pop: object is either not there or of wrong type.")));
        buf.clear();
    }
}

// Type
void av::av_pushMsg(av::Msg& msg, const Type& buf)
{
    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (Type) [%1%]") % buf.getName().c_str()))

    av_pushMsg(msg, buf.getName());
}

void av::av_popMsg(av::Msg& msg, Type& buf)
{
    std::string type_name;
    av_popMsg(msg, type_name);

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (Type) [%1%]") % type_name))

    buf = Type::getByName(type_name);

    if(!buf.isBad())
    {
        // type is known, everything is fine
        return;
    }
    else
    {
        // we don't know this type
        AVANGO_LOG(logger, logging::ERROR, boost::str(boost::format("pop: unable to initialize type '%1%'; what else can I do?") % type_name))
        return;
    }
}

// Field
void av::av_pushMsg(av::Msg& msg, av::Field* field)
{
    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (Field*) [%1%,%2%,%3%,%4%]") % field % field->getTypeId().getName() % field->getName() % field->getIndex()))

    AV_ASSERT(field);

    field->push(msg);
}

void av::av_popMsg(av::Msg& msg, av::Field* field)
{
    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (Field*) [%1%,%2%,%3%,%4%]") % field % field->getTypeId().getName() % field->getName() % field->getIndex()))

    AV_ASSERT(field);
    field->pop(msg);
}

// Distributed
void av::av_pushMsg(av::Msg& msg, av::Distributed* distrObj)
{
    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (Distributed*) [%1%,%2%]") % distrObj % distrObj->getTypeId().getName()))

    AV_ASSERT(distrObj);
    distrObj->push(msg);
}

void av::av_popMsg(av::Msg& msg, av::Distributed* distrObj)
{
    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (Distributed*) [%1%,%2%]") % distrObj % distrObj->getTypeId().getName()))

    AV_ASSERT(distrObj);
    distrObj->pop(msg);
}

// std::string
void av::av_pushMsg(av::Msg& msg, const std::string& str)
{
    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (std::string) [%1%]") % str));

    uint32_t length = str.size();
    if(length)
    {
        msg.push(str.c_str(), length);
    }
    av_pushMsg(msg, length);
}

void av::av_popMsg(av::Msg& msg, std::string& str)
{
    uint32_t length = 0;
    av_popMsg(msg, length);
    if(length)
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

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (std::string) [%1%]") % str));
}

// NetID
void av::av_pushMsg(av::Msg& msg, const av::NetID& id)
{
    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("push: (NetID) [%1%]") % id));

    av_pushMsg(msg, id.getEID());
    av_pushMsg(msg, id.getNum());
}

void av::av_popMsg(av::Msg& msg, av::NetID& id)
{
    int number = 0;
    std::string eid;
    bool well_known = false;

    av_popMsg(msg, number);
    av_popMsg(msg, eid);

    id.set(eid, number);

    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("pop: (NetID) [%1%]") % id));
}
