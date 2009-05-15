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

#if !defined(AVANGO_OSG_VIEWER_FIELDEVENTS_H)
#define AVANGO_OSG_VIEWER_FIELDEVENTS_H

/**
 * \file
 * \ingroup av_osg_viewer
 */

#include <osgGA/GUIEventHandler>

#include <avango/Object.h>
#include <avango/osg/Fields.h>
#include <avango/osg/viewer/View.h>
#include <avango/osg/viewer/windows_specific_osg_viewer.h>

namespace av
{
  namespace osg
  {
    namespace viewer
    {
      class EventFields;

      class InputEventHandler : public ::osgGA::GUIEventHandler 
      {
      public:
        InputEventHandler(EventFields&);

        /** handle events, return true if handled, false otherwise.*/
        /* virtual */ bool handle(const ::osgGA::GUIEventAdapter&, ::osgGA::GUIActionAdapter&);

      protected:
        EventFields *mEventFields;
        std::set<int> mKeys;
      };

      typedef SingleField<Link<EventFields> > SFEventFields;
      typedef MultiField<Link<EventFields> > MFEventFields;

      /**
       * \ingroup av_osg_viewer
       */
      class AV_OSG_VIEWER_DLL EventFields : public av::Object
      {
        AV_FC_DECLARE();

      public:

        /**
         * Constructor. A new ::osgViewer::EventFields is created only internally.
         */
        EventFields();

      protected:

        /**
         * Destructor made protected to prevent allocation on stack.
         */
        virtual ~EventFields();

      public:

        /**
         * Input: View to register the InputEventHandler.
         */
        SFView View;

        /**
         * Output: drag event.
         */
        SFVec2 DragEvent;

        /**
         * Output: move event.
         */
        SFVec2 MoveEvent;

        /**
         * Output: Mouse buttons.
         */
        SFBool MouseButtonLeft;
        SFBool MouseButtonMiddle;
        SFBool MouseButtonRight;

        /**
         * Output: Combinations of mouse buttons.
         */
        SFBool MouseButtons_OnlyLeft;
        SFBool MouseButtons_OnlyMiddle;
        SFBool MouseButtons_OnlyRight;
        SFBool MouseButtons_LeftAndMiddle;
        SFBool MouseButtons_LeftAndRight;
        SFBool MouseButtons_MiddleAndRight;
        SFBool MouseButtons_LeftAndMiddleAndRight;

        /**
         * Output: Mouse buttons double click trigger fields.
         * Only implemented for Windows in OSG!
         */
        SFBool MouseButtonLeftDoubleClick;
        SFBool MouseButtonMiddleDoubleClick;
        SFBool MouseButtonRightDoubleClick;

        /**
         * Output: Mouse scroll wheel trigger fields.
         */
        SFBool MouseScrollUp;
        SFBool MouseScrollDown;

        /**
         * Output: Keys.
         */
        MFInt KeysPressed;
        SFBool KeyShift;
        SFBool KeyCtrl;
        SFBool KeyAlt;
        SFBool KeyInsert;
        SFBool KeyDelete;
        SFBool KeyHome;
        SFBool KeyEnd;
        SFBool KeyPageUp;
        SFBool KeyPageDown;
        SFBool KeyLeft;
        SFBool KeyRight;
        SFBool KeyUp;
        SFBool KeyDown;
        SFBool KeyEsc;
        SFBool KeySpace;
        SFBool KeyEnter;
        SFBool KeyAltReturn;

        /* virtual */ void fieldHasChanged(const av::Field& field);

      protected:
        ::osg::ref_ptr< ::osgViewer::View> mView;

      };

    } // namespace viewer

  } // namespace osg

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OSG_VIEWER_DLL SingleField<Link<osg::viewer::EventFields> >;
  template class AV_OSG_VIEWER_DLL MultiField<Link<osg::viewer::EventFields> >;
#endif

} // namespace av

#endif
