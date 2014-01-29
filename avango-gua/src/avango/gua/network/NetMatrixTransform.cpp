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

#include "avango/gua/network/NetMatrixTransform.h"

#include <avango/gua/network/FragmentGroup.h>
#include <avango/gua/network/SharedContainerHolder.h>

#include <avango/Application.h>
#include <boost/bind.hpp>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::NetMatrixTransform"));
}

AV_FC_DEFINE(av::gua::NetMatrixTransform);

AV_FIELD_DEFINE(av::gua::SFNetMatrixTransform);
AV_FIELD_DEFINE(av::gua::MFNetMatrixTransform);

av::gua::NetMatrixTransform::NetMatrixTransform()
  : TransformNode(),
    NetNode()
{
  // the name of the network goup to join
  AV_FC_ADD_FIELD(Groupname, "");

  // the list of current group members.
  AV_FC_ADD_FIELD(Members, std::vector<std::string>());
  AV_FC_ADD_FIELD(NewMembers, std::vector<std::string>());
  AV_FC_ADD_FIELD(DepartedMembers, std::vector<std::string>());
  AV_FC_ADD_FIELD(NetId, std::string());
  AV_FC_ADD_ADAPTOR_FIELD(SharedContainers,
      boost::bind(&NetMatrixTransform::getSharedContainersCB,   this, _1),
      boost::bind(&NetMatrixTransform::setSharedContainersCB,   this, _1));


  Groupname.dontDistribute(true);
  Members.dontDistribute(true);
  NewMembers.dontDistribute(true);
  DepartedMembers.dontDistribute(true);
  NetId.dontDistribute(true);

  // for the local nodes, deletion is automatic
  mLocalGroups = new FragmentGroup;
  mLocalGroups->registerNetMatrixTransform(this);
  mLocalGroups->reference();
  mLocalGroups->Name.setValue("local");

  getGuaNode()->add_child(mLocalGroups->getGuaNode());

  mPreEvalHandle = ApplicationInstance::get().addPreEvaluationContainerCallback(boost::bind(&NetMatrixTransform::handleNetworkReceives, this));
  mPostEvalHandle = ApplicationInstance::get().addPostEvaluationContainerCallback(boost::bind(&NetMatrixTransform::handleNetworkSends, this));

}

/* virtual */
av::gua::NetMatrixTransform::~NetMatrixTransform()
{
  ApplicationInstance::get().removeCallback(mPreEvalHandle);
  ApplicationInstance::get().removeCallback(mPostEvalHandle);
}

/* static */ void
av::gua::NetMatrixTransform::initClass()
{
  if (!isTypeInitialized())
  {
    av::gua::TransformNode::initClass();
    av::gua::FragmentGroup::initClass();

    AV_FC_INIT(av::gua::TransformNode, av::gua::NetMatrixTransform, true);

    SFNetMatrixTransform::initClass("av::gua::SFNetMatrixTransform", "av::Field");
    MFNetMatrixTransform::initClass("av::gua::MFNetMatrixTransform", "av::Field");

    sClassTypeId.setDistributable(true);
  }
}


/* virtual */ void
av::gua::NetMatrixTransform::fieldHasChangedLocalSideEffect(const Field& field)
{
  TransformNode::fieldHasChangedLocalSideEffect(field);

  if (&field == &Groupname) {
    if (Groupname.getValue().size()) {
      AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fpNetDCS::fieldHasChangedLocalSideEffect: joining net-group '%1%'.") % Groupname.getValue().c_str()));
      // if the groupname is not empty try to join
      join(Groupname.getValue());
      Name.setValue(Groupname.getValue());
    } else {
      // get the hell out of here
      leave();
    }
  }
}

/* virtual */ void
av::gua::NetMatrixTransform::evaluate()
{
  TransformNode::evaluate();
}

/* virtual */ void
av::gua::NetMatrixTransform::refImpl()
{}

/* virtual */ void
av::gua::NetMatrixTransform::unrefImpl()
{}

/* virtual */ int
av::gua::NetMatrixTransform::refCountImpl()
{
  return 1;
}

/* virtual */ void
av::gua::NetMatrixTransform::_join(const std::string& fragment)
{
  // a new node is needed
  Link<FragmentGroup> group = new FragmentGroup;
  group->Name.setValue(fragment);

  NetID group_id(fragment, NetID::sNetGroupRootNode);
  registerWellKnown(group.getPtr(), group_id);

  mGroupMap[fragment] = group;
  group->registerNetMatrixTransform(this);

  getGuaNode()->add_child(group->getGuaNode());
  fragmentChildrenChanged();

  // register SharedContainerHolder
  Link<SharedContainerHolder> container_holder = new SharedContainerHolder;

  NetID container_holder_id(fragment, NetID::sNetGroupContainerHolder);
  registerWellKnown(container_holder.getPtr(), container_holder_id);

  mSharedContainerMap[fragment] = container_holder;
  sharedContainersChanged();

#if AVANGO_DEBUG
  AVANGO_LOG(logger, logging::TRACE, boost::str(boost::format("_join: added group node %1% for: %2%]") % group_id % fragment));
#endif
}

// overloaded from fp_net_node to react to view changes
/* virtual */ void
av::gua::NetMatrixTransform::_acceptNewView(const std::vector<std::string>& members,
                                            const std::vector<std::string>& newMembers,
                                            const std::vector<std::string>& departedMembers)
{
  Members.setValue (members);
  NewMembers.setValue(newMembers);
  DepartedMembers.setValue(departedMembers);

  std::string id(netEID());
  if (id != NetId.getValue()) {
    NetId.setValue(id);
  }
}

/* virtual */ void
av::gua::NetMatrixTransform::_getStateFragment(const std::string& fragment, Msg& stateMsg)
{


  Link<FragmentGroup> group;
  // send an update message for the well-known group node of this fragment
  EIDGrpMap::iterator i = mGroupMap.find(fragment);
  if (i != mGroupMap.end())
  {
    group = (*i).second;
  }
  else
  {
    // a new node is needed
    group = new FragmentGroup;
    group->Name.setValue(fragment);

    NetID group_id(fragment, NetID::sNetGroupRootNode);
    registerWellKnown(group.getPtr(), group_id);

    mGroupMap[fragment] = group;
    group->registerNetMatrixTransform(this);
    getGuaNode()->add_child(group->getGuaNode());
    fragmentChildrenChanged();
  }
  stateMsg.setType(Msg::absolute);
  makeUpdateMessage(stateMsg, group.getPtr());


  Link<SharedContainerHolder> container_holder;
  // send an update message for the well-known shared container  node of this fragment
  SharedContainerMap::iterator container_iter = mSharedContainerMap.find(fragment);
   if (container_iter != mSharedContainerMap.end())
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
     container_holder->registerNetMatrixTransform(this);
     sharedContainersChanged();
   }
   stateMsg.setType(Msg::absolute);
   makeUpdateMessage(stateMsg, container_holder.getPtr());

}

/* virtual */ void
av::gua::NetMatrixTransform::_setStateFragment(const std::string& fragment, Msg& stateMsg)
{
  // created a new fpFragmentGroup node and register with the correct well-known id.
  // it consists of the endpoint-id of the server and a constant number.

  EIDGrpMap::iterator found = mGroupMap.find(fragment);
  if (found == mGroupMap.end())
  {
    // a new node is needed
    Link<FragmentGroup> group = new FragmentGroup;
    group->Name.setValue(fragment);

    NetID group_id(fragment, NetID::sNetGroupRootNode);
    registerWellKnown(group.getPtr(), group_id);

    mGroupMap[fragment] = group;
    group->registerNetMatrixTransform(this);

    getGuaNode()->add_child(group->getGuaNode());
    fragmentChildrenChanged();
  }

  // send an update message for the well-known shared container  node of this fragment
  SharedContainerMap::iterator container_iter = mSharedContainerMap.find(fragment);
  if (container_iter == mSharedContainerMap.end())
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

/* virtual */ void
av::gua::NetMatrixTransform::_removeStateFragment(const std::string& fragment)
{
  // remove the group node for this fragment
  EIDGrpMap::iterator i = mGroupMap.find(fragment);
  if (i != mGroupMap.end())
  {
    Link<FragmentGroup> group = (*i).second;
    unregisterWellKnown(group.getPtr());
    group->registerNetMatrixTransform(0);

    getGuaNode()->remove_child(group->getGuaNode());
    mGroupMap.erase(i);
    fragmentChildrenChanged();
  }

  SharedContainerMap::iterator shared_container_iter = mSharedContainerMap.find(fragment);
  if (shared_container_iter != mSharedContainerMap.end())
  {
    Link<SharedContainerHolder> shared_container = (*shared_container_iter).second;
    unregisterWellKnown(shared_container.getPtr());

    mSharedContainerMap.erase(shared_container_iter);
    sharedContainersChanged();
  }
}

void
av::gua::NetMatrixTransform::fragmentChildrenChanged()
{
  Children.touch();
}

void
av::gua::NetMatrixTransform::sharedContainersChanged()
{
  SharedContainers.touch();
}

/* virtual */
void
av::gua::NetMatrixTransform::getChildrenCB(const av::gua::MFNode::GetValueEvent& event)
{
  av::gua::MFNode::ContainerType &children(*event.getValuePtr());

  children.clear();

  children = mLocalGroups->Children.getValue();

  for ( EIDGrpMap::iterator grpIter = mGroupMap.begin();
        grpIter != mGroupMap.end();
        ++grpIter) {
    const av::gua::MFNode::ContainerType &grpChildren = grpIter->second->Children.getValue();
    children.insert(children.end(), grpChildren.begin(), grpChildren.end());
  }
}

/* virtual */
void
av::gua::NetMatrixTransform::setChildrenCB(const av::gua::MFNode::SetValueEvent& event)
{

  for ( EIDGrpMap::iterator grpIter = mGroupMap.begin();
        grpIter != mGroupMap.end();
        ++grpIter) {
    if (!grpIter->second->Children.isEmpty()) {
      grpIter->second->Children.clear();
    }
  }

  mLocalGroups->Children.clear();


  for (MFNode::ContainerType::const_iterator nodeIter = event.getValue().begin();
       nodeIter != event.getValue().end();
       ++nodeIter) {
    Link<Node> node = *nodeIter;

    AV_ASSERT(node.isValid());
    if (node->isDistributed()) {
      AV_ASSERT(node->netNode() == static_cast<NetNode*>(this));

      EIDGrpMap::iterator distributor = mGroupMap.find(node->netCreator());

      if (distributor != mGroupMap.end()) {
        distributor->second->Children.add1Value(node);
      } else {
        AVANGO_LOG(logger, av::logging::WARN, boost::str(boost::format("cannot find node @0x%1% (created by '%2%') in group map")
                  % node.getPtr() % node->netCreator()));
      }
    } else {
      mLocalGroups->Children.add1Value(node);
    }
  }
}

/* virtual */ void
av::gua::NetMatrixTransform::getSharedContainersCB(const av::MFContainer::GetValueEvent& event)
{

  av::MFContainer::ContainerType &shared_containers(*event.getValuePtr());
  shared_containers.clear();

  for ( SharedContainerMap::iterator container_iter = mSharedContainerMap.begin();
        container_iter != mSharedContainerMap.end();
        ++container_iter) {
    const av::MFContainer::ContainerType &fragment_shared_container = container_iter->second->SharedContainers.getValue();
    shared_containers.insert(shared_containers.end(), fragment_shared_container.begin(), fragment_shared_container.end());
  }
}

/* virtual */ void
av::gua::NetMatrixTransform::setSharedContainersCB(const MFContainer::SetValueEvent& event)
{
  for ( SharedContainerMap::iterator shared_container_iter = mSharedContainerMap.begin();
        shared_container_iter != mSharedContainerMap.end();
        ++shared_container_iter) {
    if (!shared_container_iter->second->SharedContainers.isEmpty()) {
      shared_container_iter->second->SharedContainers.clear();
    }
  }

  for (MFContainer::ContainerType::const_iterator container_iter = event.getValue().begin();
       container_iter != event.getValue().end();
       ++container_iter) {
    Link<FieldContainer> container = *container_iter;


    AV_ASSERT(container.isValid());
    if (container->isDistributed()) {
      AV_ASSERT(container->netNode() == static_cast<NetNode*>(this));

      SharedContainerMap::iterator distributor = mSharedContainerMap.find(container->netCreator());

      if (distributor != mSharedContainerMap.end()) {
        distributor->second->SharedContainers.add1Value(container);
      } else {
        AVANGO_LOG(logger, av::logging::WARN, boost::str(boost::format("cannot find field container @0x%1% (created by '%2%') in shared container map") % container.getPtr() % container->netCreator()));
        container.getPtr(), container->netCreator();
      }
    } else
    {
      AVANGO_LOG(logger, av::logging::WARN, " object is not distributed !");
    }
  }
}
