#include <avango/gua/renderer/DeferredVirtualTexturingPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::DeferredVirtualTexturingPassDescription"));
}

AV_FC_DEFINE(av::gua::DeferredVirtualTexturingPassDescription);

AV_FIELD_DEFINE(av::gua::SFDeferredVirtualTexturingPassDescription);
AV_FIELD_DEFINE(av::gua::MFDeferredVirtualTexturingPassDescription);

av::gua::DeferredVirtualTexturingPassDescription::DeferredVirtualTexturingPassDescription(
  std::shared_ptr< ::gua::DeferredVirtualTexturingPassDescription> const& guaDeferredVirtualTexturingPassDescription)
    : PipelinePassDescription(guaDeferredVirtualTexturingPassDescription)
    , m_guaDeferredVirtualTexturingPassDescription(guaDeferredVirtualTexturingPassDescription)
{

}

void
av::gua::DeferredVirtualTexturingPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::DeferredVirtualTexturingPassDescription, true);

        SFDeferredVirtualTexturingPassDescription::initClass("av::gua::SFDeferredVirtualTexturingPassDescription", "av::Field");
        MFDeferredVirtualTexturingPassDescription::initClass("av::gua::MFDeferredVirtualTexturingPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}


std::shared_ptr< ::gua::DeferredVirtualTexturingPassDescription> const&
av::gua::DeferredVirtualTexturingPassDescription::getGuaDeferredVirtualTexturingPassDescription() const
{
    return m_guaDeferredVirtualTexturingPassDescription;
}
