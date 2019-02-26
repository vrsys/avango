#include <avango/gua/renderer/TexturedScreenSpaceQuadPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::TexturedScreenSpaceQuadPassDescription"));
}

AV_FC_DEFINE(av::gua::TexturedScreenSpaceQuadPassDescription);

AV_FIELD_DEFINE(av::gua::SFTexturedScreenSpaceQuadPassDescription);
AV_FIELD_DEFINE(av::gua::MFTexturedScreenSpaceQuadPassDescription);

av::gua::TexturedScreenSpaceQuadPassDescription::TexturedScreenSpaceQuadPassDescription(std::shared_ptr<::gua::TexturedScreenSpaceQuadPassDescription> const& guaTexturedScreenSpaceQuadPassDescription)
    : PipelinePassDescription(guaTexturedScreenSpaceQuadPassDescription), m_guaTexturedScreenSpaceQuadPassDescription(guaTexturedScreenSpaceQuadPassDescription)
{
}

void av::gua::TexturedScreenSpaceQuadPassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::TexturedScreenSpaceQuadPassDescription, true);

        SFTexturedScreenSpaceQuadPassDescription::initClass("av::gua::SFTexturedScreenSpaceQuadPassDescription", "av::Field");
        MFTexturedScreenSpaceQuadPassDescription::initClass("av::gua::MFTexturedScreenSpaceQuadPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr<::gua::TexturedScreenSpaceQuadPassDescription> const& av::gua::TexturedScreenSpaceQuadPassDescription::getGuaTexturedScreenSpaceQuadPassDescription() const
{
    return m_guaTexturedScreenSpaceQuadPassDescription;
}
