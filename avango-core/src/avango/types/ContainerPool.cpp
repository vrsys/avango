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

#include "avango/ContainerPool.h"

#include <iterator>

#include <avango/Assert.h>
#include <avango/logging/Logger.h>

#define AVANGO_CONTAINERPOOL_DEBUG
#undef AVANGO_CONTAINERPOOL_DEBUG

namespace
{
av::Logger& logger(av::getLogger("av::ContainerPool"));

} // namespace

/* static */ av::ContainerPool::InstancePoolType av::ContainerPool::sContainerPool;

/* static */ av::ContainerPool::NodeCreationCallbacksType av::ContainerPool::sCreationCallbacks;

/* static */ av::ContainerPool::NodeDeletionCallbacksType av::ContainerPool::sDeletionCallbacks;

/* static */ av::ContainerPool::FieldConnectCallbacksType av::ContainerPool::sConnectCallbacks;

/* static */ av::ContainerPool::FieldDisconnectCallbacksType av::ContainerPool::sDisconnectCallbacks;

/* static */ av::ContainerPool::FieldHasChangedCallbacksType av::ContainerPool::sFieldHasChangedCallbacks;

/* static */ av::ContainerPool::FieldContainerID av::ContainerPool::sLastId = 0;

/* static */ av::ContainerPool::NameToContainerMap av::ContainerPool::sNameContainerMap;

/* static */ av::ContainerPool::ContainerToNameMap av::ContainerPool::sContainerNameMap;

av::ContainerPool::FieldHasChangedReg::FieldHasChangedReg(FieldHasChangedCallback callback, av::Field* field, void* userData) : mCb(callback), mField(field), mUserData(userData) {}

/* static */ av::ContainerPool::FieldContainerID av::ContainerPool::registerInstance(av::FieldContainer* fieldContainer)
{
    AV_ASSERT(sLastId < sLastId + 1); // check overflow
    sContainerPool[++sLastId] = fieldContainer;
    return sLastId;
}

/* static */ void av::ContainerPool::unregisterInstance(av::FieldContainer* fieldContainer)
{
    sContainerPool.erase(fieldContainer->getId());
    removeNameForInstance(fieldContainer);
}

av::ContainerPool::NodeCreationCallbackID::NodeCreationCallbackID() : mCallbackIter() {}

av::ContainerPool::NodeCreationCallbackID::NodeCreationCallbackID(NodeCreationCallbacksType::iterator iter) : mCallbackIter(iter) {}

/* static */ av::ContainerPool::NodeCreationCallbackID av::ContainerPool::registerNodeCreationCallback(av::ContainerPool::NodeCreationCallback callback, void* userData)
{
    sCreationCallbacks.push_back(NodeCreationCallbacksType::value_type(callback, userData));

    NodeCreationCallbacksType::iterator last(sCreationCallbacks.end());

    std::advance(last, -1);

    return NodeCreationCallbackID(last);
}

/* static */ void av::ContainerPool::unregisterNodeCreationCallback(av::ContainerPool::NodeCreationCallbackID callbackID) { sCreationCallbacks.erase(callbackID.mCallbackIter); }

av::ContainerPool::NodeDeletionCallbackID::NodeDeletionCallbackID() : mCallbackIter() {}

av::ContainerPool::NodeDeletionCallbackID::NodeDeletionCallbackID(NodeDeletionCallbacksType::iterator iter) : mCallbackIter(iter) {}

/* static */ av::ContainerPool::NodeDeletionCallbackID av::ContainerPool::registerNodeDeletionCallback(av::ContainerPool::NodeDeletionCallback callback, void* userData)
{
    sDeletionCallbacks.push_back(NodeDeletionCallbacksType::value_type(callback, userData));

    NodeDeletionCallbacksType::iterator last(sDeletionCallbacks.end());

    std::advance(last, -1);

    return NodeDeletionCallbackID(last);
}

/* static */ void av::ContainerPool::unregisterNodeDeletionCallback(av::ContainerPool::NodeDeletionCallbackID callbackID) { sDeletionCallbacks.erase(callbackID.mCallbackIter); }

av::ContainerPool::FieldConnectCallbackID::FieldConnectCallbackID() : mCallbackIter() {}

av::ContainerPool::FieldConnectCallbackID::FieldConnectCallbackID(FieldConnectCallbacksType::iterator it) : mCallbackIter(it) {}

/* static */ av::ContainerPool::FieldConnectCallbackID av::ContainerPool::registerFieldConnectCallback(av::ContainerPool::FieldConnectCallback callback, void* userData)
{
    sConnectCallbacks.push_back(FieldConnectCallbacksType::value_type(callback, userData));
    FieldConnectCallbacksType::iterator last(sConnectCallbacks.end());
    std::advance(last, -1);
    return FieldConnectCallbackID(last);
}

/* static */ void av::ContainerPool::unregisterFieldConnectCallback(ContainerPool::FieldConnectCallbackID callbackID) { sConnectCallbacks.erase(callbackID.mCallbackIter); }

av::ContainerPool::FieldDisconnectCallbackID::FieldDisconnectCallbackID() : mCallbackIter() {}

av::ContainerPool::FieldDisconnectCallbackID::FieldDisconnectCallbackID(FieldDisconnectCallbacksType::iterator it) : mCallbackIter(it) {}

/* static */ av::ContainerPool::FieldDisconnectCallbackID av::ContainerPool::registerFieldDisconnectCallback(av::ContainerPool::FieldDisconnectCallback callback, void* userData)
{
    sDisconnectCallbacks.push_back(FieldDisconnectCallbacksType::value_type(callback, userData));
    FieldDisconnectCallbacksType::iterator last(sDisconnectCallbacks.end());
    std::advance(last, -1);
    return FieldDisconnectCallbackID(last);
}

/* static */ void av::ContainerPool::unregisterFieldDisconnectCallback(av::ContainerPool::FieldDisconnectCallbackID callbackID) { sDisconnectCallbacks.erase(callbackID.mCallbackIter); }

av::ContainerPool::FieldHasChangedCallbackID::FieldHasChangedCallbackID() : mCallbackIter() {}

av::ContainerPool::FieldHasChangedCallbackID::FieldHasChangedCallbackID(FieldHasChangedCallbacksType::iterator it) : mCallbackIter(it) {}

/* static */ av::ContainerPool::FieldHasChangedCallbackID av::ContainerPool::registerFieldHasChangedCallback(av::ContainerPool::FieldHasChangedCallback callback, Field* field, void* userData)
{
    sFieldHasChangedCallbacks.push_back(FieldHasChangedCallbacksType::value_type(callback, field, userData));
    FieldHasChangedCallbacksType::iterator last(sFieldHasChangedCallbacks.end());
    std::advance(last, -1);
    return FieldHasChangedCallbackID(last);
}

/* static */ void av::ContainerPool::unregisterFieldHasChangedCallback(av::ContainerPool::FieldHasChangedCallbackID callbackID) { sFieldHasChangedCallbacks.erase(callbackID.mCallbackIter); }

/* static */ av::FieldContainer* av::ContainerPool::getContainerById(av::ContainerPool::FieldContainerID id)
{
    InstancePoolType::const_iterator pool_iter = sContainerPool.find(id);

    if(sContainerPool.end() != pool_iter)
    {
        return pool_iter->second;
    }
    else
    {
        return 0;
    }
}

/* static */ void av::ContainerPool::setNameForInstance(av::FieldContainer* fieldContainer, const std::string& name)
{
    if(name.empty())
    {
        return; // don't add nodes without names
    }

    sNameContainerMap[name] = fieldContainer;
    sContainerNameMap[fieldContainer] = name;
}

/* static */ void av::ContainerPool::setNameForInstance(av::FieldContainer* fieldContainer, const char* name)
{
    if(!name)
    {
        return; // don't add nodes without names
    }

    sNameContainerMap[name] = fieldContainer;
    sContainerNameMap[fieldContainer] = name;
}

void av::ContainerPool::removeNameForInstance(av::FieldContainer* fieldContainer)
{
    ContainerToNameMap::iterator foundIter = sContainerNameMap.find(fieldContainer);

    if(foundIter == sContainerNameMap.end())
    {
        return;
    }

    std::string name = foundIter->second;

    sContainerNameMap.erase(fieldContainer);
    sNameContainerMap.erase(name);
}

/* static */ av::FieldContainer* av::ContainerPool::getInstanceByName(const std::string& name)
{
    NameToContainerMap::iterator foundIter;
    foundIter = sNameContainerMap.find(name);
    if(foundIter != sNameContainerMap.end())
    {
        return foundIter->second;
    }
    else
    {
        return 0;
    }
}

std::string av::ContainerPool::getNameByInstance(av::FieldContainer* fieldContainer)
{
    ContainerToNameMap::const_iterator foundIter;
    foundIter = sContainerNameMap.find(const_cast<av::FieldContainer*>(fieldContainer));
    if(foundIter != sContainerNameMap.end())
    {
        return foundIter->second;
    }
    else
    {
        return std::string();
    }
}

/*static*/ void av::ContainerPool::notifyConnect(av::Field* field)
{
    for(FieldConnectCallbacksType::iterator iter = sConnectCallbacks.begin(); iter != sConnectCallbacks.end(); ++iter)
    {
        if(iter->first)
        {
            (iter->first)(field, iter->second);
        }
    }
}

/*static*/ void av::ContainerPool::notifyDisconnect(av::Field* field)
{
    for(FieldDisconnectCallbacksType::iterator iter = sDisconnectCallbacks.begin(); iter != sDisconnectCallbacks.end(); ++iter)
    {
        if(iter->first)
        {
            (iter->first)(field, iter->second);
        }
    }
}

/*static*/ void av::ContainerPool::notifyFieldHasChanged(av::Field* field)
{
    for(FieldHasChangedCallbacksType::iterator iter = sFieldHasChangedCallbacks.begin(); iter != sFieldHasChangedCallbacks.end(); ++iter)
    {
        if(field == iter->mField)
            (iter->mCb)(field, iter->mUserData);
    }
}

/*static*/ void av::ContainerPool::notifyCreation(av::FieldContainer* fieldContainer)
{
    for(NodeCreationCallbacksType::iterator iter = sCreationCallbacks.begin(); iter != sCreationCallbacks.end(); ++iter)
    {
        if(iter->first)
        {
            (iter->first)(fieldContainer, iter->second);
        }
    }
}

/*static*/ void av::ContainerPool::notifyDeletion(av::FieldContainer* fieldContainer)
{
    for(NodeDeletionCallbacksType::iterator iter = sDeletionCallbacks.begin(); iter != sDeletionCallbacks.end(); ++iter)
    {
        if(iter->first)
        {
            (iter->first)(fieldContainer, iter->second);
        }
    }
}
