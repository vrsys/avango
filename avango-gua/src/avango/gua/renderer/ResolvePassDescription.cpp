#include <avango/gua/renderer/ResolvePassDescription.hpp>
#include <avango/Base.h>
#include <functional>
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
                    std::bind(&ResolvePassDescription::getBackgroundColorCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setBackgroundColorCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(BackgroundTexture,
                    std::bind(&ResolvePassDescription::getBackgroundTextureCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setBackgroundTextureCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(AlternativeBackgroundTexture,
                    std::bind(&ResolvePassDescription::getAlternativeBackgroundTextureCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setAlternativeBackgroundTextureCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(BackgroundTextureBlendFactor,
                    std::bind(&ResolvePassDescription::getBackgroundTextureBlendFactorCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setBackgroundTextureBlendFactorCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(BackgroundMode,
                    std::bind(&ResolvePassDescription::getBackgroundModeCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setBackgroundModeCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableFog,
                    std::bind(&ResolvePassDescription::getEnableFogCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setEnableFogCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(FogStart,
                    std::bind(&ResolvePassDescription::getFogStartCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setFogStartCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(VignetteSoftness,
                    std::bind(&ResolvePassDescription::getVignetteSoftnessCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setVignetteSoftnessCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(VignetteCoverage,
                    std::bind(&ResolvePassDescription::getVignetteCoverageCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setVignetteCoverageCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(VignetteColor,
                    std::bind(&ResolvePassDescription::getVignetteColorCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setVignetteColorCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(FogEnd,
                    std::bind(&ResolvePassDescription::getFogEndCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setFogEndCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ToneMappingMode,
                    std::bind(&ResolvePassDescription::getToneMappingModeCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setToneMappingModeCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(Exposure,
                    std::bind(&ResolvePassDescription::getExposureCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setExposureCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(EnvironmentLightingColor,
                    std::bind(&ResolvePassDescription::getEnvironmentLightingColorCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setEnvironmentLightingColorCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(EnvironmentLightingMode,
                    std::bind(&ResolvePassDescription::getEnvironmentLightingModeCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setEnvironmentLightingModeCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(EnvironmentLightingTexture,
                    std::bind(&ResolvePassDescription::getEnvironmentLightingTextureCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setEnvironmentLightingTextureCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(AlternativeEnvironmentLightingTexture,
                    std::bind(&ResolvePassDescription::getAlternativeEnvironmentLightingTextureCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setAlternativeEnvironmentLightingTextureCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(EnvironmentLightingTextureBlendFactor,
                    std::bind(&ResolvePassDescription::getEnvironmentLightingTextureBlendFactorCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setEnvironmentLightingTextureBlendFactorCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(HorizonFade,
                    std::bind(&ResolvePassDescription::getHorizonFadeCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setHorizonFadeCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableSSAO,
                    std::bind(&ResolvePassDescription::getEnableSSAOCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setEnableSSAOCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(SSAORadius,
                    std::bind(&ResolvePassDescription::getSSAORadiusCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setSSAORadiusCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(SSAOIntensity,
                    std::bind(&ResolvePassDescription::getSSAOIntensityCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setSSAOIntensityCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(SSAOFalloff,
                    std::bind(&ResolvePassDescription::getSSAOFalloffCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setSSAOFalloffCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(SSAONoiseTexture,
                    std::bind(&ResolvePassDescription::getSSAONoiseTextureCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setSSAONoiseTextureCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(EnableScreenSpaceShadow,
                    std::bind(&ResolvePassDescription::getEnableScreenSpaceShadowCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setEnableScreenSpaceShadowCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ScreenSpaceShadowRadius,
                    std::bind(&ResolvePassDescription::getScreenSpaceShadowRadiusCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setScreenSpaceShadowRadiusCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ScreenSpaceShadowMaxRadiusPX,
                    std::bind(&ResolvePassDescription::getScreenSpaceShadowMaxRadiusPXCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setScreenSpaceShadowMaxRadiusPXCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(ScreenSpaceShadowIntensity,
                    std::bind(&ResolvePassDescription::getScreenSpaceShadowIntensityCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setScreenSpaceShadowIntensityCB, this,std::placeholders::_1));

  AV_FC_ADD_ADAPTOR_FIELD(DebugTiles,
                    std::bind(&ResolvePassDescription::getDebugTilesCB, this,std::placeholders::_1),
                    std::bind(&ResolvePassDescription::setDebugTilesCB, this,std::placeholders::_1));
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
av::gua::ResolvePassDescription::getAlternativeBackgroundTextureCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->alternative_background_texture();
}

void
av::gua::ResolvePassDescription::setAlternativeBackgroundTextureCB(const SFString::SetValueEvent& event)
{
  m_guaResolvePassDescription->alternative_background_texture(event.getValue());
}

void
av::gua::ResolvePassDescription::getBackgroundTextureBlendFactorCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->background_texture_blend_factor();
}

void
av::gua::ResolvePassDescription::setBackgroundTextureBlendFactorCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->background_texture_blend_factor(event.getValue());
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
av::gua::ResolvePassDescription::getEnvironmentLightingTextureCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->environment_lighting_texture();
}

void
av::gua::ResolvePassDescription::setEnvironmentLightingTextureCB(const SFString::SetValueEvent& event)
{
  m_guaResolvePassDescription->environment_lighting_texture(event.getValue());
}

void
av::gua::ResolvePassDescription::getAlternativeEnvironmentLightingTextureCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->alternative_environment_lighting_texture();
}

void
av::gua::ResolvePassDescription::setAlternativeEnvironmentLightingTextureCB(const SFString::SetValueEvent& event)
{
  m_guaResolvePassDescription->alternative_environment_lighting_texture(event.getValue());
}

void
av::gua::ResolvePassDescription::getEnvironmentLightingTextureBlendFactorCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaResolvePassDescription->environment_lighting_texture_blend_factor();
}

void
av::gua::ResolvePassDescription::setEnvironmentLightingTextureBlendFactorCB(const SFFloat::SetValueEvent& event)
{
  m_guaResolvePassDescription->environment_lighting_texture_blend_factor(event.getValue());
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
