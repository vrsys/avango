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

#include "UpcallSerializer.h"

#include <avango/NetNode.h>
#include <boost/version.hpp>
#include <boost/static_assert.hpp>

#define AVANGO_UPCALLSERIALIZER_DEBUG 4
#undef AVANGO_UPCALLSERIALIZER_DEBUG

#if defined(AVANGO_UPCALLSERIALIZER_DEBUG)
#  include <iostream>
#  include <typeinfo>
#endif

av::Upcall::Upcall()
{
#if defined(AVANGO_UPCALLSERIALIZER_DEBUG)
  std::cerr << "Upcall::Upcall(): @0x" << static_cast<const void*> (this) << " ..." << std::endl;
#endif
}

av::Upcall::~Upcall()
{
#if defined(AVANGO_UPCALLSERIALIZER_DEBUG)
  std::cerr << "Upcall::~Upcall(): @0x" << static_cast<const void*> (this) << " ..." << std::endl;
#endif
}

// called from net process
void
av::JoinUpcall::upcallQueued(UpcallSerializer&)
{}

// called from app process
void
av::JoinUpcall::handle(NetNode* netNode, UpcallSerializer&)
{
  netNode->joined(mEID);

}

// called from net process
void
av::SetStateUpcall::upcallQueued(UpcallSerializer&)
{}

// called from app process
void
av::SetStateUpcall::handle(NetNode* netNode, UpcallSerializer&)
{
  std::cerr << "SetStateUpcall::handle - netNode->setStateFragment(mFragment, mStateMsg)" << std::endl;
  netNode->setStateFragment(mFragment, mStateMsg);

}

// called from net process
void
av::GetStateUpcall::upcallQueued(UpcallSerializer& serializer)
{
  serializer.netWait();
}

// called from app process
void
av::GetStateUpcall::handle(NetNode* netNode, UpcallSerializer& serializer)
{
  std::cerr << "SetStateUpcall::handle - netNode->getStateFragment(mFragment, mStateMsg)" << std::endl;
  netNode->getStateFragment(mFragment, mStateMsg);
  serializer.signalWait();
}

// called from net process
void
av::RemoveStateUpcall::upcallQueued(UpcallSerializer&)
{}

// called from app process
void
av::RemoveStateUpcall::handle(NetNode* netNode, UpcallSerializer&)
{
  netNode->removeStateFragment(mFragment);

}

// called from net process
void
av::MessageUpcall::upcallQueued(UpcallSerializer&)
{}

// called from app process
void
av::MessageUpcall::handle(NetNode* netNode, UpcallSerializer&)
{
  Msg sm(mMsg);
  sm.setNetNode(netNode);
  netNode->consumeMessage(sm);

}

// called from net process
void
av::AcceptedViewUpcall::upcallQueued(UpcallSerializer&)
{}

// called from app process
void
av::AcceptedViewUpcall::handle(NetNode* netNode, UpcallSerializer&)
{
  netNode->acceptNewView(mMembers, mNewMembers, mDepartedMembers, mMsg);

}

// called from net process
void
av::BlockUpcall::upcallQueued(UpcallSerializer& serializer)
{
  serializer.netWait();
}

// called from app process
void
av::BlockUpcall::handle(NetNode* netNode, UpcallSerializer& serializer)
{
  netNode->block();
  serializer.signalWait();
}

// called from net process
void
av::UnblockUpcall::upcallQueued(UpcallSerializer& /* serializer */)
{
  // serializer.net_wait();
}

// called from app process
void
av::UnblockUpcall::handle(NetNode* netNode, UpcallSerializer& /* serializer */)
{
  netNode->unblock();
  // serializer.signal_wait();
}

// called from net process
void
av::ExitUpcall::upcallQueued(UpcallSerializer& serializer)
{
  serializer.netWait();
}

// called from app process
void
av::ExitUpcall::handle(NetNode* netNode, UpcallSerializer& serializer)
{
  netNode->exitCompleted();
  serializer.signalWait();
}

av::UpcallSerializer::UpcallSerializer()
  : mUpcallQueueLock(),
    mUpcallQueue(),
    mSyncSema(0)
{
#if defined(AVANGO_UPCALLSERIALIZER_DEBUG)
  std::cerr << "UpcallSerializer::UpcallSerializer(): "
            << "w/ sema @" << &mSyncSema << " (val: " << mSyncSema.snapshot() << ") ..."
            << std::endl;
#endif
}

av::UpcallSerializer::~UpcallSerializer()
{
#if defined(AVANGO_UPCALLSERIALIZER_DEBUG)
  std::cerr << "UpcallSerializer::~UpcallSerializer(): "
            << "w/ sema @" << &mSyncSema << " (val: " << mSyncSema.snapshot() << ") ..."
            << std::endl;
#endif
}

#if defined(BOOST_SP_DISABLE_THREADS)
#define BOOST_SHAREDPTR_THREAD_SUPPORT_ENABLED false
#else
#define BOOST_SHAREDPTR_THREAD_SUPPORT_ENABLED true
#endif

#define BOOST_IS_RIGHT_VERSION (BOOST_VERSION >= 103300)

// called from net process
void
av::UpcallSerializer::makeUpcall(boost::shared_ptr<Upcall> upcall)
{
#if defined(AVANGO_UPCALLSERIALIZER_DEBUG)
  std::cerr << "UpcallSerializer::make_upcall(): "
            << "'" << typeid(upcall).name() << "' @0x" << static_cast<const void*> (upcall)
            << " ..."
            << std::endl;
#endif

  BOOST_STATIC_ASSERT(BOOST_IS_RIGHT_VERSION && BOOST_SHAREDPTR_THREAD_SUPPORT_ENABLED);

  {
    boost::mutex::scoped_lock lock(mUpcallQueueLock);

    mUpcallQueue.push_back(upcall);
  }

  upcall->upcallQueued(*this);
}

// called from app process
void
av::UpcallSerializer::handleUpcall(boost::shared_ptr<Upcall> upcall, NetNode* netNode)
{
#if defined(AVANGO_UPCALLSERIALIZER_DEBUG)
  std::cerr << "UpcallSerializer::handle_upcall(): "
            << "'" << typeid(upcall).name() << "' @0x" << static_cast<const void*> (upcall)
            << " ..."
            << std::endl;
#endif

  upcall->handle(netNode, *this);
}

// called from app process
bool
av::UpcallSerializer::handleNextUpcall(NetNode* netNode)
{
  boost::shared_ptr<Upcall> upcall;

  {
    boost::mutex::scoped_lock lock(mUpcallQueueLock);

    if (!mUpcallQueue.empty()) {
      upcall = mUpcallQueue.front();

      mUpcallQueue.pop_front();
    }
  }

  if (upcall) {
    handleUpcall(upcall, netNode);

    return true;
  }

  return false;
}

void
av::UpcallSerializer::netWait()
{
#if defined(AVANGO_UPCALLSERIALIZER_DEBUG)
  std::cerr << "UpcallSerializer::net_wait(): "
            << "waiting on sema @" << &mSyncSema
            << " (val: " << mSyncSema.snapshot() << ") ..."
            << std::endl;
#endif

  mSyncSema.wait();

#if defined(AVANGO_UPCALLSERIALIZER_DEBUG)
  std::cerr << "UpcallSerializer::net_wait(): "
            << "done waiting on sema @" << &mSyncSema << " ..."
            << std::endl;
#endif
}

void
av::UpcallSerializer::signalWait()
{
#if defined(AVANGO_UPCALLSERIALIZER_DEBUG)
  std::cerr << "UpcallSerializer::signal_wait(): "
            << "sleeping while <snapshot() == 0> for sema @" << &mSyncSema
            << " (val: " << mSyncSema.snapshot() << ") ..."
            << std::endl;
#endif

  // wait until a wait has really been performed
  while (mSyncSema.snapshot() == 0) {
    usleep(1);
  }

#if defined(AVANGO_UPCALLSERIALIZER_DEBUG)
  std::cerr << "UpcallSerializer::signal_wait(): "
            << "done sleeping on sema @" << &mSyncSema
            << " (val: " << mSyncSema.snapshot() << ") ..."
            << std::endl;
#endif

  mSyncSema.post();

#if defined(AVANGO_UPCALLSERIALIZER_DEBUG)
  std::cerr << "UpcallSerializer::signal_wait(): "
            << "posted sema @" << &mSyncSema
            << " (val: " << mSyncSema.snapshot() << ") ..."
            << std::endl;
#endif
}
