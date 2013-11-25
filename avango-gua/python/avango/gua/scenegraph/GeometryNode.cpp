#include "GeometryNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>

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

void init_GeometryNode()
 {
  register_field<av::gua::SFGeometryNode>("SFGeometryNode");
  register_multifield<av::gua::MFGeometryNode>("MFGeometryNode");
  class_<av::gua::GeometryNode, av::Link<av::gua::GeometryNode>, bases<av::gua::Node>, boost::noncopyable >("GeometryNode", "docstring", no_init);
 }
