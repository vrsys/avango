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

#include <linux/input.h>

#include <avango/SingleField.h>
#include <avango/Logger.h>
#include <avango/daemon/WiimoteActuator.h>
#include <avango/daemon/DeviceService.h>

namespace
{
av::Logger& logger(av::getLogger("av::daemon::WiimoteActuator"));
}

AV_FC_DEFINE(av::daemon::WiimoteActuator);

av::daemon::WiimoteActuator::WiimoteActuator()
{
    AV_FC_ADD_FIELD(LED0, false);
    AV_FC_ADD_FIELD(LED1, false);
    AV_FC_ADD_FIELD(LED2, false);
    AV_FC_ADD_FIELD(LED3, false);
    AV_FC_ADD_FIELD(Rumble0, false);
    AV_FC_ADD_FIELD(Rumble1, false);
    AV_FC_ADD_FIELD(Rumble2, false);
    AV_FC_ADD_FIELD(Rumble3, false);
    AV_FC_ADD_FIELD(Rumble4, false);
    AV_FC_ADD_FIELD(Rumble5, false);
    AV_FC_ADD_FIELD(Rumble6, false);
}

av::daemon::WiimoteActuator::~WiimoteActuator() { reset(); }

void av::daemon::WiimoteActuator::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();
        AV_FC_INIT(av::FieldContainer, av::daemon::WiimoteActuator, true);
    }
}

/* virtual */ void av::daemon::WiimoteActuator::evaluate()
{
    if(mService.isValid() && !(mStation.empty()))
    {
        const char* stationstr = mStation.c_str();

        // set LED states
        mService->setLED(stationstr, LED_NUML, LED0.getValue());
        mService->setLED(stationstr, LED_CAPSL, LED1.getValue());
        mService->setLED(stationstr, LED_SCROLLL, LED2.getValue());
        mService->setLED(stationstr, LED_COMPOSE, LED3.getValue());

        // set rumble modes
        mService->setLED(stationstr, LED_SLEEP, Rumble0.getValue());
        mService->setLED(stationstr, LED_SUSPEND, Rumble1.getValue());
        mService->setLED(stationstr, LED_MUTE, Rumble2.getValue());
        mService->setLED(stationstr, LED_MISC, Rumble3.getValue());
        mService->setLED(stationstr, LED_MAIL, Rumble4.getValue());
        mService->setLED(stationstr, LED_CHARGING, Rumble5.getValue());
        mService->setLED(stationstr, LED_MAX, Rumble6.getValue());
    }
}

void av::daemon::WiimoteActuator::reset()
{
    if(mService.isValid() && !(mStation.empty()))
    {
        logger.info() << "reset: reset LED and Rumble states";
        const char* stationstr = mStation.c_str();

        mService->setLED(stationstr, LED_NUML, false);
        mService->setLED(stationstr, LED_CAPSL, false);
        mService->setLED(stationstr, LED_SCROLLL, false);
        mService->setLED(stationstr, LED_COMPOSE, false);

        mService->setLED(stationstr, LED_SLEEP, false);
        mService->setLED(stationstr, LED_SUSPEND, false);
        mService->setLED(stationstr, LED_MUTE, false);
        mService->setLED(stationstr, LED_MISC, false);
        mService->setLED(stationstr, LED_MAIL, false);
        mService->setLED(stationstr, LED_CHARGING, false);
        mService->setLED(stationstr, LED_MAX, false);
    }
}
