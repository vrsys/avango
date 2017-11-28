#include "SPointsNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/scenegraph/SPointsNode.hpp>

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

void init_SPointsNode()
{
  register_field<av::gua::SFSPointsNode>("SFSPointsNode");
  register_multifield<av::gua::MFSPointsNode>("MFSPointsNode");
  class_<av::gua::SPointsNode, av::Link<av::gua::SPointsNode>,
    bases<av::gua::GeometryNode>,
    boost::noncopyable >("SPointsNode", "docstring", no_init);
}
