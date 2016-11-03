#include <avango/gua/lod/renderer/MLodPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::lod::MLodPassDescription"));
}

AV_FC_DEFINE(av::gua::lod::MLodPassDescription);

AV_FIELD_DEFINE(av::gua::lod::SFMLodPassDescription);
AV_FIELD_DEFINE(av::gua::lod::MFMLodPassDescription);

av::gua::lod::MLodPassDescription::MLodPassDescription(
  std::shared_ptr< ::gua::MLodPassDescription> const& guaMLodPassDescription)
    : PipelinePassDescription(guaMLodPassDescription)
    , m_guaMLodPassDescription(guaMLodPassDescription)
{

}

void
av::gua::lod::MLodPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::lod::MLodPassDescription, true);

        SFMLodPassDescription::initClass("av::gua::lod::SFMLodPassDescription", "av::Field");
        MFMLodPassDescription::initClass("av::gua::lod::MFMLodPassDescription", "av::Field");
    }
}


std::shared_ptr< ::gua::MLodPassDescription> const&
av::gua::lod::MLodPassDescription::getGuaMLodPassDescription() const
{
    return m_guaMLodPassDescription;
}


