#include "Point2PointConstraint.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/physics/Point2PointConstraint.hpp>

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

void init_Point2PointConstraint()
{
    register_field<av::gua::SFPoint2PointConstraint>("SFPoint2PointConstraint");
    register_multifield<av::gua::MFPoint2PointConstraint>("MFPoint2PointConstraint");

    class_<av::gua::Point2PointConstraint, av::Link<av::gua::Point2PointConstraint>, bases<av::gua::Constraint>, boost::noncopyable>("Point2PointConstraint", "docstring", no_init);
}
