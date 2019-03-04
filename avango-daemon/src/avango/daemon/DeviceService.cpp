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

// includes, system
#include <algorithm>
#include <cstdlib>
#include <list>

// includes, project
#include <avango/daemon/DeviceService.h>
#include <avango/daemon/Station.h>
#include <avango/daemon/StationSegment.h>
#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::daemon::DeviceService"));

using station_segment_list_type = std::list<av::daemon::StationSegment*>;
station_segment_list_type station_segment_list;

extern "C" void remove_remaining_station_segments()
{
    while(!station_segment_list.empty())
    {
        logger.debug() << "remove_remaining_station_segments: destructing 'fpStationSegment' @" << static_cast<const void*>(*(station_segment_list.begin()));

        delete *(station_segment_list.begin());
        station_segment_list.pop_front();
    }
}
} // namespace

AV_BASE_DEFINE(av::daemon::DeviceService);

AV_FIELD_DEFINE(av::daemon::SFDeviceService);
AV_FIELD_DEFINE(av::daemon::MFDeviceService);

av::daemon::DeviceService::DeviceService() : mIdentityMatrix(new ::gua::math::mat4), mCachedStationName(""), mCachedStation(nullptr), mStationSegment(0) { connectDaemon(); }

/* virtual */
av::daemon::DeviceService::~DeviceService() { disconnectDaemon(); }

/* static */ void av::daemon::DeviceService::initClass()
{
    if(!isTypeInitialized())
    {
        Base::initClass();
        AV_BASE_INIT(av::Base, av::daemon::DeviceService, true);
    }
}

void av::daemon::DeviceService::connectDaemon()
{
    disconnectDaemon();
    mStationSegment = new StationSegment;
    station_segment_list.push_back(mStationSegment);
}

void av::daemon::DeviceService::disconnectDaemon()
{
    station_segment_list.remove(mStationSegment);
    delete mStationSegment;
    mStationSegment = 0;
    clearStationCache();
}

void av::daemon::DeviceService::clearStationCache()
{
    mCachedStationName = "";
    mCachedStation = nullptr;
}

av::daemon::Station* av::daemon::DeviceService::lookupCachedStation(const char* station_name)
{
    if(!std::string(station_name).compare(mCachedStationName))
    {
        logger.trace() << "lookupCachedStation(): " << this << " cache hit for station " << station_name;
        return mCachedStation;
    }
    else if(mStationSegment)
    {
        logger.trace() << "lookupCachedStation(): " << this << " cache miss for station " << station_name;
        Station* station = mStationSegment->getStation(station_name);

        if(station)
        {
            mCachedStationName = std::string(station_name);
            mCachedStation = station;
            return station;
        }
        else
        {
            clearStationCache();
            logger.warn() << "lookupCachedStation(): station " << station_name << " not found; cache cleared.";
            return 0;
        }
    }
    else
    {
        logger.warn() << "lookupCachedStation(): no station segment initialized.";
    }

    return 0;
}

const gua::math::mat4& av::daemon::DeviceService::getMatrix(const char* station_name)
{
    if(mStationSegment)
    {
        const Station* station = lookupCachedStation(station_name);

        if(station)
        {
            logger.trace() << "getMatrix(): " << this << " succeeded for station " << station_name;
            return station->getMatrix();
        }
        else
            logger.warn() << "getMatrix(): " << this << " failed for station " << station_name << " (no station)";
    }
    else
        logger.warn() << "getMatrix(): " << this << " failed for station " << station_name << " (no segment)";

    return *mIdentityMatrix;
}

int av::daemon::DeviceService::getButton(const char* station_name, int which)
{
    if(mStationSegment)
    {
        const Station* station = lookupCachedStation(station_name);

        if(station)
            return station->getButton(which);
    }
    else
        logger.warn() << "getButton(): no station segment initialized.";

    return 0;
}

float av::daemon::DeviceService::getValue(const char* station_name, int which)
{
    if(mStationSegment)
    {
        const Station* station = lookupCachedStation(station_name);

        if(station)
            return station->getValue(which);
    }
    else
        logger.warn() << "getValue(): no station segment initialized.";

    return 0.0f;
}

bool av::daemon::DeviceService::getLED(const char* station_name, int which)
{
    if(mStationSegment)
    {
        const Station* station = lookupCachedStation(station_name);

        if(station)
            return station->getLED(which);
    }
    else
        logger.warn() << "getLED(): no station segment initialized.";

    return false;
}

void av::daemon::DeviceService::setMatrix(const char* station_name, const ::gua::math::mat4& value)
{
    if(mStationSegment)
    {
        Station* station = lookupCachedStation(station_name);

        if(station)
        {
            station->setMatrix(value);
            logger.info() << "setMatrix(): " << this << " succeeded for station " << station_name;
        }
        else
            logger.warn() << "setMatrix(): " << this << " failed for station " << station_name << " (no station)";
    }
    else
        logger.warn() << "setMatrix(): " << this << " failed for station " << station_name << " (no segment)";
}

void av::daemon::DeviceService::setButton(const char* station_name, int which, bool value)
{
    if(mStationSegment)
    {
        Station* station = lookupCachedStation(station_name);

        if(station)
            station->setButton(which, value);
    }
    else
        logger.warn() << "setButton(): no station segment initialized.";
}

void av::daemon::DeviceService::setValue(const char* station_name, int which, float value)
{
    if(mStationSegment)
    {
        Station* station = lookupCachedStation(station_name);

        if(station)
            station->setValue(which, value);
    }
    else
        logger.warn() << "setValue(): no station segment initialized.";
}

void av::daemon::DeviceService::setLED(const char* station_name, int which, bool value)
{
    if(mStationSegment)
    {
        Station* station = lookupCachedStation(station_name);

        if(station)
            station->setLED(which, value);
    }
    else
        logger.warn() << "setLED(): no station segment initialized.";
}

bool av::daemon::DeviceService::getMatrixUsed(const char* station_name)
{
    if(mStationSegment)
    {
        const Station* station = lookupCachedStation(station_name);

        if(station)
            return station->getMatrixUsed();
    }
    else
        logger.warn() << "getMatrixUsed(): no station segment initialized.";

    return false;
}

int av::daemon::DeviceService::getButtonsUsed(const char* station_name)
{
    if(mStationSegment)
    {
        const Station* station = lookupCachedStation(station_name);

        if(station)
            return station->getButtonsUsed();
    }
    else
        logger.warn() << "getButtonsUsed(): no station segment initialized.";

    return 0;
}

int av::daemon::DeviceService::getValuesUsed(const char* station_name)
{
    if(mStationSegment)
    {
        const Station* station = lookupCachedStation(station_name);

        if(station)
            return station->getValuesUsed();
    }
    else
        logger.warn() << "getValuesUsed(): no station segment initialized.";

    return 0;
}

int av::daemon::DeviceService::getLEDsUsed(const char* station_name)
{
    if(mStationSegment)
    {
        const Station* station = lookupCachedStation(station_name);

        if(station)
            return station->getLEDsUsed();
    }
    else
        logger.warn() << "getLEDsUsed(): no station segment initialized.";

    return 0;
}
