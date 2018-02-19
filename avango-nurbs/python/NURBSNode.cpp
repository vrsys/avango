#include "NURBSNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/nurbs/scenegraph/NURBSNode.hpp>

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

void init_NURBSNode()
{
  register_ptr_to_python<av::Link<av::gua::nurbs::NURBSNode> >();

  register_field<av::gua::nurbs::SFNURBSNode>("SFNURBSNode");
  register_multifield<av::gua::nurbs::MFNURBSNode>("MFNURBSNode");
  class_<av::gua::nurbs::NURBSNode, av::Link<av::gua::nurbs::NURBSNode>, bases<av::gua::GeometryNode>,
    boost::noncopyable >("NURBSNode", "docstring", no_init);
}
