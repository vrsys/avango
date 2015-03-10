#include <avango/gua/renderer/ScreenshotPassDescription.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::ScreenshotPassDescription"));
}

AV_FC_DEFINE(av::gua::ScreenshotPassDescription);

AV_FIELD_DEFINE(av::gua::SFScreenshotPassDescription);
AV_FIELD_DEFINE(av::gua::MFScreenshotPassDescription);

av::gua::ScreenshotPassDescription::ScreenshotPassDescription(
  std::shared_ptr< ::gua::ScreenshotPassDescription> const& guaScreenshotPassDescription)
    : PipelinePassDescription(guaScreenshotPassDescription)
    , m_guaScreenshotPassDescription(guaScreenshotPassDescription)
{
}

void
av::gua::ScreenshotPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::ScreenshotPassDescription, true);

        SFScreenshotPassDescription::initClass("av::gua::SFScreenshotPassDescription", "av::Field");
        MFScreenshotPassDescription::initClass("av::gua::MFScreenshotPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::ScreenshotPassDescription> const&
av::gua::ScreenshotPassDescription::getGuaScreenshotPassDescription() const
{
    return m_guaScreenshotPassDescription;
}
