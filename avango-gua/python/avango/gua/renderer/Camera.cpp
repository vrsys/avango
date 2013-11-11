#include "Camera.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/Camera.hpp>

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


void init_Camera()
 {

  register_field<av::gua::SFCamera>("SFCamera");
  register_multifield<av::gua::MFCamera>("MFCamera");
  class_<av::gua::Camera, av::Link<av::gua::Camera>, bases<av::FieldContainer>, boost::noncopyable >("Camera", "docstring", no_init);

 }
