// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       *
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


#if !defined(AV_DAEMON_VRPNCLIENT_H)
#define AV_DAEMON_VRPNCLIENT_H

/**
 * \file
 * \ingroup av_daemon
 */

#include <vrpn_Analog.h>
#include <vrpn_Button.h>
#include <vrpn_Dial.h>
#include <vrpn_Tracker.h>

#include <avango/daemon/Device.h>

namespace av
{
  namespace daemon
  {
    /**
     * An Avango NG device for processing packets sent by a VRPN server.
     *
     * \ingroup av_daemon
     */
    class VRPNClient : public Device
    {
      AV_BASE_DECLARE();

    public:
      /**
       * Constructor
       */
      VRPNClient();

      /*
       * Callback handler for analog devices, such as joysticks.
       */
      void handleAnalog(const vrpn_ANALOGCB a);

      /*
       * Callback handler for button devices.
       */
      void handleButton(const vrpn_BUTTONCB b);

      /**
       * Callback handler for tracking data (positions, orientations, accelerations, etc.)
       */
      void handleTracker(const vrpn_TRACKERCB t);

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~VRPNClient();

      /**
       * Inherited from base class, implements the initialization of this device.
       */
      void startDevice();

      /**
       * Inherited from base class, implements the main loop for processing data.
       */
      void readLoop();

      /**
       * Inherited from base class, implements the closing operation of this device.
       */
      void stopDevice();

      /**
       * Inherited from base class, returns a list of settable features.
       */
      const std::vector<std::string>& queryFeatures();
      
      /**
       * Before the device is started this method ensures that the required and
       * optional features are set properly. In case of an error the device will
       * not start.
       */
      bool parseFeatures();

    private:

      std::string          mVRPNServer;
      std::vector<std::string> mRequiredFeatures;
      bool                 mStopped;

      vrpn_Analog_Remote*  mVRPNAnalog;
      vrpn_Button_Remote*  mVRPNButton;
      vrpn_Tracker_Remote* mVRPNTracker;
    };
  }
}

#endif


