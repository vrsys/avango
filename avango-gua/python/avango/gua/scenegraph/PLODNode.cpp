#include "PLODNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/PLODNode.hpp>

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

void init_PLODNode()
 {
  register_field<av::gua::SFPLODNode>("SFPLODNode");
  register_multifield<av::gua::MFPLODNode>("MFPLODNode");
  class_<av::gua::PLODNode, av::Link<av::gua::PLODNode>, bases<av::gua::Node>, boost::noncopyable >("PLODNode", "docstring", no_init);

  enum_<av::gua::PLODNode::ShadowModeEnum>("ShadowMode")
        .value("OFF", av::gua::PLODNode::OFF)
        .value("LOW_QUALITY", av::gua::PLODNode::LOW_QUALITY)
        .value("HIGH_QUALITY", av::gua::PLODNode::HIGH_QUALITY)
        ;
 }
