#include <avango/gua/renderer/LineStripPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::LineStripPassDescription"));
}

AV_FC_DEFINE(av::gua::LineStripPassDescription);

AV_FIELD_DEFINE(av::gua::SFLineStripPassDescription);
AV_FIELD_DEFINE(av::gua::MFLineStripPassDescription);

av::gua::LineStripPassDescription::LineStripPassDescription(
  std::shared_ptr< ::gua::LineStripPassDescription> const& guaLineStripPassDescription)
    : PipelinePassDescription(guaLineStripPassDescription)
    , m_guaLineStripPassDescription(guaLineStripPassDescription)
{

}

void
av::gua::LineStripPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::LineStripPassDescription, true);

        SFLineStripPassDescription::initClass("av::gua::SFLineStripPassDescription", "av::Field");
        MFLineStripPassDescription::initClass("av::gua::MFLineStripPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}


std::shared_ptr< ::gua::LineStripPassDescription> const&
av::gua::LineStripPassDescription::getGuaLineStripPassDescription() const
{
    return m_guaLineStripPassDescription;
}


