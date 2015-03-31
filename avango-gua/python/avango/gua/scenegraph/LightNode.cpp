#include "LightNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/LightNode.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost {
namespace python {
template <class T>
struct pointee<av::Link<T> > {
  typedef T type;
};
}
}

void init_LightNode() {
  register_field<av::gua::SFLightNode>("SFLightNode");
  register_multifield<av::gua::MFLightNode>("MFLightNode");
  class_<av::gua::LightNode,
         av::Link<av::gua::LightNode>,
         bases<av::gua::Node>,
         boost::noncopyable>("LightNode", "docstring", no_init);


  enum_<av::gua::LightNode::LightTypeEnum>("LightType")
      .value("POINT", av::gua::LightNode::POINT)
      .value("SPOT", av::gua::LightNode::SPOT)
      .value("SUN", av::gua::LightNode::SUN)
        ;

}
