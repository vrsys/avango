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
#include <array>
#include <mutex>
#include <boost/optional.hpp>

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

        struct TUIOFinger {
            float x;
            float y;
            float x_speed;
            float y_speed;
            float ellipse_x;
            float ellipse_y;
            float ellipse_major;
            float ellipse_minor;
            float ellipse_inclination;
            int session_id;
        };

        struct TUIOHand {
            TUIO::TuioHand::Class hand_class;
            TUIO::TuioHand::FingerArray fingers;
            int session_id;
        };

      private:
        mutable std::mutex m_cursorsMutex;
        std::map<int, TUIOCursor> m_cursors;
        mutable std::mutex m_fingersMutex;
        std::map<int, TUIOFinger> m_fingers;
        mutable std::mutex m_handsMutex;
        std::map<int, TUIOHand> m_hands;
      public:

        inline std::map<int, TUIOCursor> cursors() const
        {
          std::lock_guard<std::mutex> lock(m_cursorsMutex);
          return m_cursors;
        }

        inline std::map<int, TUIOFinger> fingers() const
        {
          std::lock_guard<std::mutex> lock(m_fingersMutex);
          return m_fingers;
        }

        inline std::map<int, TUIOHand> hands() const
        {
          std::lock_guard<std::mutex> lock(m_handsMutex);
          return m_hands;
        }

        // is the given key a member of the map ?
        template<typename T>
        bool member(int) const { return false; }

        boost::optional<TUIOCursor> find_cursor(int id) const
        {
          std::lock_guard<std::mutex> lock(m_cursorsMutex);
          auto it = m_cursors.find(id);
          if (it == m_cursors.end()) {
            return boost::optional<TUIOCursor>();
          } else {
            return boost::make_optional(it->second);
          }
        }

        boost::optional<TUIOFinger> find_finger(int id) const
        {
          std::lock_guard<std::mutex> lock(m_fingersMutex);
          auto it = m_fingers.find(id);
          if (it == m_fingers.end()) {
            return boost::optional<TUIOFinger>();
          } else {
            return boost::make_optional(it->second);
          }
        }

        boost::optional<TUIOHand> find_hand(int id) const
        {
          std::lock_guard<std::mutex> lock(m_handsMutex);
          auto it = m_hands.find(id);
          if (it == m_hands.end()) {
            return boost::optional<TUIOHand>();
          } else {
            return boost::make_optional(it->second);
          }
        }

        void addTuioObject(TUIO::TuioObject* tobj) override {}
        void updateTuioObject(TUIO::TuioObject* tobj) override {}
        void removeTuioObject(TUIO::TuioObject* tobj) override {}

        void addTuioCursor(TUIO::TuioCursor* tcur) override {
          std::lock_guard<std::mutex> lock(m_cursorsMutex);
          TUIOCursor& cursor = m_cursors[tcur->getSessionID()];
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
        void updateTuioCursor(TUIO::TuioCursor* tcur) override {
          std::lock_guard<std::mutex> lock(m_cursorsMutex);
          TUIOCursor& cursor = m_cursors[tcur->getSessionID()];
          cursor.x = tcur->getX();
          cursor.y = tcur->getY();
          cursor.x_speed = tcur->getXSpeed();
          cursor.y_speed = tcur->getYSpeed();
          cursor.motion_speed = tcur->getMotionSpeed();
          cursor.motion_acceleration = tcur->getMotionAccel();
          cursor.is_moving = tcur->isMoving();
          cursor.state = tcur->getTuioState();
        }

        void removeTuioCursor(TUIO::TuioCursor* tcur) override {
          std::lock_guard<std::mutex> lock(m_cursorsMutex);
          m_cursors.erase(tcur->getSessionID());
        }

        void addTuioFinger(TUIO::TuioFinger* tfinger) override {
          std::lock_guard<std::mutex> lock(m_fingersMutex);
          TUIOFinger& finger = m_fingers[tfinger->getSessionID()];
          finger.session_id = tfinger->getSessionID();
          finger.x = tfinger->getX();
          finger.y = tfinger->getY();
          finger.ellipse_x = tfinger->getEllipseX();
          finger.ellipse_y = tfinger->getEllipseY();
          finger.ellipse_major = tfinger->getEllipseMajor();
          finger.ellipse_minor = tfinger->getEllipseMinor();
          finger.ellipse_inclination = tfinger->getEllipseInclination();
        }
        void updateTuioFinger(TUIO::TuioFinger* tfinger) override {
          addTuioFinger(tfinger);
        }
        void removeTuioFinger(TUIO::TuioFinger* tfinger) override {
          std::lock_guard<std::mutex> lock(m_fingersMutex);
          m_fingers.erase(tfinger->getSessionID());
        }

        void addTuioHand(TUIO::TuioHand* thand) override {
          std::lock_guard<std::mutex> lock(m_handsMutex);
          TUIOHand& hand = m_hands[thand->getSessionID()];
          hand.session_id = thand->getSessionID();
          hand.hand_class = thand->getHandClass();
          hand.fingers = thand->getFingerIDs();
        }
        void updateTuioHand(TUIO::TuioHand* thand) override {
          addTuioHand(thand);
        }
        void removeTuioHand(TUIO::TuioHand* thand) override {
          std::lock_guard<std::mutex> lock(m_handsMutex);
          m_hands.erase(thand->getSessionID());
        }

        void refresh(TUIO::TuioTime frameTime) override {}
    };

    // is the given cursor id a member of the cursor map ?
    template<>
    bool TUIOInputListener::member<TUIOInputListener::TUIOCursor>(int id) const
    {
      std::lock_guard<std::mutex> lock(m_cursorsMutex);
      auto it = m_cursors.find(id);
      return it != m_cursors.end();
    }

    // is the given finger id a member of the finger map ?
    template<>
    bool TUIOInputListener::member<TUIOInputListener::TUIOFinger>(int id) const
    {
      std::lock_guard<std::mutex> lock(m_fingersMutex);
      auto it = m_fingers.find(id);
      return it != m_fingers.end();
    }

    // is the given hand id a member of the hand map ?
    template<>
    bool TUIOInputListener::member<TUIOInputListener::TUIOHand>(int id) const
    {
      std::lock_guard<std::mutex> lock(m_handsMutex);
      auto it = m_hands.find(id);
      return it != m_hands.end();
    }
  }
}

#endif
