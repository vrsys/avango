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

#if !defined(AV_DAEMON_STATION_H)
#define AV_DAEMON_STATION_H

/**
 * \file
 * \ingroup av_daemon
 */

#include <vector>
#include <gua/math/math.hpp>
#include <avango/Base.h>
#include <avango/daemon/windows_specific_daemon.h>

namespace av
{
  namespace daemon
  {
    /**
     * Class representing a station of Avango NG daemon.
     *
     * \ingroup av_daemon
     */
    class AV_DAEMON_DLL Station : public Base {

      AV_BASE_DECLARE();

    public:

      /**
       * Constructor
       */
      Station();

      /**
       * Destructor
       */
      virtual ~Station();

    public:

      const char* getName() const;
      const ::gua::math::mat4& getMatrix() const;

      int getButton(int which) const;
      const ::std::vector<int> getButtons() const;

      float getValue(int which) const;
      const ::std::vector<float> getValues() const;

      bool getLED(int which) const;
      const ::std::vector<bool> getLEDs() const;

      void setName(const char* name);
      void setMatrix(const ::gua::math::mat4& matrix);
      void setButton(int which, int on);
      void setValue(int which, float val);
      void setLED(int which, bool on);
      void setUsage(bool matrix = 0, int buttons = 0, int values = 0, int leds = 0);

      bool getMatrixUsed() const;
      int  getButtonsUsed() const;
      int  getValuesUsed() const;
      int  getLEDsUsed() const;

      static const int sMaxNameLength = 128;
      static const int sMaxButtons = 32;
      static const int sMaxValues = 256;
      static const int sMaxLeds = 16;

    private:

      // retain static memory layout since it gets put into a shared
      // memory segment not autogrowable!
      char     mName[sMaxNameLength];
      ::gua::math::mat4 mMatrix;               // this should work since fpMatrix has the same memory layout float[4][4]
      int      mButton[sMaxButtons];
      float    mValue[sMaxValues];
      bool     mLed[sMaxLeds];
      bool     mMatrixUsed;
      int      mButtonsUsed;
      int      mValuesUsed;
      int      mLedsUsed;

      /**
       * Made private to prevent copying construction.
       */
      Station(const Station&);

      /**
       * Made private to prevent assignment.
       */
      const Station& operator=(const Station&);

    };
  }
}


#endif // #if !defined(AV_OSG_STATION_H)
