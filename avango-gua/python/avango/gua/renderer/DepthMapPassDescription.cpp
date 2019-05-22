#include "DepthMapPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/DepthMapPassDescription.hpp>

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

void init_DepthMapPassDescription()
{
    register_ptr_to_python<av::Link<av::gua::DepthMapPassDescription>>();

    register_field<av::gua::SFDepthMapPassDescription>("SFDepthMapPassDescription");
    register_multifield<av::gua::MFDepthMapPassDescription>("MFDepthMapPassDescription");
    class_<av::gua::DepthMapPassDescription, av::Link<av::gua::DepthMapPassDescription>, bases<av::gua::PipelinePassDescription>, boost::noncopyable>("DepthMapPassDescription", "docstring", no_init);
}
