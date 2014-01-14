// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2009 Fraunhofer-Gesellschaft zur Foerderung der       *
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

#include <osg/Vec4>
#include <osg/Geode>

#include <avango/Logger.h>
#include <avango/osg/Types.h>

#include <avango/osg/Box.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::Box"));
}

AV_FC_DEFINE(av::osg::Box);

AV_FIELD_DEFINE(av::osg::SFBox);
AV_FIELD_DEFINE(av::osg::MFBox);

av::osg::Box::Box() :
  MatrixTransform(),
  //mHints(new ::osg::TessellationHints),
  mGeode(new ::osg::Geode),
  mDimChanged(false)//,
  //mDetailRatioChanged(false)
{
  AV_FC_ADD_FIELD(Width, 1.0f);
  AV_FC_ADD_FIELD(Height, 1.0f);
  AV_FC_ADD_FIELD(Length, 1.0f);
  
  //AV_FC_ADD_FIELD(DetailRatio, 0.5f);
  AV_FC_ADD_FIELD(Color, ::osg::Vec4(1, 1, 1, 1));

  mBox = new ::osg::Box(::osg::Vec3(), Width.getValue(), Height.getValue(), Length.getValue());

  //mHints->setDetailRatio(DetailRatio.getValue());

  mShapeDrawable = new ::osg::ShapeDrawable(mBox.get());
  mShapeDrawable.get()->setColor(Color.getValue());

  mGeode.get()->addDrawable(mShapeDrawable.get());

  getOsgMatrixTransform()->addChild(mGeode.get());
}

av::osg::Box::~Box()
{}

void
av::osg::Box::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::MatrixTransform::initClass();

    AV_FC_INIT(av::osg::MatrixTransform, av::osg::Box, true);

    SFBox::initClass("av::osg::SFBox", "av::Field");
    MFBox::initClass("av::osg::MFBox", "av::Field");
  }
}


/* virtual */ void
av::osg::Box::fieldHasChangedLocalSideEffect(const av::Field& field)
{
  MatrixTransform::fieldHasChangedLocalSideEffect(field);
  AVANGO_LOG(logger, av::logging::TRACE, "fieldHasChangedLocalSideEffect()");

  if (&field == &Width)
  {
    AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fieldHadChangedLocalSideEffect(): Width changed to %1%") % Width.getValue()));
    mDimChanged = true;
  }

  if (&field == &Height)
  {
    AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fieldHadChangedLocalSideEffect(): Height changed to %1%") % Height.getValue()));
    mDimChanged = true;
  }

  if (&field == &Length)
  {
    AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fieldHadChangedLocalSideEffect(): Length changed to %1%") % Length.getValue()));
    mDimChanged = true;
  }


  // if (&field == &DetailRatio)
  // {
  //   AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fieldHadChangedLocalSideEffect(): DetailRatio changed to %1%") % DetailRatio.getValue()));
  //   mDetailRatioChanged = true;
  // }

  if (&field == &Color)
  {
    AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("fieldHadChangedLocalSideEffect(): Color changed to %1%") % &Color.getValue()));
    mColorChanged = true;
  }
}

/* virtual */ void
av::osg::Box::evaluateLocalSideEffect()
{
  MatrixTransform::evaluateLocalSideEffect();
  LOG_TRACE(logger) << "evaluateLocalSideEffect()";

  if (mDimChanged)
  {
    AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("evaluateLocalSideEffect(): got new dimensions %1%") % Radius.getValue()));
    mBox->setHalfLengths(::osg::Vec3(Width.getValue(), Height.getValue(), Length.getValue()));
    mShapeDrawable->dirtyDisplayList();
    mShapeDrawable->dirtyBound();
    mDimChanged = false;
  }

  // if (mDetailRatioChanged)
  // {
  //   AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("evaluateLocalSideEffect(): got new DetailRatio %1%") % DetailRatio.getValue()));
  //   mHints->setDetailRatio(DetailRatio.getValue());
  //   mShapeDrawable->dirtyDisplayList();
  //   mShapeDrawable->dirtyBound();
  //   mDetailRatioChanged = false;
  // }

  if (mColorChanged)
  {
    AVANGO_LOG(logger, av::logging::TRACE, boost::str(boost::format("evaluateLocalSideEffect(): got new Color %1%") % &Color.getValue()));
    mShapeDrawable.get()->setColor(Color.getValue());
    mColorChanged = false;
  }
}

::osg::ref_ptr< ::osg::Box >
av::osg::Box::getOsgBox() const {
  return mBox;
}

::osg::ref_ptr< ::osg::ShapeDrawable >
av::osg::Box::getOsgShapeDrawable() const {
  return mShapeDrawable;
}
