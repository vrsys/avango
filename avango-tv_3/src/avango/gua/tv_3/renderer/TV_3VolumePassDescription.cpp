#include <avango/gua/tv_3/renderer/TV_3VolumePassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::tv_3::TV_3VolumePassDescription"));
}

AV_FC_DEFINE(av::gua::tv_3::TV_3VolumePassDescription);

AV_FIELD_DEFINE(av::gua::tv_3::SFTV_3VolumePassDescription);
AV_FIELD_DEFINE(av::gua::tv_3::MFTV_3VolumePassDescription);

av::gua::tv_3::TV_3VolumePassDescription::TV_3VolumePassDescription(
  std::shared_ptr< ::gua::TV_3VolumePassDescription> const& guaTV_3VolumePassDescription)
    : PipelinePassDescription(guaTV_3VolumePassDescription)
    , m_guaTV_3VolumePassDescription(guaTV_3VolumePassDescription)
{

}

void
av::gua::tv_3::TV_3VolumePassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::tv_3::TV_3VolumePassDescription, true);

        SFTV_3VolumePassDescription::initClass("av::gua::tv_3::SFTV_3VolumePassDescription", "av::Field");
        MFTV_3VolumePassDescription::initClass("av::gua::tv_3::MFTV_3VolumePassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}


std::shared_ptr< ::gua::TV_3VolumePassDescription> const&
av::gua::tv_3::TV_3VolumePassDescription::getGuaTV_3VolumePassDescription() const
{
    return m_guaTV_3VolumePassDescription;
}


