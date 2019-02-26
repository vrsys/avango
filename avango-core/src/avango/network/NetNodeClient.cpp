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

#include "NetNodeClient.h"

#include <iostream>
#include <thread>
#include <zmq.hpp>

#include <avango/NetNode.h>
#include <avango/Msg.h>

namespace
{
::av::Msg av_Msg_recv(zmq::socket_t& socket)
{
    zmq::message_t zmq_message;
    socket.recv(&zmq_message); // blocking

    av::Msg av_msg;
    av_msg.resize(zmq_message.size());
    memcpy(av_msg.getBuffer(), zmq_message.data(), zmq_message.size());
    return av_msg;
}

} // namespace

av::NetNodeClient::NetNodeClient(const std::string& host, const std::string& port, av::NetNode* netnode, const std::string& ce, const std::string& se)
    : mHost(host), mPort(port), mThread(), mRunning(false), mNetNode(netnode), mClientEndpoint(ce), mServerEndpoint(se)
{
}

av::NetNodeClient::~NetNodeClient()
{
    stop();
    mThread.join();
}

void av::NetNodeClient::start()
{
    // launch thread:
    av::Msg dummy_msg;
    mNetNode->getStateFragment(mClientEndpoint, dummy_msg);
    mNetNode->setStateFragment(mServerEndpoint, dummy_msg);

    if(!mRunning)
    {
        mRunning = true;
        mThread = std::thread([this]() { this->loop(); });
    }
    else
    {
        std::cerr << "ALARM in NetNodeClient::start() , thread already running!" << std::endl;
    }
}

void av::NetNodeClient::stop() { mRunning = false; }

void av::NetNodeClient::loop()
{
    // open connection
    zmq::context_t ctx(1);              // means single threaded
    zmq::socket_t socket(ctx, ZMQ_SUB); // means a subscriber

    socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);

#if ZMQ_VERSION_MAJOR < 3
    uint64_t hwm = 1;
    socket.setsockopt(ZMQ_HWM, &hwm, sizeof(hwm));
#else
    uint32_t hwm = 1;
    socket.setsockopt(ZMQ_RCVHWM, &hwm, sizeof(hwm));
#endif

    std::string endpoint("tcp://" + mHost + ":" + mPort);
    socket.connect(endpoint.c_str());

    while(mRunning)
    {
        av::Msg av_msg = av_Msg_recv(socket);
        mNetNode->receiveMessage(mServerEndpoint, av_msg);
    }
}
