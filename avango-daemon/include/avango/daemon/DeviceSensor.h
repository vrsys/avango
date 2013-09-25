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

#if !defined(AV_DAEMON_DEVICESENSOR_H)
#define AV_DAEMON_DEVICESENSOR_H


/**
 * \file
 * \ingroup av_daemon
 */

#include <string>

#include <avango/osg/Fields.h>
#include <avango/FieldContainer.h>
#include <avango/daemon/DeviceService.h>

namespace av
{
  namespace daemon
  {
    /**
     * Communicates with DeviceService and provides input data
     * of the device the associated DeviceService is connected with.
     * Depending on how the corresponding device is configured
     * the data is provided via fields for Values, Buttons, LEDs and
     * a Matrix.
     *
     * Note: A sensor can only be used to readout input data. Setting
     * field values by hand will have no effect. To send commands
     * to a device you should use an actuator.
     *
     * \ingroup av_daemon
     */
    class AV_DAEMON_DLL DeviceSensor : public FieldContainer
    {

      AV_FC_DECLARE();

    public:

      DeviceSensor();
      virtual ~DeviceSensor();

      /**
       * Name of device service to communicate with.
       */
      SFDeviceService DeviceService;

      /**
       * Name of station to connect with.
       */
      SFString Station;

      /**
       * Reset station values to '0' after read.
       */
      SFBool   ResetValuesOnRead;

      /**
       * Transformation matrix provided by given station (read-only).
       */
      av::osg::SFMatrix  Matrix;

      /**
       * Read-only fields: Button states of connected station.
       */
      SFBool   Button0;
      SFBool   Button1;
      SFBool   Button2;
      SFBool   Button3;
      SFBool   Button4;
      SFBool   Button5;
      SFBool   Button6;
      SFBool   Button7;
      SFBool   Button8;
      SFBool   Button9;
      SFBool   Button10;
      SFBool   Button11;
      SFBool   Button12;
      SFBool   Button13;
      SFBool   Button14;
      SFBool   Button15;
      SFBool   Button16;
      SFBool   Button17;
      SFBool   Button18;
      SFBool   Button19;
      SFBool   Button20;
      SFBool   Button21;
      SFBool   Button22;
      SFBool   Button23;
      SFBool   Button24;
      SFBool   Button25;
      SFBool   Button26;
      SFBool   Button27;
      SFBool   Button28;
      SFBool   Button29;
      SFBool   Button30;
      SFBool   Button31;

      /**
       * Read-only fields: Value states of connected station.
       */
      SFFloat  Value0;
      SFFloat  Value1;
      SFFloat  Value2;
      SFFloat  Value3;
      SFFloat  Value4;
      SFFloat  Value5;
      SFFloat  Value6;
      SFFloat  Value7;
      SFFloat  Value8;
      SFFloat  Value9;
      SFFloat  Value10;
      SFFloat  Value11;
      SFFloat  Value12;
      SFFloat  Value13;
      SFFloat  Value14;
      SFFloat  Value15;

      /**
       * Read-only fields: LED states of connected station.
       */
      SFBool   LED0;
      SFBool   LED1;
      SFBool   LED2;
      SFBool   LED3;
      SFBool   LED4;
      SFBool   LED5;
      SFBool   LED6;
      SFBool   LED7;
      SFBool   LED8;
      SFBool   LED9;
      SFBool   LED10;
      SFBool   LED11;
      SFBool   LED12;
      SFBool   LED13;
      SFBool   LED14;
      SFBool   LED15;

      /**
       * Tracker to world transformation.
       */
      av::osg::SFMatrix TransmitterOffset;

      /**
       * Sensor to device transformation.
       */
      av::osg::SFMatrix ReceiverOffset;

      /**
       * Device rotation (read-only).
       */
      av::osg::SFQuat   Rotation;

      /**
       * Device translation (read-only).
       */
      av::osg::SFVec3   Translation;

      /* virtual */ void evaluate();

    private:

      /**
       * Update matrix if changed.
       */
      void updateMatrix();

      /**
       * Update button states if changed.
       */
      void updateButtons();

      /**
       * Update value states if changed.
       */
      void updateValues();

      /**
       * Update LED states if changed.
       */
      void updateLEDs();

      /**
       * Readout Matrix provided by a specific device.
       */
      void getMatrix(::osg::Matrixf&);

      /**
       * Readout state of a specified Button provided by a specific device station.
       */
      bool getButton(int b);

      /**
       * Readout state of a specified LED provided by a specific device station.
       */
      bool getLED(int l);

      /**
       * Readout state of a specified Value provided by a specific device station.
       */
      float getValue(int v);

      /**
       * Set state of a specified LED.
       */
      void setLED(int l, bool val);

      static const int sMaxButtons = 32;
      static const int sMaxLEDs = 16;
      static const int sMaxValues = 16;

    };
  }
}

#endif
