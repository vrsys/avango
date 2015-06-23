#include "Window.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/Window.hpp>

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


void init_Window()
 {

  register_field<av::gua::SFWindow>("SFWindow");
  register_multifield<av::gua::MFWindow>("MFWindow");
  class_<av::gua::Window,
         av::Link<av::gua::Window>,
         bases<av::gua::WindowBase>, boost::noncopyable >("Window", "docstring", no_init)
         ;
 }
