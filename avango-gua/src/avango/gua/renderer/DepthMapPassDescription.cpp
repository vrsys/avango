#include <avango/gua/renderer/DepthMapPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::DepthMapPassDescription"));
}

AV_FC_DEFINE(av::gua::DepthMapPassDescription);

AV_FIELD_DEFINE(av::gua::SFDepthMapPassDescription);
AV_FIELD_DEFINE(av::gua::MFDepthMapPassDescription);

av::gua::DepthMapPassDescription::DepthMapPassDescription(std::shared_ptr<::gua::DepthCubeMapPassDesciption> const& guaDepthCubeMapPassDescription)
    : PipelinePassDescription(guaDepthCubeMapPassDescription), m_guaDepthMapPassDescription(guaDepthCubeMapPassDescription)
{
}

void av::gua::DepthMapPassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::DepthMapPassDescription, true);

        SFDepthMapPassDescription::initClass("av::gua::SFDepthMapPassDescription", "av::Field");
        MFDepthMapPassDescription::initClass("av::gua::MFDepthMapPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr<::gua::DepthCubeMapPassDesciption> const& av::gua::DepthMapPassDescription::getGuaDepthMapPassDescription() const { return m_guaDepthMapPassDescription; }
