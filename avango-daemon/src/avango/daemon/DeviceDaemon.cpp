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

#include <iostream>
#include <fstream>
#include <iterator>

#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <avango/Link.h>
#include <avango/Logger.h>
#include <avango/daemon/DeviceDaemon.h>
#include <avango/daemon/Config.h>
#include <avango/daemon/Device.h>
#include <avango/daemon/StationSegment.h>
#include <avango/logging/FileAppender.h>

using namespace av::logging;

namespace po = boost::program_options;

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::DeviceDaemon"));

  void
  printVersion()
  {
    std::cout << "AvangoDaemon Version "
              << AVANGO_DAEMON_VERSION_MAJOR
              << "." << AVANGO_DAEMON_VERSION_MINOR
              //<< "." << AVANGO_DAEMON_VERSION_MAINT
              << std::endl;
  }
}


AV_BASE_DEFINE(av::daemon::DeviceDaemon);

av::daemon::DeviceDaemon::DeviceDaemon()
{}

av::daemon::DeviceDaemon::~DeviceDaemon()
{}

void
av::daemon::DeviceDaemon::initClass()
{
  if (!isTypeInitialized())
  {
    av::Base::initClass();
    AV_BASE_INIT(av::Base, av::daemon::DeviceDaemon, true);
  }
}

void
av::daemon::DeviceDaemon::appendDevice(Device* device)
{
  mDevices.push_back(Link<Device>(device));
}

void
av::daemon::DeviceDaemon::run()
{
  // print info string
  printVersion();
  std::cout << "Press 'q' to stop daemon." << std::endl;

  // start all devices
  for (auto const& d : mDevices)
    d->startUp();

  // wait for 'exit' command
  char key = 0;
  while (key != 'q')
    std::cin >> key;

  // shutdown all devices
  for (auto const& d : mDevices)
    d->shutDown();
}
