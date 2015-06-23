#ifndef AVANGO_GUA_TEXTURED_SCREEN_SPACE_QUAD_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_TEXTURED_SCREEN_SPACE_QUAD_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/TexturedScreenSpaceQuadPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::TexturedScreenSpaceQuadPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL TexturedScreenSpaceQuadPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::TexturedScreenSpaceQuadPassDescription is created.
       * Otherwise, the given ::gua::TexturedScreenSpaceQuadPassDescription is used.
       */
      TexturedScreenSpaceQuadPassDescription(std::shared_ptr< ::gua::TexturedScreenSpaceQuadPassDescription> const& TexturedScreenSpaceQuadPassDescription =
                                std::shared_ptr< ::gua::TexturedScreenSpaceQuadPassDescription>(new ::gua::TexturedScreenSpaceQuadPassDescription()) );


    public:

      /**
       * Get the wrapped ::gua::TexturedScreenSpaceQuadPassDescription.
       */
      std::shared_ptr< ::gua::TexturedScreenSpaceQuadPassDescription> const& getGuaTexturedScreenSpaceQuadPassDescription() const;

    private:

      std::shared_ptr< ::gua::TexturedScreenSpaceQuadPassDescription> m_guaTexturedScreenSpaceQuadPassDescription;


      TexturedScreenSpaceQuadPassDescription(const TexturedScreenSpaceQuadPassDescription&);
      TexturedScreenSpaceQuadPassDescription& operator=(const TexturedScreenSpaceQuadPassDescription&);
    };

    typedef SingleField<Link<TexturedScreenSpaceQuadPassDescription> > SFTexturedScreenSpaceQuadPassDescription;
    typedef MultiField<Link<TexturedScreenSpaceQuadPassDescription> > MFTexturedScreenSpaceQuadPassDescription;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TexturedScreenSpaceQuadPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::TexturedScreenSpaceQuadPassDescription> >;
#endif

}

#endif //AVANGO_GUA_TEXTURED_SCREEN_SPACE_QUAD_PASS_DESCRIPTION_HPP
