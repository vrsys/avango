#include "LightVisibilityPassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/LightVisibilityPassDescription.hpp>

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


void init_LightVisibilityPassDescription()
 {
  register_field<av::gua::SFLightVisibilityPassDescription>("SFLightVisibilityPassDescription");
  register_multifield<av::gua::MFLightVisibilityPassDescription>("MFLightVisibilityPassDescription");
  class_<av::gua::LightVisibilityPassDescription,
         av::Link<av::gua::LightVisibilityPassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("LightVisibilityPassDescription", "docstring", no_init)
         ;

    enum_<av::gua::LightVisibilityPassDescription::RasterizationModeEnum>("RasterizationMode")
        .value("AUTO", av::gua::LightVisibilityPassDescription::AUTO)
        .value("SIMPLE", av::gua::LightVisibilityPassDescription::SIMPLE)
        .value("CONSERVATIVE", av::gua::LightVisibilityPassDescription::CONSERVATIVE)
        .value("MULTISAMPLED_2", av::gua::LightVisibilityPassDescription::MULTISAMPLED_2)
        .value("MULTISAMPLED_4", av::gua::LightVisibilityPassDescription::MULTISAMPLED_4)
        .value("MULTISAMPLED_8", av::gua::LightVisibilityPassDescription::MULTISAMPLED_8)
        .value("MULTISAMPLED_16", av::gua::LightVisibilityPassDescription::MULTISAMPLED_16)
        .value("FULLSCREEN_FALLBACK", av::gua::LightVisibilityPassDescription::FULLSCREEN_FALLBACK)
        ;

 }

