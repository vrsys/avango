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

#include <avango/display/mt/MultitouchDevice.h>
#include <avango/python/register_field.h>
#include <boost/python.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
  namespace python
  {
    template <class T> struct pointee<av::Link<T> >
    {
      typedef T type;
    };
  }
}

namespace
{
  // local definitions
}

BOOST_PYTHON_MODULE(_mt)
{
  // initialize Avango type, if necessary
  av::display::mt::MultitouchDevice::initClass();
  av::display::mt::MultitouchFinger::initClass();

  // define python bindings here
  class_<av::display::mt::MultitouchDevice, av::Link<av::display::mt::MultitouchDevice>, bases<av::FieldContainer>, boost::noncopyable >("MultitouchDevice",
          "Docstring with explanations what this class can be used for.");

  class_<av::display::mt::MultitouchFinger, av::Link<av::display::mt::MultitouchFinger>, bases<av::FieldContainer>, boost::noncopyable >("MultitouchFinger",
          "Docstring with explanations what this class can be used for.");

  register_field<av::display::mt::SFMultitouchFinger>("SFMultitouchFinger");
  register_multifield<av::display::mt::MFMultitouchFinger>("MFMultitouchFinger");
}
