// -*- Mode:C++ -*-

/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2008 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include <avango/osg/viewer/Camera.h>

#include <boost/bind.hpp>

#include <osgViewer/Renderer>
#include <osg/CullSettings>

#include <avango/Logger.h>

#include <cmath>
namespace
{
  av::Logger& logger(av::getLogger("av::osg::viewer::Camera"));

  struct ComputeStereoMatricesCallback : public osgUtil::SceneView::ComputeStereoMatricesCallback
  {
    ComputeStereoMatricesCallback(av::osg::viewer::Camera* camera):
      mCamera(camera)
    {}

    ::osg::Matrixd
    computeLeftEyeProjection(const ::osg::Matrixd& projection) const
    {
      if (mCamera->Window.getValue().isValid() &&
          mCamera->Window.getValue()->ActualWidth.getValue() != 0u)
      {
        ::osg::Matrix screen_trans, eye_trans;
        mCamera->getScreenAndEyeTrans(screen_trans, eye_trans);
        eye_trans.preMult(::osg::Matrix::translate(-mCamera->EyeOffset.getValue(), 0.0, 0.0));
        return mCamera->calcProjectionMatrix(screen_trans, eye_trans);
      }

      return projection;
    }

    ::osg::Matrixd
    computeLeftEyeView(const ::osg::Matrixd& view) const
    {
      if (mCamera->Window.getValue().isValid() &&
          mCamera->Window.getValue()->ActualWidth.getValue() != 0u)
      {
        ::osg::Matrix screen_trans, eye_trans;
        mCamera->getScreenAndEyeTrans(screen_trans, eye_trans);
        eye_trans.preMult(::osg::Matrix::translate(-mCamera->EyeOffset.getValue(), 0.0, 0.0));
        return ::osg::Matrix::inverse(::osg::Matrix::rotate(screen_trans.getRotate()) *
                                      ::osg::Matrix::translate(eye_trans.getTrans()) *
                                      mCamera->ViewerTransform.getValue());
      }

      return view;
    }

    ::osg::Matrixd
    computeRightEyeProjection(const ::osg::Matrixd& projection) const
    {
      if (mCamera->Window.getValue().isValid() &&
          mCamera->Window.getValue()->ActualWidth.getValue() != 0u)
      {
        ::osg::Matrix screen_trans, eye_trans;
        mCamera->getScreenAndEyeTrans(screen_trans, eye_trans);
        eye_trans.preMult(::osg::Matrix::translate(mCamera->EyeOffset.getValue(), 0.0, 0.0));
        return mCamera->calcProjectionMatrix(screen_trans, eye_trans);
      }

      return projection;
    }

    ::osg::Matrixd
    computeRightEyeView(const ::osg::Matrixd& view) const
    {
      if (mCamera->Window.getValue().isValid() &&
          mCamera->Window.getValue()->ActualWidth.getValue() != 0u)
      {
        ::osg::Matrix screen_trans, eye_trans;
        mCamera->getScreenAndEyeTrans(screen_trans, eye_trans);
        eye_trans.preMult(::osg::Matrix::translate(mCamera->EyeOffset.getValue(), 0.0, 0.0));
        return ::osg::Matrix::inverse(::osg::Matrix::rotate(screen_trans.getRotate()) *
                                      ::osg::Matrix::translate(eye_trans.getTrans()) *
                                      mCamera->ViewerTransform.getValue());
      }

      return view;
    }

    av::osg::viewer::Camera *mCamera;
  };
}


AV_FC_DEFINE(av::osg::viewer::Camera);

AV_FIELD_DEFINE(av::osg::viewer::SFCamera);
AV_FIELD_DEFINE(av::osg::viewer::MFCamera);

av::osg::viewer::Camera::Camera(::osg::Camera* osgcamera) :
  Object(osgcamera),
  mOsgCamera(osgcamera),
  mViewportChanged(false)
{
  AV_FC_ADD_FIELD(Window, 0);
  AV_FC_ADD_FIELD(Viewport, ::osg::Vec4(0.0, 0.0, 1.0, 1.0));
  AV_FC_ADD_FIELD(Near, 0.1);
  AV_FC_ADD_FIELD(Far, 500.0);
  AV_FC_ADD_FIELD(BackgroundColor, ::osg::Vec4(0.0, 0.0, 0.0, 1.0));

  AV_FC_ADD_FIELD(ViewerTransform, ::osg::Matrix::translate(0.0, 0.0, 0.6));
  AV_FC_ADD_FIELD(ScreenTransform, ::osg::Matrix::translate(0.0, 0.0, -0.6));
  AV_FC_ADD_FIELD(ScreenRelativeToViewer, true);
  AV_FC_ADD_FIELD(EyeTransform, ::osg::Matrix::identity());
  AV_FC_ADD_FIELD(EyeRelativeToViewer, true);
  AV_FC_ADD_FIELD(EyeOffset, 0.0);

  AV_FC_ADD_FIELD(ModelViewProjection, ::osg::Matrix::identity());
  AV_FC_ADD_FIELD(MouseTransform, ::osg::Matrix::identity());
  AV_FC_ADD_FIELD(MouseViewerTransform, ::osg::Matrix::identity());
  AV_FC_ADD_FIELD(MouseNearTransform, ::osg::Matrix::identity());
  AV_FC_ADD_FIELD(ProjectionMatrix, ::osg::Matrix::identity());
  AV_FC_ADD_FIELD(ViewMatrix, ::osg::Matrix::identity());


  AV_FC_ADD_ADAPTOR_FIELD(EnableAutoComputeNearFarPlane,
                              boost::bind(&Camera::getEnableAutoComputeNearFarPlaneCB, this, _1),
                              boost::bind(&Camera::setEnableAutoComputeNearFarPlaneCB, this, _1));


  mOsgCamera->setReferenceFrame(::osg::Transform::ABSOLUTE_RF);
  mOsgCamera->setDisplaySettings(new ::osg::DisplaySettings);
  mOsgCamera->setComputeNearFarMode(::osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);

  alwaysEvaluate(true);
}

/* virtual */
av::osg::viewer::Camera::~Camera()
{
  if (mWindow.isValid())
  {
    av::osg::viewer::GraphicsWindow::removeCallback(mWindowCallbackHandle);
    av::osg::viewer::GraphicsWindow::removeCallback(mSizeCallbackHandle);
  }
}

/* static */ void
av::osg::viewer::Camera::initClass()
{
  if (!isTypeInitialized())
  {
    av::osg::Object::initClass();
    av::osg::viewer::GraphicsWindow::initClass();

    AV_FC_INIT(av::osg::Object, av::osg::viewer::Camera, true);

    SFCamera::initClass("av::osg::viewer::SFCamera", "av::Field");
    MFCamera::initClass("av::osg::viewer::MFCamera", "av::Field");
  }
}

/* virtual */ void
av::osg::viewer::Camera::fieldHasChangedLocalSideEffect(const av::Field& field)
{
  av::osg::Object::fieldHasChangedLocalSideEffect(field);

  if (&field == &Window)
  {
    if (mWindow != Window.getValue())
    {
      if (mWindow.isValid())
      {
        av::osg::viewer::GraphicsWindow::removeCallback(mWindowCallbackHandle);
        av::osg::viewer::GraphicsWindow::removeCallback(mSizeCallbackHandle);
      }

      mWindow = Window.getValue();

      if (mWindow.isValid())
      {
        mWindowCallbackHandle =
          mWindow->addWindowChangedCallback(boost::bind(&Camera::windowChangedCB, this));
        mSizeCallbackHandle =
          mWindow->addSizeChangedCallback(boost::bind(&Camera::sizeChangedCB, this));
        windowChangedCB();
      }
    }
  }
  else if (&field == &Viewport)
  {
    mViewportChanged = true;
  }
  else if (&field == &BackgroundColor)
  {
    mOsgCamera->setClearColor(BackgroundColor.getValue());
  }
}

/* virtual */ void
av::osg::viewer::Camera::evaluateLocalSideEffect()
{
  av::osg::Object::evaluateLocalSideEffect();

  if (mWindow.isValid())
  {
    evaluateDependency(*mWindow);

    if (mWindow->ActualWidth.getValue() != 0u)
    {
      if (mViewportChanged)
      {
        sizeChangedCB();
        mViewportChanged = false;
      }

      ::osg::Matrix screen_trans, eye_trans;
      getScreenAndEyeTrans(screen_trans, eye_trans);

      ::osgViewer::Renderer *renderer =
        dynamic_cast< ::osgViewer::Renderer*>(mOsgCamera->getRenderer());
      if (renderer != 0 && renderer->getSceneView(0) != 0 && renderer->getSceneView(1) != 0)
      {
        osgUtil::SceneView *scene_view0 = renderer->getSceneView(0);
        osgUtil::SceneView *scene_view1 = renderer->getSceneView(1);

        if (mWindow->StereoMode.getValue() != GraphicsWindow::STEREO_MODE_NONE)
        {
          if (scene_view0->getComputeStereoMatricesCallback() == 0)
          {
            if (!mStereoCallback.valid())
            mStereoCallback = new ComputeStereoMatricesCallback(this);
            scene_view0->setComputeStereoMatricesCallback(mStereoCallback.get());
            scene_view1->setComputeStereoMatricesCallback(mStereoCallback.get());
          }

          if (!mOsgCamera->getDisplaySettings()->getStereo())
          {
            mOsgCamera->getDisplaySettings()->setStereo(true);

            ::osg::DisplaySettings::StereoMode stereo_mode = ::osg::DisplaySettings::QUAD_BUFFER;
            switch (mWindow->StereoMode.getValue()) {
                case GraphicsWindow::STEREO_MODE_ANAGLYPHIC:            stereo_mode = ::osg::DisplaySettings::ANAGLYPHIC;break;
                case GraphicsWindow::STEREO_MODE_HORIZONTAL_SPLIT:      stereo_mode = ::osg::DisplaySettings::HORIZONTAL_SPLIT;break;
                case GraphicsWindow::STEREO_MODE_VERTICAL_SPLIT:        stereo_mode = ::osg::DisplaySettings::VERTICAL_SPLIT;break;
                case GraphicsWindow::STEREO_MODE_HORIZONTAL_INTERLACE:  stereo_mode = ::osg::DisplaySettings::HORIZONTAL_INTERLACE;break;
                case GraphicsWindow::STEREO_MODE_VERTIVAL_INTERLACE:    stereo_mode = ::osg::DisplaySettings::VERTICAL_INTERLACE;break;
                case GraphicsWindow::STEREO_MODE_CHECKERBOARD:          stereo_mode = ::osg::DisplaySettings::CHECKERBOARD;break;
                case GraphicsWindow::STEREO_MODE_QUAD_BUFFER:           stereo_mode = ::osg::DisplaySettings::QUAD_BUFFER;
                default:; // try quad buffer
            }
            mOsgCamera->getDisplaySettings()->setStereoMode(stereo_mode);
          }
        }
        else
        {
          eye_trans.preMult(::osg::Matrix::translate(EyeOffset.getValue(), 0.0, 0.0));

          if (mOsgCamera->getDisplaySettings()->getStereo())
            mOsgCamera->getDisplaySettings()->setStereo(false);
        }
      }

      mOsgCamera->setProjectionMatrix(calcProjectionMatrix(screen_trans, eye_trans));
      mOsgCamera->
        setViewMatrix(::osg::Matrix::inverse(::osg::Matrix::rotate(screen_trans.getRotate()) *
                                             ::osg::Matrix::translate(eye_trans.getTrans()) *
                                             ViewerTransform.getValue()));
      ViewMatrix.setValue(mOsgCamera->getViewMatrix());
    }
  }
}

#define CALCMINMAX(pos, transform) \
{ \
  const ::osg::Vec3 clip_pos = pos * transform; \
  if (pos[0] > 0.0) {if (clip_max[0] < clip_pos[0]) clip_max[0] = clip_pos[0];} \
  else              {if (clip_min[0] > clip_pos[0]) clip_min[0] = clip_pos[0];} \
  if (pos[1] > 0.0) {if (clip_max[1] < clip_pos[1]) clip_max[1] = clip_pos[1];} \
  else              {if (clip_min[1] > clip_pos[1]) clip_min[1] = clip_pos[1];} \
  if (pos[2] > 0.0) {if (clip_max[2] > clip_pos[2]) clip_max[2] = clip_pos[2];} \
  else              {if (clip_min[2] < clip_pos[2]) clip_min[2] = clip_pos[2];} \
}

/* virtual */ void
av::osg::viewer::Camera::evaluate()
{
  av::osg::Object::evaluate();

  if (mWindow.isValid() && mWindow->ActualWidth.getValue() != 0u)
  {
    ::osg::Matrix screen_to_clip = ScreenTransform.getValue();
    if (ScreenRelativeToViewer.getValue())
      screen_to_clip *= ViewerTransform.getValue();
    screen_to_clip *= mOsgCamera->getViewMatrix() * mOsgCamera->getProjectionMatrix();
    const ::osg::Matrix clip_to_screen = ::osg::Matrix::inverse(screen_to_clip);

    const ::osg::Vec2 mouse_pos = mWindow->MousePositionNorm.getValue();
    const ::osg::Vec2 real_window_size = mWindow->getRealWindowSize();
    const ::osg::Vec3 mouse_pos_screen(mouse_pos[0] * real_window_size[0] * 0.5,
                                       mouse_pos[1] * real_window_size[1] * 0.5, 0.0);
    const ::osg::Vec3 mouse_pos_clip = mouse_pos_screen * screen_to_clip;
    const ::osg::Vec3 mouse_far_clip = ::osg::Vec3(mouse_pos_clip[0], mouse_pos_clip[1], 0.999);
    const ::osg::Vec3 mouse_near_clip = ::osg::Vec3(mouse_pos_clip[0], mouse_pos_clip[1], -1.0);
    const ::osg::Vec3 mouse_far_screen = mouse_far_clip * clip_to_screen;
    const ::osg::Vec3 mouse_near_screen = mouse_near_clip * clip_to_screen;
    ::osg::Vec3 mouse_view_dir = mouse_far_screen - mouse_near_screen;
    mouse_view_dir.normalize();
    ::osg::Matrix mouse_trans = ::osg::Matrix::rotate(::osg::Vec3(0.0, 0.0, -1.0), mouse_view_dir) *
      ::osg::Matrix::translate(mouse_pos_screen) * ScreenTransform.getValue();
    ::osg::Matrix mouse_viewer_trans = mouse_trans;
    ::osg::Matrix mouse_near_trans =
      ::osg::Matrix::rotate(::osg::Vec3(0.0, 0.0, -1.0), mouse_view_dir) *
      ::osg::Matrix::translate(mouse_near_screen) * ScreenTransform.getValue();

    if (ScreenRelativeToViewer.getValue())
    {
      mouse_trans *= ViewerTransform.getValue();
      mouse_near_trans *= ViewerTransform.getValue();
    }
    else
      mouse_viewer_trans *= ::osg::Matrix::inverse(ViewerTransform.getValue());

    ::osg::Matrix model_view_proj = mOsgCamera->getViewMatrix() * mOsgCamera->getProjectionMatrix();
    if ((mWindow->StereoMode.getValue() != GraphicsWindow::STEREO_MODE_NONE) && (std::abs(EyeOffset.getValue()) > 0.001))
    {
      if (!mStereoCallback.valid())
        mStereoCallback = new ComputeStereoMatricesCallback(this);

      ::osg::Matrix left_to_center =
        ::osg::Matrix::inverse(mStereoCallback->computeLeftEyeView(::osg::Matrix()) *
          mStereoCallback->computeLeftEyeProjection(::osg::Matrix())) * model_view_proj;
      ::osg::Matrix right_to_center =
        ::osg::Matrix::inverse(mStereoCallback->computeRightEyeView(::osg::Matrix()) *
          mStereoCallback->computeRightEyeProjection(::osg::Matrix())) * model_view_proj;

      ::osg::Vec3 clip_min(-1.0, -1.0, -1.0), clip_max(1.0, 1.0, 1.0);
      CALCMINMAX(::osg::Vec3(-1.0, -1.0, -1.0), left_to_center)
      CALCMINMAX(::osg::Vec3(-1.0, -1.0,  1.0), left_to_center)
      CALCMINMAX(::osg::Vec3(-1.0,  1.0, -1.0), left_to_center)
      CALCMINMAX(::osg::Vec3(-1.0,  1.0,  1.0), left_to_center)
      CALCMINMAX(::osg::Vec3( 1.0, -1.0, -1.0), left_to_center)
      CALCMINMAX(::osg::Vec3( 1.0, -1.0,  1.0), left_to_center)
      CALCMINMAX(::osg::Vec3( 1.0,  1.0, -1.0), left_to_center)
      CALCMINMAX(::osg::Vec3( 1.0,  1.0,  1.0), left_to_center)
      CALCMINMAX(::osg::Vec3(-1.0, -1.0, -1.0), right_to_center)
      CALCMINMAX(::osg::Vec3(-1.0, -1.0,  1.0), right_to_center)
      CALCMINMAX(::osg::Vec3(-1.0,  1.0, -1.0), right_to_center)
      CALCMINMAX(::osg::Vec3(-1.0,  1.0,  1.0), right_to_center)
      CALCMINMAX(::osg::Vec3( 1.0, -1.0, -1.0), right_to_center)
      CALCMINMAX(::osg::Vec3( 1.0, -1.0,  1.0), right_to_center)
      CALCMINMAX(::osg::Vec3( 1.0,  1.0, -1.0), right_to_center)
      CALCMINMAX(::osg::Vec3( 1.0,  1.0,  1.0), right_to_center)

      const ::osg::Vec3 scale = clip_max - clip_min;
      model_view_proj *= ::osg::Matrix::translate(-clip_min) *
                         ::osg::Matrix::scale(2.0 / scale[0], 2.0 / scale[1], 2.0 / scale[2]) *
                         ::osg::Matrix::translate(::osg::Vec3(-1.0, -1.0, -1.0));
    }

    if (ModelViewProjection.getValue() != model_view_proj)
      ModelViewProjection.setValue(model_view_proj);
    if (MouseTransform.getValue() != mouse_trans)
      MouseTransform.setValue(mouse_trans);
    if (MouseViewerTransform.getValue() != mouse_viewer_trans)
      MouseViewerTransform.setValue(mouse_viewer_trans);
    if (MouseNearTransform.getValue() != mouse_near_trans)
      MouseNearTransform.setValue(mouse_near_trans);
    if (ProjectionMatrix.getValue() != mOsgCamera->getProjectionMatrix())
      ProjectionMatrix.setValue(mOsgCamera->getProjectionMatrix());
  }
}

::osg::Camera*
av::osg::viewer::Camera::getOsgCamera() const
{
  return mOsgCamera;
}

void
av::osg::viewer::Camera::getScreenAndEyeTrans(::osg::Matrix& screenTrans, ::osg::Matrix& eyeTrans)
{
  screenTrans = ScreenTransform.getValue();
  eyeTrans = EyeTransform.getValue();

  if (!ScreenRelativeToViewer.getValue() || !EyeRelativeToViewer.getValue())
  {
    const ::osg::Matrix viewer_inv = ::osg::Matrix::inverse(ViewerTransform.getValue());
    if (!ScreenRelativeToViewer.getValue())
      screenTrans *= viewer_inv;
    if (!EyeRelativeToViewer.getValue())
      eyeTrans *= viewer_inv;
  }
}

::osg::Matrix
av::osg::viewer::Camera::calcProjectionMatrix(const ::osg::Matrix& screenTrans,
                                              const ::osg::Matrix& eyeTrans)
{
  const ::osg::Vec3 center = eyeTrans.getTrans() * ::osg::Matrix::inverse(screenTrans);
  const float nearfac = Near.getValue() / center[2];
  const ::osg::Vec2 real_window_size = Window.getValue()->getRealWindowSize();
  const ::osg::Vec2 half_size(0.5 * Viewport.getValue()[2] * real_window_size[0],
                              0.5 * Viewport.getValue()[3] * real_window_size[1]);

  return ::osg::Matrix::frustum((-half_size[0] - center[0]) * nearfac,
                                ( half_size[0] - center[0]) * nearfac,
                                (-half_size[1] - center[1]) * nearfac,
                                ( half_size[1] - center[1]) * nearfac,
                                Near.getValue(), Far.getValue());
}

/* virtual */ void
av::osg::viewer::Camera::windowChangedCB()
{
  mOsgCamera->setGraphicsContext(mWindow.isValid() ? mWindow->getOsgGraphicsWindow() : 0);
  touch();
}

/* virtual */ void
av::osg::viewer::Camera::sizeChangedCB()
{
  if (mWindow.isValid())
  {
    const double width  = mWindow->ActualWidth.getValue();
    const double height = mWindow->ActualHeight.getValue();

    if (width > 0.0 && height > 0.0)
    {
      mOsgCamera->setViewport(static_cast<int>(Viewport.getValue()[0] * width + 0.5),
                              static_cast<int>(Viewport.getValue()[1] * height + 0.5),
                              static_cast<int>(Viewport.getValue()[2] * width + 0.5),
                              static_cast<int>(Viewport.getValue()[3] * height + 0.5));
      touch();
    }
  }
}

/* virtual */ void
av::osg::viewer::Camera::getEnableAutoComputeNearFarPlaneCB(const av::SFBool::GetValueEvent& event) {
	*(event.getValuePtr()) = mOsgCamera->getComputeNearFarMode() == ::osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES;
}
/* virtual */ void
av::osg::viewer::Camera::setEnableAutoComputeNearFarPlaneCB(const av::SFBool::SetValueEvent& event) {
	if(event.getValue()) {
		mOsgCamera->setComputeNearFarMode(::osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
	} else {
		mOsgCamera->setComputeNearFarMode(::osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	}
}
