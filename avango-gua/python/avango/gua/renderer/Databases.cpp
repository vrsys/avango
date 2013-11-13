#include "Databases.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
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
  ::gua::MaterialDatabase::instance()->lookup(materialName)->set_uniform(uniformName, value);
}

void reload_materials() {
  ::gua::MaterialDatabase::instance()->reload_all();
  ::gua::ShadingModelDatabase::instance()->reload_all();
}

void init_Databases()
{
  def("load_shading_models_from", &gua::ShadingModelDatabase::load_shading_models_from);
  def("load_materials_from", &gua::MaterialDatabase::load_materials_from);
  def("set_material_uniform", &setMaterialUniform<float>);
  def("set_material_uniform", &setMaterialUniform<int>);
  def("set_material_uniform", &setMaterialUniform<unsigned>);
  def("set_material_uniform", &setMaterialUniform<bool>);
  def("set_material_uniform", &setMaterialUniform<std::string>);
  def("set_material_uniform", &setMaterialUniform< ::gua::math::vec2>);
  def("set_material_uniform", &setMaterialUniform< ::gua::math::vec3>);
  def("set_material_uniform", &setMaterialUniform< ::gua::math::vec4>);
  def("set_material_uniform", &setMaterialUniform< ::gua::math::mat4>);
  def("reload_materials", &reload_materials);
}
