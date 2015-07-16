#include "LODNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/LODNode.hpp>

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

void init_LODNode()
 {
  register_field<av::gua::SFLODNode>("SFLODNode");
  register_multifield<av::gua::MFLODNode>("MFLODNode");
  class_<av::gua::LODNode, av::Link<av::gua::LODNode>, bases<av::gua::TransformNode>, boost::noncopyable >("LODNode", "docstring", no_init);
 }
