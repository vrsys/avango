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

#include <avango/osg/viewer/GraphicsWindow.h>

#include <avango/Logger.h>

#include <osg/DeleteHandler>

#if !defined(_WIN32)
#include <osgViewer/api/X11/GraphicsWindowX11>
#include <osgViewer/api/X11/PixelBufferX11>

#ifdef OSGVIEWER_USE_XRANDR
#include <X11/extensions/Xrandr.h>
#endif

#endif // !_WIN32

#if !defined(_WIN32)
extern "C"
{
  typedef int (*X11ErrorHandler)(Display*, XErrorEvent*);

  int X11ErrorHandling(Display* display, XErrorEvent* event)
  {
    osg::notify(osg::NOTICE) << "Got an X11ErrorHandling call display = " << display
                             << " event = " << event << std::endl;

    char buffer[256];
    XGetErrorText(display, event->error_code, buffer, 256);

    osg::notify(osg::NOTICE) << buffer << std::endl;
    osg::notify(osg::NOTICE) << "Major opcode: " << (int)event->request_code << std::endl;
    osg::notify(osg::NOTICE) << "Minor opcode: " << (int)event->minor_code << std::endl;
    osg::notify(osg::NOTICE) << "Error code: " << (int)event->error_code << std::endl;
    osg::notify(osg::NOTICE) << "Request serial: " << event->serial << std::endl;
    osg::notify(osg::NOTICE) << "Current serial: " << NextRequest( display ) - 1 << std::endl;

    switch(event->error_code)
    {
      case BadValue:
        osg::notify(osg::NOTICE) << "  Value: " << event->resourceid << std::endl;
        break;

      case BadAtom:
        osg::notify(osg::NOTICE) << "  AtomID: " << event->resourceid << std::endl;
        break;

      default:
        osg::notify(osg::NOTICE) << "  ResourceID: " << event->resourceid << std::endl;
        break;
    }

    return 0;
  }
}
#endif // !_WIN32

namespace
{
  av::Logger& logger(av::getLogger("av::osg::viewer::GraphicsWindow"));

#if !defined(_WIN32)
  // We need to check the window events in the avango evaluate instead of the
  // OSG event traversal, so we create and register a new window class, which
  // does not check the window events.
  class GraphicsWindowX11NoEvents : public osgViewer::GraphicsWindowX11
  {
  public:
    GraphicsWindowX11NoEvents(osg::GraphicsContext::Traits* traits):
      GraphicsWindowX11(traits)
    {}

    /* virtual */ void checkEvents() {}
  };

  class X11WindowingSystemInterface : public osg::GraphicsContext::WindowingSystemInterface
  {
#ifdef OSGVIEWER_USE_XRANDR

    bool _setScreen(const osg::GraphicsContext::ScreenIdentifier& si,
                    unsigned int width, unsigned height, double rate)
    {
      Display* display = XOpenDisplay(si.displayName().c_str());
      if (display)
      {
        XRRScreenConfiguration* sc = XRRGetScreenInfo(display, RootWindow(display, si.screenNum));

        if (!sc)
        {
          osg::notify(osg::NOTICE) << "Unable to create XRRScreenConfiguration on display \""
                                   << XDisplayName(si.displayName().c_str()) << "\"." << std::endl;
          return false;
        }

        int      numScreens = 0;
        int      numRates   = 0;
        Rotation currentRot = 0;
        bool     okay       = false;

        XRRConfigRotations(sc, &currentRot);

        // If the width or height are zero, use our defaults.
        if (!width || !height)
        {
          getScreenResolution(si, width, height);
        }

        // If this somehow fails, okay will still be false, no iteration will take place below,
        // and the sc pointer will still be freed later on.
        XRRScreenSize* ss = XRRConfigSizes(sc, &numScreens);

        for (int i = 0; i < numScreens; i++)
        {
          if (ss[i].width == static_cast<int>(width) && ss[i].height == static_cast<int>(height))
          {
            short* rates     = XRRConfigRates(sc, i, &numRates);
            bool   rateFound = false;

            // Search for our rate in the list of acceptable rates given to us by Xrandr.
            // If it's not found, rateFound will still be false and the call will never
            // be made to XRRSetScreenConfigAndRate since the rate will be invalid.
            for (int r = 0; r < numRates; r++)
            {
              if (rates[r] == static_cast<short>(rate))
              {
                rateFound = true;
                break;
              }
            }

            if (rate > 0.0f && !rateFound)
            {
              osg::notify(osg::NOTICE) << "Unable to find valid refresh rate " << rate
                                       << " on display \"" << XDisplayName(si.displayName().c_str())
                                       << "\"." << std::endl;
            }
            else if (XRRSetScreenConfigAndRate(display, sc, DefaultRootWindow(display), i,
                     currentRot, static_cast<short>(rate), CurrentTime) != RRSetConfigSuccess)
            {
              osg::notify(osg::NOTICE) << "Unable to set resolution to " << width << "x" << height
                                       << " on display \"" << XDisplayName(si.displayName().c_str())
                                       << "\"." << std::endl;
            }
            else
            {
              okay = true;
              break;
            }
          }
        }

        XRRFreeScreenConfigInfo(sc);

        return okay;
      }
      else
      {
        osg::notify(osg::NOTICE) << "Unable to open display \""
                                 << XDisplayName(si.displayName().c_str()) << "\"." << std::endl;
        return false;
      }
    }

#endif // OSGVIEWER_USE_XRANDR

  protected:

    bool _errorHandlerSet;

  public:

    X11WindowingSystemInterface()
    {
      osg::notify(osg::INFO) << "X11WindowingSystemInterface()" << std::endl;

      // Install an X11 error handler, if the application has not already done so.

      // Set default handler, and get pointer to current handler.
      X11ErrorHandler currentHandler = XSetErrorHandler(0);

      // Set our handler, and get pointer to default handler.
      X11ErrorHandler defHandler = XSetErrorHandler(X11ErrorHandling);

      if (currentHandler == defHandler)
      {
        // No application error handler, use ours.
        _errorHandlerSet = true;
      }
      else
      {
        // Application error handler exists, leave it set.
        _errorHandlerSet = false;
        XSetErrorHandler(currentHandler);
      }
    }

    ~X11WindowingSystemInterface()
    {
      if (osg::Referenced::getDeleteHandler())
      {
        osg::Referenced::getDeleteHandler()->setNumFramesToRetainObjects(0);
        osg::Referenced::getDeleteHandler()->flushAll();
      }

      // Unset our X11 error handler, providing the application has not replaced it.

      if (_errorHandlerSet)
      {
        X11ErrorHandler currentHandler = XSetErrorHandler(0);
        if (currentHandler != X11ErrorHandling)
        {
          // Not our error handler, leave it set.
          XSetErrorHandler(currentHandler);
        }
      }
    }

    virtual unsigned int getNumScreens(const osg::GraphicsContext::ScreenIdentifier& si)
    {
      Display* display = XOpenDisplay(si.displayName().c_str());
      if (display)
      {
        unsigned int numScreens = ScreenCount(display);
        XCloseDisplay(display);

        return numScreens;
      }
      else
      {
        osg::notify(osg::NOTICE) << "A Unable to open display \""
                                 << XDisplayName(si.displayName().c_str()) << "\"" << std::endl;
        return 0;
      }
    }

    virtual void getScreenResolution(const osg::GraphicsContext::ScreenIdentifier& si,
                                     unsigned int& width, unsigned int& height)
    {
      Display* display = XOpenDisplay(si.displayName().c_str());
      if (display)
      {
        width = DisplayWidth(display, si.screenNum);
        height = DisplayHeight(display, si.screenNum);
        XCloseDisplay(display);
      }
      else
      {
        osg::notify(osg::NOTICE) << "Unable to open display \""
                                 << XDisplayName(si.displayName().c_str()) << "\"." << std::endl;
        width = 0;
        height = 0;
      }
    }

    virtual bool setScreenResolution(const osg::GraphicsContext::ScreenIdentifier& si,
                                     unsigned int width, unsigned int height)
    {
#ifdef OSGVIEWER_USE_XRANDR
      return _setScreen(si, width, height, 0.0f);
#else
      osg::notify(osg::NOTICE) << "You must build osgViewer with Xrandr 1.2 or higher for setScreenResolution support!" << std::endl;
      return false;
#endif
    }

    virtual bool setScreenRefreshRate(const osg::GraphicsContext::ScreenIdentifier& si, double rate)
    {
#ifdef OSGVIEWER_USE_XRANDR
      return _setScreen(si, 0, 0, rate);
#else
      osg::notify(osg::NOTICE) << "You must build osgViewer with Xrandr 1.2 or higher for setScreenRefreshRate support!" << std::endl;
      return false;
#endif
    }

    virtual osg::GraphicsContext* createGraphicsContext(osg::GraphicsContext::Traits* traits)
    {
      if (traits->pbuffer)
      {
        osg::ref_ptr<osgViewer::PixelBufferX11> pbuffer = new osgViewer::PixelBufferX11(traits);
        if (pbuffer->valid())
          return pbuffer.release();
        else
          return 0;
      }
      else
      {
        osg::ref_ptr<osgViewer::GraphicsWindowX11> window = new GraphicsWindowX11NoEvents(traits);
        if (window->valid())
          return window.release();
        else
          return 0;
      }
    }
  };

  struct RegisterWindowingSystemInterfaceProxy
  {
    RegisterWindowingSystemInterfaceProxy()
    {
      ::osg::notify(::osg::INFO) << "RegisterWindowingSystemInterfaceProxy()" << std::endl;
      ::osg::GraphicsContext::setWindowingSystemInterface(new X11WindowingSystemInterface);
    }

    ~RegisterWindowingSystemInterfaceProxy()
    {
      ::osg::notify(::osg::INFO) << "~RegisterWindowingSystemInterfaceProxy()" << std::endl;

      if (::osg::Referenced::getDeleteHandler())
      {
        ::osg::Referenced::getDeleteHandler()->setNumFramesToRetainObjects(0);
        ::osg::Referenced::getDeleteHandler()->flushAll();
      }

      ::osg::GraphicsContext::setWindowingSystemInterface(0);
    }
  };

  RegisterWindowingSystemInterfaceProxy createWindowingSystemInterfaceProxy;

#endif // !_WIN32

} // namespace



AV_FC_DEFINE(av::osg::viewer::GraphicsWindow);

AV_FIELD_DEFINE(av::osg::viewer::SFGraphicsWindow);
AV_FIELD_DEFINE(av::osg::viewer::MFGraphicsWindow);

av::osg::viewer::GraphicsWindow::GraphicsWindow() :
  mStereo(false), mDoubleBuffer(false), mShowCursor(false), mFullScreen(false),
  mSizeFieldsHasChanged(false),
  mNumStencilBits(0),
  mLastMousePos(0.0, 0.0)
{
  AV_FC_ADD_FIELD(SharedContextMaster, 0);
  AV_FC_ADD_FIELD(ScreenIdentifier, (::getenv("DISPLAY") != 0 ? ::getenv("DISPLAY") : ":0.0"));
  AV_FC_ADD_FIELD(Title, "Avango NG - " + ScreenIdentifier.getValue());

  AV_FC_ADD_FIELD(Decoration, true);
  AV_FC_ADD_FIELD(QuadBufferStereo, false);
  AV_FC_ADD_FIELD(DoubleBuffer, true);
  AV_FC_ADD_FIELD(ShowCursor, true);
  AV_FC_ADD_FIELD(NumStencilBits, 0);

  AV_FC_ADD_FIELD(WantedPositionX, 100);
  AV_FC_ADD_FIELD(WantedPositionY, 100);
  AV_FC_ADD_FIELD(WantedWidth, 500);
  AV_FC_ADD_FIELD(WantedHeight, 500);

  AV_FC_ADD_FIELD(ActualPositionX, -1);
  AV_FC_ADD_FIELD(ActualPositionY, -1);
  AV_FC_ADD_FIELD(ActualWidth, 0u);
  AV_FC_ADD_FIELD(ActualHeight, 0u);

  AV_FC_ADD_FIELD(ScreenWidth, 0u);
  AV_FC_ADD_FIELD(ScreenHeight, 0u);
  AV_FC_ADD_FIELD(RealScreenWidth, 0.6);
  AV_FC_ADD_FIELD(RealScreenHeight, 0.45);
  AV_FC_ADD_FIELD(AutoHeight, true);

  AV_FC_ADD_FIELD(MousePosition, ::osg::Vec2(0.0, 0.0));
  AV_FC_ADD_FIELD(MousePositionNorm, ::osg::Vec2(0.0, 0.0));
  AV_FC_ADD_FIELD(MouseMovement, ::osg::Vec2(0.0, 0.0));
  AV_FC_ADD_FIELD(MouseMovementNorm, ::osg::Vec2(0.0, 0.0));
  AV_FC_ADD_FIELD(MouseFixed, false);
  AV_FC_ADD_FIELD(MouseButtonLeft, false);
  AV_FC_ADD_FIELD(MouseButtonMiddle, false);
  AV_FC_ADD_FIELD(MouseButtonRight, false);
  AV_FC_ADD_FIELD(MouseButtons_OnlyLeft, false);
  AV_FC_ADD_FIELD(MouseButtons_OnlyMiddle, false);
  AV_FC_ADD_FIELD(MouseButtons_OnlyRight, false);
  AV_FC_ADD_FIELD(MouseButtons_LeftAndMiddle, false);
  AV_FC_ADD_FIELD(MouseButtons_LeftAndRight, false);
  AV_FC_ADD_FIELD(MouseButtons_MiddleAndRight, false);
  AV_FC_ADD_FIELD(MouseButtons_LeftAndMiddleAndRight, false);
  AV_FC_ADD_FIELD(MouseButtonLeftDoubleClick, false);
  AV_FC_ADD_FIELD(MouseButtonMiddleDoubleClick, false);
  AV_FC_ADD_FIELD(MouseButtonRightDoubleClick, false);
  AV_FC_ADD_FIELD(MouseScrollUp, false);
  AV_FC_ADD_FIELD(MouseScrollDown, false);

  AV_FC_ADD_FIELD(KeysPressed, MFInt::ContainerType());
  AV_FC_ADD_FIELD(KeyShift, false);
  AV_FC_ADD_FIELD(KeyCtrl, false);
  AV_FC_ADD_FIELD(KeyAlt, false);
  AV_FC_ADD_FIELD(KeyInsert, false);
  AV_FC_ADD_FIELD(KeyDelete, false);
  AV_FC_ADD_FIELD(KeyHome, false);
  AV_FC_ADD_FIELD(KeyEnd, false);
  AV_FC_ADD_FIELD(KeyPageUp, false);
  AV_FC_ADD_FIELD(KeyPageDown, false);
  AV_FC_ADD_FIELD(KeyLeft, false);
  AV_FC_ADD_FIELD(KeyRight, false);
  AV_FC_ADD_FIELD(KeyUp, false);
  AV_FC_ADD_FIELD(KeyDown, false);
  AV_FC_ADD_FIELD(KeyEsc, false);
  AV_FC_ADD_FIELD(KeySpace, false);
  AV_FC_ADD_FIELD(KeyEnter, false);

  alwaysEvaluate(true);
}

/* virtual */
av::osg::viewer::GraphicsWindow::~GraphicsWindow()
{}

/* static */ void
av::osg::viewer::GraphicsWindow::initClass()
{
  if (!isTypeInitialized())
  {
    av::Object::initClass();

    AV_FC_INIT(av::Object, av::osg::viewer::GraphicsWindow, true);

    SFGraphicsWindow::initClass("av::osg::viewer::SFGraphicsWindow", "av::Field");
    MFGraphicsWindow::initClass("av::osg::viewer::MFGraphicsWindow", "av::Field");
  }
}

/* virtual */ void
av::osg::viewer::GraphicsWindow::fieldHasChangedLocalSideEffect(const av::Field& field)
{
  if (&field == &WantedPositionX ||
      &field == &WantedPositionY ||
      &field == &WantedWidth ||
      &field == &WantedHeight ||
      &field == &Decoration) // windows decoration affects the window position
  {
    mSizeFieldsHasChanged = true;
  }
}

/* virtual */ void
av::osg::viewer::GraphicsWindow::evaluateLocalSideEffect()
{
  logger.trace() << "evaluateLocalSideEffect()";

  // delete window, if settings have changed which can't be updated dynamically
  if (mOsgGraphicsWindow.valid() &&
      (mScreenIdentifier != ScreenIdentifier.getValue() ||
       mStereo != QuadBufferStereo.getValue() ||
       mDoubleBuffer != DoubleBuffer.getValue() ||
       mNumStencilBits != NumStencilBits.getValue() ||
       mOsgSharedContextMaster.valid() != SharedContextMaster.getValue().isValid() ||
       mOsgSharedContextMaster.valid() &&
       mOsgSharedContextMaster != SharedContextMaster.getValue()->getOsgGraphicsWindow()))
  {
    mOsgGraphicsWindow = 0;
    mOsgSharedContextMaster = 0;
    mWindowChangedSignal();
  }

  const bool showCursor = (ShowCursor.getValue() && !MouseFixed.getValue());

  // create window
  if (!mOsgGraphicsWindow.valid() && ::osg::GraphicsContext::getWindowingSystemInterface() != 0)
  {
    ::osg::ref_ptr< ::osg::GraphicsContext::Traits> traits = new ::osg::GraphicsContext::Traits;

    if (SharedContextMaster.getValue().isValid())
    {
      evaluateDependency(*SharedContextMaster.getValue());
      traits->sharedContext = SharedContextMaster.getValue()->getOsgGraphicsWindow();
    }

    traits->setScreenIdentifier(ScreenIdentifier.getValue());
    traits->windowName = Title.getValue();
    traits->windowDecoration = Decoration.getValue();
    traits->quadBufferStereo = QuadBufferStereo.getValue();
    traits->doubleBuffer = DoubleBuffer.getValue();
    traits->useCursor = showCursor;
    traits->stencil = NumStencilBits.getValue();

    if (traits->displayNum == -1)
    {
      traits->displayNum = 0;
      logger.warn() << "evaluateLocalSideEffect(): invalid display identifier, using display 0";
    }
    if (traits->screenNum == -1)
    {
      traits->screenNum = 0;
      logger.warn() << "evaluateLocalSideEffect(): invalid screen identifier, using screen 0";
    }

    // update screen resolution
    unsigned int screenWidth = 0u, screenHeight = 0u;
    ::osg::GraphicsContext::getWindowingSystemInterface()->
      getScreenResolution(*traits, screenWidth, screenHeight);

    if (ScreenWidth.getValue() != screenWidth)
      ScreenWidth.setValue(screenWidth);
    if (ScreenHeight.getValue() != screenHeight)
      ScreenHeight.setValue(screenHeight);

    // get wanted values after screen resolution update, because they may be connected
    traits->x = WantedPositionX.getValue();
    traits->y = WantedPositionY.getValue();
    traits->width = WantedWidth.getValue();
    traits->height = WantedHeight.getValue();

    mOsgGraphicsWindow = dynamic_cast< ::osgViewer::GraphicsWindow*>
      (::osg::GraphicsContext::createGraphicsContext(traits.get()));

    if (mOsgGraphicsWindow.valid())
    {
      if (mOsgGraphicsWindow->realize())
      {
        if (SharedContextMaster.getValue().isValid())
          mOsgSharedContextMaster = SharedContextMaster.getValue()->getOsgGraphicsWindow();

        mScreenIdentifier = ScreenIdentifier.getValue();
        mStereo = QuadBufferStereo.getValue();
        mDoubleBuffer = DoubleBuffer.getValue();
        mShowCursor = showCursor;
        mNumStencilBits = NumStencilBits.getValue();
        mWindowChangedSignal();
      }
      else
      {
        mOsgGraphicsWindow = 0;
        mOsgSharedContextMaster = 0;
        logger.warn() << "evaluateLocalSideEffect(): couldn't create window";
      }
    }
  }

  // update window
  if (mOsgGraphicsWindow.valid())
  {
    if (mSizeFieldsHasChanged)
    {
      if (mFullScreen)
      {
        mOsgGraphicsWindow->setWindowDecoration(false);
        mOsgGraphicsWindow->setWindowRectangle(0, 0,
                                               ScreenWidth.getValue(),
                                               ScreenHeight.getValue());
      }
      else
      {
        mOsgGraphicsWindow->setWindowDecoration(Decoration.getValue());
        mOsgGraphicsWindow->setWindowRectangle(WantedPositionX.getValue(),
                                               WantedPositionY.getValue(),
                                               WantedWidth.getValue(),
                                               WantedHeight.getValue());
      }

      mSizeFieldsHasChanged = false;
    }

    if (AutoHeight.getValue())
    {
      const double height =
        RealScreenWidth.getValue() * ScreenHeight.getValue() / ScreenWidth.getValue();
      if (RealScreenHeight.getValue() != height)
        RealScreenHeight.setValue(height);
    }

    if (mOsgGraphicsWindow->getWindowName() != Title.getValue())
      mOsgGraphicsWindow->setWindowName(Title.getValue());

    if (mShowCursor != showCursor)
    {
      mOsgGraphicsWindow->useCursor(showCursor);
      mShowCursor = showCursor;
    }

#if !defined(_WIN32)
    dynamic_cast< ::osgViewer::GraphicsWindowX11*>(mOsgGraphicsWindow.get())->
      ::osgViewer::GraphicsWindowX11::checkEvents();
#endif
  }
}

/* virtual */ void
av::osg::viewer::GraphicsWindow::evaluate()
{
  logger.trace() << "evaluate()";

  int posX = -1, posY = -1, width = 0, height = 0;
  ::osg::Vec2 mouseMove(0.0, 0.0), mouseMoveNorm(0.0, 0.0);

  if (MouseButtonLeftDoubleClick.getValue())
    MouseButtonLeftDoubleClick.setValue(false);
  if (MouseButtonMiddleDoubleClick.getValue())
    MouseButtonMiddleDoubleClick.setValue(false);
  if (MouseButtonRightDoubleClick.getValue())
    MouseButtonRightDoubleClick.setValue(false);
  if (MouseScrollUp.getValue())
    MouseScrollUp.setValue(false);
  if (MouseScrollDown.getValue())
    MouseScrollDown.setValue(false);

  if (mOsgGraphicsWindow.valid())
  {
    mOsgGraphicsWindow->getWindowRectangle(posX, posY, width, height);
    ::osgGA::EventQueue::Events events;
    mOsgGraphicsWindow->getEventQueue()->copyEvents(events);

    if (MouseFixed.getValue())
    {
      mOsgGraphicsWindow->requestWarpPointer(width / 2, height / 2);
      const ::osg::Vec2 mousePos(width / 2, height / 2);
      if (MousePosition.getValue() != mousePos)
        MousePosition.setValue(mousePos);
      if (MousePositionNorm.getValue() != ::osg::Vec2(0.0, 0.0))
        MousePositionNorm.setValue(::osg::Vec2(0.0, 0.0));
    }

    ::osgGA::EventQueue::Events::iterator eventIter;
    for(eventIter = events.begin(); eventIter != events.end(); ++eventIter)
    {
      ::osgGA::GUIEventAdapter *event = eventIter->get();

      switch(event->getEventType())
      {
        case(::osgGA::GUIEventAdapter::DRAG):
        case(::osgGA::GUIEventAdapter::MOVE):
        {
          if (!MouseFixed.getValue() || static_cast<int>(event->getX()) != width / 2 ||
                                        static_cast<int>(event->getY()) != height / 2)
          {
            const ::osg::Vec2 mousePos(event->getX(), event->getY());

            if (!MouseFixed.getValue())
            {
              if (MousePosition.getValue() != mousePos)
                MousePosition.setValue(mousePos);

              ::osg::Vec2 mousePosNorm(0.0, 0.0);
              if (width > 1 && height > 1)
              {
                mousePosNorm.set(-1.0 + 2.0 * mousePos[0] / (width - 1.0),
                                  1.0 - 2.0 * mousePos[1] / (height - 1.0));
              }

              if (MousePositionNorm.getValue() != mousePosNorm)
                MousePositionNorm.setValue(mousePosNorm);
            }

            mouseMove = mousePos - mLastMousePos;
            if (mouseMove.length2() < 0.1)
              mouseMove.set(0.0, 0.0);
            if (width > 1 && height > 1)
            {
              mouseMoveNorm.set( 2.0 * mouseMove[0] / (width - 1.0),
                                -2.0 * mouseMove[1] / (height - 1.0));
            }
            if (mouseMove.length2() < 0.1)
              mouseMoveNorm.set(0.0, 0.0);
          }

          break;
        }

        case(::osgGA::GUIEventAdapter::PUSH):
        case(::osgGA::GUIEventAdapter::RELEASE):
        {
          const unsigned int buttonmask = event->getButtonMask();
          const bool left   = (buttonmask & ::osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) != 0u;
          const bool middle = (buttonmask & ::osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON) != 0u;
          const bool right  = (buttonmask & ::osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) != 0u;

          if (MouseButtonLeft.getValue() != left)
            MouseButtonLeft.setValue(left);
          if (MouseButtonMiddle.getValue() != middle)
            MouseButtonMiddle.setValue(middle);
          if (MouseButtonRight.getValue() != right)
            MouseButtonRight.setValue(right);

          if (MouseButtons_OnlyLeft.getValue() != (left && !middle && !right))
            MouseButtons_OnlyLeft.setValue(left && !middle && !right);
          if (MouseButtons_OnlyMiddle.getValue() != (!left && middle && !right))
            MouseButtons_OnlyMiddle.setValue(!left && middle && !right);
          if (MouseButtons_OnlyRight.getValue() != (!left && !middle && right))
            MouseButtons_OnlyRight.setValue(!left && !middle && right);
          if (MouseButtons_LeftAndMiddle.getValue() != (left && middle && !right))
            MouseButtons_LeftAndMiddle.setValue(left && middle && !right);
          if (MouseButtons_LeftAndRight.getValue() != (left && !middle && right))
            MouseButtons_LeftAndRight.setValue(left && !middle && right);
          if (MouseButtons_MiddleAndRight.getValue() != (!left && middle && right))
            MouseButtons_MiddleAndRight.setValue(!left && middle && right);
          if (MouseButtons_LeftAndMiddleAndRight.getValue() != (left && middle && right))
            MouseButtons_LeftAndMiddleAndRight.setValue(left && middle && right);

          break;
        }

        case(::osgGA::GUIEventAdapter::DOUBLECLICK):
        {
          switch (event->getButton())
          {
            case ::osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
            {
              MouseButtonLeftDoubleClick.setValue(true);
              break;
            }

            case ::osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON:
            {
              MouseButtonMiddleDoubleClick.setValue(true);
              break;
            }

            case ::osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
            {
              MouseButtonRightDoubleClick.setValue(true);
              break;
            }

            default:
              break;
          }

          break;
        }

        case(::osgGA::GUIEventAdapter::SCROLL):
        {
          switch (event->getScrollingMotion())
          {
            case ::osgGA::GUIEventAdapter::SCROLL_UP:
            {
              MouseScrollUp.setValue(true);
              break;
            }

            case ::osgGA::GUIEventAdapter::SCROLL_DOWN:
            {
              MouseScrollDown.setValue(true);
              break;
            }

            default:
              break;
          }

          break;
        }

        case(::osgGA::GUIEventAdapter::KEYDOWN):
        case(::osgGA::GUIEventAdapter::KEYUP):
        {
          const bool pressed = (event->getEventType() == ::osgGA::GUIEventAdapter::KEYDOWN);
          const int key = event->getKey();

          const bool shiftPressed =
            (event->getModKeyMask() & (::osgGA::GUIEventAdapter::MODKEY_LEFT_SHIFT |
                                       ::osgGA::GUIEventAdapter::MODKEY_RIGHT_SHIFT)) != 0u;
          const bool ctrlPressed =
            (event->getModKeyMask() & (::osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL |
                                       ::osgGA::GUIEventAdapter::MODKEY_RIGHT_CTRL)) != 0u;
          const bool altPressed =
            (event->getModKeyMask() & (::osgGA::GUIEventAdapter::MODKEY_LEFT_ALT |
                                       ::osgGA::GUIEventAdapter::MODKEY_RIGHT_ALT)) != 0u;

          if (KeyShift.getValue() != shiftPressed)
            KeyShift.setValue(shiftPressed);
          if (KeyCtrl.getValue() != ctrlPressed)
            KeyCtrl.setValue(ctrlPressed);
          if (KeyAlt.getValue() != altPressed)
            KeyAlt.setValue(altPressed);

          if (pressed)
            mKeys.insert(key);
          else
            mKeys.erase(key);
          std::vector<int> keys(mKeys.size());
          std::copy(mKeys.begin(), mKeys.end(), keys.begin());
          KeysPressed.setValue(keys);

          switch (key)
          {
            case ::osgGA::GUIEventAdapter::KEY_Insert:    KeyInsert.setValue(pressed);   break;
            case ::osgGA::GUIEventAdapter::KEY_Delete:    KeyDelete.setValue(pressed);   break;
            case ::osgGA::GUIEventAdapter::KEY_Home:      KeyHome.setValue(pressed);     break;
            case ::osgGA::GUIEventAdapter::KEY_End:       KeyEnd.setValue(pressed);      break;
            case ::osgGA::GUIEventAdapter::KEY_Page_Up:   KeyPageUp.setValue(pressed);   break;
            case ::osgGA::GUIEventAdapter::KEY_Page_Down: KeyPageDown.setValue(pressed); break;
            case ::osgGA::GUIEventAdapter::KEY_Left:      KeyLeft.setValue(pressed);     break;
            case ::osgGA::GUIEventAdapter::KEY_Right:     KeyRight.setValue(pressed);    break;
            case ::osgGA::GUIEventAdapter::KEY_Up:        KeyUp.setValue(pressed);       break;
            case ::osgGA::GUIEventAdapter::KEY_Down:      KeyDown.setValue(pressed);     break;
            case ::osgGA::GUIEventAdapter::KEY_Escape:    KeyEsc.setValue(pressed);      break;
            case ::osgGA::GUIEventAdapter::KEY_Space:     KeySpace.setValue(pressed);    break;
            case ::osgGA::GUIEventAdapter::KEY_Return:    KeyEnter.setValue(pressed);    break;
          }

          if (altPressed && key == ::osgGA::GUIEventAdapter::KEY_Return && pressed)
          {
            mFullScreen = !mFullScreen;
            mSizeFieldsHasChanged = true;
          }

          break;
        }

        default:
          break;
      }
    }

    mLastMousePos = MousePosition.getValue();
  }

  if (MouseMovement.getValue() != mouseMove)
    MouseMovement.setValue(mouseMove);
  if (MouseMovementNorm.getValue() != mouseMoveNorm)
    MouseMovementNorm.setValue(mouseMoveNorm);

  // update output fields
  if (ActualPositionX.getValue() != posX ||
      ActualPositionY.getValue() != posY ||
      ActualWidth.getValue() != static_cast<unsigned int>(width) ||
      ActualHeight.getValue() != static_cast<unsigned int>(height))
  {
    if (ActualPositionX.getValue() != posX)
      ActualPositionX.setValue(posX);
    if (ActualPositionY.getValue() != posY)
      ActualPositionY.setValue(posY);
    if (ActualWidth.getValue() != static_cast<unsigned int>(width))
      ActualWidth.setValue(static_cast<unsigned int>(width));
    if (ActualHeight.getValue() != static_cast<unsigned int>(height))
      ActualHeight.setValue(static_cast<unsigned int>(height));
    mSizeChangedSignal();
  }
}

::osgViewer::GraphicsWindow*
av::osg::viewer::GraphicsWindow::getOsgGraphicsWindow() const
{
  return mOsgGraphicsWindow.get();
}

::osg::Vec2
av::osg::viewer::GraphicsWindow::getRealWindowSize()
{
  ::osg::Vec2 size(0.0, 0.0);

  if (ScreenWidth.getValue() != 0u && ScreenHeight.getValue() != 0u)
  {
    size[0] = RealScreenWidth.getValue() * ActualWidth.getValue() / ScreenWidth.getValue();
    size[1] = RealScreenHeight.getValue() * ActualHeight.getValue() / ScreenHeight.getValue();
  }

  return size;
}

av::osg::viewer::GraphicsWindow::CallbackHandle
av::osg::viewer::GraphicsWindow::addWindowChangedCallback(const ChangedCallback& callback)
{
  return mWindowChangedSignal.connect(callback);
}

av::osg::viewer::GraphicsWindow::CallbackHandle
av::osg::viewer::GraphicsWindow::addSizeChangedCallback(const ChangedCallback& callback)
{
  return mSizeChangedSignal.connect(callback);
}

/* static */ void
av::osg::viewer::GraphicsWindow::removeCallback(const CallbackHandle& handle)
{
  handle.disconnect();
}
