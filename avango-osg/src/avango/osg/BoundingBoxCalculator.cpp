// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
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
* AVANGO is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include <avango/osg/BoundingBoxCalculator.h>
#include <avango/osg/BoundingSphere.h>
#include <avango/Logger.h>

#include <osg/ComputeBoundsVisitor>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::BoundingBoxCalculator"));

  bool inRange(float refValue,float testValue,float range)
  {
    if (testValue > refValue-range && testValue < refValue+range)
      return true;
    else
      return false;
  }

  bool inRange(::osg::Vec3 refVec, ::osg::Vec3 testVec, float range)
  {
    return inRange(refVec[0],testVec[0],range) && inRange(refVec[1],testVec[1],range) && inRange(refVec[2],testVec[2],range);
  }
}

AV_FC_DEFINE(av::osg::BoundingBoxCalculator);

AV_FIELD_DEFINE(av::osg::SFBoundingBoxCalculator);
AV_FIELD_DEFINE(av::osg::MFBoundingBoxCalculator);

av::osg::BoundingBoxCalculator::BoundingBoxCalculator(): mCenter(::osg::Vec3(0,0,0)),mRadius(0), mFudge(0.001)
{
  av::Link< ::av::osg::BoundingSphere > b = new ::av::osg::BoundingSphere();

  AV_FC_ADD_FIELD(Fudge,0.001);
  AV_FC_ADD_FIELD(BoundingSphere, b);


  alwaysEvaluate(true);
}

av::osg::BoundingBoxCalculator::~BoundingBoxCalculator()
{}

void
av::osg::BoundingBoxCalculator::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Group::initClass();

    AV_FC_INIT(av::osg::Group, av::osg::BoundingBoxCalculator, true);

    SFBoundingBoxCalculator::initClass("av::osg::SFBoundingBoxCalculator", "av::Field");
    MFBoundingBoxCalculator::initClass("av::osg::MFBoundingBoxCalculator", "av::Field");
  }
}

/* virtual */ void
av::osg::BoundingBoxCalculator::evaluate()
{
  const ::osg::BoundingSphere approximateBounds = getOsgGroup()->getBound();

  const ::osg::Vec3 approximateCenter = approximateBounds.center();
  const float approximateRadius = approximateBounds.radius();
  const bool valid = approximateBounds.valid();

  mFudge = Fudge.getValue();

  if( (! inRange(mRadius, approximateRadius, mFudge) || ! inRange(mCenter, approximateCenter, mFudge)) && valid )
  {
    // get the exact bounds of the node. The bounds used for culling are only "fast" approximations
    ::osg::ComputeBoundsVisitor cbbv(::osg::NodeVisitor::TRAVERSE_ACTIVE_CHILDREN);
    getOsgGroup()->accept(cbbv);
    ::osg::BoundingBox bounds = cbbv.getBoundingBox();
    const ::osg::Vec3 center = bounds.center();

    const float d_x = bounds.xMax() - bounds.xMin();
    const float d_y = bounds.yMax() - bounds.yMin();
    const float d_z = bounds.zMax() - bounds.zMin();

    float radius = d_x;
    if (radius < d_y)
    {
      radius = d_y;
    }
    if (radius < d_z)
    {
      radius = d_z;
    }
    radius *= 0.5;

    BoundingSphere.getValue()->Radius.setValue(radius);
    BoundingSphere.getValue()->Center.setValue(center);
    BoundingSphere.touch();

    mRadius = approximateRadius;
    mCenter = approximateCenter;
  }
}
