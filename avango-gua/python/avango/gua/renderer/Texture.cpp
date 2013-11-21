#include "Texture.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/Fields.hpp>
#include <avango/gua/Types.hpp>

#include <gua/renderer/Texture2D.hpp>
#include <gua/databases/TextureDatabase.hpp>

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

void createTexture(std::string const& file_name) {
  std::shared_ptr< ::gua::Texture> new_tex(new gua::Texture2D(file_name));
  ::gua::TextureDatabase::instance()->add(file_name, new_tex);
}

void init_Texture()
 {
      // wrapping gua::math::vec3 functionality
    def("create_texture", &createTexture);

    // register as a field
//    register_field<av::gua::SFTexture>("SFTexture");
//    register_multifield<av::gua::MFColor>("MFColor");
 }
