#ifndef AVANGO_GUA_PLOD_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_PLOD_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/PLodPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    namespace lod {
      /**
       * Wrapper for ::gua::PLodPassDescription
       *
       * \ingroup av_gua
       */
      class AV_GUA_LOD_DLL PLodPassDescription : public av::gua::PipelinePassDescription
      {
        AV_FC_DECLARE();

      public:

        enum Flags {
          LQ_ONE_PASS = ::gua::PLodPassDescription::SurfelRenderMode::LQ_ONE_PASS,
          HQ_TWO_PASS = ::gua::PLodPassDescription::SurfelRenderMode::HQ_TWO_PASS,
          HQ_LINKED_LIST = ::gua::PLodPassDescription::SurfelRenderMode::HQ_LINKED_LIST
        };

        /**
         * Constructor. When called without arguments, a new ::gua::PLodPassDescription is created.
         * Otherwise, the given ::gua::PLodPassDescription is used.
         */
        PLodPassDescription(std::shared_ptr< ::gua::PLodPassDescription> const& PLodPassDescription =
                           std::shared_ptr< ::gua::PLodPassDescription>(new ::gua::PLodPassDescription()) );



      public:

        /**
         * Get the wrapped ::gua::PLodPassDescription.
         */
        std::shared_ptr< ::gua::PLodPassDescription> const& getGuaPLodPassDescription() const;

      private:

        std::shared_ptr< ::gua::PLodPassDescription> m_guaPLodPassDescription;


        PLodPassDescription(const PLodPassDescription&);
        PLodPassDescription& operator=(const PLodPassDescription&);

      public:
        SFInt SurfelRenderMode;

        virtual void getSurfelRenderModeCB(const SFInt::GetValueEvent& event);
        virtual void setSurfelRenderModeCB(const SFInt::SetValueEvent& event);

      };

      typedef SingleField<Link<PLodPassDescription> > SFPLodPassDescription;
      typedef MultiField<Link<PLodPassDescription> > MFPLodPassDescription;
        
    }
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::PLodPassDescription> >;
  template class AV_GUA_LOD_DLL MultiField<Link<gua::lod::PLodPassDescription> >;
#endif

}

#endif //AVANGO_GUA_PLOD_PASS_DESCRIPTION_HPP
