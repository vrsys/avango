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

#if !defined(AVANGO_OSG_VIEWER_GRAPHICSWINDOW_H)
#define AVANGO_OSG_VIEWER_GRAPHICSWINDOW_H

/**
 * \file
 * \ingroup av_osg_viewer
 */

#include <boost/signal.hpp>

#include <osgViewer/GraphicsWindow>

#include <avango/Object.h>
#include <avango/osg/Fields.h>
#include <avango/osg/viewer/windows_specific_osg_viewer.h>

namespace av
{
  namespace osg
  {
    namespace viewer
    {
      class GraphicsWindow;

      typedef SingleField<Link<GraphicsWindow> > SFGraphicsWindow;
      typedef MultiField<Link<GraphicsWindow> > MFGraphicsWindow;

      /**
       * Wrapper for ::osgViewer::GraphicsWindow
       * which internally creates a ::osgViewer::GraphicsWindow{X11,Win32,...}
       *
       * Not derived from av::osg::Object because the internal GraphicsWindow
       * is destroyed and recreated on specific field changes.
       *
       * \ingroup av_osg_viewer
       */
      class AV_OSG_VIEWER_DLL GraphicsWindow : public av::Object
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. A new ::osgViewer::GraphicsWindow is created only internally.
         */
        GraphicsWindow();

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
        virtual ~GraphicsWindow();

      public:

        /**
         * The OpenGL context will be shared with the specified window,
         * which allows to shared textures and more between multiple windows.
         */
        SFGraphicsWindow SharedContextMaster;

        /**
         * Screen identifier in the form "hostName:displayNum.screenNum".
         */
        SFString ScreenIdentifier;

        SFString Title;
        SFBool Decoration;
        SFBool QuadBufferStereo;
        SFBool DoubleBuffer;
        SFBool ShowCursor;

        /**
         * Number of wanted stencil buffer bits.
         */
        SFInt NumStencilBits;

        /**
         * Input wanted window position and size.
         * For fullscreen, set Decoration to false, position to (0, 0) and
         * connect WantedWidth and WantedHeight from ScreenWidth and ScreenHeight
         */
        SFInt WantedPositionX;
        SFInt WantedPositionY;
        SFUInt WantedWidth;
        SFUInt WantedHeight;

        /**
         * Outputs actual window position (-1, if window is invalid).
         */
        SFInt ActualPositionX;
        SFInt ActualPositionY;

        /**
         * Outputs actual window size (0, if window is invalid).
         */
        SFUInt ActualWidth;
        SFUInt ActualHeight;

        /**
         * Outputs actual screen resolution (0, if screen is invalid).
         */
        SFUInt ScreenWidth;
        SFUInt ScreenHeight;

        /**
         * Input real world screen size.
         */
        SFDouble RealScreenWidth;
        SFDouble RealScreenHeight;

        /**
         * Defines if RealScreenHeight is automatically adjusted
         * based on RealScreenWidth and aspect ratio from screen resolution
         * (assumes quadratic pixels, doesn't work e.g. with 1280x1024 on a 4:3 display).
         * Enabled by default.
         */
        SFBool AutoHeight;

        /**
         * Window coordinates of mouse pointer relative to upper left corner.
         */
        SFVec2 MousePosition;

        /**
         * Normalized window coordinates of mouse pointer, range [-1, 1], [-1, 1].
         */
        SFVec2 MousePositionNorm;

        /**
         * Mouse pointer movement since last evaluate in window coordinates.
         */
        SFVec2 MouseMovement;

        /**
         * Mouse pointer movement since last evaluate in normalized window coordinates.
         */
        SFVec2 MouseMovementNorm;

        /**
         * Reset mouse pointer to window center in every frame.
         * Used by movers using mouse movement instead of position.
         * Addititonally hides the mouse cursor.
         */
        SFBool MouseFixed;

        /**
         * Input toggle full screen mode.
         */
        SFBool ToggleFullScreen;

        /**
         * Input drag event from event handler
         */
        SFVec2 DragEvent;

        /**
         * Input move event from event handler
         */
        SFVec2 MoveEvent;

        /* virtual */ void fieldHasChangedLocalSideEffect(const av::Field& field);
        /* virtual */ void evaluateLocalSideEffect();
        /* virtual */ void evaluate();

        /**
         * Get the wrapped ::osgViewer::GraphicsWindow.
         */
        ::osgViewer::GraphicsWindow* getOsgGraphicsWindow() const;

        /**
         * Get the real world window size.
         */
        ::osg::Vec2 getRealWindowSize();

        typedef boost::signal<void ()> ChangedSignal;
        typedef ChangedSignal::slot_type ChangedCallback;
        typedef boost::signals::connection CallbackHandle;

        /**
         * add a callback which is called each time when the wrapped
         * ::osgViewer::GraphicsWindow is created or deleted
         */
        CallbackHandle addWindowChangedCallback(const ChangedCallback& callback);

        /**
         * add a callback which is called each time when the wrapped
         * ::osgViewer::GraphicsWindow is moved or resized
         */
        CallbackHandle addSizeChangedCallback(const ChangedCallback& callback);

        /**
         * remove the callback
         */
        static void removeCallback(const CallbackHandle& handle);

      private:

        ::osg::ref_ptr< ::osgViewer::GraphicsWindow> mOsgGraphicsWindow;
        ::osg::ref_ptr< ::osgViewer::GraphicsWindow> mOsgSharedContextMaster;

        std::string mScreenIdentifier;
        bool mStereo, mDoubleBuffer, mShowCursor, mFullScreen;
        bool mSizeFieldsHasChanged;
        int mNumStencilBits;
        bool mDragEvent;
        bool mMoveEvent;
        ::osg::Vec2 mLastMousePos;
        std::set<int> mKeys;

        ChangedSignal mWindowChangedSignal, mSizeChangedSignal;
      };

    } // namespace viewer

  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_VIEWER_DLL SingleField<Link<osg::viewer::GraphicsWindow> >;
  template class AV_OSG_VIEWER_DLL MultiField<Link<osg::viewer::GraphicsWindow> >;
#endif

} // namespace av

#endif
