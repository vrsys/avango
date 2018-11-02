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

#include <avango/daemon/BufferBlock.h>

#include <avango/Logger.h>
#include <cstring>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::BufferBlock"));
}

av::daemon::BufferBlock::BufferBlock()
  : mNumBuffers(0),
    mMutex()
{}

av::daemon::Buffer*
av::daemon::BufferBlock::getBuffer(const char* name)
{
  // very simple and inefficient for now
  Buffer* buffer_ptr = nullptr;

  std::lock_guard<std::mutex> lock(mMutex);

  int i = 0;

  for (i=0; i<mNumBuffers; i++)
  {
    if (std::strcmp(name, mBuffers[i].getName()) == 0)
    {
      buffer_ptr = &mBuffers[i];
      LOG_TRACE(logger) << "getBuffer(): referenced buffer '" << name << "', " << buffer_ptr;

      break;
    }
  }

  if (!buffer_ptr && i < sMaxBufferNum) {
    buffer_ptr = new (&mBuffers[i]) Buffer;
    buffer_ptr->setName(name);
    logger.debug() << "getBuffer(): created buffer '" << name << "', " << buffer_ptr;

    mNumBuffers++;
  }

  return buffer_ptr;
}
