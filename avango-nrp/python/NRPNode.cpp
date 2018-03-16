#include "NRPNode.hpp"

#include <avango/gua/nrp/NRPNode.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>
#include <avango/python/register_field.h>
#include <boost/python.hpp>

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
}
}

void preDraw(av::gua::nrp::NRPNode & nrp_node) {

   return nrp_node.pre_draw();
}

void init_NRPNode()
{
    register_ptr_to_python<av::Link<av::gua::nrp::NRPNode>>();

    register_field<av::gua::nrp::SFNRPNode>("SFNRPNode");
    register_multifield<av::gua::nrp::MFNRPNode>("MFNRPNode");
    class_<av::gua::nrp::NRPNode, av::Link<av::gua::nrp::NRPNode>, bases<av::gua::TransformNode>, boost::noncopyable>("NRPNode", "docstring")
    .def("pre_draw", &preDraw);
}
