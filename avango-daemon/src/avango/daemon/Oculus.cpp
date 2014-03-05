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

#include <errno.h>
#include <gua/math.hpp>
#include <avango/Logger.h>
#include <avango/daemon/Oculus.h>
#include <avango/daemon/Station.h>
#include <cstring>
#include <string>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::Oculus"));
}

AV_BASE_DEFINE(av::daemon::Oculus);

av::daemon::Oculus::Oculus()
  // : mOculus(new ::Oculus)
{
  mRequiredFeatures.push_back("port");
  mRequiredFeatures.push_back("timeout");
}

av::daemon::Oculus::~Oculus()
{}

void
av::daemon::Oculus::initClass()
{
  if (!isTypeInitialized()) {
    av::daemon::Device::initClass();
    AV_BASE_INIT(av::daemon::Device, av::daemon::Oculus, true);
  }
}

/* virtual */ void
av::daemon::Oculus::startDevice()
{
  if (!parseFeatures())
    return;
  std::cerr << "av::daemon::Oculus::startDevice() : not implemented yet" << std::endl;

  logger.info() << "startDevice: device configured successfully";

  // initialize
  logger.info() << "startDevice: device initialized successfully";
}

/* virtual */ void
av::daemon::Oculus::readLoop()
{
  while (mKeepRunning) {
    std::cerr << "av::daemon::Oculus::readLoop() : not implemented yet" << std::endl;
  }
}

/* virtual */ void
av::daemon::Oculus::stopDevice()
{
  std::cerr << "av::daemon::Oculus::stopDevice() : not implemented yet" << std::endl;
  logger.info() << "stopDevice: done.";
}

const std::vector<std::string>&
av::daemon::Oculus::queryFeatures()
{
  return mRequiredFeatures;
}

bool
av::daemon::Oculus::parseFeatures()
{
  return true;
}
