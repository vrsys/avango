#ifndef AVANGO_GUA_GLFW_WINDOW_HPP
#define AVANGO_GUA_GLFW_WINDOW_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/GlfwWindow.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/WindowBase.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::GlfwWindow
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL GlfwWindow : public av::gua::WindowBase
    {
      AV_FC_DECLARE();

    public:


      /**
       * Constructor. When called without arguments, a new ::gua::GlfwWindow is created.
       * Otherwise, the given ::gua::GlfwWindow is used.
       */
      GlfwWindow(std::shared_ptr< ::gua::GlfwWindow> const& guaGlfwWindow =
             std::shared_ptr< ::gua::GlfwWindow> (new ::gua::GlfwWindow()));


    public:

      /**
       * Get the wrapped ::gua::GlfwWindow.
       */
      std::shared_ptr< ::gua::GlfwWindow> const& getGuaGlfwWindow() const;

    public:



    private:

      std::shared_ptr< ::gua::GlfwWindow> m_guaGlfwWindow;

      GlfwWindow(const GlfwWindow&);
      GlfwWindow& operator=(const GlfwWindow&);
    };

    typedef SingleField<Link<GlfwWindow> > SFGlfwWindow;
    typedef MultiField<Link<GlfwWindow> > MFGlfwWindow;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::GlfwWindow> >;
  template class AV_GUA_DLL MultiField<Link<gua::GlfwWindow> >;
#endif

}

#endif //AVANGO_GUA_GLFW_WINDOW_HPP
