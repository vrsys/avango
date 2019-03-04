// #include "NURBSLoader.hpp"

// #include <boost/python.hpp>
// #include <avango/python/register_field.h>
// #include <avango/gua/renderer/NURBSLoader.hpp>

// using namespace boost::python;
// using namespace av::python;

// namespace boost
//  {
//   namespace python
//    {
//     template <class T> struct pointee<av::Link<T> >
//      {
//       typedef T type;
//      };
//    }
//  }

// av::Link<av::gua::Node> createGeometryFromFile(
//                                         av::gua::NURBSLoader const& loader,
//                                         std::string const& nodeName,
//                                         std::string const& fileName,
//                                         std::string const& fallbackMaterial,
//                                         int flags) {
//    return loader.createGeometryFromFile(nodeName, fileName, fallbackMaterial,
//                               static_cast<av::gua::NURBSLoader::Flags>(flags));
// }

// void init_NURBSLoader()
// {
//   class_<av::gua::NURBSLoader,
//          av::Link<av::gua::NURBSLoader>,
//          bases<av::FieldContainer>, boost::noncopyable> ("NURBSLoader", "docstring", no_init)
//          .def("create_geometry_from_file", &createGeometryFromFile)
//          ;

//   enum_<av::gua::NURBSLoader::Flags>("LoaderFlags")
//         .value("DEFAULTS", av::gua::NURBSLoader::DEFAULTS)
//         .value("LOAD_MATERIALS", av::gua::NURBSLoader::LOAD_MATERIALS)
//         .value("OPTIMIZE_GEOMETRY", av::gua::NURBSLoader::OPTIMIZE_GEOMETRY)
//         .value("MAKE_PICKABLE", av::gua::NURBSLoader::MAKE_PICKABLE)
//         .value("NORMALIZE_SCALE", av::gua::NURBSLoader::NORMALIZE_SCALE)
//         .value("NORMALIZE_POSITION", av::gua::NURBSLoader::NORMALIZE_POSITION)
//         ;

//   register_field<av::gua::SFNURBSLoader>("SFNURBSLoader");
//   register_multifield<av::gua::MFNURBSLoader>("MFNURBSLoader");

// }
