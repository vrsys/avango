#include "Video3DLoader.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/Video3DLoader.hpp>

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

av::Link<av::gua::Node> createVideo3DFromFile(
                                          av::gua::Video3DLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& fileName,
                                          std::string const& fallbackMaterial,
                                          int flags) {

   return loader.createVideo3DFromFile(nodeName, fileName, fallbackMaterial,
                                static_cast<av::gua::Video3DLoader::Flags>(flags));
}

void init_Video3DLoader()
{
  class_<av::gua::Video3DLoader,
         av::Link<av::gua::Video3DLoader>,
         bases<av::FieldContainer>, boost::noncopyable> ("Video3DLoader", "docstring", no_init)
         .def("create_geometry_from_file", &createVideo3DFromFile)
         ;

  enum_<av::gua::Video3DLoader::Flags>("LoaderFlags")
        .value("DEFAULTS", av::gua::Video3DLoader::DEFAULTS)
        .value("MAKE_PICKABLE", av::gua::Video3DLoader::MAKE_PICKABLE)
        .value("NORMALIZE_SCALE", av::gua::Video3DLoader::NORMALIZE_SCALE)
        .value("NORMALIZE_POSITION", av::gua::Video3DLoader::NORMALIZE_POSITION)
        ;

  register_field<av::gua::SFVideo3DLoader>("SFVideo3DLoader");
  register_multifield<av::gua::MFVideo3DLoader>("MFVideo3DLoader");

}
