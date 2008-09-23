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
#include <osg/BoundingBox>
#include <osg/BoundingSphere>
#include <osg/Vec3>
#include "OSGBoundingBox.h"

using namespace boost::python;

namespace
{
  std::string BoundingBoxString(const osg::BoundingBox& b)
  {
    std::stringstream ss;
    osg::Vec3 center = b.center();
    ss << boost::format("<BoundingBox\n");
    ss << boost::format(" min: %.6f %.6f %.6f \n") % b._min[0] % b._min[1] % b._min[2];
    ss << boost::format(" max: %.6f %.6f %.6f \n") % b._max[0] % b._max[1] % b._max[2];
    ss << boost::format(" center: %.6f %.6f %.6f \n") % center[0] % center[1] % center[2];
    ss << boost::format(">");
    return ss.str();
  }
}

void init_OSGBoundingBox(void)
 {
  // introducing member function pointers for overloaded function of osg::BoundingBox::set
  void (osg::BoundingBox::*set1)(const osg::Vec3&, const osg::Vec3&) = &osg::BoundingBox::set;
  void (osg::BoundingBox::*set2)(float, float, float, float, float, float) = &osg::BoundingBox::set;

  // introducing member function pointers for overloaded function of osg::BoundingBox::expandBy
  void (osg::BoundingBox::*expandBy1)(const osg::Vec3&) = &osg::BoundingBox::expandBy;
  void (osg::BoundingBox::*expandBy2)(float, float, float) = &osg::BoundingBox::expandBy;
  void (osg::BoundingBox::*expandBy3)(const osg::BoundingBox&) = &osg::BoundingBox::expandBy;
  void (osg::BoundingBox::*expandBy4)(const osg::BoundingSphere&) = &osg::BoundingBox::expandBy;

  // introducing member function pointers for overloaded function of osg::BoundingBox::xMin/yMin/zMin/xMax/yMax/zMax
  float (osg::BoundingBox::*xMin1)() const = &osg::BoundingBox::xMin;
  float (osg::BoundingBox::*yMin1)() const = &osg::BoundingBox::yMin;
  float (osg::BoundingBox::*zMin1)() const = &osg::BoundingBox::zMin;
  float (osg::BoundingBox::*xMax1)() const = &osg::BoundingBox::xMax;
  float (osg::BoundingBox::*yMax1)() const = &osg::BoundingBox::yMax;
  float (osg::BoundingBox::*zMax1)() const = &osg::BoundingBox::zMax;

  // wrapping osg::BoundingBox functionality
  class_<osg::BoundingBox>("BoundingBox")
    .def(init<float, float, float, float, float, float>())
    .def(init<osg::Vec3, osg::Vec3>())
    .def("init", &osg::BoundingBox::init)
    .def("valid", &osg::BoundingBox::valid)
    .def("set", set1)
    .def("set", set2)
    .def("get_center", &osg::BoundingBox::center)
    .def("radius", &osg::BoundingBox::radius)
    .def("corner", &osg::BoundingBox::corner)
    .def("x_min", xMin1)
    .def("y_min", yMin1)
    .def("z_min", zMin1)
    .def("x_max", xMax1)
    .def("y_max", yMax1)
    .def("z_max", zMax1)
    .def("expand_by", expandBy1)
    .def("expand_by", expandBy2)
    .def("expand_by", expandBy3)
    .def("expand_by", expandBy4)
    .def("intersect", &osg::BoundingBox::intersect)
    .def("intersects", &osg::BoundingBox::intersects)
    .def("contains", &osg::BoundingBox::contains)
    .def("__repr__", BoundingBoxString)
    ;
 }
