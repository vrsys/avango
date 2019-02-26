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

#include <avango/UnitTest++/UnitTest++.h>

#include <utility>

#include <avango/daemon/DeviceSensor.h>
#include <avango/daemon/DeviceService.h>

using namespace av::daemon;

namespace
{
TEST(ServiceCanBeInitialized)
{
    av::daemon::DeviceService::initClass();
    CHECK(av::Type::getByName("av::daemon::DeviceService") != av::Type::badType());
}
TEST(SensorCanBeInitialized)
{
    av::daemon::DeviceSensor::initClass();
    CHECK(av::Type::getByName("av::daemon::DeviceSensor") != av::Type::badType());
}
} // namespace
