#ifndef AVANGO_OCULUS_OCULUSWINDOW_HPP
#define AVANGO_OCULUS_OCULUSWINDOW_HPP

#include <gua/OculusWindow.hpp>

#include <avango/gua/renderer/GlfwWindow.hpp>

#include <gua/math/math.hpp>
#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

#include <avango/oculus/windows_specific.hpp>

namespace av
{
  namespace oculus
  {
    /**
    * Wrapper for ::gua::OculusWindow
    */

    class AV_OCULUS_DLL OculusWindow : public av::gua::GlfwWindow
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
        ::av::gua::SFMatrix                 SensorOrientation;
        ::av::gua::SFVec2ui                 Resolution;
        ::av::gua::SFVec2ui                 EyeResolution;
        ::av::gua::SFVec2                   LeftScreenSize;
        ::av::gua::SFVec2                   RightScreenSize;
        ::av::gua::SFVec3                   LeftScreenTranslation;
        ::av::gua::SFVec3                   RightScreenTranslation;
        ::av::SFFloat                       EyeDistance;

       public:
         /**
          * Get the wrapped ::gua::OculusRift
          */
         std::shared_ptr< ::gua::OculusWindow> const& getGuaOculusWindow() const;

       private:
        std::shared_ptr< ::gua::OculusWindow> m_guaOculusWindow;

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
