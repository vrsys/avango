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

#if !defined(AVANGO_OSG_VIEWER_CAMERA_H)
#define AVANGO_OSG_VIEWER_CAMERA_H

/**
 * \file
 * \ingroup av_osg_viewer
 */

#include <osg/Camera>
#include <osgUtil/SceneView>

#include <avango/osg/Object.h>
#include <avango/osg/Fields.h>
#include <avango/osg/viewer/GraphicsWindow.h>
#include <avango/osg/viewer/windows_specific_osg_viewer.h>

namespace av
{
  namespace osg
  {
    namespace viewer
    {
      typedef ::osg::Camera OsgCamera;

      /**
       * Wrapper for ::osg::Camera
       * In the viewer directory because it depends on GraphicsWindow
       * Derived from av::osg::Object to prevent usage in the scene graph,
       * because OSG uses it internally as root node.
       *
       * \ingroup av_osg_viewer
       */
      class AV_OSG_VIEWER_DLL Camera : public av::osg::Object
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. When called without arguments, a new ::osg::Camera is created.
         * Otherwise, the given ::osg::Camera is used.
         */
        Camera(OsgCamera* osgcamera = new OsgCamera());

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
        virtual ~Camera();

      public:

        /**
         * Window indirectly sets and gets the GraphicsContext of the wrapped ::osg::Camera.
         */
        SFGraphicsWindow Window;

        /**
         * Defines the viewport in normalized window coordinates.
         * PositionX, positionY, width and height.
         * Initial values are 0, 0, 1, 1.
         */
        SFVec4 Viewport;

        /**
         * Defines the distance to the near and far OpenGL clipping plane.
         */
        SFDouble Near;
        SFDouble Far;

        /**
         * Defines the background color.
         * Default is Vec4(0,0,0,1).
         */
        SFVec4 BackgroundColor;

        /**
         * Defines the orientation and translation of the viewer
         * relative to the root node coordinate system.
         * Default is 0.6 meter behind the center.
         */
        SFMatrix ViewerTransform;

        /**
         * Defines the orientation and translation of the screen (centered x-y-plane).
         * Default is 0.6 meter in front of the viewer coordinate system.
         */
        SFMatrix ScreenTransform;

        /**
         * True if ScreenTransform is relative to the viewer coordinate system (default).
         * False if ScreenTransform is relative to the root node coordinate system.
         */
        SFBool ScreenRelativeToViewer;

        /**
         * Defines the orientation and translation of the eyes.
         * Default is the center of the viewer coordinate system.
         */
        SFMatrix EyeTransform;

        /**
         * True if EyeTransform is relative to the viewer coordinate system (default).
         * False if EyeTransform is relative to the root node coordinate system.
         */
        SFBool EyeRelativeToViewer;

        /**
         * Defines the additional eye offset for stereo. Default is 0.
         * Use 0.03 for right eye and -0.03 for left eye in passive stereo setup.
         * Use 0.03 for active stereo setup.
         */
        SFDouble EyeOffset;

        /**
         * Outputs the ModelViewProjection matrix.
         * For active stereo setups, the clip space for both eyes is
         * combined to produce correct view frustum clipping.
         */
        SFMatrix ModelViewProjection;

        /**
         * Outputs the transformation of the mouse pointer position
         * on the screen in 3D relative to the root node coordinate system.
         * The direction is along the perspective viewing direction.
         */
        SFMatrix MouseTransform;

        /**
         * Outputs the transformation of the mouse pointer position
         * on the screen in 3D relative to the viewer coordinate system.
         * The direction is along the perspective viewing direction.
         */
        SFMatrix MouseViewerTransform;

        /**
         * Outputs the transformation of the mouse pointer position
         * on the near clipping plane in 3D relative to the root node coordinate system.
         * The direction is along the perspective viewing direction.
         */
        SFMatrix MouseNearTransform;

        /* virtual */ void fieldHasChangedLocalSideEffect(const av::Field& field);
        /* virtual */ void evaluateLocalSideEffect();
        /* virtual */ void evaluate();

        /**
         * Outputs the projection matrix of the osg camera
         */
        SFMatrix ProjectionMatrix;

        /**
         * Outputs the view matrix (modelview) of the osg camera
         */
        SFMatrix ViewMatrix;

        /**
         * Get the wrapped ::osg::Camera.
         */
        ::osg::Camera* getOsgCamera() const;

        /**
         * Get screen and eye transformation relative to the viewer coordinate system.
         */
        void getScreenAndEyeTrans(::osg::Matrix& screenTrans, ::osg::Matrix& eyeTrans);

        /**
         * Compute projection matrix from screen and eye transformation.
         */
        ::osg::Matrix calcProjectionMatrix(const ::osg::Matrix& screenTrans,
                                           const ::osg::Matrix& eyeTrans);

      protected:

        virtual void windowChangedCB();
        virtual void sizeChangedCB();

      private:

        ::osg::Camera *mOsgCamera;
        Link<av::osg::viewer::GraphicsWindow> mWindow;
        ::osg::ref_ptr< ::osgUtil::SceneView::ComputeStereoMatricesCallback> mStereoCallback;
        av::osg::viewer::GraphicsWindow::CallbackHandle mWindowCallbackHandle, mSizeCallbackHandle;
        bool mViewportChanged;
      };

      typedef SingleField<Link<Camera> > SFCamera;
      typedef MultiField<Link<Camera> > MFCamera;

    } // namespace viewer

  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_VIEWER_DLL SingleField<Link<osg::viewer::Camera> >;
  template class AV_OSG_VIEWER_DLL MultiField<Link<osg::viewer::Camera> >;
#endif

} // namespace av

#endif
