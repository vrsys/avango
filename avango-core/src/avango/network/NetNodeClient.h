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

#if !defined(AVANGO_NETNODECLIENT_H)
#define AVANGO_NETNODECLIENT_H

#include <string>
#include <thread>

namespace av
{
class NetNode;

class NetNodeClient
{
  public:
    NetNodeClient(const std::string& host, const std::string& port, av::NetNode* netnode, const std::string& ce, const std::string& se);

    ~NetNodeClient();

    // called from NetNode
    void start();

    void stop();

  private:
    // the thread loop
    void loop();

    std::string mHost;
    std::string mPort;
    std::thread mThread;
    bool mRunning;
    av::NetNode* mNetNode;
    std::string mClientEndpoint;
    std::string mServerEndpoint;
};

} // namespace av

#endif // #ifndef  AVANGO_NETNODECLIENT_H
