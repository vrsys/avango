#include "PhysicsDatabase.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/TriangleMeshShape.hpp>
#include <gua/guacamole.hpp>

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

av::gua::SFTriangleMeshShape*
createTriangleMeshShapeFromGeometryFile(std::string const& file_name,
                                        bool build_static,
                                        bool build_dynamic,
                                        int loader_flags) {

  auto field(new av::gua::SFTriangleMeshShape());

  field->setValue( new av::gua::TriangleMeshShape(
    ::gua::physics::TriangleMeshShape::FromGeometryFile(
      file_name,
      build_static,
      build_dynamic,
      static_cast< ::gua::GeometryLoader::Flags>(loader_flags)
    )));

  return field;
}

// av::gua::SFTriangleMeshShape*
// createTriangleMeshShapeFromGeometry(std::string const& file_name,
//                                     bool build_static,
//                                     bool build_dynamic,
//                                     int loader_flags) {

//   auto field(new av::gua::SFTriangleMeshShape());

//   field->setValue( new av::gua::TriangleMeshShape(
//     ::gua::physics::TriangleMeshShape::FromGeometryFile(
//       file_name,
//       build_static,
//       build_dynamic,
//       static_cast< ::gua::GeometryLoader::Flags>(loader_flags)
//     )));

//   return field;
// }

void string_test(boost::python::list const& v) {
  std::cout << "==============" << std::endl;
  // for (auto s : v) {
  //   std::cout << s << std::endl;
  // }
}

void
addCollisionShape(std::string const& name,
                  av::gua::SFCollisionShape const& shape) {
  ::gua::physics::CollisionShapeDatabase::add_shape(name,
                                                    shape.getValue()->getGuaShape());
}

void init_PhysicsDatabase()
{
  // def("add_collision_shape", &addCollisionShape);
  // def("create_triangle_mesh_shape_from_geometry_file",
  //     &createTriangleMeshShapeFromGeometryFile,
  //     return_value_policy<manage_new_object>());
  def("string_test", &string_test);

}
