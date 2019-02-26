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

#include <avango/daemon/Init.h>

#include <avango/Logger.h>

#include <avango/daemon/Config.h>
#include <avango/daemon/Device.h>
#include <avango/daemon/DeviceActuator.h>
#include <avango/daemon/DeviceDaemon.h>
#include <avango/daemon/DeviceSensor.h>
#include <avango/daemon/DeviceService.h>
#include <avango/daemon/DTrack.h>
#include <avango/daemon/KinectTrack.h>
#include <avango/daemon/SkeletonTrack.h>
#include <avango/daemon/HMDTrack.h>
#include <avango/daemon/HIDInput.h>
#include <avango/daemon/WacomTablet.h>
#include <avango/daemon/Wiimote.h>
#include <avango/daemon/WiimoteActuator.h>
#include <avango/daemon/TUIOInput.h>

#ifdef VRPN_SUPPORT
#include <avango/daemon/VRPNClient.h>
#endif

namespace
{
av::Logger& logger(av::getLogger("av::daemon::Init"));
}

AV_TYPED_DEFINE_ABSTRACT(av::daemon::Init);

/* static */ void av::daemon::Init::initClass()
{
    if(!isTypeInitialized())
    {
        av::daemon::Device::initClass();
        av::daemon::DeviceActuator::initClass();
        av::daemon::DeviceDaemon::initClass();
        av::daemon::DeviceSensor::initClass();
        av::daemon::DeviceService::initClass();
        av::daemon::DTrack::initClass();
        av::daemon::KinectTrack::initClass();
        av::daemon::SkeletonTrack::initClass();
        av::daemon::HMDTrack::initClass();
        av::daemon::TUIOInput::initClass();

        av::daemon::HIDInput::initClass();
#ifndef WIN32
        av::daemon::WacomTablet::initClass();
        av::daemon::Wiimote::initClass();
        av::daemon::WiimoteActuator::initClass();
#endif

#ifdef VRPN_SUPPORT
        av::daemon::VRPNClient::initClass();
#endif

        AV_TYPED_INIT_ABSTRACT(av::Type::badType(), "av::daemon::Init", true);
    }
}
