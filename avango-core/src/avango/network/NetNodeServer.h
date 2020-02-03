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

#if !defined(AVANGO_NETNODESERVER_H)
#define AVANGO_NETNODESERVER_H

#include <string>
#include <vector>
#include <zmq.hpp>

#include <avango/Msg.h>

namespace av
{
class NetNode;

class NetNodeServer
{
  public:
#if ZMQ_VERSION_MAJOR < 3
    NetNodeServer(const std::string& host, const std::string& port, int numClients, av::NetNode* netnode, const std::string& ce, const std::string& se, uint64_t hwm);
#else
    NetNodeServer(const std::string& host, const std::string& port, int numClients, av::NetNode* netnode, const std::string& ce, const std::string& se, uint32_t hwm);
#endif

    // called from NetNode
    void cast(av::Msg& av_msg);

  private:
    std::string mHost;
    std::string mPort;
    av::NetNode* mNetNode;
    zmq::context_t mCtx;
    std::vector<zmq::socket_t*> mSockets;
    std::string mClientEndpoint;
    std::string mServerEndpoint;
};

}; // namespace av

#endif // #ifndef  AVANGO_NETNODESERVER_H
