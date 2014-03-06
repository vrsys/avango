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
#include <avango/daemon/Oculus.h>
#include <avango/daemon/Station.h>
#include <cstring>
#include <string>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::Oculus"));
}

AV_BASE_DEFINE(av::daemon::Oculus);

av::daemon::Oculus::Oculus()
  : mRequiredFeatures()
  , device_manager_(nullptr)
  , sensor_enumerator_()
  , sensor_fusions_()
{
}

av::daemon::Oculus::~Oculus()
{}

void
av::daemon::Oculus::initClass()
{
  if (!isTypeInitialized()) {
    av::daemon::Device::initClass();
    AV_BASE_INIT(av::daemon::Device, av::daemon::Oculus, true);
  }
}

/* virtual */ void
av::daemon::Oculus::startDevice()
{
  if (!parseFeatures())
    return;

  // configure
  logger.info() << "startDevice: device configured successfully";

  // initialize
  ::OVR::System::Init(::OVR::Log::ConfigureDefaultLog(::OVR::LogMask_All));
  device_manager_ = ::OVR::DeviceManager::Create();
  sensor_enumerator_ = device_manager_->EnumerateDevices< ::OVR::SensorDevice>();
  logger.info() << "startDevice: device initialized successfully";
}

/* virtual */ void
av::daemon::Oculus::readLoop()
{
  while (mKeepRunning) {
    
    // check if stations were added or removed
    if (sensor_fusions_.size() != mStations.size()) {

      logger.trace() << "readLoop: Oculus: number of stations changed to %d", mStations.size();

      // try to create new sensor device
      ::OVR::SensorDevice* sensor = sensor_enumerator_.CreateDevice();

      // check if a valid sensor device was returned
      if (!sensor) {
        logger.error() << "readLoop: Oculus: not enought OculusRift Sensors attached.";
      } else {

        // create and store sensor fusion of the sensor device
        ::OVR::SensorFusion* sensor_fusion = new ::OVR::SensorFusion();
        sensor_fusion->AttachToSensor(sensor);
        sensor_fusions_.push_back(sensor_fusion);

        logger.trace() << "readLoop: Oculus: successfully created new Oculus sensor";

        // go to the next device
        sensor_enumerator_.Next();
      }

    }


    // update station matrices
    for (unsigned int i = 0; i < sensor_fusions_.size(); ++i) {
      NumStationMap::iterator it = mStations.find(i);

      if (it != mStations.end()) {

        ::OVR::SensorFusion* sensor_fusion = sensor_fusions_[i];
        ::OVR::Quatf orient = sensor_fusion->GetPredictedOrientation();
        ::OVR::Matrix4f ovr_mat(orient.Inverted());

        it->second->setMatrix(::gua::math::mat4(ovr_mat.M[0][0], ovr_mat.M[0][1], ovr_mat.M[0][2], ovr_mat.M[0][3],
                                                ovr_mat.M[1][0], ovr_mat.M[1][1], ovr_mat.M[1][2], ovr_mat.M[1][3],
                                                ovr_mat.M[2][0], ovr_mat.M[2][1], ovr_mat.M[2][2], ovr_mat.M[2][3],
                                                ovr_mat.M[3][0], ovr_mat.M[3][1], ovr_mat.M[3][2], ovr_mat.M[3][3]));

        logger.trace() << "readLoop: OVR: matrix of station number %d set", i;

      } else {
        logger.debug() << "readLoop: Oculus: station number %d could not be found!", i;
      }
    }

  }
}

/* virtual */ void
av::daemon::Oculus::stopDevice()
{
  logger.info() << "stopDevice: done.";
}

const std::vector<std::string>&
av::daemon::Oculus::queryFeatures()
{
  return mRequiredFeatures;
}

bool
av::daemon::Oculus::parseFeatures()
{
  return true;
}
