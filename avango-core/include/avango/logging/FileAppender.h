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

#if !defined(AVANGO_LOGGING_FILEAPPENDER_H)
#define AVANGO_LOGGING_FILEAPPENDER_H

// includes, system

#include <string>
#include <fstream>

// includes, project

#include <avango/logging/Appender.h>
#include <avango/windows_specific.h>

namespace av
{

  namespace logging
  {

    /**
     * Appender for printing log messages to a file.
     *
     * \todo Error handling
     * \todo Make it thread-safe
     */
    class AV_DLL FileAppender : public Appender
    {

    public:

      /**
       * Create a file appender which logs to filename.
       */
      FileAppender(const std::string& filename);

      /**
       * Destructor
       */
      virtual ~FileAppender();

      /**
       * Returns the filename of the file we are logging to.
       */
      const std::string& getFilename() const;

      virtual void doAppend(LoggingEvent& event);

    private:

      const std::string mFilename;
      std::ofstream mLogFile;

    };

  } // namespace logging

} // namespace av

#endif // #if !defined(AVANGO_LOGGING_FILEAPPENDER_H)
