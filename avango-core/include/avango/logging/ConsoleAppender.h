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

#if !defined(AVANGO_LOGGING_CONSOLEAPPENDER_H)
#define AVANGO_LOGGING_CONSOLEAPPENDER_H

// includes, system

// includes, project

#include <avango/logging/Appender.h>
#include <avango/Singleton.h>
#include <avango/windows_specific.h>


namespace av
{

  namespace logging
  {

    /**
     * Appender for printing log messages to std::clog.
     * ConsoleAppender is a singleton.
     *
     * \todo Make it thread-safe
     */

    class AV_DLL ConsoleAppender : public Appender
    {

      //friend class Singleton<ConsoleAppender>;

    public:

      /* virtual */ void doAppend(LoggingEvent& event);
      ConsoleAppender() {}
      virtual ~ConsoleAppender() {}
    protected:


    };
    typedef Singleton<ConsoleAppender> ConsoleAppenderInstance;

  } // namespace logging

} // namespace av

#endif // #if !defined(AVANGO_LOGGING_CONSOLEAPPENDER_H)

