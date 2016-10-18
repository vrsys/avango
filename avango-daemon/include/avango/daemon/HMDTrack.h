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

#if !defined(AV_DAEMON_HMD_TRACK_H)
#define AV_DAEMON_HMD_TRACK_H

#include <avango/daemon/Device.h>

/**
 * \file
 * \ingroup av_daemon
 */

namespace av
{
  namespace daemon
  {

    struct HMD_Controller{ // adapted for vive controller
      HMD_Controller() :
        status{false},
        id{-1},
        app_menu{false},
        grip{false},
        pad_touch{false},
        pad_press{false},
        trigger_p{false},
        pad_x{0.0},
        pad_y{0.0},
        trigger{0.0} 
      { };

      ::gua::math::mat4f matrix;
      bool status;
      short id;
      bool app_menu;  // Application Menu button on Vive controller
      bool grip;    // Grip button on Vive controller
      bool pad_touch; // touch/untouch on Touchpad
      bool pad_press; // Touchpad button
      bool trigger_p; // Trigger is fully pressed
      float pad_x;  // X values on touchpad
      float pad_y;  // Y values on touchpad
      float trigger;  // Trigger value
    };

    struct HMD_Tracker { // e.g. Lighthouse or Oculus tracking device
      HMD_Tracker() :
        status{false},
        id{-1}
      { };

      ::gua::math::mat4f matrix;
      bool status;
      short id;
    };

    struct HMD_Message {
      HMD_Message() :
        hmd_status{false}, 
        hmd_id{-1}
      { };
      
      bool hmd_status;
      short hmd_id;
      ::gua::math::mat4f hmd;
      std::array<HMD_Controller, 4> controller;
      std::array<HMD_Tracker, 2> tracker;
    };

    class AV_DAEMON_DLL HMDTrack : public Device
    {
      AV_BASE_DECLARE();

    public:
      /**
       * Constructor
       */
      HMDTrack();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~HMDTrack() {}

      /**
       * Inherited from base class, implements the initialization of this device.
       */
      void startDevice() override;

      /**
       * Inherited from base class, implements the loop in which the device is read out.
       */
      void readLoop() override;

      /**
       * Inherited from base class, implements the closing operation of this device.
       */
      void stopDevice() override;

      /**
       * Inherited from base class, returns a list of settable features.
       */
      const std::vector<std::string>& queryFeatures() override;

    private:

      ::std::vector< ::std::string> mRequiredFeatures;
      std::string mPort;
      std::string mServer;
      bool parseFeatures();
    };
  }
}

#endif