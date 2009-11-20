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

#include <boost/bind.hpp>
#include <avango/osg/AutoTransform.h>
#include <avango/Logger.h>
#include <osg/Version>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::AutoTransform"));
}

AV_FC_DEFINE(av::osg::AutoTransform);

AV_FIELD_DEFINE(av::osg::SFAutoTransform);
AV_FIELD_DEFINE(av::osg::MFAutoTransform);

av::osg::AutoTransform::AutoTransform(::osg::AutoTransform* osgAutoTransform) :
  Transform(osgAutoTransform),
  mOsgAutoTransform(osgAutoTransform)
{
  AV_FC_ADD_ADAPTOR_FIELD(Scale,
                               boost::bind(&AutoTransform::getScaleCB, this, _1),
                               boost::bind(&AutoTransform::setScaleCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(AutoScaleToScreen,
                               boost::bind(&AutoTransform::getAutoScaleToScreenCB, this, _1),
                               boost::bind(&AutoTransform::setAutoScaleToScreenCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(MinimumScale,
                             boost::bind(&AutoTransform::getMinimumScaleCB, this, _1),
                             boost::bind(&AutoTransform::setMinimumScaleCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(MaximumScale,
                             boost::bind(&AutoTransform::getMaximumScaleCB, this, _1),
                             boost::bind(&AutoTransform::setMaximumScaleCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(AutoRotateMode,
                             boost::bind(&AutoTransform::getAutoRotateModeCB, this, _1),
                             boost::bind(&AutoTransform::setAutoRotateModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Position,
                            boost::bind(&AutoTransform::getPositionCB, this, _1),
                            boost::bind(&AutoTransform::setPositionCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(PivotPoint,
                              boost::bind(&AutoTransform::getPivotPointCB, this, _1),
                              boost::bind(&AutoTransform::setPivotPointCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(AutoScaleTransitionWidthRatio,
                              boost::bind(&AutoTransform::getAutoScaleTransitionWidthRatioCB, this, _1),
                              boost::bind(&AutoTransform::setAutoScaleTransitionWidthRatioCB, this, _1));



}

av::osg::AutoTransform::~AutoTransform()
{}

void
av::osg::AutoTransform::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Transform::initClass();

    AV_FC_INIT(av::osg::Transform, av::osg::AutoTransform, true);

    SFAutoTransform::initClass("av::osg::SFAutoTransform", "av::Field");
    MFAutoTransform::initClass("av::osg::MFAutoTransform", "av::Field");
  }
}

osg::AutoTransform*
av::osg::AutoTransform::getOsgAutoTransform() const
{
  return mOsgAutoTransform;
}

/* virtual */ void
av::osg::AutoTransform::getScaleCB(const av::osg::SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgAutoTransform->getScale();
}

/* virtual */ void
av::osg::AutoTransform::setScaleCB(const av::osg::SFVec3::SetValueEvent& event)
{
  mOsgAutoTransform->setScale(event.getValue());
}

/* virtual */ void
av::osg::AutoTransform::getMinimumScaleCB(const av::SFDouble::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgAutoTransform->getMinimumScale ();
}

/* virtual */ void
av::osg::AutoTransform::setMinimumScaleCB(const av::SFDouble::SetValueEvent& event)
{
  mOsgAutoTransform->setMinimumScale(event.getValue());
}

/* virtual */ void
av::osg::AutoTransform::getMaximumScaleCB(const av::SFDouble::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgAutoTransform->getMaximumScale ();
}

/* virtual */ void
av::osg::AutoTransform::setMaximumScaleCB(const av::SFDouble::SetValueEvent& event)
{
  mOsgAutoTransform->setMaximumScale(event.getValue());
}

/* virtual */ void
av::osg::AutoTransform::getAutoRotateModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgAutoTransform->getAutoRotateMode();
}

/* virtual */ void
av::osg::AutoTransform::setAutoRotateModeCB(const SFUInt::SetValueEvent& event)
{
  mOsgAutoTransform->setAutoRotateMode(static_cast< ::osg::AutoTransform::AutoRotateMode>(event.getValue()));
}

/* virtual */ void
av::osg::AutoTransform::getAutoScaleToScreenCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgAutoTransform->getAutoScaleToScreen();
}

/* virtual */ void
av::osg::AutoTransform::setAutoScaleToScreenCB(const SFBool::SetValueEvent& event)
{
  mOsgAutoTransform->setAutoScaleToScreen(event.getValue());
}

/* virtual */ void
av::osg::AutoTransform::getPositionCB(const av::osg::SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgAutoTransform->getPosition();
}

/* virtual */ void
av::osg::AutoTransform::setPositionCB(const av::osg::SFVec3::SetValueEvent& event)
{
  mOsgAutoTransform->setPosition(event.getValue());
}

/* virtual */ void
av::osg::AutoTransform::getPivotPointCB(const av::osg::SFVec3::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgAutoTransform->getPivotPoint();
}

/* virtual */ void
av::osg::AutoTransform::setPivotPointCB(const av::osg::SFVec3::SetValueEvent& event)
{
  mOsgAutoTransform->setPivotPoint(event.getValue());
}

/* virtual */ void
av::osg::AutoTransform::getAutoScaleTransitionWidthRatioCB(const av::SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = mOsgAutoTransform->getAutoScaleTransitionWidthRatio();
}

/* virtual */ void
av::osg::AutoTransform::setAutoScaleTransitionWidthRatioCB(const av::SFFloat::SetValueEvent& event)
{
  mOsgAutoTransform->setAutoScaleTransitionWidthRatio(event.getValue());
}
