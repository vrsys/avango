#ifndef AVANGO_GUA_GUI_RESOURCE_HPP
#define AVANGO_GUA_GUI_RESOURCE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/gui/GuiResource.hpp>
#include <gua/math/math.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::GuiResource
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL GuiResource : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::GuiResource is created.
       * Otherwise, the given ::gua::GuiResource is used.
       */
      GuiResource(std::shared_ptr< ::gua::GuiResource> guaGuiResource =
                  std::shared_ptr< ::gua::GuiResource>(new ::gua::GuiResource()));

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~GuiResource();

    public:

      /**
       * Get the wrapped ::gua::GuiResource.
       */
      std::shared_ptr< ::gua::GuiResource> getGuaGuiResource() const;
      unsigned     getUserDataHandle() const;


    public:

      void init(std::string const& name, std::string const& url,
                ::gua::math::vec2 const& size);

    private:

      std::shared_ptr< ::gua::GuiResource> m_guaGuiResource;

      GuiResource(const GuiResource&);
      GuiResource& operator=(const GuiResource&);
    };

    typedef SingleField<Link<GuiResource> > SFGuiResource;
    typedef MultiField<Link<GuiResource> > MFGuiResource;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::GuiResource> >;
  template class AV_GUA_DLL MultiField<Link<gua::GuiResource> >;
#endif

}

#endif //AVANGO_GUA_GUI_RESOURCE_HPP
