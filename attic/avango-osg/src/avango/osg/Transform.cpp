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

#include <avango/osg/Transform.h>

#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Transform"));
}

AV_FC_DEFINE(av::osg::Transform);

AV_FIELD_DEFINE(av::osg::SFTransform);
AV_FIELD_DEFINE(av::osg::MFTransform);

av::osg::Transform::Transform(::osg::Transform* osgtransform) :
  Group(osgtransform),
  mOsgTransform(osgtransform)
{
  AV_FC_ADD_FIELD(ReferenceFrame,"RELATIVE_RF");
}

av::osg::Transform::~Transform()
{}

void
av::osg::Transform::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Group::initClass();

    AV_FC_INIT(av::osg::Group, av::osg::Transform, true);

    SFTransform::initClass("av::osg::SFTransform", "av::Field");
    MFTransform::initClass("av::osg::MFTransform", "av::Field");
  }
}

void
av::osg::Transform::fieldHasChangedLocalSideEffect(const av::Field& field)
{
  av::osg::Group::fieldHasChangedLocalSideEffect(field);
  if (&field == &ReferenceFrame)
  {
    if(ReferenceFrame.getValue()=="RELATIVE_RF") {
      mOsgTransform->setReferenceFrame(::osg::Transform::RELATIVE_RF);
    } else if (ReferenceFrame.getValue()=="ABSOLUTE_RF") {
      mOsgTransform->setReferenceFrame(::osg::Transform::ABSOLUTE_RF);
    } else if (ReferenceFrame.getValue()=="ABSOLUTE_RF_INHERIT_VIEWPOINT") {
      mOsgTransform->setReferenceFrame(::osg::Transform::ABSOLUTE_RF_INHERIT_VIEWPOINT);
    }

  }
}

::osg::Transform*
av::osg::Transform::getOsgTransform() const
{
  return mOsgTransform;
}
