#include "TexturedScreenSpaceQuadNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/TexturedScreenSpaceQuadNode.hpp>

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

void init_TexturedScreenSpaceQuadNode()
 {
   register_ptr_to_python<av::Link<av::gua::TexturedScreenSpaceQuadNode> >();
  register_field<av::gua::SFTexturedScreenSpaceQuadNode>("SFTexturedScreenSpaceQuadNode");
  register_multifield<av::gua::MFTexturedScreenSpaceQuadNode>("MFTexturedScreenSpaceQuadNode");
  class_<av::gua::TexturedScreenSpaceQuadNode,
        av::Link<av::gua::TexturedScreenSpaceQuadNode>,
        bases<av::gua::Node>, boost::noncopyable >("TexturedScreenSpaceQuadNode", "docstring", no_init)
        .def("pixel_to_texcoords", &av::gua::TexturedScreenSpaceQuadNode::pixel_to_texcoords)
        ;
 }
