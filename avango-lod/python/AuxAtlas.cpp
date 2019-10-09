#include "AuxAtlas.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/lod/renderer/AuxAtlas.hpp>

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

void init_AuxAtlas()
{
  register_ptr_to_python<av::Link< av::gua::lod::AuxAtlas> >();

  register_field<av::gua::lod::SFAuxAtlas>("SFAuxAtlas");
  register_multifield<av::gua::lod::MFAuxAtlas>("MFAuxAtlas");

  class_< av::gua::lod::AuxAtlas,
          av::Link<av::gua::lod::AuxAtlas>,
          bases<av::FieldContainer>, boost::noncopyable>("AuxAtlas", "docstring", no_init)
          .def("get_tiles", &av::gua::lod::AuxAtlas::getNumAtlasTiles)
          .def("get_width", &av::gua::lod::AuxAtlas::getAtlasWidth)
          .def("get_height", &av::gua::lod::AuxAtlas::getAtlasHeight)
          .def("get_rotated", &av::gua::lod::AuxAtlas::getRotated)
    ;

  // register_field<av::gua::lod::SFAux>("SFAux");
  // register_multifield<av::gua::lod::MFAux>("MFAux");
}
