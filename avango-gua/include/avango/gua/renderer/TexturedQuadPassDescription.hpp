#ifndef AVANGO_GUA_TEXTURED_QUAD_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_TEXTURED_QUAD_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/TexturedQuadPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::TexturedQuadPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL TexturedQuadPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::TexturedQuadPassDescription is created.
       * Otherwise, the given ::gua::TexturedQuadPassDescription is used.
       */
      TexturedQuadPassDescription(std::shared_ptr< ::gua::TexturedQuadPassDescription> const& TexturedQuadPassDescription =
                             std::shared_ptr< ::gua::TexturedQuadPassDescription>(new ::gua::TexturedQuadPassDescription()) );



    public:

      /**
       * Get the wrapped ::gua::TexturedQuadPassDescription.
       */
      std::shared_ptr< ::gua::TexturedQuadPassDescription> const& getGuaTexturedQuadPassDescription() const;

    private:

      std::shared_ptr< ::gua::TexturedQuadPassDescription> m_guaTexturedQuadPassDescription;


      TexturedQuadPassDescription(const TexturedQuadPassDescription&);
      TexturedQuadPassDescription& operator=(const TexturedQuadPassDescription&);
    };

    using SFTexturedQuadPassDescription = SingleField<Link<TexturedQuadPassDescription> >;
    using MFTexturedQuadPassDescription = MultiField<Link<TexturedQuadPassDescription> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TexturedQuadPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::TexturedQuadPassDescription> >;
#endif

}

#endif //AVANGO_GUA_TEXTURED_QUAD_PASS_DESCRIPTION_HPP
