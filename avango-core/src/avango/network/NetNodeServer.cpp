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

#include "NetNodeServer.h"

#include <avango/NetNode.h>
#include <chrono>
#include <iostream>
#include <thread>

#if ZMQ_VERSION_MAJOR < 3
av::NetNodeServer::NetNodeServer(const std::string& host, const std::string& port, int numClients, av::NetNode* netnode, const std::string& ce, const std::string& se, uint64_t hwm)
#else
av::NetNodeServer::NetNodeServer(const std::string& host, const std::string& port, int numClients, av::NetNode* netnode, const std::string& ce, const std::string& se, uint32_t hwm)
#endif
    : mHost(host), mPort(port), mNetNode(netnode), mCtx(1), mClientEndpoint(ce), mServerEndpoint(se) {
#if ZMQ_VERSION_MAJOR < 3
    //mSocket.setsockopt(ZMQ_HWM, &hwm, sizeof(hwm));
#else
    //mSocket.setsockopt(ZMQ_SNDHWM, &hwm, sizeof(hwm));
#endif
	for(int i = 0; i < numClients; ++i)
    {
        zmq::socket_t* socket = new zmq::socket_t(mCtx, ZMQ_PUSH);
        std::string endpoint("tcp://" + mHost + ":" + std::to_string(std::stoi(mPort)+i));
        socket->bind(endpoint.c_str());
        mSockets.push_back(socket);
	}

	if(numClients > 0)
    {
       std::cout << "Started with " << numClients << " ZMQ_PUSH sockets from ports " << mPort << " to " << std::to_string(std::stoi(mPort) + numClients - 1) << std::endl;
    }
    else
    {
       std::cout << "Started without sockets, server-only mode" << std::endl;
	}
	
}

void av::NetNodeServer::cast(av::Msg& av_msg)
{
    for(zmq::socket_t* socket : mSockets)
    {
        zmq::message_t zmq_message(av_msg.getSize());
        memcpy(zmq_message.data(), av_msg.getBuffer(), av_msg.getSize());
        socket->send(zmq_message);
    }
}
