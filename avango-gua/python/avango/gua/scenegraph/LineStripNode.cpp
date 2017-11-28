#include "LineStripNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/scenegraph/LineStripNode.hpp>

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

void init_LineStripNode()
{
  register_ptr_to_python<av::Link<av::gua::LineStripNode> >();

  register_field<av::gua::SFLineStripNode>("SFLineStripNode");
  register_multifield<av::gua::MFLineStripNode>("MFLineStripNode");
  class_<av::gua::LineStripNode, av::Link<av::gua::LineStripNode>,
    bases<av::gua::GeometryNode>,
    boost::noncopyable >("LineStripNode", "docstring", no_init);
}
