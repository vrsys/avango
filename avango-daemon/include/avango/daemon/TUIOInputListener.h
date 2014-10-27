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

#ifndef AV_DAEMON_TUIO_INPUT_LISTENER_H
#define AV_DAEMON_TUIO_INPUT_LISTENER_H

#include <TuioListener.h>

namespace av
{
  namespace daemon
  {
    class TUIOInputListener : public TUIO::TuioListener {

      public:
        struct TUIOCursor {
          float x;
          float y;
          float x_speed;
          float y_speed;
          float motion_speed;
          float motion_acceleration;
          bool is_moving;
          int state;
          int session_id;
        };

        std::map<int, TUIOCursor> cursors;

        void addTuioObject(TUIO::TuioObject* tobj) {}
        void updateTuioObject(TUIO::TuioObject* tobj) {}
        void removeTuioObject(TUIO::TuioObject* tobj) {}

        void addTuioCursor(TUIO::TuioCursor* tcur) {
          TUIOCursor& cursor = cursors[tcur->getCursorID()];
          cursor.session_id = tcur->getSessionID();
          cursor.x = tcur->getX();
          cursor.y = tcur->getY();
          cursor.x_speed = tcur->getXSpeed();
          cursor.y_speed = tcur->getYSpeed();
          cursor.motion_speed = tcur->getMotionSpeed();
          cursor.motion_acceleration = tcur->getMotionAccel();
          cursor.is_moving = tcur->isMoving();
          cursor.state = tcur->getTuioState();
        }
        void updateTuioCursor(TUIO::TuioCursor* tcur) {
          TUIOCursor& cursor = cursors[tcur->getCursorID()];
          cursor.x = tcur->getX();
          cursor.y = tcur->getY();
          cursor.x_speed = tcur->getXSpeed();
          cursor.y_speed = tcur->getYSpeed();
          cursor.motion_speed = tcur->getMotionSpeed();
          cursor.motion_acceleration = tcur->getMotionAccel();
          cursor.is_moving = tcur->isMoving();
          cursor.state = tcur->getTuioState();
        }

        void removeTuioCursor(TUIO::TuioCursor* tcur) {
          cursors.erase(tcur->getCursorID());
        }

        void addTuioFinger(TUIO::TuioFinger* tfinger) {}
        void updateTuioFinger(TUIO::TuioFinger* tfinger) {}
        void removeTuioFinger(TUIO::TuioFinger* tfinger) {}

        void addTuioHand(TUIO::TuioHand* thand) {}
        void updateTuioHand(TUIO::TuioHand* thand) {}
        void removeTuioHand(TUIO::TuioHand* thand) {}

        void refresh(TUIO::TuioTime frameTime) {}
    };
  }
}

#endif
