#include "MLodNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/lod/scenegraph/MLodNode.hpp>

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

void init_MLodNode()
{
    register_field<av::gua::lod::SFMLodNode>("SFMLodNode");
    register_multifield<av::gua::lod::MFMLodNode>("MFMLodNode");

    class_<av::gua::lod::MLodNode, av::Link<av::gua::lod::MLodNode>, bases<av::gua::GeometryNode>, boost::noncopyable>("MLodNode", "docstring", no_init);
}
