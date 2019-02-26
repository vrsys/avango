#include "TransformNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/TransformNode.hpp>

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

void init_TransformNode()
{
    register_ptr_to_python<av::Link<av::gua::TransformNode>>();
    register_field<av::gua::SFTransformNode>("SFTransformNode");
    register_multifield<av::gua::MFTransformNode>("MFTransformNode");
    class_<av::gua::TransformNode, av::Link<av::gua::TransformNode>, bases<av::gua::Node>, boost::noncopyable>("TransformNode", "docstring", no_init);
}
