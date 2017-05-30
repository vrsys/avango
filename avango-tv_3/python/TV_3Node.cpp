#include "TV_3Node.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/tv_3/scenegraph/TV_3Node.hpp>

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

void init_TV_3Node()
{
  register_ptr_to_python<av::Link<av::gua::tv_3::TV_3Node> >();
  register_field<av::gua::tv_3::SFTV_3Node>("SFTV_3Node");
  register_multifield<av::gua::tv_3::MFTV_3Node>("MFTV_3Node");
  class_<av::gua::tv_3::TV_3Node, av::Link<av::gua::tv_3::TV_3Node>, bases<av::gua::GeometryNode>,
    boost::noncopyable >("TV_3Node", "docstring", no_init);
}
