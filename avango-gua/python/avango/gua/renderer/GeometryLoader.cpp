#include "GeometryLoader.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/GeometryLoader.hpp>

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
                                          av::gua::GeometryLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& fileName,
                                          std::string const& fallbackMaterial,
                                          int flags) {

   return loader.createGeometryFromFile(nodeName, fileName, fallbackMaterial,
                                static_cast<av::gua::GeometryLoader::Flags>(flags));
}

av::Link<av::gua::Node> createVVolumeFromFile(
                                          av::gua::GeometryLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& vfileName,
                                          int flags,
                                          std::size_t vol_hdd_cache_size,
                                          std::size_t vol_gpu_cache_size
                                          ) {

   return loader.createVVolumeFromFile(nodeName, vfileName,
                                static_cast<av::gua::GeometryLoader::Flags>(flags),
                                vol_hdd_cache_size, vol_gpu_cache_size
                                );
}


void init_GeometryLoader()
{
  class_<av::gua::GeometryLoader,
         av::Link<av::gua::GeometryLoader>,
         bases<av::FieldContainer>, boost::noncopyable> ("GeometryLoader", "docstring", no_init)
         .def("create_geometry_from_file", &createGeometryFromFile)
         .def("create_vvolume_from_file", &createVVolumeFromFile)
         ;

  enum_<av::gua::GeometryLoader::Flags>("LoaderFlags")
        .value("DEFAULTS", av::gua::GeometryLoader::DEFAULTS)
        .value("LOAD_MATERIALS", av::gua::GeometryLoader::LOAD_MATERIALS)
        .value("OPTIMIZE_GEOMETRY", av::gua::GeometryLoader::OPTIMIZE_GEOMETRY)
        .value("MAKE_PICKABLE", av::gua::GeometryLoader::MAKE_PICKABLE)
        .value("NORMALIZE_POSITION", av::gua::GeometryLoader::NORMALIZE_POSITION)
        .value("NORMALIZE_SCALE", av::gua::GeometryLoader::NORMALIZE_SCALE)
        ;

  register_field<av::gua::SFGeometryLoader>("SFGeometryLoader");
  register_multifield<av::gua::MFGeometryLoader>("MFGeometryLoader");

}
