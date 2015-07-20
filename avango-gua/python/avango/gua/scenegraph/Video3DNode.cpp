#include "Video3DNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/scenegraph/Video3DNode.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
  namespace python
  {
    template <class T> struct pointee<av::Link<T> >
    {
      using type = T;
    };
  }
}

void init_Video3DNode()
{
  register_field<av::gua::SFVideo3DNode>("SFVideo3DNode");
  register_multifield<av::gua::MFVideo3DNode>("MFVideo3DNode");
  class_<av::gua::Video3DNode, av::Link<av::gua::Video3DNode>,
    bases<av::gua::GeometryNode>,
    boost::noncopyable >("Video3DNode", "docstring", no_init);
}
