#include "AuxView.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/lod/renderer/AuxView.hpp>

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

void init_AuxView()
{
  register_ptr_to_python<av::Link< av::gua::lod::AuxView> >();

  register_field<av::gua::lod::SFAuxView>("SFAuxView");
  register_multifield<av::gua::lod::MFAuxView>("MFAuxView");

  class_< av::gua::lod::AuxView,
          av::Link<av::gua::lod::AuxView>,
          bases<av::FieldContainer>, boost::noncopyable>("AuxView", "docstring", no_init)
          .def("get_camera_id", &av::gua::lod::AuxView::getCameraId)
          .def("get_position", &av::gua::lod::AuxView::getPosition)
          .def("get_transform", &av::gua::lod::AuxView::getTransform)
          .def("get_focal_value_x", &av::gua::lod::AuxView::getFocalValueX)
          .def("get_focal_value_y", &av::gua::lod::AuxView::getFocalValueY)
          .def("get_center_x", &av::gua::lod::AuxView::getCenterX)
          .def("get_center_y", &av::gua::lod::AuxView::getCenterY)
          .def("get_distortion", &av::gua::lod::AuxView::getDistortion)
          .def("get_image_width", &av::gua::lod::AuxView::getImageWidth)
          .def("get_image_height", &av::gua::lod::AuxView::getImageHeight)
          .def("get_atlas_tile_id", &av::gua::lod::AuxView::getAtlasTileId)
          .def("get_image_file", &av::gua::lod::AuxView::getImageFile)
          
    ;

  // register_field<av::gua::lod::SFAux>("SFAux");
  // register_multifield<av::gua::lod::MFAux>("MFAux");
}
