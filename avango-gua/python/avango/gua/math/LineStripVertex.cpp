#include "LineStripVertex.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/math/LineStripVertex.hpp>
#include <avango/gua/Types.hpp>

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

void pushLineStripVertex(av::gua::LineStripVertex& lhs,
                         float x, float y, float z,
                         float col_r, float col_g, float col_b,
                         //::gua::math::vec3f const& v_position,
                         //::gua::math::vec3f const& v_color,
                         float const& thickness) {


  ::gua::math::vec3f v_position{x, y, z};
  ::gua::math::vec3f v_color{col_r, col_g, col_b};
  lhs.pushLineStripVertex(v_position, v_color, thickness);
}


void init_LineStripVertex()
{
  register_ptr_to_python<av::Link<av::gua::LineStripVertex> >();

  register_field<av::gua::SFLineStripVertex>("SFLineStripVertex");
  register_multifield<av::gua::MFLineStripVertex>("MFLineStripVertex");

  class_<av::gua::LineStripVertex,
         av::Link<av::gua::LineStripVertex>,
         bases<av::FieldContainer>, boost::noncopyable >("LineStripVertex",
                                                         "docstring",
                                                         no_init)
    .def("push_line_strip_vertex", &pushLineStripVertex);
  ;


}
