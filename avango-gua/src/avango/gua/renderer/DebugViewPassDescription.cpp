#include <avango/gua/renderer/DebugViewPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::DebugViewPassDescription"));
}

AV_FC_DEFINE(av::gua::DebugViewPassDescription);

AV_FIELD_DEFINE(av::gua::SFDebugViewPassDescription);
AV_FIELD_DEFINE(av::gua::MFDebugViewPassDescription);

av::gua::DebugViewPassDescription::DebugViewPassDescription(std::shared_ptr<::gua::DebugViewPassDescription> const& guaDebugViewPassDescription)
    : PipelinePassDescription(guaDebugViewPassDescription), m_guaDebugViewPassDescription(guaDebugViewPassDescription)
{
}

void av::gua::DebugViewPassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::DebugViewPassDescription, true);

        SFDebugViewPassDescription::initClass("av::gua::SFDebugViewPassDescription", "av::Field");
        MFDebugViewPassDescription::initClass("av::gua::MFDebugViewPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr<::gua::DebugViewPassDescription> const& av::gua::DebugViewPassDescription::getGuaDebugViewPassDescription() const { return m_guaDebugViewPassDescription; }
