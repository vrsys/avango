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

// include i/f header

#include "avango/Distributed.h"

// includes, system

#include <cstdlib>

// includes, project

#include <avango/Assert.h>

#if defined(AVANGO_DISTRIBUTION_SUPPORT)
// #include <avango/MaestroEID.h>
#include <avango/NetID.h>
#include <avango/NetInfo.h>
#include <avango/Msg.h>
#include <avango/NetNode.h>
#endif

#define AVANGO_DISTRIBUTED_DEBUG
#undef AVANGO_DISTRIBUTED_DEBUG

// internal unnamed namespace

namespace
{
  // types, internal

  // variables, internal

  av::logging::Logger& logger(av::logging::Logger::getLogger("av::Distributed"));

  // functions, internal

} // namespace

// variables, exported

// functions, exported

AV_BASE_DEFINE_ABSTRACT(av::Distributed)

av::Distributed::Distributed()
#if defined(AVANGO_DISTRIBUTION_SUPPORT)
  : mNetInfo(0)
#endif
{}

/* virtual */
av::Distributed::~Distributed()
{}

/* static */ void
av::Distributed::initClass()
{
  if (!isTypeInitialized())
  {
    Base::initClass();

    AV_BASE_INIT_ABSTRACT(av::Base, av::Distributed, true);
  }
}

bool
av::Distributed::isOwned() const
{
#if defined(AVANGO_DISTRIBUTION_SUPPORT)
  return (!isDistributed() || (netCreator() == netNode()->netEID()));
#else
  return true;
#endif
}

#if defined(AVANGO_DISTRIBUTION_SUPPORT)

bool
av::Distributed::isDistributed() const
{
  return (mNetInfo != 0);
}

av::NetInfo*
av::Distributed::getNetInfo()
{
  return mNetInfo;
}

void
av::Distributed::setNetInfo(NetInfo* netInfo)
{
  mNetInfo = netInfo;
}

void
av::Distributed::notifyLocalChange()
{
  if (!isDistributed())
  {
    return;
  }

  Link<Distributed> tmp(this);
  mNetInfo->mNode->updateDistributedObject(tmp);
}

const av::NetID&
av::Distributed::netID() const
{
  AV_ASSERT(mNetInfo);
  return mNetInfo->getId();
}

const std::string&
av::Distributed::netCreator() const
{
  AV_ASSERT(mNetInfo);
  return mNetInfo->getId().getEID();
}

const av::NetNode*
av::Distributed::netNode() const
{
  AV_ASSERT(mNetInfo);
  return mNetInfo->getNode();
}

av::NetNode*
av::Distributed::netNode()
{
  AV_ASSERT(mNetInfo);
  return mNetInfo->getNode();
}

/* virtual */ void
av::Distributed::becomingDistributed()
{}

/* virtual */ void
av::Distributed::becomingUndistributed()
{}

#endif // #if defined(AVANGO_DISTRIBUTION_SUPPORT)
