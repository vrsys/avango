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

#include <avango/daemon/BufferSegment.h>

#include <avango/Assert.h>
#include <avango/daemon/BufferBlock.h>

#ifdef WIN32
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::daemon::BufferSegment"));
}

#else
#include <avango/daemon/SharedMemorySegment.h>
#endif

av::daemon::BufferSegment::BufferSegment()
{
#ifndef WIN32
    mSharedMem = new SharedMemorySegment(0xc3, sizeof(BufferBlock));

    AV_ASSERT(mSharedMem);
    AV_ASSERT(mSharedMem->segment());

    if(mSharedMem->created())
    {
        mBufferBlock = new(mSharedMem->segment()) BufferBlock;
    }
    else
    {
        mBufferBlock = static_cast<BufferBlock*>(mSharedMem->segment());
    }
#else
    mShmName = "AvDaemon";
    using namespace boost::interprocess;

    // a dirty trick to calculate required segment size
    const std::size_t shmSize = sizeof(BufferBlock) * 8;

    // construct managed shared memory
    mSegment = new managed_windows_shared_memory(open_or_create, mShmName.c_str(), shmSize);

    if(mSegment->get_size() < shmSize)
    {
        delete mSegment;
        boost::interprocess::shared_memory_object::remove(mShmName.c_str());
        mSegment = new managed_windows_shared_memory(create_only, mShmName.c_str(), shmSize);
        logger.warn() << "BufferSegment(): Shared memory segment has been rebuilt with new size "
                      << "because the old size was less that required.";
    }

    mBufferBlock = mSegment->find_or_construct<BufferBlock>("BufferBlock")();
    AV_ASSERT(mBufferBlock);
#endif
}

av::daemon::BufferSegment::~BufferSegment()
{
#ifndef WIN32
    delete mSharedMem;
#else
    delete mSegment;
#endif
}

av::daemon::Buffer* av::daemon::BufferSegment::getBuffer(const char* name) { return (mBufferBlock) ? mBufferBlock->getBuffer(name) : 0; }

av::daemon::Buffer* av::daemon::BufferSegment::getBuffer(const std::string& name) { return getBuffer(name.c_str()); }
