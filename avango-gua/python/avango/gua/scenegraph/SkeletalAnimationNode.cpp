#include "SkeletalAnimationNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/scenegraph/SkeletalAnimationNode.hpp>

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

void init_SkeletalAnimationNode()
{
  register_field<av::gua::SFSkeletalAnimationNode>("SFSkeletalAnimationNode");
  register_multifield<av::gua::MFSkeletalAnimationNode>("MFSkeletalAnimationNode");
  class_<av::gua::SkeletalAnimationNode, av::Link<av::gua::SkeletalAnimationNode>,
    bases<av::gua::GeometryNode>,
    boost::noncopyable >("SkeletalAnimationNode", "docstring", no_init);
}
