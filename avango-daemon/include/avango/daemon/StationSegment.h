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

#if !defined(AV_DAEMON_STATIONSEGMENT_H)
#define AV_DAEMON_STATIONSEGMENT_H

/**
 * \file
 * \ingroup av_daemon
 */

#include <string>
#include <avango/daemon/windows_specific_daemon.h>

#ifdef WIN32
  #include <boost/interprocess/managed_windows_shared_memory.hpp>
#endif

namespace av
{
  namespace daemon
  {
    class Station;
    class StationBlock;
    class SharedMemorySegment;

    /**
     * Class for handling a shared memory segment.
     *
     * \ingroup av_daemon
     */
    class AV_DAEMON_DLL StationSegment {


    public:

      StationSegment();
      virtual ~StationSegment();

      /**
       * Returns station of given name.
       */
      Station* getStation(const char*);

      /**
       * Returns station of given name.
       */
      Station* getStation(const ::std::string&);


    private:

      /**
       * Made private to prevent copying construction.
       */
      StationSegment(const StationSegment&);

      /**
       * Made private to prevent assignment.
       */
      const StationSegment& operator=(const StationSegment&);

#ifdef WIN32
      boost::interprocess::managed_windows_shared_memory* mSegment;
      std::string          mShmName;
#else
      SharedMemorySegment* mSharedMem;
#endif

      StationBlock*        mStationBlock;

    };
  }
}

#endif // #if !defined(AV_OSG_STATIONSEGMENT_H)
