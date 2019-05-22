#include "PickResult.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/PickResult.hpp>

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

void init_PickResult()
{
    register_ptr_to_python<av::Link<av::gua::PickResult>>();

    register_field<av::gua::SFPickResult>("SFPickResult");
    register_multifield<av::gua::MFPickResult>("MFPickResult");
    class_<av::gua::PickResult, av::Link<av::gua::PickResult>, bases<av::FieldContainer>, boost::noncopyable>("PickResult", "docstring", no_init);

    enum_<av::gua::PickResult::Options>("PickingOptions")
        .value("PICK_ALL", av::gua::PickResult::PICK_ALL)
        .value("PICK_ONLY_FIRST_OBJECT", av::gua::PickResult::PICK_ONLY_FIRST_OBJECT)
        .value("PICK_ONLY_FIRST_FACE", av::gua::PickResult::PICK_ONLY_FIRST_FACE)
        .value("GET_POSITIONS", av::gua::PickResult::GET_POSITIONS)
        .value("GET_WORLD_POSITIONS", av::gua::PickResult::GET_WORLD_POSITIONS)
        .value("GET_NORMALS", av::gua::PickResult::GET_NORMALS)
        .value("GET_WORLD_NORMALS", av::gua::PickResult::GET_WORLD_NORMALS)
        .value("INTERPOLATE_NORMALS", av::gua::PickResult::INTERPOLATE_NORMALS)
        .value("GET_TEXTURE_COORDS", av::gua::PickResult::GET_TEXTURE_COORDS);
}
