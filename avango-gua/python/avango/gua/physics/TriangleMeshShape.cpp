#include "TriangleMeshShape.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/TriangleMeshShape.hpp>

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

av::Link<av::gua::TriangleMeshShape>
createTriangleMeshShapeFromGeometry (
      std::vector<std::string> const& geometry_list,
      bool build_static,
      bool build_dynamic) {

  return av::Link<av::gua::TriangleMeshShape> (
    new av::gua::TriangleMeshShape( ::gua::physics::TriangleMeshShape::FromGeometry(
        geometry_list, build_static, build_dynamic
      ))
    );
}

av::Link<av::gua::TriangleMeshShape>
createTriangleMeshShapeFromGeometryFile (std::string const& file_name,
                                         bool build_static,
                                         bool build_dynamic,
                                         unsigned flags =
                                          ::gua::TriMeshLoader::DEFAULTS) {

  return av::Link<av::gua::TriangleMeshShape> (
    new av::gua::TriangleMeshShape( ::gua::physics::TriangleMeshShape::FromGeometryFile(
        file_name, build_static, build_dynamic, flags
      ))
    );
}

void init_TriangleMeshShape()
{
  register_field<av::gua::SFTriangleMeshShape>("SFTriangleMeshShape");
  register_multifield<av::gua::MFTriangleMeshShape>("MFTriangleMeshShape");

  class_<av::gua::TriangleMeshShape,
         av::Link<av::gua::TriangleMeshShape>,
         bases<av::gua::CollisionShape>, boost::noncopyable >("TriangleMeshShape",
                                                              "docstring",
                                                              no_init);
  def("create_triangle_mesh_shape_from_geometry", &createTriangleMeshShapeFromGeometry);
  def("create_triangle_mesh_shape_from_geometry_file", &createTriangleMeshShapeFromGeometryFile);
}
