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
#include <avango/daemon/KinectTrack.h>
#include <avango/daemon/Station.h>
#include <cstring>
#include <string>
#include <iostream>
#include <zmq.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::KinectTrack"));
}

AV_BASE_DEFINE(av::daemon::KinectTrack);

av::daemon::KinectTrack::KinectTrack()
  : Device()
  , mRequiredFeatures{"port", "server"}
  , mPort{"7000"}
  , mServer{"127.0.0.1"}
{}

void
av::daemon::KinectTrack::initClass()
{
  if (!isTypeInitialized())
  {
    av::daemon::Device::initClass();
    AV_BASE_INIT(av::daemon::Device, av::daemon::KinectTrack, true);
  }
}

/* virtual */ void
av::daemon::KinectTrack::startDevice()
{
  if (!parseFeatures())
    return;

  // initialize

  logger.info() << "startDevice: device initialized successfully";
  std::cout << "KinectTrack::startDevice\n";
}

/* virtual */ void
av::daemon::KinectTrack::readLoop()
{
  std::string mServer = queryFeature("server");
  std::string mPort = queryFeature("port");
  std::string address = mServer + ":" + mPort; // {"127.0.0.1:7000"};
  logger.info() << "readLoop: start";
  std::cout << "KinectTrack::readLoop " << address << "\n";

  zmq::context_t ctx(1); // means single threaded
  zmq::socket_t  socket(ctx, ZMQ_SUB); // means a publisher
  socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);;
  uint64_t hwm = 1;
  socket.setsockopt(ZMQ_HWM,&hwm, sizeof(hwm));
  std::string endpoint("tcp://" + address);
  socket.connect(endpoint.c_str());

  // stations[0] |-> head

  while (mKeepRunning) {
    zmq::message_t message(16*sizeof(float));
    socket.recv(&message);
    // * TODO Use port and server property to set address
    ::gua::math::mat4f pose;
    std::memcpy( &pose, reinterpret_cast<::gua::math::mat4f*>(message.data()), sizeof(::gua::math::mat4f));
    mStations[0]->setMatrix(::gua::math::mat4(pose));
  }
}

/* virtual */ void
av::daemon::KinectTrack::stopDevice()
{
#if 0
  mKinectTrack->exit();
#endif
  logger.info() << "stopDevice: done.";
}

const std::vector<std::string>&
av::daemon::KinectTrack::queryFeatures()
{
  return mRequiredFeatures;
}

bool
av::daemon::KinectTrack::parseFeatures()
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

// from readLoop
#if 0
{
  logger.debug() << "readLoop: frame: %lu, stamp: %.3f, bodycal: %d, bodies: %d, sticks: %d, tools: %d, marker: %d",
                 framenr, timestamp, nbodycal, nbody, nflystick, nmeatool, nmarker;

  for (int i=0; i<nbody && i<MAX_NBODY; i++)
  {
    logger.debug() << "readLoop: body   %02d: qu[%.3f] pos[%f %f %f]",
                   body[i].id, body[i].quality, body[i].loc[0], body[i].loc[1], body[i].loc[2];
  }

  for (int i=0; i<nflystick && i<MAX_NFLYSTICK; i++)
  {
    logger.debug() << "readLoop: stick  %02d: qu[%.3f] bt[%lx] pos[%f %f %f]",
                   flystick[i].id, flystick[i].quality, flystick[i].bt,
                   flystick[i].loc[0], flystick[i].loc[1], flystick[i].loc[2];
  }
  for (int i=0; i<nmeatool && i<MAX_NMEATOOL; i++)
  {
    logger.debug() << "readLoop: tool   %02d: qu[%.3f] bt[%lx] pos[%f %f %f]",
                   meatool[i].id, meatool[i].quality, meatool[i].bt,
                   meatool[i].loc[0], meatool[i].loc[1], meatool[i].loc[2];
  }

  for (int i=0; i<nmarker && i<MAX_NMARKER; i++)
  {
    logger.debug() << "readLoop: marker %02d: qu[%.3f] pos[%f %f %f]",
                   marker[i].id, marker[i].quality,
                   marker[i].loc[0], marker[i].loc[1], marker[i].loc[2];
  }
}
#endif // #if 0
