#include "RayNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/RayNode.hpp>

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

void init_RayNode()
{
    register_ptr_to_python<av::Link<av::gua::RayNode>>();
    register_field<av::gua::SFRayNode>("SFRayNode");
    register_multifield<av::gua::MFRayNode>("MFRayNode");
    class_<av::gua::RayNode, av::Link<av::gua::RayNode>, bases<av::gua::Node>, boost::noncopyable>("RayNode", "docstring", no_init);
}
