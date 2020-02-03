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

#include "avango/gua/network/NetTransform.h"

#include <avango/gua/network/SharedContainerHolder.h>

#include <avango/Application.h>
#include <functional>

#include <avango/Logger.h>

namespace
{
av::Logger& logger(av::getLogger("av::gua::NetTransform"));
}

AV_FC_DEFINE(av::gua::NetTransform);

AV_FIELD_DEFINE(av::gua::SFNetTransform);
AV_FIELD_DEFINE(av::gua::MFNetTransform);

av::gua::NetTransform::NetTransform() : TransformNode(), NetNode()
{
    // the name of the network goup to join
    AV_FC_ADD_FIELD(Groupname, "");
	AV_FC_ADD_FIELD(TotalNumClients, 1);

    // the list of current group members.
    AV_FC_ADD_FIELD(Members, std::vector<std::string>());
    AV_FC_ADD_FIELD(NewMembers, std::vector<std::string>());
    AV_FC_ADD_FIELD(DepartedMembers, std::vector<std::string>());
    AV_FC_ADD_FIELD(NetId, std::string());
    AV_FC_ADD_ADAPTOR_FIELD(
        SharedContainers, std::bind(&NetTransform::getSharedContainersCB, this, std::placeholders::_1), std::bind(&NetTransform::setSharedContainersCB, this, std::placeholders::_1));

    Groupname.dontDistribute(true);
    TotalNumClients.dontDistribute(true);
    Members.dontDistribute(true);
    NewMembers.dontDistribute(true);
    DepartedMembers.dontDistribute(true);
    NetId.dontDistribute(true);

    mPreEvalHandle = ApplicationInstance::get().addPreEvaluationContainerCallback(std::bind(&NetTransform::handleNetworkReceives, this));
    mPostEvalHandle = ApplicationInstance::get().addPostEvaluationContainerCallback(std::bind(&NetTransform::handleNetworkSends, this));
}

/* virtual */
av::gua::NetTransform::~NetTransform()
{
    ApplicationInstance::get().removeCallback(mPreEvalHandle);
    ApplicationInstance::get().removeCallback(mPostEvalHandle);
}

/* static */ void av::gua::NetTransform::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::TransformNode::initClass();

        AV_FC_INIT(av::gua::TransformNode, av::gua::NetTransform, true);

        SFNetTransform::initClass("av::gua::SFNetTransform", "av::Field");
        MFNetTransform::initClass("av::gua::MFNetTransform", "av::Field");

        sClassTypeId.setDistributable(true);
    }
}

/* virtual */ void av::gua::NetTransform::fieldHasChangedLocalSideEffect(const Field& field)
{
    TransformNode::fieldHasChangedLocalSideEffect(field);

    if(&field == &Groupname)
    {
        if(Groupname.getValue().size())
        {
            AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fpNetDCS::fieldHasChangedLocalSideEffect: joining net-group '%1%'.") % Groupname.getValue().c_str()));
            // if the groupname is not empty try to join
            join(Groupname.getValue(), TotalNumClients.getValue());
            // Name.setValue(Groupname.getValue());
        }
        else
        {
            // get the hell out of here
            leave();
        }
    }
}

/* virtual */ void av::gua::NetTransform::evaluate() { TransformNode::evaluate(); }

/* virtual */ void av::gua::NetTransform::refImpl() {}

/* virtual */ void av::gua::NetTransform::unrefImpl() {}

/* virtual */ int av::gua::NetTransform::refCountImpl() { return 1; }

/* virtual */ void av::gua::NetTransform::_join(const std::string& fragment)
{
    NetID group_id(fragment, NetID::sNetGroupRootNode);
    registerWellKnown(this, group_id);

    mGroupMap.insert(fragment);

    // register SharedContainerHolder
    Link<SharedContainerHolder> container_holder = new SharedContainerHolder;

    NetID container_holder_id(fragment, NetID::sNetGroupContainerHolder);
    registerWellKnown(container_holder.getPtr(), container_holder_id);

    mSharedContainerMap[fragment] = container_holder;
    sharedContainersChanged();

#ifdef AVANGO_DEBUG
    AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("_join: added group node %1% for: %2%]") % group_id % fragment));
#endif
}

// overloaded from fp_net_node to react to view changes
/* virtual */ void av::gua::NetTransform::_acceptNewView(const std::vector<std::string>& members, const std::vector<std::string>& newMembers, const std::vector<std::string>& departedMembers)
{
    Members.setValue(members);
    NewMembers.setValue(newMembers);
    DepartedMembers.setValue(departedMembers);

    std::string id(netEID());
    if(id != NetId.getValue())
    {
        NetId.setValue(id);
    }
}

/* virtual */ void av::gua::NetTransform::_getStateFragment(const std::string& fragment, Msg& stateMsg)
{
    // send an update message for the well-known group node of this fragment
    EIDGrpMap::iterator i = mGroupMap.find(fragment);

    if(i != mGroupMap.end())
    {
        NetID group_id(fragment, NetID::sNetGroupRootNode);
        registerWellKnown(this, group_id);

        mGroupMap.insert(fragment);
    }

    stateMsg.setType(Msg::absolute);
    makeUpdateMessage(stateMsg, this);

    Link<SharedContainerHolder> container_holder;
    // send an update message for the well-known shared container  node of this fragment
    SharedContainerMap::iterator container_iter = mSharedContainerMap.find(fragment);
    if(container_iter != mSharedContainerMap.end())
    {
        container_holder = (*container_iter).second;
    }
    else
    {
        // a new node is needed
        container_holder = new SharedContainerHolder;
        container_holder->Name.setValue(fragment);

        NetID container_holder_id(fragment, NetID::sNetGroupContainerHolder);
        registerWellKnown(container_holder.getPtr(), container_holder_id);

        mSharedContainerMap[fragment] = container_holder;
        container_holder->registerNetTransform(this);
        sharedContainersChanged();
    }
    stateMsg.setType(Msg::absolute);
    makeUpdateMessage(stateMsg, container_holder.getPtr());
}

/* virtual */ void av::gua::NetTransform::_setStateFragment(const std::string& fragment, Msg& stateMsg)
{
    // created a new fpFragmentGroup node and register with the correct well-known id.
    // it consists of the endpoint-id of the server and a constant number.

    EIDGrpMap::iterator found = mGroupMap.find(fragment);
    if(found == mGroupMap.end())
    {
        NetID group_id(fragment, NetID::sNetGroupRootNode);
        registerWellKnown(this, group_id);

        mGroupMap.insert(fragment);
    }

    // send an update message for the well-known shared container  node of this fragment
    SharedContainerMap::iterator container_iter = mSharedContainerMap.find(fragment);
    if(container_iter == mSharedContainerMap.end())
    {
        Link<SharedContainerHolder> container_holder = new SharedContainerHolder;
        NetID container_holder_id(fragment, NetID::sNetGroupContainerHolder);
        registerWellKnown(container_holder.getPtr(), container_holder_id);
        mSharedContainerMap[fragment] = container_holder;
        sharedContainersChanged();
    }
    // consume the state message
    consumeMessage(stateMsg);
    // consume the shared container message
    consumeMessage(stateMsg);
}

/* virtual */ void av::gua::NetTransform::_removeStateFragment(const std::string& fragment)
{
    // remove the group node for this fragment
    EIDGrpMap::iterator i = mGroupMap.find(fragment);
    if(i != mGroupMap.end())
    {
        unregisterWellKnown(this);

        mGroupMap.erase(i);
    }

    SharedContainerMap::iterator shared_container_iter = mSharedContainerMap.find(fragment);
    if(shared_container_iter != mSharedContainerMap.end())
    {
        Link<SharedContainerHolder> shared_container = (*shared_container_iter).second;
        unregisterWellKnown(shared_container.getPtr());

        mSharedContainerMap.erase(shared_container_iter);
        sharedContainersChanged();
    }
}

void av::gua::NetTransform::fragmentChildrenChanged() { Children.touch(); }

void av::gua::NetTransform::sharedContainersChanged() { SharedContainers.touch(); }

/* virtual */ void av::gua::NetTransform::getSharedContainersCB(const av::MFContainer::GetValueEvent& event)
{
    av::MFContainer::ContainerType& shared_containers(*event.getValuePtr());
    shared_containers.clear();

    for(SharedContainerMap::iterator container_iter = mSharedContainerMap.begin(); container_iter != mSharedContainerMap.end(); ++container_iter)
    {
        const av::MFContainer::ContainerType& fragment_shared_container = container_iter->second->SharedContainers.getValue();
        shared_containers.insert(shared_containers.end(), fragment_shared_container.begin(), fragment_shared_container.end());
    }
}

/* virtual */ void av::gua::NetTransform::setSharedContainersCB(const MFContainer::SetValueEvent& event)
{
    for(SharedContainerMap::iterator shared_container_iter = mSharedContainerMap.begin(); shared_container_iter != mSharedContainerMap.end(); ++shared_container_iter)
    {
        if(!shared_container_iter->second->SharedContainers.isEmpty())
        {
            shared_container_iter->second->SharedContainers.clear();
        }
    }

    for(MFContainer::ContainerType::const_iterator container_iter = event.getValue().begin(); container_iter != event.getValue().end(); ++container_iter)
    {
        Link<FieldContainer> container = *container_iter;

        AV_ASSERT(container.isValid());
        if(container->isDistributed())
        {
            AV_ASSERT(container->netNode() == static_cast<NetNode*>(this));

            SharedContainerMap::iterator distributor = mSharedContainerMap.find(container->netCreator());

            if(distributor != mSharedContainerMap.end())
            {
                distributor->second->SharedContainers.add1Value(container);
            }
            else
            {
                AVANGO_LOG(logger,
                           av::logging::WARN,
                           boost::str(boost::format("cannot find field container @0x%1% (created by '%2%') in shared container map") % container.getPtr() % container->netCreator()));
                container.getPtr(), container->netCreator();
            }
        }
        else
        {
            AVANGO_LOG(logger, av::logging::WARN, " object is not distributed !");
        }
    }
}
