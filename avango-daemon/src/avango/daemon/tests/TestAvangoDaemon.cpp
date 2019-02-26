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

#include <avango/Application.h>
#include <avango/Logger.h>

#include <avango/UnitTest++/UnitTest++.h>

#include <avango/daemon/DeviceSensor.h>
#include <avango/daemon/DeviceService.h>
#include <avango/daemon/DTrack.h>
#include <avango/daemon/KinectTrack.h>
#include <avango/daemon/SkeletonTrack.h>
#include <avango/daemon/HMDTrack.h>
#include <avango/daemon/HIDInput.h>
#include <avango/daemon/Wiimote.h>

int main()
{
    av::getRootLogger().addConsoleAppender();
    av::ApplicationInstance::get();
    return UnitTest::RunAllTests();
}
