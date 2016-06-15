#include <avango/gua/renderer/StencilPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::StencilPassDescription"));
}

AV_FC_DEFINE(av::gua::StencilPassDescription);

AV_FIELD_DEFINE(av::gua::SFStencilPassDescription);
AV_FIELD_DEFINE(av::gua::MFStencilPassDescription);

av::gua::StencilPassDescription::StencilPassDescription(
  std::shared_ptr< ::gua::StencilPassDescription> const& guaStencilPassDescription)
    : PipelinePassDescription(guaStencilPassDescription)
    , m_guaStencilPassDescription(guaStencilPassDescription)
{

}

void
av::gua::StencilPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::StencilPassDescription, true);

        SFStencilPassDescription::initClass("av::gua::SFStencilPassDescription", "av::Field");
        MFStencilPassDescription::initClass("av::gua::MFStencilPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}


std::shared_ptr< ::gua::StencilPassDescription> const&
av::gua::StencilPassDescription::getGuaStencilPassDescription() const
{
    return m_guaStencilPassDescription;
}


