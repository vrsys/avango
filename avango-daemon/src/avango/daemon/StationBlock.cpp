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

#include <avango/daemon/StationBlock.h>

#include <avango/Logger.h>
#include <cstring>

namespace
{
av::Logger& logger(av::getLogger("av::daemon::StationBlock"));
}

av::daemon::StationBlock::StationBlock() : mNumStations(0), mMutex() {}

av::daemon::Station* av::daemon::StationBlock::getStation(const char* name)
{
    // very simple and inefficient for now
    Station* station = nullptr;

    std::lock_guard<std::mutex> lock(mMutex);

    int i = 0;

    for(i = 0; i < mNumStations; i++)
    {
        if(std::strcmp(name, mStations[i].getName()) == 0)
        {
            station = &mStations[i];
            LOG_TRACE(logger) << "getStation(): referenced station '" << name << "', " << station;

            break;
        }
    }

    if(!station && i < sMaxStationNum)
    {
        station = new(&mStations[i]) Station;
        station->setName(name);
        logger.debug() << "getStation(): created station '" << name << "', " << station;

        mNumStations++;
    }

    return station;
}
