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

#include <avango/daemon/StationSegment.h>

#include <avango/Assert.h>
#include <avango/daemon/StationBlock.h>
#include <avango/daemon/SharedMemorySegment.h>


av::daemon::StationSegment::StationSegment()
  : mSharedMem(new SharedMemorySegment (0xc3, sizeof(StationBlock)))
{
  AV_ASSERT(mSharedMem);
  AV_ASSERT(mSharedMem->segment());

  if (mSharedMem->created()) {
    mStationBlock = new (mSharedMem->segment()) StationBlock;
  } else {
    mStationBlock = static_cast<StationBlock*>(mSharedMem->segment());
  }
}

av::daemon::StationSegment::~StationSegment()
{
  delete mSharedMem;
}

av::daemon::Station*
av::daemon::StationSegment::getStation(const char* name)
{
  return (mStationBlock) ? mStationBlock->getStation(name) : 0;
}

av::daemon::Station*
av::daemon::StationSegment::getStation(const std::string& name)
{
  return getStation(name.c_str());
}
