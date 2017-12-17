#include "LineStripLoader.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/LineStripLoader.hpp>

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

av::Link<av::gua::Node> createGeometryFromFile1(
                                          av::gua::LineStripLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& fileName,
                                          av::gua::Material const& fallbackMaterial,
                                          int flags) {

   return loader.createGeometryFromFile(nodeName, fileName, fallbackMaterial,
                                static_cast<av::gua::LineStripLoader::Flags>(flags));
}

av::Link<av::gua::Node> createGeometryFromFile2(
                                          av::gua::LineStripLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& fileName,
                                          int flags) {

   return loader.createGeometryFromFile(nodeName, fileName,
                                static_cast<av::gua::LineStripLoader::Flags>(flags));
}

av::Link<av::gua::Node> createGeometryFromFile3(
                                          av::gua::LineStripLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& fileName,
                                          av::gua::Material const& fallbackMaterial) {

   return loader.createGeometryFromFile(nodeName, fileName, fallbackMaterial,
                                static_cast<av::gua::LineStripLoader::Flags>(0));
}

av::Link<av::gua::Node> createGeometryFromFile4(
                                          av::gua::LineStripLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& fileName) {

   return loader.createGeometryFromFile(nodeName, fileName,
                                static_cast<av::gua::LineStripLoader::Flags>(0));
}

av::Link<av::gua::Node> createEmptyGeometry1(
                                          av::gua::LineStripLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& emptyName,
                                          av::gua::Material const& fallbackMaterial) {

   return loader.createEmptyGeometry(nodeName, emptyName, fallbackMaterial,
                                     static_cast<av::gua::LineStripLoader::Flags>(0));
}

av::Link<av::gua::Node> createEmptyGeometry2(
                                          av::gua::LineStripLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& emptyName) {

   return loader.createEmptyGeometry(nodeName, emptyName,
                                     static_cast<av::gua::LineStripLoader::Flags>(0));
}

void init_LineStripLoader()
{
  register_ptr_to_python<av::Link<av::gua::LineStripLoader> >();

  class_<av::gua::LineStripLoader,
         av::Link<av::gua::LineStripLoader>,
         bases<av::FieldContainer>, boost::noncopyable> ("LineStripLoader", "docstring", no_init)
         .def("create_geometry_from_file", &createGeometryFromFile1)
         .def("create_geometry_from_file", &createGeometryFromFile2)
         .def("create_geometry_from_file", &createGeometryFromFile3)
         .def("create_geometry_from_file", &createGeometryFromFile4)
         .def("create_empty_geometry", &createEmptyGeometry1)
         .def("create_empty_geometry", &createEmptyGeometry2)
         ;

  enum_<av::gua::LineStripLoader::Flags>("LineStripLoaderFlags")
        .value("DEFAULTS", av::gua::LineStripLoader::DEFAULTS)
        .value("MAKE_PICKABLE", av::gua::LineStripLoader::MAKE_PICKABLE)
        .value("NORMALIZE_SCALE", av::gua::LineStripLoader::NORMALIZE_SCALE)
        .value("NORMALIZE_POSITION", av::gua::LineStripLoader::NORMALIZE_POSITION)
        .value("NO_SHARED_MATERIALS", av::gua::LineStripLoader::NO_SHARED_MATERIALS)
        ;

  register_field<av::gua::SFLineStripLoader>("SFLineStripLoader");
  register_multifield<av::gua::MFLineStripLoader>("MFLineStripLoader");
}
