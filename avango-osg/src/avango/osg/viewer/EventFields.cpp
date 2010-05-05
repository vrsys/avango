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

#include <iostream>
#include <avango/Logger.h>
#include <osgGA/GUIEventHandler>
#include <avango/osg/viewer/EventFields.h>

namespace
{
  av::Logger& logger(av::getLogger("av::osg::viewer::EventFields"));
} // namespace

av::osg::viewer::InputEventHandler::InputEventHandler(EventFields& ev) :
 mEventFields(&ev)
{

}

/* virtual */ bool 
av::osg::viewer::InputEventHandler::handle(const ::osgGA::GUIEventAdapter& event, ::osgGA::GUIActionAdapter& action)
{
  logger.trace() << "handle()";

  if (mEventFields->MouseButtonLeftDoubleClick.getValue())
    mEventFields->MouseButtonLeftDoubleClick.setValue(false);
  if (mEventFields->MouseButtonMiddleDoubleClick.getValue())
    mEventFields->MouseButtonMiddleDoubleClick.setValue(false);
  if (mEventFields->MouseButtonRightDoubleClick.getValue())
    mEventFields->MouseButtonRightDoubleClick.setValue(false);
  if (mEventFields->MouseScrollUp.getValue())
    mEventFields->MouseScrollUp.setValue(false);
  if (mEventFields->MouseScrollDown.getValue())
    mEventFields->MouseScrollDown.setValue(false);

  switch(event.getEventType())
  {
    case(::osgGA::GUIEventAdapter::DRAG):
    {
      const ::osg::Vec2 mousePos(event.getX(), event.getY());
      mEventFields->DragEvent.setValue(mousePos);

      break;
    }

    case(::osgGA::GUIEventAdapter::MOVE):
    {
      const ::osg::Vec2 mousePos(event.getX(), event.getY());
      mEventFields->MoveEvent.setValue(mousePos);

      break;
    }

    case(::osgGA::GUIEventAdapter::PUSH):
    case(::osgGA::GUIEventAdapter::RELEASE):
    {
      const unsigned int buttonmask = event.getButtonMask();
      const bool left   = (buttonmask & ::osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) != 0u;
      const bool middle = (buttonmask & ::osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON) != 0u;
      const bool right  = (buttonmask & ::osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) != 0u;

      if (mEventFields->MouseButtonLeft.getValue() != left)
        mEventFields->MouseButtonLeft.setValue(left);
      if (mEventFields->MouseButtonMiddle.getValue() != middle)
        mEventFields->MouseButtonMiddle.setValue(middle);
      if (mEventFields->MouseButtonRight.getValue() != right)
        mEventFields->MouseButtonRight.setValue(right);

      if (mEventFields->MouseButtons_OnlyLeft.getValue() != (left && !middle && !right))
        mEventFields->MouseButtons_OnlyLeft.setValue(left && !middle && !right);
      if (mEventFields->MouseButtons_OnlyMiddle.getValue() != (!left && middle && !right))
        mEventFields->MouseButtons_OnlyMiddle.setValue(!left && middle && !right);
      if (mEventFields->MouseButtons_OnlyRight.getValue() != (!left && !middle && right))
        mEventFields->MouseButtons_OnlyRight.setValue(!left && !middle && right);
      if (mEventFields->MouseButtons_LeftAndMiddle.getValue() != (left && middle && !right))
        mEventFields->MouseButtons_LeftAndMiddle.setValue(left && middle && !right);
      if (mEventFields->MouseButtons_LeftAndRight.getValue() != (left && !middle && right))
        mEventFields->MouseButtons_LeftAndRight.setValue(left && !middle && right);
      if (mEventFields->MouseButtons_MiddleAndRight.getValue() != (!left && middle && right))
        mEventFields->MouseButtons_MiddleAndRight.setValue(!left && middle && right);
      if (mEventFields->MouseButtons_LeftAndMiddleAndRight.getValue() != (left && middle && right))
        mEventFields->MouseButtons_LeftAndMiddleAndRight.setValue(left && middle && right);

      break;
    }

    case(::osgGA::GUIEventAdapter::DOUBLECLICK):
    {
      switch (event.getButton())
      {
        case ::osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
        {
          mEventFields->MouseButtonLeftDoubleClick.setValue(true);
          break;
        }

        case ::osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON:
        {
          mEventFields->MouseButtonMiddleDoubleClick.setValue(true);
          break;
        }

        case ::osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
        {
          mEventFields->MouseButtonRightDoubleClick.setValue(true);
          break;
        }

        default:
          break;
      }

      break;
    }

    case(::osgGA::GUIEventAdapter::SCROLL):
    {
      switch (event.getScrollingMotion())
      {
        case ::osgGA::GUIEventAdapter::SCROLL_UP:
        {
          mEventFields->MouseScrollUp.setValue(true);
          break;
        }

        case ::osgGA::GUIEventAdapter::SCROLL_DOWN:
        {
          mEventFields->MouseScrollDown.setValue(true);
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
      const bool pressed = (event.getEventType() == ::osgGA::GUIEventAdapter::KEYDOWN);
      const int key = event.getKey();

      const bool shiftPressed =
        (event.getModKeyMask() & (::osgGA::GUIEventAdapter::MODKEY_LEFT_SHIFT |
                                  ::osgGA::GUIEventAdapter::MODKEY_RIGHT_SHIFT)) != 0u;
      const bool ctrlPressed =
        (event.getModKeyMask() & (::osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL |
                                  ::osgGA::GUIEventAdapter::MODKEY_RIGHT_CTRL)) != 0u;
      const bool altPressed =
        (event.getModKeyMask() & (::osgGA::GUIEventAdapter::MODKEY_LEFT_ALT |
                                  ::osgGA::GUIEventAdapter::MODKEY_RIGHT_ALT)) != 0u;

      if (mEventFields->KeyShift.getValue() != shiftPressed)
        mEventFields->KeyShift.setValue(shiftPressed);
      if (mEventFields->KeyCtrl.getValue() != ctrlPressed)
        mEventFields->KeyCtrl.setValue(ctrlPressed);
      if (mEventFields->KeyAlt.getValue() != altPressed)
        mEventFields->KeyAlt.setValue(altPressed);

      if (pressed)
        mKeys.insert(key);
      else
        mKeys.erase(key);

      std::vector<int> keys(mKeys.size());
      std::copy(mKeys.begin(), mKeys.end(), keys.begin());
      mEventFields->KeysPressed.setValue(keys);

      switch (key)
      {
        case ::osgGA::GUIEventAdapter::KEY_Insert:    mEventFields->KeyInsert.setValue(pressed);   break;
        case ::osgGA::GUIEventAdapter::KEY_Delete:    mEventFields->KeyDelete.setValue(pressed);   break;
        case ::osgGA::GUIEventAdapter::KEY_Home:      mEventFields->KeyHome.setValue(pressed);     break;
        case ::osgGA::GUIEventAdapter::KEY_End:       mEventFields->KeyEnd.setValue(pressed);      break;
        case ::osgGA::GUIEventAdapter::KEY_Page_Up:   mEventFields->KeyPageUp.setValue(pressed);   break;
        case ::osgGA::GUIEventAdapter::KEY_Page_Down: mEventFields->KeyPageDown.setValue(pressed); break;
        case ::osgGA::GUIEventAdapter::KEY_Left:      mEventFields->KeyLeft.setValue(pressed);     break;
        case ::osgGA::GUIEventAdapter::KEY_Right:     mEventFields->KeyRight.setValue(pressed);    break;
        case ::osgGA::GUIEventAdapter::KEY_Up:        mEventFields->KeyUp.setValue(pressed);       break;
        case ::osgGA::GUIEventAdapter::KEY_Down:      mEventFields->KeyDown.setValue(pressed);     break;
        case ::osgGA::GUIEventAdapter::KEY_Escape:    mEventFields->KeyEsc.setValue(pressed);      break;
        case ::osgGA::GUIEventAdapter::KEY_Space:     mEventFields->KeySpace.setValue(pressed);    break;
        case ::osgGA::GUIEventAdapter::KEY_Return:    mEventFields->KeyEnter.setValue(pressed);    break;
        case ::osgGA::GUIEventAdapter::KEY_F1:        mEventFields->KeyF1.setValue(pressed);       break;
        case ::osgGA::GUIEventAdapter::KEY_F2:        mEventFields->KeyF2.setValue(pressed);       break;
        case ::osgGA::GUIEventAdapter::KEY_F3:        mEventFields->KeyF3.setValue(pressed);       break;
        case ::osgGA::GUIEventAdapter::KEY_F4:        mEventFields->KeyF4.setValue(pressed);       break;
        case ::osgGA::GUIEventAdapter::KEY_F5:        mEventFields->KeyF5.setValue(pressed);       break;
        case ::osgGA::GUIEventAdapter::KEY_F6:        mEventFields->KeyF6.setValue(pressed);       break;
        case ::osgGA::GUIEventAdapter::KEY_F7:        mEventFields->KeyF7.setValue(pressed);       break;
        case ::osgGA::GUIEventAdapter::KEY_F8:        mEventFields->KeyF8.setValue(pressed);       break;
        case ::osgGA::GUIEventAdapter::KEY_F9:        mEventFields->KeyF9.setValue(pressed);       break;
        case ::osgGA::GUIEventAdapter::KEY_F10:       mEventFields->KeyF10.setValue(pressed);      break;
        case ::osgGA::GUIEventAdapter::KEY_F11:       mEventFields->KeyF11.setValue(pressed);      break;
        case ::osgGA::GUIEventAdapter::KEY_F12:       mEventFields->KeyF12.setValue(pressed);      break;

      }

      if (altPressed && key == ::osgGA::GUIEventAdapter::KEY_Return && pressed)
      {
        mEventFields->KeyAltReturn.setValue(true);
      }

      break;
    }

    default:
      break;
  }

  return false;
}


AV_FC_DEFINE(av::osg::viewer::EventFields);

AV_FIELD_DEFINE(av::osg::viewer::SFEventFields);
AV_FIELD_DEFINE(av::osg::viewer::MFEventFields);

av::osg::viewer::EventFields::EventFields() :
  mView(0)
{
  AV_FC_ADD_FIELD(View, 0);
  AV_FC_ADD_FIELD(DragEvent, ::osg::Vec2(0.0, 0.0));
  AV_FC_ADD_FIELD(MoveEvent, ::osg::Vec2(0.0, 0.0));
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
  AV_FC_ADD_FIELD(KeyAltReturn, false);
  AV_FC_ADD_FIELD(KeyF1, false);
  AV_FC_ADD_FIELD(KeyF2, false);
  AV_FC_ADD_FIELD(KeyF3, false);
  AV_FC_ADD_FIELD(KeyF4, false);
  AV_FC_ADD_FIELD(KeyF5, false);
  AV_FC_ADD_FIELD(KeyF6, false);
  AV_FC_ADD_FIELD(KeyF7, false);
  AV_FC_ADD_FIELD(KeyF8, false);
  AV_FC_ADD_FIELD(KeyF9, false);
  AV_FC_ADD_FIELD(KeyF10, false);
  AV_FC_ADD_FIELD(KeyF11, false);
  AV_FC_ADD_FIELD(KeyF12, false);
}

/* virtual */
av::osg::viewer::EventFields::~EventFields()
{}

/* static */ void
av::osg::viewer::EventFields::initClass()
{
  if (!isTypeInitialized())
  {
    av::FieldContainer::initClass();

    AV_FC_INIT(av::FieldContainer, av::osg::viewer::EventFields, true);

    SFEventFields::initClass("av::osg::viewer::SFEventFields", "av::Field");
    MFEventFields::initClass("av::osg::viewer::MFEventFields", "av::Field");
  }
}

/* virtual */ void
av::osg::viewer::EventFields::fieldHasChanged(const av::Field& field)
{
  if ((&field == &View) && View.getValue().isValid())
  {
    if (mView.valid())
    {
      logger.error() << "changing view not supported.";
    }
    else
    {
      mView = View.getValue().getPtr()->getOsgView();
      InputEventHandler* ih = new av::osg::viewer::InputEventHandler(*this);
      mView->addEventHandler(ih);
    }
  }
}

