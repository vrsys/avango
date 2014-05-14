#include "TriMeshNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/TriMeshNode.hpp>

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

void init_TriMeshNode()
 {
  register_field<av::gua::SFTriMeshNode>("SFTriMeshNode");
  register_multifield<av::gua::MFTriMeshNode>("MFTriMeshNode");
  class_<av::gua::TriMeshNode, av::Link<av::gua::TriMeshNode>, bases<av::gua::Node>, boost::noncopyable >("TriMeshNode", "docstring", no_init);

  enum_<av::gua::TriMeshNode::ShadowModeEnum>("ShadowMode")
        .value("OFF", av::gua::TriMeshNode::OFF)
        .value("LOW_QUALITY", av::gua::TriMeshNode::LOW_QUALITY)
        .value("HIGH_QUALITY", av::gua::TriMeshNode::HIGH_QUALITY)
        ;
 }
