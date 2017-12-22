#include <avango/gua/renderer/SPointsPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::SPointsPassDescription"));
}

AV_FC_DEFINE(av::gua::SPointsPassDescription);

AV_FIELD_DEFINE(av::gua::SFSPointsPassDescription);
AV_FIELD_DEFINE(av::gua::MFSPointsPassDescription);

av::gua::SPointsPassDescription::SPointsPassDescription(
  std::shared_ptr< ::gua::SPointsPassDescription> const& guaSPointsPassDescription)
    : PipelinePassDescription(guaSPointsPassDescription)
    , m_guaSPointsPassDescription(guaSPointsPassDescription)
{

}

void
av::gua::SPointsPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::SPointsPassDescription, true);

        SFSPointsPassDescription::initClass("av::gua::SFSPointsPassDescription", "av::Field");
        MFSPointsPassDescription::initClass("av::gua::MFSPointsPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}


std::shared_ptr< ::gua::SPointsPassDescription> const&
av::gua::SPointsPassDescription::getGuaSPointsPassDescription() const
{
    return m_guaSPointsPassDescription;
}
