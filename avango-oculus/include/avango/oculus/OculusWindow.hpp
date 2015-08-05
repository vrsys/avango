#ifndef AVANGO_OCULUS_OCULUSWINDOW_HPP
#define AVANGO_OCULUS_OCULUSWINDOW_HPP

#include <gua/OculusSDK2Window.hpp>

#include <avango/gua/renderer/Window.hpp>

#include <gua/math/math.hpp>
#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

#include <avango/oculus/windows_specific.hpp>

namespace av
{
  namespace oculus
  {
    /**
    * Wrapper for ::gua::OculusSDK2Window
    */

    class AV_OCULUS_DLL OculusWindow : public av::gua::Window
    {
      AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::OculusRift is created.
         * Otherwise, the given ::gua::OculusRift is used.
         */

         OculusWindow(std::shared_ptr< ::gua::OculusSDK2Window> const& guaOculusWindow =
                      std::shared_ptr< ::gua::OculusSDK2Window> (new ::gua::OculusSDK2Window(":0.0")));

       protected:

         /**
          * Destructor made protected to prevent allocation on stack.
          */

         virtual ~OculusWindow();

       public:
        ::av::gua::SFMatrix                  Orientation;

       public:
         /**
          * Get the wrapped ::gua::OculusRift
          */
         std::shared_ptr< ::gua::OculusSDK2Window> const& getGuaOculusWindow() const;

       private:
        std::shared_ptr< ::gua::OculusSDK2Window> m_guaOculusWindow;

        void evaluate() override;

        OculusWindow(const OculusWindow&);
        OculusWindow& operator=(const OculusWindow&);
    };

    using SFOculusWindow = SingleField<Link<OculusWindow> >;
    using MFOculusWindow = MultiField<Link<OculusWindow> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OCULUS_DLL SingleField<Link<oculus::OculusWindow> >;
  template class AV_OCULUS_DLL MultiField<Link<oculus::OculusWindow> >;
#endif
}

#endif //AVANGO_OCULUS_OCULUSWINDOW_HPP
