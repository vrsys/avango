#include <python3.5/Python.h>
#include <avango/python/register_field.h>
#include <boost/python.hpp>

#include "NRPPass.hpp"
#include <avango/gua/nrp/NRPPass.hpp>
#include <avango/gua/renderer/PipelinePassDescription.hpp>

namespace boost
{
namespace python
{
template <class T>
struct pointee<av::Link<T>>
{
    typedef T type;
};
} // namespace python
} // namespace boost

void init_NRPPassDescription()
{
    boost::python::register_ptr_to_python<av::Link<av::gua::nrp::NRPPassDescription>>();

    av::python::register_field<av::gua::nrp::SFNRPPassDescription>("SFNRPPassDescription");
    av::python::register_multifield<av::gua::nrp::MFNRPPassDescription>("MFNRPPassDescription");
    boost::python::class_<av::gua::nrp::NRPPassDescription, av::Link<av::gua::nrp::NRPPassDescription>, boost::python::bases<av::gua::PipelinePassDescription>, boost::noncopyable>(
        "NRPPassDescription", "docstring");
}
