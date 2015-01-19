#ifndef AVANGO_GUA_LIGHT_VISIBILITY_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_LIGHT_VISIBILITY_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/LightVisibilityPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::LightVisibilityPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL LightVisibilityPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      enum RasterizationModeEnum {
        AUTO = static_cast<unsigned>(::gua::LightVisibilityPassDescription::AUTO),
        SIMPLE = static_cast<unsigned>(::gua::LightVisibilityPassDescription::SIMPLE),
        CONSERVATIVE = static_cast<unsigned>(::gua::LightVisibilityPassDescription::CONSERVATIVE),
        MULTISAMPLED_2 = static_cast<unsigned>(::gua::LightVisibilityPassDescription::MULTISAMPLED_2),
        MULTISAMPLED_4 = static_cast<unsigned>(::gua::LightVisibilityPassDescription::MULTISAMPLED_4),
        MULTISAMPLED_8 = static_cast<unsigned>(::gua::LightVisibilityPassDescription::MULTISAMPLED_8),
        MULTISAMPLED_16 = static_cast<unsigned>(::gua::LightVisibilityPassDescription::MULTISAMPLED_16),
        FULLSCREEN_FALLBACK = static_cast<unsigned>(::gua::LightVisibilityPassDescription::FULLSCREEN_FALLBACK)
      };

      /**
       * Constructor. When called without arguments, a new ::gua::LightVisibilityPassDescription is created.
       * Otherwise, the given ::gua::LightVisibilityPassDescription is used.
       */
      LightVisibilityPassDescription(std::shared_ptr< ::gua::LightVisibilityPassDescription> const& LightVisibilityPassDescription =
                                std::shared_ptr< ::gua::LightVisibilityPassDescription>(new ::gua::LightVisibilityPassDescription()) );


    public:

      SFUInt RasterizationMode;
      SFUInt TilePower;

      virtual void getRasterizationModeCB(const SFUInt::GetValueEvent& event);
      virtual void setRasterizationModeCB(const SFUInt::SetValueEvent& event);

      virtual void getTilePowerCB(const SFUInt::GetValueEvent& event);
      virtual void setTilePowerCB(const SFUInt::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::LightVisibilityPassDescription.
       */
      std::shared_ptr< ::gua::LightVisibilityPassDescription> const& getGuaLightVisibilityPassDescription() const;

    private:

      std::shared_ptr< ::gua::LightVisibilityPassDescription> m_guaLightVisibilityPassDescription;


      LightVisibilityPassDescription(const LightVisibilityPassDescription&);
      LightVisibilityPassDescription& operator=(const LightVisibilityPassDescription&);
    };

    typedef SingleField<Link<LightVisibilityPassDescription> > SFLightVisibilityPassDescription;
    typedef MultiField<Link<LightVisibilityPassDescription> > MFLightVisibilityPassDescription;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::LightVisibilityPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::LightVisibilityPassDescription> >;
#endif

}

#endif //AVANGO_GUA_LIGHT_VISIBILITY_PASS_DESCRIPTION_HPP
