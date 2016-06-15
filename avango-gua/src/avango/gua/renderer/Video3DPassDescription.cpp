#include <avango/gua/renderer/Video3DPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Video3DPassDescription"));
}

AV_FC_DEFINE(av::gua::Video3DPassDescription);

AV_FIELD_DEFINE(av::gua::SFVideo3DPassDescription);
AV_FIELD_DEFINE(av::gua::MFVideo3DPassDescription);

av::gua::Video3DPassDescription::Video3DPassDescription(
  std::shared_ptr< ::gua::Video3DPassDescription> const& guaVideo3DPassDescription)
    : PipelinePassDescription(guaVideo3DPassDescription)
    , m_guaVideo3DPassDescription(guaVideo3DPassDescription)
{

}

void
av::gua::Video3DPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::Video3DPassDescription, true);

        SFVideo3DPassDescription::initClass("av::gua::SFVideo3DPassDescription", "av::Field");
        MFVideo3DPassDescription::initClass("av::gua::MFVideo3DPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}


std::shared_ptr< ::gua::Video3DPassDescription> const&
av::gua::Video3DPassDescription::getGuaVideo3DPassDescription() const
{
    return m_guaVideo3DPassDescription;
}
