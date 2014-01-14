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

#include <boost/python.hpp>
#include <osg/Plane>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/BoundingSphere>
#include <osg/BoundingBox>
#include "OSGPlane.h"

using namespace boost::python;

namespace
{
  osg::Vec3 PlaneGetNormal(const osg::Plane& p)
  {
    return p.getNormal();
  }

  osg::Vec4 PlaneAsVec4(const osg::Plane& p)
  {
    return p.asVec4();
  }

  osg::Vec3::value_type PlaneDistance(const osg::Plane& p, const osg::Vec3& v)
  {
    return p.distance(v);
  }

  float PlaneDotProductNormal(const osg::Plane& p, const osg::Vec3& v)
  {
    return p.dotProductNormal(v);
  }

  int PlaneIntersectSphere(const osg::Plane& p, const osg::BoundingSphere& bs)
  {
    return p.intersect(bs);
  }

  int PlaneIntersectBox(const osg::Plane& p, const osg::BoundingBox& bb)
  {
    return p.intersect(bb);
  }

  osg::Vec3 PlaneIntersectLine(const osg::Plane& p, const osg::Vec3& origin, const osg::Vec3& direction)
  {
    return origin - direction * p.distance(origin) / p.dotProductNormal(direction);
  }

  osg::Vec3 PlaneClosestPoint(const osg::Plane& p, const osg::Vec3& v)
  {
    return v - p.getNormal() * p.distance(v);
  }
}

void init_OSGPlane(void)
 {
  // wrapping osg::Plane functionality
  class_<osg::Plane>("Plane")
    .def(init<osg::Plane::value_type, osg::Plane::value_type, osg::Plane::value_type, osg::Plane::value_type>())
    .def(init<osg::Vec4>())
    .def(init<osg::Vec3, osg::Plane::value_type>())
    .def(init<osg::Vec3, osg::Vec3>())
    .def(init<osg::Vec3, osg::Vec3, osg::Vec3>())
    .def("valid", &osg::Plane::valid)
    .def("as_vec4", PlaneAsVec4)
    .def("get_normal", PlaneGetNormal)
    .def("distance", PlaneDistance)
    .def("dot_product_normal", PlaneDotProductNormal)
    .def("intersect", PlaneIntersectSphere)
    .def("intersect", PlaneIntersectBox)
    .def("intersect", PlaneIntersectLine)
    .def("transform", &osg::Plane::transform)
    .def("transform_providing_inverse", &osg::Plane::transformProvidingInverse)
    .def("flip", &osg::Plane::flip)
    .def("closest_point", PlaneClosestPoint)
    ;
 }
