#include <avango/gua/tv_3/renderer/TV_3SurfacePassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::tv_3::TV_3SurfacePassDescription"));
}

AV_FC_DEFINE(av::gua::tv_3::TV_3SurfacePassDescription);

AV_FIELD_DEFINE(av::gua::tv_3::SFTV_3SurfacePassDescription);
AV_FIELD_DEFINE(av::gua::tv_3::MFTV_3SurfacePassDescription);

av::gua::tv_3::TV_3SurfacePassDescription::TV_3SurfacePassDescription(
  std::shared_ptr< ::gua::TV_3SurfacePassDescription> const& guaTV_3SurfacePassDescription)
    : PipelinePassDescription(guaTV_3SurfacePassDescription)
    , m_guaTV_3SurfacePassDescription(guaTV_3SurfacePassDescription)
{

}

void
av::gua::tv_3::TV_3SurfacePassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::tv_3::TV_3SurfacePassDescription, true);

        SFTV_3SurfacePassDescription::initClass("av::gua::tv_3::SFTV_3SurfacePassDescription", "av::Field");
        MFTV_3SurfacePassDescription::initClass("av::gua::tv_3::MFTV_3SurfacePassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}


std::shared_ptr< ::gua::TV_3SurfacePassDescription> const&
av::gua::tv_3::TV_3SurfacePassDescription::getGuaTV_3SurfacePassDescription() const
{
    return m_guaTV_3SurfacePassDescription;
}


