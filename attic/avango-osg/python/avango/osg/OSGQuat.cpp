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
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include <osg/Matrix>
#include "OSGVectors.h"

#include <avango/python/register_field.h>

using namespace boost::python;
using namespace av::python;

namespace
{
std::string QuatString(const osg::Quat& q)
{
    std::stringstream ss;
    osg::Quat::value_type angle = 0;
    osg::Vec3* axis = new osg::Vec3(0, 0, 0);
    q.getRotate(angle, *axis);
    ss << boost::format("<Quat xyzw: %.6f %.6f %.6f %.6f  angle: %.6f  axis: %.6f %.6f %.6f >") % q[0] % q[1] % q[2] % q[3] % angle % (*axis)[0] % (*axis)[1] % (*axis)[2];
    return ss.str();
}

osg::Quat::value_type QuatGetAngle(osg::Quat& q)
{
    osg::Quat::value_type angle = 0;
    osg::Vec3* axis = new osg::Vec3(0, 0, 0);
    q.getRotate(angle, *axis);
    return angle;
}

osg::Vec3 QuatGetAxis(osg::Quat& q)
{
    osg::Quat::value_type angle = 0;
    osg::Vec3* axis = new osg::Vec3(0, 0, 0);
    q.getRotate(angle, *axis);
    return *axis;
}

} // namespace

void init_OSGQuat(void)
{
    // introducing member function pointers for overloaded function of osg::Quat::makeRotate
    void (osg::Quat::*mrot1)(const osg::Vec3&, const osg::Vec3&) = &osg::Quat::makeRotate;
    void (osg::Quat::*mrot2)(osg::Quat::value_type, const osg::Vec3&) = &osg::Quat::makeRotate;
    void (osg::Quat::*mrot3)(osg::Quat::value_type, osg::Quat::value_type, osg::Quat::value_type, osg::Quat::value_type) = &osg::Quat::makeRotate;

    // introducing member function pointers for overloaded function of osg::Quat::slerp
    void (osg::Quat::*slerp)(osg::Quat::value_type, const osg::Quat&, const osg::Quat&) = &osg::Quat::slerp;

    // wrapping osg::Quat functionality
    class_<osg::Quat>("Quat")
        .def(init<osg::Quat::value_type, osg::Quat::value_type, osg::Quat::value_type, osg::Quat::value_type>())
        .def(init<osg::Vec4>())
        .def(init<osg::Quat::value_type, osg::Vec3>())
        .add_property("x", &getX<osg::Quat>, &setX<osg::Quat>)
        .add_property("y", &getY<osg::Quat>, &setY<osg::Quat>)
        .add_property("z", &getZ<osg::Quat>, &setZ<osg::Quat>)
        .add_property("w", &getW<osg::Quat>, &setW<osg::Quat>)
        .def(self == other<osg::Quat>())
        .def(self != other<osg::Quat>())
        .def(self < other<osg::Quat>())
        .def(self + other<osg::Quat>())
        .def(self - other<osg::Quat>())
        .def(self * other<osg::Quat>())
        .def(self * other<osg::Vec3>())
        .def(self * osg::Quat::value_type())
        .def(self / osg::Quat::value_type())
        .def(self *= osg::Quat::value_type())
        .def(self += other<osg::Quat>())
        .def(self -= other<osg::Quat>())
        .def(self /= osg::Quat::value_type())
        .def("length", &osg::Quat::length)
        .def("length2", &osg::Quat::length2)
        .def("make_rotate", mrot1)
        .def("make_rotate", mrot2)
        .def("make_rotate", mrot3)
        .def("slerp", slerp)
        .def("get_angle", QuatGetAngle)
        .def("get_axis", QuatGetAxis)
        .def("__repr__", QuatString)
        .def("conjugate", &osg::Quat::conj);

    // register as a field
    register_field<av::osg::SFQuat>("SFQuat");
    register_multifield<av::osg::MFQuat>("MFQuat");
}
