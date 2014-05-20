#include "NURBSNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/NURBSNode.hpp>

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

void init_NURBSNode()
 {
  register_field<av::gua::SFNURBSNode>("SFNURBSNode");
  register_multifield<av::gua::MFNURBSNode>("MFNURBSNode");
  class_<av::gua::NURBSNode, av::Link<av::gua::NURBSNode>, bases<av::gua::Node>, boost::noncopyable >("NURBSNode", "docstring", no_init);

  enum_<av::gua::NURBSNode::ShadowModeEnum>("ShadowMode")
        .value("OFF", av::gua::NURBSNode::OFF)
        .value("LOW_QUALITY", av::gua::NURBSNode::LOW_QUALITY)
        .value("HIGH_QUALITY", av::gua::NURBSNode::HIGH_QUALITY)
        ;
 }
