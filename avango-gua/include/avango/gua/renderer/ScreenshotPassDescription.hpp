#ifndef AVANGO_GUA_SCREENSHOT_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_SCREENSHOT_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/ScreenshotPass.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::ScreenshotPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL ScreenshotPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::ScreenshotPassDescription is created.
       * Otherwise, the given ::gua::ScreenshotPassDescription is used.
       */
      ScreenshotPassDescription(std::shared_ptr< ::gua::ScreenshotPassDescription> const& ScreenshotPassDescription =
                                std::shared_ptr< ::gua::ScreenshotPassDescription>(new ::gua::ScreenshotPassDescription()) );


    public:

      /**
       * Get the wrapped ::gua::ScreenshotPassDescription.
       */
      std::shared_ptr< ::gua::ScreenshotPassDescription> const& getGuaScreenshotPassDescription() const;

    private:

      std::shared_ptr< ::gua::ScreenshotPassDescription> m_guaScreenshotPassDescription;


      ScreenshotPassDescription(const ScreenshotPassDescription&) = delete;
      ScreenshotPassDescription& operator=(const ScreenshotPassDescription&) = delete;
    };

    typedef SingleField<Link<ScreenshotPassDescription> > SFScreenshotPassDescription;
    typedef MultiField<Link<ScreenshotPassDescription> > MFScreenshotPassDescription;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::ScreenshotPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::ScreenshotPassDescription> >;
#endif

}

#endif //AVANGO_GUA_SCREENSHOT_PASS_DESCRIPTION_HPP
