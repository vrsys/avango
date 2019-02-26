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

#if !defined(AV_DAEMON_STATIONBLOCK_H)
#define AV_DAEMON_STATIONBLOCK_H

/**
 * \file
 * \ingroup av_daemon
 */

#include <mutex>
#include <avango/daemon/Station.h>
#include <avango/daemon/windows_specific_daemon.h>

namespace av
{
namespace daemon
{
/**
 * Helper class for getting a station from av-daemon (used by av::daemon::StationSegment).
 *
 * \ingroup av_daemon
 */
class AV_DAEMON_DLL StationBlock
{
  public:
    /**
     * Constructor
     */
    StationBlock();

    /**
     * Get a station by given name.
     */
    Station* getStation(const char* name);

    // Boost managed_shared_memory requires public destructor
    // protected:

    /**
     * Destructor made protected to prevent allocation on stack.
     */
    virtual ~StationBlock() = default;

  private:
    /**
     * Builtin declaration here; definition is in impl. unit w/o value!
     * needed because of the array size for mStations!!!
     */
    static const int sMaxStationNum = 64;

    /**
     * Retain static memory layout since it gets put into a shared
     * memory segment not autogrowable!
     */
    Station mStations[sMaxStationNum];
    int mNumStations;
    std::mutex mMutex;

    /**
     * Made private to prevent copying construction.
     */
    StationBlock(const StationBlock&);

    /**
     * Made private to prevent assignment.
     */
    const StationBlock& operator=(const StationBlock&);
};
} // namespace daemon
} // namespace av

#endif // #if !defined(AV_DAEMON_STATIONBLOCK_H)
