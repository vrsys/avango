#include "DynamicTriangleLoader.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/DynamicTriangleLoader.hpp>

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

av::Link<av::gua::Node> createEmptyGeometry1(
                                          av::gua::DynamicTriangleLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& emptyName,
                                          av::gua::Material const& fallbackMaterial) {

   return loader.createEmptyGeometry(nodeName, emptyName, fallbackMaterial,
                                     static_cast<av::gua::DynamicTriangleLoader::Flags>(0));
}

av::Link<av::gua::Node> createEmptyGeometry2(
                                          av::gua::DynamicTriangleLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& emptyName) {
   // return loader.createEmptyGeometry(nodeName, emptyName,
   //                                   static_cast<av::gua::DynamicTriangleLoader::Flags>(0));
   auto node = loader.createEmptyGeometry(nodeName, emptyName,
                                     static_cast<av::gua::DynamicTriangleLoader::Flags>(0));

   return node;
}

av::Link<av::gua::Node> createEmptyGeometry3(
                                          av::gua::DynamicTriangleLoader const& loader,
                                          std::string const& nodeName,
                                          std::string const& emptyName, 
                                          int flags) {
   // return loader.createEmptyGeometry(nodeName, emptyName,
   //                                   static_cast<av::gua::DynamicTriangleLoader::Flags>(0));
   auto node = loader.createEmptyGeometry(nodeName, emptyName,
                                     static_cast<av::gua::DynamicTriangleLoader::Flags>(flags));

   return node;
}

void init_DynamicTriangleLoader()
{
  register_ptr_to_python<av::Link<av::gua::DynamicTriangleLoader> >();

  class_<av::gua::DynamicTriangleLoader,
         av::Link<av::gua::DynamicTriangleLoader>,
         bases<av::FieldContainer>, boost::noncopyable> ("DynamicTriangleLoader", "docstring", no_init)
         .def("create_empty_geometry", &createEmptyGeometry1)
         .def("create_empty_geometry", &createEmptyGeometry2)
         .def("create_empty_geometry", &createEmptyGeometry3)
         ;

  enum_<av::gua::DynamicTriangleLoader::Flags>("DynamicTriangleLoaderFlags")
        .value("DEFAULTS", av::gua::DynamicTriangleLoader::DEFAULTS)
        .value("MAKE_PICKABLE", av::gua::DynamicTriangleLoader::MAKE_PICKABLE)
        .value("NORMALIZE_SCALE", av::gua::DynamicTriangleLoader::NORMALIZE_SCALE)
        .value("NORMALIZE_POSITION", av::gua::DynamicTriangleLoader::NORMALIZE_POSITION)
        .value("NO_SHARED_MATERIALS", av::gua::DynamicTriangleLoader::NO_SHARED_MATERIALS)
        ;

  register_field<av::gua::SFDynamicTriangleLoader>("SFDynamicTriangleLoader");
  register_multifield<av::gua::MFDynamicTriangleLoader>("MFDynamicTriangleLoader");
}
