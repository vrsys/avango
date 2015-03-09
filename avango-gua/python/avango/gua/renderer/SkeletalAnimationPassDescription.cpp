#include "SkeletalAnimationPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/SkeletalAnimationPassDescription.hpp>

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
  register_field<av::gua::SFSkeletalAnimationPassDescription>("SFSkeletalAnimationPassDescription");
  register_multifield<av::gua::MFSkeletalAnimationPassDescription>("MFSkeletalAnimationPassDescription");
  class_<av::gua::SkeletalAnimationPassDescription,
         av::Link<av::gua::SkeletalAnimationPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("SkeletalAnimationPassDescription", "docstring", no_init)
         ;
 }

