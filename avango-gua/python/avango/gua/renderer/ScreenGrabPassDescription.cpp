#include "ScreenGrabPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/ScreenGrabPassDescription.hpp>

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

void grabNext(av::gua::ScreenGrabPassDescription const& pass) { pass.getGuaScreenGrabPassDescription()->set_grab_next(true); }

void setOutputPrefix(av::gua::ScreenGrabPassDescription const& pass, std::string const& prefix) { pass.getGuaScreenGrabPassDescription()->set_output_prefix(prefix); }

void init_ScreenGrabPassDescription()
{
    register_ptr_to_python<av::Link<av::gua::ScreenGrabPassDescription>>();

    register_field<av::gua::SFScreenGrabPassDescription>("SFScreenGrabPassDescription");
    register_multifield<av::gua::MFScreenGrabPassDescription>("MFScreenGrabPassDescription");
    class_<av::gua::ScreenGrabPassDescription, av::Link<av::gua::ScreenGrabPassDescription>, bases<av::gua::PipelinePassDescription>, boost::noncopyable>(
        "ScreenGrabPassDescription", "docstring", no_init)
        .def("grabNext", &grabNext)
        .def("setOutputPrefix", &setOutputPrefix);
}
