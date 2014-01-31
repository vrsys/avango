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
#include <avango/osg/Fields.h>
#include <osg/Vec2>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include "OSGVectors.h"
#include <avango/python/register_field.h>

using namespace boost::python;
using namespace av::python;

namespace
{
  osg::Vec2 operator*(osg::Vec2::value_type lhs, osg::Vec2 rhs)
  {
    return rhs*lhs;
  }

  osg::Vec3 operator*(osg::Vec3::value_type lhs, osg::Vec3 rhs)
  {
    return rhs*lhs;
  }

  osg::Vec3 operator*(osg::Vec3 lhs, osg::Quat rhs)
  {
    return rhs*lhs;
  }

  osg::Vec4 operator*(osg::Vec4::value_type lhs, osg::Vec4 rhs)
  {
    return rhs*lhs;
  }

  std::string Vec2String(const osg::Vec2& v)
  {
    std::stringstream ss;
    ss << boost::format("<Vec2: %.6f %.6f>") % v[0] % v[1];
    return ss.str();
  }

  std::string Vec3String(const osg::Vec3& v)
  {
    std::stringstream ss;
    ss << boost::format("<Vec3: %.6f %.6f %.6f>") % v[0] % v[1] % v[2];
    return ss.str();
  }

  std::string Vec4String(const osg::Vec4& v)
  {
    std::stringstream ss;
    ss << boost::format("<Vec4: %.6f %.6f %.6f %.6f>") % v[0] % v[1] % v[2] % v[3];
    return ss.str();
  }

  osg::Vec2::value_type distanceVec2(const osg::Vec2& pos1, const osg::Vec2& pos2)
  {
    return (pos1 - pos2).length();
  }

  osg::Vec3::value_type distanceVec3(const osg::Vec3& pos1, const osg::Vec3& pos2)
  {
    return (pos1 - pos2).length();
  }
}

void init_OSGVec2(void)
 {
  // wrapping osg::Vec2 functionality
  class_<osg::Vec2>("Vec2")
    .def(init<osg::Vec2::value_type,osg::Vec2::value_type>())
    .def(init<osg::Vec2>())
    .add_property("x", &getX<osg::Vec2>, &setX<osg::Vec2>)
    .add_property("y", &getY<osg::Vec2>, &setY<osg::Vec2>)
    .def(self == other<osg::Vec2>())
    .def(self != other<osg::Vec2>())
    .def(self * osg::Vec2::value_type())
    .def(osg::Vec2::value_type() * self) // Uses own implementation above as none provided by OSG
    .def(self * other<osg::Vec2>())
    .def(self + other<osg::Vec2>())
    .def(self - other<osg::Vec2>())
    .def(self / osg::Vec2::value_type())
    .def(self < other<osg::Vec2>())
    .def("valid", &osg::Vec2::valid)
    .def("is_nan", &osg::Vec2::isNaN)
    .def("length", &osg::Vec2::length)
    .def("length2", &osg::Vec2::length2)
    .def("normalize", &osg::Vec2::normalize)
    .def("distance", distanceVec2)
    .def(self *= osg::Vec2::value_type())
    .def(self += other<osg::Vec2>())
    .def(self -= other<osg::Vec2>())
    .def(self /= osg::Vec2::value_type())
    .def("__repr__", Vec2String)
    ;

  // register as a field
  register_field<av::osg::SFVec2>("SFVec2");
  register_multifield<av::osg::MFVec2>("MFVec2");
 }

void init_OSGVec3(void)
 {
  // wrapping osg::Vec3 functionality
  class_<osg::Vec3>("Vec3")
    .def(init<osg::Vec3::value_type,osg::Vec3::value_type,osg::Vec3::value_type>())
    .def(init<osg::Vec3>())
    .add_property("x", &getX<osg::Vec3>, &setX<osg::Vec3>)
    .add_property("y", &getY<osg::Vec3>, &setY<osg::Vec3>)
    .add_property("z", &getZ<osg::Vec3>, &setZ<osg::Vec3>)
    .def(self == other<osg::Vec3>())
    .def(self != other<osg::Vec3>())
    .def(self < other<osg::Vec3>())
    .def(self + other<osg::Vec3>())
    .def(self - other<osg::Vec3>())
    .def(self * other<osg::Vec3>())
    .def(self * osg::Vec3::value_type())
    .def(self ^ other<osg::Vec3>())
    .def(osg::Vec3::value_type() * self) // Uses own implementation above as none provided by OSG
    .def(self * other<osg::Quat>()) // Uses own implementation above as none provided by OSG
    .def(self / osg::Vec3::value_type())
    .def(self *= osg::Vec3::value_type())
    .def(self += other<osg::Vec3>())
    .def(self -= other<osg::Vec3>())
    .def(self /= osg::Vec3::value_type())
    .def("valid", &osg::Vec3::valid)
    .def("is_nan", &osg::Vec3::isNaN)
    .def("length", &osg::Vec3::length)
    .def("length2", &osg::Vec3::length2)
    .def("normalize", &osg::Vec3::normalize)
    .def("distance", distanceVec3)
    .def("__repr__", Vec3String)
    ;

  // register as a field
  register_field<av::osg::SFVec3>("SFVec3");
  register_multifield<av::osg::MFVec3>("MFVec3");
 }

void init_OSGVec4(void)
 {
  // wrapping osg::Vec4 functionality
  class_<osg::Vec4>("Vec4")
    .def(init<osg::Vec4::value_type,osg::Vec4::value_type,osg::Vec4::value_type,osg::Vec4::value_type>())
    .def(init<osg::Vec4>())
    .add_property("x", &getX<osg::Vec4>, &setX<osg::Vec4>)
    .add_property("y", &getY<osg::Vec4>, &setY<osg::Vec4>)
    .add_property("z", &getZ<osg::Vec4>, &setZ<osg::Vec4>)
    .add_property("w", &getW<osg::Vec4>, &setW<osg::Vec4>)
    .def(self == other<osg::Vec4>())
    .def(self != other<osg::Vec4>())
    .def(self < other<osg::Vec4>())
    .def(self + other<osg::Vec4>())
    .def(self - other<osg::Vec4>())
    .def(self * other<osg::Vec4>())
    .def(self * osg::Vec4::value_type())
    .def(osg::Vec4::value_type() * self) // Uses own implementation above as none provided by OSG
    .def(self / osg::Vec4::value_type())
    .def(self *= osg::Vec4::value_type())
    .def(self += other<osg::Vec4>())
    .def(self -= other<osg::Vec4>())
    .def(self /= osg::Vec4::value_type())
    .def("valid", &osg::Vec4::valid)
    .def("is_nan", &osg::Vec4::isNaN)
    .def("length", &osg::Vec4::length)
    .def("length2", &osg::Vec4::length2)
    .def("normalize", &osg::Vec4::normalize)
    .def("__repr__", Vec4String)
    ;

  // register as a field
  register_field<av::osg::SFVec4>("SFVec4");
  register_multifield<av::osg::MFVec4>("MFVec4");
 }
