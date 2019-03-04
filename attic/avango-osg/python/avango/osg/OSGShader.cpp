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
#include <boost/python/enum.hpp>
#include <osg/Shader>
#include <avango/python/register_field.h>
#include <avango/osg/Shader.h>
#include "OSGShader.h"

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

void init_OSGShader(void)
{
    // wrapping osg::Shader functionality
    register_field<av::osg::SFShader>("SFShader");
    register_multifield<av::osg::MFShader>("MFShader");
    class_<av::osg::Shader, av::Link<av::osg::Shader>, bases<av::osg::Object>, boost::noncopyable>("Shader", "docstring", no_init);

    enum_<osg::Shader::Type>("shadertype").value("VERTEX", ::osg::Shader::VERTEX).value("FRAGMENT", ::osg::Shader::FRAGMENT).value("GEOMETRY", ::osg::Shader::GEOMETRY).export_values();
}
