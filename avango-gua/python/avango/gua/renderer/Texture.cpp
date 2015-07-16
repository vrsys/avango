#include "Texture.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/Fields.hpp>
#include <avango/gua/Types.hpp>

#include <gua/renderer/Texture2D.hpp>
#include <gua/renderer/TextureCube.hpp>
#include <gua/databases/TextureDatabase.hpp>

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

void createTexture(std::string const& file_name) {
  std::shared_ptr< ::gua::Texture> new_tex(new gua::Texture2D(file_name));
  ::gua::TextureDatabase::instance()->add(file_name, new_tex);
}

void createTextureCube(std::string const& name,
                       std::string const& file_px,
                       std::string const& file_nx,
                       std::string const& file_py,
                       std::string const& file_ny,
                       std::string const& file_pz,
                       std::string const& file_nz) {

  std::shared_ptr< ::gua::Texture> new_tex(
    new gua::TextureCube(file_px, file_nx, file_py, file_ny, file_pz, file_nz));

  ::gua::TextureDatabase::instance()->add(name, new_tex);
}

void init_Texture()
 {
      // wrapping gua::math::vec3 functionality
    def("create_texture", &createTexture);
    def("create_texture_cube", &createTextureCube);

    // register as a field
//    register_field<av::gua::SFTexture>("SFTexture");
//    register_multifield<av::gua::MFColor>("MFColor");
 }
