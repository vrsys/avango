#include <avango/gua/renderer/PhysicallyBasedShadingPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::PhysicallyBasedShadingPassDescription"));
}

AV_FC_DEFINE(av::gua::PhysicallyBasedShadingPassDescription);

AV_FIELD_DEFINE(av::gua::SFPhysicallyBasedShadingPassDescription);
AV_FIELD_DEFINE(av::gua::MFPhysicallyBasedShadingPassDescription);

av::gua::PhysicallyBasedShadingPassDescription::PhysicallyBasedShadingPassDescription(std::shared_ptr<::gua::PhysicallyBasedShadingPassDescription> const& guaPhysicallyBasedShadingPassDescription)
    : PipelinePassDescription(guaPhysicallyBasedShadingPassDescription), m_guaPhysicallyBasedShadingPassDescription(guaPhysicallyBasedShadingPassDescription)
{
}

void av::gua::PhysicallyBasedShadingPassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::PhysicallyBasedShadingPassDescription, true);

        SFPhysicallyBasedShadingPassDescription::initClass("av::gua::SFPhysicallyBasedShadingPassDescription", "av::Field");
        MFPhysicallyBasedShadingPassDescription::initClass("av::gua::MFPhysicallyBasedShadingPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr<::gua::PhysicallyBasedShadingPassDescription> const& av::gua::PhysicallyBasedShadingPassDescription::getGuaPhysicallyBasedShadingPassDescription() const
{
    return m_guaPhysicallyBasedShadingPassDescription;
}
