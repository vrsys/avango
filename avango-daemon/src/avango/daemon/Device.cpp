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

#include <boost/bind.hpp>
#include <avango/Logger.h>
#include <avango/daemon/Device.h>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::Device"));
}

AV_BASE_DEFINE_ABSTRACT(av::daemon::Device);

av::daemon::Device::Device() :
  mKeepRunning(false),
  mEmptyFeature(std::string("")),
  mRunning(false)
{}

av::daemon::Device::~Device()
{}

void
av::daemon::Device::initClass()
{
  if (!isTypeInitialized())
  {
    av::Typed::initClass();
    AV_BASE_INIT_ABSTRACT(av::Typed, av::daemon::Device, true);
  }
}

void
av::daemon::Device::addStation(int station_number, Station* station)
{
  mStations.insert(NumStationMap::value_type(station_number, station));
}

bool
av::daemon::Device::startUp()
{
  if(mRunning) return false;

  if(mStations.empty()) throw std::runtime_error("No station attached to this device.");

  startDevice();
  mKeepRunning = true;
  mThread.reset(new Thread(boost::bind(&av::daemon::Device::threadFunction, this)));
  mRunning = true;
  return true;
}

bool
av::daemon::Device::shutDown()
{
  if (!mRunning) return false;

  mKeepRunning = false;

#ifdef WIN32
  // wait for thread to be closed
  mThread->join();
#endif

  stopDevice();

  return true;
}

bool
av::daemon::Device::isDeviceRunning()
{
  return mRunning;
}

void
av::daemon::Device::threadFunction(Device* device)
{
  device->readLoop();
  device->mRunning = false;
}

/* virtual */ std::string
av::daemon::Device::getStationName(int id)
{
  NumStationMap::iterator found = mStations.find(id);
  if (found != mStations.end())
    return found->second->getName();
  else
    return "";
}

/* virtual */ void
av::daemon::Device::configureFeature(const std::string& feature, const std::string& value)
{
  mFeatures[feature] = value;
}

/* virtual */ void
av::daemon::Device::unconfigureFeature(const std::string& feature)
{
  mFeatures.erase(feature);
}

/* virtual */ std::string
av::daemon::Device::queryFeature(const std::string& feature)
{
  StringStringMap::iterator i = mFeatures.find(feature);
  return (i != mFeatures.end()) ? (*i).second : mEmptyFeature;
}
