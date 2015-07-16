#include "PhysicsDatabase.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/BoxShape.hpp>
#include <avango/gua/physics/CylinderShape.hpp>
#include <avango/gua/physics/PlaneShape.hpp>
#include <avango/gua/physics/SphereShape.hpp>
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
      using type = T;
     };
   }
 }

void
addCollisionShape(std::string const& name,
                  av::gua::CollisionShape* shape) {

  ::gua::physics::CollisionShapeDatabase::add_shape(name,
                                                    shape->getGuaShape());
}

av::Link<av::gua::BoxShape>
createBoxShape(std::string const& shape_name,
               ::gua::math::vec3 const& half_extents) {

  auto shape(new av::gua::BoxShape(
    new ::gua::physics::BoxShape(half_extents)
    ));

  addCollisionShape(shape_name, shape);

  return av::Link<av::gua::BoxShape>(shape);

}

av::Link<av::gua::CylinderShape>
createCylinderShape(std::string const& shape_name,
                    ::gua::math::vec3 const& half_extents) {

  auto shape(new av::gua::CylinderShape(
    new ::gua::physics::CylinderShape(half_extents)
    ));

  addCollisionShape(shape_name, shape);

  return av::Link<av::gua::CylinderShape>(shape);

}

av::Link<av::gua::PlaneShape>
createPlaneShape(std::string const& shape_name,
                 ::gua::math::vec3 const& normal,
                 float plane_constant) {

  auto shape(new av::gua::PlaneShape(
    new ::gua::physics::PlaneShape(normal[0],
                                   normal[1],
                                   normal[2],
                                   plane_constant)
    ));

  addCollisionShape(shape_name, shape);

  return av::Link<av::gua::PlaneShape>(shape);

}

av::Link<av::gua::SphereShape>
createSphereShape(std::string const& shape_name,
                  float radius) {

  auto shape(new av::gua::SphereShape(
    new ::gua::physics::SphereShape(radius)
    ));

  // addCollisionShape(shape_name, shape);

  ::gua::physics::CollisionShapeDatabase::add_shape(shape_name,
                                                    shape->getGuaShape());

  return av::Link<av::gua::SphereShape>(shape);

}

av::Link<av::gua::TriangleMeshShape>
createTriangleMeshShapeFromGeometryFile(std::string const& shape_name,
                                        std::string const& file_name,
                                        bool build_static,
                                        bool build_dynamic,
                                        int loader_flags) {

  auto shape(new av::gua::TriangleMeshShape(
    ::gua::physics::TriangleMeshShape::FromGeometryFile(
      file_name,
      build_static,
      build_dynamic,
      static_cast< ::gua::TriMeshLoader::Flags>(loader_flags)
    )));

  addCollisionShape(shape_name, shape);

  return av::Link<av::gua::TriangleMeshShape>(shape);

}

av::Link<av::gua::TriangleMeshShape>
createTriangleMeshShapeFromGeometry(std::string const& shape_name,
                                    boost::python::list const& geometry_list,
                                    bool build_static,
                                    bool build_dynamic,
                                    int loader_flags) {


  std::vector<std::string> geo_list;

  boost::python::ssize_t n = boost::python::len(geometry_list);
  for(boost::python::ssize_t i(0); i < n; ++i) {
    boost::python::object elem = geometry_list[i];
    std::string geometry = boost::python::extract<std::string>(elem);
    geo_list.push_back(geometry);
  }

  auto shape(new av::gua::TriangleMeshShape(
    ::gua::physics::TriangleMeshShape::FromGeometry(
      geo_list,
      build_static,
      build_dynamic
  )));

  addCollisionShape(shape_name, shape);

  return av::Link<av::gua::TriangleMeshShape>(shape);

}



void init_PhysicsDatabase()
{
  def("create_box_shape",
      &createBoxShape);

  def("create_cylinder_shape",
      &createCylinderShape);

  def("create_plane_shape",
      &createPlaneShape);

  def("create_sphere_shape",
      &createSphereShape);

  def("create_triangle_mesh_shape_from_geometry_file",
      &createTriangleMeshShapeFromGeometryFile);

  def("create_triangle_mesh_shape_from_geometry",
      &createTriangleMeshShapeFromGeometry);

}
