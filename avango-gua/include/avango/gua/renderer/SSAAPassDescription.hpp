#ifndef AVANGO_GUA_SSAA_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_SSAA_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/SSAAPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::SSAAPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL SSAAPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      enum SSAAMode {
        FAST_FXAA = static_cast<unsigned>(::gua::SSAAPassDescription::SSAAMode::FAST_FXAA),
        FXAA311 = static_cast<unsigned>(::gua::SSAAPassDescription::SSAAMode::FXAA311),
        DISABLED = static_cast<unsigned>(::gua::SSAAPassDescription::SSAAMode::DISABLED)
      };

      /**
       * Constructor. When called without arguments, a new ::gua::SSAAPassDescription is created.
       * Otherwise, the given ::gua::SSAAPassDescription is used.
       */
      SSAAPassDescription(std::shared_ptr< ::gua::SSAAPassDescription> const& SSAAPassDescription =
                             std::shared_ptr< ::gua::SSAAPassDescription>(new ::gua::SSAAPassDescription()) );



    public:

      SFUInt   Mode;

      // This can effect sharpness.
      // 1.00 - upper limit (softer)
      // 0.75 - default amount of filtering
      // 0.50 - lower limit (sharper, less sub-pixel aliasing removal)
      // 0.25 - almost off
      // 0.00 - completely off
      SFFloat  FxaaQualitySubpix;

      // The minimum amount of local contrast required to apply algorithm.
      // 0.333 - too little (faster)
      // 0.250 - low quality
      // 0.166 - default
      // 0.125 - high quality
      // 0.063 - overkill (slower)
      SFFloat  FxaaEdgeThreshold;

      // Trims the algorithm from processing darks.
      // 0.0833 - upper limit (default, the start of visible unfiltered edges)
      // 0.0625 - high quality (faster)
      // 0.0312 - visible limit (slower)
      SFFloat  FxaaThresholdMin;

      virtual void getModeCB(const SFUInt::GetValueEvent& event);
      virtual void setModeCB(const SFUInt::SetValueEvent& event);

      virtual void getFxaaQualitySubpixCB(const SFFloat::GetValueEvent& event);
      virtual void setFxaaQualitySubpixCB(const SFFloat::SetValueEvent& event);

      virtual void getFxaaEdgeThresholdCB(const SFFloat::GetValueEvent& event);
      virtual void setFxaaEdgeThresholdCB(const SFFloat::SetValueEvent& event);

      virtual void getFxaaThresholdMinCB(const SFFloat::GetValueEvent& event);
      virtual void setFxaaThresholdMinCB(const SFFloat::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::SSAAPassDescription.
       */
      std::shared_ptr< ::gua::SSAAPassDescription> const& getGuaSSAAPassDescription() const;

    private:

      std::shared_ptr< ::gua::SSAAPassDescription> m_guaSSAAPassDescription;


      SSAAPassDescription(const SSAAPassDescription&);
      SSAAPassDescription& operator=(const SSAAPassDescription&);
    };

    using SFSSAAPassDescription = SingleField<Link<SSAAPassDescription> >;
    using MFSSAAPassDescription = MultiField<Link<SSAAPassDescription> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::SSAAPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::SSAAPassDescription> >;
#endif

}

#endif //AVANGO_GUA_SSAA_PASS_DESCRIPTION_HPP
