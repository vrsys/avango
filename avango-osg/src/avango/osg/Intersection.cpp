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

#include <avango/osg/Intersection.h>

#include <avango/logging/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Intersection"));
}

AV_FC_DEFINE(av::osg::Intersection);

AV_FIELD_DEFINE(av::osg::SFIntersection);
AV_FIELD_DEFINE(av::osg::MFIntersection);

av::osg::Intersection::Intersection()
{
  AV_FC_ADD_FIELD(NodePath, MFNode::ContainerType());
  AV_FC_ADD_FIELD(Point, ::osg::Vec3(0.0, 0.0, 0.0));
  AV_FC_ADD_FIELD(Normal, ::osg::Vec3(0.0, 0.0, 0.0));
}

/* virtual */
av::osg::Intersection::~Intersection()
{}

/* static */ void
av::osg::Intersection::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::osg::Intersection, true);

    SFIntersection::initClass("av::osg::SFIntersection", "av::Field");
    MFIntersection::initClass("av::osg::MFIntersection", "av::Field");
  }
}
