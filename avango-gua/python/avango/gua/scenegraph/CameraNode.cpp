#include "CameraNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/CameraNode.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>

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


void init_CameraNode()
 {

  register_field<av::gua::SFCameraNode>("SFCameraNode");
  register_multifield<av::gua::MFCameraNode>("MFCameraNode");
  class_<av::gua::CameraNode, av::Link<av::gua::CameraNode>, bases<av::FieldContainer>, boost::noncopyable >("CameraNode", "docstring", no_init)
    .def("get_frustum", &av::gua::CameraNode::get_frustum)
    ;

  enum_<av::gua::CameraNode::ProjectionMode>("ProjectionMode")
        .value("PERSPECTIVE", av::gua::CameraNode::PERSPECTIVE)
        .value("ORTHOGRAPHIC", av::gua::CameraNode::ORTHOGRAPHIC)
        ;

  enum_<av::gua::CameraNode::CameraMode>("CameraMode")
        .value("CENTER", av::gua::CameraNode::CENTER)
        .value("LEFT", av::gua::CameraNode::LEFT)
        .value("RIGHT", av::gua::CameraNode::RIGHT)
        ;
 }
