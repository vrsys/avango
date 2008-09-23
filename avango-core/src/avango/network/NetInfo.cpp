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

#include "avango/NetInfo.h"

// includes, system

#include <iostream>

// includes, project

#include <avango/NetNode.h>

#include <avango/Config.h>

// internal unnamed namespace

namespace {
  av::Logger &logger(av::getLogger("av::NetInfo"));
} // namespace {

av::NetInfo::NetInfo(const NetID& a, const NetNode* b)
  : mId (a),
    mNode (const_cast<NetNode*> (b))
{
#if AVANGO_DEBUG
  LOG_TRACE(logger) << "NetInfo::NetInfo: "
                 << " @" << this
                 << " id: " << mId
                 << " node: @" << mNode;
#endif
}

av::NetInfo::~NetInfo()
{
#if AVANGO_DEBUG
  LOG_TRACE(logger) << "NetInfo::~NetInfo: "
                 << " @" << this
                 << " id: " << mId
                 << " node: @" << mNode;
#endif
}

const av::NetID&
av::NetInfo::getId() const
{
#if AVANGO_DEBUG
  LOG_TRACE(logger) << "NetInfo::get_id: "
                 << " @" << this
                 << " id: " << mId
                 << " node: @" << mNode;
#endif

  return mId;
}

const av::NetNode*
av::NetInfo::getNode() const
{
#if AVANGO_DEBUG
  LOG_TRACE(logger) << "NetInfo::get_node: "
                 << " @" << this
                 << " id: " << mId
                 << " node: @" << mNode;
#endif
  return mNode;
}

av::NetNode*
av::NetInfo::getNode()
{
#if AVANGO_DEBUG
  LOG_TRACE(logger) << "NetInfo::get_node: "
                 << " @" << this
                 << " id: " << mId
                 << " node: @" << mNode;
#endif
  return mNode;
}
