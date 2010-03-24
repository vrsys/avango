// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include <iostream>

#include <avango/Logger.h>
#include <avango/display/mt/MultitouchDevice.h>



namespace
{
av::Logger& logger(av::getLogger("av::display::mt::MultitouchDevice"));
}


AV_FC_DEFINE(av::display::mt::MultitouchDevice);

av::display::mt::MultitouchDevice::MultitouchDevice()
{
  AV_FC_ADD_FIELD(Added, MFMultitouchFinger::ContainerType());
  AV_FC_ADD_FIELD(Available, MFMultitouchFinger::ContainerType());
  AV_FC_ADD_FIELD(Removed, MFMultitouchFinger::ContainerType());
  AV_FC_ADD_FIELD(Filtering, false);
  AV_FC_ADD_FIELD(FilterSize, 0);
  AV_FC_ADD_FIELD(TimeIn, 0.0);

  mFingerClient = new FingerClient(3333);

  if (mFingerClient)
  {
    mFingerClient->registerFingerListener(this);

    alwaysEvaluate(true);
  }
}

av::display::mt::MultitouchDevice::~MultitouchDevice()
{
  if (mFingerClient)
    delete mFingerClient;
}

void
av::display::mt::MultitouchDevice::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();
    AV_FC_INIT(av::FieldContainer, av::display::mt::MultitouchDevice, true);
  }
}

/* virtual */ void
av::display::mt::MultitouchDevice::evaluate()
{
  boost::mutex::scoped_lock mLock(mMutex);

  mFingerClient->setPositionHistorySize(FilterSize.getValue());

  if (!Added.getValue().empty())
  {
    // hand over to Available
    MFMultitouchFinger::ContainerType::const_iterator iter = Added.getValue().begin();
    MFMultitouchFinger::ContainerType::const_iterator iter_end = Added.getValue().end();
    for (; iter != iter_end; ++iter)
      Available.add1Value(*iter);

    // clear Added
    Added.setValue(MFMultitouchFinger::ContainerType());
  }

  if (!Removed.getValue().empty())
  {
    // clear removed
    Removed.setValue(MFMultitouchFinger::ContainerType());
  }

  FingerMap::iterator iter = mFingerMap.begin();
  FingerMap::iterator iter_end = mFingerMap.end();

  float time = TimeIn.getValue();
  //::std::cerr << "Finger create: " << time << ::std::endl;

  while (iter != iter_end)
  {
    FingerAction &action = iter->second;
    if (action.do_add)
    {
      action.finger = new MultitouchFinger(action.finger_id, ::osg::Vec2(action.position.x, action.position.y), time);
      Added.add1Value(action.finger);
      action.do_add = false;
    }
    else if (action.do_move)
    {
      action.finger->Position.setValue(::osg::Vec2(action.position.x, action.position.y));
      action.do_move = false;
    }
    else if (action.do_remove)
    {
      action.finger->Position.setValue(::osg::Vec2(action.position.x, action.position.y));
      Available.remove1Value(action.finger);
      Removed.add1Value(action.finger);
      mFingerMap.erase(iter++);
      action.do_remove = false;
      continue;
    }
    ++iter;
  }
}

void
av::display::mt::MultitouchDevice::fingerAdded(FingerInfo *pFingerInfo)
{
  boost::mutex::scoped_lock mLock(mMutex);
  // FIXME: Between two frames (evaluate() calls), it can happen, that a finger id
  // is added again, before the former one was removed completely.
  // In this case this id is ignored.
  const int finger_id = pFingerInfo->getFingerId();
  FingerMap::iterator iter = mFingerMap.find(finger_id);
  if (iter == mFingerMap.end())
  {
    iter = mFingerMap.insert(FingerMap::value_type(finger_id, FingerAction(finger_id))).first;
    FingerAction &action = iter->second;
    action.position = Filtering.getValue() ? pFingerInfo->getFilteredPosition() : pFingerInfo->getPosition();
    action.do_add = true;
  }
}

void
av::display::mt::MultitouchDevice::fingerMoved(FingerInfo *pFingerInfo)
{
  boost::mutex::scoped_lock mLock(mMutex);
  const int finger_id = pFingerInfo->getFingerId();
  FingerMap::iterator iter = mFingerMap.find(finger_id);
  if (iter != mFingerMap.end())
  {
    FingerAction &action = iter->second;
    action.position = Filtering.getValue() ? pFingerInfo->getFilteredPosition() : pFingerInfo->getPosition();
    action.do_move = true;
  }
}

void
av::display::mt::MultitouchDevice::fingerRemoved(FingerInfo *pFingerInfo)
{
  boost::mutex::scoped_lock mLock(mMutex);
  const int finger_id = pFingerInfo->getFingerId();
  FingerMap::iterator iter = mFingerMap.find(finger_id);
  if (iter != mFingerMap.end())
  {
    FingerAction &action = iter->second;
    if (action.do_add)
      // Avango should not see fingers that are deleted before being added officially, so we delete them here directly
      mFingerMap.erase(iter);
    else
    {
      // if a finger was added officially, it has to go through all the states correctly, so tag them here for remove
      action.position = Filtering.getValue() ? pFingerInfo->getFilteredPosition() : pFingerInfo->getPosition();
      action.do_remove = true;
    }
  }
}
