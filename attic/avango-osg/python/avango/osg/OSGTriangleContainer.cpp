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
#include <avango/python/register_field.h>
#include <avango/osg/TriangleContainer.h>
#include "OSGTriangleContainer.h"

using namespace boost::python;
using namespace av::python;

namespace boost
{
namespace python
{
template <class T>
struct pointee<av::Link<T>>
{
    typedef T type;
};
} // namespace python
} // namespace boost

void init_OSGTriangleContainer(void)
{
    // wrapping osg::TriangleContainer functionality
    register_field<av::osg::SFTriangleContainer>("SFTriangleContainer");
    register_multifield<av::osg::MFTriangleContainer>("MFTriangleContainer");
    class_<av::osg::TriangleContainer, av::Link<av::osg::TriangleContainer>, bases<av::osg::Geometry>, boost::noncopyable>(
        "TriangleContainer",
        "renders triangles in its multifield.\n"
        "Fields:  MFVec3 Vertices; MFVec4 Colors; MFVec3 Normals; SFInt Mode\n"
        "Supported modes: 0: GL_POINTS 1: GL_LINES 2: GL_LINE_LOOP 3: GL_LINE_STRIP 4: GL_TRIANGLES 5: GL_TRIANGLE_STRIP 6: GL_TRIANGLE_FAN 7: GL_QUADS 8: GL_QUAD_STRIP 9: GL_POLYGON",
        no_init)
        .def("getTriangleCount", &av::osg::TriangleContainer::getTriangleCount)
        .def("getNormalCount", &av::osg::TriangleContainer::getNormalCount);
}
