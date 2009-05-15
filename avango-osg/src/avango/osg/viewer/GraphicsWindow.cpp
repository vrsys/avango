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

#include <osg/Version>
#include <osg/DeleteHandler>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::viewer::GraphicsWindow"));
} // namespace



AV_FC_DEFINE(av::osg::viewer::GraphicsWindow);

AV_FIELD_DEFINE(av::osg::viewer::SFGraphicsWindow);
AV_FIELD_DEFINE(av::osg::viewer::MFGraphicsWindow);

av::osg::viewer::GraphicsWindow::GraphicsWindow() :
  mStereo(false), mDoubleBuffer(false), mShowCursor(false), mFullScreen(false),
  mSizeFieldsHasChanged(false),
  mNumStencilBits(0),
  mDragEvent(false),
  mMoveEvent(false),
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
  AV_FC_ADD_FIELD(DragEvent, ::osg::Vec2(0.0, 0.0));
  AV_FC_ADD_FIELD(MoveEvent, ::osg::Vec2(0.0, 0.0));

  AV_FC_ADD_FIELD(ToggleFullScreen, false);

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
  else if ((&field == &ToggleFullScreen) && ToggleFullScreen.getValue())
  {
    mFullScreen = !mFullScreen;
    mSizeFieldsHasChanged = true;
  }
  else if (&field == &DragEvent && (DragEvent.getValue() != ::osg::Vec2(0.0, 0.0)))
  {
    mDragEvent = true;
  }
  else if (&field == &MoveEvent && (MoveEvent.getValue() != ::osg::Vec2(0.0, 0.0)))
  {
    mMoveEvent = true;
  }
}

/* virtual */ void
av::osg::viewer::GraphicsWindow::evaluateLocalSideEffect()
{
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
      AVANGO_LOG(logger, av::logging::WARN, "evaluateLocalSideEffect(): invalid display identifier, using display 0");
    }
    if (traits->screenNum == -1)
    {
      traits->screenNum = 0;
      AVANGO_LOG(logger, av::logging::WARN, "evaluateLocalSideEffect(): invalid screen identifier, using screen 0");
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
        AVANGO_LOG(logger, av::logging::WARN, "evaluateLocalSideEffect(): couldn't create window");
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
  }
}

/* virtual */ void
av::osg::viewer::GraphicsWindow::evaluate()
{
  int posX = -1, posY = -1, width = 0, height = 0;
  ::osg::Vec2 mouseMove(0.0, 0.0), mouseMoveNorm(0.0, 0.0);

  if (mOsgGraphicsWindow.valid())
  {
    mOsgGraphicsWindow->getWindowRectangle(posX, posY, width, height);

    if (MouseFixed.getValue())
    {
      mOsgGraphicsWindow->requestWarpPointer(width / 2, height / 2);
      const ::osg::Vec2 mousePos(width / 2, height / 2);
      if (MousePosition.getValue() != mousePos)
        MousePosition.setValue(mousePos);
      if (MousePositionNorm.getValue() != ::osg::Vec2(0.0, 0.0))
        MousePositionNorm.setValue(::osg::Vec2(0.0, 0.0));
    }

    if (mDragEvent || mMoveEvent)
    {
      ::osg::Vec2 eventPos(0.0, 0.0);

      /*
      Events have set Y_INCREASING_UPWARDS. Should be Y_INCREASING_DOWNWARDS.
      Events processed with ::osgGA::GUIEventHandler::handle(const ::osgGA::GUIEventAdapter&, ::osgGA::GUIActionAdapter&)
      seem not to be aware of their graphics context...
      */
      if (mDragEvent)
      {
        eventPos = DragEvent.getValue();
        eventPos[1] = height-eventPos[1];
      }
      else
      {
        eventPos = MoveEvent.getValue();
        eventPos[1] = height-eventPos[1];
      }

      if (!MouseFixed.getValue() || (eventPos[0] != width / 2) || (eventPos[1] != height / 2))
      {
        ::osg::Vec2 mousePos(eventPos[0], eventPos[1]);

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

      mDragEvent = false;
      mMoveEvent = false;
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
