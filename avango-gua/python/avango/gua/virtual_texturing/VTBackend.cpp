#include "VTBackend.hpp"

#include <avango/gua/virtual_texturing/VTBackend.hpp>
#include <avango/python/register_field.h>
#include <boost/python.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost {
namespace python {
template <class T> struct pointee<av::Link<T>> { using type = T; };
}
}

void start_backend(av::gua::VTBackend const &vt_backend) {
  vt_backend.start_backend();
}

void stop_backend(av::gua::VTBackend const &vt_backend) {
  vt_backend.stop_backend();
}

void add_camera(av::gua::VTBackend const &vt_backend,
                                   av::gua::CameraNode const &camera) {
  vt_backend.add_camera(camera);
}

void init_VTBackend() {
  register_ptr_to_python<av::Link<av::gua::VTBackend>>();

  class_<av::gua::VTBackend, av::Link<av::gua::VTBackend>,
         bases<av::FieldContainer>, boost::noncopyable>("VTBackend",
                                                        "docstring")
      .def("start_backend", &start_backend)
      .def("stop_backend", &stop_backend)
      .def("add_camera", &add_camera);

  register_field<av::gua::SFVTBackend>("SFVTBackend");
  register_multifield<av::gua::MFVTBackend>("MFVTBackend");
}