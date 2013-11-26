#ifndef AVANGO_GUA_WINDOW_HPP
#define AVANGO_GUA_WINDOW_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Window.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::Window
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Window : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      enum Mode {
        MONO = static_cast<unsigned>(::gua::StereoMode::MONO),
        SIDE_BY_SIDE = static_cast<unsigned>(::gua::StereoMode::SIDE_BY_SIDE),
        ANAGLYPH_RED_GREEN = static_cast<unsigned>(::gua::StereoMode::ANAGLYPH_RED_GREEN),
        ANAGLYPH_RED_CYAN = static_cast<unsigned>(::gua::StereoMode::ANAGLYPH_RED_CYAN),
        CHECKERBOARD = static_cast<unsigned>(::gua::StereoMode::CHECKERBOARD)
      };

      /**
       * Constructor. When called without arguments, a new ::gua::Window is created.
       * Otherwise, the given ::gua::Window is used.
       */
      Window(::gua::Window* guaWindow = new ::gua::Window());

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Window();

    public:

      SFVec2ui Size;
      SFString Title;
      SFString Display;

      SFUInt   StereoMode;
      SFVec2ui LeftResolution;
      SFVec2ui LeftPosition;
      SFVec2ui RightResolution;
      SFVec2ui RightPosition;

      SFBool   EnableVsync;

      SFString WarpMatrixRedRight;
      SFString WarpMatrixGreenRight;
      SFString WarpMatrixBlueRight;
      SFString WarpMatrixRedLeft;
      SFString WarpMatrixGreenLeft;
      SFString WarpMatrixBlueLeft;

      virtual void getSizeCB(const SFVec2ui::GetValueEvent& event);
      virtual void setSizeCB(const SFVec2ui::SetValueEvent& event);

      virtual void getTitleCB(const SFString::GetValueEvent& event);
      virtual void setTitleCB(const SFString::SetValueEvent& event);

      virtual void getDisplayCB(const SFString::GetValueEvent& event);
      virtual void setDisplayCB(const SFString::SetValueEvent& event);

      virtual void getStereoModeCB(const SFUInt::GetValueEvent& event);
      virtual void setStereoModeCB(const SFUInt::SetValueEvent& event);

      virtual void getLeftResolutionCB(const SFVec2ui::GetValueEvent& event);
      virtual void setLeftResolutionCB(const SFVec2ui::SetValueEvent& event);

      virtual void getLeftPositionCB(const SFVec2ui::GetValueEvent& event);
      virtual void setLeftPositionCB(const SFVec2ui::SetValueEvent& event);

      virtual void getRightResolutionCB(const SFVec2ui::GetValueEvent& event);
      virtual void setRightResolutionCB(const SFVec2ui::SetValueEvent& event);

      virtual void getRightPositionCB(const SFVec2ui::GetValueEvent& event);
      virtual void setRightPositionCB(const SFVec2ui::SetValueEvent& event);

      virtual void getEnableVsyncCB(const SFBool::GetValueEvent& event);
      virtual void setEnableVsyncCB(const SFBool::SetValueEvent& event);

      virtual void getWarpMatrixRedRight(const SFString::GetValueEvent& event);
      virtual void setWarpMatrixRedRight(const SFString::SetValueEvent& event);

      virtual void getWarpMatrixGreenRight(const SFString::GetValueEvent& event);
      virtual void setWarpMatrixGreenRight(const SFString::SetValueEvent& event);

      virtual void getWarpMatrixBlueRight(const SFString::GetValueEvent& event);
      virtual void setWarpMatrixBlueRight(const SFString::SetValueEvent& event);

      virtual void getWarpMatrixRedLeft(const SFString::GetValueEvent& event);
      virtual void setWarpMatrixRedLeft(const SFString::SetValueEvent& event);

      virtual void getWarpMatrixGreenLeft(const SFString::GetValueEvent& event);
      virtual void setWarpMatrixGreenLeft(const SFString::SetValueEvent& event);

      virtual void getWarpMatrixBlueLeft(const SFString::GetValueEvent& event);
      virtual void setWarpMatrixBlueLeft(const SFString::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::Window.
       */
      ::gua::Window* getGuaWindow() const;

    public:



    private:

      ::gua::Window *m_guaWindow;

      Window(const Window&);
      Window& operator=(const Window&);
    };

    typedef SingleField<Link<Window> > SFWindow;
    typedef MultiField<Link<Window> > MFWindow;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Window> >;
  template class AV_GUA_DLL MultiField<Link<gua::Window> >;
#endif

}

#endif //AVANGO_GUA_WINDOW_HPP
