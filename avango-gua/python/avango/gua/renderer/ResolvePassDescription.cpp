#include "ResolvePassDescription.hpp"

#include <boost/python.hpp>
#include <avango/python/register_field.h>
#include <avango/gua/renderer/ResolvePassDescription.hpp>

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


void init_ResolvePassDescription()
 {
   register_ptr_to_python<av::Link<av::gua::ResolvePassDescription> >();
  register_field<av::gua::SFResolvePassDescription>("SFResolvePassDescription");
  register_multifield<av::gua::MFResolvePassDescription>("MFResolvePassDescription");
  class_<av::gua::ResolvePassDescription,
         av::Link<av::gua::ResolvePassDescription>,
         bases<av::gua::PipelinePassDescription>, boost::noncopyable >("ResolvePassDescription", "docstring", no_init)
         ;

    enum_<av::gua::ResolvePassDescription::BackgroundModeEnum>("BackgroundMode")
        .value("COLOR", av::gua::ResolvePassDescription::COLOR)
        .value("SKYMAP_TEXTURE", av::gua::ResolvePassDescription::SKYMAP_TEXTURE)
        .value("QUAD_TEXTURE", av::gua::ResolvePassDescription::QUAD_TEXTURE)
        .value("CUBEMAP_TEXTURE", av::gua::ResolvePassDescription::CUBEMAP_TEXTURE)
        ;

    enum_<av::gua::ResolvePassDescription::ToneMappingModeEnum>("ToneMappingMode")
        .value("LINEAR", av::gua::ResolvePassDescription::LINEAR)
        .value("HEJL", av::gua::ResolvePassDescription::HEJL)
        .value("REINHARD", av::gua::ResolvePassDescription::REINHARD)
        .value("UNCHARTED", av::gua::ResolvePassDescription::UNCHARTED)
        ;

    enum_<av::gua::ResolvePassDescription::EnvironmentLightingModeEnum>("EnvironmentLightingMode")
        .value("SPHEREMAP", av::gua::ResolvePassDescription::SPHEREMAP)
        .value("CUBEMAP", av::gua::ResolvePassDescription::CUBEMAP)
        .value("AMBIENT_COLOR", av::gua::ResolvePassDescription::AMBIENT_COLOR)
        ;


 }

