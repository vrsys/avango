#include "ClippingPlaneNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/ClippingPlaneNode.hpp>

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

gua::math::vec3 get_center(av::gua::ClippingPlaneNode& self) { return self.getGuaNode()->get_center(); }

gua::math::vec3 get_normal(av::gua::ClippingPlaneNode& self) { return self.getGuaNode()->get_normal(); }

void init_ClippingPlaneNode()
{
    register_ptr_to_python<av::Link<av::gua::ClippingPlaneNode>>();

    register_field<av::gua::SFClippingPlaneNode>("SFClippingPlaneNode");
    register_multifield<av::gua::MFClippingPlaneNode>("MFClippingPlaneNode");
    class_<av::gua::ClippingPlaneNode, av::Link<av::gua::ClippingPlaneNode>, bases<av::gua::Node>, boost::noncopyable>("ClippingPlaneNode", "docstring", no_init)
        .def("get_center", get_center)
        .def("get_normal", get_normal);
}
