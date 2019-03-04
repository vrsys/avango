#include "TexturedQuadPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/TexturedQuadPassDescription.hpp>

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

void init_TexturedQuadPassDescription()
{
    register_ptr_to_python<av::Link<av::gua::TexturedQuadPassDescription>>();
    register_field<av::gua::SFTexturedQuadPassDescription>("SFTexturedQuadPassDescription");
    register_multifield<av::gua::MFTexturedQuadPassDescription>("MFTexturedQuadPassDescription");
    class_<av::gua::TexturedQuadPassDescription, av::Link<av::gua::TexturedQuadPassDescription>, bases<av::gua::PipelinePassDescription>, boost::noncopyable>(
        "TexturedQuadPassDescription", "docstring", no_init);
}
