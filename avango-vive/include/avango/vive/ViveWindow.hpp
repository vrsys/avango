#ifndef AVANGO_VIVE_VIVEWINDOW_HPP
#define AVANGO_VIVE_VIVEWINDOW_HPP

#include <gua/ViveWindow.hpp>

#include <avango/gua/renderer/GlfwWindow.hpp>

#include <gua/math/math.hpp>
#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

#include <avango/vive/windows_specific.hpp>

namespace av
{
  namespace vive
  {
    /**
    * Wrapper for ::gua::ViveWindow
    */

    class AV_VIVE_DLL ViveWindow : public av::gua::GlfwWindow
    {
      AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::gua::ViveRift is created.
         * Otherwise, the given ::gua::ViveRift is used.
         */

         ViveWindow(std::shared_ptr< ::gua::ViveWindow> const& guaViveWindow =
                      std::shared_ptr< ::gua::ViveWindow> (new ::gua::ViveWindow(":0.0")));

       protected:

         /**
          * Destructor made protected to prevent allocation on stack.
          */

         virtual ~ViveWindow();

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
          * Get the wrapped ::gua::ViveRift
          */
         std::shared_ptr< ::gua::ViveWindow> const& getGuaViveWindow() const;

       private:
        std::shared_ptr< ::gua::ViveWindow> m_guaViveWindow;

        void evaluate() override;

        ViveWindow(const ViveWindow&);
        ViveWindow& operator=(const ViveWindow&);
    };

    using SFViveWindow = SingleField<Link<ViveWindow> >;
    using MFViveWindow = MultiField<Link<ViveWindow> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_VIVE_DLL SingleField<Link<vive::ViveWindow> >;
  template class AV_VIVE_DLL MultiField<Link<vive::ViveWindow> >;
#endif
}

#endif //AVANGO_VIVE_VIVEWINDOW_HPP
