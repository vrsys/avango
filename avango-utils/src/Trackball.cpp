// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include <avango/utils/Trackball.h>
#include <avango/Logger.h>
#include <boost/bind.hpp>
#include <osg/Quat>
#include <cmath>

namespace
{
  av::Logger& logger(av::getLogger("av::utils::Trackball"));

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

AV_FC_DEFINE(av::utils::Trackball);

AV_FIELD_DEFINE(av::utils::SFTrackball);
AV_FIELD_DEFINE(av::utils::MFTrackball);

av::utils::Trackball::Trackball():
  mTimeLastMovement(0.0),
  mLastDirection(0.0, 0.0),
  mLastProjected(0.0, 0.0, 0.0),
  mDragging(false),
  mSpinning(false)
{
  AV_FC_ADD_FIELD(Matrix, ::osg::Matrix::identity());
  AV_FC_ADD_FIELD(TimeIn, 0.0);
  AV_FC_ADD_FIELD(Direction, ::osg::Vec2(0.0, 0.0));
  AV_FC_ADD_FIELD(RotateTrigger, false);
  AV_FC_ADD_FIELD(ZoomTrigger, false);
  AV_FC_ADD_FIELD(PanTrigger, false);
  AV_FC_ADD_FIELD(ResetTrigger, false);
  AV_FC_ADD_FIELD(AutoAdjustCenterTransform, true);
  AV_FC_ADD_FIELD(SpinningTimeThreshold, 0.3);
  AV_FC_ADD_FIELD(SpinningWeightingCoefficient,0.97);
  AV_FC_ADD_FIELD(CenterTransform, ::osg::Matrix::translate(0.0, 0.0, -0.6));
  AV_FC_ADD_FIELD(CenterTransformReset, ::osg::Matrix::translate(0.0, 0.0, -0.6));


  mRotation = ::osg::Matrix::identity();
  mCenterTransInv = ::osg::Matrix::inverse(CenterTransform.getValue());

}

av::utils::Trackball::~Trackball()
{}

void
av::utils::Trackball::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::utils::Trackball, true);

    SFTrackball::initClass("av::utils::SFTrackball", "av::Field");
    MFTrackball::initClass("av::utils::MFTrackball", "av::Field");
  }
}

/* virtual */ void
av::utils::Trackball::fieldHasChanged(const av::Field& field)
{
  av::FieldContainer::fieldHasChanged(field);

  if (&field == &CenterTransform)
    mCenterTransInv = ::osg::Matrix::inverse(CenterTransform.getValue());
}

/* virtual */ void
av::utils::Trackball::evaluate()
{
  av::FieldContainer::evaluate();

  const bool newDragging =
    (RotateTrigger.getValue() || ZoomTrigger.getValue() || PanTrigger.getValue());

  const ::osg::Vec3 projected = projectToSphere(Direction.getValue());

  if (mDragging && newDragging)
  {
    if (RotateTrigger.getValue())
    {
      ::osg::Matrix rotMat = ::osg::Matrix::rotate(projected, mLastProjected);
      float fac = SpinningWeightingCoefficient.getValue();
      mRotation = rotMat * ::osg::Matrix::scale(fac,fac,fac) * mRotation;

      Matrix.setValue(mCenterTransInv * rotMat *
                      CenterTransform.getValue() * Matrix.getValue());
    }
    else if (ZoomTrigger.getValue())
    {
      const ::osg::Vec2 offset = mLastDirection - Direction.getValue();

      if(AutoAdjustCenterTransform.getValue())
      {
        ::osg::Matrix mat = CenterTransform.getValue() * ::osg::Matrix::translate(0.0, 0.0, offset[1]);
        CenterTransform.setValue(mat);
      }

      Matrix.setValue(mCenterTransInv * ::osg::Matrix::translate(0.0, 0.0, -offset[1]) *
                      CenterTransform.getValue() * Matrix.getValue());
    }
    else if (PanTrigger.getValue())
    {
      const ::osg::Vec2 offset = mLastDirection - Direction.getValue();
      Matrix.setValue(mCenterTransInv * ::osg::Matrix::translate(offset[0], offset[1], 0.0) *
                      CenterTransform.getValue() * Matrix.getValue());
    }
    mSpinning = false;
  }
  else if (!mDragging && newDragging)
  {
    mRotation = ::osg::Matrix::identity();
    mTimeLastMovement = TimeIn.getValue();
  }
  else if (mDragging && !newDragging)
  {
    float timeSinceLastRecordEvent = TimeIn.getValue() - mTimeLastMovement;
    if (timeSinceLastRecordEvent < SpinningTimeThreshold.getValue())
    {
      mSpinning = true;
    }
  }

  if (mSpinning)
  {
    ::osg::Quat rot = mRotation.getRotate();
    Matrix.setValue(mCenterTransInv * ::osg::Matrix::rotate(rot) *
                    CenterTransform.getValue() * Matrix.getValue());
  }

  if (ResetTrigger.getValue())
  {
    Matrix.setValue(::osg::Matrix::identity());
    CenterTransform.setValue(CenterTransformReset.getValue());
  }

  mDragging = newDragging;
  mLastDirection = Direction.getValue();
  mLastProjected = projected;
}
