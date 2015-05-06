#ifndef AVANGO_GUA_RESOLVE_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_RESOLVE_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/ResolvePass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::ResolvePassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL ResolvePassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      enum BackgroundModeEnum {
        COLOR = static_cast<unsigned>(::gua::ResolvePassDescription::BackgroundMode::COLOR),
        SKYMAP_TEXTURE = static_cast<unsigned>(::gua::ResolvePassDescription::BackgroundMode::SKYMAP_TEXTURE),
        QUAD_TEXTURE = static_cast<unsigned>(::gua::ResolvePassDescription::BackgroundMode::QUAD_TEXTURE),
        CUBEMAP_TEXTURE = static_cast<unsigned>(::gua::ResolvePassDescription::BackgroundMode::CUBEMAP)
      };

      enum ToneMappingModeEnum {
        LINEAR = static_cast<unsigned>(::gua::ResolvePassDescription::ToneMappingMethod::LINEAR),
        HEJL = static_cast<unsigned>(::gua::ResolvePassDescription::ToneMappingMethod::HEJL),
        REINHARD = static_cast<unsigned>(::gua::ResolvePassDescription::ToneMappingMethod::REINHARD),
        UNCHARTED = static_cast<unsigned>(::gua::ResolvePassDescription::ToneMappingMethod::UNCHARTED)
      };

      enum EnvironmentLightingModeEnum {
        SPHEREMAP = static_cast<unsigned>(::gua::ResolvePassDescription::EnvironmentLightingMode::SPHEREMAP),
        CUBEMAP = static_cast<unsigned>(::gua::ResolvePassDescription::EnvironmentLightingMode::CUBEMAP),
        AMBIENT_COLOR = static_cast<unsigned>(::gua::ResolvePassDescription::EnvironmentLightingMode::AMBIENT_COLOR)
      };

      /**
       * Constructor. When called without arguments, a new ::gua::ResolvePassDescription is created.
       * Otherwise, the given ::gua::ResolvePassDescription is used.
       */
      ResolvePassDescription(std::shared_ptr< ::gua::ResolvePassDescription> const& ResolvePassDescription =
                                std::shared_ptr< ::gua::ResolvePassDescription>(new ::gua::ResolvePassDescription()) );


    public:

      SFColor  BackgroundColor;
      SFString BackgroundTexture;
      SFString AlternativeBackgroundTexture;
      SFFloat  BackgroundTextureBlendFactor;
      SFUInt   BackgroundMode;

      SFBool   EnableFog;
      SFFloat  FogStart;
      SFFloat  FogEnd;

      SFFloat  VignetteSoftness;
      SFFloat  VignetteCoverage;
      SFVec4   VignetteColor;

      SFUInt   ToneMappingMode;
      SFFloat  Exposure;

      SFColor  EnvironmentLightingColor;
      SFUInt   EnvironmentLightingMode;
      SFString EnvironmentLightingTexture;
      SFString AlternativeEnvironmentLightingTexture;
      SFFloat  EnvironmentLightingTextureBlendFactor;
      SFFloat  HorizonFade;

      SFBool   EnableSSAO;
      SFFloat  SSAORadius;
      SFFloat  SSAOIntensity;
      SFFloat  SSAOFalloff;
      SFString SSAONoiseTexture;

      SFBool   DebugTiles;

      SFBool   EnableScreenSpaceShadow;
      SFFloat  ScreenSpaceShadowRadius;
      SFFloat  ScreenSpaceShadowMaxRadiusPX;
      SFFloat  ScreenSpaceShadowIntensity;

      virtual void getBackgroundColorCB(const SFColor::GetValueEvent& event);
      virtual void setBackgroundColorCB(const SFColor::SetValueEvent& event);

      virtual void getBackgroundTextureCB(const SFString::GetValueEvent& event);
      virtual void setBackgroundTextureCB(const SFString::SetValueEvent& event);

      virtual void getAlternativeBackgroundTextureCB(const SFString::GetValueEvent& event);
      virtual void setAlternativeBackgroundTextureCB(const SFString::SetValueEvent& event);

      virtual void getBackgroundTextureBlendFactorCB(const SFFloat::GetValueEvent& event);
      virtual void setBackgroundTextureBlendFactorCB(const SFFloat::SetValueEvent& event);

      virtual void getBackgroundModeCB(const SFUInt::GetValueEvent& event);
      virtual void setBackgroundModeCB(const SFUInt::SetValueEvent& event);

      virtual void getEnableFogCB(const SFBool::GetValueEvent& event);
      virtual void setEnableFogCB(const SFBool::SetValueEvent& event);

      virtual void getFogStartCB(const SFFloat::GetValueEvent& event);
      virtual void setFogStartCB(const SFFloat::SetValueEvent& event);

      virtual void getFogEndCB(const SFFloat::GetValueEvent& event);
      virtual void setFogEndCB(const SFFloat::SetValueEvent& event);

      virtual void getVignetteSoftnessCB(const SFFloat::GetValueEvent& event);
      virtual void setVignetteSoftnessCB(const SFFloat::SetValueEvent& event);

      virtual void getVignetteCoverageCB(const SFFloat::GetValueEvent& event);
      virtual void setVignetteCoverageCB(const SFFloat::SetValueEvent& event);

      virtual void getVignetteColorCB(const SFVec4::GetValueEvent& event);
      virtual void setVignetteColorCB(const SFVec4::SetValueEvent& event);

      virtual void getToneMappingModeCB(const SFUInt::GetValueEvent& event);
      virtual void setToneMappingModeCB(const SFUInt::SetValueEvent& event);

      virtual void getExposureCB(const SFFloat::GetValueEvent& event);
      virtual void setExposureCB(const SFFloat::SetValueEvent& event);

      virtual void getEnableSSAOCB(const SFBool::GetValueEvent& event);
      virtual void setEnableSSAOCB(const SFBool::SetValueEvent& event);

      virtual void getSSAORadiusCB(const SFFloat::GetValueEvent& event);
      virtual void setSSAORadiusCB(const SFFloat::SetValueEvent& event);

      virtual void getSSAOIntensityCB(const SFFloat::GetValueEvent& event);
      virtual void setSSAOIntensityCB(const SFFloat::SetValueEvent& event);

      virtual void getSSAOFalloffCB(const SFFloat::GetValueEvent& event);
      virtual void setSSAOFalloffCB(const SFFloat::SetValueEvent& event);

      virtual void getSSAONoiseTextureCB(const SFString::GetValueEvent& event);
      virtual void setSSAONoiseTextureCB(const SFString::SetValueEvent& event);

      virtual void getEnvironmentLightingColorCB(const SFColor::GetValueEvent& event);
      virtual void setEnvironmentLightingColorCB(const SFColor::SetValueEvent& event);

      virtual void getEnvironmentLightingModeCB(const SFUInt::GetValueEvent& event);
      virtual void setEnvironmentLightingModeCB(const SFUInt::SetValueEvent& event);

      virtual void getEnvironmentLightingTextureCB(const SFString::GetValueEvent& event);
      virtual void setEnvironmentLightingTextureCB(const SFString::SetValueEvent& event);

      virtual void getAlternativeEnvironmentLightingTextureCB(const SFString::GetValueEvent& event);
      virtual void setAlternativeEnvironmentLightingTextureCB(const SFString::SetValueEvent& event);

      virtual void getEnvironmentLightingTextureBlendFactorCB(const SFFloat::GetValueEvent& event);
      virtual void setEnvironmentLightingTextureBlendFactorCB(const SFFloat::SetValueEvent& event);

      virtual void getHorizonFadeCB(const SFFloat::GetValueEvent& event);
      virtual void setHorizonFadeCB(const SFFloat::SetValueEvent& event);

      virtual void getEnableScreenSpaceShadowCB(const SFBool::GetValueEvent& event);
      virtual void setEnableScreenSpaceShadowCB(const SFBool::SetValueEvent& event);

      virtual void getScreenSpaceShadowRadiusCB(const SFFloat::GetValueEvent& event);
      virtual void setScreenSpaceShadowRadiusCB(const SFFloat::SetValueEvent& event);

      virtual void getScreenSpaceShadowMaxRadiusPXCB(const SFFloat::GetValueEvent& event);
      virtual void setScreenSpaceShadowMaxRadiusPXCB(const SFFloat::SetValueEvent& event);

      virtual void getScreenSpaceShadowIntensityCB(const SFFloat::GetValueEvent& event);
      virtual void setScreenSpaceShadowIntensityCB(const SFFloat::SetValueEvent& event);

      virtual void getDebugTilesCB(const SFBool::GetValueEvent& event);
      virtual void setDebugTilesCB(const SFBool::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::ResolvePassDescription.
       */
      std::shared_ptr< ::gua::ResolvePassDescription> const& getGuaResolvePassDescription() const;

    private:

      std::shared_ptr< ::gua::ResolvePassDescription> m_guaResolvePassDescription;


      ResolvePassDescription(const ResolvePassDescription&);
      ResolvePassDescription& operator=(const ResolvePassDescription&);
    };

    typedef SingleField<Link<ResolvePassDescription> > SFResolvePassDescription;
    typedef MultiField<Link<ResolvePassDescription> > MFResolvePassDescription;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::ResolvePassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::ResolvePassDescription> >;
#endif

}

#endif //AVANGO_GUA_RESOLVE_PASS_DESCRIPTION_HPP
