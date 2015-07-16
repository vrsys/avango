#ifndef AVANGO_GUA_VOLUME_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_VOLUME_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/volume/VolumePass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::VolumePassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL VolumePassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::VolumePassDescription is created.
       * Otherwise, the given ::gua::VolumePassDescription is used.
       */
      VolumePassDescription(std::shared_ptr< ::gua::VolumePassDescription> const& VolumePassDescription =
                             std::shared_ptr< ::gua::VolumePassDescription>(new ::gua::VolumePassDescription()) );



    public:

      /**
       * Get the wrapped ::gua::VolumePassDescription.
       */
      std::shared_ptr< ::gua::VolumePassDescription> const& getGuaVolumePassDescription() const;

    private:

      std::shared_ptr< ::gua::VolumePassDescription> m_guaVolumePassDescription;


      VolumePassDescription(const VolumePassDescription&);
      VolumePassDescription& operator=(const VolumePassDescription&);
    };

    using SFVolumePassDescription = SingleField<Link<VolumePassDescription> >;
    using MFVolumePassDescription = MultiField<Link<VolumePassDescription> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::VolumePassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::VolumePassDescription> >;
#endif

}

#endif //AVANGO_GUA_VOLUME_PASS_DESCRIPTION_HPP
