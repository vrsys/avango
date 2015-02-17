#include "BackgroundPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/BackgroundPassDescription.hpp>

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


void init_BackgroundPassDescription()
 {
  register_field<av::gua::SFBackgroundPassDescription>("SFBackgroundPassDescription");
  register_multifield<av::gua::MFBackgroundPassDescription>("MFBackgroundPassDescription");
  class_<av::gua::BackgroundPassDescription,
         av::Link<av::gua::BackgroundPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("BackgroundPassDescription", "docstring", no_init)
         ;

    enum_<av::gua::BackgroundPassDescription::BackgroundMode>("BackgroundMode")
        .value("COLOR", av::gua::BackgroundPassDescription::COLOR)
        .value("SKYMAP_TEXTURE", av::gua::BackgroundPassDescription::SKYMAP_TEXTURE)
        .value("QUAD_TEXTURE", av::gua::BackgroundPassDescription::QUAD_TEXTURE)
        ;

 }

