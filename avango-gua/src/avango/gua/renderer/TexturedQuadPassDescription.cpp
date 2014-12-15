#include <avango/gua/renderer/TexturedQuadPassDescription.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::TexturedQuadPassDescription"));
}

AV_FC_DEFINE(av::gua::TexturedQuadPassDescription);

AV_FIELD_DEFINE(av::gua::SFTexturedQuadPassDescription);
AV_FIELD_DEFINE(av::gua::MFTexturedQuadPassDescription);

av::gua::TexturedQuadPassDescription::TexturedQuadPassDescription(
  std::shared_ptr< ::gua::TexturedQuadPassDescription> const& guaTexturedQuadPassDescription)
    : PipelinePassDescription(guaTexturedQuadPassDescription)
    , m_guaTexturedQuadPassDescription(guaTexturedQuadPassDescription)
{

}

void
av::gua::TexturedQuadPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::TexturedQuadPassDescription, true);

        SFTexturedQuadPassDescription::initClass("av::gua::SFTexturedQuadPassDescription", "av::Field");
        MFTexturedQuadPassDescription::initClass("av::gua::MFTexturedQuadPassDescription", "av::Field");
    }
}


std::shared_ptr< ::gua::TexturedQuadPassDescription> const&
av::gua::TexturedQuadPassDescription::getGuaTexturedQuadPassDescription() const
{
    return m_guaTexturedQuadPassDescription;
}


