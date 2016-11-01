#include "PLodNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/lod/scenegraph/PLodNode.hpp>

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

void init_PLodNode()
{
  register_field<av::gua::lod::SFPLodNode>("SFPLodNode");
  register_multifield<av::gua::lod::MFPLodNode>("MFPLodNode");

  class_< av::gua::lod::PLodNode,
          av::Link<av::gua::lod::PLodNode>,
          bases<av::gua::GeometryNode>, boost::noncopyable 
    >("PLodNode", "docstring", no_init)
    ;
}
