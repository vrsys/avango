#include "TriMeshLoader.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/TriMeshLoader.hpp>

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

av::Link<av::gua::Node> createGeometryFromFile1(
                                          av::gua::TriMeshLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& fileName,
                                          av::gua::Material const& fallbackMaterial,
                                          int flags) {

   return loader.createGeometryFromFile(nodeName, fileName, fallbackMaterial,
                                static_cast<av::gua::TriMeshLoader::Flags>(flags));
}

av::Link<av::gua::Node> createGeometryFromFile2(
                                          av::gua::TriMeshLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& fileName,
                                          int flags) {

   return loader.createGeometryFromFile(nodeName, fileName,
                                static_cast<av::gua::TriMeshLoader::Flags>(flags));
}

av::Link<av::gua::Node> createGeometryFromFile3(
                                          av::gua::TriMeshLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& fileName,
                                          av::gua::Material const& fallbackMaterial) {

   return loader.createGeometryFromFile(nodeName, fileName, fallbackMaterial,
                                static_cast<av::gua::TriMeshLoader::Flags>(0));
}

av::Link<av::gua::Node> createGeometryFromFile4(
                                          av::gua::TriMeshLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& fileName) {

   return loader.createGeometryFromFile(nodeName, fileName,
                                static_cast<av::gua::TriMeshLoader::Flags>(0));
}

void init_TriMeshLoader()
{
  class_<av::gua::TriMeshLoader,
         av::Link<av::gua::TriMeshLoader>,
         bases<av::FieldContainer>, boost::noncopyable> ("TriMeshLoader", "docstring", no_init)
         .def("create_geometry_from_file", &createGeometryFromFile1)
         .def("create_geometry_from_file", &createGeometryFromFile2)
         .def("create_geometry_from_file", &createGeometryFromFile3)
         .def("create_geometry_from_file", &createGeometryFromFile4)
         ;

  enum_<av::gua::TriMeshLoader::Flags>("LoaderFlags")
        .value("DEFAULTS", av::gua::TriMeshLoader::DEFAULTS)
        .value("LOAD_MATERIALS", av::gua::TriMeshLoader::LOAD_MATERIALS)
        .value("OPTIMIZE_GEOMETRY", av::gua::TriMeshLoader::OPTIMIZE_GEOMETRY)
        .value("MAKE_PICKABLE", av::gua::TriMeshLoader::MAKE_PICKABLE)
        .value("NORMALIZE_SCALE", av::gua::TriMeshLoader::NORMALIZE_SCALE)
        .value("NORMALIZE_POSITION", av::gua::TriMeshLoader::NORMALIZE_POSITION)
        .value("NO_SHARED_MATERIALS", av::gua::TriMeshLoader::NO_SHARED_MATERIALS)
        .value("OPTIMIZE_MATERIALS", av::gua::TriMeshLoader::OPTIMIZE_MATERIALS)
        ;

  register_field<av::gua::SFTriMeshLoader>("SFTriMeshLoader");
  register_multifield<av::gua::MFTriMeshLoader>("MFTriMeshLoader");
}
