#include "Databases.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/WindowBase.hpp>
#include <avango/gua/renderer/MaterialShaderDescription.hpp>
#include <avango/gua/renderer/Material.hpp>
#include <avango/gua/renderer/Texture2D.hpp>
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

av::Link<av::gua::Texture2D> lookup_texture2d(std::string const& name) {
  auto tex = ::gua::TextureDatabase::instance()->lookup(name);
  auto tex2d = std::dynamic_pointer_cast< ::gua::Texture2D>(tex);
  return av::Link<av::gua::Texture2D>(new av::gua::Texture2D(tex2d));
}

av::Link<av::gua::MaterialShaderDescription> lookup_material_shader(std::string const& materialName) {
  auto shader(::gua::MaterialShaderDatabase::instance()->lookup(materialName));
  auto description(shader->get_description());
  return av::Link<av::gua::MaterialShaderDescription>(new av::gua::MaterialShaderDescription(description));
}

void register_texture2d(std::string const& name, av::gua::Texture2D const& tex) {
  ::gua::TextureDatabase::instance()->add(name, tex.getGuaTexture2D());
}

void register_window(std::string const& name, av::gua::WindowBase const& window) {
  ::gua::WindowDatabase::instance()->add(name, window.getGuaWindowBase());
}

void register_material_shader(av::gua::MaterialShaderDescription const& desc, std::string const& materialName) {
  auto shader(std::make_shared<gua::MaterialShader>(materialName, desc.getGuaMaterialShaderDescription()));
  ::gua::MaterialShaderDatabase::instance()->add(shader);
}

void init_Databases()
{
  def("lookup_material_shader", &lookup_material_shader);
  def("register_material_shader", &register_material_shader);
  def("register_window", &register_window);

  def("lookup_texture2d", &lookup_texture2d);
  def("register_texture2d", &register_texture2d);
}
