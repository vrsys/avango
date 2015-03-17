#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/skelanim/renderer/SkeletalAnimationPassDescription.hpp>

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


void init_SkeletalAnimationPassDescription()
 {
  register_field<av::gua::skelanim::SFSkeletalAnimationPassDescription>("SFSkeletalAnimationPassDescription");
  register_multifield<av::gua::skelanim::MFSkeletalAnimationPassDescription>("MFSkeletalAnimationPassDescription");
  class_<av::gua::skelanim::SkeletalAnimationPassDescription,
         av::Link<av::gua::skelanim::SkeletalAnimationPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("SkeletalAnimationPassDescription", "docstring", no_init)
         ;
 }

