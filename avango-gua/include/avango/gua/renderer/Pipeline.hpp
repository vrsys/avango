// #ifndef AVANGO_GUA_PIPELINE_HPP
// #define AVANGO_GUA_PIPELINE_HPP

// /**
//  * \file
//  * \ingroup av_gua
//  */

// #include <gua/renderer/Pipeline.hpp>

// #include <avango/gua/renderer/Camera.hpp>
// #include <avango/gua/renderer/Window.hpp>

// #include <avango/gua/Fields.hpp>
// #include <avango/FieldContainer.h>

// namespace av
// {
//   namespace gua
//   {
//     /**
//      * Wrapper for ::gua::Pipeline
//      *
//      * \ingroup av_gua
//      */
//     class AV_GUA_DLL Pipeline : public av::FieldContainer
//     {
//       AV_FC_DECLARE();

//     public:

//       enum BackgroundModeEnum {
//         COLOR = ::gua::Pipeline::BackgroundMode::COLOR,
//         SKYMAP_TEXTURE = ::gua::Pipeline::BackgroundMode::SKYMAP_TEXTURE,
//         QUAD_TEXTURE = ::gua::Pipeline::BackgroundMode::QUAD_TEXTURE
//       };

//       /**
//        * Constructor. When called without arguments, a new ::gua::Pipeline is created.
//        * Otherwise, the given ::gua::Pipeline is used.
//        */
//       Pipeline(::gua::Pipeline* guaPipeline = new ::gua::Pipeline());

//       void print_shaders(std::string const& directory) const;

//     protected:

//       /**
//        * Destructor made protected to prevent allocation on stack.
//        */
// //      virtual ~Pipeline();

//     public:

//       SFString OutputTextureName;
//       SFCamera Camera;

//       SFBool  Enabled;

//       SFBool  EnableGlobalClippingPlane;
//       SFVec4  GlobalClippingPlane;

//       SFWindow Window;

//       MultiField<Link<Pipeline>> PreRenderPipelines;

//       SFBool  EnableSsao;
//       SFFloat SsaoRadius;
//       SFFloat SsaoIntensity;
//       SFFloat SsaoFalloff;

//       SFFloat NearClip;
//       SFFloat FarClip;

//       SFBool  EnableBloom;
//       SFFloat BloomRadius;
//       SFFloat BloomThreshold;
//       SFFloat BloomIntensity;

//       SFBool   EnableFog;
//       SFFloat  FogStart;
//       SFFloat  FogEnd;
//       SFString FogTexture;
//       SFColor  FogColor;

//       SFUInt   BackgroundMode;
//       SFString BackgroundTexture;
//       SFColor  BackgroundColor;

//       SFBool   EnableVignette;
//       SFColor  VignetteColor;
//       SFFloat  VignetteCoverage;
//       SFFloat  VignetteSoftness;

//       SFBool   EnablePreviewDisplay;
//       SFBool   EnableFPSDisplay;
//       SFBool   EnableRayDisplay;
//       SFBool   EnableBBoxDisplay;
//       SFBool   EnableWireframe;

//       SFBool   EnableFXAA;

//       SFBool   EnableFrustumCulling;
//       SFBool   EnableBackfaceCulling;

//       SFInt    MaxTesselation;
//       SFFloat  TesselationMaxError;

//       SFBool   EnableHDR;
//       SFFloat  HDRKey;

//       SFColor  AmbientColor;

//       SFBool   EnableStereo;
//       SFVec2ui LeftResolution;
//       SFVec2ui RightResolution;

//       virtual void getOutputTextureNameCB(const SFString::GetValueEvent& event);
//       virtual void setOutputTextureNameCB(const SFString::SetValueEvent& event);

//       virtual void getCameraCB(const SFCamera::GetValueEvent& event);
//       virtual void setCameraCB(const SFCamera::SetValueEvent& event);

//       virtual void getEnabledCB(const SFBool::GetValueEvent& event);
//       virtual void setEnabledCB(const SFBool::SetValueEvent& event);

//       virtual void getEnableGlobalClippingPlaneCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableGlobalClippingPlaneCB(const SFBool::SetValueEvent& event);

//       virtual void getGlobalClippingPlaneCB(const SFVec4::GetValueEvent& event);
//       virtual void setGlobalClippingPlaneCB(const SFVec4::SetValueEvent& event);

//       virtual void getWindowCB(const SFWindow::GetValueEvent& event);
//       virtual void setWindowCB(const SFWindow::SetValueEvent& event);

//       virtual void getPreRenderPipelinesCB(const  MultiField<Link<Pipeline>>::GetValueEvent& event);
//       virtual void setPreRenderPipelinesCB(const  MultiField<Link<Pipeline>>::SetValueEvent& event);

//       virtual void getEnableSsaoCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableSsaoCB(const SFBool::SetValueEvent& event);

//       virtual void getSsaoRadiusCB(const SFFloat::GetValueEvent& event);
//       virtual void setSsaoRadiusCB(const SFFloat::SetValueEvent& event);

//       virtual void getSsaoIntensityCB(const SFFloat::GetValueEvent& event);
//       virtual void setSsaoIntensityCB(const SFFloat::SetValueEvent& event);

//       virtual void getSsaoFalloffCB(const SFFloat::GetValueEvent& event);
//       virtual void setSsaoFalloffCB(const SFFloat::SetValueEvent& event);

//       virtual void getNearClipCB(const SFFloat::GetValueEvent& event);
//       virtual void setNearClipCB(const SFFloat::SetValueEvent& event);

//       virtual void getFarClipCB(const SFFloat::GetValueEvent& event);
//       virtual void setFarClipCB(const SFFloat::SetValueEvent& event);

//       virtual void getEnableBloomCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableBloomCB(const SFBool::SetValueEvent& event);

//       virtual void getBloomRadiusCB(const SFFloat::GetValueEvent& event);
//       virtual void setBloomRadiusCB(const SFFloat::SetValueEvent& event);

//       virtual void getBloomThresholdCB(const SFFloat::GetValueEvent& event);
//       virtual void setBloomThresholdCB(const SFFloat::SetValueEvent& event);

//       virtual void getBloomIntensityCB(const SFFloat::GetValueEvent& event);
//       virtual void setBloomIntensityCB(const SFFloat::SetValueEvent& event);

//       virtual void getEnableFogCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableFogCB(const SFBool::SetValueEvent& event);

//       virtual void getFogStartCB(const SFFloat::GetValueEvent& event);
//       virtual void setFogStartCB(const SFFloat::SetValueEvent& event);

//       virtual void getFogEndCB(const SFFloat::GetValueEvent& event);
//       virtual void setFogEndCB(const SFFloat::SetValueEvent& event);

//       virtual void getFogTextureCB(const SFString::GetValueEvent& event);
//       virtual void setFogTextureCB(const SFString::SetValueEvent& event);

//       virtual void getFogColorCB(const SFColor::GetValueEvent& event);
//       virtual void setFogColorCB(const SFColor::SetValueEvent& event);

//       virtual void getBackgroundModeCB(const SFUInt::GetValueEvent& event);
//       virtual void setBackgroundModeCB(const SFUInt::SetValueEvent& event);

//       virtual void getBackgroundTextureCB(const SFString::GetValueEvent& event);
//       virtual void setBackgroundTextureCB(const SFString::SetValueEvent& event);

//       virtual void getBackgroundColorCB(const SFColor::GetValueEvent& event);
//       virtual void setBackgroundColorCB(const SFColor::SetValueEvent& event);

//       virtual void getEnableVignetteCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableVignetteCB(const SFBool::SetValueEvent& event);

//       virtual void getVignetteColorCB(const SFColor::GetValueEvent& event);
//       virtual void setVignetteColorCB(const SFColor::SetValueEvent& event);

//       virtual void getVignetteCoverageCB(const SFFloat::GetValueEvent& event);
//       virtual void setVignetteCoverageCB(const SFFloat::SetValueEvent& event);

//       virtual void getVignetteSoftnessCB(const SFFloat::GetValueEvent& event);
//       virtual void setVignetteSoftnessCB(const SFFloat::SetValueEvent& event);

//       virtual void getEnablePreviewDisplayCB(const SFBool::GetValueEvent& event);
//       virtual void setEnablePreviewDisplayCB(const SFBool::SetValueEvent& event);

//       virtual void getEnableFPSDisplayCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableFPSDisplayCB(const SFBool::SetValueEvent& event);

//       virtual void getEnableRayDisplayCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableRayDisplayCB(const SFBool::SetValueEvent& event);

//       virtual void getEnableBBoxDisplayCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableBBoxDisplayCB(const SFBool::SetValueEvent& event);

//       virtual void getEnableWireframeCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableWireframeCB(const SFBool::SetValueEvent& event);

//       virtual void getEnableFXAACB(const SFBool::GetValueEvent& event);
//       virtual void setEnableFXAACB(const SFBool::SetValueEvent& event);

//       virtual void getEnableFrustumCullingCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableFrustumCullingCB(const SFBool::SetValueEvent& event);

//       virtual void getEnableBackfaceCullingCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableBackfaceCullingCB(const SFBool::SetValueEvent& event);

//       virtual void getMaxTesselationCB(const SFInt::GetValueEvent& event);
//       virtual void setMaxTesselationCB(const SFInt::SetValueEvent& event);

//       virtual void getTesselationMaxErrorCB(const SFFloat::GetValueEvent& event);
//       virtual void setTesselationMaxErrorCB(const SFFloat::SetValueEvent& event);

//       virtual void getEnableHDRCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableHDRCB(const SFBool::SetValueEvent& event);

//       virtual void getHDRKeyCB(const SFFloat::GetValueEvent& event);
//       virtual void setHDRKeyCB(const SFFloat::SetValueEvent& event);

//       virtual void getAmbientColorCB(const SFColor::GetValueEvent& event);
//       virtual void setAmbientColorCB(const SFColor::SetValueEvent& event);

//       virtual void getEnableStereoCB(const SFBool::GetValueEvent& event);
//       virtual void setEnableStereoCB(const SFBool::SetValueEvent& event);

//       virtual void getLeftResolutionCB(const SFVec2ui::GetValueEvent& event);
//       virtual void setLeftResolutionCB(const SFVec2ui::SetValueEvent& event);

//       virtual void getRightResolutionCB(const SFVec2ui::GetValueEvent& event);
//       virtual void setRightResolutionCB(const SFVec2ui::SetValueEvent& event);

//       /**
//        * Get the wrapped ::gua::Pipeline.
//        */
//       ::gua::Pipeline* getGuaPipeline() const;

//     public:



//     private:

//       ::gua::Pipeline *m_guaPipeline;

//       av::Link<av::gua::Window> m_Window;
//       MultiField<Link<Pipeline>>::ContainerType m_preRenderPipelines;

//       av::Link<av::gua::Camera> m_Camera;

//       Pipeline(const Pipeline&);
//       Pipeline& operator=(const Pipeline&);
//     };

//     typedef SingleField<Link<Pipeline>> SFPipeline;
//     typedef MultiField<Link<Pipeline>> MFPipeline;

//   }

// #ifdef AV_INSTANTIATE_FIELD_TEMPLATES
//   template class AV_GUA_DLL SingleField<Link<gua::Pipeline> >;
//   template class AV_GUA_DLL MultiField<Link<gua::Pipeline> >;
// #endif

// }

// #endif //AVANGO_GUA_PIPELINE_HPP
