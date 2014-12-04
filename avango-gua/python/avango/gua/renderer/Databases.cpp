#include "Databases.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/WindowBase.hpp>
#include <avango/gua/renderer/MaterialShaderDescription.hpp>
#include <avango/gua/renderer/Material.hpp>
#include <gua/guacamole.hpp>

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

template <typename T>
void setMaterialUniform(std::string const& materialName, std::string const& uniformName, T const& value) {
  // ::gua::MaterialDatabase::instance()->lookup(materialName)->set_uniform(uniformName, value);
}

void reload_materials() {
  // ::gua::MaterialDatabase::instance()->reload_all();
  // ::gua::ShadingModelDatabase::instance()->reload_all();
}

av::Link<av::gua::Material> create_material_from_description(av::gua::MaterialShaderDescription const& desc, std::string const& materialName) {

  auto shader(std::make_shared<gua::MaterialShader>(materialName, desc.getGuaMaterialShaderDescription()));
  ::gua::MaterialShaderDatabase::instance()->add(shader);
  return av::Link<av::gua::Material>(new av::gua::Material(shader->get_default_material()));
}

void register_window(std::string const& name, av::gua::WindowBase const& window) {
  ::gua::WindowDatabase::instance()->add(name, window.getGuaWindowBase());
}

void init_Databases()
{
  // def("load_shading_models_from", &gua::ShadingModelDatabase::load_shading_models_from);
  // def("load_shading_model", &gua::ShadingModelDatabase::load_shading_model);
  // def("load_materials_from", &gua::MaterialDatabase::load_materials_from);
  // def("load_material", &gua::MaterialDatabase::load_material);
  // def("set_material_uniform", &setMaterialUniform<float>);
  // def("set_material_uniform", &setMaterialUniform<int>);
  // def("set_material_uniform", &setMaterialUniform<unsigned>);
  // def("set_material_uniform", &setMaterialUniform<bool>);
  // def("set_material_uniform", &setMaterialUniform<std::string>);
  // def("set_material_uniform", &setMaterialUniform< ::gua::math::vec2>);
  // def("set_material_uniform", &setMaterialUniform< ::gua::math::vec3>);
  // def("set_material_uniform", &setMaterialUniform< ::gua::math::vec4>);
  // def("set_material_uniform", &setMaterialUniform< ::gua::math::mat4>);
  // def("reload_materials", &reload_materials);
  def("create_material_from_description", &create_material_from_description);
  def("register_window", &register_window);
}
