#include "DynamicTriangleNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/scenegraph/DynamicTriangleNode.hpp>

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

void clearVertices(av::gua::DynamicTriangleNode const& node) {
  return node.clearVertices();
}

void popBackVertex(av::gua::DynamicTriangleNode const& node) {

   return node.popBackVertex();
}

void popFrontVertex(av::gua::DynamicTriangleNode const& node) {

   return node.popFrontVertex();
}

void pushVertex1(av::gua::DynamicTriangleNode const& node,
                 float x_pos, float y_pos, float z_pos) {

   return node.pushVertex(x_pos, y_pos, z_pos);
}

void pushVertex2(av::gua::DynamicTriangleNode const& node,
                 float x_pos, float y_pos, float z_pos,
                 float col_r, float col_g, float col_b) {

   return node.pushVertex(x_pos, y_pos, z_pos,
                          col_r, col_g, col_b, 1.0);
}

void pushVertex3(av::gua::DynamicTriangleNode const& node,
                 float x_pos, float y_pos, float z_pos,
                 float thickness,
                 float u, float v) {

   return node.pushVertex(x_pos, y_pos, z_pos,
                          0.0f, 0.0f, 0.0f, 1.0, //black vertex
                          thickness, u, v);
}

void pushVertex4(av::gua::DynamicTriangleNode const& node,
                 float x_pos, float y_pos, float z_pos,
                 float col_r, float col_g, float col_b,
                 float thickness) {

   return node.pushVertex(x_pos, y_pos, z_pos,
                          col_r, col_g, col_b, 1.0,
                          thickness);
}

void pushVertex5(av::gua::DynamicTriangleNode const& node,
                 float x_pos, float y_pos, float z_pos,
                 float col_r, float col_g, float col_b,
                 float thickness,
                 float u, float v) {

   return node.pushVertex(x_pos, y_pos, z_pos,
                          col_r, col_g, col_b, 1.0,
                          thickness, u , v);
}

void updateVertex1(av::gua::DynamicTriangleNode const& node,
                 int vertex_id, float x_pos, float y_pos, float z_pos) {

   return node.updateVertex(vertex_id, x_pos, y_pos, z_pos);
}

void updateVertex2(av::gua::DynamicTriangleNode const& node,
                 int vertex_id, float x_pos, float y_pos, float z_pos,
                 float col_r, float col_g, float col_b) {

   return node.updateVertex(vertex_id, x_pos, y_pos, z_pos,
                          col_r, col_g, col_b, 1.0);
}

void updateVertex3(av::gua::DynamicTriangleNode const& node,
                 int vertex_id, float x_pos, float y_pos, float z_pos,
                 float thickness,
                 float u, float v) {

   return node.updateVertex(vertex_id, x_pos, y_pos, z_pos,
                          0.0f, 0.0f, 0.0f, 1.0, //black vertex
                          thickness, u, v);
}

void updateVertex4(av::gua::DynamicTriangleNode const& node,
                 int vertex_id, float x_pos, float y_pos, float z_pos,
                 float col_r, float col_g, float col_b,
                 float thickness) {

   return node.updateVertex(vertex_id, x_pos, y_pos, z_pos,
                          col_r, col_g, col_b, 1.0,
                          thickness);
}

void updateVertex5(av::gua::DynamicTriangleNode const& node,
                 int vertex_id, float x_pos, float y_pos, float z_pos,
                 float col_r, float col_g, float col_b,
                 float thickness,
                 float u, float v) {

   return node.updateVertex(vertex_id, x_pos, y_pos, z_pos,
                          col_r, col_g, col_b, 1.0,
                          thickness, u , v);
}

void enqueueVertex1(av::gua::DynamicTriangleNode const& node,
                 float x_pos, float y_pos, float z_pos) {

   return node.enqueueVertex(x_pos, y_pos, z_pos);
}

void enqueueVertex2(av::gua::DynamicTriangleNode const& node,
                 float x_pos, float y_pos, float z_pos,
                 float col_r, float col_g, float col_b) {

   return node.enqueueVertex(x_pos, y_pos, z_pos,
                          col_r, col_g, col_b, 1.0);
}

void enqueueVertex3(av::gua::DynamicTriangleNode const& node,
                 float x_pos, float y_pos, float z_pos,
                 float thickness) {

   return node.enqueueVertex(x_pos, y_pos, z_pos,
                          0.0f, 0.0f, 0.0f, 1.0, //black vertex
                          thickness);
}

void enqueueVertex4(av::gua::DynamicTriangleNode const& node,
                 float x_pos, float y_pos, float z_pos,
                 float col_r, float col_g, float col_b,
                 float thickness) {

   return node.enqueueVertex(x_pos, y_pos, z_pos,
                          col_r, col_g, col_b, 1.0,
                          thickness);
}

void startVertexList(av::gua::DynamicTriangleNode& node) {
  node.startVertexList();
}

void endVertexList(av::gua::DynamicTriangleNode& node) {
  node.endVertexList();
}

void init_DynamicTriangleNode()
{
  register_ptr_to_python<av::Link<av::gua::DynamicTriangleNode> >();

  register_field<av::gua::SFDynamicTriangleNode>("SFDynamicTriangleNode");
  register_multifield<av::gua::MFDynamicTriangleNode>("MFDynamicTriangleNode");

  class_<av::gua::DynamicTriangleNode,
         av::Link<av::gua::DynamicTriangleNode>,
         bases<av::FieldContainer>, boost::noncopyable> ("DynamicTriangleNode", "docstring", no_init)
         .def("clear_vertices", &clearVertices)         
         .def("pop_back_vertex", &popBackVertex)
         .def("pop_front_vertex", &popFrontVertex)
         .def("push_vertex", &pushVertex1)
         .def("push_vertex", &pushVertex2)
         .def("push_vertex", &pushVertex3)
         .def("push_vertex", &pushVertex4)
         .def("push_vertex", &pushVertex5)
         .def("update_vertex", &updateVertex1)
         .def("update_vertex", &updateVertex2)
         .def("update_vertex", &updateVertex3)
         .def("update_vertex", &updateVertex4)
         .def("update_vertex", &updateVertex5)
         .def("enqueue_vertex", &enqueueVertex1)
         .def("enqueue_vertex", &enqueueVertex2)
         .def("enqueue_vertex", &enqueueVertex3)
         .def("enqueue_vertex", &enqueueVertex4)
         .def("start_vertex_list", &startVertexList)
         .def("end_vertex_list", &endVertexList)
         ;

}
