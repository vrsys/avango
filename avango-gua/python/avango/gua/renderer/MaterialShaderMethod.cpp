#include "MaterialShaderMethod.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/MaterialShaderMethod.hpp>

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


void init_MaterialShaderMethod()
 {

  register_field<av::gua::SFMaterialShaderMethod>("SFMaterialShaderMethod");
  register_multifield<av::gua::MFMaterialShaderMethod>("MFMaterialShaderMethod");
  class_<av::gua::MaterialShaderMethod,
         av::Link<av::gua::MaterialShaderMethod>,
         bases<av::FieldContainer>, boost::noncopyable >("MaterialShaderMethod", "docstring", no_init)
         .def("set_uniform", &av::gua::MaterialShaderMethod::set_uniform<float>)
         .def("set_uniform", &av::gua::MaterialShaderMethod::set_uniform<int>)
         // .def("set_uniform", &av::gua::MaterialShaderMethod::set_uniform<unsigned>)
         .def("set_uniform", &av::gua::MaterialShaderMethod::set_uniform<bool>)
         .def("set_uniform", &av::gua::MaterialShaderMethod::set_uniform<std::string>)
         .def("set_uniform", &av::gua::MaterialShaderMethod::set_uniform< ::gua::math::vec2>)
         .def("set_uniform", &av::gua::MaterialShaderMethod::set_uniform< ::gua::math::vec3>)
         .def("set_uniform", &av::gua::MaterialShaderMethod::set_uniform< ::gua::math::vec4>)
         .def("set_uniform", &av::gua::MaterialShaderMethod::set_uniform< ::gua::math::mat4>)
         .def("load_from_file", &av::gua::MaterialShaderMethod::load_from_file)
         .def("load_from_json", &av::gua::MaterialShaderMethod::load_from_json)
         ;
 }
