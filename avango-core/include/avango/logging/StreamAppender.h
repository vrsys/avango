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

#if !defined(AVANGO_LOGGING_STREAMAPPENDER_H)
#define AVANGO_LOGGING_STREAMAPPENDER_H

// includes, system

#include <ostream>

// includes, project

#include <avango/logging/Appender.h>
#include <avango/windows_specific.h>

namespace av
{
namespace logging
{
/**
 * Appender for printing log messages to a stream.
 * \todo Make it thread-safe
 */
class AV_DLL StreamAppender : public Appender
{
  public:
    /**
     * Constructor made private to prevent multiple instantiation.
     */
    StreamAppender(std::ostream& os);

    /**
     * Destructor
     */
    virtual ~StreamAppender();

    /* virtual */ void doAppend(LoggingEvent& event);

  private:
    std::ostream& mStream;
};

} // namespace logging

} // namespace av

#endif // #if !defined(AVANGO_LOGGING_STREAMAPPENDER_H)
