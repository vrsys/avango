#ifndef AVANGO_GUA_HEADLESS_SURFACE_HPP
#define AVANGO_GUA_HEADLESS_SURFACE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/HeadlessSurface.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/WindowBase.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::HeadlessSurface
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL HeadlessSurface : public av::gua::WindowBase
    {
      AV_FC_DECLARE();

    public:


      /**
       * Constructor. When called without arguments, a new
       * ::gua::HeadlessSurface is created. Otherwise, the given
       * ::gua::HeadlessSurface is used.
       */
      HeadlessSurface(std::shared_ptr< ::gua::HeadlessSurface> const& guaWindow =
             std::shared_ptr< ::gua::HeadlessSurface> (new ::gua::HeadlessSurface()));


    public:

      /**
       * Get the wrapped ::gua::HeadlessSurface.
       */
      std::shared_ptr< ::gua::HeadlessSurface> const& getGuaWindow() const;

    private:

      std::shared_ptr< ::gua::HeadlessSurface> m_guaWindow;

      HeadlessSurface(const HeadlessSurface&) = delete;
      HeadlessSurface& operator=(const HeadlessSurface&) = delete;
    };

    typedef SingleField<Link<HeadlessSurface> > SFHeadlessSurface;
    typedef MultiField<Link<HeadlessSurface> > MFHeadlessSurface;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::HeadlessSurface> >;
  template class AV_GUA_DLL MultiField<Link<gua::HeadlessSurface> >;
#endif

}

#endif //AVANGO_GUA_HEADLESS_SURFACE_HPP
