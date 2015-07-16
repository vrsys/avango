#include "TriMeshNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/scenegraph/TriMeshNode.hpp>

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

void init_TriMeshNode()
{
  register_field<av::gua::SFTriMeshNode>("SFTriMeshNode");
  register_multifield<av::gua::MFTriMeshNode>("MFTriMeshNode");
  class_<av::gua::TriMeshNode, av::Link<av::gua::TriMeshNode>,
    bases<av::gua::GeometryNode>,
    boost::noncopyable >("TriMeshNode", "docstring", no_init);
}
