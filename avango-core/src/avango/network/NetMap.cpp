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

#include "avango/NetMap.h"

#include <iostream>

#include <avango/logging/Logger.h>
#include <avango/NetID.h>

#include "Helper.h"

#define FP_NET_DEBUG 3
#undef FP_NET_DEBUG

namespace
{
av::Logger& logger(av::getLogger("av::NetMap"));
}

av::NetMap::NetMap() : mEIDMap() {}

av::NetMap::~NetMap() {}

void av::NetMap::addSlot(const std::string& eid)
{
#if FP_NET_DEBUG > 1
    std::cout << "NetMap::add_slot: "
              << "for eid: " << eid << std::endl;
#endif

    // make sure it doesn't already exist
    if(mEIDMap.find(eid) == mEIDMap.end())
        mEIDMap[eid] = IntDstMap();
    else
        logger.warn() << "add_slot: '%s' already added", eid.c_str();
}

void av::NetMap::removeSlot(const std::string& eid)
{
#if FP_NET_DEBUG > 1
    std::cout << "NetMap::remove_slot: "
              << "for eid: " << eid << std::endl;
#endif

    // make sure it's there
    EIDMapMap::iterator i = mEIDMap.find(eid);
    if(i != mEIDMap.end())
        mEIDMap.erase(i);
    else
        logger.warn() << "remove_slot: '%s' not yet added", eid.c_str();
}

bool av::NetMap::slotExists(const std::string& eid) const { return (mEIDMap.find(eid) != mEIDMap.end()); }

void av::NetMap::registerObj(const av::Link<Distributed>& obj)
{
#if FP_NET_DEBUG > 1
    std::cout << "NetMap::register_obj: "
              << "id: " << obj->netID() << std::endl;
#endif

    AV_ASSERT(obj->isDistributed());
    const std::string& eid(obj->netID().getEID());
    int num(obj->netID().getNum());

    // make sure the slot exists, but ...
    EIDMapMap::iterator i = mEIDMap.find(eid);

    if(i == mEIDMap.end())
    {
        std::cout << "NetMap::register_obj: "
                  << "slot not found for: " << obj->netID() << std::endl;
    }

    // ... the object doesn't.
    IntDstMap& dst_map = (*i).second;

    if(dst_map.find(num) != dst_map.end())
    {
        logger.warn() << "register_obj: "
                      << "'%s' of type '%s' cannot be registered twice!",
            getName(obj), obj->getTypeId().getName().c_str();

        return;
    }

    // insert the object
    dst_map[num] = obj;
}

void av::NetMap::unregisterObj(const av::Link<Distributed>& obj)
{
#if FP_NET_DEBUG > 1
    std::cout << "NetMap::unregister_obj: "
              << "id: " << obj->netID() << std::endl;
#endif

    // assert(obj->is_distributed());
    if(!obj->isDistributed())
    {
        logger.info() << "unregister_obj: "
                      << "'%s' of type '%s' not yet distributed!",
            getName(obj), obj->getTypeId().getName().c_str();

        return;
    }

    const std::string& eid = obj->netID().getEID();
    int num = obj->netID().getNum();

    // make sure the slot exists, and ...
    EIDMapMap::iterator i = mEIDMap.find(eid);

    if(mEIDMap.end() != i)
    {
        // the object does also!
        IntDstMap& dst_map = (*i).second;
        IntDstMap::iterator j = dst_map.find(num);

        if(dst_map.end() != j)
        {
            // remove the object
            dst_map.erase(j);
        }
        else
        {
            logger.warn() << "unregister_obj: "
                          << "'%s' of type '%s' does not exist in destination map!",
                getName(obj), obj->getTypeId().getName().c_str();
        }
    }
    else
    {
        logger.info() << "unregister_obj: "
                      << "'%s' of type '%s' does not exist!",
            getName(obj), obj->getTypeId().getName().c_str();
    }
}

bool av::NetMap::lookup(const av::NetID& id, av::Link<Distributed>& result) const
{
    // return a single object, object not found is illegal
    const std::string& eid = id.getEID();
    int num = id.getNum();

    // make sure the slot exists, and ...
    EIDMapMap::const_iterator i = mEIDMap.find(eid);
    if(i == mEIDMap.end())
    {
#if FP_NET_DEBUG > 2
        std::cout << "NetMap::lookup: "
                  << "failed for id: " << id << std::endl;
#endif
        return false;
    }

    // the object does also!
    const IntDstMap& dst_map = (*i).second;
    IntDstMap::const_iterator j = dst_map.find(num);
    if(j == dst_map.end())
    {
#if FP_NET_DEBUG > 2
        std::cout << "NetMap::lookup: "
                  << "failed for id: " << id << std::endl;
#endif
        return false;
    }

#if FP_NET_DEBUG > 2
    std::cout << "NetMap::lookup: "
              << "succeeded for id: " << id << std::endl;
#endif

    result = (*j).second;
    return true;
}

bool av::NetMap::lookup(const std::string& eid, std::vector<av::Link<av::Distributed>>& result) const
{
    // return all objects with the matching eid
    // make sure the slot exists, and ...
    EIDMapMap::const_iterator i = mEIDMap.find(eid);
    if(i == mEIDMap.end())
    {
#if FP_NET_DEBUG > 2
        std::cout << "NetMap::lookup: "
                  << "failed for eid: " << eid << std::endl;
#endif
        return false;
    }

    // ... copy all objects into a std::vector
    const IntDstMap& dst_map = (*i).second;
    IntDstMap::const_iterator j;
    result.clear();
    for(j = dst_map.begin(); j != dst_map.end(); ++j)
        result.push_back((*j).second);

#if FP_NET_DEBUG > 2
    std::cout << "NetMap::lookup: "
              << "succeeded for eid: " << eid << std::endl;
#endif

    return true;
}

bool av::NetMap::lookupExcept(const std::string& eid, std::vector<av::Link<av::Distributed>>& result) const
{
    // return all objects except the ones with the matching eid
    // make sure the slot exists, and ...
    EIDMapMap::const_iterator not_this = mEIDMap.find(eid);
    if(not_this == mEIDMap.end())
    {
#if FP_NET_DEBUG > 2
        std::cout << "NetMap::lookup_except: "
                  << "excepted eid not there anyway: " << eid << std::endl;
#endif
        return false;
    }

    // ... copy all objects into a std::vector
    result.clear();
    EIDMapMap::const_iterator i;
    for(i = mEIDMap.begin(); i != mEIDMap.end(); ++i)
    {
        if(i != not_this)
        {
            const IntDstMap& dst_map = (*i).second;
            IntDstMap::const_iterator j;
            for(j = dst_map.begin(); j != dst_map.end(); j++)
                result.push_back((*j).second);
        }
    }
#if FP_NET_DEBUG > 2
    std::cout << "NetMap::lookup_except: "
              << "succeeded for eid: " << eid << std::endl;
#endif

    return true;
}

bool av::NetMap::lookup(std::vector<av::Link<av::Distributed>>& result) const
{
    result.clear();
    EIDMapMap::const_iterator i;
    for(i = mEIDMap.begin(); i != mEIDMap.end(); i++)
    {
        const IntDstMap& dst_map = (*i).second;
        IntDstMap::const_iterator j;
        for(j = dst_map.begin(); j != dst_map.end(); j++)
            result.push_back((*j).second);
    }
    return true;
}

void av::NetMap::removeAllSlots() { mEIDMap.clear(); }

void av::NetMap::removeAllSlotsExcept(const std::string& eid)
{
    EIDMapMap::const_iterator i = mEIDMap.find(eid);
    AV_ASSERT(i != mEIDMap.end());
    IntDstMap dst_map = (*i).second; // hardcore copy
    mEIDMap.clear();
    mEIDMap[eid] = dst_map; // hardcore copy
}

void av::NetMap::emptyAllSlots()
{
    EIDMapMap::iterator i;
    for(i = mEIDMap.begin(); i != mEIDMap.end(); i++)
    {
        IntDstMap& dst_map = (*i).second;
        dst_map.clear();
    }
}

void av::NetMap::emptyAllSlotsExcept(const std::string& eid)
{
    EIDMapMap::iterator i = mEIDMap.find(eid);
    AV_ASSERT(i != mEIDMap.end());

    for(i = mEIDMap.begin(); i != mEIDMap.end(); i++)
        if(eid != (*i).first)
        {
            IntDstMap& dst_map = (*i).second;
            dst_map.clear();
        }
}

void av::NetMap::emptySlot(const std::string& eid)
{
    EIDMapMap::iterator i = mEIDMap.find(eid);
    if(i != mEIDMap.end())
    {
        IntDstMap& dst_map = (*i).second;
        dst_map.clear();
    }
}

void av::NetMap::dump(std::ostream& os) const
{
    if(!mEIDMap.empty())
    {
        EIDMapMap::const_iterator i;

        os << "NetMap::dump>: " << mEIDMap.size() << " entries" << std::endl;

        for(i = mEIDMap.begin(); i != mEIDMap.end(); i++)
        {
            const std::string& eid = (*i).first;
            os << "    eid: " << eid << std::endl;
            const IntDstMap& dst_map = (*i).second;
            IntDstMap::const_iterator j;
            for(j = dst_map.begin(); j != dst_map.end(); j++)
            {
                int num = (*j).first;
                os << "         " << num << std::endl;
            }
        }
    }
}
