#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/lod/renderer/MLodPassDescription.hpp>

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
} // namespace python
} // namespace boost

void init_MLodPassDescription()
{
    register_field<av::gua::lod::SFMLodPassDescription>("SFMLodPassDescription");
    register_multifield<av::gua::lod::MFMLodPassDescription>("MFMLodPassDescription");
    class_<av::gua::lod::MLodPassDescription, av::Link<av::gua::lod::MLodPassDescription>, bases<av::gua::PipelinePassDescription>, boost::noncopyable>("MLodPassDescription", "docstring", no_init);
}
