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
#include <thread>

av::NetNodeServer::NetNodeServer(const std::string& host, const std::string& port, av::NetNode* netnode, const std::string& ce, const std::string& se, uint64_t hwm)
  : mHost(host),
    mPort(port),
    mNetNode(netnode),
    mCtx(1),
    mSocket(mCtx, ZMQ_PUB),
    mClientEndpoint(ce),
    mServerEndpoint(se)
{
  //int64_t  rate(500);
  //mSocket.setsockopt(ZMQ_RATE,&rate, sizeof(rate));

  mSocket.setsockopt(ZMQ_HWM,&hwm, sizeof(hwm));
  std::string endpoint("tcp://" + mHost + ":" + mPort);
  mSocket.bind(endpoint.c_str());
  std::chrono::milliseconds dura(200);
  std::this_thread::sleep_for(dura);
}

av::NetNodeServer::~NetNodeServer()
{}

void
av::NetNodeServer::cast(av::Msg& av_msg)
{
  zmq::message_t zmq_message(av_msg.getSize());
  memcpy(zmq_message.data(), av_msg.getBuffer(), av_msg.getSize());
  mSocket.send(zmq_message);
}
