#include "VolumeNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/volume/VolumeNode.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
  namespace python
  {
    template <class T> struct pointee<av::Link<T> >
    {
      typedef T type;
    };
  }
}

void init_VolumeNode()
{
  register_field<av::gua::SFVolumeNode>("SFVolumeNode");
  register_multifield<av::gua::MFVolumeNode>("MFVolumeNode");
  class_<av::gua::VolumeNode, av::Link<av::gua::VolumeNode>,
    bases<av::gua::GeometryNode>,
    boost::noncopyable >("VolumeNode", "docstring", no_init);
}
