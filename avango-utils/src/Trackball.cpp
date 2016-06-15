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
#include <gua/math.hpp>
#include <cmath>

namespace
{

av::Logger& logger(av::getLogger("av::utils::Trackball"));

::gua::math::vec3 projectToSphere(::gua::math::vec2 const& position) {
  const double distance = ::scm::math::length(position);
  const double z = distance < 0.7071
                  ? std::sqrt(1.0 - distance * distance)
                  : 0.5 / distance;
  ::gua::math::vec3 polarVec(position[0], position[1], z);
  ::scm::math::normalize(polarVec);
  return polarVec;
}

} // namespace {

AV_FC_DEFINE(av::utils::Trackball);

AV_FIELD_DEFINE(av::utils::SFTrackball);
AV_FIELD_DEFINE(av::utils::MFTrackball);

av::utils::Trackball::Trackball() :
  mTimeLastMovement(0.0),
  mLastDirection(0.0, 0.0),
  mLastProjected(0.0, 0.0, 0.0),
  mDragging(false),
  mSpinning(false),
  mReset(false)
{
  av::Link< ::av::gua::BoundingSphere > b = new ::av::gua::BoundingSphere();

  AV_FC_ADD_FIELD(Matrix, ::gua::math::mat4::identity());
  AV_FC_ADD_FIELD(TimeIn, 0.0);
  AV_FC_ADD_FIELD(Direction, ::gua::math::vec2(0.0, 0.0));
  AV_FC_ADD_FIELD(RotateTrigger, false);
  AV_FC_ADD_FIELD(ZoomTrigger, false);
  AV_FC_ADD_FIELD(PanTrigger, false);
  AV_FC_ADD_FIELD(ResetTrigger, false);
  AV_FC_ADD_FIELD(Enable, true);
  AV_FC_ADD_FIELD(AutoAdjustCenterTransform, true);
  AV_FC_ADD_FIELD(EnableSpinning,true);
  AV_FC_ADD_FIELD(SpinningTimeThreshold, 0.3);
  AV_FC_ADD_FIELD(SpinningWeightingCoefficient,0.97);
  AV_FC_ADD_FIELD(CenterTransform, ::scm::math::make_translation(0.0, 0.0, -0.6));
  AV_FC_ADD_FIELD(CenterToBoundingSphere, false);
  AV_FC_ADD_FIELD(BoundingSphere, b);
  AV_FC_ADD_FIELD(CenterTransformOffset, ::gua::math::vec3(0,-1.7,0));
  AV_FC_ADD_FIELD(CenterTransformOffsetZCoefficient, 17.0);
  AV_FC_ADD_FIELD(ZoomPanFactor,2.0);


  mRotation = ::gua::math::mat4::identity();
  mCenterTransInv = ::scm::math::inverse(CenterTransform.getValue());

  Name.setValue("Trackball");
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

void
av::utils::Trackball::reset()
{
  if(CenterToBoundingSphere.getValue() && Enable.getValue())
  {
    const ::gua::math::vec3 center = BoundingSphere.getValue()->Center.getValue();
    const float radius = BoundingSphere.getValue()->Radius.getValue();
    ::gua::math::vec3 offset = CenterTransformOffset.getValue();
    offset.z = offset.z + radius*CenterTransformOffsetZCoefficient.getValue();

    ::gua::math::vec3 offset_inv = ::gua::math::vec3(-offset.x
                                                    ,-offset.y
                                                    ,-offset.z);
    Matrix.setValue( ::scm::math::make_translation( center + offset ));

    CenterTransform.setValue(::scm::math::make_translation(offset_inv));
  }
  mReset = false;
}

/* virtual */ void
av::utils::Trackball::fieldHasChanged(const av::Field& field)
{
  av::FieldContainer::fieldHasChanged(field);

  if (&field == &CenterTransform) {
    mCenterTransInv = ::scm::math::inverse(CenterTransform.getValue());
  } else if(&field == &BoundingSphere) {
    mReset = true;
  }
}

/* virtual */ void
av::utils::Trackball::evaluate()
{
  av::FieldContainer::evaluate();

  if (!Enable.getValue())
      return;

  if (mReset)
    reset();

  const bool newDragging = RotateTrigger.getValue()
                        || ZoomTrigger.getValue()
                        || PanTrigger.getValue();

  const ::gua::math::vec3 projected = projectToSphere(Direction.getValue());

  if (mDragging && newDragging)
  {
    if (RotateTrigger.getValue())
    {
      auto quat = ::scm::math::quat<double>::from_arc(projected, mLastProjected);
      ::gua::math::mat4 rotMat = quat.to_matrix();

      double fac = double(SpinningWeightingCoefficient.getValue());
      mRotation = rotMat * ::scm::math::make_scale(fac,fac,fac) * mRotation;

      Matrix.setValue(mCenterTransInv * rotMat *
                      CenterTransform.getValue() * Matrix.getValue());
    }
    else if (ZoomTrigger.getValue())
    {
      const ::gua::math::vec2 offset = mLastDirection - Direction.getValue();
      float zoomFactor = offset.y;
      zoomFactor *= BoundingSphere.getValue()->Radius.getValue()
                  * ZoomPanFactor.getValue();

      if(AutoAdjustCenterTransform.getValue()) {
        ::gua::math::mat4 mat = CenterTransform.getValue()
                          * ::scm::math::make_translation(double(0.0), double(0.0), double(zoomFactor));
        CenterTransform.setValue(mat);
      }

      Matrix.setValue(mCenterTransInv
                    * ::scm::math::make_translation(double(0.0), double(0.0), double(-zoomFactor))
                    * CenterTransform.getValue()
                    * Matrix.getValue());
    }
    else if (PanTrigger.getValue())
    {
      const ::gua::math::vec2 offset = mLastDirection - Direction.getValue();

      float xPanFactor = offset.x
                        * BoundingSphere.getValue()->Radius.getValue()
                        * ZoomPanFactor.getValue();
      float yPanFactor = offset.y
                        * BoundingSphere.getValue()->Radius.getValue()
                        * ZoomPanFactor.getValue();

      Matrix.setValue(mCenterTransInv
                    * ::scm::math::make_translation(double(xPanFactor), double(yPanFactor), double(0.0))
                    * CenterTransform.getValue()
                    * Matrix.getValue());
    }
    mSpinning = false;
  }
  else if (!mDragging && newDragging)
  {
    mRotation = ::gua::math::mat4::identity();
    mTimeLastMovement = TimeIn.getValue();
  }
  else if (mDragging && !newDragging) {
    float timeSinceLastRecordEvent = TimeIn.getValue() - mTimeLastMovement;
    if (timeSinceLastRecordEvent < SpinningTimeThreshold.getValue()) {
      mSpinning = true;
    }
  }

  if (mSpinning) {
    ::gua::math::quat rot = ::scm::math::quat<double>::from_matrix(mRotation);
    Matrix.setValue(mCenterTransInv
                    * rot.to_matrix()
                    * CenterTransform.getValue()
                    * Matrix.getValue());
  }

  if (ResetTrigger.getValue()) {
    reset();

    mSpinning = false;
  }

  mDragging = newDragging;
  mLastDirection = Direction.getValue();
  mLastProjected = projected;
}
