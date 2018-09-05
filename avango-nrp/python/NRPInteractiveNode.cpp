#include <python3.5/Python.h>
#include <avango/python/register_field.h>
#include <boost/python.hpp>

#include "NRPInteractiveNode.hpp"
#include <avango/gua/nrp/NRPInteractiveNode.hpp>
#include <avango/gua/scenegraph/TransformNode.hpp>

namespace boost
{
namespace python
{
template <class T>
struct pointee<av::Link<T>>
{
    typedef T type;
};
}
}

void init_NRPInteractiveNode()
{
    boost::python::register_ptr_to_python<av::Link<av::gua::nrp::NRPInteractiveNode>>();

    av::python::register_field<av::gua::nrp::SFNRPInteractiveNode>("SFNRPInteractiveNode");
    av::python::register_multifield<av::gua::nrp::MFNRPInteractiveNode>("MFNRPInteractiveNode");
    boost::python::class_<av::gua::nrp::NRPInteractiveNode, av::Link<av::gua::nrp::NRPInteractiveNode>, boost::python::bases<av::gua::TransformNode>, boost::noncopyable>("NRPInteractiveNode", "docstring");
}
