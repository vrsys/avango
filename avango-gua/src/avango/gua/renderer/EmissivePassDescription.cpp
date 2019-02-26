#include <avango/gua/renderer/EmissivePassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::EmissivePassDescription"));
}

AV_FC_DEFINE(av::gua::EmissivePassDescription);

AV_FIELD_DEFINE(av::gua::SFEmissivePassDescription);
AV_FIELD_DEFINE(av::gua::MFEmissivePassDescription);

av::gua::EmissivePassDescription::EmissivePassDescription(std::shared_ptr<::gua::EmissivePassDescription> const& guaEmissivePassDescription)
    : PipelinePassDescription(guaEmissivePassDescription), m_guaEmissivePassDescription(guaEmissivePassDescription)
{
}

void av::gua::EmissivePassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::EmissivePassDescription, true);

        SFEmissivePassDescription::initClass("av::gua::SFEmissivePassDescription", "av::Field");
        MFEmissivePassDescription::initClass("av::gua::MFEmissivePassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr<::gua::EmissivePassDescription> const& av::gua::EmissivePassDescription::getGuaEmissivePassDescription() const { return m_guaEmissivePassDescription; }
