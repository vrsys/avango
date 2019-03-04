#include "LineStripPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/LineStripPassDescription.hpp>

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

void init_LineStripPassDescription()
{
    register_ptr_to_python<av::Link<av::gua::LineStripPassDescription>>();
    register_field<av::gua::SFLineStripPassDescription>("SFLineStripPassDescription");
    register_multifield<av::gua::MFLineStripPassDescription>("MFLineStripPassDescription");
    class_<av::gua::LineStripPassDescription, av::Link<av::gua::LineStripPassDescription>, bases<av::gua::PipelinePassDescription>, boost::noncopyable>(
        "LineStripPassDescription", "docstring", no_init);
}
