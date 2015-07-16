#ifndef AVANGO_GUA_BACKGROUND_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_BACKGROUND_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/BackgroundPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::BackgroundPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL BackgroundPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      enum BackgroundMode {
        COLOR = static_cast<unsigned>(::gua::BackgroundPassDescription::COLOR),
        SKYMAP_TEXTURE = static_cast<unsigned>(::gua::BackgroundPassDescription::SKYMAP_TEXTURE),
        QUAD_TEXTURE = static_cast<unsigned>(::gua::BackgroundPassDescription::QUAD_TEXTURE)
      };

      /**
       * Constructor. When called without arguments, a new ::gua::BackgroundPassDescription is created.
       * Otherwise, the given ::gua::BackgroundPassDescription is used.
       */
      BackgroundPassDescription(std::shared_ptr< ::gua::BackgroundPassDescription> const& BackgroundPassDescription =
                                std::shared_ptr< ::gua::BackgroundPassDescription>(new ::gua::BackgroundPassDescription()) );


    public:

      SFColor  Color;
      SFString Texture;
      SFUInt   Mode;

      SFBool   EnableFog;
      SFFloat  FogStart;
      SFFloat  FogEnd;


      virtual void getColorCB(const SFColor::GetValueEvent& event);
      virtual void setColorCB(const SFColor::SetValueEvent& event);

      virtual void getTextureCB(const SFString::GetValueEvent& event);
      virtual void setTextureCB(const SFString::SetValueEvent& event);

      virtual void getModeCB(const SFUInt::GetValueEvent& event);
      virtual void setModeCB(const SFUInt::SetValueEvent& event);

      virtual void getEnableFogCB(const SFBool::GetValueEvent& event);
      virtual void setEnableFogCB(const SFBool::SetValueEvent& event);

      virtual void getFogStartCB(const SFFloat::GetValueEvent& event);
      virtual void setFogStartCB(const SFFloat::SetValueEvent& event);

      virtual void getFogEndCB(const SFFloat::GetValueEvent& event);
      virtual void setFogEndCB(const SFFloat::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::BackgroundPassDescription.
       */
      std::shared_ptr< ::gua::BackgroundPassDescription> const& getGuaBackgroundPassDescription() const;

    private:

      std::shared_ptr< ::gua::BackgroundPassDescription> m_guaBackgroundPassDescription;


      BackgroundPassDescription(const BackgroundPassDescription&);
      BackgroundPassDescription& operator=(const BackgroundPassDescription&);
    };

    using SFBackgroundPassDescription = SingleField<Link<BackgroundPassDescription> >;
    using MFBackgroundPassDescription = MultiField<Link<BackgroundPassDescription> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::BackgroundPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::BackgroundPassDescription> >;
#endif

}

#endif //AVANGO_GUA_BACKGROUND_PASS_DESCRIPTION_HPP
