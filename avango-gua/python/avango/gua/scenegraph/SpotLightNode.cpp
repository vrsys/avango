#include "SpotLightNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/SpotLightNode.hpp>

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

void init_SpotLightNode()
 {
  register_field<av::gua::SFSpotLightNode>("SFSpotLightNode");
  register_multifield<av::gua::MFSpotLightNode>("MFSpotLightNode");
  class_<av::gua::SpotLightNode, av::Link<av::gua::SpotLightNode>, bases<av::gua::Node>, boost::noncopyable >("SpotLightNode", "docstring", no_init);
 }
