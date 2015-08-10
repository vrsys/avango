#include "DepthMapNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/DepthMapNode.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>

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

void init_DepthMapNode()
{
  register_field<av::gua::SFDepthMapNode>("SFDepthMapNode");
  register_multifield<av::gua::MFDepthMapNode>("MFDepthMapNode");
  class_<av::gua::DepthMapNode, av::Link<av::gua::DepthMapNode>,
    bases<av::gua::GeometryNode>,
    boost::noncopyable >("DepthMapNode", "docstring", no_init);
}
