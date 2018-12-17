#include <avango/gua/renderer/DynamicTrianglePassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::DynamicTrianglePassDescription"));
}

AV_FC_DEFINE(av::gua::DynamicTrianglePassDescription);

AV_FIELD_DEFINE(av::gua::SFDynamicTrianglePassDescription);
AV_FIELD_DEFINE(av::gua::MFDynamicTrianglePassDescription);

av::gua::DynamicTrianglePassDescription::DynamicTrianglePassDescription(
  std::shared_ptr< ::gua::DynamicTrianglePassDescription> const& guaDynamicTrianglePassDescription)
    : PipelinePassDescription(guaDynamicTrianglePassDescription)
    , m_guaDynamicTrianglePassDescription(guaDynamicTrianglePassDescription)
{

}

void
av::gua::DynamicTrianglePassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::DynamicTrianglePassDescription, true);

        SFDynamicTrianglePassDescription::initClass("av::gua::SFDynamicTrianglePassDescription", "av::Field");
        MFDynamicTrianglePassDescription::initClass("av::gua::MFDynamicTrianglePassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}


std::shared_ptr< ::gua::DynamicTrianglePassDescription> const&
av::gua::DynamicTrianglePassDescription::getGuaDynamicTrianglePassDescription() const
{
    return m_guaDynamicTrianglePassDescription;
}


