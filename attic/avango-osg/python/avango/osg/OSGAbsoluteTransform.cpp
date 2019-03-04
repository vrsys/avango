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
#include <avango/osg/AbsoluteTransform.h>
#include "OSGAbsoluteTransform.h"

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

void init_OSGAbsoluteTransform(void)
{
    // wrapping osg::AbsoluteTransform functionality
    register_field<av::osg::SFAbsoluteTransform>("SFAbsoluteTransform");
    register_multifield<av::osg::MFAbsoluteTransform>("MFAbsoluteTransform");
    class_<av::osg::AbsoluteTransform, av::Link<av::osg::AbsoluteTransform>, bases<av::osg::Node>, boost::noncopyable>("AbsoluteTransform", "docstring", no_init);
}
