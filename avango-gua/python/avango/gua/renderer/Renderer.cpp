#include "Renderer.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/Renderer.hpp>

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

av::Link<av::gua::Renderer> createRenderer (av::gua::Pipeline* pipe) {
  return av::Link<av::gua::Renderer> (new av::gua::Renderer({pipe}));
}

void init_Renderer()
 {
  class_<av::gua::Renderer,
         av::Link<av::gua::Renderer>,
         bases<av::FieldContainer>, boost::noncopyable> ("Renderer", "docstring", no_init)
         .def("queueDraw", &av::gua::Renderer::queue_draw)
         ;

  register_field<av::gua::SFRenderer>("SFRenderer");
  register_multifield<av::gua::MFRenderer>("MFRenderer");

  def("create_renderer", &createRenderer);
 }
