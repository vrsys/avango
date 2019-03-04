// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of AVANGO.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* AVANGO is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* AVANGO is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with AVANGO. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
\************************************************************************/

#include <boost/python.hpp>

#include <avango/Config.h>

#include <avango/shade/Init.h>
#include <avango/shade/State.h>
#include <osg/Object>
#include <avango/python/register_field.h>

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

BOOST_PYTHON_MODULE(_shade)
{
    av::shade::Init::initClass();

    register_field<av::shade::SFDataVariance>("SFDataVariance");
    register_field<av::shade::SFState>("SFState");
    register_multifield<av::shade::MFState>("MFState");

    class_<av::shade::State, av::Link<av::shade::State>, bases<av::FieldContainer>, boost::noncopyable>("State", "A State represents one compiled shader program", no_init);

    def("append_search_path", av::shade::Init::appendSearchPath, "Append given path to search path for shader code");

    enum_<::osg::Object::DataVariance>("DataVariance").value("unspecified", ::osg::Object::UNSPECIFIED).value("static", ::osg::Object::STATIC).value("dynamic", ::osg::Object::DYNAMIC);
}
