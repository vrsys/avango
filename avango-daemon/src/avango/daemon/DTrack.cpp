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

#include "dtrack/dtrack.h"
#include <errno.h>
#include <gua/math.hpp>
#include <avango/Logger.h>
#include <avango/daemon/DTrack.h>
#include <avango/daemon/Station.h>
#include <cstring>
#include <string>



namespace
{
  av::Logger& logger(av::getLogger("av::daemon::DTrack"));

  const size_t default_udp_bufsize = 10000;
  const size_t default_udp_timeout = 1000000; // 1 sec
}

AV_BASE_DEFINE(av::daemon::DTrack);

av::daemon::DTrack::DTrack() :
  mDTrack(new ::DTrack)
{
  mRequiredFeatures.push_back("port");
  mRequiredFeatures.push_back("timeout");
}

av::daemon::DTrack::~DTrack()
{}

void
av::daemon::DTrack::initClass()
{
  if (!isTypeInitialized())
  {
    av::daemon::Device::initClass();
    AV_BASE_INIT(av::daemon::Device, av::daemon::DTrack, true);
  }
}

/* virtual */ void
av::daemon::DTrack::startDevice()
{
  if (!parseFeatures())
    return;

  logger.info() << "startDevice: device configured successfully";

  // initialize
  dtrack_init_type ini;

  ini.udpport = mPort;
  ini.udpbufsize = default_udp_bufsize;
  ini.udptimeout_us = mTimeout;

  std::strcpy(ini.remote_ip, "");
  ini.remote_port = 0;

  if (int err = mDTrack->init(&ini))
  {
    logger.error() << "startDevice: dtrack init error: %d", err;
    stopDevice();
    return;
  }
  else
  {
    logger.info() << "startDevice: enabling cameras and calculation";
    mDTrack->send_udp_command(DTRACK_CMD_CAMERAS_AND_CALC_ON, 1);
  }

  logger.info() << "startDevice: device initialized successfully";
}

/* virtual */ void
av::daemon::DTrack::readLoop()
{
  unsigned long     framenr;
  double            timestamp;
  int               nbodycal;
  int               nbody;
  dtrack_body_type* body = 0;
  unsigned int      max_bodies = 0;
  int               dummy;
  unsigned int      max_station_id = 1;

  while (mKeepRunning)
  {
    using station_map_it = NumStationMap::const_iterator;

    for (station_map_it current = mStations.begin(); current != mStations.end(); ++current)
    {
      if (static_cast<unsigned>(current->first) > max_station_id)
        max_station_id = (*current).first;
    }

    if (max_bodies < max_station_id)
    {
      if (body)
        ::free(body);

      max_bodies = max_station_id;
      body = (dtrack_body_type*) ::malloc(sizeof(dtrack_body_type) * max_bodies);
    }

    const int err = mDTrack->receive_udp_ascii(&framenr, &timestamp, &nbodycal,
                                              &nbody, body, max_bodies,
                                              &dummy, 0,    0,
                                              &dummy, 0,    0,
                                              &dummy, 0,    0);
    if (DTRACK_ERR_NONE != err)
    {
      std::string msg;

      switch (err)
      {
        case DTRACK_ERR_UDP:     msg = "error handling the udp socket"; break;
        case DTRACK_ERR_MEM:     msg = "error handling the udp buffer"; break;
        case DTRACK_ERR_TIMEOUT: msg = "timeout while receiving data"; break;
        case DTRACK_ERR_CMD:     msg = "error while sending remote command"; break;
        case DTRACK_ERR_PCK:     msg = "error in udp packet"; break;
      }

      logger.error() << "readLoop: dtrack: '%s'", msg.c_str();
      continue;
    }
    else
    {
      const unsigned int body_limit = std::min(static_cast<unsigned> (nbody), max_bodies);

      for (unsigned int i = 0; i < body_limit; ++i)
      {
        logger.trace() << "readLoop: detected body with ID = %s", body[i].id;
        const int body_idx = body[i].id;
        NumStationMap::iterator it = mStations.find(body_idx + 1);

        if (it != mStations.end())
        {
          ::gua::math::mat4 xform (body[i].rot[0], body[i].rot[1], body[i].rot[2], 0.0f,
                                   body[i].rot[3], body[i].rot[4], body[i].rot[5], 0.0f,
                                   body[i].rot[6], body[i].rot[7], body[i].rot[8], 0.0f,
                                   body[i].loc[0] * 0.001f, body[i].loc[1] * 0.001f, body[i].loc[2] * 0.001f, 1.0f);

          it->second->setMatrix(xform);
          logger.trace() << "readLoop: set matrix of station number '%s'", body_idx + 1;
        }
        else logger.debug() << "readLoop: can't find station for body #%d (station not configured?)", body_idx;
      }
    }
  }
}

/* virtual */ void
av::daemon::DTrack::stopDevice()
{
  mDTrack->exit();
  logger.info() << "stopDevice: done.";
}

unsigned long int
av::daemon::DTrack::convert_charpointer_to_ulong(const char* arg) const
{
  unsigned long int result = 0;
  errno = 0;

  if (arg)
  {
    // man strtoul:
    // ...
    // The string must begin with an arbitrary amount of white space (as determined by isspace(3))
    // followed by a single optional '+' or '-' sign.  If base is zero  or 16, the string may then
    // include a '0x' prefix, and the number will be read in base 16; otherwise, a zero base is
    // taken as 10 (decimal) unless the next character is '0', in which case it is taken as 8
    // (octal).
    static int base_magic(0);

    result = ::strtoul(arg, 0, base_magic);

    if (errno)
    {
      logger.warn() << "convert_charpointer_to_ulong: unable to convert arg '%s' to int; errno: '%s' ",
                 arg, std::strerror(errno);
      result = 0;
    }
    else
      LOG_TRACE(logger) << "convert_charpointer_to_ulong: converted arg '%s' to %ld", arg, result;

  }
  else
    logger.warn() << "convert_charpointer_to_ulong: got null pointer for arg conversion";

  return result;
}

const std::vector<std::string>&
av::daemon::DTrack::queryFeatures()
{
  return mRequiredFeatures;
}

bool
av::daemon::DTrack::parseFeatures()
{
  std::string port(queryFeature("port"));
  if (port == "")
  {
    logger.warn() << "parseFeatures: feature 'port' not specified";
    return false;
  }
  else
  {
    logger.info() << "parseFeatures: configured feature 'port' = %s", port;
    mPort = convert_charpointer_to_ulong(port.c_str());
  }

  std::string timeout(queryFeature("timeout"));
  if (timeout == "")
  {
    logger.info() << "parseFeatures: feature 'timeout' not specified; using %d usec.",default_udp_timeout;
    mTimeout = default_udp_timeout;
  }
  else mTimeout = convert_charpointer_to_ulong(timeout.c_str());

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
