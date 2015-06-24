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

#if !defined(AVANGO_DAEMON_DEVICEDAEMON_H)
#define AVANGO_DAEMON_DEVICEDAEMON_H



/**
 * \file
 * \ingroup av_daemon
 */

#include <avango/Base.h>
#include <avango/Link.h>
#include <avango/daemon/Device.h>

namespace av
{
  namespace daemon
  {
    /**
     * The DeviceDaemon main class, contains the main loop in which for all
     * given devices a thread is created.
     *
     * \ingroup av_daemon
     */
    class AV_DAEMON_DLL DeviceDaemon : public Base
    {
      AV_BASE_DECLARE();

    public:

      /**
       * Constructor.
       */
      DeviceDaemon() = default;

      /**
       * This method appends a device to the internal list of devices that
       * will be started when run() is called.
       */
      void appendDevice(av::daemon::Device* device);

      /**
       * This method tries to start up all given devices and then enters
       * the main loop until devices should be shut down.
       */
      void run();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      ~DeviceDaemon() = default;

    private:

      std::vector<av::Link<av::daemon::Device> > mDevices;

    };
  }
}

#endif
