#include "AuxSparsePoint.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/lod/renderer/AuxSparsePoint.hpp>

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

void init_AuxSparsePoint()
{
  register_ptr_to_python<av::Link< av::gua::lod::AuxSparsePoint> >();

  register_field<av::gua::lod::SFAuxSparsePoint>("SFAuxSparsePoint");
  register_multifield<av::gua::lod::MFAuxSparsePoint>("MFAuxSparsePoint");

  class_< av::gua::lod::AuxSparsePoint,
          av::Link<av::gua::lod::AuxSparsePoint>,
          bases<av::FieldContainer>, boost::noncopyable>("AuxSparsePoint", "docstring", no_init)
          .def("get_pos", &av::gua::lod::AuxSparsePoint::getPosition)
          .def("r", &av::gua::lod::AuxSparsePoint::getRed)
          .def("g", &av::gua::lod::AuxSparsePoint::getGreen)
          .def("b", &av::gua::lod::AuxSparsePoint::getBlue)
          .def("a", &av::gua::lod::AuxSparsePoint::getAlpha)
    ;

  // register_field<av::gua::lod::SFAux>("SFAux");
  // register_multifield<av::gua::lod::MFAux>("MFAux");
}
