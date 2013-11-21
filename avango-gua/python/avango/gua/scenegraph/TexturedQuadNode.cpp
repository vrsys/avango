#include "TexturedQuadNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/TexturedQuadNode.hpp>

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

void init_TexturedQuadNode()
 {
  register_field<av::gua::SFTexturedQuadNode>("SFTexturedQuadNode");
  register_multifield<av::gua::MFTexturedQuadNode>("MFTexturedQuadNode");
  class_<av::gua::TexturedQuadNode, av::Link<av::gua::TexturedQuadNode>, bases<av::gua::Node>, boost::noncopyable >("TexturedQuadNode", "docstring", no_init);
 }
