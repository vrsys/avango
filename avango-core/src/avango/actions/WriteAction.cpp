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

#include "avango/WriteAction.h"

// includes, system

#include <fstream>
#include <stdexcept>

// includes, project

#include <avango/logging/Logger.h>

// internal unnamed namespace

namespace
{
  // types, internal

  // variables, internal

  av::Logger& logger(av::getLogger("av::WriteAction"));

  const int ID_COUNTER_INIT_VALUE = 0;
  const int BINARY_INIT_VALUE = true;

  const int DEPTH_INDENT_SPACES = 4;

// functions, internal

} // namespace

AV_BASE_DEFINE(av::WriteAction)
;

av::WriteAction::WriteAction() :
  mIdCounter(ID_COUNTER_INIT_VALUE), mBinary(BINARY_INIT_VALUE)
{
}

av::WriteAction::WriteAction(const std::string& fileName) :
  mIdCounter(ID_COUNTER_INIT_VALUE), mBinary(BINARY_INIT_VALUE)
{
  setFileName(fileName);
}

/* virtual */
av::WriteAction::~WriteAction()
{
}

void av::WriteAction::initClass()
{
  if (!isTypeInitialized())
  {
    av::Action::initClass();

    AV_BASE_INIT(av::Action, av::WriteAction, true);
  }
}

void av::WriteAction::setFileName(const std::string& fileName)
{
  LOG_TRACE(logger) << "setFileName(): file name is '" << mName << "'";
  mName = fileName;
}

/* virtual */void av::WriteAction::apply(av::Link<av::Base> node)
{
  LOG_TRACE(logger) << "apply(): writing to '" << mName << "'";

  std::ofstream file(mName.c_str());

  if (file.fail())
  {
    logger.warn() << "apply(): can't open file '" << mName << "'";
    return;
  }

  mIdMap.clear();
  mIdCounter = 0;
  mOutStream = file;
  mOutStream.setWriteAction(this);
  mOutStream.enableBinary(mBinary);

  mTravDepth = -1;
  traverse(node);

  mIdMap.clear();

  LOG_TRACE(logger) << "apply(): closing file '" << mName << "'";
}

/* virtual */void av::WriteAction::traverse(av::Link<av::Base> node)
{
  ++mTravDepth;

  LOG_TRACE(logger) << "traverse(): %snode: 0x%x (%s)", depthIndent(), node.getPtr(), node->getTypeId().getName().c_str();

  if (node.isValid())
  {
    node->doWriteAction(*this);
  }
  --mTravDepth;
}

void av::WriteAction::enableBinary(bool bin)
{
  mBinary = bin;
}

bool av::WriteAction::isBinaryEnabled()
{
  return mBinary;
}

av::OutputStream& av::WriteAction::getStream()
{
  return mOutStream;
}

std::string av::WriteAction::getNewId()
{
  std::ostringstream new_id_string;

  new_id_string << "object" << mIdCounter;
  mIdCounter++;
  return new_id_string.str();
}

std::string av::WriteAction::lookupObjectId(av::Link<av::Base> obj)
{
  if (!obj.isValid())
  {
    return "null";
  }

  BaseStringMap::iterator object_id = mIdMap.find(obj);
  if (object_id != mIdMap.end())
  {
    return (*object_id).second;
  }
  else
  {
    std::string id = getNewId();
    mIdMap.insert(BaseStringMap::value_type(obj, id));
    return id;
  }
}

bool av::WriteAction::isObjectWritten(av::Link<av::Base> obj)
{
  if (!obj.isValid())
  {
    throw std::invalid_argument("av::Link is not valid");
  }

  BaseStringMap::iterator object_id = mIdMap.find(obj);
  if (object_id != mIdMap.end())
  {
    return true;
  }
  else
  {
    std::string id = getNewId();
    mIdMap.insert(BaseStringMap::value_type(obj, id));
    return false;
  }
}

std::string av::WriteAction::depthIndent()
{
  if (mTravDepth == -1)
  {
    return "";
  }

  // create a string with mTravDepth * DEPTH_INDENT_SPACES spaces
  std::string indent_string(mTravDepth * DEPTH_INDENT_SPACES, ' ');

  return indent_string;
}
