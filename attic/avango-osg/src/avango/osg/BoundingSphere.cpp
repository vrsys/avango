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

#include <avango/osg/BoundingSphere.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::BoundingSphere"));
}

AV_FC_DEFINE(av::osg::BoundingSphere);

AV_FIELD_DEFINE(av::osg::SFBoundingSphere);
AV_FIELD_DEFINE(av::osg::MFBoundingSphere);

av::osg::BoundingSphere::BoundingSphere()
{
  AV_FC_ADD_FIELD(Center,::osg::Vec3(0,0,0));
  AV_FC_ADD_FIELD(Radius,0.0);
}

/* virtual */
av::osg::BoundingSphere::~BoundingSphere()
{}

/* static */ void
av::osg::BoundingSphere::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::osg::BoundingSphere, true);

    SFBoundingSphere::initClass("av::osg::SFBoundingSphere", "av::Field");
    MFBoundingSphere::initClass("av::osg::MFBoundingSphere", "av::Field");
  }
}
