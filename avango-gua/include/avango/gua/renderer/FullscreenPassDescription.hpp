#ifndef AVANGO_GUA_FULLSCREEN_PASS_DESCRIPTION_HPP
#define AVANGO_GUA_FULLSCREEN_PASS_DESCRIPTION_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/FullscreenPass.hpp>
#include <gua/renderer/Uniform.hpp>

#include <avango/gua/renderer/PipelinePassDescription.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::FullscreenPassDescription
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL FullscreenPassDescription : public av::gua::PipelinePassDescription
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::FullscreenPassDescription is created.
       * Otherwise, the given ::gua::FullscreenPassDescription is used.
       */
      FullscreenPassDescription(std::shared_ptr< ::gua::FullscreenPassDescription> const& FullscreenPassDescription =
                                std::shared_ptr< ::gua::FullscreenPassDescription>(new ::gua::FullscreenPassDescription()) );


    public:

      SFString Source;
      SFString SourceFile;

      virtual void getSourceCB(const SFString::GetValueEvent& event);
      virtual void setSourceCB(const SFString::SetValueEvent& event);

      virtual void getSourceFileCB(const SFString::GetValueEvent& event);
      virtual void setSourceFileCB(const SFString::SetValueEvent& event);

      template <typename T>
      void set_uniform(std::string const& name, T const& value) {
        m_guaFullscreenPassDescription->uniform(name, ::gua::uniform_compatible_type(value));
      }

      /**
       * Get the wrapped ::gua::FullscreenPassDescription.
       */
      std::shared_ptr< ::gua::FullscreenPassDescription> const& getGuaFullscreenPassDescription() const;

    private:

      std::shared_ptr< ::gua::FullscreenPassDescription> m_guaFullscreenPassDescription;


      FullscreenPassDescription(const FullscreenPassDescription&);
      FullscreenPassDescription& operator=(const FullscreenPassDescription&);
    };

    using SFFullscreenPassDescription = SingleField<Link<FullscreenPassDescription> >;
    using MFFullscreenPassDescription = MultiField<Link<FullscreenPassDescription> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::FullscreenPassDescription> >;
  template class AV_GUA_DLL MultiField<Link<gua::FullscreenPassDescription> >;
#endif

}

#endif //AVANGO_GUA_FULLSCREEN_PASS_DESCRIPTION_HPP
