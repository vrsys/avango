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

av::Link<av::gua::Material> create_material_from_description(av::gua::MaterialShaderDescription const& desc, std::string const& materialName) {

  auto shader(std::make_shared<gua::MaterialShader>(materialName, desc.getGuaMaterialShaderDescription()));
  ::gua::MaterialShaderDatabase::instance()->add(shader);
  return av::Link<av::gua::Material>(new av::gua::Material(shader->get_default_material()));
}

av::Link<av::gua::Material> create_default_material() {
  auto shader(gua::MaterialShaderDatabase::instance()->lookup("gua_default_material"));
  return av::Link<av::gua::Material>(new av::gua::Material(shader->make_new_material()));
}

void register_window(std::string const& name, av::gua::WindowBase const& window) {
  ::gua::WindowDatabase::instance()->add(name, window.getGuaWindowBase());
}

void init_Databases()
{

  def("create_material_from_description", &create_material_from_description);
  def("create_default_material", &create_default_material);
  def("register_window", &register_window);
}
