#ifndef AVANGO_GUA_PIPELINE_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_PIPELINE_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/PipelinePass.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::PipelinePassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL PipelinePassDescription : public av::FieldContainer
    {
      AV_FC_DECLARE_ABSTRACT();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::PipelinePassDescription is created.
       * Otherwise, the given ::gua::PipelinePassDescription is used.
       */
      PipelinePassDescription(std::shared_ptr< ::gua::PipelinePassDescription> const& PipelinePassDescription);



    public:

      /**
       * Get the wrapped ::gua::PipelinePassDescription.
       */
      std::shared_ptr< ::gua::PipelinePassDescription> const& getGuaPipelinePassDescription() const;

    private:

      std::shared_ptr< ::gua::PipelinePassDescription> m_guaPipelinePassDescription;


      PipelinePassDescription(const PipelinePassDescription&);
      PipelinePassDescription& operator=(const PipelinePassDescription&);
    };

    typedef SingleField<Link<PipelinePassDescription> > SFPipelinePassDescription;
    typedef MultiField<Link<PipelinePassDescription> > MFPipelinePassDescription;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::PipelinePassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::PipelinePassDescription> >;
#endif

}

#endif //AVANGO_GUA_PIPELINE_PASS_DESCRIPTION_HPP
