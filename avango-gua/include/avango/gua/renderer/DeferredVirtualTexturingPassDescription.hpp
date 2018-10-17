#ifndef AVANGO_GUA_DEFERRED_VIRTUAL_TEXTURING_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_DEFERRED_VIRTUAL_TEXTURING_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/virtual_texturing/DeferredVirtualTexturingPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::DeferredVirtualTexturingPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL DeferredVirtualTexturingPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::DeferredVirtualTexturingPassDescription is created.
       * Otherwise, the given ::gua::DeferredVirtualTexturingPassDescription is used.
       */
      DeferredVirtualTexturingPassDescription(std::shared_ptr< ::gua::DeferredVirtualTexturingPassDescription> const& DeferredVirtualTexturingPassDescription =
                             std::shared_ptr< ::gua::DeferredVirtualTexturingPassDescription>(new ::gua::DeferredVirtualTexturingPassDescription()) );



    public:

      /**
       * Get the wrapped ::gua::DeferredVirtualTexturingPassDescription.
       */
      std::shared_ptr< ::gua::DeferredVirtualTexturingPassDescription> const& getGuaDeferredVirtualTexturingPassDescription() const;

    private:

      std::shared_ptr< ::gua::DeferredVirtualTexturingPassDescription> m_guaDeferredVirtualTexturingPassDescription;


      DeferredVirtualTexturingPassDescription(const DeferredVirtualTexturingPassDescription&);
      DeferredVirtualTexturingPassDescription& operator=(const DeferredVirtualTexturingPassDescription&);
    };

    using SFDeferredVirtualTexturingPassDescription = SingleField<Link<DeferredVirtualTexturingPassDescription> >;
    using MFDeferredVirtualTexturingPassDescription = MultiField<Link<DeferredVirtualTexturingPassDescription> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::DeferredVirtualTexturingPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::DeferredVirtualTexturingPassDescription> >;
#endif

}

#endif //AVANGO_GUA_DEFERRED_VIRTUAL_TEXTURING_PASS_DESCRIPTION_HPP
