#include "PointLightNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/PointLightNode.hpp>

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

void init_PointLightNode()
 {
  register_field<av::gua::SFPointLightNode>("SFPointLightNode");
  register_multifield<av::gua::MFPointLightNode>("MFPointLightNode");
  class_<av::gua::PointLightNode, av::Link<av::gua::PointLightNode>, bases<av::gua::Node>, boost::noncopyable >("PointLightNode", "docstring", no_init);
 }
