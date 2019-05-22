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

#if !defined(AVANGO_LOGGING_LOGGINGEVENT_H)
#define AVANGO_LOGGING_LOGGINGEVENT_H

// includes, system

#include <string>

// includes, project

#include <avango/logging/Logger.h>
#include <avango/windows_specific.h>

namespace av
{
namespace logging
{
/**
 * Internal representation of logging events.
 * \todo Record process-id, thread-id etc.
 */
class AV_DLL LoggingEvent
{
  public:
    /**
     * Constructor.
     */
    LoggingEvent(Logger& logger, Level level, const std::string& msg);

    /**
     * Destructor.
     */
    ~LoggingEvent();

    /**
     * Get originating logger of this logging event.
     */
    Logger& getLogger();

    /**
     * Get message of this logging event.
     */
    const std::string& getMessage() const;

    /**
     * Get timestamp of this logging event (seconds since 1970-01-01 GMT).
     */
    double getTimeStamp() const;

    /**
     * Get level of this logging event.
     */
    Level getLevel();

    /**
     * Get formatted string of this logging event.
     */
    std::string getFormattedString();

  private:
    Logger& mLogger;
    std::string mMsg;
    Level mLevel;
    double mTimeStamp;
};

} // namespace logging

} // namespace av

#endif // #if !defined(AV_LOGGINGEVENT_H)
