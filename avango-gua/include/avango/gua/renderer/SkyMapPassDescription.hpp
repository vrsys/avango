#ifndef AVANGO_GUA_SKY_MAP_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_SKY_MAP_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/SkyMapPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::SkyMapPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL SkyMapPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:


      /**
       * Constructor. When called without arguments, a new ::gua::SkyMapPassDescription is created.
       * Otherwise, the given ::gua::SkyMapPassDescription is used.
       */
      SkyMapPassDescription(std::shared_ptr< ::gua::SkyMapPassDescription> const& SkyMapPassDescription =
                            std::shared_ptr< ::gua::SkyMapPassDescription>(new ::gua::SkyMapPassDescription()) );


    public:

      SFString OutputTextureName;
      SFColor  LightColor;
      SFVec3   LightDirection;
      SFColor  GroundColor;

      virtual void getOutputTextureNameCB(const SFString::GetValueEvent& event);
      virtual void setOutputTextureNameCB(const SFString::SetValueEvent& event);

      virtual void getLightColorCB(const SFColor::GetValueEvent& event);
      virtual void setLightColorCB(const SFColor::SetValueEvent& event);

      virtual void getLightDirectionCB(const SFVec3::GetValueEvent& event);
      virtual void setLightDirectionCB(const SFVec3::SetValueEvent& event);

      virtual void getGroundColorCB(const SFColor::GetValueEvent& event);
      virtual void setGroundColorCB(const SFColor::SetValueEvent& event);

       /**
       * Get the wrapped ::gua::SkyMapPassDescription.
       */
      std::shared_ptr< ::gua::SkyMapPassDescription> const& getGuaSkyMapPassDescription() const;

    private:

      std::shared_ptr< ::gua::SkyMapPassDescription> m_guaSkyMapPassDescription;


      SkyMapPassDescription(const SkyMapPassDescription&);
      SkyMapPassDescription& operator=(const SkyMapPassDescription&);
    };

    typedef SingleField<Link<SkyMapPassDescription> > SFSkyMapPassDescription;
    typedef MultiField<Link<SkyMapPassDescription> > MFSkyMapPassDescription;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SkyMapPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::SkyMapPassDescription> >;
#endif

}

#endif //AVANGO_GUA_SKY_MAP_PASS_DESCRIPTION_HPP
