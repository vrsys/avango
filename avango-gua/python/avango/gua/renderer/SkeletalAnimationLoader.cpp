#include "SkeletalAnimationLoader.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/SkeletalAnimationLoader.hpp>

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

av::Link<av::gua::Node> createGeometryFromFile(
                                          av::gua::SkeletalAnimationLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& fileName,
                                          av::Link<av::gua::Material> const& fallbackMaterial,
                                          int flags) {

   return loader.createGeometryFromFile(nodeName, fileName, fallbackMaterial,
                                static_cast<av::gua::SkeletalAnimationLoader::Flags>(flags));
}

void init_SkeletalAnimationLoader()
{
  class_<av::gua::SkeletalAnimationLoader,
         av::Link<av::gua::SkeletalAnimationLoader>,
         bases<av::FieldContainer>, boost::noncopyable> ("SkeletalAnimationLoader", "docstring", no_init)
         .def("create_geometry_from_file", &createGeometryFromFile)
         ;

  enum_<av::gua::SkeletalAnimationLoader::Flags>("LoaderFlags")
        .value("DEFAULTS", av::gua::SkeletalAnimationLoader::DEFAULTS)
        .value("LOAD_MATERIALS", av::gua::SkeletalAnimationLoader::LOAD_MATERIALS)
        .value("OPTIMIZE_GEOMETRY", av::gua::SkeletalAnimationLoader::OPTIMIZE_GEOMETRY)
        .value("MAKE_PICKABLE", av::gua::SkeletalAnimationLoader::MAKE_PICKABLE)
        .value("NORMALIZE_SCALE", av::gua::SkeletalAnimationLoader::NORMALIZE_SCALE)
        .value("NORMALIZE_POSITION", av::gua::SkeletalAnimationLoader::NORMALIZE_POSITION)
        ;

  register_field<av::gua::SFSkeletalAnimationLoader>("SFSkeletalAnimationLoader");
  register_multifield<av::gua::MFSkeletalAnimationLoader>("MFSkeletalAnimationLoader");
}
