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

#if !defined(AV_DISPLAY_MT_DLL_MULTITOUCHDEVICE_H)
#define AV_DISPLAY_MT_DLL_MULTITOUCHDEVICE_H

#include <boost/thread.hpp>

#include <avango/FieldContainer.h>
#include <avango/osg/Fields.h>
#include <avango/TimeSensor.h>
#include <avango/display/mt/MultitouchFinger.h>
#include <avango/display/mt/windows_specific_devices.h>

#include "FingerClient.h"

namespace av
{
  namespace display
  {
    namespace mt
    {
      class AV_DISPLAY_MT_DLL MultitouchDevice : public FieldContainer, public FingerListener
      {
        AV_FC_DECLARE();

      public:

        MultitouchDevice();
        virtual ~MultitouchDevice();

        MFMultitouchFinger Added;
        MFMultitouchFinger Available;
        MFMultitouchFinger Removed;

        SFFloat TimeIn;

        SFBool Filtering;
        SFInt FilterSize;

        /* virtual */ void evaluate();

        // FingerListener interface
        void fingerAdded(FingerInfo *pFingerInfo);
        void fingerMoved(FingerInfo *pFingerInfo);
        void fingerRemoved(FingerInfo *pFingerInfo);

      private:

        FingerClient *mFingerClient;
        struct FingerAction
        {
          bool do_add;
          bool do_move;
          bool do_remove;
          int finger_id;
          Vec2 position;
		  float angle, width, height, area;
          Link<MultitouchFinger> finger;
          FingerAction(int _finger_id = -1) : do_add(false), do_move(false), do_remove(false), finger_id(_finger_id)
          {
          }
        };
        typedef std::map<int, FingerAction> FingerMap;
        FingerMap mFingerMap;
        boost::mutex mMutex;
      };
    }
  }
}

#endif

