#ifndef AVANGO_GUA_PIPELINE_DESCRIPTION_HPP
#define AVANGO_GUA_PIPELINE_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/PipelineDescription.hpp>

#include <avango/gua/renderer/TriMeshPassDescription.hpp>
#include <avango/gua/renderer/TexturedQuadPassDescription.hpp>
#include <avango/gua/renderer/BackgroundPassDescription.hpp>
#include <avango/gua/renderer/FullscreenPassDescription.hpp>
#include <avango/gua/renderer/BBoxPassDescription.hpp>
#include <avango/gua/renderer/EmissivePassDescription.hpp>
#include <avango/gua/renderer/PhysicallyBasedShadingPassDescription.hpp>
#include <avango/gua/renderer/TexturedScreenSpaceQuadPassDescription.hpp>
#include <avango/gua/renderer/SSAOPassDescription.hpp>
#include <avango/gua/renderer/ResolvePassDescription.hpp>
#include <avango/gua/renderer/LightVisibilityPassDescription.hpp>

#if defined(AVANGO_PBR_SUPPORT)
#include <avango/gua/renderer/PLODPassDescription.hpp>
#endif

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::PipelineDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL PipelineDescription : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::PipelineDescription is created.
       * Otherwise, the given ::gua::PipelineDescription is used.
       */
      PipelineDescription(std::shared_ptr< ::gua::PipelineDescription> const& PipelineDescription =
                          std::shared_ptr< ::gua::PipelineDescription>(new ::gua::PipelineDescription()));


    public:

      SFBool EnableABuffer;
      SFInt  ABufferSize;

      virtual void getEnableABufferCB(const SFBool::GetValueEvent& event);
      virtual void setEnableABufferCB(const SFBool::SetValueEvent& event);

      virtual void getABufferSizeCB(const SFInt::GetValueEvent& event);
      virtual void setABufferSizeCB(const SFInt::SetValueEvent& event);

      av::Link<av::gua::TriMeshPassDescription>                   add_tri_mesh_pass();
      av::Link<av::gua::TriMeshPassDescription>                   get_tri_mesh_pass();
      std::vector<av::Link<av::gua::TriMeshPassDescription>>      get_tri_mesh_passes();

      av::Link<av::gua::TexturedQuadPassDescription>              add_textured_quad_pass();
      av::Link<av::gua::TexturedQuadPassDescription>              get_textured_quad_pass();
      std::vector<av::Link<av::gua::TexturedQuadPassDescription>> get_textured_quad_passes();

      av::Link<av::gua::BackgroundPassDescription>                add_background_pass();
      av::Link<av::gua::BackgroundPassDescription>                get_background_pass();
      std::vector<av::Link<av::gua::BackgroundPassDescription>>   get_background_passes();

      av::Link<av::gua::BBoxPassDescription>                      add_bbox_pass();
      av::Link<av::gua::BBoxPassDescription>                      get_bbox_pass();
      std::vector<av::Link<av::gua::BBoxPassDescription>>         get_bbox_passes();

      av::Link<av::gua::EmissivePassDescription>                  add_emissive_pass();
      av::Link<av::gua::EmissivePassDescription>                  get_emissive_pass();
      std::vector<av::Link<av::gua::EmissivePassDescription>>     get_emissive_passes();

      av::Link<av::gua::PhysicallyBasedShadingPassDescription>                   add_physically_based_shading_pass();
      av::Link<av::gua::PhysicallyBasedShadingPassDescription>                   get_physically_based_shading_pass();
      std::vector<av::Link<av::gua::PhysicallyBasedShadingPassDescription>>      get_physically_based_shading_passes();

      av::Link<av::gua::TexturedScreenSpaceQuadPassDescription>                  add_textured_screen_space_quad_pass();
      av::Link<av::gua::TexturedScreenSpaceQuadPassDescription>                  get_textured_screen_space_quad_pass();
      std::vector<av::Link<av::gua::TexturedScreenSpaceQuadPassDescription>>     get_textured_screen_space_quad_passes();

      av::Link<av::gua::FullscreenPassDescription>                add_fullscreen_pass();
      av::Link<av::gua::FullscreenPassDescription>                get_fullscreen_pass();
      std::vector<av::Link<av::gua::FullscreenPassDescription>>   get_fullscreen_passes();

      av::Link<av::gua::SSAOPassDescription>                      add_ssao_pass();
      av::Link<av::gua::SSAOPassDescription>                      get_ssao_pass();
      std::vector<av::Link<av::gua::SSAOPassDescription>>         get_ssao_passes();

      av::Link<av::gua::ResolvePassDescription>                   add_resolve_pass();
      av::Link<av::gua::ResolvePassDescription>                   get_resolve_pass();
      std::vector<av::Link<av::gua::ResolvePassDescription>>      get_resolve_passes();

      av::Link<av::gua::LightVisibilityPassDescription>              add_light_visibility_pass();
      av::Link<av::gua::LightVisibilityPassDescription>              get_light_visibility_pass();
      std::vector<av::Link<av::gua::LightVisibilityPassDescription>> get_light_visibility_passes();

      #if defined(AVANGO_PBR_SUPPORT)

      av::Link<av::gua::PLODPassDescription>                         add_plod_pass();
      av::Link<av::gua::PLODPassDescription>                         get_plod_pass();
      std::vector<av::Link<av::gua::PLODPassDescription>>            get_plod_passes();

      #endif

      /**
       * Get the wrapped ::gua::PipelineDescription.
       */
      std::shared_ptr< ::gua::PipelineDescription> const& getGuaPipelineDescription() const;

    private:

      std::shared_ptr< ::gua::PipelineDescription> m_guaPipelineDescription;

      PipelineDescription(const PipelineDescription&);
      PipelineDescription& operator=(const PipelineDescription&);
    };

    typedef SingleField<Link<PipelineDescription> > SFPipelineDescription;
    typedef MultiField<Link<PipelineDescription> > MFPipelineDescription;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::PipelineDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::PipelineDescription> >;
#endif

}

#endif //AVANGO_GUA_PIPELINE_DESCRIPTION_HPP
