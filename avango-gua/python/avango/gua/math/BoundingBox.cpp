#include "BoundingBox.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/math/BoundingBox.hpp>
#include <avango/gua/Types.hpp>

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

float size(av::gua::BoundingBox const& box, int dimension) {
  return box.getGuaBoundingBox()->size(dimension);
}

bool contains(av::gua::BoundingBox const& box, ::gua::math::vec3 const& vec) {
  return box.getGuaBoundingBox()->contains(vec);
}

bool intersects(av::gua::BoundingBox const& box, av::gua::BoundingBox const& rhs) {
  return box.getGuaBoundingBox()->intersects(*rhs.getGuaBoundingBox());
}

void expandBy1(av::gua::BoundingBox & box, ::gua::math::vec3 vec) {
  box.getGuaBoundingBox()->expandBy(vec);
}

void expandBy2(av::gua::BoundingBox & box, av::gua::BoundingBox const& rhs) {
  box.getGuaBoundingBox()->expandBy(*rhs.getGuaBoundingBox());
}

bool isEmpty(av::gua::BoundingBox const& box) {
  return box.getGuaBoundingBox()->isEmpty();
}

::gua::math::vec3 center(av::gua::BoundingBox const& box) {
  return box.getGuaBoundingBox()->center();
}

av::gua::BoundingBox combine(av::gua::BoundingBox const& lhs,
                             av::gua::BoundingBox const& rhs) {

  auto combined(::gua::math::combine(*lhs.getGuaBoundingBox(),
                                     *rhs.getGuaBoundingBox()));

  auto combined_ptr( new ::gua::math::BoundingBox< ::gua::math::vec3>(combined));

  av::gua::BoundingBox result(combined_ptr);
  return result;
}

av::gua::BoundingBox intersection(av::gua::BoundingBox const& lhs,
                                  av::gua::BoundingBox const& rhs) {

  auto intersected(::gua::math::intersection(*lhs.getGuaBoundingBox(),
                                             *rhs.getGuaBoundingBox()));

  auto intersected_ptr( new ::gua::math::BoundingBox< ::gua::math::vec3>(intersected));

  av::gua::BoundingBox result(intersected_ptr);
  return result;
}

void init_BoundingBox()
{
  register_ptr_to_python<av::Link<av::gua::BoundingBox> >();

  register_field<av::gua::SFBoundingBox>("SFBoundingBox");
  register_multifield<av::gua::MFBoundingBox>("MFBoundingBox");

  class_<av::gua::BoundingBox,
         av::Link<av::gua::BoundingBox>,
         bases<av::FieldContainer>, boost::noncopyable >("BoundingBox",
                                                         "docstring",
                                                         no_init)
      .def(self == other< av::gua::BoundingBox>())
      .def(self != other< av::gua::BoundingBox>())
      .def("size", &size)
      .def("contains", &contains)
      .def("intersects", &intersects)
      .def("expand_by", &expandBy1)
      .def("expand_by", &expandBy2)
      .def("is_empty", &isEmpty)
      .def("center", &center)
  ;

  def("combine", &combine);
  def("intersection", &intersection);
}
