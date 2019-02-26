#include <avango/gua/renderer/SSAOPassDescription.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::SSAOPassDescription"));
}

AV_FC_DEFINE(av::gua::SSAOPassDescription);

AV_FIELD_DEFINE(av::gua::SFSSAOPassDescription);
AV_FIELD_DEFINE(av::gua::MFSSAOPassDescription);

av::gua::SSAOPassDescription::SSAOPassDescription(std::shared_ptr<::gua::SSAOPassDescription> const& guaSSAOPassDescription)
    : PipelinePassDescription(guaSSAOPassDescription), m_guaSSAOPassDescription(guaSSAOPassDescription)
{
    AV_FC_ADD_ADAPTOR_FIELD(Radius, std::bind(&SSAOPassDescription::getRadiusCB, this, std::placeholders::_1), std::bind(&SSAOPassDescription::setRadiusCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Intensity, std::bind(&SSAOPassDescription::getIntensityCB, this, std::placeholders::_1), std::bind(&SSAOPassDescription::setIntensityCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(Falloff, std::bind(&SSAOPassDescription::getFalloffCB, this, std::placeholders::_1), std::bind(&SSAOPassDescription::setFalloffCB, this, std::placeholders::_1));
}

void av::gua::SSAOPassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::SSAOPassDescription, true);

        SFSSAOPassDescription::initClass("av::gua::SFSSAOPassDescription", "av::Field");
        MFSSAOPassDescription::initClass("av::gua::MFSSAOPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

void av::gua::SSAOPassDescription::getRadiusCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaSSAOPassDescription->radius(); }

void av::gua::SSAOPassDescription::setRadiusCB(const SFFloat::SetValueEvent& event) { m_guaSSAOPassDescription->radius(event.getValue()); }

void av::gua::SSAOPassDescription::getIntensityCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaSSAOPassDescription->intensity(); }

void av::gua::SSAOPassDescription::setIntensityCB(const SFFloat::SetValueEvent& event) { m_guaSSAOPassDescription->intensity(event.getValue()); }

void av::gua::SSAOPassDescription::getFalloffCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaSSAOPassDescription->falloff(); }

void av::gua::SSAOPassDescription::setFalloffCB(const SFFloat::SetValueEvent& event) { m_guaSSAOPassDescription->falloff(event.getValue()); }

std::shared_ptr<::gua::SSAOPassDescription> const& av::gua::SSAOPassDescription::getGuaSSAOPassDescription() const { return m_guaSSAOPassDescription; }
