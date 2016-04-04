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
#include <avango/daemon/TUIOInput.h>
#include <avango/daemon/TUIOInputListener.h>
#include <avango/daemon/Station.h>
#include <string>
#include <sstream>
#include <boost/algorithm/string/predicate.hpp>

#include <TuioClient.h>

#ifdef AVANGO_DAEMON_DEBUG
#include <cstdlib>
#include <cstring>
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>
#include <ucontext.h>
#include <unistd.h>
#endif

namespace
{
av::Logger& logger(av::getLogger("av::daemon::TUIOInput"));

#ifdef AVANGO_DAEMON_DEBUG

// This structure mirrors the one found in /usr/include/asm/ucontext.h
struct sig_ucontext_t {
  unsigned long     uc_flags;
  struct ucontext   *uc_link;
  stack_t           uc_stack;
  struct sigcontext uc_mcontext;
  sigset_t          uc_sigmask;
};

void crit_err_hdlr(int sig_num, siginfo_t* info, void * ucontext)
{
  sig_ucontext_t* uc = static_cast<sig_ucontext_t *>(ucontext);

  /* Get the address at the time the signal was raised */
#if defined(__i386__) // gcc specific
  void * caller_address = (void *) uc->uc_mcontext.eip; // EIP: x86 specific
#elif defined(__x86_64__) // gcc specific
  void * caller_address = (void *) uc->uc_mcontext.rip; // RIP: x86_64 specific
#else
#error Unsupported architecture. // TODO: Add support for other arch.
#endif

  std::cerr << "signal " << sig_num << "(" << strsignal(sig_num) << ")"
    << " address is " << info->si_addr << " from " << ((void *)caller_address) << "\n";

  void* array[50];
  auto n = backtrace(array, 50);

  // overwrite sigaction with caller's address
  array[1] = caller_address; // index 1 , why ????

  char** messages = backtrace_symbols(array, n);

  //for (unsigned i = 0; i < n && messages != nullptr; ++i) {
  // skip first stack frame (points here)
  for (unsigned i = 1; i < n && messages != nullptr; ++i) {
    std::cerr << "[bt]: ( " << i << ") " << messages[i] << "\n";
  }

  std::free(messages);

  std::exit(EXIT_FAILURE);
}

void sigsegv_handler(int sig) {

  void* array[10];
  size_t n = backtrace(array, 10);
  char** strings = backtrace_symbols(array, n);

  std::cout << "Obtained " << n << "stack frames.\n";
  for (unsigned i = 0; i < n; ++i) {
    std::cout << strings[i] << std::endl;
  }

  std::free(strings);

  std::exit(EXIT_FAILURE);
}
#endif // AVANGO_DAEMON_DEBUG

} // namespace

AV_BASE_DEFINE(av::daemon::TUIOInput);

av::daemon::TUIOInput::TUIOInput()
  : mTUIOClient(nullptr)
  , mTUIOInputListener(std::make_shared<TUIOInputListener>())
  , mRequiredFeatures()
  , mStationToSessionID_cursors()
  , mStationToSessionID_fingers()
  , mStationToSessionID_hands()
{
  mRequiredFeatures.push_back("port");
}

void
av::daemon::TUIOInput::initClass()
{
  if (!isTypeInitialized())
  {
    av::daemon::Device::initClass();
    AV_BASE_INIT(av::daemon::Device, av::daemon::TUIOInput, true);
  }
}

/* virtual */ void
av::daemon::TUIOInput::startDevice()
{
  if (!parseFeatures())
    return;

  logger.info() << "startDevice: device configured successfully";

  // initialize

  mTUIOClient = std::make_shared<TUIO::TuioClient>(mPort);
  mTUIOClient->addTuioListener(mTUIOInputListener.get());
  mTUIOClient->connect();

  logger.info() << "startDevice: device initialized successfully";
}

int
av::daemon::TUIOInput::getSessionIDForStation_cursors(std::pair<int, av::daemon::Station*> const& station)
{
  auto cursors = mTUIOInputListener->cursors();
  auto left_it(mStationToSessionID_cursors.left.find(station.first));

  // remove mapping if session ID has expired
  if (left_it != mStationToSessionID_cursors.left.end()
      && cursors.find(left_it->second) == cursors.end())
  {
    mStationToSessionID_cursors.left.erase(left_it);
    left_it = mStationToSessionID_cursors.left.end();
  }

  int sessionId = -1;

  if (left_it == mStationToSessionID_cursors.left.end()) {
    for (auto const& cursor : cursors) {
      auto right_it(mStationToSessionID_cursors.right.find(
                      cursor.second.session_id));
      if (right_it == mStationToSessionID_cursors.right.end()) {
        mStationToSessionID_cursors.insert(
            ::boost::bimap<int, int>::value_type(station.first,
                                                 cursor.second.session_id));
        sessionId = cursor.second.session_id;
        break;
      }
    }
  } else {
    sessionId = left_it->second;
  }

  return sessionId;
}

int
av::daemon::TUIOInput::getSessionIDForStation_fingers(std::pair<int, av::daemon::Station*> const& station)
{
  auto fingers = mTUIOInputListener->fingers();
  auto left_it(mStationToSessionID_fingers.left.find(station.first));

  // remove mapping if session ID has expired
  if (left_it != mStationToSessionID_fingers.left.end()
      && fingers.find(left_it->second) == fingers.end())
  {
    mStationToSessionID_fingers.left.erase(left_it);
    left_it = mStationToSessionID_fingers.left.end();
  }

  int sessionId = -1;

  if (left_it == mStationToSessionID_fingers.left.end()) {
    for (auto const& finger : fingers) {
      auto right_it(mStationToSessionID_fingers.right.find(
                      finger.second.session_id));
      if (right_it == mStationToSessionID_fingers.right.end()) {
        mStationToSessionID_fingers.insert(
            ::boost::bimap<int, int>::value_type(station.first,
                                                 finger.second.session_id));
        sessionId = finger.second.session_id;
        break;
      }
    }
  } else {
    sessionId = left_it->second;
  }

  return sessionId;
}

int
av::daemon::TUIOInput::getSessionIDForStation_hands(std::pair<int, av::daemon::Station*> const& station)
{
  auto hands = mTUIOInputListener->hands();
  auto left_it(mStationToSessionID_hands.left.find(station.first));

  // remove mapping if session ID has expired
  if (left_it != mStationToSessionID_hands.left.end()
      && hands.find(left_it->second) == hands.end())
  {
    mStationToSessionID_hands.left.erase(left_it);
    left_it = mStationToSessionID_hands.left.end();
  }

  int sessionId = -1;

  if (left_it == mStationToSessionID_hands.left.end()) {
    for (auto const& hand : hands) {
      auto right_it(mStationToSessionID_hands.right.find(
                      hand.second.session_id));
      if (right_it == mStationToSessionID_hands.right.end()) {
        mStationToSessionID_hands.insert(
            ::boost::bimap<int, int>::value_type(station.first,
                                                 hand.second.session_id));
        sessionId = hand.second.session_id;
        break;
      }
    }
  } else {
    sessionId = left_it->second;
  }

  return sessionId;
}

/* virtual */ void
av::daemon::TUIOInput::readLoop()
{
#ifdef AVANGO_DAEMON_DEBUG
  //signal(SIGSEGV, sigsegv_handler);

  struct sigaction sigact;

  sigact.sa_sigaction = crit_err_hdlr;
  sigact.sa_flags = SA_RESTART | SA_SIGINFO;

  if (sigaction(SIGSEGV, &sigact, (struct sigaction *)nullptr) != 0) {
    std::cerr << "error setting signal handler for " << SIGSEGV << " (" << strsignal(SIGSEGV) << ")\n";
    std::exit(EXIT_FAILURE);
  }
#endif
  while (mKeepRunning) {
    for (auto const& station : mStations) {
      if (nullptr == station.second) {
        // we exceeded the maximum number of stations
        break;
      }

      std::string name = station.second->getName();

      if (boost::algorithm::ends_with(name, "#cursor")/* || name.find("#") == std::string::npos*/) {
        int sessionID = getSessionIDForStation_cursors(station);
        if (auto cursor = mTUIOInputListener->find_cursor(sessionID)) {
          station.second->setValue(0, cursor->session_id);
          station.second->setValue(1, cursor->x);
          station.second->setValue(2, cursor->y);
          station.second->setValue(3, cursor->x_speed);
          station.second->setValue(4, cursor->y_speed);
          station.second->setValue(5, cursor->motion_speed);
          station.second->setValue(6, cursor->motion_acceleration);
          station.second->setValue(7, cursor->is_moving);
          station.second->setValue(8, cursor->state);
        } else {
          station.second->setValue(0, -1.f);
          station.second->setValue(1, -1.f);
          station.second->setValue(2, -1.f);
          station.second->setValue(3, 0.f);
          station.second->setValue(4, 0.f);
          station.second->setValue(5, 0.f);
          station.second->setValue(6, 0.f);
          station.second->setValue(7, false);
          station.second->setValue(8, TUIOState::REMOVED);
        }
      } else if (boost::algorithm::ends_with(name, "#finger")) {
          int sessionID = getSessionIDForStation_fingers(station);
          if (auto finger = mTUIOInputListener->find_finger(sessionID)) {
            //std::cout << "finger  "<< finger->session_id << "  [" << finger->x << ", " << finger->y << "]  hand id: " << finger->hand_id << std::endl;
            station.second->setValue(0, finger->session_id);
            station.second->setValue(1, finger->x);
            station.second->setValue(2, finger->y);
            station.second->setValue(3, finger->hand_id);
            station.second->setValue(4, finger->finger_class);
          } else {
            station.second->setValue(0, -1.f);
            station.second->setValue(1, -1.f);
            station.second->setValue(2, -1.f);
            station.second->setValue(3, -1.f);
            station.second->setValue(4, TUIO::TuioFinger::Class::UNKNOWN);
          }
        } else if (boost::algorithm::ends_with(name, "#hand")) {
          int sessionID = getSessionIDForStation_hands(station);
          if (auto hand = mTUIOInputListener->find_hand(sessionID)) {
              //std::cout << "hand  " << hand->session_id << "  f1: " << hand->fingers[0] << std::endl;
            station.second->setValue(0,  hand->session_id);
            station.second->setValue(1,  hand->x);
            station.second->setValue(2,  hand->y);
            station.second->setValue(3,  hand->fingers[0]);
            station.second->setValue(4,  hand->fingers[1]);
            station.second->setValue(5,  hand->fingers[2]);
            station.second->setValue(6,  hand->fingers[3]);
            station.second->setValue(7,  hand->fingers[4]);
            station.second->setValue(8,  hand->hand_class);
            station.second->setValue(9,  hand->ellipse_x);
            station.second->setValue(10, hand->ellipse_y);
            station.second->setValue(11, hand->ellipse_major);
            station.second->setValue(12, hand->ellipse_minor);
            station.second->setValue(13, hand->ellipse_inclination);
          } else {
            station.second->setValue(0,  -1.f);
            station.second->setValue(1,  -1.f);
            station.second->setValue(2,  -1.f);
            station.second->setValue(3,  -1.f);
            station.second->setValue(4,  -1.f);
            station.second->setValue(5,  -1.f);
            station.second->setValue(6,  -1.f);
            station.second->setValue(7,  -1.f);
            station.second->setValue(8,  TUIO::TuioHand::Class::UNKNOWN);
            station.second->setValue(9,  -1.f);
            station.second->setValue(10, -1.f);
            station.second->setValue(11, -1.f);
            station.second->setValue(12, -1.f);
            station.second->setValue(13, -1.f);
          }
        }
    }
  }
}

/* virtual */ void
av::daemon::TUIOInput::stopDevice()
{
  mTUIOClient->disconnect();
  logger.info() << "stopDevice: done.";
}

const std::vector<std::string>&
av::daemon::TUIOInput::queryFeatures()
{
  return mRequiredFeatures;
}

bool
av::daemon::TUIOInput::parseFeatures()
{
  std::string port(queryFeature("port"));
  if (port == "") {
    logger.warn() << "parseFeatures: feature 'port' not specified";
    return false;
  } else {
    logger.info() << "parseFeatures: configured feature 'port' = %s", port;
    std::stringstream(port) >> mPort;
  }

  return true;
}
