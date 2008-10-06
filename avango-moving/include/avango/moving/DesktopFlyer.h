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

// $Id$

#if !defined(AVANGO_MOVING_DESKTOPFLYER_H)
#define AVANGO_MOVING_DESKTOPFLYER_H

/**
 * \file
 * \ingroup av_moving
 */
#include <avango/osg/Object.h>
#include <avango/osg/Fields.h>
#include <avango/osg/MatrixTransform.h>
//#include <avango/TimeSensor.h>
#include "windows_specific_moving.h"

namespace av
{
  namespace moving
  {
    /**
     * DesktopFlyer mover
     *
     * \ingroup av_moving
     */
    class AV_MOVING_DLL DesktopFlyer : public av::osg::MatrixTransform
    {
      AV_FC_DECLARE();

    public:

      /**
       * Constructor.
       */
      DesktopFlyer();

    protected:

      /**
       * Destructor made protected to prevent allocation on stack.
       */
      virtual ~DesktopFlyer();

    public:

      /**
       * Input Fields to connect from nomalized mouse axes fields of a GraphicsWindow
       */
      av::osg::SFVec2 MouseIn;

      /**
       * Trigger for rotating and panning.
       * Typically connected from one of the mouse button fields of a GraphicsWindow.
       */
      SFBool RotateXYTrigger;
      SFBool MoveYRotateZTrigger;
      SFBool MoveZRotateZTrigger;
      SFBool MoveXZTrigger;
      SFBool MoveXYTrigger;

      /**
       * Input Fields to be typically connected from keyboard fields of a GraphicsWindow
       */
      SFBool MoveForward;
      SFBool MoveBackward;
      SFBool MoveLeft;
      SFBool MoveRight;
      SFBool MoveUp;
      SFBool MoveDown;
      SFBool RotateUp;
      SFBool RotateDown;
      SFBool RotateLeft;
      SFBool RotateRight;
      SFBool RotateCW;
      SFBool RotateCCW;

      /**
       * Time Input field to allow frame independent speeds, connect with Time Sensor Time
       */
      SFDouble TimeIn;

      /**
       * Output Trigger to be connected to the FixedMouse field of a Graphics Window
       * deactivates mouse pointer while mouse interaction to generate relative input
       */
      SFBool RelativeMouseTrigger;

      /**
       * controls if halt movement on release, should be switched off with acceleration control
       * default true
       */
      SFBool HaltOnClutch;

      /**
       * change speed behaviour of DesktopFlyer:
       * MoveSpeed default is 27.78 m/s = 50 km/h, MoveAcceleration default is 0
       * MoveBreak only breaks when Acceleration > 0, default is 5 m/s^2
       * default MaxSpeed is 55.56 m/s 200 Km/H, default RotationSpeed is 3.14/s = 180deg/s
       *
       */
      SFFloat MoveSpeed;
      SFFloat MoveAcceleration;
      SFFloat MoveBreak;
      SFFloat MaxSpeed;
      SFFloat RotationSpeed;

      /**
       * Invert Y Rotation Axis, default is false
       */
      SFBool InvertYMouseAxis;

      /* virtual */ void fieldHasChanged(const av::Field& field);
      /* virtual */ void evaluate();

    private:

      float mOldSpeedX;
      float mOldSpeedY;
      float mOldSpeedZ;

      double mCurrentTime;
      double mOldTime;
      double mFrameTime;

      float mRotX;
      float mRotY;
      float mRotZ;

      float mMoveX;
      float mMoveY;
      float mMoveZ;

      int mModeChangeCounter;

    };

    typedef SingleField<Link<DesktopFlyer> > SFDesktopFlyer;
    typedef MultiField<Link<DesktopFlyer> > MFDesktopFlyer;
  }

#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_MOVING_DLL SingleField<Link<moving::DesktopFlyer> >;
  template class AV_MOVING_DLL MultiField<Link<moving::DesktopFlyer> >;
#endif
}

#endif

// $Id$
