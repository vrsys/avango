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
#include <avango/daemon/HMDTrack.h>
#include <avango/daemon/Station.h>
#include <cstring>
#include <string>
#include <iostream>
#include <zmq.hpp>
#include <array>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::HMDTrack"));
}

AV_BASE_DEFINE(av::daemon::HMDTrack);

av::daemon::HMDTrack::HMDTrack()
  : Device()
  , mRequiredFeatures({ "port", "server" })
  , mPort("7770")
  , mServer("127.0.0.1")
{}

void
av::daemon::HMDTrack::initClass()
{
  if (!isTypeInitialized())
  {
    av::daemon::Device::initClass();
    AV_BASE_INIT(av::daemon::Device, av::daemon::HMDTrack, true);
  }
}

/* virtual */ void
av::daemon::HMDTrack::startDevice()
{
  if (!parseFeatures())
    return;

  // initialize

  logger.info() << "startDevice: device initialized successfully";
  std::cout << "HMDTrack::startDevice\n";
}

/* virtual */ void
av::daemon::HMDTrack::readLoop()
{
  std::string mServer = queryFeature("server");
  std::string mPort = queryFeature("port");
  std::string address = mServer + ":" + mPort; // {"127.0.0.1:7000"};
  logger.info() << "readLoop: start";
  std::cout << "HMDTrack::readLoop " << address << "\n";

  zmq::context_t ctx(1); // means single threaded
  zmq::socket_t  socket(ctx, ZMQ_SUB); // means a subscriber
  socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
#if ZMQ_VERSION_MAJOR < 3
  uint64_t hwm = 1;
  socket.setsockopt(ZMQ_HWM,&hwm, sizeof(hwm));
#else
  uint32_t hwm = 1;
  socket.setsockopt(ZMQ_RCVHWM, &hwm, sizeof(hwm));
#endif
  std::string endpoint("tcp://" + address);
  socket.connect(endpoint.c_str());

  // stations[0] |-> head

  while (mKeepRunning) {
    zmq::message_t message(sizeof(HMD_Message));
    socket.recv(&message);
    // ::gua::math::mat4f pose;
    // std::memcpy( &pose, reinterpret_cast<::gua::math::mat4f*>(message.data()), sizeof(::gua::math::mat4f));
    HMD_Message vrdevices;
    std::memcpy( &vrdevices, reinterpret_cast<HMD_Message*>(message.data()),
      sizeof(HMD_Message));
    mStations[0]->setButton(0, vrdevices.hmd_status);
    if (vrdevices.hmd_status) {
      //std::cout << "HMD Status OK!" << std::endl;
      mStations[0]->setMatrix(::gua::math::mat4(vrdevices.hmd));
    }
    mStations[0]->setValue(0, vrdevices.hmd_id);
    if (!vrdevices.controller.empty()) {  
      for (int i = 1; i < 5 ; ++i)
      {
        mStations[i]->setButton(0, vrdevices.controller[i-1].status);
        if(vrdevices.controller[i-1].status) {
          mStations[i]->setMatrix(::gua::math::mat4(vrdevices.controller[i-1].matrix));
        }
        //std::cout << "Mat = " << vrdevices.controller[i-1].matrix << std::endl;
        mStations[i]->setValue(0, vrdevices.controller[i-1].id);
        mStations[i]->setButton(1, vrdevices.controller[i-1].app_menu);
        mStations[i]->setButton(2, vrdevices.controller[i-1].grip);
        //std::cout << vrdevices.controller[i-1].grip << std::endl;
        mStations[i]->setButton(3, vrdevices.controller[i-1].pad_touch);
        mStations[i]->setButton(4, vrdevices.controller[i-1].pad_press);
        mStations[i]->setButton(5, vrdevices.controller[i-1].trigger_p);
        mStations[i]->setValue(1, vrdevices.controller[i-1].pad_x);
        //std::cout << vrdevices.controller[i-1].pad_x << std::endl;
        mStations[i]->setValue(2, vrdevices.controller[i-1].pad_y);
        //std::cout << vrdevices.controller[i-1].pad_y << std::endl;
        mStations[i]->setValue(3, vrdevices.controller[i-1].trigger);
        //std::cout << vrdevices.controller[i-1].trigger << std::endl;
      }
    }
    if (!vrdevices.tracker.empty()) {
      for (int i = 5 ; i < 7; ++i)
      {
        mStations[i]->setValue(0, vrdevices.tracker[i-5].id);
        if(vrdevices.tracker[i-5].id) {
          mStations[i]->setMatrix(::gua::math::mat4(vrdevices.tracker[i-5].matrix));
        }
        mStations[i]->setButton(0, vrdevices.tracker[i-5].status);
      }
    }
    // mStations[0]->setMatrix(::gua::math::mat4(pose));
  }
}

/* virtual */ void
av::daemon::HMDTrack::stopDevice()
{
#if 0
  mVHMDTrack->exit();
#endif
  logger.info() << "stopDevice: done.";
}

const std::vector<std::string>&
av::daemon::HMDTrack::queryFeatures()
{
  return mRequiredFeatures;
}

bool
av::daemon::HMDTrack::parseFeatures()
{
  mPort = queryFeature("port");
  if (mPort == "") {
    logger.warn() << "parseFeatures: feature 'port' not specified";
    return false;
  } else {
    logger.info() << "parseFeatures: configured feature 'port' = %s", mPort;
  }

  mServer = queryFeature("server");
  if (mServer == "") {
    logger.warn() << "parseFeatures: feature 'server' not specified";
    return false;
  } else {
    logger.info() << "parseFeatures: configured feature 'server' = %s", mServer;
  }

  return true;
}
