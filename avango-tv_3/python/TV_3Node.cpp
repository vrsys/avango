#include "TV_3Node.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/tv_3/scenegraph/TV_3Node.hpp>

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

void init_TV_3Node()
{
    register_ptr_to_python<av::Link<av::gua::tv_3::TV_3Node>>();

    enum_<av::gua::tv_3::TV_3Node::NodeRenderMode>("RenderMode")
        .value("VOL_ISOSURFACE", av::gua::tv_3::TV_3Node::VOL_ISOSURFACE)
        .value("VOL_MAX_INTENSITY", av::gua::tv_3::TV_3Node::VOL_MAX_INTENSITY)
        .value("VOL_COMPOSITING", av::gua::tv_3::TV_3Node::VOL_COMPOSITING)
        .value("VOL_AVG_INTENSITY", av::gua::tv_3::TV_3Node::VOL_AVG_INTENSITY)
        .value("SUR_PBR", av::gua::tv_3::TV_3Node::SUR_PBR);

    enum_<av::gua::tv_3::TV_3Node::NodeSpatialFilterMode>("SpatialFilterMode").value("S_NEAREST", av::gua::tv_3::TV_3Node::S_NEAREST).value("S_LINEAR", av::gua::tv_3::TV_3Node::S_LINEAR);

    enum_<av::gua::tv_3::TV_3Node::NodeTemporalFilterMode>("TemporalFilterMode").value("T_NEAREST", av::gua::tv_3::TV_3Node::T_NEAREST).value("T_LINEAR", av::gua::tv_3::TV_3Node::T_LINEAR);

    enum_<av::gua::tv_3::TV_3Node::NodePlaybackMode>("PlaybackMode")
        .value("NONE", av::gua::tv_3::TV_3Node::NONE)
        .value("FORWARD", av::gua::tv_3::TV_3Node::FORWARD)
        .value("BACKWARD", av::gua::tv_3::TV_3Node::BACKWARD);

    register_field<av::gua::tv_3::SFTV_3Node>("SFTV_3Node");
    register_multifield<av::gua::tv_3::MFTV_3Node>("MFTV_3Node");
    class_<av::gua::tv_3::TV_3Node, av::Link<av::gua::tv_3::TV_3Node>, bases<av::gua::GeometryNode>, boost::noncopyable>("TV_3Node", "docstring", no_init);
}
