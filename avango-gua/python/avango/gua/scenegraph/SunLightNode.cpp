#include "SunLightNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/SunLightNode.hpp>

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

void init_SunLightNode()
 {
  register_field<av::gua::SFSunLightNode>("SFSunLightNode");
  register_multifield<av::gua::MFSunLightNode>("MFSunLightNode");
  class_<av::gua::SunLightNode, av::Link<av::gua::SunLightNode>, bases<av::gua::Node>, boost::noncopyable >("SunLightNode", "docstring", no_init);
 }
