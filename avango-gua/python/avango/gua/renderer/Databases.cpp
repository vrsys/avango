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
       using type = T;
     };
   }
 }

av::Link<av::gua::MaterialShaderDescription> lookup_material_shader(std::string const& materialName) {
  auto shader(::gua::MaterialShaderDatabase::instance()->lookup(materialName));
  auto description(shader->get_description());
  return av::Link<av::gua::MaterialShaderDescription>(new av::gua::MaterialShaderDescription(description));
}

void register_material_shader(av::gua::MaterialShaderDescription const& desc, std::string const& materialName) {
  auto shader(std::make_shared<gua::MaterialShader>(materialName, desc.getGuaMaterialShaderDescription()));
  ::gua::MaterialShaderDatabase::instance()->add(shader);
}

void register_window(std::string const& name, av::gua::WindowBase const& window) {
  ::gua::WindowDatabase::instance()->add(name, window.getGuaWindowBase());
}

void init_Databases()
{

  def("lookup_material_shader", &lookup_material_shader);
  def("register_material_shader", &register_material_shader);
  def("register_window", &register_window);
}


