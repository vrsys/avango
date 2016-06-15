#include <avango/gua/renderer/BBoxPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::BBoxPassDescription"));
}

AV_FC_DEFINE(av::gua::BBoxPassDescription);

AV_FIELD_DEFINE(av::gua::SFBBoxPassDescription);
AV_FIELD_DEFINE(av::gua::MFBBoxPassDescription);

av::gua::BBoxPassDescription::BBoxPassDescription(
  std::shared_ptr< ::gua::BBoxPassDescription> const& guaBBoxPassDescription)
    : PipelinePassDescription(guaBBoxPassDescription)
    , m_guaBBoxPassDescription(guaBBoxPassDescription)
{}

void
av::gua::BBoxPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::BBoxPassDescription, true);

        SFBBoxPassDescription::initClass("av::gua::SFBBoxPassDescription", "av::Field");
        MFBBoxPassDescription::initClass("av::gua::MFBBoxPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

std::shared_ptr< ::gua::BBoxPassDescription> const&
av::gua::BBoxPassDescription::getGuaBBoxPassDescription() const
{
    return m_guaBBoxPassDescription;
}


