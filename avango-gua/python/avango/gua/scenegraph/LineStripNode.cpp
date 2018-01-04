#include "LineStripNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/scenegraph/LineStripNode.hpp>

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

void clearVertices(av::gua::LineStripNode const& node) {
  return node.clearVertices();
}

void popBackVertex(av::gua::LineStripNode const& node) {

   return node.popBackVertex();
}

void popFrontVertex(av::gua::LineStripNode const& node) {

   return node.popFrontVertex();
}

void pushVertex1(av::gua::LineStripNode const& node,
                 float x_pos, float y_pos, float z_pos) {

   return node.pushVertex(x_pos, y_pos, z_pos);
}

void pushVertex2(av::gua::LineStripNode const& node,
                 float x_pos, float y_pos, float z_pos,
                 float col_r, float col_g, float col_b) {

   return node.pushVertex(x_pos, y_pos, z_pos,
                          col_r, col_g, col_b);
}

void pushVertex3(av::gua::LineStripNode const& node,
                 float x_pos, float y_pos, float z_pos,
                 float thickness) {

   return node.pushVertex(x_pos, y_pos, z_pos,
                          0.0f, 0.0f, 0.0f, //black vertex
                          thickness);
}

void pushVertex4(av::gua::LineStripNode const& node,
                 float x_pos, float y_pos, float z_pos,
                 float col_r, float col_g, float col_b,
                 float thickness) {

   return node.pushVertex(x_pos, y_pos, z_pos,
                          col_r, col_g, col_b,
                          thickness);
}

void init_LineStripNode()
{
  register_ptr_to_python<av::Link<av::gua::LineStripNode> >();

  register_field<av::gua::SFLineStripNode>("SFLineStripNode");
  register_multifield<av::gua::MFLineStripNode>("MFLineStripNode");
 /* class_<av::gua::LineStripNode, av::Link<av::gua::LineStripNode>,
    bases<av::gua::GeometryNode>,
    boost::noncopyable >("LineStripNode", "docstring", no_init);
*/

  class_<av::gua::LineStripNode,
         av::Link<av::gua::LineStripNode>,
         bases<av::FieldContainer>, boost::noncopyable> ("LineStripNode", "docstring", no_init)
         .def("clear_vertices", &clearVertices)         
         .def("pop_back_vertex", &popBackVertex)
         .def("pop_front_vertex", &popFrontVertex)
         .def("push_vertex", &pushVertex1)
         .def("push_vertex", &pushVertex2)
         .def("push_vertex", &pushVertex3)
         .def("push_vertex", &pushVertex4)
         ;

}
