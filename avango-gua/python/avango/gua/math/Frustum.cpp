#include "Frustum.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/math/Frustum.hpp>
#include <avango/gua/scenegraph/PickResult.hpp>
#include <avango/gua/scenegraph/RayNode.hpp>
#include <avango/gua/utils/Ray.hpp>
#include <avango/gua/Types.hpp>

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

av::gua::Frustum makeOthoFrustum(::gua::math::mat4 const& camera_transform,
                                 ::gua::math::mat4 const& screen_transform,
                                 float near_plane,
                                 float far_plane) {

  auto frustum(::gua::Frustum::orthographic(camera_transform, screen_transform, near_plane, far_plane));
  av::gua::Frustum result(new ::gua::Frustum(frustum));
  return result;
}

av::gua::Frustum makePerspectiveFrustum(::gua::math::mat4 const& camera_transform,
                                 ::gua::math::mat4 const& screen_transform,
                                 float near_plane,
                                 float far_plane) {

  auto frustum(::gua::Frustum::perspective(camera_transform, screen_transform, near_plane, far_plane));
  av::gua::Frustum result(new ::gua::Frustum(frustum));
  return result;
}

bool contains(av::gua::Frustum const& frustum, ::gua::math::vec3 const& point) {
  return frustum.getGuaFrustum()->contains(point);
}

bool intersects(av::gua::Frustum const& frustum, av::gua::BoundingBox const& bbox) {
  return frustum.getGuaFrustum()->intersects(*bbox.getGuaBoundingBox());
}

av::gua::MFPickResult* rayTest1(av::gua::Frustum const& frustum,
                                av::gua::Ray const& ray,
                                int options) {

  auto gua_results(frustum.getGuaFrustum()->ray_test(
                                    *(ray.getGuaRay()),
                                    static_cast< ::gua::PickResult::Options>(options)));
  auto results(new av::gua::MFPickResult());
  for (auto result : gua_results) {
    results->add1Value(new av::gua::PickResult(result));
  }

  return results;
}

av::gua::MFPickResult* rayTest2(av::gua::Frustum const& frustum,
                                av::gua::RayNode const& ray,
                                int options) {

  auto gua_results(frustum.getGuaFrustum()->ray_test(
                                    *(ray.getGuaNode()),
                                    static_cast< ::gua::PickResult::Options>(options)));
  auto results(new av::gua::MFPickResult());
  for (auto result : gua_results) {
    results->add1Value(new av::gua::PickResult(result));
  }

  return results;
}

av::gua::MFPickResult* rayTest3(av::gua::Frustum const& frustum,
                                av::gua::Ray const& ray) {

  return rayTest1(frustum, ray, 0);
}

av::gua::MFPickResult* rayTest4(av::gua::Frustum const& frustum,
                                av::gua::RayNode const& ray) {

  return rayTest2(frustum, ray, 0);
}

void init_Frustum()
{
  register_field<av::gua::SFFrustum>("SFFrustum");
  register_multifield<av::gua::MFFrustum>("MFFrustum");

  class_<av::gua::Frustum,
         av::Link<av::gua::Frustum>,
         bases<av::FieldContainer>, boost::noncopyable >("Frustum",
                                                         "docstring",
                                                         no_init)
         .def("contains", &contains)
         .def("intersects", &intersects)
         .def("ray_test", &rayTest1, return_value_policy<manage_new_object>())
         .def("ray_test", &rayTest2, return_value_policy<manage_new_object>())
         .def("ray_test", &rayTest3, return_value_policy<manage_new_object>())
         .def("ray_test", &rayTest4, return_value_policy<manage_new_object>())
  ;

  def("make_ortho_frustum", makeOthoFrustum);
  def("make_perspective_frustum", makePerspectiveFrustum);
}
