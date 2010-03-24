// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
*  Avango Software, Copyright 2001 - 2009 Fraunhofer Gesellschaft        *
*  zur Foerderung der angewandten Forschung (FhG), Munich, Germany.      *
*                                                                        *
*  Avango Software (both binary and source code - if released) is        *
*  intellectual property owned by FhG and copyrighted by FhG worldwide.  *
*                                                                        *
*  Avango is a trademark owned by FhG.                                   *
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
