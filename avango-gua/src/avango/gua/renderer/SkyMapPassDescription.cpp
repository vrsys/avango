#include <avango/gua/renderer/SkyMapPassDescription.hpp>
#include <avango/Base.h>
#include <functional>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
av::Logger& logger(av::getLogger("av::gua::SkyMapPassDescription"));
}

AV_FC_DEFINE(av::gua::SkyMapPassDescription);

AV_FIELD_DEFINE(av::gua::SFSkyMapPassDescription);
AV_FIELD_DEFINE(av::gua::MFSkyMapPassDescription);

av::gua::SkyMapPassDescription::SkyMapPassDescription(std::shared_ptr<::gua::SkyMapPassDescription> const& guaSkyMapPassDescription)
    : PipelinePassDescription(guaSkyMapPassDescription), m_guaSkyMapPassDescription(guaSkyMapPassDescription)
{
    AV_FC_ADD_ADAPTOR_FIELD(OutputTextureName,
                            std::bind(&SkyMapPassDescription::getOutputTextureNameCB, this, std::placeholders::_1),
                            std::bind(&SkyMapPassDescription::setOutputTextureNameCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        LightColor, std::bind(&SkyMapPassDescription::getLightColorCB, this, std::placeholders::_1), std::bind(&SkyMapPassDescription::setLightColorCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        LightDirection, std::bind(&SkyMapPassDescription::getLightDirectionCB, this, std::placeholders::_1), std::bind(&SkyMapPassDescription::setLightDirectionCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        LightBrightness, std::bind(&SkyMapPassDescription::getLightBrightnessCB, this, std::placeholders::_1), std::bind(&SkyMapPassDescription::setLightBrightnessCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        GroundColor, std::bind(&SkyMapPassDescription::getGroundColorCB, this, std::placeholders::_1), std::bind(&SkyMapPassDescription::setGroundColorCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(
        RayleighFactor, std::bind(&SkyMapPassDescription::getRayleighFactorCB, this, std::placeholders::_1), std::bind(&SkyMapPassDescription::setRayleighFactorCB, this, std::placeholders::_1));

    AV_FC_ADD_ADAPTOR_FIELD(MieFactor, std::bind(&SkyMapPassDescription::getMieFactorCB, this, std::placeholders::_1), std::bind(&SkyMapPassDescription::setMieFactorCB, this, std::placeholders::_1));
}

void av::gua::SkyMapPassDescription::initClass()
{
    if(!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::SkyMapPassDescription, true);

        SFSkyMapPassDescription::initClass("av::gua::SFSkyMapPassDescription", "av::Field");
        MFSkyMapPassDescription::initClass("av::gua::MFSkyMapPassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

void av::gua::SkyMapPassDescription::getOutputTextureNameCB(const SFString::GetValueEvent& event) { *(event.getValuePtr()) = m_guaSkyMapPassDescription->output_texture_name(); }

void av::gua::SkyMapPassDescription::setOutputTextureNameCB(const SFString::SetValueEvent& event) { m_guaSkyMapPassDescription->output_texture_name(event.getValue()); }

void av::gua::SkyMapPassDescription::getLightColorCB(const SFColor::GetValueEvent& event) { *(event.getValuePtr()) = ::gua::utils::Color3f(m_guaSkyMapPassDescription->light_color()); }

void av::gua::SkyMapPassDescription::setLightColorCB(const SFColor::SetValueEvent& event) { m_guaSkyMapPassDescription->light_color(event.getValue().vec3f()); }

void av::gua::SkyMapPassDescription::getLightDirectionCB(const SFVec3::GetValueEvent& event) { *(event.getValuePtr()) = m_guaSkyMapPassDescription->light_direction(); }

void av::gua::SkyMapPassDescription::setLightDirectionCB(const SFVec3::SetValueEvent& event) { m_guaSkyMapPassDescription->light_direction(::gua::math::vec3f(event.getValue())); }

void av::gua::SkyMapPassDescription::getLightBrightnessCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaSkyMapPassDescription->light_brightness(); }

void av::gua::SkyMapPassDescription::setLightBrightnessCB(const SFFloat::SetValueEvent& event) { m_guaSkyMapPassDescription->light_brightness(event.getValue()); }

void av::gua::SkyMapPassDescription::getGroundColorCB(const SFColor::GetValueEvent& event) { *(event.getValuePtr()) = ::gua::utils::Color3f(m_guaSkyMapPassDescription->ground_color()); }

void av::gua::SkyMapPassDescription::setGroundColorCB(const SFColor::SetValueEvent& event) { m_guaSkyMapPassDescription->ground_color(event.getValue().vec3f()); }

void av::gua::SkyMapPassDescription::getRayleighFactorCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaSkyMapPassDescription->rayleigh_factor(); }

void av::gua::SkyMapPassDescription::setRayleighFactorCB(const SFFloat::SetValueEvent& event) { m_guaSkyMapPassDescription->rayleigh_factor(event.getValue()); }

void av::gua::SkyMapPassDescription::getMieFactorCB(const SFFloat::GetValueEvent& event) { *(event.getValuePtr()) = m_guaSkyMapPassDescription->mie_factor(); }

void av::gua::SkyMapPassDescription::setMieFactorCB(const SFFloat::SetValueEvent& event) { m_guaSkyMapPassDescription->mie_factor(event.getValue()); }
