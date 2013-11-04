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

#if !defined(AV_DAEMON_LINUXEVENT_H)
#define AV_DAEMON_LINUXEVENT_H

#include <string>
#include <map>

/**
 * \file
 * \ingroup av_daemon
 */

namespace av
{
  namespace daemon
  {
    /**
     * Convenience class for converting a linux event type, given as string,
     * into the corresponding event code. (see /usr/include/linux/input.h).
     *
     * \ingroup av_daemon
     */
    class LinuxEvent : public ::std::map<std::string, unsigned long>
    {
    public:
      /**
       * Returns the event code of a given event type. (e.g. "KEY_DOWN" = 108).
       */
      static unsigned long getEventCode(const std::string& eventType);
      /**
       * Constructor
       */
      LinuxEvent();

    };
  }
}

#endif
