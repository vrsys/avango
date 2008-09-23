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
#include <boost/format.hpp>
#include <osg/BoundingSphere>
#include <osg/BoundingBox>
#include <osg/Vec3>
#include "OSGBoundingSphere.h"

using namespace boost::python;

namespace
{
  osg::Vec3 BoundingSphereGetCenter(const osg::BoundingSphere& b)
  {
    return b.center();
  }

  std::string BoundingSphereString(const osg::BoundingSphere& b)
  {
    std::stringstream ss;
    osg::Vec3 v = b.center();
    ss << boost::format("<BoundingSphere center: %.6f %.6f %.6f  radius: %.6f>") % v[0] % v[1] % v[2] % b.radius();
    return ss.str();
  }
}

void init_OSGBoundingSphere(void)
 {
  // introducing member function pointers for overloaded function of osg::BoundingSphere::radius
  float (osg::BoundingSphere::*radius1)() const = &osg::BoundingSphere::radius;

  // introducing member function pointers for overloaded function of osg::BoundingSphere::expandBy
  void (osg::BoundingSphere::*expandBy1)(const osg::Vec3&) = &osg::BoundingSphere::expandBy;
  void (osg::BoundingSphere::*expandBy2)(const osg::BoundingBox&) = &osg::BoundingSphere::expandBy;
  void (osg::BoundingSphere::*expandBy3)(const osg::BoundingSphere&) = &osg::BoundingSphere::expandBy;
  void (osg::BoundingSphere::*expandRadiusBy1)(const osg::Vec3&) = &osg::BoundingSphere::expandRadiusBy;
  void (osg::BoundingSphere::*expandRadiusBy2)(const osg::BoundingBox&) = &osg::BoundingSphere::expandRadiusBy;
  void (osg::BoundingSphere::*expandRadiusBy3)(const osg::BoundingSphere&) = &osg::BoundingSphere::expandRadiusBy;

  // wrapping osg::BoundingSphere functionality
  class_<osg::BoundingSphere>("BoundingSphere")
    .def(init<osg::Vec3, float>())
    .def(init<osg::BoundingSphere>())
    .def(init<osg::BoundingBox>())
    .def("init", &osg::BoundingSphere::init)
    .def("valid", &osg::BoundingSphere::valid)
    .def("set", &osg::BoundingSphere::set)
    .def("get_center", BoundingSphereGetCenter)
    .def("radius", radius1)
    .def("radius2", &osg::BoundingSphere::radius2)
    .def("expand_by", expandBy1)
    .def("expand_by", expandBy2)
    .def("expand_by", expandBy3)
    .def("expand_radius_by", expandRadiusBy1)
    .def("expand_radius_by", expandRadiusBy2)
    .def("expand_radius_by", expandRadiusBy3)
    .def("intersects", &osg::BoundingSphere::intersects)
    .def("contains", &osg::BoundingSphere::contains)
    .def("__repr__", BoundingSphereString)
    ;
 }
