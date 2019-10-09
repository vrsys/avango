#include "OctreeNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/lod/renderer/OctreeNode.hpp>

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

void init_OctreeNode()
{
  register_ptr_to_python<av::Link< av::gua::lod::OctreeNode> >();

  register_field<av::gua::lod::SFOctreeNode>("SFOctreeNode");
  register_multifield<av::gua::lod::MFOctreeNode>("MFOctreeNode");

  class_< av::gua::lod::OctreeNode,
          av::Link<av::gua::lod::OctreeNode>,
          bases<av::FieldContainer>, boost::noncopyable>("OctreeNode", "docstring", no_init)
          .def("get_idx", &av::gua::lod::OctreeNode::getIdx)
          .def("get_child_mask", &av::gua::lod::OctreeNode::getChildMask)
          .def("get_child_idx", &av::gua::lod::OctreeNode::getChildIdx)
          .def("get_min", &av::gua::lod::OctreeNode::getMin)
          .def("get_max", &av::gua::lod::OctreeNode::getMax)
          .def("get_number_fotos", &av::gua::lod::OctreeNode::getNumberFotos)
          .def("get_foto_by_id", &av::gua::lod::OctreeNode::getFotoById)
    ;

  // register_field<av::gua::lod::SFAux>("SFAux");
  // register_multifield<av::gua::lod::MFAux>("MFAux");
}
