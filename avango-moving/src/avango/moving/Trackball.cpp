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

#include <avango/moving/Trackball.h>

#include <avango/Logger.h>

#include <boost/bind.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::moving::Trackball"));

  ::osg::Vec3
  projectToSphere(::osg::Vec2 position)
  {
    const double distance = position.length();
    const double z = (distance < 0.7071 ? std::sqrt(1.0 - distance * distance) : 0.5 / distance);
    ::osg::Vec3 polarVec(position[0], position[1], z);
    polarVec.normalize();
    return polarVec;
  }
}

AV_FC_DEFINE(av::moving::Trackball);

AV_FIELD_DEFINE(av::moving::SFTrackball);
AV_FIELD_DEFINE(av::moving::MFTrackball);

av::moving::Trackball::Trackball():
  mLastDirection(0.0, 0.0),
  mLastProjected(0.0, 0.0, 0.0),
  mDragging(false)
{
  AV_FC_ADD_FIELD(Direction, ::osg::Vec2(0.0, 0.0));
  AV_FC_ADD_FIELD(RotateTrigger, false);
  AV_FC_ADD_FIELD(ZoomTrigger, false);
  AV_FC_ADD_FIELD(PanTrigger, false);
  AV_FC_ADD_FIELD(CenterTransform, ::osg::Matrix::translate(0.0, 0.0, -0.6));

  mCenterTransInv = ::osg::Matrix::inverse(CenterTransform.getValue());
}

av::moving::Trackball::~Trackball()
{}

void
av::moving::Trackball::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::MatrixTransform::initClass();

    AV_FC_INIT(av::osg::MatrixTransform, av::moving::Trackball, true);

    SFTrackball::initClass("av::moving::SFTrackball", "av::Field");
    MFTrackball::initClass("av::moving::MFTrackball", "av::Field");
  }
}

/* virtual */ void
av::moving::Trackball::fieldHasChanged(const av::Field& field)
{
  av::osg::MatrixTransform::fieldHasChanged(field);

  if (&field == &CenterTransform)
    mCenterTransInv = ::osg::Matrix::inverse(CenterTransform.getValue());
}

/* virtual */ void
av::moving::Trackball::evaluate()
{
  av::osg::MatrixTransform::evaluate();

  const bool newDragging =
    (RotateTrigger.getValue() || ZoomTrigger.getValue() || PanTrigger.getValue());
  const ::osg::Vec3 projected = projectToSphere(Direction.getValue());

  if (mDragging && newDragging)
  {
    if (RotateTrigger.getValue())
    {
      Matrix.setValue(mCenterTransInv * ::osg::Matrix::rotate(projected, mLastProjected) *
                      CenterTransform.getValue() * Matrix.getValue());
    }
    else if (ZoomTrigger.getValue())
    {
      const ::osg::Vec2 offset = mLastDirection - Direction.getValue();
      Matrix.setValue(mCenterTransInv * ::osg::Matrix::translate(0.0, 0.0, -offset[1]) *
                      CenterTransform.getValue() * Matrix.getValue());
    }
    else if (PanTrigger.getValue())
    {
      const ::osg::Vec2 offset = mLastDirection - Direction.getValue();
      Matrix.setValue(mCenterTransInv * ::osg::Matrix::translate(offset[0], offset[1], 0.0) *
                      CenterTransform.getValue() * Matrix.getValue());
    }
  }

  mDragging = newDragging;
  mLastDirection = Direction.getValue();
  mLastProjected = projected;
}
