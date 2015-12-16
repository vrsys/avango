#ifndef AVANGO_GUA_WINDOW_BASE_HPP
#define AVANGO_GUA_WINDOW_BASE_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/WindowBase.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::WindowBase
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL WindowBase : public av::FieldContainer
    {
      AV_FC_DECLARE_ABSTRACT();

    public:

      enum Mode {
        MONO = static_cast<unsigned>(::gua::StereoMode::MONO),
        SIDE_BY_SIDE = static_cast<unsigned>(::gua::StereoMode::SIDE_BY_SIDE),
        ANAGLYPH_RED_GREEN = static_cast<unsigned>(::gua::StereoMode::ANAGLYPH_RED_GREEN),
        ANAGLYPH_RED_CYAN = static_cast<unsigned>(::gua::StereoMode::ANAGLYPH_RED_CYAN),
        CHECKERBOARD = static_cast<unsigned>(::gua::StereoMode::CHECKERBOARD),
        NVIDIA_3D_VISION = static_cast<unsigned>(::gua::StereoMode::NVIDIA_3D_VISION),
        QUAD_BUFFERED = static_cast<unsigned>(::gua::StereoMode::QUAD_BUFFERED)
      };

      /**
       * Constructor. When called without arguments, a new ::gua::WindowBase is created.
       * Otherwise, the given ::gua::WindowBase is used.
       */
      WindowBase(std::shared_ptr< ::gua::WindowBase> const& guaWindowBase);


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
      SFBool   EnableFullscreen;

      SFString WarpMatrixRedRight;
      SFString WarpMatrixGreenRight;
      SFString WarpMatrixBlueRight;
      SFString WarpMatrixRedLeft;
      SFString WarpMatrixGreenLeft;
      SFString WarpMatrixBlueLeft;
      SFFloat  RenderingFPS;

      virtual void open();
      virtual bool is_open() const;
      virtual bool should_close() const;
      virtual void close();
      virtual void process_events();

      virtual void getSizeCB(const SFVec2ui::GetValueEvent& event);
      virtual void setSizeCB(const SFVec2ui::SetValueEvent& event);

      virtual void getTitleCB(const SFString::GetValueEvent& event);
      virtual void setTitleCB(const SFString::SetValueEvent& event);

      virtual void getDisplayCB(const SFString::GetValueEvent& event);
      virtual void setDisplayCB(const SFString::SetValueEvent& event);

      virtual void setEnableFullscreenCB(const SFBool::SetValueEvent& event);
      virtual void getEnableFullscreenCB(const SFBool::GetValueEvent& event);

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

      virtual void getRenderingFPSCB(const SFFloat::GetValueEvent& event);
      virtual void setRenderingFPSCB(const SFFloat::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::WindowBase.
       */
      std::shared_ptr< ::gua::WindowBase> const& getGuaWindowBase() const;

    public:



    private:

      std::shared_ptr< ::gua::WindowBase> m_guaWindowBase;

      WindowBase(const WindowBase&);
      WindowBase& operator=(const WindowBase&);
    };

    using SFWindowBase = SingleField<Link<WindowBase> >;
    using MFWindowBase = MultiField<Link<WindowBase> >;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::WindowBase> >;
  template class AV_GUA_DLL MultiField<Link<gua::WindowBase> >;
#endif

}

#endif //AVANGO_GUA_WINDOW_BASE_HPP
