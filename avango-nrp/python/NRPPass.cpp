#include "NRPPass.hpp"

#include <avango/gua/nrp/NRPPass.hpp>
#include <avango/gua/renderer/PipelinePassDescription.hpp>
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
    typedef T type;
};
}
}

void init_NRPPassDescription()
{
    register_ptr_to_python<av::Link<av::gua::nrp::NRPPassDescription>>();

    register_field<av::gua::nrp::SFNRPPassDescription>("SFNRPPassDescription");
    register_multifield<av::gua::nrp::MFNRPPassDescription>("MFNRPPassDescription");
    class_<av::gua::nrp::NRPPassDescription, av::Link<av::gua::nrp::NRPPassDescription>, bases<av::gua::PipelinePassDescription>, boost::noncopyable>("NRPPassDescription", "docstring");
}
