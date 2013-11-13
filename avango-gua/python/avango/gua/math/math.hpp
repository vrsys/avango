#ifndef AV_PYTHON_GUA_MATH_HPP
#define AV_PYTHON_GUA_MATH_HPP

#include <string>
#include <sstream>
#include <boost/python.hpp>

#include <gua/math/math.hpp>

// helper functions to get an element
template <class T> typename T::value_type getVecElement(T* self, int index) {
  return (*self)[index];
}

// helper functions to get and set x, y, z, w properties on Vec2, Vec3, Vec4, 
// Quat
template <class T> typename T::value_type getX(T* self) { return (*self)[0]; }
template <class T> typename T::value_type getY(T* self) { return (*self)[1]; }
template <class T> typename T::value_type getZ(T* self) { return (*self)[2]; }
template <class T> typename T::value_type getW(T* self) { return (*self)[3]; }

template <class T> void  setX(T* self, typename T::value_type const& value) {
  (*self)[0] = value;
}

template <class T> void  setY(T* self, typename T::value_type const& value) {
  (*self)[1] = value;
}

template <class T> void  setZ(T* self, typename T::value_type const& value) {
  (*self)[2] = value;
}

template <class T> void  setW(T* self, typename T::value_type const& value) {
  (*self)[3] = value;
}

template <class T> typename T::value_type length (T* self) {
  return scm::math::length(*self);
}

template <class T> typename T::value_type length2 (T* self) {
  return scm::math::length_sqr(*self);
}

template <class T> typename T::value_type distanceTo (T* self, T* other) {
  return scm::math::distance(*self, *other);
}

template <class T> T lerpTo      (T* self, T* other, typename T::value_type a) {
  return scm::math::lerp(*self, *other, a);
}

template <class T> typename T::value_type normalize(T* self) {
    typename T::value_type length(scm::math::length(*self));
    *self /= length;
    return length;
}

template <class T> std::string toString(T* self) {
    std::ostringstream s;
    s << *self;
    return s.str();
}

// helper functions for contruction time initialisation
template <class T> boost::shared_ptr<T> constructorMat() {
  return boost::shared_ptr<T>( new T(T::identity()));
}

template <class T> boost::shared_ptr<T> constructorVec() {
  return boost::shared_ptr<T>( new T(T::zero()));
}

void init_Vec2();
void init_Vec2ui();
void init_Vec3();
void init_Vec4();

void init_Quat();

void init_Mat3();
void init_Mat4();

#endif //AV_PYTHON_GUA_MATH_HPP
