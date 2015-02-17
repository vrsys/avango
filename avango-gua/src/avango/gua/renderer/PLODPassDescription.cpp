#include <avango/gua/renderer/PLODPassDescription.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::PLODPassDescription"));
}

AV_FC_DEFINE(av::gua::PLODPassDescription);

AV_FIELD_DEFINE(av::gua::SFPLODPassDescription);
AV_FIELD_DEFINE(av::gua::MFPLODPassDescription);

av::gua::PLODPassDescription::PLODPassDescription(
  std::shared_ptr< ::gua::PLODPassDescription> const& guaPLODPassDescription)
    : PipelinePassDescription(guaPLODPassDescription)
    , m_guaPLODPassDescription(guaPLODPassDescription)
{

}

void
av::gua::PLODPassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::PLODPassDescription, true);

        SFPLODPassDescription::initClass("av::gua::SFPLODPassDescription", "av::Field");
        MFPLODPassDescription::initClass("av::gua::MFPLODPassDescription", "av::Field");
    }
}


std::shared_ptr< ::gua::PLODPassDescription> const&
av::gua::PLODPassDescription::getGuaPLODPassDescription() const
{
    return m_guaPLODPassDescription;
}


