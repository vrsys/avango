#include "OculusWindow.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/oculus/OculusWindow.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
  namespace python
  {
    template<class T> struct pointee<av::Link<T> >
    {
      using type = T;
    };
  }
}

void init_OculusWindow()
{
  register_field<av::oculus::SFOculusWindow>("SFOculusWindow");
  register_multifield<av::oculus::MFOculusWindow>("MFOculusWindow");

  class_<av::oculus::OculusWindow,
       av::Link<av::oculus::OculusWindow>,
       bases<av::gua::Window>, boost::noncopyable >("OculusWindow", "docstring", no_init)
        ;
}
