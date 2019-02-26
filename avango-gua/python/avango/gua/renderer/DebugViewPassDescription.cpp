#include "DebugViewPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/DebugViewPassDescription.hpp>

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

void init_DebugViewPassDescription()
{
    register_ptr_to_python<av::Link<av::gua::DebugViewPassDescription>>();

    register_field<av::gua::SFDebugViewPassDescription>("SFDebugViewPassDescription");
    register_multifield<av::gua::MFDebugViewPassDescription>("MFDebugViewPassDescription");
    class_<av::gua::DebugViewPassDescription, av::Link<av::gua::DebugViewPassDescription>, bases<av::gua::PipelinePassDescription>, boost::noncopyable>(
        "DebugViewPassDescription", "docstring", no_init);
}
