#include <avango/gua/renderer/ResolvePassDescription.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

#include <avango/gua/Types.hpp>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::ResolvePassDescription"));
}

AV_FC_DEFINE(av::gua::ResolvePassDescription);

AV_FIELD_DEFINE(av::gua::SFResolvePassDescription);
AV_FIELD_DEFINE(av::gua::MFResolvePassDescription);

av::gua::ResolvePassDescription::ResolvePassDescription(
  std::shared_ptr< ::gua::ResolvePassDescription> const& guaResolvePassDescription)
    : PipelinePassDescription(guaResolvePassDescription)
    , m_guaResolvePassDescription(guaResolvePassDescription)
{
  AV_FC_ADD_ADAPTOR_FIELD(BackgroundColor,
                    boost::bind(&ResolvePassDescription::getBackgroundColorCB, this, _1),
                    boost::bind(&ResolvePassDescription::setBackgroundColorCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(BackgroundTexture,
                    boost::bind(&ResolvePassDescription::getBackgroundTextureCB, this, _1),
                    boost::bind(&ResolvePassDescription::setBackgroundTextureCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(BackgroundMode,
                    boost::bind(&ResolvePassDescription::getBackgroundModeCB, this, _1),
                    boost::bind(&ResolvePassDescription::setBackgroundModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableFog,
                    boost::bind(&ResolvePassDescription::getEnableFogCB, this, _1),
                    boost::bind(&ResolvePassDescription::setEnableFogCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(FogStart,
                    boost::bind(&ResolvePassDescription::getFogStartCB, this, _1),
                    boost::bind(&ResolvePassDescription::setFogStartCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(VignetteSoftness,
                    boost::bind(&ResolvePassDescription::getVignetteSoftnessCB, this, _1),
                    boost::bind(&ResolvePassDescription::setVignetteSoftnessCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(VignetteCoverage,
                    boost::bind(&ResolvePassDescription::getVignetteCoverageCB, this, _1),
                    boost::bind(&ResolvePassDescription::setVignetteCoverageCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(VignetteColor,
                    boost::bind(&ResolvePassDescription::getVignetteColorCB, this, _1),
                    boost::bind(&ResolvePassDescription::setVignetteColorCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(FogEnd,
                    boost::bind(&ResolvePassDescription::getFogEndCB, this, _1),
                    boost::bind(&ResolvePassDescription::setFogEndCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ToneMappingMode,
                    boost::bind(&ResolvePassDescription::getToneMappingModeCB, this, _1),
                    boost::bind(&ResolvePassDescription::setToneMappingModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(Exposure,
                    boost::bind(&ResolvePassDescription::getExposureCB, this, _1),
                    boost::bind(&ResolvePassDescription::setExposureCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EnvironmentLightingColor,
                    boost::bind(&ResolvePassDescription::getEnvironmentLightingColorCB, this, _1),
                    boost::bind(&ResolvePassDescription::setEnvironmentLightingColorCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EnvironmentLightingMode,
                    boost::bind(&ResolvePassDescription::getEnvironmentLightingModeCB, this, _1),
                    boost::bind(&ResolvePassDescription::setEnvironmentLightingModeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EnvironmentLightingSphereMap,
                    boost::bind(&ResolvePassDescription::getEnvironmentLightingSphereMapCB, this, _1),
                    boost::bind(&ResolvePassDescription::setEnvironmentLightingSphereMapCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(HorizonFade,
                    boost::bind(&ResolvePassDescription::getHorizonFadeCB, this, _1),
                    boost::bind(&ResolvePassDescription::setHorizonFadeCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableSSAO,
                    boost::bind(&ResolvePassDescription::getEnableSSAOCB, this, _1),
                    boost::bind(&ResolvePassDescription::setEnableSSAOCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(SSAORadius,
                    boost::bind(&ResolvePassDescription::getSSAORadiusCB, this, _1),
                    boost::bind(&ResolvePassDescription::setSSAORadiusCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(SSAOIntensity,
                    boost::bind(&ResolvePassDescription::getSSAOIntensityCB, this, _1),
                    boost::bind(&ResolvePassDescription::setSSAOIntensityCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(SSAOFalloff,
                    boost::bind(&ResolvePassDescription::getSSAOFalloffCB, this, _1),
                    boost::bind(&ResolvePassDescription::setSSAOFalloffCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(SSAONoiseTexture,
                    boost::bind(&ResolvePassDescription::getSSAONoiseTextureCB, this, _1),
                    boost::bind(&ResolvePassDescription::setSSAONoiseTextureCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableScreenSpaceShadow,
                    boost::bind(&ResolvePassDescription::getEnableScreenSpaceShadowCB, this, _1),
                    boost::bind(&ResolvePassDescription::setEnableScreenSpaceShadowCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ScreenSpaceShadowRadius,
                    boost::bind(&ResolvePassDescription::getScreenSpaceShadowRadiusCB, this, _1),
                    boost::bind(&ResolvePassDescription::setScreenSpaceShadowRadiusCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ScreenSpaceShadowMaxRadiusPX,
                    boost::bind(&ResolvePassDescription::getScreenSpaceShadowMaxRadiusPXCB, this, _1),
                    boost::bind(&ResolvePassDescription::setScreenSpaceShadowMaxRadiusPXCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(ScreenSpaceShadowIntensity,
                    boost::bind(&ResolvePassDescription::getScreenSpaceShadowIntensityCB, this, _1),
                    boost::bind(&ResolvePassDescription::setScreenSpaceShadowIntensityCB, this, _1));

  AV_FC_ADD_ADAPTOR_FIELD(DebugTiles,
                    boost::bind(&ResolvePassDescription::getDebugTilesCB, this, _1),
                    boost::bind(&ResolvePassDescription::setDebugTilesCB, this, _1));
}

void
av::gua::ResolvePassDescription::initClass()
{
    if (!isTypeInitialized())
    {
        av::gua::PipelinePassDescription::initClass();

        AV_FC_INIT(av::gua::PipelinePassDescription, av::gua::ResolvePassDescription, true);

        SFResolvePassDescription::initClass("av::gua::SFResolvePassDescription", "av::Field");
        MFResolvePassDescription::initClass("av::gua::MFResolvePassDescription", "av::Field");
        sClassTypeId.setDistributable(true);
    }
}

void
av::gua::ResolvePassDescription::getBackgroundColorCB(const SFColor::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->background_color();
}

void
av::gua::ResolvePassDescription::setBackgroundColorCB(const SFColor::SetValueEvent& event)
{
  m_guaResolvePassDescription->background_color(event.getValue());
}

void
av::gua::ResolvePassDescription::getBackgroundTextureCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->background_texture();
}

void
av::gua::ResolvePassDescription::setBackgroundTextureCB(const SFString::SetValueEvent& event)
{
  m_guaResolvePassDescription->background_texture(event.getValue());
}

void
av::gua::ResolvePassDescription::getBackgroundModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaResolvePassDescription->background_mode());
}

void
av::gua::ResolvePassDescription::setBackgroundModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaResolvePassDescription->background_mode(static_cast< ::gua::ResolvePassDescription::BackgroundMode>(event.getValue()));
}

void
av::gua::ResolvePassDescription::getEnableFogCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->enable_fog();
}

void
av::gua::ResolvePassDescription::setEnableFogCB(const SFBool::SetValueEvent& event)
{
  m_guaResolvePassDescription->enable_fog(event.getValue());
}

void
av::gua::ResolvePassDescription::getFogStartCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->fog_start();
}

void
av::gua::ResolvePassDescription::setFogStartCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->fog_start(event.getValue());
}

void
av::gua::ResolvePassDescription::getFogEndCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->fog_end();
}

void
av::gua::ResolvePassDescription::setFogEndCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->fog_end(event.getValue());
}

void
av::gua::ResolvePassDescription::getVignetteSoftnessCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->vignette_softness();
}

void
av::gua::ResolvePassDescription::setVignetteSoftnessCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->vignette_softness(event.getValue());
}

void
av::gua::ResolvePassDescription::getVignetteCoverageCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->vignette_coverage();
}

void
av::gua::ResolvePassDescription::setVignetteCoverageCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->vignette_coverage(event.getValue());
}

void
av::gua::ResolvePassDescription::getVignetteColorCB(const SFVec4::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->vignette_color();
}

void
av::gua::ResolvePassDescription::setVignetteColorCB(const SFVec4::SetValueEvent& event)
{
  m_guaResolvePassDescription->vignette_color(::gua::math::vec4f(event.getValue()));
}

void
av::gua::ResolvePassDescription::getToneMappingModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaResolvePassDescription->tone_mapping_method());
}

void
av::gua::ResolvePassDescription::setToneMappingModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaResolvePassDescription->tone_mapping_method(static_cast< ::gua::ResolvePassDescription::ToneMappingMethod>(event.getValue()));
}

void
av::gua::ResolvePassDescription::getExposureCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->tone_mapping_exposure();
}

void
av::gua::ResolvePassDescription::setExposureCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->tone_mapping_exposure(event.getValue());
}

std::shared_ptr< ::gua::ResolvePassDescription> const&
av::gua::ResolvePassDescription::getGuaResolvePassDescription() const
{
    return m_guaResolvePassDescription;
}

void
av::gua::ResolvePassDescription::getEnvironmentLightingColorCB(const SFColor::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->environment_lighting();
}

void
av::gua::ResolvePassDescription::setEnvironmentLightingColorCB(const SFColor::SetValueEvent& event)
{
  m_guaResolvePassDescription->environment_lighting(event.getValue());
}

void
av::gua::ResolvePassDescription::getEnvironmentLightingModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaResolvePassDescription->environment_lighting_mode());
}

void
av::gua::ResolvePassDescription::setEnvironmentLightingModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaResolvePassDescription->environment_lighting_mode(static_cast< ::gua::ResolvePassDescription::EnvironmentLightingMode>(event.getValue()));
}

void
av::gua::ResolvePassDescription::getEnvironmentLightingSphereMapCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->environment_lighting_spheremap();
}

void
av::gua::ResolvePassDescription::setEnvironmentLightingSphereMapCB(const SFString::SetValueEvent& event)
{
  m_guaResolvePassDescription->environment_lighting_spheremap(event.getValue());
}

void
av::gua::ResolvePassDescription::getHorizonFadeCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->horizon_fade();
}

void
av::gua::ResolvePassDescription::setHorizonFadeCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->horizon_fade(event.getValue());
}

void
av::gua::ResolvePassDescription::getEnableSSAOCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->ssao_enable();
}

void
av::gua::ResolvePassDescription::setEnableSSAOCB(const SFBool::SetValueEvent& event)
{
  m_guaResolvePassDescription->ssao_enable(event.getValue());
}

void
av::gua::ResolvePassDescription::getSSAORadiusCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->ssao_radius();
}

void
av::gua::ResolvePassDescription::setSSAORadiusCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->ssao_radius(event.getValue());
}

void
av::gua::ResolvePassDescription::getSSAOIntensityCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->ssao_intensity();
}

void
av::gua::ResolvePassDescription::setSSAOIntensityCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->ssao_intensity(event.getValue());
}

void
av::gua::ResolvePassDescription::getSSAOFalloffCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->ssao_falloff();
}

void
av::gua::ResolvePassDescription::setSSAOFalloffCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->ssao_falloff(event.getValue());
}

void
av::gua::ResolvePassDescription::getSSAONoiseTextureCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->ssao_noise_texture();
}

void
av::gua::ResolvePassDescription::setSSAONoiseTextureCB(const SFString::SetValueEvent& event)
{
  m_guaResolvePassDescription->ssao_noise_texture(event.getValue());
}

void
av::gua::ResolvePassDescription::getDebugTilesCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->debug_tiles();
}

void
av::gua::ResolvePassDescription::setDebugTilesCB(const SFBool::SetValueEvent& event)
{
  m_guaResolvePassDescription->debug_tiles(event.getValue());
}

void
av::gua::ResolvePassDescription::getEnableScreenSpaceShadowCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->screen_space_shadows();
}

void
av::gua::ResolvePassDescription::setEnableScreenSpaceShadowCB(const SFBool::SetValueEvent& event)
{
  m_guaResolvePassDescription->screen_space_shadows(event.getValue());
}

void
av::gua::ResolvePassDescription::getScreenSpaceShadowRadiusCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->screen_space_shadow_radius();
}

void
av::gua::ResolvePassDescription::setScreenSpaceShadowRadiusCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->screen_space_shadow_radius(event.getValue());
}

void
av::gua::ResolvePassDescription::getScreenSpaceShadowMaxRadiusPXCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->screen_space_shadow_max_radius_px();
}

void
av::gua::ResolvePassDescription::setScreenSpaceShadowMaxRadiusPXCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->screen_space_shadow_max_radius_px(event.getValue());
}

void
av::gua::ResolvePassDescription::getScreenSpaceShadowIntensityCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->screen_space_shadow_intensity();
}

void
av::gua::ResolvePassDescription::setScreenSpaceShadowIntensityCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->screen_space_shadow_intensity(event.getValue());
}
