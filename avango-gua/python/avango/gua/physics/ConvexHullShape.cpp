#include "ConvexHullShape.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/ConvexHullShape.hpp>

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

av::Link<av::gua::ConvexHullShape>
createConvexHullShapeFromGeometry (
      std::vector<std::string> const& geometry_list,
      bool compensate_collision_margin = true) {

  return av::Link<av::gua::ConvexHullShape> (
    new av::gua::ConvexHullShape( ::gua::physics::ConvexHullShape::FromGeometry(
        geometry_list, compensate_collision_margin
      ))
    );
}

av::Link<av::gua::ConvexHullShape>
createConvexHullShapeFromGeometryFile (std::string const& file_name,
                                         bool compensate_collision_margin = true,
                                         unsigned flags =
                                          ::gua::TriMeshLoader::DEFAULTS) {

  return av::Link<av::gua::ConvexHullShape> (
    new av::gua::ConvexHullShape( ::gua::physics::ConvexHullShape::FromGeometryFile(
        file_name, compensate_collision_margin, flags
      ))
    );
}

void init_ConvexHullShape()
{
  register_field<av::gua::SFConvexHullShape>("SFConvexHullShape");
  register_multifield<av::gua::MFConvexHullShape>("MFConvexHullShape");

  class_<av::gua::ConvexHullShape,
         av::Link<av::gua::ConvexHullShape>,
         bases<av::gua::CollisionShape>, boost::noncopyable >("ConvexHullShape",
                                                              "docstring",
                                                              no_init)
         .def("add_point", &::gua::physics::ConvexHullShape::add_point)
         ;

  def("create_convex_hull_shape_from_geometry", &createConvexHullShapeFromGeometry);
  def("create_convex_hull_shape_from_geometry_file", &createConvexHullShapeFromGeometryFile);
}
