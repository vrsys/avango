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


#if !defined(AVANGO_DAEMON_SHAREDMEMORYSEGMENT_H)
#define AVANGO_DAEMON_SHAREDMEMORYSEGMENT_H

/**
 * \file
 * \ingroup av_daemon
 */

#include <boost/noncopyable.hpp>
#include <cstddef>


namespace av
{
  namespace daemon
  {

    /**
     * Represents a shared memory segment.
     *
     * \ingroup av_daemon
     */
    class SharedMemorySegment : private boost::noncopyable {

    public:

      /**
       * Constructor.
       */
      SharedMemorySegment(int /* key */, int /* size */);

      /**
       * Destructor.
       */
      ~SharedMemorySegment();

      /**
       * Create segment.
       */
      void*  segment() const;

      /**
       * Returns size of segment.
       */
      size_t size() const;

      /**
       * Returns true if segment was created.
       */
      bool   created() const;

    private:

      bool   mCreated;
      int    mSegmentKey;
      int    mSegmentId;
      void*  mSegment;
      size_t mSegmentSize;

    };
  }
}

#endif // #if !defined(AVANGO_DAEMON_SHAREDMEMORYSEGMENT_H)
