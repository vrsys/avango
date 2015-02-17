#ifndef AVANGO_OCULUS_OCULUSWINDOW_HPP
#define AVANGO_OCULUS_OCULUSWINDOW_HPP

#include <gua/OculusWindow.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/Window.hpp>

#include <gua/math/math.hpp>
#include <avango/FieldContainer.h>

#include <avango/oculus/windows_specific.hpp>

namespace av
{
  namespace oculus
  {
    /**
    * Wrapper for ::gua::OculusWindow
    */

    class AV_OCULUS_DLL OculusWindow : public av::gua::Window
    {
      AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::OculusRift is created.
         * Otherwise, the given ::gua::OculusRift is used.
         */

         OculusWindow(std::shared_ptr< ::gua::OculusWindow> const& guaOculusWindow =
                      std::shared_ptr< ::gua::OculusWindow> (new ::gua::OculusWindow(":0.0")));

       protected:

         /**
          * Destructor made protected to prevent allocation on stack.
          */

         virtual ~OculusWindow();

       public:
         /**
          * Get the wrapped ::gua::OculusRift
          */
         std::shared_ptr< ::gua::OculusWindow> const& getGuaOculusWindow() const;

       private:
        std::shared_ptr< ::gua::OculusWindow> m_guaOculusWindow;

        OculusWindow(const OculusWindow&);
        OculusWindow& operator=(const OculusWindow&);
    };

    typedef SingleField<Link<OculusWindow> > SFOculusWindow;
    typedef MultiField<Link<OculusWindow> > MFOculusWindow;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OCULUS_DLL SingleField<Link<oculus::OculusWindow> >;
  template class AV_OCULUS_DLL MultiField<Link<oculus::OculusWindow> >;
#endif
}

#endif //AVANGO_OCULUS_OCULUSWINDOW_HPP
