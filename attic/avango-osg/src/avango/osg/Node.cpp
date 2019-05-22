// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <avango/osg/Node.h>
#include <avango/osg/Object.h>
#include <avango/osg/ObjectLink.h>
#include <avango/osg/Group.h>

#include <avango/Logger.h>

#include <boost/bind.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::osg::Node"));
}

AV_FC_DEFINE(av::osg::Node);

AV_FIELD_DEFINE(av::osg::SFNode);
AV_FIELD_DEFINE(av::osg::MFNode);

av::osg::Node::Node(::osg::Node* osgnode) : Object(osgnode), mOsgNode(osgnode)
{
    AV_FC_ADD_ADAPTOR_FIELD(Parents, boost::bind(&Node::getParentsCB, this, _1), boost::bind(&Node::setParentsCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(StateSet, boost::bind(&Node::getStateSetCB, this, _1), boost::bind(&Node::setStateSetCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(CullingActive, boost::bind(&Node::getCullingActiveCB, this, _1), boost::bind(&Node::setCullingActiveCB, this, _1));
    AV_FC_ADD_ADAPTOR_FIELD(NodeMask, boost::bind(&Node::getNodeMaskCB, this, _1), boost::bind(&Node::setNodeMaskCB, this, _1));
}

av::osg::Node::~Node() {}

void av::osg::Node::initClass()
{
    if(!isTypeInitialized())
    {
        av::osg::Object::initClass();
        av::osg::StateSet::initClass();

        AV_FC_INIT(av::osg::Object, av::osg::Node, true);

        SFNode::initClass("av::osg::SFNode", "av::Field");
        MFNode::initClass("av::osg::MFNode", "av::Field");
    }
}

::osg::Node* av::osg::Node::getOsgNode() const { return mOsgNode; }

::osg::Matrix av::osg::Node::getAbsoluteTransform(av::FieldContainer* caller) const
{
    ::osg::Node* osg_node = getOsgNode();

    // traverse to the root node and trigger evaluate() if a caller was given
    while(osg_node != 0)
    {
        ::osg::Node::ParentList parent_list = osg_node->getParents();

        if(parent_list.size() > 0)
        {
            Link<av::osg::Node> av_node = av::osg::get_from_osg_object<av::osg::Node>(parent_list[0]);

            if(av_node.isValid())
            {
                if(caller != 0)
                    caller->evaluateDependency(*av_node);
                osg_node = av_node->getOsgNode();
            }
            else
                break;
        }
        else
            break;
    }

    ::osg::Matrix abs_mat;
    ::osg::MatrixList abs_mat_list = getOsgNode()->getWorldMatrices(osg_node);
    if(abs_mat_list.size() > 0u)
        abs_mat = abs_mat_list[0];

    return abs_mat;
}

::osg::Matrix av::osg::Node::getAbsoluteTransformHaltAtNode(av::FieldContainer* caller, av::osg::Node* haltTraversalAtNode) const
{
    ::osg::Node* osg_node = getOsgNode();

    // traverse to the root node and trigger evaluate() if a caller was given
    while(osg_node != 0)
    {
        ::osg::Node::ParentList parent_list = osg_node->getParents();

        if(parent_list.size() > 0)
        {
            Link<av::osg::Node> av_node = av::osg::get_from_osg_object<av::osg::Node>(parent_list[0]);

            if(av_node.isValid())
            {
                if(caller != 0)
                    caller->evaluateDependency(*av_node);
                osg_node = av_node->getOsgNode();
            }
            else
                break;
        }
        else
            break;
    }

    ::osg::Matrix abs_mat;
    ::osg::MatrixList abs_mat_list;
    if(haltTraversalAtNode == 0)
        abs_mat_list = getOsgNode()->getWorldMatrices(osg_node);
    else
        abs_mat_list = getOsgNode()->getWorldMatrices(haltTraversalAtNode->getOsgNode());

    if(abs_mat_list.size() > 0u)
        abs_mat = abs_mat_list[0];

    return abs_mat;
}

/* virtual */ void av::osg::Node::getParentsCB(const av::osg::MFGroup::GetValueEvent& event)
{
    av::osg::MFGroup::ContainerType& parents(*event.getValuePtr());
    parents.clear();

    unsigned int num_parents = mOsgNode->getNumParents();
    parents.reserve(num_parents);

    for(unsigned int parent = 0u; parent < num_parents; ++parent)
    {
        ::osg::Group* osg_parent = mOsgNode->getParent(parent);
        if(osg_parent != 0)
        {
            av::osg::Group* group = av::osg::get_from_osg_object<av::osg::Group>(osg_parent);
            if(group != 0)
            {
                parents.push_back(group);
            }
            else
            {
                AVANGO_LOG(logger, av::logging::INFO, "getParentsCB: found user data at a parent that doesn't reference an av::osg::Group.");
            }
        }
        else
        {
            AVANGO_LOG(logger, av::logging::WARN, "getParentsCB: invalid osg parent found!");
        }
    }
}

/* virtual */ void av::osg::Node::setParentsCB(const av::osg::MFGroup::SetValueEvent&) { AVANGO_LOG(logger, av::logging::INFO, "setParentsCB: Parents field is read only!"); }

/* virtual */ void av::osg::Node::getStateSetCB(const av::osg::SFStateSet::GetValueEvent& event) { *(event.getValuePtr()) = av::osg::get_from_osg_object<av::osg::StateSet>(mOsgNode->getStateSet()); }

/* virtual */ void av::osg::Node::setStateSetCB(const av::osg::SFStateSet::SetValueEvent& event)
{
    if(!event.getValue().isValid())
    {
        return;
    }

    mOsgNode->setStateSet(event.getValue()->getOsgStateSet());
}

/* virtual */ void av::osg::Node::getCullingActiveCB(const av::SFBool::GetValueEvent& event) { *(event.getValuePtr()) = mOsgNode->getCullingActive(); }

/* virtual */ void av::osg::Node::setCullingActiveCB(const av::SFBool::SetValueEvent& event) { mOsgNode->setCullingActive(event.getValue()); }

/* virtual */ void av::osg::Node::getNodeMaskCB(const av::SFUInt::GetValueEvent& event) { *(event.getValuePtr()) = mOsgNode->getNodeMask(); }

/* virtual */ void av::osg::Node::setNodeMaskCB(const av::SFUInt::SetValueEvent& event) { mOsgNode->setNodeMask(event.getValue()); }
