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

av::display::mt::MultitouchDevice::MultitouchDevice() : mOriginalPort(3333), mReceivedPort(mOriginalPort)
{
    AV_FC_ADD_FIELD(Added, MFMultitouchFinger::ContainerType());
    AV_FC_ADD_FIELD(Available, MFMultitouchFinger::ContainerType());
    AV_FC_ADD_FIELD(Removed, MFMultitouchFinger::ContainerType());

    AV_FC_ADD_FIELD(AddedUsers, MFMultitouchUser::ContainerType());
    AV_FC_ADD_FIELD(AvailableUsers, MFMultitouchUser::ContainerType());
    AV_FC_ADD_FIELD(RemovedUsers, MFMultitouchUser::ContainerType());

    AV_FC_ADD_FIELD(Filtering, false);
    AV_FC_ADD_FIELD(FilterSize, 0);
    AV_FC_ADD_FIELD(TimeIn, 0.0);

    mFingerClient = new FingerClient(mOriginalPort);

    if(mFingerClient)
    {
        mFingerClient->registerFingerListener(this);
        mFingerClient->registerUserListener(this);

        alwaysEvaluate(true);
    }

    mReceivedPortTime = getMilliseconds();
}

av::display::mt::MultitouchDevice::~MultitouchDevice()
{
    if(mFingerClient)
        delete mFingerClient;
}

void av::display::mt::MultitouchDevice::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();
        AV_FC_INIT(av::FieldContainer, av::display::mt::MultitouchDevice, true);
    }
}

void av::display::mt::MultitouchDevice::processMessages(std::queue<std::pair<FingerClient::MyMessage, int>>& messages)
{
    while(!messages.empty())
    {
        std::pair<FingerClient::MyMessage, int> message = messages.front();
        messages.pop();
        switch(message.first)
        {
        case FingerClient::MYMESS_SET_PORT:
            mReceivedPort = message.second;
            // mReceivedPortTime = timeGetTime();
            mReceivedPortTime = getMilliseconds();
            break;
        default:
            // todo: messages pushen
            break;
        };
    }
}

void av::display::mt::MultitouchDevice::handleFingers()
{
    mFingerClient->setPositionHistorySize(FilterSize.getValue());

    if(!Added.getValue().empty())
    {
        // hand over to Available
        MFMultitouchFinger::ContainerType::const_iterator iter = Added.getValue().begin();
        MFMultitouchFinger::ContainerType::const_iterator iter_end = Added.getValue().end();
        for(; iter != iter_end; ++iter)
            Available.add1Value(*iter);

        // clear Added
        Added.setValue(MFMultitouchFinger::ContainerType());
    }

    if(!Removed.getValue().empty())
    {
        // clear removed
        Removed.setValue(MFMultitouchFinger::ContainerType());
    }

    FingerMap::iterator iter = mFingerMap.begin();
    FingerMap::iterator iter_end = mFingerMap.end();

    float time = TimeIn.getValue();
    //::std::cerr << "Finger create: " << time << ::std::endl;

    while(iter != iter_end)
    {
        FingerAction& action = iter->second;
        if(action.do_add)
        {
            action.finger = new MultitouchFinger(action.finger_id, ::osg::Vec2(action.position.x, action.position.y), time, action.angle, action.width, action.height, action.area, action.user_id);
            Added.add1Value(action.finger);
            action.do_add = false;
        }
        else if(action.do_move)
        {
            action.finger->Position.setValue(::osg::Vec2(action.position.x, action.position.y));
            action.finger->Angle.setValue(action.angle);
            action.finger->Width.setValue(action.width);
            action.finger->Height.setValue(action.height);
            action.finger->Area.setValue(action.area);
            action.finger->UserID.setValue(action.user_id);
            action.do_move = false;
        }
        else if(action.do_remove)
        {
            action.finger->Position.setValue(::osg::Vec2(action.position.x, action.position.y));
            action.finger->Angle.setValue(action.angle);
            action.finger->Width.setValue(action.width);
            action.finger->Height.setValue(action.height);
            action.finger->Area.setValue(action.area);
            action.finger->UserID.setValue(action.user_id);
            Available.remove1Value(action.finger);
            Removed.add1Value(action.finger);
            mFingerMap.erase(iter++);
            action.do_remove = false;
            continue;
        }
        ++iter;
    }
}

void av::display::mt::MultitouchDevice::handleUsers()
{
    if(!AddedUsers.getValue().empty())
    {
        // hand over to Available
        MFMultitouchUser::ContainerType::const_iterator iter = AddedUsers.getValue().begin();
        MFMultitouchUser::ContainerType::const_iterator iter_end = AddedUsers.getValue().end();
        for(; iter != iter_end; ++iter)
            AvailableUsers.add1Value(*iter);

        // clear Added
        AddedUsers.setValue(MFMultitouchUser::ContainerType());
    }

    if(!RemovedUsers.getValue().empty())
    {
        // clear removed
        RemovedUsers.setValue(MFMultitouchUser::ContainerType());
    }

    UserMap::iterator iter = mUserMap.begin();
    UserMap::iterator iter_end = mUserMap.end();

    float time = TimeIn.getValue();

    while(iter != iter_end)
    {
        UserAction& action = iter->second;
        if(action.do_add)
        {
            action.user = new MultitouchUser(action.user_id, time);
            AddedUsers.add1Value(action.user);
            action.do_add = false;
        }
        else if(action.do_move)
        {
            action.do_move = false;
        }
        else if(action.do_remove)
        {
            AvailableUsers.remove1Value(action.user);
            RemovedUsers.add1Value(action.user);
            mUserMap.erase(iter++);
            action.do_remove = false;
            continue;
        }
        ++iter;
    }
}

/* virtual */ void av::display::mt::MultitouchDevice::evaluate()
{
    boost::mutex::scoped_lock mLock(mMutex);

    processMessages(mFingerClient->getMessages());

    //  if(mFingerClient->getPort() != mOriginalPort && (timeGetTime() - mReceivedPortTime) > 500.0) {
    if(mFingerClient->getPort() != mOriginalPort && (getMilliseconds() - mReceivedPortTime) > 500.0)
    {
        mReceivedPort = mOriginalPort;
    }

    if(mReceivedPort != mFingerClient->getPort())
    {
        if(mFingerClient)
            delete mFingerClient;
        mFingerClient = new FingerClient(mReceivedPort);
        if(mFingerClient)
        {
            mFingerClient->registerFingerListener(this);
            mFingerClient->registerUserListener(this);
        }
    }

    handleFingers();
    handleUsers();
}

void av::display::mt::MultitouchDevice::fingerAdded(FingerInfo* pFingerInfo)
{
    boost::mutex::scoped_lock mLock(mMutex);
    // FIXME: Between two frames (evaluate() calls), it can happen, that a finger id
    // is added again, before the former one was removed completely.
    // In this case this id is ignored.
    const int finger_id = pFingerInfo->getFingerId();
    FingerMap::iterator iter = mFingerMap.find(finger_id);
    if(iter == mFingerMap.end())
    {
        iter = mFingerMap.insert(FingerMap::value_type(finger_id, FingerAction(finger_id))).first;
        FingerAction& action = iter->second;
        action.position = Filtering.getValue() ? pFingerInfo->getFilteredPosition() : pFingerInfo->getPosition();
        action.angle = pFingerInfo->getAngle();
        action.width = pFingerInfo->getWidth();
        action.height = pFingerInfo->getHeight();
        action.area = pFingerInfo->getArea();
        action.user_id = pFingerInfo->getUserId();
        action.do_add = true;
    }
}

void av::display::mt::MultitouchDevice::fingerMoved(FingerInfo* pFingerInfo)
{
    boost::mutex::scoped_lock mLock(mMutex);
    const int finger_id = pFingerInfo->getFingerId();
    FingerMap::iterator iter = mFingerMap.find(finger_id);
    if(iter != mFingerMap.end())
    {
        FingerAction& action = iter->second;
        action.position = Filtering.getValue() ? pFingerInfo->getFilteredPosition() : pFingerInfo->getPosition();
        action.angle = pFingerInfo->getAngle();
        action.width = pFingerInfo->getWidth();
        action.height = pFingerInfo->getHeight();
        action.area = pFingerInfo->getArea();
        action.user_id = pFingerInfo->getUserId();
        action.do_move = true;
    }
}

void av::display::mt::MultitouchDevice::fingerRemoved(FingerInfo* pFingerInfo)
{
    boost::mutex::scoped_lock mLock(mMutex);
    const int finger_id = pFingerInfo->getFingerId();
    FingerMap::iterator iter = mFingerMap.find(finger_id);
    if(iter != mFingerMap.end())
    {
        FingerAction& action = iter->second;
        if(action.do_add)
            // Avango should not see fingers that are deleted before being added officially, so we delete them here directly
            mFingerMap.erase(iter);
        else
        {
            // if a finger was added officially, it has to go through all the states correctly, so tag them here for remove
            action.position = Filtering.getValue() ? pFingerInfo->getFilteredPosition() : pFingerInfo->getPosition();
            action.angle = pFingerInfo->getAngle();
            action.width = pFingerInfo->getWidth();
            action.height = pFingerInfo->getHeight();
            action.area = pFingerInfo->getArea();
            action.user_id = pFingerInfo->getUserId();
            action.do_remove = true;
        }
    }
}

void av::display::mt::MultitouchDevice::userAdded(UserInfo* pUserInfo)
{
    boost::mutex::scoped_lock mLock(mMutex);
    // FIXME: Between two frames (evaluate() calls), it can happen, that a finger id
    // is added again, before the former one was removed completely.
    // In this case this id is ignored.
    const int user_id = pUserInfo->getUserId();
    UserMap::iterator iter = mUserMap.find(user_id);
    if(iter == mUserMap.end())
    {
        iter = mUserMap.insert(UserMap::value_type(user_id, UserAction(user_id))).first;
        UserAction& action = iter->second;
        action.do_add = true;
    }
}

void av::display::mt::MultitouchDevice::userMoved(UserInfo* pUserInfo)
{
    boost::mutex::scoped_lock mLock(mMutex);
    const int user_id = pUserInfo->getUserId();
    UserMap::iterator iter = mUserMap.find(user_id);
    if(iter != mUserMap.end())
    {
        UserAction& action = iter->second;
        action.do_move = true;
    }
}

void av::display::mt::MultitouchDevice::userRemoved(UserInfo* pUserInfo)
{
    boost::mutex::scoped_lock mLock(mMutex);
    const int user_id = pUserInfo->getUserId();
    UserMap::iterator iter = mUserMap.find(user_id);
    if(iter != mUserMap.end())
    {
        UserAction& action = iter->second;
        if(action.do_add)
            // Avango should not see fingers that are deleted before being added officially, so we delete them here directly
            mUserMap.erase(iter);
        else
        {
            action.do_remove = true;
        }
    }
}
