#include <avango/gua/renderer/OcclusionSlaveResolvePassDescription.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::OcclusionSlaveResolvePassDescription"));
}

AV_FC_DEFINE(av::gua::OcclusionSlaveResolvePassDescription);

AV_FIELD_DEFINE(av::gua::SFOcclusionSlaveResolvePassDescription);
AV_FIELD_DEFINE(av::gua::MFOcclusionSlaveResolvePassDescription);

av::gua::OcclusionSlaveResolvePassDescription::OcclusionSlaveResolvePassDescription(std::shared_ptr<::gua::OcclusionSlaveResolvePassDescription> const& guaOcclusionSlaveResolvePassDescription)
    : PipelinePassDescription(guaOcclusionSlaveResolvePassDescription), m_guaOcclusionSlaveResolvePassDescription(guaOcclusionSlaveResolvePassDescription)
{
}

void av::gua::OcclusionSlaveResolvePassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::OcclusionSlaveResolvePassDescription, true);

        SFOcclusionSlaveResolvePassDescription::initClass("av::gua::SFOcclusionSlaveResolvePassDescription", "av::Field");
        MFOcclusionSlaveResolvePassDescription::initClass("av::gua::MFOcclusionSlaveResolvePassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}