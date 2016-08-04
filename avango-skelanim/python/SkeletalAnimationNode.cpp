#include "SkeletalAnimationNode.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/scenegraph/GeometryNode.hpp>
#include <avango/gua/skelanim/scenegraph/SkeletalAnimationNode.hpp>

using namespace boost::python;
using namespace av::python;

namespace boost
{
  namespace python
  {
    template <class T> struct pointee<av::Link<T> >
    {
      using type = T;
    };
  }
}

float getAnimDuration(av::gua::skelanim::SkeletalAnimationNode const& node, std::string const& name) {
   return node.getAnimDuration(name);
}

void loadAnimation(av::gua::skelanim::SkeletalAnimationNode& node,
    std::string const& file_name,
    std::string const& animation_name) {
  node.loadAnimation(file_name, animation_name);
}

void init_SkeletalAnimationNode()
{
  register_field<av::gua::skelanim::SFSkeletalAnimationNode>("SFSkeletalAnimationNode");
  register_multifield<av::gua::skelanim::MFSkeletalAnimationNode>("MFSkeletalAnimationNode");

  class_<av::gua::skelanim::SkeletalAnimationNode,
          av::Link<av::gua::skelanim::SkeletalAnimationNode>,
          bases<av::gua::GeometryNode>, 
          boost::noncopyable>("SkeletalAnimationNode", "docstring", no_init)
            .def("get_duration", &getAnimDuration)
            .def("load_animation", &loadAnimation)
    ;
}
