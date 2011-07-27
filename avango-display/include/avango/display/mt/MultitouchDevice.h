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

#ifdef __linux__
#include <sys/time.h>
#endif




namespace av
{
  namespace display
  {
    namespace mt
    {
      class AV_DISPLAY_MT_DLL MultitouchDevice : public FieldContainer, public FingerListener, public UserListener
      {
        AV_FC_DECLARE();

      public:

        MultitouchDevice();
        virtual ~MultitouchDevice();

        MFMultitouchFinger Added;
        MFMultitouchFinger Available;
        MFMultitouchFinger Removed;

		MFMultitouchUser AddedUsers;
        MFMultitouchUser AvailableUsers;
        MFMultitouchUser RemovedUsers;

        SFFloat TimeIn;

        SFBool Filtering;
        SFInt FilterSize;

		void handleFingers();
		void handleUsers();
        /* virtual */ void evaluate();

        // FingerListener interface
        void fingerAdded(FingerInfo *pFingerInfo);
        void fingerMoved(FingerInfo *pFingerInfo);
        void fingerRemoved(FingerInfo *pFingerInfo);

		// FingerListener interface
        void userAdded(UserInfo *pUserInfo);
        void userMoved(UserInfo *pUserInfo);
        void userRemoved(UserInfo *pUserInfo);

      private:

		void processMessages(std::queue< std::pair< FingerClient::MyMessage, int > > &messages);

#ifdef _WIN32
		inline long getMilliseconds() const{
		  timeGetTime();
    };
#else
		inline long getMilliseconds() const{
      struct timeval start;
      long mtime, seconds, useconds;
      gettimeofday(&start, NULL);
      seconds  = start.tv_sec;
      useconds = start.tv_usec;
      mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
      return mtime;
    };
#endif

        FingerClient *mFingerClient;
        struct FingerAction
        {
          bool do_add;
          bool do_move;
          bool do_remove;
          int finger_id, user_id;
          Vec2 position;
		  float angle, width, height, area;
          Link<MultitouchFinger> finger;
          FingerAction(int _finger_id = -1) : do_add(false), do_move(false), do_remove(false), finger_id(_finger_id), user_id(-1)
          {
          }
        };
		struct UserAction
        {
          bool do_add;
          bool do_move;
          bool do_remove;
          int user_id;
          Link<MultitouchUser> user;
          UserAction(int _user_id = -1) : do_add(false), do_move(false), do_remove(false), user_id(_user_id)
          {
          }
        };
        typedef std::map<int, FingerAction> FingerMap;
		typedef std::map<int, UserAction> UserMap;
        FingerMap mFingerMap;
		UserMap mUserMap;
        boost::mutex mMutex;
		int mOriginalPort, mReceivedPort;
		double mReceivedPortTime;
      };
    }
  }
}

#endif

