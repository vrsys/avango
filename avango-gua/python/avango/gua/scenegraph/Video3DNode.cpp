#include "Video3DNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/Video3DNode.hpp>

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

void init_Video3DNode()
 {
  register_field<av::gua::SFVideo3DNode>("SFVideo3DNode");
  register_multifield<av::gua::MFVideo3DNode>("MFVideo3DNode");
  class_<av::gua::Video3DNode, av::Link<av::gua::Video3DNode>, bases<av::gua::Node>, boost::noncopyable >("Video3DNode", "docstring", no_init);

  enum_<av::gua::Video3DNode::ShadowModeEnum>("ShadowMode")
        .value("OFF", av::gua::Video3DNode::OFF)
        .value("LOW_QUALITY", av::gua::Video3DNode::LOW_QUALITY)
        .value("HIGH_QUALITY", av::gua::Video3DNode::HIGH_QUALITY)
        ;
 }
