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

#include <avango/osg/Group.h>

#include <boost/bind.hpp>

#include <avango/osg/ObjectLink.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Group"));
}

AV_FC_DEFINE(av::osg::Group);

AV_FIELD_DEFINE(av::osg::SFGroup);
AV_FIELD_DEFINE(av::osg::MFGroup);

av::osg::Group::Group(::osg::Group* osggroup) :
  Node(osggroup),
  mOsgGroup(osggroup)
{
  AV_FC_ADD_ADAPTOR_FIELD(Children,
                          boost::bind(&av::osg::Group::getChildrenCB, this, _1),
                          boost::bind(&av::osg::Group::setChildrenCB, this, _1));
}

/* virtual */
av::osg::Group::~Group()
{}

/* static */ void
av::osg::Group::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Node::initClass();

    AV_FC_INIT(av::osg::Node, av::osg::Group, false);

    SFGroup::initClass("av::osg::SFGroup", "av::Field");
    MFGroup::initClass("av::osg::MFGroup", "av::Field");
    sClassTypeId.setDistributable(true);
  }
}

::osg::Group*
av::osg::Group::getOsgGroup() const
{
  return mOsgGroup;
}

/* virtual */ void
av::osg::Group::getChildrenCB(const av::osg::MFNode::GetValueEvent& event)
{
  av::osg::MFNode::ContainerType &children(*event.getValuePtr());

  children.clear();

  int num_children = mOsgGroup->getNumChildren();
  children.reserve(num_children);

  for (int ch = 0; ch < num_children; ++ch)
  {
    ::osg::Node *osg_child = mOsgGroup->getChild(ch);

    if (osg_child != 0)
    {
      av::osg::Node *node = av::osg::get_from_osg_object<av::osg::Node>(osg_child);

      if (node != 0)
      {
        children.push_back(node);
      }
      else
      {
        logger.info() << "getChildrenCB: "
                      << "found user data at a child that doesn't reference an av::osg::Node.";
      }
    }
    else
    {
      logger.warn() << "getChildrenCB: null osg child found!";
    }
  }
}

/* virtual */ void
av::osg::Group::setChildrenCB(const av::osg::MFNode::SetValueEvent& event)
{
  int num_children = mOsgGroup->getNumChildren();

  // Remove (but do not delete) all children that are av::osg::Nodes.
  // Children of type ::osg::Node are not affected.

  for (int ch = num_children-1; ch >= 0 ; --ch)
  {
    ::osg::Node *osg_child = mOsgGroup->getChild(ch);

    if (osg_child != 0)
    {
      ObjectLink *av_child_link = dynamic_cast<ObjectLink*>(osg_child->getUserData());
      if (av_child_link != 0 && av_child_link->getObject() != 0)
      {
        mOsgGroup->removeChild(osg_child);
      }
    }
    else
    {
      logger.warn() << "setChildrenCB: null osg child found!";
    }
  }

  // Add new children set

  const av::osg::MFNode::ContainerType &children(event.getValue());

  std::vector<av::Link<av::osg::Node> >::const_iterator ch_it;
  for (ch_it = children.begin(); ch_it != children.end(); ++ch_it)
  {
    if (ch_it->isValid() && ch_it->getPtr()->getOsgNode() != 0)
    {
      if (!mOsgGroup->addChild(ch_it->getPtr()->getOsgNode()))
        logger.warn() << "setChildrenCB: couldn't insert child!";
    }
    else
    {
      logger.warn() << "setChildrenCB: invalid child to add found!";
    }
  }
}
