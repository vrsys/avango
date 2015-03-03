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
        QUAD_TEXTURE = static_cast<unsigned>(::gua::ResolvePassDescription::BackgroundMode::QUAD_TEXTURE)
      };

      enum ToneMappingModeEnum {
        LINEAR = static_cast<unsigned>(::gua::ResolvePassDescription::ToneMappingMethod::LINEAR),
        HEJL = static_cast<unsigned>(::gua::ResolvePassDescription::ToneMappingMethod::HEJL),
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
      SFUInt   BackgroundMode;

      SFBool   EnableFog;
      SFFloat  FogStart;
      SFFloat  FogEnd;

      SFUInt   ToneMappingMode;
      SFFloat  Exposure;

      SFColor  EnvironmentLightingColor;
      //SFUInt   EnvironmentLightingMode;
      //SFUInt   EnvironmentLightingSphereMap;

      SFBool   EnableSSAO;
      //SFFloat  SSAOradius
      //SFFloat  SSAOintensity
      //SFFloat  SSAOfalloff

      //SFBool   ShowDebugTiles;

      virtual void getBackgroundColorCB(const SFColor::GetValueEvent& event);
      virtual void setBackgroundColorCB(const SFColor::SetValueEvent& event);

      virtual void getBackgroundTextureCB(const SFString::GetValueEvent& event);
      virtual void setBackgroundTextureCB(const SFString::SetValueEvent& event);

      virtual void getBackgroundModeCB(const SFUInt::GetValueEvent& event);
      virtual void setBackgroundModeCB(const SFUInt::SetValueEvent& event);

      virtual void getEnableFogCB(const SFBool::GetValueEvent& event);
      virtual void setEnableFogCB(const SFBool::SetValueEvent& event);

      virtual void getFogStartCB(const SFFloat::GetValueEvent& event);
      virtual void setFogStartCB(const SFFloat::SetValueEvent& event);

      virtual void getFogEndCB(const SFFloat::GetValueEvent& event);
      virtual void setFogEndCB(const SFFloat::SetValueEvent& event);

      virtual void getToneMappingModeCB(const SFUInt::GetValueEvent& event);
      virtual void setToneMappingModeCB(const SFUInt::SetValueEvent& event);

      virtual void getExposureCB(const SFFloat::GetValueEvent& event);
      virtual void setExposureCB(const SFFloat::SetValueEvent& event);

      virtual void getEnableSSAOCB(const SFBool::GetValueEvent& event);
      virtual void setEnableSSAOCB(const SFBool::SetValueEvent& event);

      virtual void getEnvironmentLightingColorCB(const SFColor::GetValueEvent& event);
      virtual void setEnvironmentLightingColorCB(const SFColor::SetValueEvent& event);

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
