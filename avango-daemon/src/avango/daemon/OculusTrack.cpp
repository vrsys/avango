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

#include <errno.h>
#include <gua/math.hpp>
#include <avango/Logger.h>
#include <avango/daemon/OculusTrack.h>
#include <avango/daemon/Station.h>
#include <cstring>
#include <string>
#include <iostream>
#include <zmq.hpp>
#include <array>

namespace
{
av::Logger& logger(av::getLogger("av::daemon::OculusTrack"));
}

AV_BASE_DEFINE(av::daemon::OculusTrack);

av::daemon::OculusTrack::OculusTrack() : Device(), mRequiredFeatures({"port", "server"}), mPort("7701"), mServer("127.0.0.1") {}

void av::daemon::OculusTrack::initClass()
{
    if(!isTypeInitialized())
    {
        av::daemon::Device::initClass();
        AV_BASE_INIT(av::daemon::Device, av::daemon::OculusTrack, true);
    }
}

/* virtual */ void av::daemon::OculusTrack::startDevice()
{
    if(!parseFeatures())
        return;

    // initialize

    logger.info() << "startDevice: device initialized successfully";
    std::cout << "OculusTrack::startDevice\n";
}

/* virtual */ void av::daemon::OculusTrack::readLoop()
{
    std::string mServer = queryFeature("server");
    std::string mPort = queryFeature("port");
    std::string address = mServer + ":" + mPort; // {"127.0.0.1:7000"};
    logger.info() << "readLoop: start";
    std::cout << "OculusTrack::readLoop " << address << "\n";

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
    std::string endpoint("tcp://" + address);
    socket.connect(endpoint.c_str());

    // stations[0] |-> head

    while(mKeepRunning)
    {
        zmq::message_t message(sizeof(std::array<float, 16>)); //::gua::math::mat4f));
        socket.recv(&message);
        // ::gua::math::mat4f pose;
        // std::memcpy( &pose, reinterpret_cast<::gua::math::mat4f*>(message.data()), sizeof(::gua::math::mat4f));
        ::gua::math::mat4f matrix;
        std::memcpy(&matrix, reinterpret_cast<::gua::math::mat4f*>(message.data()), sizeof(::gua::math::mat4f));
        // mStations[0]->setMatrix(::gua::math::mat4(pose));
        mStations[0]->setMatrix(::gua::math::mat4(matrix));
    }
}

/* virtual */ void av::daemon::OculusTrack::stopDevice()
{
#if 0
  mOculusTrack->exit();
#endif
    logger.info() << "stopDevice: done.";
}

const std::vector<std::string>& av::daemon::OculusTrack::queryFeatures() { return mRequiredFeatures; }

bool av::daemon::OculusTrack::parseFeatures()
{
    mPort = queryFeature("port");
    if(mPort == "")
    {
        logger.warn() << "parseFeatures: feature 'port' not specified";
        return false;
    }
    else
    {
        logger.info() << "parseFeatures: configured feature 'port' = %s", mPort;
    }

    mServer = queryFeature("server");
    if(mServer == "")
    {
        logger.warn() << "parseFeatures: feature 'server' not specified";
        return false;
    }
    else
    {
        logger.info() << "parseFeatures: configured feature 'server' = %s", mServer;
    }

    return true;
}
