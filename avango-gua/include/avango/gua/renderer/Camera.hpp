#ifndef AVANGO_GUA_CAMERA_HPP
#define AVANGO_GUA_CAMERA_HPP

/**
 * \file
 * \ingroup av_gua
 */

#include <gua/renderer/Camera.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/FieldContainer.h>

namespace av
{
  namespace gua
  {
    /**
     * Wrapper for ::gua::Camera
     *
     * \ingroup av_gua
     */
    class AV_GUA_DLL Camera : public av::FieldContainer
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor. When called without arguments, a new ::gua::Camera is created.
       * Otherwise, the given ::gua::Camera is used.
       */
      Camera(::gua::Camera* guaCamera = new ::gua::Camera());

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~Camera();

    public:

      SFString LeftEye;
      SFString RightEye;
      SFString LeftScreen;
      SFString RightScreen;
      SFString SceneGraph;
      SFString RenderMask;

      virtual void getLeftEyeCB(const SFString::GetValueEvent& event);
      virtual void setLeftEyeCB(const SFString::SetValueEvent& event);

      virtual void getRightEyeCB(const SFString::GetValueEvent& event);
      virtual void setRightEyeCB(const SFString::SetValueEvent& event);

      virtual void getLeftScreenCB(const SFString::GetValueEvent& event);
      virtual void setLeftScreenCB(const SFString::SetValueEvent& event);

      virtual void getRightScreenCB(const SFString::GetValueEvent& event);
      virtual void setRightScreenCB(const SFString::SetValueEvent& event);

      virtual void getSceneGraphCB(const SFString::GetValueEvent& event);
      virtual void setSceneGraphCB(const SFString::SetValueEvent& event);

      virtual void getRenderMaskCB(const SFString::GetValueEvent& event);
      virtual void setRenderMaskCB(const SFString::SetValueEvent& event);

      /**
       * Get the wrapped ::gua::Camera.
       */
      ::gua::Camera* getGuaCamera() const;

    public:



    private:

      ::gua::Camera *m_guaCamera;

      Camera(const Camera&);
      Camera& operator=(const Camera&);
    };

    typedef SingleField<Link<Camera> > SFCamera;
    typedef MultiField<Link<Camera> > MFCamera;

  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_GUA_DLL SingleField<Link<gua::Camera> >;
  template class AV_GUA_DLL MultiField<Link<gua::Camera> >;
#endif

}

#endif //AVANGO_GUA_CAMERA_HPP
