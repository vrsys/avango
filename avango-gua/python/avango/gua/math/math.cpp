#include "math.hpp"

#include <boost/format.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/Fields.hpp>
#include <avango/gua/Types.hpp>

#include <gua/math/math.hpp>

#include <array>

using namespace boost::python;
using namespace av::python;

#define ADD_VECTOR_OPERATORS(TYPE) \
  .def(self *  other< TYPE ::value_type>())\
  .def(self /  other< TYPE ::value_type>())\
  .def(self +  other< TYPE ::value_type>())\
  .def(self -  other< TYPE ::value_type>())\
  .def(self *= other< TYPE ::value_type>())\
  .def(self /= other< TYPE ::value_type>())\
  .def(self += other< TYPE ::value_type>())\
  .def(self -= other< TYPE ::value_type>())\
  .def(self *  other< TYPE >())\
  .def(self /  other< TYPE >())\
  .def(self +  other< TYPE >())\
  .def(self -  other< TYPE >())\
  .def(self *= other< TYPE >())\
  .def(self /= other< TYPE >())\
  .def(self += other< TYPE >())\
  .def(self -= other< TYPE >())

void init_Vec2() {
  // wrapping gua::math::vec2 functionality
  class_< ::gua::math::vec2>("Vec2", no_init)
    .def("__init__", make_constructor(&constructorVec< ::gua::math::vec2>))
    .def(init< ::gua::math::vec2::value_type, ::gua::math::vec2::value_type>())
    .def(init< ::gua::math::vec2>())
    ADD_VECTOR_OPERATORS( ::gua::math::vec2)
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

void init_Vec2i() {
  // wrapping gua::math::vec2i functionality
  class_< ::gua::math::vec2i>("Vec2i", no_init)
    .def("__init__", make_constructor(&constructorVec< ::gua::math::vec2i>))
    .def(init< unsigned, unsigned>())
    .def(init< ::gua::math::vec2i>())
    ADD_VECTOR_OPERATORS( ::gua::math::vec2i)
    .add_property("x", &getX< ::gua::math::vec2i>, &setX< ::gua::math::vec2i>)
    .add_property("y", &getY< ::gua::math::vec2i>, &setY< ::gua::math::vec2i>)
    .def("normalize", &normalize< ::gua::math::vec2i>)
    .def("length", &length< ::gua::math::vec2i>)
    .def("length2", &length2< ::gua::math::vec2i>)
    .def("distance_to", &distanceTo< ::gua::math::vec2i>)
    .def("lerp_to", &lerpTo< ::gua::math::vec2i>)
    .def("__repr__", &toString< ::gua::math::vec2i>)
    .def("__getitem__", &getVecElement< ::gua::math::vec2i>)
  ;

  // register as a field
  register_field<av::gua::SFVec2i>("SFVec2i");
  register_multifield<av::gua::MFVec2i>("MFVec2i");
}

void init_Vec2ui() {
  // wrapping gua::math::vec2ui functionality
  class_< ::gua::math::vec2ui>("Vec2ui", no_init)
    .def("__init__", make_constructor(&constructorVec< ::gua::math::vec2ui>))
    .def(init< unsigned, unsigned>())
    .def(init< ::gua::math::vec2ui>())
    ADD_VECTOR_OPERATORS( ::gua::math::vec2ui)
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
    ADD_VECTOR_OPERATORS( ::gua::math::vec3)
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

void init_Vec3i() {
  // wrapping gua::math::vec3i functionality
  class_< ::gua::math::vec3i>("Vec3i", no_init)
    .def("__init__", make_constructor(&constructorVec< ::gua::math::vec3i>))
    .def(init< ::gua::math::vec3i::value_type,
               ::gua::math::vec3i::value_type,
               ::gua::math::vec3i::value_type>())
    .def(init< ::gua::math::vec3i>())
    ADD_VECTOR_OPERATORS( ::gua::math::vec3i)
    .add_property("x", &getX< ::gua::math::vec3i>, &setX< ::gua::math::vec3i>)
    .add_property("y", &getY< ::gua::math::vec3i>, &setY< ::gua::math::vec3i>)
    .add_property("z", &getZ< ::gua::math::vec3i>, &setZ< ::gua::math::vec3i>)
    .def("normalize", &normalize< ::gua::math::vec3i>)
    .def("length", &length< ::gua::math::vec3i>)
    .def("length2", &length2< ::gua::math::vec3i>)
    .def("distance_to", &distanceTo< ::gua::math::vec3i>)
    .def("lerp_to", &lerpTo< ::gua::math::vec3i>)
    .def("__repr__", &toString< ::gua::math::vec3i>)
    .def("__getitem__", &getVecElement< ::gua::math::vec3i>)
  ;

  // register as a field
  register_field<av::gua::SFVec3i>("SFVec3i");
  register_multifield<av::gua::MFVec3i>("MFVec3i");
}

void init_Vec3ui() {
  // wrapping gua::math::vec3ui functionality
  class_< ::gua::math::vec3ui>("Vec3ui", no_init)
    .def("__init__", make_constructor(&constructorVec< ::gua::math::vec3ui>))
    .def(init< ::gua::math::vec3ui::value_type,
               ::gua::math::vec3ui::value_type,
               ::gua::math::vec3ui::value_type>())
    .def(init< ::gua::math::vec3ui>())
    ADD_VECTOR_OPERATORS( ::gua::math::vec3ui)
    .add_property("x", &getX< ::gua::math::vec3ui>, &setX< ::gua::math::vec3ui>)
    .add_property("y", &getY< ::gua::math::vec3ui>, &setY< ::gua::math::vec3ui>)
    .add_property("z", &getZ< ::gua::math::vec3ui>, &setZ< ::gua::math::vec3ui>)
    .def("normalize", &normalize< ::gua::math::vec3ui>)
    .def("length", &length< ::gua::math::vec3ui>)
    .def("length2", &length2< ::gua::math::vec3ui>)
    .def("distance_to", &distanceTo< ::gua::math::vec3ui>)
    .def("lerp_to", &lerpTo< ::gua::math::vec3ui>)
    .def("__repr__", &toString< ::gua::math::vec3ui>)
    .def("__getitem__", &getVecElement< ::gua::math::vec3ui>)
  ;

  // register as a field
  register_field<av::gua::SFVec3ui>("SFVec3ui");
  register_multifield<av::gua::MFVec3ui>("MFVec3ui");
}

void init_Vec4() {
  // wrapping gua::math::vec4 functionality
  class_< ::gua::math::vec4>("Vec4", no_init)
    .def("__init__", make_constructor(&constructorVec< ::gua::math::vec4>))
    .def(init< ::gua::math::vec4::value_type,
               ::gua::math::vec4::value_type,
               ::gua::math::vec4::value_type,
               ::gua::math::vec4::value_type>())
    .def(init< ::gua::math::vec4>())
    ADD_VECTOR_OPERATORS( ::gua::math::vec4)
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

void init_Vec4i() {
  // wrapping gua::math::vec4i functionality
  class_< ::gua::math::vec4i>("Vec4i", no_init)
    .def("__init__", make_constructor(&constructorVec< ::gua::math::vec4i>))
    .def(init< ::gua::math::vec4i::value_type,
               ::gua::math::vec4i::value_type,
               ::gua::math::vec4i::value_type,
               ::gua::math::vec4i::value_type>())
    .def(init< ::gua::math::vec4i>())
    ADD_VECTOR_OPERATORS( ::gua::math::vec4i)
    .add_property("x", &getX< ::gua::math::vec4i>, &setX< ::gua::math::vec4i>)
    .add_property("y", &getY< ::gua::math::vec4i>, &setY< ::gua::math::vec4i>)
    .add_property("z", &getZ< ::gua::math::vec4i>, &setZ< ::gua::math::vec4i>)
    .add_property("w", &getW< ::gua::math::vec4i>, &setW< ::gua::math::vec4i>)
    .def("normalize", &normalize< ::gua::math::vec4i>)
    .def("length", &length< ::gua::math::vec4i>)
    .def("length2", &length2< ::gua::math::vec4i>)
    .def("distance_to", &distanceTo< ::gua::math::vec4i>)
    .def("lerp_to", &lerpTo< ::gua::math::vec4i>)
    .def("__repr__", &toString< ::gua::math::vec4i>)
    .def("__getitem__", &getVecElement< ::gua::math::vec4i>)
  ;

  // register as a field
  register_field<av::gua::SFVec4i>("SFVec4i");
  register_multifield<av::gua::MFVec4i>("MFVec4i");
}

void init_Vec4ui() {
  // wrapping gua::math::vec4ui functionality
  class_< ::gua::math::vec4ui>("Vec4ui", no_init)
    .def("__init__", make_constructor(&constructorVec< ::gua::math::vec4ui>))
    .def(init< ::gua::math::vec4ui::value_type,
               ::gua::math::vec4ui::value_type,
               ::gua::math::vec4ui::value_type,
               ::gua::math::vec4ui::value_type>())
    .def(init< ::gua::math::vec4ui>())
    ADD_VECTOR_OPERATORS( ::gua::math::vec4ui)
    .add_property("x", &getX< ::gua::math::vec4ui>, &setX< ::gua::math::vec4ui>)
    .add_property("y", &getY< ::gua::math::vec4ui>, &setY< ::gua::math::vec4ui>)
    .add_property("z", &getZ< ::gua::math::vec4ui>, &setZ< ::gua::math::vec4ui>)
    .add_property("w", &getW< ::gua::math::vec4ui>, &setW< ::gua::math::vec4ui>)
    .def("normalize", &normalize< ::gua::math::vec4ui>)
    .def("length", &length< ::gua::math::vec4ui>)
    .def("length2", &length2< ::gua::math::vec4ui>)
    .def("distance_to", &distanceTo< ::gua::math::vec4ui>)
    .def("lerp_to", &lerpTo< ::gua::math::vec4ui>)
    .def("__repr__", &toString< ::gua::math::vec4ui>)
    .def("__getitem__", &getVecElement< ::gua::math::vec4ui>)
  ;

  // register as a field
  register_field<av::gua::SFVec4ui>("SFVec4ui");
  register_multifield<av::gua::MFVec4ui>("MFVec4ui");
}

void init_Mat3() {
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

::gua::math::vec3 const getScale(::gua::math::mat4 const& mat) {
    ::gua::math::vec3 x_vec(mat[0], mat[1], mat[2]);
    ::gua::math::vec3 y_vec(mat[4], mat[5], mat[6]);
    ::gua::math::vec3 z_vec(mat[8], mat[9], mat[10]);
    return ::gua::math::vec3(scm::math::length(x_vec), scm::math::length(y_vec), scm::math::length(z_vec));
}

::gua::math::quat const getRotate1(::gua::math::mat4 const& mat) {
  return ::gua::math::quat::from_matrix(mat);
}

::gua::math::quat const getRotate2(::gua::math::mat4 const& mat) {
  ::gua::math::vec3 scale(getScale(mat));
  auto un_scaled_mat(scm::math::make_scale(1.0f/scale.x, 1.0f/scale.y, 1.0f/scale.z) * mat);
  return ::gua::math::quat::from_matrix(un_scaled_mat);
}

void init_Mat4() {
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
    .def("get_rotate", getRotate1)
    .def("get_rotate_scale_corrected", getRotate2)
    .def("get_scale", getScale)
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

void normalizeQuat(::gua::math::quat& quat) {
  quat = scm::math::normalize(quat);
}

void conjugateQuat(::gua::math::quat& quat) {
  quat = scm::math::conjugate(quat);
}

::gua::math::quat::value_type retrieveQuatAngle(::gua::math::quat const& quat) {
    ::gua::math::quat::value_type angle;
    ::gua::math::vec3 axis;
    quat.retrieve_axis_angle(angle, axis);
    return angle;
}

::gua::math::vec3 retrieveQuatAxis(::gua::math::quat const& quat) {
    ::gua::math::quat::value_type angle;
    ::gua::math::vec3 axis;
    quat.retrieve_axis_angle(angle, axis);
    return axis;
}

void init_Quat() {

  ::gua::math::quat::value_type const (*magnitude)( ::gua::math::quat const&) =
      &scm::math::magnitude;

  class_< ::gua::math::quat>("Quat", no_init)
    .def("__init__", make_constructor(&constructorQuat< ::gua::math::quat>))
    .def(init< ::gua::math::quat::value_type,
               ::gua::math::quat::value_type,
               ::gua::math::quat::value_type,
               ::gua::math::quat::value_type>())
    .def(init< ::gua::math::quat::value_type, ::gua::math::vec3>())
    .def(init< ::gua::math::quat>())
    .def(self == other< ::gua::math::quat>())
    .def_readwrite("x", & ::gua::math::quat::x)
    .def_readwrite("y", & ::gua::math::quat::y)
    .def_readwrite("z", & ::gua::math::quat::z)
    .def_readwrite("w", & ::gua::math::quat::w)
    .def("lerp_to", &lerpTo< ::gua::math::quat>)
    .def("slerp_to", &slerpTo< ::gua::math::quat>)
    .def("magnitude", magnitude)
    .def("normalize", &normalizeQuat)
    .def("conjugate", &conjugateQuat)
    .def("get_angle", &retrieveQuatAngle)
    .def("get_axis", &retrieveQuatAxis)
    .def("__repr__", &toString< ::gua::math::quat>)
  ;

  // register as a field
  register_field<av::gua::SFQuat>("SFQuat");
  register_multifield<av::gua::MFQuat>("MFQuat");

}
