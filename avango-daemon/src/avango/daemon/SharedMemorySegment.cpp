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

#include <avango/daemon/SharedMemorySegment.h>

#include <cerrno>    // errno
#include <cstdlib>   // EXIT_FAILURE, ::exit
#include <cstring>   // ::strerror
#include <iostream>  // std::cerr, std::cout, std::endl
#include <sys/ipc.h> // ::ftok
#include <sys/shm.h> // ::shmat, ::shmctl, ::shmdt, ::shmget

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::daemon::SharedMemorySegment"));

  const bool segment_info_printing_enabled = (0 != ::getenv("AVANGO_STATIONSEGMENT_INFO_ENABLED"));

  void
  print_shm_segment_info (ssize_t key, ssize_t id, const char* extra_text = 0)
  {
    if (!segment_info_printing_enabled)
      return;

    struct ::shmid_ds info;

    if (-1 == ::shmctl(id, IPC_STAT, &info)) {
      std::cerr << "<unamed>::print_shm_segment_info: "
                << "shmctl(" << id << ", IPC_STAT, &info) for key "
                << std::hex << key << std::dec
                << " failed with reason '"
                << ::strerror(errno) << "'" << std::endl;

      return;
    }

    std::cout << "<unamed>::print_shm_segment_info: ";

    if (extra_text)
      std::cout << extra_text;

    std::cout << std::endl;

    std::cout << " shm_perm.key          : "
              << std::hex
#if defined(__GLIBC__)
              << info.shm_perm.__key
#else
              << info.shm_perm.key
#endif
              << std::dec
              << std::endl;

    std::cout << " shm_perm.owner        : "
              << info.shm_perm.uid << ':' << info.shm_perm.gid
              << std::endl;

    std::cout << " shm_perm.creator      : "
              << info.shm_perm.cuid << ':' << info.shm_perm.cgid
              << std::endl;

    std::cout << " shm_perm.mode         : "
              << info.shm_perm.mode
              << std::endl;

    std::cout << " shm_perm.seq          : "
#if defined(__GLIBC__)
              << info.shm_perm.__seq
#else
              << info.shm_perm.seq
#endif
              << std::endl;

    std::cout << " size                  : "
              << info.shm_segsz
              << " byte" << ((1 < info.shm_segsz) ? "s" : "")
              << std::endl;

    std::cout << " creator pid           : "
              << info.shm_cpid
              << std::endl;

    std::cout << " last operator pid     : "
              << info.shm_lpid
              << std::endl;

    std::cout << " # of attaches         : "
              << info.shm_nattch
              << std::endl;

    std::cout << " last shmat            : "
              << ((0 == info.shm_atime) ? "never" : ::asctime(::localtime(&info.shm_atime)))
              << std::endl;

    std::cout << " last shmdt            : "
              << ((0 == info.shm_dtime) ? "never" : ::asctime(::localtime(&info.shm_dtime)))
              << std::endl;

    std::cout << " last change by shmctl : "
              << ((0 == info.shm_ctime) ? "never" : ::asctime(::localtime(&info.shm_ctime)))
              << std::endl;
  }

}

av::daemon::SharedMemorySegment::SharedMemorySegment(int a, int b)
  : mCreated(false),
    mSegmentKey(::ftok("/dev/zero", a)),
    mSegmentId(-1),
    mSegment(0),
    mSegmentSize(b)
{
  if (-1 == mSegmentKey) {
    logger.error() << "SharedMemorySegment(): invalid segment key " << mSegmentKey << ", reason " << ::strerror(errno);
  }

  if (-1 == (mSegmentId = ::shmget (mSegmentKey, mSegmentSize, IPC_CREAT|0777))) {
    logger.warn() << "SharedMemorySegment(): shmget( %x, %d, IPC_CREAT|0777) failed with reason: %s",
                  mSegmentKey, mSegmentSize, ::strerror(errno);

    mSegmentSize = 0;

    throw std::runtime_error("Could not create shared memory segment!");
  }

  print_shm_segment_info (mSegmentKey, mSegmentId, "before shmat");

  // find out if we created or joined the shm segment before attaching to it
  struct ::shmid_ds segment_info;

  if (-1 == ::shmctl(mSegmentId, IPC_STAT, &segment_info)) {
    logger.warn() << "SharedMemorySegment(): shmctl(" << mSegmentKey << ", IPC_STAT, &segment_info) failed with reason: "
                  << ::strerror(errno);

    mSegmentSize = 0;

    throw std::runtime_error("Could not determine shared memory segment info!");
  }

  const bool we_are_first = (0 == segment_info.shm_nattch) ? true : false;

  if (!we_are_first && (mSegmentSize > segment_info.shm_segsz)) {
    logger.warn() << "SharedMemorySegment(): expected segment size (" << mSegmentSize << " byte"
                  << ((1 < mSegmentSize) ? "s" : "") << ") less than actual segment size ("
                  << segment_info.shm_segsz << " byte" << ((1 < segment_info.shm_segsz) ? "s" : "") << ")";

    const uid_t euid = ::geteuid();

    if (segment_info.shm_perm.uid == euid || segment_info.shm_perm.cuid == euid || 0 == euid)
    {
      logger.info() << "SharedMemorySegment(): attempting to resize segment size to "
                    << mSegmentSize << " byte" << ((1 < mSegmentSize) ? "s" : "");

      segment_info.shm_segsz = mSegmentSize;

      if (-1 == ::shmctl(mSegmentId, IPC_SET, &segment_info))
        logger.warn() << "SharedMemorySegment(): resize operation shmctl(" << mSegmentKey
                      << ", IPC_SET, &segment_info) failed with reason " << ::strerror(errno)
                      << " good look though";
      else
      {
        if (-1 == ::shmctl(mSegmentId, IPC_STAT, &segment_info)) {
          logger.warn() << "SharedMemorySegment(): shmctl(" << mSegmentKey << ", IPC_STAT, &segment_info) failed with reason: "
                        << ::strerror(errno);

          mSegmentSize = 0;

          throw std::runtime_error("Could not determine shared memory segment info!");
        }

        print_shm_segment_info (mSegmentKey, mSegmentId, "after resize, before shmat");

        if (mSegmentSize == segment_info.shm_segsz)
          logger.warn() << "SharedMemorySegment(): succesfully resized segment size to " << segment_info.shm_segsz
                        << " byte" << ((1 < segment_info.shm_segsz) ? "s" : "") << " for shm key <0x"
                        << mSegmentKey << "> (fd: " << mSegmentId << ")";
        else
        {
          logger.warn() << "SharedMemorySegment(): resizing segment size for shm key <0x" << mSegmentKey
                        << "> (fd: " << mSegmentId << ") failed; " << "again good look";
        }
      }
    }
    else
      logger.warn() << "SharedMemorySegment(): insufficient rights to resize segment size for shm key <0x"
                    << mSegmentKey << "> (fd: " << mSegmentId << "); " << "good look though";
  }

  // actually reference the shm segment
  mSegment = ::shmat(mSegmentId, 0, 0);

  if (!mSegment)
  {
    logger.warn() << "SharedMemorySegment(): attaching to shm key <0x"
                  << mSegmentKey << "> failed; reason: " << ::strerror(errno);

    mSegmentSize = 0;
    throw std::runtime_error("Could not attach to shared memory segment!");
  }

  print_shm_segment_info(mSegmentKey, mSegmentId, "after shmat");

  logger.info() << "SharedMemorySegment(): %s shm key <0x%x> @0x%x (fd: %d)",
                    (we_are_first ? "created" : "joined"), mSegmentKey, mSegment, mSegmentId;

  mCreated = we_are_first && mSegment;
}

av::daemon::SharedMemorySegment::~SharedMemorySegment()
{
  if (mSegment)
  {
    if (-1 == ::shmdt(mSegment))
      return;

    struct ::shmid_ds segment_info;

    if (-1 == ::shmctl(mSegmentId, IPC_STAT, &segment_info))
      return;

    // if there are no more processes attached then remove the segment
    // note: user must be the (effectively) owner, creator, or the super-user
    //       to do this

    const uid_t euid = ::geteuid();

    if (0 == segment_info.shm_nattch &&
        (segment_info.shm_perm.uid == euid || segment_info.shm_perm.cuid == euid || 0 == euid))
      if (-1 == ::shmctl(mSegmentId, IPC_RMID, 0))
        return;
  }
}

void*
av::daemon::SharedMemorySegment::segment() const
{
  return mSegment;
}

size_t
av::daemon::SharedMemorySegment::size() const
{
  return mSegmentSize;
}

bool
av::daemon::SharedMemorySegment::created() const
{
  return mCreated;
}
