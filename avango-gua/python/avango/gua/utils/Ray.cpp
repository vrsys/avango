#include "Ray.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/utils/Ray.hpp>

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

void init_Ray()
{
    register_ptr_to_python<av::Link<av::gua::Ray>>();
    register_field<av::gua::SFRay>("SFRay");
    register_multifield<av::gua::MFRay>("MFRay");
    class_<av::gua::Ray, av::Link<av::gua::Ray>, bases<av::FieldContainer>, boost::noncopyable>("Ray", "docstring", no_init);
}
