#include "TV_3VolumePassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/tv_3/renderer/TV_3VolumePassDescription.hpp>

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

void init_TV_3VolumePassDescription()
{
    register_ptr_to_python<av::Link<av::gua::tv_3::TV_3VolumePassDescription>>();
    register_field<av::gua::tv_3::SFTV_3VolumePassDescription>("SFTV_3VolumePassDescription");
    register_multifield<av::gua::tv_3::MFTV_3VolumePassDescription>("MFTV_3VolumePassDescription");
    class_<av::gua::tv_3::TV_3VolumePassDescription, av::Link<av::gua::tv_3::TV_3VolumePassDescription>, bases<av::gua::PipelinePassDescription>, boost::noncopyable>(
        "TV_3VolumePassDescription", "docstring", no_init);
}
