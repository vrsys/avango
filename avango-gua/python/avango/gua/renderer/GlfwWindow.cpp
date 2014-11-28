#include "GlfwWindow.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/GlfwWindow.hpp>

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


void init_GlfwWindow()
 {

  register_field<av::gua::SFGlfwWindow>("SFGlfwWindow");
  register_multifield<av::gua::MFGlfwWindow>("MFGlfwWindow");
  class_<av::gua::GlfwWindow,
         av::Link<av::gua::GlfwWindow>,
         bases<av::gua::WindowBase>, boost::noncopyable >("GlfwWindow", "docstring", no_init)
         ;
 }
