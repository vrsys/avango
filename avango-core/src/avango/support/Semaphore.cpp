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

#include "avango/Semaphore.h"

av::Semaphore::Semaphore(unsigned int initial) { mValue = initial; }

av::Semaphore::~Semaphore() {}

void av::Semaphore::wait()
{
    boost::mutex::scoped_lock mLock(mMutex);

    --mValue;

    while(mValue < 0)
    {
        mCondition.wait(mLock);
    }
}

int av::Semaphore::trywait()
{
    boost::mutex::scoped_lock mLock(mMutex);

    if(mValue == 0)
    {
        return 0;
    }
    else
    {
        --mValue;
        return 1;
    }
}

void av::Semaphore::post()
{
    boost::mutex::scoped_lock mLock(mMutex);

    ++mValue;
    mLock.unlock();
    mCondition.notify_one();
}

int av::Semaphore::snapshot()
{
    boost::mutex::scoped_lock mLock(mMutex);
    int result = mValue;
    return result;
}
