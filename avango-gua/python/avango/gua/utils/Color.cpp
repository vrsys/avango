#include "Color.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/Fields.hpp>
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

float getR(::gua::utils::Color3f* self) { return self->r(); }
float getG(::gua::utils::Color3f* self) { return self->g(); }
float getB(::gua::utils::Color3f* self) { return self->b(); }

void setR(::gua::utils::Color3f* self, float value) { self->r(value); }
void setG(::gua::utils::Color3f* self, float value) { self->g(value); }
void setB(::gua::utils::Color3f* self, float value) { self->b(value); }

void init_Color()
{
    // wrapping gua::math::vec3 functionality
    class_<::gua::utils::Color3f>("Color")
        .def(init<float, float, float>())
        .def(init<::gua::utils::Color3f>())
        .add_property("r", &getR, &setR)
        .add_property("g", &getG, &setG)
        .add_property("b", &getB, &setB);

    // register as a field
    register_field<av::gua::SFColor>("SFColor");
    //    register_multifield<av::gua::MFColor>("MFColor");
}
