#include "VolumePassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/volume/VolumePassDescription.hpp>

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


void init_VolumePassDescription()
 {
  register_field<av::gua::SFVolumePassDescription>("SFVolumePassDescription");
  register_multifield<av::gua::MFVolumePassDescription>("MFVolumePassDescription");
  class_<av::gua::VolumePassDescription,
         av::Link<av::gua::VolumePassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("VolumePassDescription", "docstring", no_init)
         ;
 }

