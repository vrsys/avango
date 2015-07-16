#ifndef AVANGO_GUA_PLOD_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_PLOD_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/PLODPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::PLODPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL PLODPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::PLODPassDescription is created.
       * Otherwise, the given ::gua::PLODPassDescription is used.
       */
      PLODPassDescription(std::shared_ptr< ::gua::PLODPassDescription> const& PLODPassDescription =
                             std::shared_ptr< ::gua::PLODPassDescription>(new ::gua::PLODPassDescription()) );



    public:

      /**
       * Get the wrapped ::gua::PLODPassDescription.
       */
      std::shared_ptr< ::gua::PLODPassDescription> const& getGuaPLODPassDescription() const;

    private:

      std::shared_ptr< ::gua::PLODPassDescription> m_guaPLODPassDescription;


      PLODPassDescription(const PLODPassDescription&);
      PLODPassDescription& operator=(const PLODPassDescription&);
    };

    using SFPLODPassDescription = SingleField<Link<PLODPassDescription> >;
    using MFPLODPassDescription = MultiField<Link<PLODPassDescription> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::PLODPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::PLODPassDescription> >;
#endif

}

#endif //AVANGO_GUA_PLOD_PASS_DESCRIPTION_HPP
