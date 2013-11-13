#include "math.hpp"

#include <boost/format.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/Fields.hpp>
#include <avango/gua/Types.hpp>

#include <gua/math/math.hpp>

#include <array>

using namespace boost::python;
using namespace av::python;

void init_Vec2() {
  // wrapping gua::math::vec2 functionality
  class_< ::gua::math::vec2>("Vec2", no_init)
    .def("__init__", make_constructor(&constructorVec< ::gua::math::vec2>))
    .def(init< ::gua::math::vec2::value_type, ::gua::math::vec2::value_type>())
    .def(init< ::gua::math::vec2>())
    .add_property("x", &getX< ::gua::math::vec2>, &setX< ::gua::math::vec2>)
    .add_property("y", &getY< ::gua::math::vec2>, &setY< ::gua::math::vec2>)
    .def("normalize", &normalize< ::gua::math::vec2>)
    .def("length", &length< ::gua::math::vec2>)
    .def("length2", &length2< ::gua::math::vec2>)
    .def("distance_to", &distanceTo< ::gua::math::vec2>)
    .def("lerp_to", &lerpTo< ::gua::math::vec2>)
    .def("__repr__", &toString< ::gua::math::vec2>)
    .def("__getitem__", &getVecElement< ::gua::math::vec2>)
  ;

  // register as a field
  register_field<av::gua::SFVec2>("SFVec2");
  register_multifield<av::gua::MFVec2>("MFVec2");
}

void init_Vec2ui() {
  // wrapping gua::math::vec2ui functionality
  class_< ::gua::math::vec2ui>("Vec2ui", no_init)
    .def("__init__", make_constructor(&constructorVec< ::gua::math::vec2ui>))
    .def(init< unsigned, unsigned>())
    .def(init< ::gua::math::vec2ui>())
    .add_property("x", &getX< ::gua::math::vec2ui>, &setX< ::gua::math::vec2ui>)
    .add_property("y", &getY< ::gua::math::vec2ui>, &setY< ::gua::math::vec2ui>)
    .def("normalize", &normalize< ::gua::math::vec2ui>)
    .def("length", &length< ::gua::math::vec2ui>)
    .def("length2", &length2< ::gua::math::vec2ui>)
    .def("distance_to", &distanceTo< ::gua::math::vec2ui>)
    .def("lerp_to", &lerpTo< ::gua::math::vec2ui>)
    .def("__repr__", &toString< ::gua::math::vec2ui>)
    .def("__getitem__", &getVecElement< ::gua::math::vec2ui>)
  ;

  // register as a field
  register_field<av::gua::SFVec2ui>("SFVec2ui");
  register_multifield<av::gua::MFVec2ui>("MFVec2ui");
}

void init_Vec3() {
  // wrapping gua::math::vec3 functionality
  class_< ::gua::math::vec3>("Vec3", no_init)
    .def("__init__", make_constructor(&constructorVec< ::gua::math::vec3>))
    .def(init< ::gua::math::vec3::value_type,
               ::gua::math::vec3::value_type,
               ::gua::math::vec3::value_type>())
    .def(init< ::gua::math::vec3>())
    .def(self *  other< ::gua::math::vec3::value_type>())
    .def(self /  other< ::gua::math::vec3::value_type>())
    .def(self *= other< ::gua::math::vec3::value_type>())
    .def(self /= other< ::gua::math::vec3::value_type>())
    .def(self +  other< ::gua::math::vec3>())
    .def(self -  other< ::gua::math::vec3>())
    .def(self += other< ::gua::math::vec3>())
    .def(self -= other< ::gua::math::vec3>())
    .add_property("x", &getX< ::gua::math::vec3>, &setX< ::gua::math::vec3>)
    .add_property("y", &getY< ::gua::math::vec3>, &setY< ::gua::math::vec3>)
    .add_property("z", &getZ< ::gua::math::vec3>, &setZ< ::gua::math::vec3>)
    .def("normalize", &normalize< ::gua::math::vec3>)
    .def("length", &length< ::gua::math::vec3>)
    .def("length2", &length2< ::gua::math::vec3>)
    .def("distance_to", &distanceTo< ::gua::math::vec3>)
    .def("lerp_to", &lerpTo< ::gua::math::vec3>)
    .def("__repr__", &toString< ::gua::math::vec3>)
    .def("__getitem__", &getVecElement< ::gua::math::vec3>)
  ;

  // register as a field
  register_field<av::gua::SFVec3>("SFVec3");
  register_multifield<av::gua::MFVec3>("MFVec3");
}

void init_Vec4()
{
  // wrapping gua::math::vec4 functionality
  class_< ::gua::math::vec4>("Vec4", no_init)
    .def("__init__", make_constructor(&constructorVec< ::gua::math::vec4>))
    .def(init< ::gua::math::vec4::value_type,
               ::gua::math::vec4::value_type,
               ::gua::math::vec4::value_type,
               ::gua::math::vec4::value_type>())
    .def(init< ::gua::math::vec4>())
    .def(self *  other< ::gua::math::vec4::value_type>())
    .def(self /  other< ::gua::math::vec4::value_type>())
    .def(self *= other< ::gua::math::vec4::value_type>())
    .def(self /= other< ::gua::math::vec4::value_type>())
    .def(self +   other< ::gua::math::vec4::value_type>())
    .def(self -   other< ::gua::math::vec4::value_type>())
    .def(self +=  other< ::gua::math::vec4::value_type>())
    .def(self -=  other< ::gua::math::vec4::value_type>())
    .add_property("x", &getX< ::gua::math::vec4>, &setX< ::gua::math::vec4>)
    .add_property("y", &getY< ::gua::math::vec4>, &setY< ::gua::math::vec4>)
    .add_property("z", &getZ< ::gua::math::vec4>, &setZ< ::gua::math::vec4>)
    .add_property("w", &getW< ::gua::math::vec4>, &setW< ::gua::math::vec4>)
    .def("normalize", &normalize< ::gua::math::vec4>)
    .def("length", &length< ::gua::math::vec4>)
    .def("length2", &length2< ::gua::math::vec4>)
    .def("distance_to", &distanceTo< ::gua::math::vec4>)
    .def("lerp_to", &lerpTo< ::gua::math::vec4>)
    .def("__repr__", &toString< ::gua::math::vec4>)
    .def("__getitem__", &getVecElement< ::gua::math::vec4>)
  ;

  // register as a field
  register_field<av::gua::SFVec4>("SFVec4");
  register_multifield<av::gua::MFVec4>("MFVec4");
}



void init_Mat3()
{
  // wrapping gua::math::mat3 functionality
  class_< ::gua::math::mat3>("Mat3", no_init)
    .def("__init__", make_constructor(&constructorMat< ::gua::math::mat3>))
    .def(init< ::gua::math::mat3>())
    .def("__repr__", &toString< ::gua::math::mat3>)
  ;

  // register as a field
  register_field<av::gua::SFMatrix3>("SFMatrix3");
  register_multifield<av::gua::MFMatrix3>("MFMatrix3");
}

::gua::math::mat4::value_type getMatElement(::gua::math::mat4 const& mat,
                                            int row,
                                            int col) {
  return mat[row + col * 4];
}

void setElement(::gua::math::mat4& mat,
                int row,
                int col,
                ::gua::math::mat4::value_type val) {
  mat[row + col * 4] = val;
}

::gua::math::vec3 const getTranslate(::gua::math::mat4 const& mat) {
  ::gua::math::vec3 trans;
  trans[0] = mat[12];
  trans[1] = mat[13];
  trans[2] = mat[14];
  return trans;
}

void setTranslate1(::gua::math::mat4& mat,
                   ::gua::math::vec3 const& vec) {

  mat[12] = vec[0];
  mat[13] = vec[1];
  mat[14] = vec[2];
}

void setTranslate2(::gua::math::mat4& mat,
                   ::gua::math::mat4::value_type x,
                   ::gua::math::mat4::value_type y,
                   ::gua::math::mat4::value_type z) {
  mat[12] = x;
  mat[13] = y;
  mat[14] = z;
}

void init_Mat4()
{
  void (*translate1)( ::gua::math::mat4&, ::gua::math::vec3 const&) =
      &scm::math::translate;

  void (*translate2)( ::gua::math::mat4&,
                      ::gua::math::mat4::value_type,
                      ::gua::math::mat4::value_type,
                      ::gua::math::mat4::value_type) =
      &scm::math::translate;

  void (*rotate1)( ::gua::math::mat4&,
                   ::gua::math::mat4::value_type,
                   ::gua::math::vec3 const&) =
      &scm::math::rotate;

  void (*rotate2)( ::gua::math::mat4&,
                   ::gua::math::mat4::value_type,
                   ::gua::math::mat4::value_type,
                   ::gua::math::mat4::value_type,
                   ::gua::math::mat4::value_type) =
      &scm::math::rotate;

  void (*scale1)( ::gua::math::mat4&,
                  ::gua::math::vec3 const&) =
      &scm::math::scale;

  void (*scale2)( ::gua::math::mat4&,
                  ::gua::math::mat4::value_type,
                  ::gua::math::mat4::value_type,
                  ::gua::math::mat4::value_type) =
      &scm::math::scale;

  ::gua::math::mat4 const (*inverse)( ::gua::math::mat4 const&) =
      &scm::math::inverse;

  // wrapping gua::math::mat4 functionality
  class_< ::gua::math::mat4>("Mat4", no_init)
    .def("__init__", make_constructor(&constructorMat< ::gua::math::mat4>))
    .def(init< ::gua::math::mat4>())
    .def(self == other< ::gua::math::mat4>())
    .def(self != other< ::gua::math::mat4>())
    .def(self *= other< ::gua::math::mat4>())
    .def(self * other< ::gua::math::mat4>())
    .def(self * other< ::gua::math::vec3>())
    .def(self * other< ::gua::math::vec4>())
    .def(self * other< ::gua::math::mat4::value_type>())
    .def(self + other< ::gua::math::mat4>())
    .def(other< ::gua::math::vec4>() * self)
    .def("translate", translate1)
    .def("translate", translate2)
    .def("rotate", rotate1)
    .def("rotate", rotate2)
    .def("scale", scale1)
    .def("scale", scale2)
    .def("get_element", getMatElement)
    .def("set_element", setElement)
    .def("get_translate", getTranslate)
    .def("set_translate", setTranslate1)
    .def("set_translate", setTranslate2)
    .def("__repr__", &toString< ::gua::math::mat4>)
  ;

  // register as a field
  register_field<av::gua::SFMatrix4>("SFMatrix4");
  register_multifield<av::gua::MFMatrix4>("MFMatrix4");

  // global methods
  ::gua::math::mat4 const (*makeTrans1)(::gua::math::vec3 const&) =
      &scm::math::make_translation;
  ::gua::math::mat4 const (*makeTrans2)(::gua::math::mat4::value_type,
                                        ::gua::math::mat4::value_type,
                                        ::gua::math::mat4::value_type) =
      &scm::math::make_translation;

  ::gua::math::mat4 const (*makeRot1)(::gua::math::mat4::value_type,
                                      ::gua::math::vec3 const&) =
      &scm::math::make_rotation;

  ::gua::math::mat4 const (*makeRot2)(::gua::math::mat4::value_type,
                                      ::gua::math::mat4::value_type,
                                      ::gua::math::mat4::value_type,
                                      ::gua::math::mat4::value_type) =
      &scm::math::make_rotation;

  ::gua::math::mat4 const (*makeScale1)(::gua::math::vec3 const&) =
      &scm::math::make_scale;

  ::gua::math::mat4 const (*makeScale2)(::gua::math::mat4::value_type,
                                        ::gua::math::mat4::value_type,
                                        ::gua::math::mat4::value_type) =
      &scm::math::make_scale;

  def("make_identity_mat",
     & ::gua::math::mat4::identity,
     return_value_policy<copy_const_reference>());

  def("make_trans_mat", makeTrans1);
  def("make_trans_mat", makeTrans2);

  def("make_rot_mat", makeRot1);
  def("make_rot_mat", makeRot2);

  def("make_scale_mat", makeScale1);
  def("make_scale_mat", makeScale2);

  def("make_inverse_mat", inverse);
}

}
