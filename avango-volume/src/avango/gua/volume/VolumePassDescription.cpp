#include <avango/gua/volume/VolumePassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::VolumePassDescription"));
}

AV_FC_DEFINE(av::gua::VolumePassDescription);

AV_FIELD_DEFINE(av::gua::SFVolumePassDescription);
AV_FIELD_DEFINE(av::gua::MFVolumePassDescription);

av::gua::VolumePassDescription::VolumePassDescription(std::shared_ptr<::gua::VolumePassDescription> const& guaVolumePassDescription)
    : PipelinePassDescription(guaVolumePassDescription), m_guaVolumePassDescription(guaVolumePassDescription)
{
}

void av::gua::VolumePassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::VolumePassDescription, true);

        SFVolumePassDescription::initClass("av::gua::SFVolumePassDescription", "av::Field");
        MFVolumePassDescription::initClass("av::gua::MFVolumePassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr<::gua::VolumePassDescription> const& av::gua::VolumePassDescription::getGuaVolumePassDescription() const { return m_guaVolumePassDescription; }
