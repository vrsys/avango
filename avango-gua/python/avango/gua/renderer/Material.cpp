#include "Material.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/Material.hpp>

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

list list_uniforms(av::gua::Material& mat) {
    list result;
    auto const& uniforms = mat.getGuaMaterial()->get_uniforms();

    for (auto& uniform : uniforms){
      result.append(uniform.first);
    }
    return result;
}

void init_Material()
 {

  register_field<av::gua::SFMaterial>("SFMaterial");
  register_multifield<av::gua::MFMaterial>("MFMaterial");
  class_<av::gua::Material,
         av::Link<av::gua::Material>,
         bases<av::FieldContainer>, boost::noncopyable >("Material", "docstring", no_init)
         .def("list_uniforms", list_uniforms)
         .def("reset_view_uniform", &av::gua::Material::reset_view_uniform)
         .def("set_uniform", &av::gua::Material::set_uniform<float>)
         .def("set_uniform", &av::gua::Material::set_uniform<int>)
         // .def("set_uniform", &av::gua::Material::set_uniform<unsigned>)
         .def("set_uniform", &av::gua::Material::set_uniform<bool>)
         .def("set_uniform", &av::gua::Material::set_uniform<std::string>)
         .def("set_uniform", &av::gua::Material::set_uniform< ::gua::math::vec2f>)
         .def("set_uniform", &av::gua::Material::set_uniform< ::gua::math::vec3f>)
         .def("set_uniform", &av::gua::Material::set_uniform< ::gua::math::vec4f>)
         .def("set_uniform", &av::gua::Material::set_uniform< ::gua::math::mat4f>)
         .def("set_view_uniform", &av::gua::Material::set_view_uniform<float>)
         .def("set_view_uniform", &av::gua::Material::set_view_uniform<int>)
         // .def("set_view_uniform", &av::gua::Material::set_view_uniform<unsigned>)
         .def("set_view_uniform", &av::gua::Material::set_view_uniform<bool>)
         .def("set_view_uniform", &av::gua::Material::set_view_uniform<std::string>)
         .def("set_view_uniform", &av::gua::Material::set_view_uniform< ::gua::math::vec2f>)
         .def("set_view_uniform", &av::gua::Material::set_view_uniform< ::gua::math::vec3f>)
         .def("set_view_uniform", &av::gua::Material::set_view_uniform< ::gua::math::vec4f>)
         .def("set_view_uniform", &av::gua::Material::set_view_uniform< ::gua::math::mat4f>)
         ;
 }
