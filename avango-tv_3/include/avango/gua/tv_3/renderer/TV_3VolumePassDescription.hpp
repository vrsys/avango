#ifndef AVANGO_GUA_TV_3_VOLUME_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_TV_3_VOLUME_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/TV_3VolumePass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    namespace tv_3 
    {
      /**
       * Wrapper for ::gua::TV_3VolumePassDescription
       *
       * \ingroup av_gua
       */
      class AV_GUA_DLL TV_3VolumePassDescription : public av::gua::PipelinePassDescription
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::TV_3VolumePassDescription is created.
         * Otherwise, the given ::gua::TV_3VolumePassDescription is used.
         */
        TV_3VolumePassDescription(std::shared_ptr< ::gua::TV_3VolumePassDescription> const& TV_3VolumePassDescription =
                                   std::shared_ptr< ::gua::TV_3VolumePassDescription>(new ::gua::TV_3VolumePassDescription()) );



      public:

        /**
         * Get the wrapped ::gua::TV_3VolumePassDescription.
         */
        std::shared_ptr< ::gua::TV_3VolumePassDescription> const& getGuaTV_3VolumePassDescription() const;

      private:

        std::shared_ptr< ::gua::TV_3VolumePassDescription> m_guaTV_3VolumePassDescription;


        TV_3VolumePassDescription(const TV_3VolumePassDescription&);
        TV_3VolumePassDescription& operator=(const TV_3VolumePassDescription&);
      };

      using SFTV_3VolumePassDescription = SingleField<Link<TV_3VolumePassDescription> >;
      using MFTV_3VolumePassDescription = MultiField<Link<TV_3VolumePassDescription> >;
    }
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::TV_3VolumePassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::TV_3VolumePassDescription> >;
#endif

}

#endif //AVANGO_GUA_TV_3_VOLUME_PASS_DESCRIPTION_HPP
