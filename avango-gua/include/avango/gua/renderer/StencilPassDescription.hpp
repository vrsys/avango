#ifndef AVANGO_GUA_STENCIL_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_STENCIL_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/StencilPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::StencilPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL StencilPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::StencilPassDescription is created.
       * Otherwise, the given ::gua::StencilPassDescription is used.
       */
      StencilPassDescription(std::shared_ptr< ::gua::StencilPassDescription> const& StencilPassDescription =
                             std::shared_ptr< ::gua::StencilPassDescription>(new ::gua::StencilPassDescription()) );



    public:

      /**
       * Get the wrapped ::gua::StencilPassDescription.
       */
      std::shared_ptr< ::gua::StencilPassDescription> const& getGuaStencilPassDescription() const;

    private:

      std::shared_ptr< ::gua::StencilPassDescription> m_guaStencilPassDescription;


      StencilPassDescription(const StencilPassDescription&);
      StencilPassDescription& operator=(const StencilPassDescription&);
    };

    typedef SingleField<Link<StencilPassDescription> > SFStencilPassDescription;
    typedef MultiField<Link<StencilPassDescription> > MFStencilPassDescription;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::StencilPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::StencilPassDescription> >;
#endif

}

#endif //AVANGO_GUA_STENCIL_PASS_DESCRIPTION_HPP
