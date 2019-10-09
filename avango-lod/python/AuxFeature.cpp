#include "AuxFeature.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/lod/renderer/AuxFeature.hpp>

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

void init_AuxFeature()
{
  register_ptr_to_python<av::Link< av::gua::lod::AuxFeature> >();

  register_field<av::gua::lod::SFAuxFeature>("SFAuxFeature");
  register_multifield<av::gua::lod::MFAuxFeature>("MFAuxFeature");

  class_< av::gua::lod::AuxFeature,
          av::Link<av::gua::lod::AuxFeature>,
          bases<av::FieldContainer>, boost::noncopyable>("AuxFeature", "docstring", no_init)
          .def("get_camera_id", &av::gua::lod::AuxFeature::getCameraId)
          .def("get_using_count", &av::gua::lod::AuxFeature::getUsingCount)
          .def("get_coords", &av::gua::lod::AuxFeature::getCoords)
          .def("get_error", &av::gua::lod::AuxFeature::getError)
    ;

  // register_field<av::gua::lod::SFAux>("SFAux");
  // register_multifield<av::gua::lod::MFAux>("MFAux");
}
