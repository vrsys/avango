#include "AuxAtlasTile.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/lod/renderer/AuxAtlasTile.hpp>

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

void init_AuxAtlasTile()
{
  register_ptr_to_python<av::Link< av::gua::lod::AuxAtlasTile> >();

  register_field<av::gua::lod::SFAuxAtlasTile>("SFAuxAtlasTile");
  register_multifield<av::gua::lod::MFAuxAtlasTile>("MFAuxAtlasTile");

  class_< av::gua::lod::AuxAtlasTile,
          av::Link<av::gua::lod::AuxAtlasTile>,
          bases<av::FieldContainer>, boost::noncopyable>("AuxAtlasTile", "docstring", no_init)
          .def("get_tile_id", &av::gua::lod::AuxAtlasTile::getAtlasTileId)
          .def("get_x", &av::gua::lod::AuxAtlasTile::getX)
          .def("get_y", &av::gua::lod::AuxAtlasTile::getY)
          .def("get_width", &av::gua::lod::AuxAtlasTile::getWidth)
          .def("get_height", &av::gua::lod::AuxAtlasTile::getHeight)
    ;

  // register_field<av::gua::lod::SFAux>("SFAux");
  // register_multifield<av::gua::lod::MFAux>("MFAux");
}
