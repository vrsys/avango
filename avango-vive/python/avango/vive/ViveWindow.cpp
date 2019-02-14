#include "ViveWindow.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/vive/ViveWindow.hpp>

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

void triggerHapticPulse(av::vive::ViveWindow const& window,
	unsigned int controllerId, float strength) {
	window.triggerHapticPulse(controllerId, strength);
}

void init_ViveWindow()
{
  register_ptr_to_python<av::Link<av::vive::ViveWindow> >();

  register_field<av::vive::SFViveWindow>("SFViveWindow");
  register_multifield<av::vive::MFViveWindow>("MFViveWindow");

  class_<av::vive::ViveWindow,
       av::Link<av::vive::ViveWindow>,
       bases<av::gua::GlfwWindow>, boost::noncopyable >("ViveWindow", "docstring")
	   .def("trigger_haptic_pulse", &triggerHapticPulse)
        ;
}
