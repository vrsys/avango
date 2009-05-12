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

#if !defined(AVANGO_SEMAPHORE_H)
#define AVANGO_SEMAPHORE_H

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include "windows_specific.h"

namespace av
{
  /**
   * Semaphore implementation using boost::mutex
   *
   * \ingroup av
   */
  class AV_DLL Semaphore
  {
    public:

    Semaphore(unsigned int initial = 1);
    ~Semaphore();

    /**
     * If semaphore value is > 0 then decrement it and carry on. If it's
     * already 0 then block.
     */
    void wait();

    /**
     * If semaphore value is > 0 then decrement it and return 1 (true).
     * If it's already 0 then return 0 (false).
     */
    int trywait();

    /**
     * If any threads are blocked in wait(), wake one of them up. Otherwise
     * increment the value of the semaphore.
     */
    void post();

    /**
     * Return semaphore value.
     */
    int snapshot();

  private:

    // dummy copy constructor and operator= to prevent copying
    Semaphore(const Semaphore&);
    const Semaphore& operator=(const Semaphore&);

    boost::condition mCondition;
    boost::mutex mMutex;
    int mValue;

  };

  /**
   * Scoped semaphore lock
   *
   * \ingroup av
   */
  class SemaphoreLock {

  public:

    SemaphoreLock(Semaphore& s) : mSemaphore(s)
    {
      mSemaphore.wait();
    }

    ~SemaphoreLock()
    {
      mSemaphore.post();
    }

  private:

    // dummy copy constructor and operator= to prevent copying
    SemaphoreLock(const SemaphoreLock&);
    const SemaphoreLock& operator=(const SemaphoreLock&);

    Semaphore& mSemaphore;

  };

}

#endif // #if !defined(AVANGO_SEMAPHORE_H)
