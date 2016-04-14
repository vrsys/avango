#include "WindowBase.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/WindowBase.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
 {
  namespace python
   {
    template <class T> struct pointee<av::Link<T> >
     {
       using type = T;
     };
   }
 }


void init_WindowBase()
 {
   register_ptr_to_python<av::Link<av::gua::WindowBase> >();
  register_field<av::gua::SFWindowBase>("SFWindowBase");
  register_multifield<av::gua::MFWindowBase>("MFWindowBase");
  class_<av::gua::WindowBase,
         av::Link<av::gua::WindowBase>,
         bases<av::FieldContainer>, boost::noncopyable >("WindowBase", "docstring", no_init)
         .def("open", &av::gua::WindowBase::open)
         .def("close", &av::gua::WindowBase::close)
         ;

  enum_<av::gua::WindowBase::Mode>("StereoMode")
        .value("MONO", av::gua::WindowBase::MONO)
        .value("SIDE_BY_SIDE", av::gua::WindowBase::SIDE_BY_SIDE)
        .value("ANAGLYPH_RED_GREEN", av::gua::WindowBase::ANAGLYPH_RED_GREEN)
        .value("ANAGLYPH_RED_CYAN", av::gua::WindowBase::ANAGLYPH_RED_CYAN)
        .value("CHECKERBOARD", av::gua::WindowBase::CHECKERBOARD)
        .value("NVIDIA_3D_VISION", av::gua::WindowBase::NVIDIA_3D_VISION)
        .value("QUAD_BUFFERED", av::gua::WindowBase::QUAD_BUFFERED)
        ;

 }
