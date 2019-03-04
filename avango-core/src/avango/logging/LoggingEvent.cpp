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

#include "avango/logging/LoggingEvent.h"

#include <sstream>

#if defined(_WIN32)
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

av::logging::LoggingEvent::LoggingEvent(Logger& logger, Level level, const std::string& msg) : mLogger(logger), mMsg(msg), mLevel(level)
{
#if defined(_WIN32)
    struct _timeb now;
    _ftime_s(&now);
    mTimeStamp = now.time + (now.millitm / 1000.0);
#else
    timeval now;
    gettimeofday(&now, NULL);
    mTimeStamp = now.tv_sec + (now.tv_usec / 1000000.);
#endif
}

av::logging::LoggingEvent::~LoggingEvent() {}

av::logging::Logger& av::logging::LoggingEvent::getLogger() { return mLogger; }

const std::string& av::logging::LoggingEvent::getMessage() const { return mMsg; }

double av::logging::LoggingEvent::getTimeStamp() const { return mTimeStamp; }

av::logging::Level av::logging::LoggingEvent::getLevel() { return mLevel; }

std::string av::logging::LoggingEvent::getFormattedString()
{
    std::ostringstream os;

    os << "[" << levelToString(mLevel);

    if(!getLogger().getName().empty())
    {
        os << " " << getLogger().getName();
    }

    os << "] " << getMessage();

    return os.str();
}
