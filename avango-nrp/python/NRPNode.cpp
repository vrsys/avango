#include <python3.5/Python.h>
#include <avango/python/register_field.h>
#include <boost/python.hpp>

#include "NRPNode.hpp"
#include <avango/gua/nrp/NRPNode.hpp>
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

void init_NRPNode()
{
    boost::python::register_ptr_to_python<av::Link<av::gua::nrp::NRPNode>>();

    av::python::register_field<av::gua::nrp::SFNRPNode>("SFNRPNode");
    av::python::register_multifield<av::gua::nrp::MFNRPNode>("MFNRPNode");
    boost::python::class_<av::gua::nrp::NRPNode, av::Link<av::gua::nrp::NRPNode>, boost::python::bases<av::gua::TransformNode>, boost::noncopyable>("NRPNode", "docstring");
}
