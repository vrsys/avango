#include <avango/gua/renderer/Pipeline.hpp>
#include <avango/gua/Types.hpp>
#include <avango/Base.h>
#include <boost/bind.hpp>
#include <avango/Logger.h>

namespace
{
  av::Logger& logger(av::getLogger("av::gua::Pipeline"));
}

AV_FC_DEFINE(av::gua::Pipeline);

AV_FIELD_DEFINE(av::gua::SFPipeline);
AV_FIELD_DEFINE(av::gua::MFPipeline);

av::gua::Pipeline::Pipeline(::gua::Pipeline* guaPipeline)
    : m_guaPipeline(guaPipeline),
      m_Window(nullptr)
{

    AV_FC_ADD_ADAPTOR_FIELD(OutputTextureName,
                          boost::bind(&Pipeline::getOutputTextureNameCB, this, _1),
                          boost::bind(&Pipeline::setOutputTextureNameCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Camera,
                          boost::bind(&Pipeline::getCameraCB, this, _1),
                          boost::bind(&Pipeline::setCameraCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Enabled,
                          boost::bind(&Pipeline::getEnabledCB, this, _1),
                          boost::bind(&Pipeline::setEnabledCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableGlobalClippingPlane,
                          boost::bind(&Pipeline::getEnableGlobalClippingPlaneCB, this, _1),
                          boost::bind(&Pipeline::setEnableGlobalClippingPlaneCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(GlobalClippingPlane,
                          boost::bind(&Pipeline::getGlobalClippingPlaneCB, this, _1),
                          boost::bind(&Pipeline::setGlobalClippingPlaneCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(Window,
                          boost::bind(&Pipeline::getWindowCB, this, _1),
                          boost::bind(&Pipeline::setWindowCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(PreRenderPipelines,
                          boost::bind(&Pipeline::getPreRenderPipelinesCB, this, _1),
                          boost::bind(&Pipeline::setPreRenderPipelinesCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableSsao,
                          boost::bind(&Pipeline::getEnableSsaoCB, this, _1),
                          boost::bind(&Pipeline::setEnableSsaoCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(SsaoRadius,
                          boost::bind(&Pipeline::getSsaoRadiusCB, this, _1),
                          boost::bind(&Pipeline::setSsaoRadiusCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(SsaoIntensity,
                          boost::bind(&Pipeline::getSsaoIntensityCB, this, _1),
                          boost::bind(&Pipeline::setSsaoIntensityCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(SsaoFalloff,
                          boost::bind(&Pipeline::getSsaoFalloffCB, this, _1),
                          boost::bind(&Pipeline::setSsaoFalloffCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(NearClip,
                          boost::bind(&Pipeline::getNearClipCB, this, _1),
                          boost::bind(&Pipeline::setNearClipCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(FarClip,
                          boost::bind(&Pipeline::getFarClipCB, this, _1),
                          boost::bind(&Pipeline::setFarClipCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableBloom,
                          boost::bind(&Pipeline::getEnableBloomCB, this, _1),
                          boost::bind(&Pipeline::setEnableBloomCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(BloomRadius,
                          boost::bind(&Pipeline::getBloomRadiusCB, this, _1),
                          boost::bind(&Pipeline::setBloomRadiusCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(BloomThreshold,
                          boost::bind(&Pipeline::getBloomThresholdCB, this, _1),
                          boost::bind(&Pipeline::setBloomThresholdCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(BloomIntensity,
                          boost::bind(&Pipeline::getBloomIntensityCB, this, _1),
                          boost::bind(&Pipeline::setBloomIntensityCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableFog,
                          boost::bind(&Pipeline::getEnableFogCB, this, _1),
                          boost::bind(&Pipeline::setEnableFogCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(FogStart,
                          boost::bind(&Pipeline::getFogStartCB, this, _1),
                          boost::bind(&Pipeline::setFogStartCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(FogEnd,
                          boost::bind(&Pipeline::getFogEndCB, this, _1),
                          boost::bind(&Pipeline::setFogEndCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(FogTexture,
                          boost::bind(&Pipeline::getFogTextureCB, this, _1),
                          boost::bind(&Pipeline::setFogTextureCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(FogColor,
                          boost::bind(&Pipeline::getFogColorCB, this, _1),
                          boost::bind(&Pipeline::setFogColorCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(BackgroundMode,
                      boost::bind(&Pipeline::getBackgroundModeCB, this, _1),
                      boost::bind(&Pipeline::setBackgroundModeCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(BackgroundTexture,
                          boost::bind(&Pipeline::getBackgroundTextureCB, this, _1),
                          boost::bind(&Pipeline::setBackgroundTextureCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(BackgroundColor,
                          boost::bind(&Pipeline::getBackgroundColorCB, this, _1),
                          boost::bind(&Pipeline::setBackgroundColorCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableVignette,
                          boost::bind(&Pipeline::getEnableVignetteCB, this, _1),
                          boost::bind(&Pipeline::setEnableVignetteCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(VignetteColor,
                          boost::bind(&Pipeline::getVignetteColorCB, this, _1),
                          boost::bind(&Pipeline::setVignetteColorCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(VignetteCoverage,
                          boost::bind(&Pipeline::getVignetteCoverageCB, this, _1),
                          boost::bind(&Pipeline::setVignetteCoverageCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(VignetteSoftness,
                          boost::bind(&Pipeline::getVignetteSoftnessCB, this, _1),
                          boost::bind(&Pipeline::setVignetteSoftnessCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnablePreviewDisplay,
                          boost::bind(&Pipeline::getEnablePreviewDisplayCB, this, _1),
                          boost::bind(&Pipeline::setEnablePreviewDisplayCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableFPSDisplay,
                          boost::bind(&Pipeline::getEnableFPSDisplayCB, this, _1),
                          boost::bind(&Pipeline::setEnableFPSDisplayCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableRayDisplay,
                          boost::bind(&Pipeline::getEnableRayDisplayCB, this, _1),
                          boost::bind(&Pipeline::setEnableRayDisplayCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableBBoxDisplay,
                          boost::bind(&Pipeline::getEnableBBoxDisplayCB, this, _1),
                          boost::bind(&Pipeline::setEnableBBoxDisplayCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableWireframe,
                          boost::bind(&Pipeline::getEnableWireframeCB, this, _1),
                          boost::bind(&Pipeline::setEnableWireframeCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableFXAA,
                          boost::bind(&Pipeline::getEnableFXAACB, this, _1),
                          boost::bind(&Pipeline::setEnableFXAACB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableFrustumCulling,
                          boost::bind(&Pipeline::getEnableFrustumCullingCB, this, _1),
                          boost::bind(&Pipeline::setEnableFrustumCullingCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableBackfaceCulling,
                          boost::bind(&Pipeline::getEnableBackfaceCullingCB, this, _1),
                          boost::bind(&Pipeline::setEnableBackfaceCullingCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(MaxTesselation,
                          boost::bind(&Pipeline::getMaxTesselationCB, this, _1),
                          boost::bind(&Pipeline::setMaxTesselationCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(TesselationMaxError,
                          boost::bind(&Pipeline::getTesselationMaxErrorCB, this, _1),
                          boost::bind(&Pipeline::setTesselationMaxErrorCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableHDR,
                          boost::bind(&Pipeline::getEnableHDRCB, this, _1),
                          boost::bind(&Pipeline::setEnableHDRCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(HDRKey,
                          boost::bind(&Pipeline::getHDRKeyCB, this, _1),
                          boost::bind(&Pipeline::setHDRKeyCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(AmbientColor,
                          boost::bind(&Pipeline::getAmbientColorCB, this, _1),
                          boost::bind(&Pipeline::setAmbientColorCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(EnableStereo,
                          boost::bind(&Pipeline::getEnableStereoCB, this, _1),
                          boost::bind(&Pipeline::setEnableStereoCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(LeftResolution,
                          boost::bind(&Pipeline::getLeftResolutionCB, this, _1),
                          boost::bind(&Pipeline::setLeftResolutionCB, this, _1));

    AV_FC_ADD_ADAPTOR_FIELD(RightResolution,
                          boost::bind(&Pipeline::getRightResolutionCB, this, _1),
                          boost::bind(&Pipeline::setRightResolutionCB, this, _1));
}


//av::gua::Pipeline::~Pipeline()
//{}

void
av::gua::Pipeline::initClass()
{
    if (!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::gua::Pipeline, true);

        SFPipeline::initClass("av::gua::SFPipeline", "av::Field");
        MFPipeline::initClass("av::gua::MFPipeline", "av::Field");
    }
}

::gua::Pipeline*
av::gua::Pipeline::getGuaPipeline() const
{
    return m_guaPipeline;
}


void
av::gua::Pipeline::getOutputTextureNameCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_output_texture_name();
}

void
av::gua::Pipeline::setOutputTextureNameCB(const SFString::SetValueEvent& event)
{
  m_guaPipeline->config.set_output_texture_name(event.getValue());
}

void
av::gua::Pipeline::getCameraCB(const SFCamera::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_Camera;
}

void
av::gua::Pipeline::setCameraCB(const SFCamera::SetValueEvent& event)
{
  m_Camera = event.getValue();
  auto camera(dynamic_cast<av::gua::Camera*> (event.getValue().getBasePtr()));
  if (camera) {
    m_guaPipeline->config.set_camera(*camera->getGuaCamera());
  }

}

void
av::gua::Pipeline::getEnabledCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enabled();
}

void
av::gua::Pipeline::setEnabledCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enabled(event.getValue());
}


void
av::gua::Pipeline::getEnableGlobalClippingPlaneCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_global_clipping_plane();
}

void
av::gua::Pipeline::setEnableGlobalClippingPlaneCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_global_clipping_plane(event.getValue());
}

void
av::gua::Pipeline::getGlobalClippingPlaneCB(const SFVec4::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_global_clipping_plane();
}

void
av::gua::Pipeline::setGlobalClippingPlaneCB(const SFVec4::SetValueEvent& event)
{
  m_guaPipeline->config.set_global_clipping_plane(event.getValue());
}


void
av::gua::Pipeline::getWindowCB(const SFWindow::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_Window;
}

void
av::gua::Pipeline::setWindowCB(const SFWindow::SetValueEvent& event)
{
  m_Window = event.getValue();
  auto window(dynamic_cast<av::gua::Window*> (event.getValue().getBasePtr()));
  if (window)
    m_guaPipeline->set_window(window->getGuaWindow());
}

void
av::gua::Pipeline::getPreRenderPipelinesCB(const MultiField<Link<Pipeline>>::GetValueEvent& event)
{

  *(event.getValuePtr()) = m_preRenderPipelines;
}

void
av::gua::Pipeline::setPreRenderPipelinesCB(const MultiField<Link<Pipeline>>::SetValueEvent& event)
{
  std::vector< ::gua::Pipeline*> gua_pipes;
  m_preRenderPipelines.clear();
  for (auto pipe: event.getValue()) {
    m_preRenderPipelines.push_back(pipe);
    gua_pipes.push_back(pipe->getGuaPipeline());
  }

  m_guaPipeline->set_prerender_pipelines(gua_pipes);
}

void
av::gua::Pipeline::getEnableSsaoCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_ssao();
}

void
av::gua::Pipeline::setEnableSsaoCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_ssao(event.getValue());
}

void
av::gua::Pipeline::getSsaoRadiusCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_ssao_radius();
}

void
av::gua::Pipeline::setSsaoRadiusCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_ssao_radius(event.getValue());
}

void
av::gua::Pipeline::getSsaoIntensityCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_ssao_intensity();
}

void
av::gua::Pipeline::setSsaoIntensityCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_ssao_intensity(event.getValue());
}

void
av::gua::Pipeline::getSsaoFalloffCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_ssao_falloff();
}

void
av::gua::Pipeline::setSsaoFalloffCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_ssao_falloff(event.getValue());
}

void
av::gua::Pipeline::getNearClipCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_near_clip();
}

void
av::gua::Pipeline::setNearClipCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_near_clip(event.getValue());
}

void
av::gua::Pipeline::getFarClipCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_far_clip();
}

void
av::gua::Pipeline::setFarClipCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_far_clip(event.getValue());
}

void
av::gua::Pipeline::getEnableBloomCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_bloom();
}

void
av::gua::Pipeline::setEnableBloomCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_bloom(event.getValue());
}

void
av::gua::Pipeline::getBloomRadiusCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_bloom_radius();
}

void
av::gua::Pipeline::setBloomRadiusCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_bloom_radius(event.getValue());
}

void
av::gua::Pipeline::getBloomThresholdCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_bloom_threshold();
}

void
av::gua::Pipeline::setBloomThresholdCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_bloom_threshold(event.getValue());
}

void
av::gua::Pipeline::getBloomIntensityCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_bloom_intensity();
}

void
av::gua::Pipeline::setBloomIntensityCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_bloom_intensity(event.getValue());
}

void
av::gua::Pipeline::getEnableFogCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_fog();
}

void
av::gua::Pipeline::setEnableFogCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_fog(event.getValue());
}

void
av::gua::Pipeline::getFogStartCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_fog_start();
}

void
av::gua::Pipeline::setFogStartCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_fog_start(event.getValue());
}

void
av::gua::Pipeline::getFogEndCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_fog_end();
}

void
av::gua::Pipeline::setFogEndCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_fog_end(event.getValue());
}

void
av::gua::Pipeline::getFogTextureCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_fog_texture();
}

void
av::gua::Pipeline::setFogTextureCB(const SFString::SetValueEvent& event)
{
  m_guaPipeline->config.set_fog_texture(event.getValue());
}

void
av::gua::Pipeline::getFogColorCB(const SFColor::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_fog_color();
}

void
av::gua::Pipeline::setFogColorCB(const SFColor::SetValueEvent& event)
{
  m_guaPipeline->config.set_fog_color(event.getValue());
}

void
av::gua::Pipeline::getBackgroundModeCB(const SFUInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = static_cast<unsigned>(m_guaPipeline->config.background_mode());
}

void
av::gua::Pipeline::setBackgroundModeCB(const SFUInt::SetValueEvent& event)
{
  m_guaPipeline->config.background_mode() = static_cast< ::gua::BackgroundMode>(event.getValue());
}


void
av::gua::Pipeline::getBackgroundTextureCB(const SFString::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_background_texture();
}

void
av::gua::Pipeline::setBackgroundTextureCB(const SFString::SetValueEvent& event)
{
  m_guaPipeline->config.set_background_texture(event.getValue());
}

void
av::gua::Pipeline::getBackgroundColorCB(const SFColor::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_background_color();
}

void
av::gua::Pipeline::setBackgroundColorCB(const SFColor::SetValueEvent& event)
{
  m_guaPipeline->config.set_background_color(event.getValue());
}

void
av::gua::Pipeline::getEnableVignetteCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_vignette();
}

void
av::gua::Pipeline::setEnableVignetteCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_vignette(event.getValue());
}

void
av::gua::Pipeline::getVignetteColorCB(const SFColor::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_vignette_color();
}

void
av::gua::Pipeline::setVignetteColorCB(const SFColor::SetValueEvent& event)
{
  m_guaPipeline->config.set_vignette_color(event.getValue());
}

void
av::gua::Pipeline::getVignetteCoverageCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_vignette_coverage();
}

void
av::gua::Pipeline::setVignetteCoverageCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_vignette_coverage(event.getValue());
}

void
av::gua::Pipeline::getVignetteSoftnessCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_vignette_softness();
}

void
av::gua::Pipeline::setVignetteSoftnessCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_vignette_softness(event.getValue());
}

void
av::gua::Pipeline::getEnablePreviewDisplayCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_preview_display();
}

void
av::gua::Pipeline::setEnablePreviewDisplayCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_preview_display(event.getValue());
}

void
av::gua::Pipeline::getEnableFPSDisplayCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_fps_display();
}

void
av::gua::Pipeline::setEnableFPSDisplayCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_fps_display(event.getValue());
}

void
av::gua::Pipeline::getEnableRayDisplayCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_ray_display();
}

void
av::gua::Pipeline::setEnableRayDisplayCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_ray_display(event.getValue());
}

void
av::gua::Pipeline::getEnableBBoxDisplayCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_bbox_display();
}

void
av::gua::Pipeline::setEnableBBoxDisplayCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_bbox_display(event.getValue());
}

void
av::gua::Pipeline::getEnableWireframeCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_wireframe();
}

void
av::gua::Pipeline::setEnableWireframeCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_wireframe(event.getValue());
}

void
av::gua::Pipeline::getEnableFXAACB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_fxaa();
}

void
av::gua::Pipeline::setEnableFXAACB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_fxaa(event.getValue());
}

void
av::gua::Pipeline::getEnableFrustumCullingCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_frustum_culling();
}

void
av::gua::Pipeline::setEnableFrustumCullingCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_frustum_culling(event.getValue());
}

void
av::gua::Pipeline::getEnableBackfaceCullingCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_backface_culling();
}

void
av::gua::Pipeline::setEnableBackfaceCullingCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_backface_culling(event.getValue());
}

void
av::gua::Pipeline::getMaxTesselationCB(const SFInt::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_max_tesselation();
}

void
av::gua::Pipeline::setMaxTesselationCB(const SFInt::SetValueEvent& event)
{
  m_guaPipeline->config.set_max_tesselation(event.getValue());
}

void
av::gua::Pipeline::getTesselationMaxErrorCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_tesselation_max_error();
}

void
av::gua::Pipeline::setTesselationMaxErrorCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_tesselation_max_error(event.getValue());
}

void
av::gua::Pipeline::getEnableHDRCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_hdr();
}

void
av::gua::Pipeline::setEnableHDRCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_hdr(event.getValue());
}

void
av::gua::Pipeline::getHDRKeyCB(const SFFloat::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_hdr_key();
}

void
av::gua::Pipeline::setHDRKeyCB(const SFFloat::SetValueEvent& event)
{
  m_guaPipeline->config.set_hdr_key(event.getValue());
}

void
av::gua::Pipeline::getAmbientColorCB(const SFColor::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_ambient_color();
}

void
av::gua::Pipeline::setAmbientColorCB(const SFColor::SetValueEvent& event)
{
  m_guaPipeline->config.set_ambient_color(event.getValue());
}

void
av::gua::Pipeline::getEnableStereoCB(const SFBool::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_enable_stereo();
}

void
av::gua::Pipeline::setEnableStereoCB(const SFBool::SetValueEvent& event)
{
  m_guaPipeline->config.set_enable_stereo(event.getValue());
}

void
av::gua::Pipeline::getLeftResolutionCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_left_resolution();
}

void
av::gua::Pipeline::setLeftResolutionCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaPipeline->config.set_left_resolution(event.getValue());
}

void
av::gua::Pipeline::getRightResolutionCB(const SFVec2ui::GetValueEvent& event)
{
  *(event.getValuePtr()) = m_guaPipeline->config.get_right_resolution();
}

void
av::gua::Pipeline::setRightResolutionCB(const SFVec2ui::SetValueEvent& event)
{
  m_guaPipeline->config.set_right_resolution(event.getValue());
}

