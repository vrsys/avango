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

#include <avango/Logger.h>
#include <avango/daemon/Wiimote.h>

namespace
{
av::Logger& logger(av::getLogger("av::daemon::Wiimote"));
}

AV_BASE_DEFINE(av::daemon::Wiimote);

av::daemon::Wiimote::Wiimote() {}

av::daemon::Wiimote::~Wiimote() {}

void av::daemon::Wiimote::initClass()
{
    if(!isTypeInitialized())
    {
        av::daemon::HIDInput::initClass();
        AV_BASE_INIT(av::daemon::HIDInput, av::daemon::Wiimote, true);
    }
}

void av::daemon::Wiimote::startDevice()
{
    // auto-configure LED and Rumble features
    std::string stationLED = getFirstStation() + "::STATION_LED::";
    configureFeature(stationLED + "0", "EV_LED::LED_NUML");    // LED 0
    configureFeature(stationLED + "1", "EV_LED::LED_CAPSL");   // LED 1
    configureFeature(stationLED + "2", "EV_LED::LED_SCROLLL"); // LED 2
    configureFeature(stationLED + "3", "EV_LED::LED_COMPOSE"); // LED 3

    configureFeature(stationLED + "5", "EV_LED::LED_SLEEP");     // Rumble 0
    configureFeature(stationLED + "6", "EV_LED::LED_SUSPEND");   // Rumble 1
    configureFeature(stationLED + "7", "EV_LED::LED_MUTE");      // Rumble 2
    configureFeature(stationLED + "8", "EV_LED::LED_MISC");      // Rumble 3
    configureFeature(stationLED + "9", "EV_LED::LED_MAIL");      // Rumble 4
    configureFeature(stationLED + "10", "EV_LED::LED_CHARGING"); // Rumble 5
    configureFeature(stationLED + "11", "EV_LED::LED_SLEEP");    // Rumble 6
}

/* virtual */ void av::daemon::Wiimote::updateLED(unsigned short eventCode, bool on)
{
    // be sure to have correct virtual rumble LED states in linux input system
    if(on)
    {
        switch(eventCode)
        {
        case LED_SLEEP:
        case LED_SUSPEND:
        case LED_MUTE:
        case LED_MISC:
#if defined(LED_MAIL)
        case LED_MAIL:
#endif
#if defined(LED_CHARGING)
        case LED_CHARGING:
#endif
        case LED_MAX:
            HIDInput::updateLED(eventCode, false);
        }
    }
    HIDInput::updateLED(eventCode, on);
}

/* virtual */ void av::daemon::Wiimote::stopLEDs()
{
    HIDInput::stopLEDs();

    // enable flashing
    updateLED(LED_KANA, true);

    // disable rumble
    updateLED(LED_SLEEP, false);
    updateLED(LED_SUSPEND, false);
    updateLED(LED_MUTE, false);
    updateLED(LED_MISC, false);
#if defined(LED_MAIL)
    updateLED(LED_MAIL, false);
#endif
#if defined(LED_CHARGING)
    updateLED(LED_CHARGING, false);
#endif
    updateLED(LED_MAX, false);
}
