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

#include <avango/osg/ClipNode.h>

#include <boost/bind.hpp>

#include <avango/osg/ObjectLink.h>

#include <avango/Logger.h>

#include <osg/Plane>
#include <iostream>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::ClipNode"));
}

AV_FC_DEFINE(av::osg::ClipNode);

AV_FIELD_DEFINE(av::osg::SFClipNode);
AV_FIELD_DEFINE(av::osg::MFClipNode);

av::osg::ClipNode::ClipNode(::osg::ClipNode* osggroup) :
  Group(osggroup),
  mOsgClipNode(osggroup)
{
    AV_FC_ADD_ADAPTOR_FIELD(ClipPlanes,
                            boost::bind(&av::osg::ClipNode::getClipPlaneCB, this, _1),
                            boost::bind(&av::osg::ClipNode::setClipPlaneCB, this, _1));
}

/* virtual */
av::osg::ClipNode::~ClipNode()
{}

/* static */ void
av::osg::ClipNode::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Group::initClass();

    AV_FC_INIT(av::osg::Group, av::osg::ClipNode, false);

    SFClipNode::initClass("av::osg::SFClipNode", "av::Field");
    MFClipNode::initClass("av::osg::MFClipNode", "av::Field");
    sClassTypeId.setDistributable(true);
  }
}

::osg::ClipNode*
av::osg::ClipNode::getOsgClipNode() const
{
  return mOsgClipNode;
}

void
av::osg::ClipNode::createClipBox(::osg::Vec3 min, ::osg::Vec3 max)
{
  ::osg::BoundingBox bBox;
  bBox.set(min,max);
  mOsgClipNode->createClipBox(bBox);
}

/* virtual */ void
av::osg::ClipNode::getClipPlaneCB(const av::osg::MFClipPlane::GetValueEvent& event)
{
  av::osg::MFClipPlane::ContainerType &clipPlanes(*event.getValuePtr());

  clipPlanes.clear();

  int num_clipPlanes = mOsgClipNode->getNumClipPlanes();
  clipPlanes.reserve(num_clipPlanes);

  for (int ch = 0; ch < num_clipPlanes; ++ch)
  {
    ::osg::ClipPlane *osg_ClipPlane = mOsgClipNode->getClipPlane(ch);

    if (osg_ClipPlane != 0)
    {
      av::osg::ClipPlane *clipPlane = av::osg::get_from_osg_object<av::osg::ClipPlane>(osg_ClipPlane);

      if (clipPlane != 0)
      {
        clipPlanes.push_back(clipPlane);
      }
      else
      {
        AVANGO_LOG(logger, av::logging::INFO, "getClipPlanesCB: found user data at a child that doesn't reference an av::osg::Node.");
      }
    }
    else
    {
      AVANGO_LOG(logger, av::logging::WARN, "getClipPlanesCB: null osg child found!");
    }
  }
}

/* virtual */ void
av::osg::ClipNode::setClipPlaneCB(const av::osg::MFClipPlane::SetValueEvent& event)
{
  int num_clipPlanes = mOsgClipNode->getNumClipPlanes();

  // Remove (but do not delete) all clipPlanes that are av::osg::ClipPlane.
  // ClipPlanes of type ::osg::Node are not affected.

  for (int ch = num_clipPlanes-1; ch >= 0 ; --ch)
  {
    ::osg::ClipPlane *osg_clipPlane = mOsgClipNode->getClipPlane(ch);

    if (osg_clipPlane != 0)
    {
      ObjectLink *av_child_link = dynamic_cast<ObjectLink*>(osg_clipPlane->getUserData());
      if (av_child_link != 0 && av_child_link->getObject() != 0)
      {
        mOsgClipNode->removeClipPlane(osg_clipPlane);
      }
    }
    else
    {
      AVANGO_LOG(logger, av::logging::WARN, "setClipPlanesCB: null osg ClipNodes found!");
    }
  }

  // Add new clipPlane set

  const av::osg::MFClipPlane::ContainerType &clipPlanes(event.getValue());

  std::vector<av::Link<av::osg::ClipPlane> >::const_iterator ch_it;
  for (ch_it = clipPlanes.begin(); ch_it != clipPlanes.end(); ++ch_it)
  {

    if (ch_it->isValid() && ch_it->getPtr()->getOsgClipPlane() != 0)
    {
      if (! mOsgClipNode->addClipPlane(ch_it->getPtr()->getOsgClipPlane()))
      {
        AVANGO_LOG(logger, av::logging::WARN, "setClipPlanesCB: couldn't insert clip node!");
      }
    }
    else
    {
      AVANGO_LOG(logger, av::logging::WARN, "setClipPlanesCB: invalid clip node to add found!");
    }
  }
}
