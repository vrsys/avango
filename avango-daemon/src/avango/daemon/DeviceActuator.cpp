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

#include <avango/daemon/DeviceActuator.h>

#include <avango/SingleField.h>
#include <avango/Logger.h>


namespace
{
  av::Logger& logger(av::getLogger("av::daemon::DeviceActuator"));
}

AV_FC_DEFINE(av::daemon::DeviceActuator);

av::daemon::DeviceActuator::DeviceActuator()
{
  AV_FC_ADD_FIELD(DeviceService, 0);
  AV_FC_ADD_FIELD(Station,       ::std::string());
}

av::daemon::DeviceActuator::~DeviceActuator()
{}

void
av::daemon::DeviceActuator::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();
    AV_FC_INIT(av::FieldContainer, av::daemon::DeviceActuator, true);
  }
}

/* virtual */ void
av::daemon::DeviceActuator::fieldHasChanged(const av::Field& field)
{
  if (&field == &DeviceService)
    mService = DeviceService.getValue();
  else if (&field == &Station)
    mStation = Station.getValue();
}
