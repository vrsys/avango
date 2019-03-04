#include <avango/gua/lod/renderer/PLodPassDescription.hpp>
#include <avango/Base.h>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::lod::PLodPassDescription"));
}

AV_FC_DEFINE(av::gua::lod::PLodPassDescription);

AV_FIELD_DEFINE(av::gua::lod::SFPLodPassDescription);
AV_FIELD_DEFINE(av::gua::lod::MFPLodPassDescription);

av::gua::lod::PLodPassDescription::PLodPassDescription(std::shared_ptr<::gua::PLodPassDescription> const& guaPLodPassDescription)
    : PipelinePassDescription(guaPLodPassDescription), m_guaPLodPassDescription(guaPLodPassDescription)
{
    AV_FC_ADD_ADAPTOR_FIELD(
        SurfelRenderMode, std::bind(&PLodPassDescription::getSurfelRenderModeCB, this, std::placeholders::_1), std::bind(&PLodPassDescription::setSurfelRenderModeCB, this, std::placeholders::_1));
}

void av::gua::lod::PLodPassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::lod::PLodPassDescription, true);

        SFPLodPassDescription::initClass("av::gua::lod::SFPLodPassDescription", "av::Field");
        MFPLodPassDescription::initClass("av::gua::lod::MFPLodPassDescription", "av::Field");
    }
}

std::shared_ptr<::gua::PLodPassDescription> const& av::gua::lod::PLodPassDescription::getGuaPLodPassDescription() const { return m_guaPLodPassDescription; }

void av::gua::lod::PLodPassDescription::getSurfelRenderModeCB(const SFInt::GetValueEvent& event) { *(event.getValuePtr()) = (int)m_guaPLodPassDescription->mode(); }

void av::gua::lod::PLodPassDescription::setSurfelRenderModeCB(const SFInt::SetValueEvent& event)
{
    m_guaPLodPassDescription->mode((::gua::PLodPassDescription::SurfelRenderMode)event.getValue());
    m_guaPLodPassDescription->touch();
}
