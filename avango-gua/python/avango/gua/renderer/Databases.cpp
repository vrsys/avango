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

void register_material_shader(av::gua::MaterialShaderDescription const& desc, std::string const& materialName) {
  auto shader(std::make_shared<gua::MaterialShader>(materialName, desc.getGuaMaterialShaderDescription()));
  ::gua::MaterialShaderDatabase::instance()->add(shader);
}

void register_window(std::string const& name, av::gua::WindowBase const& window) {
  ::gua::WindowDatabase::instance()->add(name, window.getGuaWindowBase());
}

void init_Databases()
{

  def("register_material_shader", &register_material_shader);
  def("register_window", &register_window);
}


