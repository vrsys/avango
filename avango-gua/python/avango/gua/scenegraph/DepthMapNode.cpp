#include "DepthMapNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/DepthMapNode.hpp>
#include <avango/gua/scenegraph/SceneGraph.hpp>

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

void init_DepthMapNode()
{
    register_ptr_to_python<av::Link<av::gua::DepthMapNode>>();

    register_field<av::gua::SFDepthMapNode>("SFDepthMapNode");
    register_multifield<av::gua::MFDepthMapNode>("MFDepthMapNode");
    class_<av::gua::DepthMapNode, av::Link<av::gua::DepthMapNode>, bases<av::gua::Node>, boost::noncopyable>("DepthMapNode", "docstring", no_init)
        .def("create_weights", &av::gua::DepthMapNode::create_weights)
        .def("get_push_back", &av::gua::DepthMapNode::get_push_back)
        .def("get_pull_in", &av::gua::DepthMapNode::get_pull_in);

    enum_<av::gua::DepthMapNode::RenderModeEnum>("DepthCubeMapRenderMode").value("COMPLETE", av::gua::DepthMapNode::COMPLETE).value("ONE_SIDE_PER_FRAME", av::gua::DepthMapNode::ONE_SIDE_PER_FRAME);
}
