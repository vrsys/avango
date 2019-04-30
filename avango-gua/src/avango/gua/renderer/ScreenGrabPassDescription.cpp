#include <avango/gua/renderer/ScreenGrabPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::ScreenGrabPassDescription"));
}

AV_FC_DEFINE(av::gua::ScreenGrabPassDescription);

AV_FIELD_DEFINE(av::gua::SFScreenGrabPassDescription);
AV_FIELD_DEFINE(av::gua::MFScreenGrabPassDescription);

av::gua::ScreenGrabPassDescription::ScreenGrabPassDescription(std::shared_ptr<::gua::ScreenGrabPassDescription> const& guaScreenGrabPassDescription)
    : PipelinePassDescription(guaScreenGrabPassDescription), m_guaScreenGrabPassDescription(guaScreenGrabPassDescription)
{
}

void av::gua::ScreenGrabPassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::ScreenGrabPassDescription, true);

        SFScreenGrabPassDescription::initClass("av::gua::SFScreenGrabPassDescription", "av::Field");
        MFScreenGrabPassDescription::initClass("av::gua::MFScreenGrabPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr<::gua::ScreenGrabPassDescription> const& av::gua::ScreenGrabPassDescription::getGuaScreenGrabPassDescription() const { return m_guaScreenGrabPassDescription; }
