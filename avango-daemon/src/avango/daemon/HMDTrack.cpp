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
  uint32_t conflate_messages = 1;
  socket.setsockopt(ZMQ_CONFLATE, &conflate_messages, sizeof(conflate_messages));
  std::string endpoint("tcp://" + address);
  socket.connect(endpoint.c_str());


  while (mKeepRunning) {
    zmq::message_t message(sizeof(HMD_Message));
    socket.recv(&message);
    HMD_Message hmdMessage;
    std::memcpy(&hmdMessage, reinterpret_cast<HMD_Message*>(message.data()), sizeof(HMD_Message));
   
	// Set values for headset
	mStations[0]->setButton(0, hmdMessage.hmd.status);
    if (hmdMessage.hmd.status) {
      mStations[0]->setMatrix(::gua::math::mat4(hmdMessage.hmd.matrix));
    }
    mStations[0]->setValue(0, hmdMessage.hmd.id);

	// Set values for controllers
    if (!hmdMessage.controllers.empty()) {  
      for (int i = 0; i < 2; ++i)
      {
        mStations[i + 1]->setButton(0, hmdMessage.controllers[i].status);
        if(hmdMessage.controllers[i].status) {
            mStations[i + 1]->setMatrix(::gua::math::mat4(hmdMessage.controllers[i].matrix));
        }
        mStations[i + 1]->setValue(0, hmdMessage.controllers[i].id);
        mStations[i + 1]->setButton(1, hmdMessage.controllers[i].appMenuButton);
        mStations[i + 1]->setButton(2, hmdMessage.controllers[i].gripButton);
        mStations[i + 1]->setButton(3, hmdMessage.controllers[i].padTouchButton);
        mStations[i + 1]->setButton(4, hmdMessage.controllers[i].padButton);
        mStations[i + 1]->setButton(5, hmdMessage.controllers[i].triggerButton);
        mStations[i + 1]->setValue(1, hmdMessage.controllers[i].padX);
        mStations[i + 1]->setValue(2, hmdMessage.controllers[i].padY);
        mStations[i + 1]->setValue(3, hmdMessage.controllers[i].trigger);
      }
    }

	// Set values for base stations
    if (!hmdMessage.baseStations.empty()) {
      for (int i = 0 ; i < 2; ++i)
      {
        mStations[i + 3]->setValue(0, hmdMessage.baseStations[i].id);
        if(hmdMessage.baseStations[i].id) {
          mStations[i + 3]->setMatrix(::gua::math::mat4(hmdMessage.baseStations[i].matrix));
        }
        mStations[i + 3]->setButton(0, hmdMessage.baseStations[i].status);
      }
    }

	// Set values for trackers
	if (!hmdMessage.trackers.empty()) {
		for (int i = 0; i < 2; ++i)
		{
			mStations[i + 5]->setValue(0, hmdMessage.trackers[i].id);
			if (hmdMessage.trackers[i].id) {
				mStations[i + 5]->setMatrix(::gua::math::mat4(hmdMessage.trackers[i].matrix));
			}
			mStations[i + 5]->setButton(0, hmdMessage.trackers[i].status);
		}
	}
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
