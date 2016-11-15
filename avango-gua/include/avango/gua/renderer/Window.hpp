#ifndef AVANGO_GUA_WINDOW_HPP
#define AVANGO_GUA_WINDOW_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Window.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/WindowBase.hpp>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::Window
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Window : public av::gua::WindowBase
    {
      AV_FC_DECLARE();

    public:


      /**
       * Constructor. When called without arguments, a new ::gua::Window is created.
       * Otherwise, the given ::gua::Window is used.
       */
      Window(std::shared_ptr< ::gua::Window> const& guaWindow =
             std::shared_ptr< ::gua::Window> (new ::gua::Window()));


    public:

      /**
       * Get the wrapped ::gua::Window.
       */
      std::shared_ptr< ::gua::Window> const& getGuaWindow() const;

    public:

      SFUInt   SwapGroup;
      SFUInt   SwapBarrier;

      virtual void getSwapGroupCB(const SFUInt::GetValueEvent& event);
      virtual void setSwapGroupCB(const SFUInt::SetValueEvent& event);

      virtual void getSwapBarrierCB(const SFUInt::GetValueEvent& event);
      virtual void setSwapBarrierCB(const SFUInt::SetValueEvent& event);


    private:

      std::shared_ptr< ::gua::Window> m_guaWindow;

      Window(const Window&);
      Window& operator=(const Window&);
    };

    using SFWindow = SingleField<Link<Window> >;
    using MFWindow = MultiField<Link<Window> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Window> >;
  template class AV_GUA_DLL MultiField<Link<gua::Window> >;
#endif

}

#endif //AVANGO_GUA_WINDOW_HPP
