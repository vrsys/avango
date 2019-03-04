#include "BoundingSphere.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/math/BoundingSphere.hpp>
#include <avango/gua/Types.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
namespace python
{
template <class T>
struct pointee<av::Link<T>>
{
    using type = T;
};
} // namespace python
} // namespace boost

bool contains(av::gua::BoundingSphere const& box, ::gua::math::vec3 const& vec) { return box.getGuaBoundingSphere()->contains(vec); }

void expandBy2(av::gua::BoundingSphere& box, av::gua::BoundingSphere const& rhs) { box.getGuaBoundingSphere()->expandBy(*rhs.getGuaBoundingSphere()); }

bool isEmpty(av::gua::BoundingSphere const& box) { return box.getGuaBoundingSphere()->isEmpty(); }

av::gua::BoundingSphere combine(av::gua::BoundingSphere const& lhs, av::gua::BoundingSphere const& rhs)
{
    auto combined(::gua::math::combine(*lhs.getGuaBoundingSphere(), *rhs.getGuaBoundingSphere()));

    auto combined_ptr(new ::gua::math::BoundingSphere<::gua::math::vec3>(combined));

    av::gua::BoundingSphere result(combined_ptr);
    return result;
}

void init_BoundingSphere()
{
    register_ptr_to_python<av::Link<av::gua::BoundingSphere>>();

    register_field<av::gua::SFBoundingSphere>("SFBoundingSphere");
    register_multifield<av::gua::MFBoundingSphere>("MFBoundingSphere");

    class_<av::gua::BoundingSphere, av::Link<av::gua::BoundingSphere>, bases<av::FieldContainer>, boost::noncopyable>("BoundingSphere", "docstring", no_init)
        .def(self == other<av::gua::BoundingSphere>())
        .def(self != other<av::gua::BoundingSphere>())
        .def("contains", &contains)
        .def("expand_by", &expandBy2)
        .def("is_empty", &isEmpty);

    def("combine", &combine);
}
