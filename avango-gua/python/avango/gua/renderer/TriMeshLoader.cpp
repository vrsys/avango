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

av::Link<av::gua::Node> createGeometryFromFile(
                                          av::gua::TriMeshLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& fileName,
                                          av::Link<av::gua::Material> const& fallbackMaterial,
                                          int flags) {

   return loader.createGeometryFromFile(nodeName, fileName, fallbackMaterial,
                                static_cast<av::gua::TriMeshLoader::Flags>(flags));
}

void init_TriMeshLoader()
{
  class_<av::gua::TriMeshLoader,
         av::Link<av::gua::TriMeshLoader>,
         bases<av::FieldContainer>, boost::noncopyable> ("TriMeshLoader", "docstring", no_init)
         .def("create_geometry_from_file", &createGeometryFromFile)
         ;

  enum_<av::gua::TriMeshLoader::Flags>("LoaderFlags")
        .value("DEFAULTS", av::gua::TriMeshLoader::DEFAULTS)
        .value("LOAD_MATERIALS", av::gua::TriMeshLoader::LOAD_MATERIALS)
        .value("OPTIMIZE_GEOMETRY", av::gua::TriMeshLoader::OPTIMIZE_GEOMETRY)
        .value("MAKE_PICKABLE", av::gua::TriMeshLoader::MAKE_PICKABLE)
        .value("NORMALIZE_SCALE", av::gua::TriMeshLoader::NORMALIZE_SCALE)
        .value("NORMALIZE_POSITION", av::gua::TriMeshLoader::NORMALIZE_POSITION)
        ;

  register_field<av::gua::SFTriMeshLoader>("SFTriMeshLoader");
  register_multifield<av::gua::MFTriMeshLoader>("MFTriMeshLoader");
}
