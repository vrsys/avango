#include "Viewer.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/viewer/Viewer.hpp>

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

void init_Viewer()
{
    register_ptr_to_python<av::Link<av::gua::Viewer>>();

    class_<av::gua::Viewer, av::Link<av::gua::Viewer>, bases<av::FieldContainer>, boost::noncopyable>("Viewer", "docstring", no_init)
        .def("run", &av::gua::Viewer::run)
        .def("frame", &av::gua::Viewer::frame);

    // register as a field
    register_field<av::gua::SFViewer>("SFViewer");
    register_multifield<av::gua::MFViewer>("MFViewer");
}
