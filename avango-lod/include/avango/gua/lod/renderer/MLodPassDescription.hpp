#ifndef AVANGO_GUA_MLOD_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_MLOD_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/MLodPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    namespace lod {
      /**
       * Wrapper for ::gua::MLodPassDescription
       *
       * \ingroup av_gua
       */
      class AV_GUA_LOD_DLL MLodPassDescription : public av::gua::PipelinePassDescription
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::MLodPassDescription is created.
         * Otherwise, the given ::gua::MLodPassDescription is used.
         */
        MLodPassDescription(std::shared_ptr< ::gua::MLodPassDescription> const& MLodPassDescription =
                           std::shared_ptr< ::gua::MLodPassDescription>(new ::gua::MLodPassDescription()) );



      public:

        /**
         * Get the wrapped ::gua::MLodPassDescription.
         */
        std::shared_ptr< ::gua::MLodPassDescription> const& getGuaMLodPassDescription() const;

      private:

        std::shared_ptr< ::gua::MLodPassDescription> m_guaMLodPassDescription;


        MLodPassDescription(const MLodPassDescription&);
        MLodPassDescription& operator=(const MLodPassDescription&);
      };

      typedef SingleField<Link<MLodPassDescription> > SFMLodPassDescription;
      typedef MultiField<Link<MLodPassDescription> > MFMLodPassDescription;
        
    }
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_LOD_DLL SingleField<Link<gua::lod::MLodPassDescription> >;
  template class AV_GUA_LOD_DLL MultiField<Link<gua::lod::MLodPassDescription> >;
#endif

}

#endif //AVANGO_GUA_MLOD_PASS_DESCRIPTION_HPP
