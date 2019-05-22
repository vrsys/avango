#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/lod/renderer/PLodPassDescription.hpp>

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

void init_PLodPassDescription()
{
    register_field<av::gua::lod::SFPLodPassDescription>("SFPLodPassDescription");
    register_multifield<av::gua::lod::MFPLodPassDescription>("MFPLodPassDescription");
    class_<av::gua::lod::PLodPassDescription, av::Link<av::gua::lod::PLodPassDescription>, bases<av::gua::PipelinePassDescription>, boost::noncopyable>("PLodPassDescription", "docstring", no_init);
    enum_<av::gua::lod::PLodPassDescription::Flags>("RenderFlags")
        .value("LQ_ONE_PASS", av::gua::lod::PLodPassDescription::LQ_ONE_PASS)
        .value("HQ_TWO_PASS", av::gua::lod::PLodPassDescription::HQ_TWO_PASS);
}
