#ifndef AVANGO_GUA_PIPELINE_DESCRIPTION_HPP
#define AVANGO_GUA_PIPELINE_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/PipelineDescription.hpp>

#include <avango/gua/renderer/TriMeshPassDescription.hpp>
#include <avango/gua/renderer/BackgroundPassDescription.hpp>

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


      av::gua::TriMeshPassDescription*    add_tri_mesh_pass() const;
      av::gua::BackgroundPassDescription* add_background_pass() const;


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
