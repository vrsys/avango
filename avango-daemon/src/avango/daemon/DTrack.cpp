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

#include "DTrackSDK/DTrackSDK.hpp"
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
}

AV_BASE_DEFINE(av::daemon::DTrack);

av::daemon::DTrack::DTrack() : mDTrackSDK(nullptr) { mRequiredFeatures.push_back("port"); }

av::daemon::DTrack::~DTrack() {}

void av::daemon::DTrack::initClass()
{
    if(!isTypeInitialized())
    {
        av::daemon::Device::initClass();
        AV_BASE_INIT(av::daemon::Device, av::daemon::DTrack, true);
    }
}

/* virtual */ void av::daemon::DTrack::startDevice()
{
    if(!parseFeatures())
    {
        return;
    }

    if(mPort <= 0 || mPort >= 65536)
    {
        logger.error() << "Invalid port for DTrackSDK";
        stopDevice();
        return;
    }

    mDTrackSDK = std::shared_ptr<::DTrackSDK>(new ::DTrackSDK(mPort));

    if(!mDTrackSDK->isLocalDataPortValid())
    {
        logger.error() << "DTrackSDK initialization error";
        return;
    }

    logger.info() << "startDevice: device initialized successfully";
}

/* virtual */ void av::daemon::DTrack::readLoop()
{
    if(mDTrackSDK != nullptr)
    {
        while(mKeepRunning)
        {
            if(mDTrackSDK->receive())
            {
                // success in receiving data
                DTrack_Body_Type_d body;

                for(unsigned int bodyId = 0; bodyId < mDTrackSDK->getNumBody(); ++bodyId)
                {
                    NumStationMap::iterator it = mStations.find(bodyId + 1);

                    if(it != mStations.end())
                    {
                        body = *mDTrackSDK->getBody(bodyId);

                        if(body.quality >= 0)
                        {
                            ::gua::math::mat4 xform(body.rot[0],
                                                    body.rot[1],
                                                    body.rot[2],
                                                    0.0f,
                                                    body.rot[3],
                                                    body.rot[4],
                                                    body.rot[5],
                                                    0.0f,
                                                    body.rot[6],
                                                    body.rot[7],
                                                    body.rot[8],
                                                    0.0f,
                                                    body.loc[0] * 0.001f,
                                                    body.loc[1] * 0.001f,
                                                    body.loc[2] * 0.001f,
                                                    1.0f);
                            it->second->setMatrix(xform);
                        }
                    }
                }
            }
            else
            {
                // error in receiving data
                if(mDTrackSDK->getLastDataError() == DTrackSDK::ERR_TIMEOUT)
                {
                    logger.info() << "readLoop: timeout while waiting for tracking data";
                }

                if(mDTrackSDK->getLastDataError() == DTrackSDK::ERR_NET)
                {
                    logger.info() << "readLoop: error while receiving tracking data";
                }

                if(mDTrackSDK->getLastDataError() == DTrackSDK::ERR_PARSE)
                {
                    logger.info() << "readLoop: error while parsing tracking data";
                }
            }
        }
    }
}

/* virtual */ void av::daemon::DTrack::stopDevice() { logger.info() << "stopDevice: device successfully stopped"; }

unsigned long int av::daemon::DTrack::convert_charpointer_to_ulong(const char* arg) const
{
    unsigned long int result = 0;
    errno = 0;

    if(arg)
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

        if(errno)
        {
            logger.warn() << "convert_charpointer_to_ulong: unable to convert arg '%s' to int; errno: '%s' ", arg, std::strerror(errno);
            result = 0;
        }
        else
            LOG_TRACE(logger) << "convert_charpointer_to_ulong: converted arg '%s' to %ld", arg, result;
    }
    else
        logger.warn() << "convert_charpointer_to_ulong: got null pointer for arg conversion";

    return result;
}

const std::vector<std::string>& av::daemon::DTrack::queryFeatures() { return mRequiredFeatures; }

bool av::daemon::DTrack::parseFeatures()
{
    std::string port(queryFeature("port"));
    if(port == "")
    {
        logger.warn() << "parseFeatures: feature 'port' not specified";
        return false;
    }
    else
    {
        logger.info() << "parseFeatures: configured feature 'port' = %s", port;
        mPort = convert_charpointer_to_ulong(port.c_str());
    }

    return true;
}