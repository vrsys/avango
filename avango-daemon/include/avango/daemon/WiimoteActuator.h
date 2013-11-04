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

#if !defined(AV_DAEMON_WIIMOTEACTUATOR_H)
#define AV_DAEMON_WIIMOTEACTUATOR_H


/**
 * \file
 * \ingroup av_daemon
 */

#include <avango/daemon/DeviceActuator.h>

namespace av
{
  namespace daemon
  {
    /**
     * Inherited from av::daemon::DeviceActuator. Immplements a
     * concrete actuator to set LED states and different Rumble modes
     * of an associated Nintendo Wiimote.
     *
     * \ingroup av_daemon
     */
    class WiimoteActuator : public DeviceActuator
    {

      AV_FC_DECLARE();

    public:

      WiimoteActuator();
      ~WiimoteActuator();

      /**
       * Set LEDs of the connected Wiimote.
       */
      SFBool   LED0;
      SFBool   LED1;
      SFBool   LED2;
      SFBool   LED3;

      /**
       * Set Rumble mode of the connected Wiimote.
       */
      SFBool   Rumble0;
      SFBool   Rumble1;
      SFBool   Rumble2;
      SFBool   Rumble3;
      SFBool   Rumble4;
      SFBool   Rumble5;
      SFBool   Rumble6;

      /**
       * Resets the state of all LEDs and Rumble modes to their initial values.
       */
      void reset();

      /* virtual */ void evaluate();
    };
  }
}

#endif
