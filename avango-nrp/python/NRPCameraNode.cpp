#include <python3.5/Python.h>
#include <avango/python/register_field.h>
#include <boost/python.hpp>

#include "NRPCameraNode.hpp"
#include <avango/gua/nrp/NRPCameraNode.hpp>
#include <avango/gua/scenegraph/CameraNode.hpp>

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

void init_NRPCameraNode()
{
    boost::python::register_ptr_to_python<av::Link<av::gua::nrp::NRPCameraNode>>();

    av::python::register_field<av::gua::nrp::SFNRPCameraNode>("SFNRPCameraNode");
    av::python::register_multifield<av::gua::nrp::MFNRPCameraNode>("MFNRPCameraNode");
    boost::python::class_<av::gua::nrp::NRPCameraNode, av::Link<av::gua::nrp::NRPCameraNode>, boost::python::bases<av::gua::CameraNode>, boost::noncopyable>("NRPCameraNode", "docstring");
}
