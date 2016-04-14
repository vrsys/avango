#include "ScreenNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/ScreenNode.hpp>

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

void init_ScreenNode()
 {
   register_ptr_to_python<av::Link<av::gua::ScreenNode> >();
  register_field<av::gua::SFScreenNode>("SFScreenNode");
  register_multifield<av::gua::MFScreenNode>("MFScreenNode");
  class_<av::gua::ScreenNode, av::Link<av::gua::ScreenNode>, bases<av::gua::Node>, boost::noncopyable >("ScreenNode", "docstring", no_init);
 }
