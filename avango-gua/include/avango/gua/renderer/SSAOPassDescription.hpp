#ifndef AVANGO_GUA_SSAO_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_SSAO_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/SSAOPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::SSAOPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL SSAOPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::SSAOPassDescription is created.
       * Otherwise, the given ::gua::SSAOPassDescription is used.
       */
      SSAOPassDescription(std::shared_ptr< ::gua::SSAOPassDescription> const& SSAOPassDescription =
                                std::shared_ptr< ::gua::SSAOPassDescription>(new ::gua::SSAOPassDescription()) );


    public:

      SFFloat Radius;
      SFFloat Intensity;
      SFFloat Falloff;

      virtual void getRadiusCB(const SFFloat::GetValueEvent& event);
      virtual void setRadiusCB(const SFFloat::SetValueEvent& event);

      virtual void getIntensityCB(const SFFloat::GetValueEvent& event);
      virtual void setIntensityCB(const SFFloat::SetValueEvent& event);

      virtual void getFalloffCB(const SFFloat::GetValueEvent& event);
      virtual void setFalloffCB(const SFFloat::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::SSAOPassDescription.
       */
      std::shared_ptr< ::gua::SSAOPassDescription> const& getGuaSSAOPassDescription() const;

    private:

      std::shared_ptr< ::gua::SSAOPassDescription> m_guaSSAOPassDescription;


      SSAOPassDescription(const SSAOPassDescription&);
      SSAOPassDescription& operator=(const SSAOPassDescription&);
    };

    typedef SingleField<Link<SSAOPassDescription> > SFSSAOPassDescription;
    typedef MultiField<Link<SSAOPassDescription> > MFSSAOPassDescription;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SSAOPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::SSAOPassDescription> >;
#endif

}

#endif //AVANGO_GUA_SSAO_PASS_DESCRIPTION_HPP
